/**
 * CSE 100 PA2 Document Generator
 */

#ifndef DOCUMENT_GENERATOR_HPP
#define DOCUMENT_GENERATOR_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <unordered_map> 
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>


#define SPACE ' '
#define SP " "
#define DOT "."
#define SLASH  "/"
using namespace std;

class DocumentGenerator
{
public:
  const string whitespace = " \t\r\n\v\f";
  unordered_map<string, vector<string>*> * map;
  
 /**
 * Builds a document generator from the documents in the given directory
 * This should open all files in the directory, read them in, tokenize them into words,
 * and build the datastructure from that stream of words.
 *
 * To tokenize a document, you are required to do the following,
 * in this order (or in a manner that is logically equivalent
 * to doing them in this order):
 * * remove all UNWANTED_CHARACTERS from the document
 * * split the document into different tokens based on whitespace
 *  (treat all whitespace breaks equally - newlines and spaces should be treated the same way. Whitespace is thrown away and not included in tokens. There should be no empty tokens.)
 * * additionally split each PUNCTUATION character into its own token
 *   (equivalently - treat all punctuation as if it was surrounded on both sides by a space)
 * * characters that are neither UNWANTED_CHARACTERS nor PUNCTUATION should be treated normally, the same way as any alphabetic character. This includes single apostrophes and accented characters.
 * * from here on, we'll use the word "word" to refer to all tokenized strings, such as "hello", "." or ","
 *
 * So, for instance, the short sentence
 * "I like the man's $10,000 trains. Sally jumped $ ov^er the moon, I think? I. I think."
 * Would be tokenized into ["I", "like", "the", "man's", "10", "000", "trains", ".", "Sally",
 *   "jumped", "over", "the", "moon", ",", "I", "think", "?", "I", ".", "I" "think", "."]
 * and the frequencies of the words after "I" would be;
 *  like - 1
 *  think - 2
 *  . - 1
 *
 * A few notes:
 * 1) you must treat words with different capitalizatoins differently
 * * (so "foo" and "Foo" are different words, and the frequencies of the words that follow (or precede)
 * * "foo" will be different than the frequencies of the words that follow (or precede) "Foo" )
 * 2) pretend that the first word in each document is preceeded by a periood (That way, it is considered when starting any new sentence)
 */
  DocumentGenerator(const string & documentsDirectory){
    // main unordered map to store all possible word appeared in the file list
    map = new unordered_map<string,vector<string>*>();
    // convert arg from string to char * in oredr to open dir
    //const char * directory = documentsDirectory.c_str();

    // read all files in the directory 
    DIR * dir;
    struct dirent *ent;
    if((dir = opendir(documentsDirectory.c_str())) != NULL){
      while((ent = readdir (dir)) != NULL){
	// modified word from a single file 
	string fileName = string(ent->d_name);
	string finalPath = documentsDirectory + SLASH + fileName;
	//cout << finalPath << "------" << endl;
	
        vector<string> * file = getWords(finalPath);
	if(file->size() == 0){	
	  delete file;
	  continue;
 	}

	// loop through the vector and put each word into words
	// and modify the nextWords field according to next word appeared 
	for(unsigned int i = 0; i < file->size(); i++){
	//for(unsigned int i = 0; i < file->size(); i++){
	  string cur = file->at(i); 
	  //cout << "  " << file->size() -1 << endl;
	  // if not in the map, add it in the map
	  if(map->find(cur) == map->end())  map->insert(make_pair(cur,new vector<string>()));
	  // put the next word in the mapped value of the map
	  unordered_map<string,vector<string>*> ::iterator toAdd = map->find(cur);
	  if(i+1 < file->size()){
	    toAdd->second->push_back(file->at(i+1));
	  }
	}
	// always put the first word of a file as the next word of "."
	if(map->find(DOT) == map->end())  map->insert(make_pair(DOT,new vector<string>()));

	unordered_map<string,vector<string>*> ::iterator it= map->find(DOT);
	it->second->push_back(file->at(0));
	
	// TESTING ----------
	/*
	cout << "file name:" << finalPath << endl;
	cout << "file size:" << file->size() << endl;
	cout << "map size" << map->size() << endl;
	*/
	// TESTING ----------
	

        delete file;
      }
    /*
    unordered_map<string,vector<string>*> :: iterator it;
    for(it = map->begin(); it != map->end(); it++){
	//cout << "------" << endl;
	cout << "size:" <<  it->second->size() << endl;
	cout << "word:" <<  it->first << endl;
	
	//for(unsigned int i = 0; i < it->second->size(); i++){
	// cout << "element " <<i <<" :" << it->second->at(i) << "  "; 
	//}
	cout << "------" << endl;
    } 
*/
// TESTING ---------------------
    }
    closedir(dir);
    
  };

  /**
 * Suppose you're in the middle of generating a document and that you've just added the word prevWord
 * to the document. generateNextWord(prevWord) should generate the next word at random for the document,
 * according to the frequencies of all the words that followed prevWord in the input documents
 *
 * So, for instance, if in all the input documents the word "foo" was followed by the word "bar" twice and "baz" 8 times,
 * then 80% of the time generateNextWord("foo") should return "baz" and 20% of the time you should return "bar".
 * 
 * This method should return punctuation words like "." and "," as if they were any other word.
 * Likewise, it should be able to take punctuation words as input (generateNextWord(","))
 * 
 * You can assume that prevWord is always a word that's present in one of the documents you've read in.
 */
  string generateNextWord(const string & prevWord){
    // word that has no successor return SPACE string
    if(map->find(prevWord) == map->end()) return SP;
    unordered_map<string,vector<string>*> ::iterator toAdd 
                                                     = map->find(prevWord);
    if(toAdd->second->size() == 0) return SP;
    int index = rand() % (toAdd->second->size());
    return toAdd->second->at(index); 
  };

  /**
 * Generate a document with numWords words in it.
 *
 * See the docs on generateNextWord for a description of what it means to generate a word.
 * After generating all the words - concatenate them all into a single string representing the entire document, and return that string.
 *
 * Notes:
 * The first word you generate should be as if the previous word was '.'
 * Your document should not contain whitespace except for spaces.
 * Your should insert a single space in front of each word except:
 * * Don't insert spaces in front of punctuation marks
 * * Don't insert a space at the start of the document
 * Punctuation "words" count against this word total.
 * If you generate a word which has no successor in the documents you
 *   are mimicing (because the word only appeared at the ends of documents)
 *   generate '.' as the next word.
 *
 * The document will likely not end at the end of a sentence. That's okay.
 */
  string generateDocument(const int numWords){
    int index = 0;
    string cur = DOT;
    //vector<string> temp = vector<string>();
    string temp;
    while(index < numWords){
      cur = generateNextWord(cur);

      // word that has no succesor
      if(cur == SP){ 
        cur = DOT;
        //continue;	
      }
 
      if(index != 0 && PUNCTUATION.find(cur) == string::npos)  temp+=SP;

      temp+=cur;

      index++;
    }

    return temp;
    
  };

 //--------Utilities
 //break a line over whitespace separators
 //caller must delete returned vector
 vector<string> * tokenize(const string & line) {
	/*
  	vector<string> * words = new vector<string>();
	vector<char> word;
	
	// create a char vector copy of string line
	vector<char> temp;
	//copy(line.begin(), line.end(), std::back_inserter(temp));
	for(unsigned int i = 0; i < line.size(); i++){
	  char cur = line.at(i);
          if(UNWANTED_CHARACTERS.find(cur) != string::npos){
	    continue;
	  }
	  else if( PUNCTUATION.find(cur) != string::npos){
            temp.push_back(SPACE);
	    temp.push_back(cur);
            temp.push_back(SPACE);
	    continue;
	  }
	  else{
	    temp.push_back(cur);
	  }
	}
	
	
	// remove all unwanted char and add a space before every punctuation
	for(vector<char>::iterator ptr = temp.begin(); ptr != temp.end(); ptr++){
          char cur = *ptr;
	  //cout << cur << "---" << endl;
          // exist unwanted char in the word 
          if(UNWANTED_CHARACTERS.find(cur) != string::npos){
            ptr = temp.erase(ptr);
	    // make up with ptr++
	    ptr--;
 	  }
          // check for punctuation if found a punc insert a space before it  
          else if(ptr != temp.begin() && PUNCTUATION.find(cur) != string::npos){
	    // put space at both side of punctuation
            temp.insert(ptr,SPACE);
	    ptr+=2;
	    if(ptr != temp.end())
              temp.insert(ptr,SPACE);
	  }
	}
	*/
	// convert the temp back to string 
	//string modLine = string(temp.begin(), temp.end());
	//cout << modLine << "---result" << endl;
	vector<string> * words = new vector<string>();
        vector<char> word;
	for (unsigned char c : line) {
	  // if c is not unwanted 
	  if(UNWANTED_CHARACTERS.find(c) == string::npos){
	        // push all valid char and punctuation 
		if (whitespace.find(c) == string::npos) {
		  // if c is a punctuation
		  if(PUNCTUATION.find(c) != string::npos){
		    if(word.size() > 0){
		      words->push_back(string(word.begin(), word.end()));
		      word.clear();
		    }
		    words->push_back(string(1,c));
		    continue;
		  }
	          word.push_back(c);
		} 
	        // push the word to words if a word finish 
                else if (word.size() > 0) {
			words->push_back(string(word.begin(), word.end()));
			word.clear();
		}
	  }
	}
	//for(unsigned int o = 0; o < words->size(); o++){
	//  cout << words->at(o) << " ";
	//}
	//cout << endl;

	if (word.size() > 0) {
		words->push_back(string(word.begin(), word.end()));
	}
	return words;
 } 

 //load a file and return a vector of all the words in that file
 //this function only splits the contents of the file over whitespace
 //caller must delete returned vector
 vector<string> * getWords(const string & fileLocation) {
	vector<string> * words = new vector<string>();
	// TODO ???????????
	//words->push_back(DOT);

	ifstream infile;
	infile.open(fileLocation);

	if (!infile) {
		cerr << "Couldn't open file: " << fileLocation << endl;
		exit(1);
	}

	string line;
	while(getline(infile, line)) {
		vector<string> * wordsInLine = tokenize(line);
		//TESTIGN _____
		/*
		for(unsigned int i = 0; i < wordsInLine->size();i++){
			cout << "#########" << wordsInLine->at(i) << endl;
		}
		i*/
		//TESTIGN _____
		words->insert(words->end(), wordsInLine->begin(), wordsInLine->end());
		//cout << "TESTING" << endl;
		//for(unsigned int j = 0; j < wordsInLine->size(); j++){
		//  words->push_back(wordsInLine->at(j));
		//}
		delete wordsInLine;
		//wordsInLine = NULL;
	}

	return words;  
 }

  ~DocumentGenerator(){
    unordered_map<string,vector<string>*> :: iterator it;
    for(it = map->begin(); it != map->end(); it++){
	it->second->clear();
	delete it->second;
    } 
    delete map;
  };

private:
  const string PUNCTUATION = ".!,?";
  const string UNWANTED_CHARACTERS = ";:\"~()[]{}\\/^_<>*=&%@$+|`";
};

#endif //DOCUMENT_GENERATOR_HPP

