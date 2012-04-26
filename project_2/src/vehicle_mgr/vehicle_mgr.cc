#include "vehicle_mgr.h"

#include <random>
#include <sys/time.h>
#include <cmath>
#include <algorithm>
#include <cstdlib>

const unsigned short vehicle_mgr::_SPEED_CM_PER_SECOND_MIN = 1000;
const unsigned short vehicle_mgr::_SPEED_CM_PER_SECOND_MAX = 2000;

vehicle_mgr::vehicle_mgr ( mc * m, unsigned int update_interval_in_milliseconds, std::vector<float> & grid_points_x, std::vector<float> & grid_points_y, std::unordered_map<unsigned int, location> & initial_locations, unsigned short mean_speed, double prob_turn ) {
    this->_m = m;
    if(prob_turn > 1 && prob_turn < 0) throw ;
    this->_prob_turn = prob_turn;
    this->_grid_points_x = grid_points_x;
    this->_grid_points_y = grid_points_y;
    this->_update_interval_in_milliseconds = update_interval_in_milliseconds;

    { // initialize vehicles
        std::ranlux64_base_01 eng;
        std::normal_distribution<double> speed_generator(mean_speed, 2.2);
        std::uniform_int<int> direction_generator(1, 2);
        for(auto i = initial_locations.begin(), counter = 0; i != initial_locations.end(); ++i, ++counter) {
            mc_vehicle v;

            v._x = i->second.x;
            v._y = i->second.y;
            v._z = i->second.z;

            // determin whether vehicle is on 'rows' or 'columns'
            bool is_on_rows;
            bool is_on_columns;
            if(this->_grid_points_x.end() == std::find(this->_grid_points_x.begin(), this->_grid_points_x.end(), v._x))
                is_on_columns = true;
            else if(this->_grid_points_y.end() == std::find(this->_grid_points_y.begin(), this->_grid_points_y.end(), v._y))
                is_on_rows = true;
            else 
                throw ;

            // generate direction
            if (is_on_rows) {
                if(1 == direction_generator(eng))
                    v._direction = mc_vehicle::E;
                else
                    v._direction = mc_vehicle::W;
            }
            else {
                if(1 == direction_generator(eng))
                    v._direction = mc_vehicle::N;
                else
                    v._direction = mc_vehicle::S;
            }

            // generate speed
            v._speed = (unsigned short) std::round(speed_generator(eng));
            v._speed = std::min(v._speed, _SPEED_CM_PER_SECOND_MAX);
            v._speed = std::max(v._speed, _SPEED_CM_PER_SECOND_MIN);

            v._acceleration = 0;

            this->_vehicles[mc_vehicle::vehicleID2mcID((unsigned int)i->first)] = v;
        }
    }

}

bool vehicle_mgr::_find_grid_point(float current, float change, bool is_x, float * first_point) {
    std::vector<float> & _grid_points = is_x ? this->_grid_points_x : this->_grid_points_y;
    float lower = std::min(current, current + change);
    float upper = std::max(current, current + change);
    std::vector<float> grid_points_found;
    bool r = false;
    for(auto i = _grid_points.begin(); i != _grid_points.end(); ++i)
        if(lower <= *i && *i <= upper) {
            r = true;
            if(first_point)
                grid_points_found.push_back(*i);
            else
                return true;
        }
    float first;
    if(first_point && !grid_points_found.empty()){
        first = grid_points_found[0];
        for(auto i = grid_points_found.begin(); i != grid_points_found.end(); ++i)
            if(abs(*i-current) < abs(first-current))
                first = *i;
        *first_point = first;
    }
    return r;
}

void vehicle_mgr::run() {
    while(true) {
        { // send update to memcached
            std::vector<std::string> keys;
            std::vector<std::vector<char>> values;
            for(auto i = this->_vehicles.begin(); i != this->_vehicles.end(); ++i) {
                keys.push_back(i->first);
                std::vector<char> value;
                i->second.to_memcache(value);
                values.push_back(value);
            }
            this->_m->set_all(keys, values);
            usleep(1000 * this->_update_interval_in_milliseconds);
        }

        { // update vehicles
            for(auto v = this->_vehicles.begin(); v != this->_vehicles.end(); ++v) {

                // compute change
                float x_change, y_change;
                x_change =((v->second._direction >> 1) != 1 ? 0 : // whether the direction is E-W
                        (- ((v->second._direction & 0b1) * 2 - 1 ) ) * // determine whether it's E or W
                        (v->second._speed * this->_update_interval_in_milliseconds / 1000.0 / 100.0) // convert to meters
                        );
                y_change = ((v->second._direction >> 1) != 0 ? 0 : // whether the direction is N-S
                        (- ((v->second._direction & 0b1) * 2 - 1 ) ) * // determine whether it's N or S
                        (v->second._speed * this->_update_interval_in_milliseconds / 1000.0 / 100.0) // convert to meters
                        );

                if(this->_find_grid_point(v->second._x, x_change, true) || this->_find_grid_point(v->second._y, y_change, false) ) { // intersection on the way
                    static std::ranlux64_base_01 eng;
                    static std::uniform_real<double> turn_dist;
                    double turn_p = turn_dist(eng);
                    if(turn_p > this->_prob_turn) { // no need to turn
                        v->second._x = v->second._x + x_change;
                        v->second._y = v->second._y + y_change;
                        { // check if out of boundary
                            if(v->second._x > this->_grid_points_x.back())
                                v->second._x = v->second._x - (this->_grid_points_x.back() - this->_grid_points_x.front());
                            else if(v->second._x < this->_grid_points_x.front())
                                v->second._x = v->second._x + (this->_grid_points_x.back() - this->_grid_points_x.front());
                            if(v->second._y > this->_grid_points_y.back())
                                v->second._y = v->second._y - (this->_grid_points_y.back() - this->_grid_points_y.front());
                            else if(v->second._y < this->_grid_points_y.front())
                                v->second._y = v->second._y + (this->_grid_points_y.back() - this->_grid_points_y.front());
                        }
                    }
                    else { // need to turn
                        float first;
                        if(v->second._direction >> 1 == 1) { // on E-W
                            this->_find_grid_point(v->second._x, x_change, true, &first);
                            v->second._x = first;
                            v->second._direction ^= mc_vehicle::TURN;
                            if(turn_p < this->_prob_turn / 2)
                                v->second._direction ^= mc_vehicle::U_TURN;
                        }
                        else { //on N-S
                            this->_find_grid_point(v->second._y, y_change, false, &first);
                            v->second._y = first;
                            v->second._direction ^= mc_vehicle::TURN;
                            if(turn_p < this->_prob_turn / 2)
                                v->second._direction ^= mc_vehicle::U_TURN;
                        }
                    }
                }
                else {
                    v->second._x += x_change;
                    v->second._y += y_change;
                }
            }
        }
    }
}
