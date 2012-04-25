#ifndef _HelloMsg_h
#define _HelloMsg_h
#include <unordered_set>
using namespace std;
class HelloMsg{
    private:
        unsigned int originator;
        unsigned char link_status;
        //give a fixed size neighbor list, will fill with uint type's max value if the node's neighbors is less than 10
        unsigned int neighbors[10];
     //here don't know whether 10 is big enough, need to test
    public:
        //the total size for Hello msg is 45 bytes. 4 + 1 + 4*10
        static const unsigned int HELLOMSG_SIZE = 45;
        //consts for link status
        static const unsigned char UNIDIRECTION = 1;
        static const unsigned char BIDIRECTION = 2;
        static const unsigned char MPR = 3;
        /*@param o --originator ID
         *@param s --link status
         *@param n --1 hop neighbors set
         *This method is going to create a new Hello Msg according to the paramters given
         */
        static HelloMsg* create_Hello(unsigned int o, unsigned char s, const unordered_set<unsigned int>& n);
        //This method is going to set the link status to 1 of the 3 kinds
        void setLinkStatus(unsigned char s);
        //This method is going to get the link status of the current message
        unsigned char getLinkStatus();
        //This method is going to get the Originator ID of the Hello Msg
        unsigned int getOriginator();
        //this method returns the 1 hop neighbors stored in Hello Msg
        unordered_set<unsigned int> getNeighbors();
        //this method serializes the Hello Msg to char*, store at buffer given as input
        //buffer should be at least 45 bytes long
        void to_bytes(unsigned char buffer[]) const;
        //this method takes serialized char*, and convert it back to a HelloMsg object
        static HelloMsg* from_bytes(const unsigned char data[]);
};

#endif
