#include "ActorGraph.hpp"
#include <iterator> 
#include <map> 
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
  ActorGraph ag;
  ag.loadFromFile(argv[1],false,true);

  ofstream output(argv[3]); 

  map<string,set<string>>:: iterator it = ag.friends.begin();
  map<string,set<string>>:: iterator remove;
  bool going = true;
  unsigned int k = atoi(argv[2]);
  
  while(going){
    it = ag.friends.begin();
    going = false;
    while(it != ag.friends.end()){
      if(it->second.size() < k){
	// we need to erase it, before that 
	// delete the link between the actor and its 
	// collaborated actor
	set<string> :: iterator inner = it->second.begin();
	while(inner != it->second.end()){
	  remove = ag.friends.find(*inner);
	  remove->second.erase(it->first);
	  inner++;
	}
	it = ag.friends.erase(it); 
	going = true;
        it = ag.friends.begin();
	continue;
      }
      else{
        it++;
      }
    }
  }
  output << "Actor" << "\n";
  it = ag.friends.begin();
  while(it != ag.friends.end()){
    output << it->first << "\n";
    it++;
  }

  return 0;
}
