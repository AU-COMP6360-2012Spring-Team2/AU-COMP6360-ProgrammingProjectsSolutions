#include "HelloMsg.h"
#include <limits>
#include "string.h"
using namespace std;

HelloMsg * HelloMsg::create_Hello(unsigned int o, unsigned char s, const unordered_set<unsigned int>& n){
    HelloMsg *m = new HelloMsg;
    m->originator = o;
    m->link_status = s;
    int j = 0;
    for(auto it = n.begin(); it != n.end(); it++){
       m->neighbors[j++] = *it;
    }

    for(; j < 10; j++){
        m->neighbors[j] = numeric_limits<unsigned int>::max();
    }
    return m;
}

void HelloMsg::setLinkStatus(unsigned char s){
    this->link_status = s;
}

unsigned char HelloMsg::getLinkStatus(){
    return this->link_status;
}

unsigned int HelloMsg::getOriginator(){
    return this->originator;
}

unordered_set<unsigned int> HelloMsg::getNeighbors(){
   
    unordered_set<unsigned int> temp;
    for(int i = 0; i < 10; i++){
        if(neighbors[i] != numeric_limits<unsigned int>::max()){
            temp.insert(neighbors[i]);
        }
    }

    return temp;
}

void HelloMsg::to_bytes(unsigned char buffer[]) const {
   
  memcpy(buffer, &(this->originator), 4);
  memcpy(buffer+4, &(this->link_status), 1);
  memcpy(buffer+5, this->neighbors, 40);
}

HelloMsg* HelloMsg::from_bytes(const unsigned char data[]){
    HelloMsg *m = new HelloMsg;
    memcpy(&(m->originator), data, 4);
    memcpy(&(m->link_status), data+4, 1);
    memcpy((m->neighbors), data+5, 40);
    return m;
}
