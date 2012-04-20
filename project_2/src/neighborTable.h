#ifndef _neighborTable_h
#define _neighborTable_h
#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include "message.h"
using namespace std;
class neighborTable{
    private:
        typedef unsigned int uint;
     // the below struct is used as value type in table
        struct neighbor_time{
            unordered_set<uint> neighbors;
            time_t time;
        };
    // the below struct is used as the compare template argument
    // in the min_element method from STL algorithm
        struct myCompare{
            bool operator()(neighbor_time n1, neighbor_time n2){
                return n1.neighbors.size() > n2.neighbors.size();
            }
        }myCmpObj;
        
        bool changed;
    // below is the neighbor table, key is one hop node, value is
    // this key's one hop neighbors and the latest time the entry 
    // is inserted to the table
        unordered_map<uint, neighbor_time> table;
    // @param t-- a input table
    // This is a helping function used in public method get_MPRs()
    // the function will return true if all 2 hop neighbors are 
    // covered by the current MPR set, otherwise return false
        bool hop2AllCovered(const unordered_map<uint, neighbor_time>& t);
    public:
    /* @param m --a newly received hello message pointer
     * this method is going to insert an entry to table if the 
     * originator was not previously in table, otherwise
     * update the corresponding entry's timestamp  
     */
        void newHelloMsg(message* m);
    /* the update() method will remove entries in the table 
     * which are older than 3s
     */
        void update();
    /*This method is going to return all 1 hop neighbors as an 
     *unordered_set
     */
        unordered_set<uint> get_1hop_neighbors();
     /*this method is going to compute a set of MPRs which
     *will cover all 2 hop neighbors
     */
        unordered_set<uint> get_MPRs();
    /*This method is going to return the 1 hop neighbors of 
     *a given 1 hop node
     */
        unordered_set<uint> get_1hop_byNode(uint node);
 };
#endif
