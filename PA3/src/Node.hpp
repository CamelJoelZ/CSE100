#ifndef NODE_HPP
#define NODE_HPP
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//#include "Edge.hpp"

#define MAX 100000000
using namespace std;

class Edge;
//template <typename Data>
class Node {
   public:
    /** Constructor.  Initialize a Node with the given Data item,
     */
    Node(const string &d){ actor = d;}
    string actor = "";
    string prevMovie = "";
    vector <Edge*> edges;
    int dist = MAX;
    Node * prev = NULL;
    bool done = false;
    int commonNeighbor = 0;
    

};


#endif  // BSTNODE_HPP
