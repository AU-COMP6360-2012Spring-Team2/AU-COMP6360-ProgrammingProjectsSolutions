#include "physical_world.h"

#include <sys/time.h>
#include <cmath>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <sstream>

physical_world * physical_world::_instance = NULL;

void physical_world::_update_from_memcache() {
    /*std::stringstream ss;
    ss<<"/tmp/location-"<<this->_me_id;
    std::stringstream ssc;
    ssc<<"/bin/cp "<<this->_shared_config_file<<" "<<ss.str();
    system(ssc.str().c_str());
    std::ifstream ifs;
    ifs.open(ss.str());*/
    std::ifstream ifs;
    ifs.open(this->_shared_config_file);
    if(!ifs)
        std::cout<<"aaaaa cannot open1111111!!!!!!"<<this->_shared_config_file<<std::endl;
    while(!ifs.eof()){
        std::string id;
        float x,y,z;
        unsigned short speed;
        short accer;
        ifs>>id>>x>>y>>z>>speed>>accer;
        std::cout<<"nodesIn"<<id<<" "<<x<<" "<<y<<" "<<speed<<" "<<accer<<" "<<std::endl;
        if(!id.empty() && id!="") {
            this->_vehicles[id]._x = x;
            this->_vehicles[id]._y = y;
            this->_vehicles[id]._z = z;
            this->_vehicles[id]._speed = speed;
            this->_vehicles[id]._acceleration = accer;
            std::cout<<"read: "<<id<<" "<<this->_vehicles[id]._x<<" "<<this->_vehicles[id]._y<<std::endl;
        }
    }
    ifs.close();
}

void physical_world::_update_thread_fun() {
    while(true) {
        usleep(1000 * this->_update_interval_in_milliseconds);
        this->_update_from_memcache();
    }
}

void physical_world::initialize(std::string shared_config_file, unsigned int update_interval_in_milliseconds, std::vector<unsigned int> vehicle_id, unsigned int me_id) {
    physical_world::_instance = new physical_world;
    _instance->_shared_config_file = shared_config_file;
    _instance->_update_interval_in_milliseconds = update_interval_in_milliseconds;
    _instance->_me_id = me_id;
    for(auto i = vehicle_id.begin(); i != vehicle_id.end(); ++i){
        mc_vehicle v;
        _instance->_vehicles[mc_vehicle::vehicleID2mcID(*i)] = v;
    }
    _instance->_update_from_memcache();
   _instance->_update_thread = new std::thread(&physical_world::_update_thread_fun, _instance);
}

mc_vehicle & physical_world::vehicle_info(unsigned int vehicle_id) {
    std::lock_guard<std::mutex> l(this->_mutex_vehicles);
    return this->_vehicles[mc_vehicle::vehicleID2mcID(vehicle_id)];
}

bool physical_world::is_in_communication_range(unsigned int vehicle_id_1, unsigned int vehicle_id_2, float communication_range) {
    std::lock_guard<std::mutex> l(this->_mutex_vehicles);
    mc_vehicle & v1 = this->_vehicles[mc_vehicle::vehicleID2mcID(vehicle_id_1)],
               v2 = this->_vehicles[mc_vehicle::vehicleID2mcID(vehicle_id_2)];
    return (pow(communication_range,2) >= pow(v2.x() - v1.x(),2) + pow(v2.y() - v1.y(),2));
}

std::unordered_set<unsigned int> physical_world::all_vehicles_in_communication_range(unsigned int vehicle_id, float communication_range) {
    std::lock_guard<std::mutex> l(this->_mutex_vehicles);
    mc_vehicle & me = this->_vehicles[mc_vehicle::vehicleID2mcID(vehicle_id)];
    std::unordered_set<unsigned int> r;
    for(auto i = this->_vehicles.begin(); i != this->_vehicles.end(); ++i)
    {
        std::cout<<"comparing me("<<me.x()<<","<<me.y()<<") and i("<<i->second.x()<<","<<i->second.y()<<")"<<std::endl;
        if(pow(communication_range,2) >= pow(me.x() - i->second.x(),2) + pow(me.y() - i->second.y(),2)) {
            std::cout<<"comparing me("<<me.x()<<","<<me.y()<<") and i("<<i->second.x()<<","<<i->second.y()<<") are in range"<<std::endl;
            unsigned int _id = mc_vehicle::mcID2vehicleID(i->first);
            if(_id != vehicle_id)
                r.insert(_id);
        }
    }
    return r;
}

float physical_world::distance(unsigned int vehicle_id_1, unsigned int vehicle_id_2) {
    std::lock_guard<std::mutex> l(this->_mutex_vehicles);
    mc_vehicle & v1 = this->_vehicles[mc_vehicle::vehicleID2mcID(vehicle_id_1)],
               v2 = this->_vehicles[mc_vehicle::vehicleID2mcID(vehicle_id_2)];
    return sqrt(pow(v2.x() - v1.x(),2) + pow(v2.y() - v1.y(),2));
}

