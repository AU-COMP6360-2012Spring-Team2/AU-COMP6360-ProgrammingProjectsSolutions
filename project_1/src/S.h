#ifndef CLASS_S
#define CLASS_S

#include <string>
#include <queue>
#include <mutex>

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
        short _acceleration;

        std::mutex _mutex_eebl_sending_queue;
        std::mutex _mutex_gps;
        std::mutex _mutex_speed;
        std::mutex _mutex_acceleration;

    public:
        static void initialize(std::string * config_file, unsigned int node_number);
        static S * get();
        
         configuration * get_config() const;

        void eebl_enqueue(message * msg);
        message * eebl_dequeue();
        const bool eebl_queue_empty() ;

        cache * get_cache();
        const vehicle * me() const;

        const gps & get_gps() ;
        void set_gps(float x, float y, float z);
        unsigned short get_speed() ;
        void set_speed(unsigned short speed);
        short get_acceleration() ;
        short get_deceleration() ;
        void set_acceleration(short acceleration);
        void set_deceleration(short deceleration);


        float random_float_0_1();
        float random_float_negative_to_positive_1();
        unsigned int random_int_0_inf();
        int random_int(int lower_bound, int upper_bound);

       
};

#endif
