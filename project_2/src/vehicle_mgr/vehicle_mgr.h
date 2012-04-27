#ifndef H_vehicle_mgr
#define H_vehicle_mgr

#include <unordered_map>

#include "../include/mc_vehicle.h"

class vehicle_mgr {
    private:
        std::string _shared_location_file;
        unsigned int _update_interval_in_milliseconds;
        double _prob_turn;
        std::vector<float> _grid_points_x;
        std::vector<float> _grid_points_y;
        std::unordered_map<std::string, mc_vehicle> _vehicles;
        bool _find_grid_point(float current, float change, bool is_x, float * first_point = NULL);
        void _correct_out_of_boundary(mc_vehicle & v);

    public:
        static unsigned short _SPEED_CM_PER_SECOND_MIN;
        static unsigned short _SPEED_CM_PER_SECOND_MAX;
        struct location {
            float x, y, z;
            location(float _x, float _y, float _z) { x=_x; y=_y; z=_z; }
            location(){};
        };
        vehicle_mgr ( std::string shared_location_file, unsigned int update_interval_in_milliseconds, std::vector<float> & grid_points_x, std::vector<float> & grid_points_y, std::unordered_map<unsigned int, location> & initial_locations, unsigned short mean_speed, double prob_turn ) ;
        void run();
};

#endif
