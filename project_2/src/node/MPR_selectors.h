#ifndef _MPR_selectors_h
#define _MPR_selectors_h

#include <map>
#include <ctime>
#include "HelloMsg.h"
#include <mutex>
using namespace std;

class MPR_selectors{
    private:
       
    map<unsigned int, time_t> selectors;

    mutex m_selectors;

    public:
    /*This method is going to check whether a newly received
     *Hello message is MPR status
     *if so, insert a new entry
     *or update the timestamp of an existing entry according to
     *whether the originator is already in the table or not
     */
    void newHelloMsg(HelloMsg *m);
    /*This method is going to remove entries older than 3s
     * the method should be invoked every second
     */
    void update();
    /*This method checks whether a given node in the input param
     *is an MPR selector of the receiving node, so the receiving node
     *can decide whether to rebroadcast the given node's eebl
     */
    bool isMPRSelector(unsigned int node);

};

#endif
