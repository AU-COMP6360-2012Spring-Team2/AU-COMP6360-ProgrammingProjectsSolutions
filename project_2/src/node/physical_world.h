#ifndef H_physical_world
#define H_physical_world

#include <mutex>
#include <unordered_map>
#include <thread>
#include <unordered_set>

#include "../include/mc_vehicle.h"
#include "../include/mc.h"

#include <iostream>

class physical_world {
    private:
        mc * _m;
        std::thread * _update_thread;
        static physical_world * _instance;
        unsigned int _update_interval_in_milliseconds;

        std::mutex _mutex_vehicles;
        std::unordered_map<std::string, mc_vehicle> _vehicles;

        physical_world () { }
        physical_world ( const physical_world & ro ) { }
        physical_world operator= ( const physical_world & ro ) { std::cout<<"= called, is gonna ghrow"; throw; }

        void _update_from_memcache();
        void _update_thread_fun();

    public:
        static physical_world & get () {
            if(!_instance)
                throw;
            return *_instance;
        }
        ~physical_world() {
            delete _update_thread;
        }

        static void initialize(std::string memcache_hostname, in_port_t memcache_port, unsigned int update_interval_in_milliseconds, std::vector<unsigned int> vehicle_ids);
        mc_vehicle & vehicle_info(unsigned int vehicle_id);
        bool is_in_communication_range(unsigned int vehicle_id_1, unsigned int vehicle_id_2, float communication_range);
        std::unordered_set<unsigned int> all_vehicles_in_communication_range(unsigned int vehicle_id, float communication_range);
        float distance(unsigned int vehicle_id_1, unsigned int vehicle_id_2);
};


#endif
