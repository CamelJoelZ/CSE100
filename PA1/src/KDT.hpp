#ifndef KDT_HPP
#define KDT_HPP

#include <math.h>     // pow, abs
#include <algorithm>  // sort, max, min
#include <iostream>   // cout, cin, and cerr
#include <limits>     // numeric_limits<type>::max()
#include <vector>     // vector<typename>
#include "BST.hpp"

#define INDEX 2
#define DEFDISTANCE 10000000000

using namespace std;

class Point {
   private:
    double delta = 0.00005;

   public:
    
    /** x and y point coordinates */
    double x, y;

    /** Default constructor: point at (0, 0) */
    Point() : x(0.0), y(0.0) {}

    /** constructor that defines point location */
    Point(double x, double y) : x(x), y(y) {}

    /** Equals operator */
    bool operator==(const Point &other) const {
        return abs(x - other.x) < delta && abs(y - other.y) < delta;
    }

    /** Not-equals operator */
    bool operator!=(const Point &other) const {
        return abs(x - other.x) > delta || abs(y - other.y) > delta;
    }

    /** Less than operator */
    bool operator<(const Point &other) const {
        cerr << "This operator should not be called, you will need "
             << "check the x and y components each time" << endl;
        return false;
    }

    /** Return the squre of the Euclidean distance between two points p1 and p2
     */
    // TODOi
    static double squareDistance(const Point &p1, const Point &p2) {
	double sum = pow(p1.x-p2.x,INDEX) + pow(p1.y-p2.y,INDEX);
	return sum;
    }
};

std::ostream &operator<<(std::ostream &out, const Point &data) {
    out << "(" << data.x << ", " << data.y << ")";
    return out;
}

bool xLessThan(const Point &p1, const Point &p2) { return p1.x < p2.x; }

bool yLessThan(const Point &p1, const Point &p2) { return p1.y < p2.y; }

class KDT : public BST<Point> {
   public:
    /** Given a vector of Point references, insert a copy into the BST.
     *  Return the number of elements that the fully constructed tree contains
     *  Note. Because of the nature of the KDTree, a simple < operator
     *        will not be sufficient.  You will need to check the x and y
     *        coordinates individually.  Two functions have been provided
     *        for your convenience above, (xLessThan and yLessThan). You may
     *        find them useful for the sort function from #include<algorithm>.
     *  Note. Since KDT inherits from BST, you still have access to the
     *        root, isize, and iheight instance variables and should modify
     *        them accordingly.
     *  Note. We have also provided a private helper method for the build
     *        function called buildSubset defined below.
     */
    // TODO
    virtual unsigned int build(vector<Point> &items) {
	// check null
	if(items.size() == 0) return 0;
	

	// start to build
	int size = items.size();
	root = buildSubset(items,0,size,0,1);
	return isize;
    }

    /** Find the nearest neighbor to a given point
     *  Returns the point in the kd-tree nearest to the parameter item.
     *  Note. THIS IS DIFFERENT THAN HOW THE PREVIOUS FIND FUNCTION WORKS
     *  Note. Since KDT inherits from BST, you still have access to the
     *        root, isize, and iheight member variables.
     *  Note. The dimension ordering you used to build the tree must be used
     *        to search through it.
     *  Note. We have added a private helper method for your convenience called
     *        findNNHelper defined below.
     */
    // TODO
    virtual iterator findNearestNeighbor(const Point &item) const {
	double smallestDis = DEFDISTANCE;
	BSTNode<Point> * smallestNode = NULL;
	findNNHelper(root,item,&smallestDis,&smallestNode,0);	
	return BSTIterator<Point>(smallestNode);
    }

    /** For the kd-tree, the find method should not be used.  Use the function
     *  findNearestNeighbor instead.
     */
    virtual iterator find(const Point &item) const override {
        cerr << "The KD Tree find method should not be called" << endl;
        cerr << "Use findNearestNeighbor instead" << endl;
        return 0;
    }

    /** For the kd-tree, the insert method should not be used.  The tree is
     *  built all at once.  There should be no scenario where you add individual
     * points.
     */
    virtual bool insert(const Point &item) override {
        cerr << "The KD Tree insert method should not be called" << endl;
        return false;
    }
   
    /** 
     *   overloading function to return the height of KDT
     */
   unsigned int height() const {
	return findHeight(root);
   }

   private:
    /** Recursively add a subset of the array to the KD tree
     *  Parameters items: the list of points that are used to build the kd tree
     *             start: the starting index for this subsection of the tree
     *             end: the non-inclusive ending index for this subtree
     *             dimension: 0 if sorting along the x-axis, and 1 if sorting
     *                        along the y-axis
     *  PostConditions: the parent node should have its left and right children
     *                  set if there are any available points and recursive
     *                  calls should be made for the left and right subtree
     *                    (again, if there are any remaining points).
     *  NOTE: We gave you two methods: xLessThan and yLessThan.  We expect you
     *        that you will find these methods very helpful.  For your sake,
     *        please keep this in mind.
     */
    // TODO
    BSTNode<Point> *buildSubset(vector<Point> items, unsigned int start,
                                unsigned int end, unsigned int dimension,
                                unsigned int height) {
      // reach the leaf
      //if(end-1 == start){
      //isize++;
        //if(height > iheight) iheight = height;
	
	//return new BSTNode<Point>(items[start]);
     // }
 
      // iterator for start and end
      auto head = items.begin()+start;
      auto tail = items.begin()+end;
      // sort the vector according to x
      if(dimension == 0)
	std::sort(head,tail,xLessThan);
      // sort the vector according to y
      else
	std::sort(head,tail,yLessThan);


      // select(leftmost) median for index mid
      unsigned int midIndex = 0;
      if((start+end) % 2 == 0) midIndex = ((end+start)/2) - 1;
      else midIndex = ((end+start)/2);
	
      // find median according to dimension
      if(dimension == 0){
      	while(midIndex != 0 && items[midIndex].x == items[midIndex-1].x
		&& midIndex-1 >= start)
	  midIndex--;
      }
      if(dimension == 1){
      	while(midIndex != 0 && items[midIndex].y == items[midIndex-1].y
		&& midIndex-1 >= start)
	  midIndex--;
      }
      // create Node p holding median
      BSTNode<Point> *p = new BSTNode<Point>(items[midIndex]);  
      isize++;

      // set right and left field
      // TODO midINddex?????????????
      int nextDim = 0;
      if(dimension == 0) nextDim = 1;

      // recursive call left
      if(start < midIndex)
        p->left = buildSubset(items,start,midIndex,nextDim,height+1);
      // skip all duplicates 
      //while(midIndex < items.size()-1 && items[midIndex].y == items[midIndex+1].y
      //&& items[midIndex].x == items[midIndex+1].x) midIndex++;
      
      // recurse right
      if(end > midIndex+1)
        p->right = buildSubset(items,midIndex+1,end,nextDim,height+1);
      return p;
    }

    /** Find the node in the subtree with that is closest to the given point p
     *  Parameters: node the root of the subtree being searched
     *              queryPoint the point whose nearest neighbor should be found
     *              smallestSquareDistance the smallest distance value found in
     *                                     the tree so far which can be updated
     *              closestPoint the node corresponding to the current closest
     *                           node in the tree which can be udpated
     *              dimension flags whether the node parameter splits remaining
     *                        nodes in its subtree by the x or y dimension
     *                        (has the value of either 0 or 1)
     *  Return: a BSTNode object that represents the point in the subtree
     *          closest to the given point p (including the root node)
     */
    // TODO
    void findNNHelper(BSTNode<Point> *node, const Point &queryPoint,
                      double *smallestSquareDistance,
                      BSTNode<Point> **closestPoint,
                      unsigned int dimension) const {
	
	// reach the leaf
        if(node == NULL) return;	
	
	// flag to see which subtree done, 0 for left, 1 for right
	int flag = 0;
	// recursive call to next node if comparing x
	if(dimension == 0){
          // recurse left
	  if(queryPoint.x < node->data.x){    
	    flag = 0;
            findNNHelper(node->left,queryPoint,
	                 smallestSquareDistance,closestPoint,1); 
          }
          // recurse right 
	  else{
	    flag = 1;
            findNNHelper(node->right,queryPoint,
	                 smallestSquareDistance,closestPoint,1); 
	  }
	}

	// recursive call to the next node if comparing y
	else if(dimension == 1){
          // recurse left
	  if(queryPoint.y < node->data.y){    
	    flag = 0;
            findNNHelper(node->left,queryPoint,
	                 smallestSquareDistance,closestPoint,0); 
          }
          // recurse right 
	  else{
	    flag = 1;
            findNNHelper(node->right,queryPoint,
	                 smallestSquareDistance,closestPoint,0); 
	  }
	}

	//After recursion first subtree of node ends.
	//Compute squared distance two points
	double sqrDistance = Point :: squareDistance(queryPoint,node->data);
	//update SD and CP if necessary.
	if(sqrDistance < *smallestSquareDistance){ 
	  *smallestSquareDistance = sqrDistance;
	  *closestPoint = node;
	}

	// Decide whether or not to recurse into the other subtree of N 
	// based on comparing (Q.z-N.z)^2 and SD
	double disAnotherTree = 0;
	int nextDim = 0;
	
	// calculate if go to another subTree
	if(dimension == 0){
	  nextDim = 1; 
	  disAnotherTree = pow(node->data.x - queryPoint.x,INDEX);
 	}
	else{
	  nextDim = 0;
	  disAnotherTree = pow(node->data.y - queryPoint.y,INDEX);
	}
	
	// have gone left, now we go right 
	if(disAnotherTree < *smallestSquareDistance && flag == 0){
          findNNHelper(node->right,queryPoint,
                       smallestSquareDistance,closestPoint,nextDim); 
	}
	// have gone right, now we go left
	else if(disAnotherTree < *smallestSquareDistance && flag == 1){
          findNNHelper(node->left,queryPoint,
	               smallestSquareDistance,closestPoint,nextDim); 
	}
        return;	
    }
    /** Inorder traverse BST, helper func to find the height 
     */
    unsigned int findHeight(BSTNode<Point>* root) const{
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
};

#endif  // KDT_HPP
