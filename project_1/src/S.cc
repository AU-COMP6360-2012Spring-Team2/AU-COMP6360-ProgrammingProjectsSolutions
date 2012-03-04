#include "S.h"

#include <iostream>
#include <ctime>


S * S::instance = NULL;

S::S() : _eng(time(NULL)), _negative_one_one_dist(-1, 1){
    this->_cache = new cache;

}

void S::initialize(std::string * config_file, unsigned int node_number){
    if(instance){
        std::cerr<<"error: S has already been initialized."<<endl;
        throw ;
    }
    instance = new S;
    // here initialize objects in S.
    instance->_config = new configuration(config_file);
    instance->_me = new vehicle(node_number, 0, 2, 1, 1);
}

S * S::get(){
    if(instance)
        return instance;
    else {
        std::cerr<<"error: S not initialized. Call S::initialize(config_file) first.";
        throw ;
    }
}

const configuration * S::get_config() const {
    return this->_config;
}

std::queue<message * > S::get_eebl_sending_queue() const {
    return this->_eebl_sending_queue;
}

cache * S::get_cache() {
    return this->_cache;
}

const vehicle * S::me() const{
    return this->_me;
}

const unsigned short S::get_speed() const {
    return this->_speed;
}

void S::set_speed(unsigned short speed) {
    this->_speed = speed;
}

const gps & S::get_gps() const {
    return this->_gps;
}

void S::set_gps(float x, float y, float z) {
    this->_gps.gps_x = x;
    this->_gps.gps_y = y;
    this->_gps.gps_z = z;
}

float S::random_float_0_1() {
    return this->_zero_one_dist(this->_eng);
}

float S::random_float_negative_to_positive_1() {
    return this->_negative_one_one_dist(this->_eng);
}

unsigned int S::random_int_0_inf() {
    return this->_zero_inf_dist(this->_eng);
}

int S::random_int(int lower_bound, int upper_bound) {
    std::uniform_int_distribution<int> d(lower_bound, upper_bound);
    return d(this->_eng);
}
