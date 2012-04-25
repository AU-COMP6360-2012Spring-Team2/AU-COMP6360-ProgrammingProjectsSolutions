#ifndef _neighborTable_h
#define _neighborTable_h
#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include "HelloMsg.h"
#include <mutex>
using namespace std;

typedef unsigned int uint;

class neighborTable{
    private:
        
             // the below struct is used as value type in table as a map
        struct neighbor_time{
            unordered_set<uint> neighbors;
            time_t time;
        };

        typedef unordered_map<uint, neighbor_time> M; 
   /* the below struct is used as the compare template argument
    * in the max_element method from STL algorithm, which is used
    * in the public method get_MPRs();
    */
        struct myCompare{
            bool operator()(M::value_type& n1, M::value_type& n2){
                return n1.second.neighbors.size() < n2.second.neighbors.size();
            }
        }myCmpObj;
     /*the bool variable "changed" indicates whether the neighbor table has
     *changed or not since last computing MPRs,
     *so recomputing of MPRs is necessary only when this bool is true 
     */
        bool changed;
    /* below is the neighbor table, key is one hop node, value is
    * this key's one hop neighbors and the latest time the entry 
    * is inserted to the table
    */
        unordered_map<uint, neighbor_time> table;
    /* @param t-- a input table
    * This is a helping function used in public method get_MPRs()
    * the function will return true if all 2 hop neighbors are 
    * covered by the current MPR set, otherwise return false
    */
        bool hop2AllCovered(const unordered_map<uint, neighbor_time>& t);
    //the mutex variable to ensure read and write to "table" is thread safe
        mutex mutex_table;
    public:
    /* @param m --a newly received hello message pointer
     * this method is going to insert an entry to table if the 
     * originator was not previously in table, otherwise
     * update the corresponding entry's timestamp  
     */
        void newHelloMsg(HelloMsg* m);
    /* the update() method will remove entries in the table 
     * which are older than 3s
     */
        void update();
    /*This method is going to return all 1 hop neighbors as an 
     *unordered_set
     */
        unordered_set<uint> get_1hop_neighbors();
    /*This method is going to return the field "changed"
     *which indicates whether the table has changed or not
     *since last time get_MPRs() is invoked
     */
        bool tableChanged();
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
