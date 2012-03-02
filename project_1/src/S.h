#ifndef CLASS_S
#define CLASS_S

#include <string>
#include <queue>

#include "configuration.h"
#include "message.h"
#include "cache.h"

class S {
    private:
        static S * instance;
        S();
        
        //here put objects that should be contained in S. objects should not be 'static'.
        configuration * _config;
        std::queue<message * > _eebl_sending_queue;
        cache * _cache;

    public:
        static void initialize(std::string * config_file);
        static S * get();

        cache * get_cache();
};

#endif
