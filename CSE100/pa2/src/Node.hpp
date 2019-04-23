#ifndef NODE_HPP
#define NODE_HPP
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

//template <typename Data>
class Node {
   public:
    /** Constructor.  Initialize a Node with the given Data item,
     *  no parent, and no children.
     */
    Node(const char &d) : data(d) { mid = left = right = parent = NULL;}
    string word = "";
    Node* left;
    Node* mid;
    Node* right;
    Node* parent;
    int isWord = 0;
    int freq = 0;
    const char data;  // the const Data in this node.

};


#endif  // BSTNODE_HPP
