#ifndef CLASS_S
#define CLASS_S

#include <string>
#include <queue>
#include <random>

#include "configuration.h"
#include "message.h"
#include "cache.h"
#include "vehicle.h"

class S {
    private:
        static S * instance;
        S();
        
        //here put objects that should be contained in S. objects should NOT be 'static'.
        configuration * _config;
        std::queue<message * > _eebl_sending_queue;
        cache * _cache;
        vehicle * _me;
        gps _gps;
        unsigned short _speed;
        std::mt19937 _eng;
        std::uniform_real_distribution<float> _zero_one_dist;
        std::uniform_real_distribution<float> _negative_one_one_dist;
        std::uniform_int_distribution<unsigned int> _zero_inf_dist;

    public:
        static void initialize(std::string * config_file, unsigned int node_number);
        static S * get();
        
        const configuration * get_config() const;
        std::queue<message * > get_eebl_sending_queue() const;

        cache * get_cache();
        const vehicle * me() const;

        const gps & get_gps() const;
        void set_gps(float x, float y, float z);
        const unsigned short get_speed() const;
        void set_speed(unsigned short speed);


        float random_float_0_1();
        float random_float_negative_to_positive_1();
        unsigned int random_int_0_inf();
        int random_int(int lower_bound, int upper_bound);

       
};

#endif
