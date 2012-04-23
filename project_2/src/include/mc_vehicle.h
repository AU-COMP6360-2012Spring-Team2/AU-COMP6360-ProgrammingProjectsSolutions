#ifndef H_mc_vehicle
#define H_mc_vehicle

#include <vector>
#include <sstrem>
#include <cstring>

class mc_vehicle {
    private:
        friend class vehicle_mgr;
        mc_vehicle () { }
        static const unsigned int MC_VALUE_SIZE = 16;
        float _x, _y, _z;
        unsigned short _speed;
        short _acceleration;

        static const unsigned int N = 0b00;
        static const unsigned int E = 0b10;
        static const unsigned int S = 0b01;
        static const unsigned int W = 0b11;
        static const unsigned int U_TURN = 0b01;
        static const unsigned int TURN = 0b10;
        unsigned int _direction;

        void update_from_memcache(const std::vector<char> & value) {
            int p = 0;
#define GET(V,SIZE) { memcpy( &(V), &value[0] + p, SIZE); p += SIZE; }
            GET( this->_x               , 4 );
            GET( this->_y               , 4 );
            GET( this->_z               , 4 );
            GET( this->_speed           , 2 );
            GET( this->_acceleration    , 2 );
#undef GET
        }

        void to_memcache(std::vector<char> & value) {
            int p = 0;
            value.resize( MC_VALUE_SIZE, '?' );
#define SET(V,SIZE) { memcpy( &value[0] + p, &(V), SIZE); p += SIZE; }
            SET( this->_x               , 4 );
            SET( this->_y               , 4 );
            SET( this->_z               , 4 );
            SET( this->_speed           , 2 );
            SET( this->_acceleration    , 2 );
#undef SET
        }

        static unsigned int mcID2vehicleID(std::string & id) {
            if(!id.find_first_of("VEHICLE:"))
                throw;
            unsigned int r;
            std::istringstream convert(id);
            convert >> r;
            return r;
        }

        static std::string vehicleID2mcID(unsigned int & id) {
            std::ostringstream convert;
            convert<<"VEHICLE:"<<id;
            return convert.str();
        }

    public:
        float x() { return this->_x; }
        float y() { return this->_y; }
        float z() { return this->_z; }
        unsigned short speed_cm_per_second() { return this->_speed; }
        short acceleration_cm_per_squared_second() { return this->_acceleration; }
};

#endif
