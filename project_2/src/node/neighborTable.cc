#include "neighborTable.h"
#include "container_operators.h"
#include <algorithm>
#include <iostream>
void neighborTable::newHelloMsg(HelloMsg *m){
    uint org = m->getOriginator();
    unordered_set<uint> nbs = m->getNeighbors();

    //lock the neighbor table by mutex
    lock_guard<mutex> lk(this->mutex_table);
    //if the originator is already in the table
    if(table.count(org) > 0){
        //check whether the 1 hop neighbors of the originator has
        //changed
        if (nbs == table[org].neighbors){
            table[org].time = time(NULL);    
        }
        else{
            table.erase(org);
            neighbor_time temp;
            temp.neighbors = nbs;
            temp.time = time(NULL);
            table.insert(pair<uint, neighbor_time>(org, temp) );
            changed = true;
        }
    }
    else{
        //if the originator was not in table, need to add an entry 
        //to the table
        neighbor_time temp;
        temp.neighbors = nbs;
        temp.time = time(NULL);
        table.insert(pair<uint, neighbor_time>(org, temp) );
        changed = true;
    }

}

void neighborTable::update(){
    //lock the neighbor table by mutex
    lock_guard<mutex> lk(this->mutex_table);
    //check the table entry by entry, remove the ones older than 3s
    auto it = table.begin();

    time_t t = time(NULL);
    while( it != table.end()){
        if (it->second.time < (t -3)){
            it = table.erase(it);
            changed = true;
        }
        else{
            it++;
        }

    }
}

unordered_set<uint> neighborTable::get_1hop_neighbors(){
    //lock the neighbor table by mutex
    lock_guard<mutex> lk(this->mutex_table);
    //iterate through neighbor table, add 1 hop neighbor nodes to result
    auto it = table.begin();
    unordered_set<uint> hop1;

    for(; it != table.end(); it++){
        hop1.insert(it->first);
    }

    return hop1;
}


unordered_set<uint> neighborTable::get_MPRs(){
    unordered_set<uint> MPRs;

    unordered_map<uint, neighbor_time> temp_table;
    {
        //lock the neighbor table by mutex
        lock_guard<mutex> lk(this->mutex_table);
        temp_table = this->table;
        changed = false;
    }
    //the lock is now released, only operate on temp_table in the rest of the method

    //get the 1 hop neighbors in temp_table
    auto it = temp_table.begin();
    unordered_set<uint> hop1;

    for(; it != temp_table.end(); it++){
        hop1.insert(it->first);
    }

    //first substract all 1 hop neighbors, then pick the node with 
    //the biggest coverage of 2 hop neighbors
    it = temp_table.begin();
    for(; it!= temp_table.end(); it++){
        it->second.neighbors -= hop1;
    }

    std::cout<<"!!!!"<<std::endl;
    if(!temp_table.empty()) {
        uint mpr = (max_element(temp_table.begin(), temp_table.end(), myCmpObj))->first;
        std::cout<<"ok"<<std::endl;
        MPRs.insert(mpr);
        //substract the newly selected mpr node's 1 hop neighbors,
        //for every entry in the neighbor table
        hop1 = temp_table[mpr].neighbors;
        for(it = temp_table.begin(); it!= temp_table.end(); it++){
            it->second.neighbors -= hop1;     

        }


        while(!hop2AllCovered(temp_table)){

            mpr = (max_element(temp_table.begin(), temp_table.end(), myCmpObj))->first;
            MPRs.insert(mpr);
            hop1 = temp_table[mpr].neighbors;

            for(it = temp_table.begin(); it!= temp_table.end(); it++){
                it->second.neighbors -= hop1; 
            }

        } 

    }

    return MPRs;
}

bool neighborTable::hop2AllCovered(const unordered_map<uint, neighbor_time>& t){
    auto it = t.begin();
    for(; it!=t.end(); it++){
        if(it->second.neighbors.size() > 0){
            return false;
        }
    }
    return true;
}


unordered_set<uint> neighborTable::get_1hop_byNode(uint node){
    //lock the neighbor table by mutex
    lock_guard<mutex> lk(this->mutex_table);

    return table[node].neighbors;
}

bool neighborTable::tableChanged(){
    //lock the neighbor table by mutex
    lock_guard<mutex> lk(this->mutex_table);

    return changed;
}
