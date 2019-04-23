#include "ActorGraph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue> 
#include <string>
#include <stack> 
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> search(ActorGraph ag,string actor);

int main(int argc, char *argv[]) {
  
  ActorGraph ag;
  // construct the graph using the input file
  vector<string> result; 
  
  ag.loadFromFile(argv[1],false,false);

  // open the third file and parse out each pair of actors
  ifstream infile(argv[2]);
  // write to the file 
  ofstream futureOut(argv[4]);
  ofstream existingOut(argv[3]);
  futureOut << "Actor1,Actor2,Actor3,Actor4" << "\n";
  existingOut << "Actor1,Actor2,Actor3,Actor4" << "\n";

  bool have_header = false;

  while(infile){
    string s;
    // get the next line
    if (!getline(infile,s)) break;

    if(!have_header){
      have_header = true;
      continue;
    }
	
    istringstream ss(s);
    vector<string> record;

    while(ss){
      string next;

      // get the next sring before hiiting a tab and put it in the next
      if(!getline(ss,next, '\n')) break;

      record.push_back(next);
    }
    
    if(record.size() != 1){
      continue;
    }
	
    // parse out two actor name 
    string actor1(record[0]);
    
    result = search(ag,actor1);
    
    // write in the file 
    if (futureOut.is_open()){
      futureOut << result[1] << "\n";
    }
    
    if (existingOut.is_open()){
      existingOut << result[0] << "\n";
    }
  }
  
  //if(!infile.eof()){
  //  cerr << "Failed to read" << in_filename << "!\n";
  //}
 
  infile.close();
  futureOut.close();
  existingOut.close();
  return 0;
}

/*
 * go over each of the actor's two jump neighbors and find out 
 * the 4 neighbors who have the most number of common neighbors 
 * with the actor 
 */
vector<string> search(ActorGraph ag, string actor){
  // reset Nodes' field before searching 
  unordered_map<string,Node*>:: iterator itr;
  for(itr = ag.allActor.begin(); itr != ag.allActor.end(); itr++){
    itr->second->commonNeighbor = 0;
  }

  // node to begin 
  Node * actorNode = ag.allActor.find(actor) -> second;

  set<Node*> exploreSet;
  set<Node*> futureSet;
  set<Node*> visitedFirst;
  
  // go over all the edges of the actor to find all other actors collaborated
  for(unsigned int i = 0; i < actorNode->edges.size(); i++){

    // find the one jump neighbor
    Node * firstNeighbor = actorNode->edges[i]->node1;
    if(firstNeighbor == actorNode){ 
      firstNeighbor = actorNode->edges[i]->node2;
    }
    // check if this node has been visited as the first Neighbor
    if(visitedFirst.find(firstNeighbor) != visitedFirst.end()){
      continue;
    }
    visitedFirst.insert(firstNeighbor);
    exploreSet.insert(firstNeighbor);

    set<Node*> visitedSecond;
    // go over all neighbors of the 1st jump neighbor to find the 2nd-jump ones
    for(unsigned int j = 0; j < firstNeighbor->edges.size(); j++){
      Node * secondNeighbor = firstNeighbor->edges[j]->node1;	
      if(secondNeighbor == firstNeighbor || secondNeighbor == actorNode){ 
        secondNeighbor = firstNeighbor->edges[j]->node2;
	// the edge is the edge between actorNode and firstNeigh
	if(secondNeighbor == firstNeighbor || secondNeighbor == actorNode){
	  continue;
	}
      }
      if(visitedSecond.find(secondNeighbor) != visitedSecond.end()) continue;
 
      // reach secondNeighbor already, increment its commonNeighbor
      if(secondNeighbor != actorNode){
        secondNeighbor->commonNeighbor++;
	visitedSecond.insert(secondNeighbor);
	// secondNeighbor is valid only when it is connected with actorNode
        if(ag.pair.find(actorNode->actor+secondNeighbor->actor)!= ag.pair.end()
  	||ag.pair.find(secondNeighbor->actor+actorNode->actor)!= ag.pair.end())
        {
          exploreSet.insert(secondNeighbor);
        }
	else{
	  futureSet.insert(secondNeighbor);
	}
      }
    } 
    visitedSecond.clear();
  }
  visitedFirst.clear();

  // for existing result
  // iteration of every 2nd jump neighbors done
  // get the 4 most largest ones out using priority queue
  priority_queue<pair<int,Node*>> pq;
  set<Node*> :: iterator it;
  for(it = exploreSet.begin(); it != exploreSet.end(); it++){
    pq.push(make_pair((*it)->commonNeighbor,*it));
  }
  // sort the existing result alphabetically  
  int count = pq.top().second->commonNeighbor;  
  vector<string> result;
  vector<string> sortContainer;
  //for(unsigned int k = 0; k < pq.size(); k++){
  while(!pq.empty()){
    int temp = pq.top().second->commonNeighbor;  
        //  cout << pq.top().second->actor << endl;
        //  cout << pq.top().second->commonNeighbor << endl;
	// cout << "--------" << endl;
    if(count == temp){
      sortContainer.push_back(pq.top().second->actor);
      pq.pop();
      continue;
    }
    else{
      sort(sortContainer.begin(),sortContainer.end());
      result.insert(result.end(),sortContainer.begin(),sortContainer.end());
      sortContainer.clear();
      sortContainer.push_back(pq.top().second->actor);
      count = pq.top().second->commonNeighbor;
      pq.pop();
      if(result.size()>4) break;

      continue;
    }
  }
  if(!sortContainer.empty()){ 
    sort(sortContainer.begin(),sortContainer.end());
    result.insert(result.end(),sortContainer.begin(),sortContainer.end());
    sortContainer.clear();
  }
  
  // sort the future result alphabetically   
  pq = priority_queue <pair<int,Node*>> ();
  vector<string> resultFuture;
  for(it = futureSet.begin(); it != futureSet.end(); it++){
    pq.push(make_pair((*it)->commonNeighbor,*it));
  }
  if(!pq.empty()) count = pq.top().second->commonNeighbor;  
  //for(unsigned int k = 0; k < pq.size(); k++){
  while(!pq.empty()){
    int temp = pq.top().second->commonNeighbor;  
    if(count == temp){
      sortContainer.push_back(pq.top().second->actor);
      pq.pop();
      continue;
    }
    else{
      sort(sortContainer.begin(),sortContainer.end());
      resultFuture.insert(resultFuture.end(),sortContainer.begin(),sortContainer.end());
      sortContainer.clear();
      sortContainer.push_back(pq.top().second->actor);
      count = pq.top().second->commonNeighbor;
      pq.pop();
      if(resultFuture.size()>4) break;

      continue;
    }
  }
  if(!sortContainer.empty()){ 
    sort(sortContainer.begin(),sortContainer.end());
    resultFuture.insert(resultFuture.end(),sortContainer.begin(),sortContainer.end());
    sortContainer.clear();
  }
  
  vector<string> ret(2);
  for (unsigned int m = 0; m < 4; m++){
    if(m < result.size()){
      ret[0]+= result[m];
      ret[0]+= "\t";
    }
  }
  for (unsigned int m = 0; m < 4; m++){
    if(m < resultFuture.size()){
      ret[1]+= resultFuture[m];
      ret[1]+= "\t";
    }
  }
  return ret;
}
