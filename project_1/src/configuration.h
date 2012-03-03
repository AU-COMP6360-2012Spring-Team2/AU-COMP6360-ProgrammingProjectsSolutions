#ifndef _conf_h
#define _conf_h
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

struct gps {
    unsigned int gps_x, gps_y, gps_z;
};

struct tuxname_port{
    string tuxname;
    int port;
};
 
class configuration{
//fields
    private:
        map<int, vector<int> > * links;
        map<int, struct gps > *gps_coordinate; 
        map<int, struct tuxname_port> *name_port;

    public:
        configuration(string filename);
        map<int, vector<int> > * getlinks();
        map<int, struct gps> * get_gps_coordinate();
        map<int, struct tuxname_port> * get_name_port();
        ~configuration();
};
 
#endif
