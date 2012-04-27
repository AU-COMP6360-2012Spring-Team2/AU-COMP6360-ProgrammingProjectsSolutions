#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "vehicle_mgr.h"

using namespace std;

bool wrong_arg(char * argv0) {
    cout<<"usage: "<<argv0<<" -c config_file"<<endl; 
    return false;
}

bool parse_arg(int argc, char ** argv, std::string & config_file) {
    int c;
    bool c_flag = false;
    while ((c = getopt ( argc, argv, "c:")) != -1)
        switch (c) {
           case 'c':
                c_flag = true;
                config_file = std::string(optarg);
                break;
            default:
                return wrong_arg(argv[0]);
        }

    if (!c_flag)
        return wrong_arg(argv[0]);

    return true;
}

void parse_config_file(std::string config_file, std::unordered_map<unsigned int, vehicle_mgr::location> & initial_locations) {
    ifstream ifs ( config_file.c_str(), ifstream::in );
    while(ifs.good()){
        std::string line;
        getline(ifs, line);
        stringstream ss(line);
        std::string str;
        unsigned int id;
        float x, y;
        ss >> str >> id >> str >> str >> str >> str >> str >> x >> y ;
        initial_locations[id]=vehicle_mgr::location(x, y, 0);
    }
    ifs.close();
}
int main(int argc, char ** argv) {
    std::string config_file;

    if (parse_arg(argc, argv, config_file)) {
        std::unordered_map<unsigned int, vehicle_mgr::location> initial_locations;
        parse_config_file(config_file, initial_locations);
        std::vector<float> grid_points_x({0,120,240,360,480,600,720});
        std::vector<float> grid_points_y({0,120,240,360,480,600,720});
        vehicle_mgr vm("../locations", 200, grid_points_x, grid_points_y, initial_locations, (vehicle_mgr::_SPEED_CM_PER_SECOND_MIN + vehicle_mgr::_SPEED_CM_PER_SECOND_MAX) / 2, .4);
        vm.run();
    }

    return 0;
}
