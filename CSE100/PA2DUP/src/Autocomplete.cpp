/**
 *  CSE 100 PA2 C++ Autocomplete
*/
#ifndef AUTOCOMPLETE_CPP
#define AUTOCOMPLETE_CPP

#include <vector>
#include <string>
#include <utility>
#include <stack>
#include "Node.cpp"
#define NUM 10
using namespace std;


//Class Node<Data>;
/**
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie.
 *
 *  You may not use std::map in this implementation
 */
class Autocomplete
{
public:
  Node<Data>* root;
	
  /* 
  Create an Autocomplete object.
  This object should be trained on the corpus vector
  That is - the predictCompletions() function below should pull autocomplete
  suggestions from this vector
  This vector will likely contain duplicates.
  This duplication should be your gauge of frequencey.

  Input: corpus - the corpus of words to learn from.
  Assume preprocessing has been done for you on this! E.g.
  if one of the words is "dán't", assume that each of those characters
  should be included in your trie and don't modify that word any further
  */
  Autocomplete::Autocomplete(const vector<string> & corpus){

    // go over the whole vector
    for(int i = 0; i < corpus.size(); i++){
      insert(corpus[i]); 
    }

  };
	
  /*
   * to do a single insertion of a word into the TST
   */
  void Autocomplete:: insert(const string str){ 
    int index = 0;
    char ch = str.at(index);
    Node * cur = root;
    int strLen = str.size();
    
    // initialize the TST by inserting the first string
    if(root == NULL){
      for(int j = 0; j < strLen; j++){
        cur = new Node(str.at(j));
        cur = cur->mid;
      }
      return; 
    }

    while(index < strLen-1){
      // set node to the middle child and set letter to the next letter 
      if( ch == cur->val){
        // if mid node is null create a new one
        if(cur->mid == NULL){
	  // complete the newly added part of the Tree
	  while(index < strLen-1){
	    index++;
	    ch = str.at(index);	
	    cur->mid = new Node(ch);
	    cur = cur->mid;
 	  }
        }
        else if(cur->mid != NULL){
	  cur = cur->mid;
          index++;
	  ch = str.at(index);
        }
      }
	
      // set node to the left child 
      else if( ch < cur->val){
        // if left node is null create a new one
        if(cur->left == NULL){
 	  cur->left = new Node(ch);
	  // complete the newly added part of the Tree
	  while(index < strLen-1){
	    index++;
	    ch = str.at(index);	
	    cur->mid = new Node(ch);
	    cur = cur->mid;
 	  }
	  return;
        }
        else if(cur->left != NULL){
	  cur = cur->left;
        }
      }

      // set node to the right child 
      else if( ch > cur->val){
        // if right node is null create a new one
        if(cur->right == NULL){
	  cur->right = new Node(ch);
	  // complete the newly added part of the Tree
	  while(index < strLen-1){
	    index++;
	    ch = str.at(index);	
	    cur->mid = new Node(ch);
	    cur = cur->mid;
 	  }
	  return;
        }
        else if(cur->right != NULL){
	  cur = cur->left;
        }  
      }
    }
    // update  all the field after insertion
    cur->isWord = 1;
    cur->freq++;
    cur->string = str;
  }

  /* Return up to 10 of the most frequent completions
   * of the prefix, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than 10 legal completions, this
   * function returns a vector with as many completions as possible.
   * Otherwise, 10 completions should be returned.
   * If no completions exist, then the function returns a vector of size 0.
   * The prefix itself might be included in the returned words if the prefix
   * is a word (and is among the 10 most frequent completions
   * of the prefix)
   * If you need to choose between two or more completions which have the same frequency,
   * choose the one that comes first in alphabetical order.
   *
   * Inputs: prefix. The prefix to be completed. Must be of length >= 1.
   * Return: the vector of completions
   */
  vector<string> Autocomplete:: predictCompletions(const string & prefix) const{ 
    // var to store all strs found 
    vector<string> ret{};

    int strLen = prefix.size(); 
    int index = 0;
    Node * cur = root;
    char ch;    

    // find the last char of prefix	
    while(index < strLen){
      ch = prefix.at(index);
	
      // empty tree
      if(cur == NULL) return ret;
      // less than cur
      if(ch < cur->val) cur = cur->left;
      // greater than cur
      else if(ch > cur->val) cur = cur->right;
      // equal 
      else{
	if(cur->mid != NULL){
	  cur = cur->mid;
	  index++;
	}
	else return ret;
      }
    }
    if(cur->isWord != 1) return ret;

    // start to find the remaining part of the prefix
    stack <Node> s;
    // to store all possible words
    priority_queue<pair<int,string>> pq;

    // DFS the tree
    s.push(*cur);
    while(s.size() != 0){
      Node temp = s.pop();
      if(temp.isWord == 1) pq.push(make_pair(temp.freq,temp.word));
      if(temp.left != NULL) s.push(*(temp.left));
      if(temp.mid != NULL) s.push(*(temp.mid));
      if(temp.right != NULL) s.push(*(temp.right));
    }

    // push first 10 ele in pq to the return vector 
    for(int k = 0; k < NUM; k++){ 
      auto pair = pq.pop();
      ret.push_back(pair.second);
    }

    return ret;
	 
    
  };

  /* Destructor */
  Autocomplete::~Autocomplete(){};
};
#endif // AUTOCOMPLETE_HPP

