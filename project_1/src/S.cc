#include "S.h"

#include <iostream>


S * S::instance = NULL;

S::S(){
}

void S::initialize(std::string * config_file){
    instance = new S;
    // here initialize objects in S.
    instance->config = new configuration(config_file);
}

S * S::get(){
    if(instance)
        return instance;
    else {
        std::cerr<<"error: S not initialized. Call S::initialize(config_file) first.";
        throw ;
    }
}
