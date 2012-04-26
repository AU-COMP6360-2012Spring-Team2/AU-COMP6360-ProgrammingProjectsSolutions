#ifndef _conf_h
#define _conf_h
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;
typedef unsigned int usint;
struct gps {
    float gps_x, gps_y, gps_z;
};

struct tuxname_port{
    string tuxname;
    unsigned short port;
};
 
class configuration{
//fields
    private:
       // map<usint, vector<usint> > * links;
        map<usint, struct gps > *gps_coordinate; 
        map<usint, struct tuxname_port> *name_port;

    public:
        configuration(string * filename);
       // map<usint, vector<usint> > * getlinks();
        map<usint, struct gps> * get_gps_coordinate();
        map<usint, struct tuxname_port> * get_name_port();
        map<usint, struct tuxname_port> * get_name_port_hello();
        ~configuration();
};
 
#endif
