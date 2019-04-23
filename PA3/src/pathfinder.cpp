#include "ActorGraph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue> 
#include <string>
#include <stack> 

#define MAX 100000000
#define RECORDSIZE 2
#define EVEN 2

using namespace std;

string search(ActorGraph ag,string actor1, string actor2);

int main(int argc, char *argv[]) {
  bool first = true;
  ActorGraph ag;
  // construct the graph using the input file
  string result; 
  string choice = string(argv[2]);
  
  ag.loadFromFile(argv[1],choice == "w",false);

  // open the third file and parse out each pair of actors
  ifstream infile(argv[3]);
  // write to the file 
  ofstream outfile(argv[4]);

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
      if(!getline(ss,next, '\t')) break;

      record.push_back(next);
    }
    
    if(record.size() != RECORDSIZE){
      continue;
    }
	
    // parse out two actor name 
    string actor1(record[0]);
    string actor2(record[1]);
    
    result = search(ag,actor1,actor2);
    
    // write in the file 
    if (outfile.is_open()){
      if(first == true){
        outfile << "(actor)--[movie#@year]-->(actor)--..." << "\n";
        first = false;
      }
      outfile << result << "\n";
    }
  }
  
  //if(!infile.eof()){
  //  cerr << "Failed to read" << in_filename << "!\n";
  //}
  infile.close();
  outfile.close();
  return 0;
}

/*
 * using priority queue to do the search
 */
string search(ActorGraph ag,string actor1, string actor2){
    priority_queue<pair<int,Node*>,std::vector<pair<int,Node*>>,
    std::greater<pair<int,Node*>>> pq;

  // reset the field to default set
  unordered_map<string,Node*>:: iterator itr;
  for(itr = ag.allActor.begin(); itr != ag.allActor.end(); itr++){
    itr->second->dist = MAX;
    itr->second->prev = NULL;
    itr->second->done = false;
    itr->second->prevMovie = "";
  }

  // perform the traversal
  Node * start = ag.allActor.find(actor1)->second;
  start->dist = 0;

  pq.push(make_pair(start->dist,start));

  while(!pq.empty()){
    Node * cur = pq.top().second;

    pq.pop();
    if(cur->done == false){
      cur->done = true;
      vector <Edge*> edgeList = cur->edges;
      for(unsigned int i = 0; i < edgeList.size(); i++){
	// total distance to neighbor to neighbor thorugh cur
        Edge * edge = edgeList.at(i);
        int totalDis = cur->dist + edge->weight;
	// go over each node on the other sied of edge
	Node * neighbor = edge->node1;
  	if(neighbor == cur) neighbor = edge->node2;
 	
	// push neighbor to the pq
	if(totalDis < neighbor->dist){
	  neighbor->prev = cur;
	  neighbor->prevMovie = edge->movie + "#@" + to_string(edge->year);
	  neighbor->dist = totalDis;
	  pq.push(make_pair(totalDis,neighbor));
	}	
      } 
    }
  }
  
  // the actor2 node already have a path back to actor1 thorugh prev
  stack <string> s;
  Node* now = ag.allActor.find(actor2)->second;
  while(now->prev != NULL){
    s.push(now->actor);
    s.push(now->prevMovie);
    now = now->prev;
  }
  s.push(now->actor);
  
  // formatting return 
  int x = 0;
  int size = s.size();
  string ret;
  while(x < size){
    if(x % EVEN == 0){
      ret+= "(";
      ret+= s.top();
      s.pop();
      ret+= ")";
      x++;
    }
    else{
      ret+= "--[";
      ret+= s.top();
      s.pop();
      ret+="]-->";
      x++;
    }
  }
  return ret;
}

