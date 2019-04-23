#ifndef BST_HPP
#define BST_HPP
#include <iostream>
#include "BSTIterator.hpp"
#include "BSTNode.hpp"
using namespace std;

template <typename Data>
class BST {
   protected:
    /** Pointer to the root of this BST, or 0 if the BST is empty */
    BSTNode<Data> *root;

    /** Number of Data items stored in this BST. */
    unsigned int isize;

    /** Height of this BST. */
    unsigned int iheight;

   public:
    /** define iterator as an aliased typename for BSTIterator<Data>. */
    typedef BSTIterator<Data> iterator;

    /** Default constructor.
        Initialize an empty BST.
     */
    BST() : root(0), isize(0), iheight(0) {}

    /** Default destructor.
        Delete every node in this BST.
     */
    // TODOi
    virtual ~BST() {
	deleteAll(root);
    }

    /** Given a reference to a Data item, insert a copy of it in this BST.
     *  Return  true if the item was added to this BST
     *  as a result of this call to insert,
     *  false if an item equal to this one was already in this BST.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=)  For the reasoning
     *  behind this, see the assignment writeup.
     */
    // TODOi
    virtual bool insert(const Data &item) {
	//element already exists
	if(find(item) != end()){
	  return false;
	}
	
	// empty BST
	if(root == 0){
	  root = new BSTNode<Data>(item);
	  isize++;
	  return true;
	}
	
	// pre to store the parent of cur 
    	BSTNode<Data> *cur = root;
    	BSTNode<Data> *pre = root;
	int flag = 0;
	// if item smaller than cur
	while(cur != 0){
	  // item should go to right subtree
	  if(cur->data < item){
	    pre = cur;
	    cur = cur->right;
	    flag = 0;
	  }
          // left subtree
	  else{
	    pre = cur;
	    cur = cur->left;    
            flag = 1;
	  }
	}	
	// create a new node after reaching the leaf
	cur = new BSTNode<Data>(item);
	// reassign member vars
	if(flag == 1) pre->left = cur;
	else pre->right = cur;
	cur->parent = pre;
	isize++;
	return true;	
    }

    /** Find a Data item in the BST.
     *  Return an iterator pointing to the item, or pointing past
     *  the last node in the BST if not found.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=).  For the reasoning
     *  behind this, see the assignment writeup.
     */
    // TODOi
    virtual iterator find(const Data &item) const {
    	BSTNode<Data> *cur = root;
	while(cur != NULL){
	  // left subtree
	  if(item < cur->data)
	    cur = cur->left;
	  else if(cur->data < item)
	    cur = cur->right;
	  else
	    return BSTIterator<Data>(cur);
	}
        return end();
    }

    /** Return the number of items currently in the BST.
     */
    // TODOi
    unsigned int size() const {
	return isize;
    }

    /** Return the height of the BST.
     */
    unsigned int height() const {
    	return findHeight(this->root);
    }

    /** Return true if the BST is empty, else false.
     */
    // TODOi
    bool empty() const {
	return (this->root == 0);
    }

    /** Return an iterator pointing to the first item in the BST (not the root).
     */
    // TODOi
    iterator begin() const {
	
    	BSTNode<Data> *cur = root;
	// empty BST 
	//if(cur == 0) return;
	// find the leftmost element
 	while(cur->left != 0){
	  cur = cur->left;
	}		
	// DO I NEED TO CREATE IT ON THE HEAP?????????????????
	return BSTIterator<Data>(cur);
    }

    /** Return an iterator pointing past the last item in the BST.
     */
    iterator end() const { return typename BST<Data>::iterator(0); }

    /** Perform an inorder traversal of this BST.
     */
    void inorder() const { inorder(root); }

   private:
    /** Recursive inorder traversal 'helper' function */

    /** Inorder traverse BST, print out the data of each node in ascending
       order. Implementing inorder and deleteAll base on the pseudo code is an
       easy way to get started.
     */
    // TODOi
    void inorder(BSTNode<Data> *n) const {
        /* Pseudo Code:
          if current node is null: return;
          recursively traverse left sub-tree
          print current node data
          recursively traverse right sub-tree
        */

	// if cur is null 
	if(n == 0) return; 
	
	// traverse the left sub-tree
	if(n->left != 0) inorder(n->left);
        //print current node data
        cout << n -> data << endl;
	// traverse the right sub-tree
	if(n->right != 0) inorder(n->right);
    }
    /** Inorder traverse BST, helper func to find the height 
     */
    unsigned int findHeight(BSTNode<Data>* root) const{
	if(root == NULL){
          return 0;
 	}
        else{
	  int lHeight = findHeight(root->left);
	  int rHeight = findHeight(root->right);
	  
          if(lHeight < rHeight)  return (++rHeight);
	  else return (++lHeight);
	}
    }
    /** Inorder traverse BST, helper func to find the  
     */
    iterator traverse(BSTNode<Data> * n, const Data &item) const {

	// if cur is null 
	if(n == 0) return end();		

	// if the passed in data is found in the curr node
	if(!(n->data < item) && !(item < n->data)) 
	  return BSTIterator<Data>(n);
	// if the passed in data is greater than the node
	else if(n->right != 0 && n->data < item)
	  return traverse(n->right,item);
	// if the passed in data is smaller than the node
	else if(n->left != 0 && item < n->data)
	  return traverse(n->left,item);
	// reach the end of the BST
	//else(n->left == 0 && n->right == 0)
	return end();
    }

    /** Find the first element of the BST
     */
    static BSTNode<Data> *first(BSTNode<Data> *curr) {
        if (curr == 0) return 0;
        while (curr->left != 0) curr = curr->left;
        return curr;
    }

    /** do a postorder traversal, deleting nodes
     */
    // TODOi
    static void deleteAll(BSTNode<Data> *n) {
        /* Pseudo Code:
          if current node is null: return;
          recursively delete left sub-tree
          recursively delete right sub-tree
          delete current node
        */
	
	// if cur is null 
	if(n == 0) return; 

	// delete the left sub-tree
	if(n->left != 0) deleteAll(n->left);
	// delete the right sub-tree
	if(n->right != 0) deleteAll(n->right);
	// delete the current node
	delete n;
    }
};

#endif  // BST_HPP
