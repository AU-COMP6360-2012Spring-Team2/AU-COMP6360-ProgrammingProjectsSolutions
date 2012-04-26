#ifndef H_vehicle_mgr
#define H_vehicle_mgr

#include <unordered_map>

#include "../include/mc.h"
#include "../include/mc_vehicle.h"

class vehicle_mgr {
    private:
        mc * _m;
        unsigned int _update_interval_in_milliseconds;
        double _prob_turn;
        std::vector<float> _grid_points_x;
        std::vector<float> _grid_points_y;
        std::unordered_map<std::string, mc_vehicle> _vehicles;
        bool _find_grid_point(float current, float change, bool is_x, float * first_point = NULL);

    public:
        static const unsigned short _SPEED_CM_PER_SECOND_MIN;
        static const unsigned short _SPEED_CM_PER_SECOND_MAX;
        struct location {
            float x, y, z;
            location(float _x, float _y, float _z) { x=_x; y=_y; z=_z; }
            location(){};
        };
        vehicle_mgr ( mc * m, unsigned int update_interval_in_milliseconds, std::vector<float> & grid_points_x, std::vector<float> & grid_points_y, std::unordered_map<std::string, location> & initial_locations, unsigned short mean_speed, double prob_turn ) ;
        void run();
};

#endif
