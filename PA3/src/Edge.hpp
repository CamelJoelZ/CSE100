#ifndef EDGE_HPP
#define EDGE_HPP
#include <iomanip>
#include <iostream>
#include <string>
//#include "Node.hpp"

using namespace std;
class Node;
//template <typename Data>
class Edge {
   public:
    /** Constructor.  Initialize a Node with the given Data item,
     *  no parent, and no children.
     */
    Edge(const string &d, const int &y){ movie = d; year = y;}
    string movie = "";
    int year = 0;
    int weight = 1;
    Node * node1= NULL;
    Node * node2 = NULL;
};


#endif  // EDGE_HPP
