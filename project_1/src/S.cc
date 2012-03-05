#include "S.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

S * S::instance = NULL;

S::S() {
    this->_cache = new cache;
    srand(time(NULL));
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

configuration * S::get_config() const {
    return this->_config;
}

void S::eebl_enqueue(message * msg) {
    std::lock_guard<std::mutex> lk(this->_mutex_eebl_sending_queue);
    this->_eebl_sending_queue.push(msg);
}

message * S::eebl_dequeue() {
    std::lock_guard<std::mutex> lk(this->_mutex_eebl_sending_queue);
    if (this->_eebl_sending_queue.empty())
        return NULL;
    else {
        auto r = this->_eebl_sending_queue.front();
        this->_eebl_sending_queue.pop();
        return r;
    }
}

const bool S::eebl_queue_empty() {
    std::lock_guard<std::mutex> lk(this->_mutex_eebl_sending_queue);
    return this->_eebl_sending_queue.empty();
}

cache * S::get_cache() {
    return this->_cache;
}

const vehicle * S::me() const{
    return this->_me;
}

unsigned short S::get_speed(){
    std::lock_guard<std::mutex> lk(this->_mutex_speed);
    return this->_speed;
}

void S::set_speed(unsigned short speed) {
    std::lock_guard<std::mutex> lk(this->_mutex_speed);
    this->_speed = speed;
}

const gps & S::get_gps() {
    std::lock_guard<std::mutex> lk(this->_mutex_gps);
    return this->_gps;
}

void S::set_gps(float x, float y, float z) {
    std::lock_guard<std::mutex> lk(this->_mutex_gps);
    this->_gps.gps_x = x;
    this->_gps.gps_y = y;
    this->_gps.gps_z = z;
}
short S::get_acceleration() {
    std::lock_guard<std::mutex> lk(this->_mutex_acceleration);
    return this->_acceleration;
}

short S::get_deceleration(){
    std::lock_guard<std::mutex> lk(this->_mutex_acceleration);
    return - this->_acceleration;
}

void S::set_acceleration(short acceleration) {
    std::lock_guard<std::mutex> lk(this->_mutex_acceleration);
    this->_acceleration = acceleration;
}

void S::set_deceleration(short deceleration) {
    std::lock_guard<std::mutex> lk(this->_mutex_acceleration);
    this->_acceleration = - deceleration;
}

float S::random_float_0_1() {
    return rand() * 1.0f / RAND_MAX;
}

float S::random_float_negative_to_positive_1() {
    return (rand() * 1.0f / RAND_MAX) * 2 - 1;
}

unsigned int S::random_int_0_inf() {
    return rand();
}

int S::random_int(int lower_bound, int upper_bound) {
    return rand() % (upper_bound - lower_bound) + lower_bound;
}

void S::log(std::string content) {
    std::cout<<"Timestamp: "<<time(NULL)<<" Node: "<<this->_me->id()<<" "<<content<<endl;
}
