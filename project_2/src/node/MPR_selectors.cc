#include <vector>

#include "MPR_selectors.h"
#include "container_operators.h"

void MPR_selectors::newHelloMsg(HelloMsg *m){
    //if the link status is MPR, then check whether the originator is in MPR selectors table or not
    if(m->getLinkStatus() == HelloMsg::MPR){
        lock_guard<mutex> lk(m_selectors);
        auto it = selectors.begin();
        for(; it != selectors.end(); it++){
//if the originator is already in table, then update its timestamp
            if(it->first == m->getOriginator()){
                it->second = time(NULL);
            }
        }
//if the originator is not in the table, then insert a new entry
        if(it == selectors.end()){
            selectors.insert(pair<unsigned int, time_t>(m->getOriginator(), time(NULL)));
        }
    }
}

void MPR_selectors::update(){
    lock_guard<mutex> lk(m_selectors);
    time_t now = time(NULL);
    /*
    auto it = selectors.begin();
    while(it != selectors.end()){
        if(it->second < now-3){
            //if the entry is older than 3s, then remove it
            it = selectors.erase(it);
        }
        else{
            it++;
        }
    } */
    std::vector<map<unsigned int, time_t>::iterator> to_erase;
    for(auto i = selectors.begin(); i != selectors.end(); ++i)
        if(i->second < now - 3)
            to_erase.push_back(i);
    for(auto i = to_erase.begin(); i != to_erase.end(); ++i)
        selectors.erase(*i);
}

bool MPR_selectors::isMPRSelector(unsigned int node){
    lock_guard<mutex> lk(m_selectors);
   return selectors.count(node)>0; 
}
