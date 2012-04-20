#include "neighborTable.h"
#include "container_operators.h"
void neighborTable::newHelloMsg(message *m){
    uint org = m->getOriginator();
    unordered_set<uint> nbs = m->getNeighbors();

    //if the originator is already in the table
    if(table.count(org) > 0){
        //check whether the 1 hop neighbors of the originator has
        //changed
        if (nbs == this->get_1hop_byNode(org)){
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
    unordered_map<uint, neighbor_time>::iterator it = table.begin();
    changed = false;
    time_t t = time(NULL);
    while( it != table.end()){
        if (it->second.time < (t -3)){
           it = table.erase(it);
           updated = true;
        }
        else{
            it++;
        }

    }
}

unordered_set<uint> neighborTable::get_1hop_neighbors(){

    unordered_map<uint, neighbor_time>::iterator it = table.begin();
    unordered_set<uint> hop1;

    for(; it != table.end(); it++){
        hop1.insert(it->first);
    }

    return hop1;
}


unordered_set<uint> neighborTable::get_MPRs(){
    unordered_set<uint> MPRs;

    unordered_map<uint, neighbor_time> temp_table = table;
    unordered_map<uint, neighbor_time>::iterator it = temp_table.begin();
    //first subtract all 1 hop neighbors, then pick the node with 
    //the biggest coverage of 2 hop neighbors
    for(; it!= temp_table.end(); it++){
        it->second.neighbors -= get_1hop_neighbors();
    }
    
    uint mpr = (max_element(temp_table.begin(), temp_table.end(), myCmpObj))->first;
    MPRs.insert(mpr);

  while(!hop2AllCovered(temp_table)){
    for(it = temp_table.begin(); it!= temp_table.end(); it++){
        it->second.neighbors -= temp_table[mpr].neighbors;
    }

    mpr = (max_element(temp_table.begin(), temp_table.end(), myCmpObj))->first;
    MPRs.insert(mpr);
  }

  return MPRs;
}

bool neighborTable::hop2AllCovered(const unordered_map<uint, neighbor_time>& t){
    unordered_map<uint, neighbor_time>::iterator it = t.begin();
    for(; it!=t.end(); it++){
        if(it->second.neighbors.size() > 0){
            return false;
        }
    }
    return true;
}


unordered_set<uint> get_1hop_byNode(uint node){
    return table[node].neighbors;
}

