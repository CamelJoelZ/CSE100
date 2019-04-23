/*
 * ActorGraph.cpp
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in movie_casts.tsv.
 *
 * Feel free to modify any/all aspects as you wish.
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define OFFSET 2018

using namespace std;

ActorGraph::ActorGraph(void) {
}


bool ActorGraph::loadFromFile(const char *in_filename,
                              const bool use_weighted_edges,
				const bool buildFriends) {
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline(infile, s)) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss(s);
    vector<string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline(ss, next, '\t')) break;

      record.push_back(next);
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    // unique movie name 
    string uMovie = record[1] + record[2];
    // no same movie read before
    //if(allMovie.find(uMovie) == allMovie.end()){
    if(allMovie.find(uMovie) == allMovie.end()){
      //allMovie[uMovie] = vector<string> ();
      allMovie.insert(make_pair(uMovie,vector<string>()));
    }
    // link the actor name with movie name 
    allMovie.find(uMovie)->second.push_back(actor_name);

    // actor currently not exist
    if(allActor.find(actor_name) == allActor.end()){
      Node * cur = new Node(actor_name);
      allActor.insert(make_pair(actor_name,cur)); 
       
    }
	
    // connecting all the nodes who has same movie name 
    Node * temp = allActor.find(actor_name)->second;
    // for each pair linked by an edge, put them in pair set 
    if(buildFriends && friends.find(temp->actor) == friends.end()){
      friends.insert(make_pair(temp->actor,set<string>()));
    }
    // list of actor needs to connect to temp
    vector<string> toLink = allMovie.find(uMovie)->second;

    for(unsigned int i = 0; i < toLink.size(); i++){
      string actor = toLink.at(i);
      // TODO --------- here changed 
      if(actor == actor_name)  continue;

      Node* nodeToLink = allActor.find(actor)->second;
      Edge * newEdge = new Edge(movie_title,movie_year);
      newEdge -> node1 = temp;
      newEdge -> node2 = nodeToLink;
      if(buildFriends){
        friends.find(temp->actor)->second.insert(nodeToLink->actor);
        friends.find(nodeToLink->actor)->second.insert(temp->actor);
      }
      pair.insert(temp->actor + nodeToLink->actor);
      //pair.insert(nodeToLink->actor + temp->actor);

      // check if weighted 
      if(use_weighted_edges){
        newEdge -> weight = 1 + (OFFSET - movie_year);
      }

      temp->edges.push_back(newEdge);
      nodeToLink->edges.push_back(newEdge);
    }
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}
