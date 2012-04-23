#ifndef H_physical_world
#define H_physical_world

#include "../include/mc_vehicle.h"
#include "../include/mc.h"

class physical_world {
    private:
        physical_world () { }
        physical_world ( const physical_world & ro ) { }
        physical_world operator= ( const physical_world & ro ) { }

    public:
        static physical_world & get () {
            static physical_world instalce;
            return instance;
        }
        mc_vehicle & vehicle_info(unsigned int vehicle_id);
        bool is_in_communication_range(unsigned int vehicle_id_1, unsigned int vehicle_id_2, float communication_range);
        float distance(unsigned int vehicle_id_1, unsigned int vehicle_id_2);
};


#endif
