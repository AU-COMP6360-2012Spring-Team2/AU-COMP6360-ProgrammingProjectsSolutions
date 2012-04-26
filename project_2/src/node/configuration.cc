#include "configuration.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string.h>

using namespace std;

configuration::configuration(string * filename){
    ifstream in(filename->c_str());
    //links = new map<usint, vector<usint> >;
    gps_coordinate = new map<usint, struct gps>;
    name_port = new map<usint, struct tuxname_port>;
    name_port_hello = new map<usint, struct tuxname_port>;

    if(!in){
        cout<<"cannot open file "<<filename<<endl;
    }

    string line;
    int i = 0;
    struct tuxname_port tuxnode;
    struct gps gps_coord;
    //vector<usint> linkednodes;
    while(getline(in,line))
    {
        istringstream iss(line);
        istream_iterator<string> iit(iss);
        istream_iterator<string> eos;
        iit++; //omit the first word "Node"
        iit++; //omit node id
        if(i<30){
            tuxnode.tuxname = *iit;
            iit++;
            tuxnode.port = atoi((*iit).c_str());
            name_port->insert(pair<usint, struct tuxname_port>(i+1, tuxnode));
            iit++;
            tuxnode.tuxname = *iit;
            iit++;
            tuxnode.port = atoi((*iit).c_str());
            name_port_hello->insert(pair<usint, struct tuxname_port>(i+1, tuxnode));

            iit++;
            //linkednodes.clear();//make sure the vector is empty at first;
           // while((*iit).compare("gps")!=0){
             //   linkednodes.push_back(atoi((*iit).c_str()));
               // iit++;
            //}
            iit++;//omit the word "gps"
            //links->insert(pair<usint, vector<usint> >(i+1,linkednodes));
            gps_coord.gps_x = (float) atof((*iit).c_str());
            iit++;
            gps_coord.gps_y = (float) atof((*iit).c_str());
            gps_coord.gps_z = 0; 
            gps_coordinate->insert(pair<usint, struct gps>(i+1,gps_coord));
        }
        i++;
    }
    in.close();
}

//map<usint, vector<usint> > * configuration:: getlinks(){
  //  return links;
//}

map<usint, struct gps>* configuration:: get_gps_coordinate(){
    return gps_coordinate;
}

map<usint, struct tuxname_port> * configuration:: get_name_port() {
    return name_port;
}

map<usint, struct tuxname_port> * configuration:: get_name_port_hello() {
    return name_port_hello;
}


configuration::~configuration() {
    //delete links;
    delete gps_coordinate;
    delete name_port;
     delete name_port_hello;

}

