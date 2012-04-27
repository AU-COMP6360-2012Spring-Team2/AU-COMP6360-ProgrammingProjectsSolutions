#ifndef H_physical_world
#define H_physical_world

#include <mutex>
#include <unordered_map>
#include <thread>
#include <unordered_set>

#include "../include/mc_vehicle.h"

#include <iostream>

class physical_world {
    private:
        std::string _shared_config_file;
        std::thread * _update_thread;
        static physical_world * _instance;
        unsigned int _update_interval_in_milliseconds;
        unsigned int _me_id;

        std::mutex _mutex_vehicles;
        std::unordered_map<std::string, mc_vehicle> _vehicles;

        physical_world () { }
        physical_world ( const physical_world & ro ) { }
        physical_world operator= ( const physical_world & ro ) { std::cerr<<"= called, is gonna ghrow"; throw; }

        void _update_from_memcache();
        void _update_thread_fun();

    public:
        static physical_world & get () {
            if(!_instance) {
                std::cerr<<"physical_world instance not initialized."<<std::endl;
                throw;
            }
            return *_instance;
        }
        ~physical_world() {
            delete _update_thread;
        }

        static void initialize(std::string shared_config_file, unsigned int update_interval_in_milliseconds, std::vector<unsigned int> vehicle_ids, unsigned int me_id);
        mc_vehicle & vehicle_info(unsigned int vehicle_id);
        bool is_in_communication_range(unsigned int vehicle_id_1, unsigned int vehicle_id_2, float communication_range);
        std::unordered_set<unsigned int> all_vehicles_in_communication_range(unsigned int vehicle_id, float communication_range);
        float distance(unsigned int vehicle_id_1, unsigned int vehicle_id_2);
};


#endif
