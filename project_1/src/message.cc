#include <cstring>
#include <iostream>
#include "message.h"

using namespace std;

message * message::create_beacon(const float & gps_coordinate_x, const float & gps_coordinate_y, const float & gps_coordinate_z, const unsigned short & speed, const short & acceleration, const vehicle & vehicle) {
    message * m = new message;

    m->_type = message::TYPE_BEACON;
    m->_packet_id = 0; // !! TODO: increamental packet_id. range of each vehicle should be specified in configuration file. probably use singleton as an id generator.
    m->_originator_id = vehicle.id();
    m->_ttl = 0;
    m->_sender_id = vehicle.id();
    m->_gps_coordinate_x = gps_coordinate_x;
    m->_gps_coordinate_y = gps_coordinate_y;
    m->_gps_coordinate_z = gps_coordinate_z;
    m->_timestamp = time(NULL);
    m->_speed = speed;
    m->_acceleration = acceleration;
    m->_heading = vehicle.heading();
    m->_size_length = vehicle.size_length();
    m->_size_width = vehicle.size_width();
    m->_size_height = vehicle.size_height();

    return m;
}

message * message::create_EEBL(const float & gps_coordinate_x, const float & gps_coordinate_y, const float & gps_coordinate_z, const unsigned short & speed, const short & deceleration, const vehicle * vehicle) {
    message * m = new message;

    m->_type = message::TYPE_EEBL;
    m->_packet_id = 0; // !! TODO: increamental packet_id. range of each vehicle should be specified in configuration file. probably use singleton as an id generator.
    m->_originator_id = vehicle->id();
    m->_ttl = message::EEBL_TTL;
    m->_sender_id = vehicle->id();
    m->_gps_coordinate_x = gps_coordinate_x;
    m->_gps_coordinate_y = gps_coordinate_y;
    m->_gps_coordinate_z = gps_coordinate_z;
    m->_timestamp = time(NULL);
    m->_speed = speed;
    m->_acceleration = -deceleration;
    m->_heading = vehicle->heading();
    m->_size_length = vehicle->size_length();
    m->_size_width = vehicle->size_width();
    m->_size_height = vehicle->size_height();

    return m;
}

message * message::create_rebroadcasted_EEBL(const message * original_EEBL, const vehicle * rebroadcasting_vehicle) {
    message * m = new message;

    m->_type = message::TYPE_EEBL;
    m->_packet_id = 0; // !! TODO: increamental packet_id. range of each vehicle should be specified in configuration file. probably use singleton as an id generator.
    m->_originator_id = original_EEBL->originator_id();
    m->_ttl = original_EEBL->ttl() - 1;
    m->_sender_id = rebroadcasting_vehicle->id();
    m->_gps_coordinate_x = original_EEBL->_gps_coordinate_x;
    m->_gps_coordinate_y = original_EEBL->_gps_coordinate_y;
    m->_gps_coordinate_z = original_EEBL->_gps_coordinate_z;
    m->_timestamp = time(NULL);
    m->_speed = original_EEBL->speed();
    m->_acceleration = original_EEBL->acceleration();
    m->_heading = original_EEBL->heading();
    m->_size_length = original_EEBL->size_length();
    m->_size_width = original_EEBL->size_width();
    m->_size_height = original_EEBL->size_height();

    return m;
}

message::message() { }

message * message::from_bytes(const unsigned char data[]) {
    if (sizeof(data) != message::MESSAGE_SIZE) {
        cerr<<"error[message::from_bytes] Destination data[] size is not properly set. Please use message::MESSAGE_SIZE contant.";
        throw;
    }

    message * m = new message;
    int p = 0;
    #define GET(V,SIZE) { memcpy(&(V), data + p, SIZE); p += SIZE; }
    GET( m->_type             , 1  );
    GET( m->_packet_id        , 4  );
    GET( m->_originator_id    , 4  );
    GET( m->_ttl              , 1  );
    GET( m->_sender_id        , 4  );
    GET( m->_gps_coordinate_x , 4  );
    GET( m->_gps_coordinate_y , 4  );
    GET( m->_gps_coordinate_z , 4  );
    GET( m->_timestamp        , 8  );
    GET( m->_speed            , 2  );
    GET( m->_acceleration     , 2  );
    GET( m->_heading          , 2  );
    GET( m->_size_length      , 2  );
    GET( m->_size_width       , 2  );
    GET( m->_size_height      , 2  );
    
    return m;
}

void message::to_bytes(unsigned char buffer[]) const {
    if (sizeof(buffer) != message::MESSAGE_SIZE) {
       cerr<<"error[message::to_bytes] Destination buffer[] size is not properly set. Please use message::MESSAGE_SIZE contant.";
       throw;
    }

    int p = 0;
    #define PUT(V,SIZE) { memcpy(buffer + p, &(V), SIZE); p += SIZE; }
    PUT( this->_type             , 1  );
    PUT( this->_packet_id        , 4  );
    PUT( this->_originator_id    , 4  );
    PUT( this->_ttl              , 1  );
    PUT( this->_sender_id        , 4  );
    PUT( this->_gps_coordinate_x , 4  );
    PUT( this->_gps_coordinate_y , 4  );
    PUT( this->_gps_coordinate_z , 4  );
    PUT( this->_timestamp        , 8  );
    PUT( this->_speed            , 2  );
    PUT( this->_acceleration     , 2  );
    PUT( this->_heading          , 2  );
    PUT( this->_size_length      , 2  );
    PUT( this->_size_width       , 2  );
    PUT( this->_size_height      , 2  );
} 


unsigned char message::type() const { return this->_type; }
bool message::is_beacon() const { return ( this->_type == message::TYPE_BEACON ) ; }
bool message::is_EEBL() const { return ( this->_type == message::TYPE_EEBL ) ; }
unsigned int message::packet_id() const { return this->_packet_id; }
unsigned int message::originator_id() const { return this->_originator_id; }
unsigned char message::ttl() const { return this->_ttl; }
unsigned int message::sender_id() const { return this->_sender_id; }
float message::gps_coordinate_x() const { return this->_gps_coordinate_x; }
float message::gps_coordinate_y() const { return this->_gps_coordinate_y; }
float message::gps_coordinate_z() const { return this->_gps_coordinate_z; }
time_t message::timestamp() const { return this->_timestamp; }
unsigned short message::speed() const { return this->_speed; }
short message::acceleration() const { return this->_acceleration; }
short message::deceleration() const { return -this->_acceleration; }
unsigned short message::heading() const { return this->_heading; }
unsigned short message::size_length() const { return this->_size_length; }
unsigned short message::size_width() const { return this->_size_width; }
unsigned short message::size_height() const { return this->_size_height; }

vehicle message::get_originator() const {
    vehicle r(this->_originator_id, this->_heading, this->_size_length, this->_size_width, this->_size_height);
    return r;
}
