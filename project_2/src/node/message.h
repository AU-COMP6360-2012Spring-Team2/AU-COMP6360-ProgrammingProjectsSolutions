#ifndef CLASS_message
#define CLASS_message

#include <ctime>

#include "vehicle.h"

class message {
    private:

        static unsigned int _packet_id_counter;
        static unsigned int _get_next_packet_id();

        // 1 byte Type
        unsigned char _type;

        // 4 bytes packet id
        unsigned int _packet_id;

        // 4 bytes originator id
        unsigned int _originator_id;

        // 1 byte TTL
        unsigned char _ttl;

        // 4 bytes sender id;
        unsigned int _sender_id;

        // 12 bytes GPS coordinates
        float _gps_coordinate_x;
        float _gps_coordinate_y;
        float _gps_coordinate_z;

        // 8 bytes Timestamp
        time_t _timestamp;

        // 2 bytes Vehicle speed
        unsigned short _speed;

        // 2 bytes Vehicle acceleration/decceleration
        short _acceleration;

        // 2 bytes Vehicle heading
        unsigned short _heading;

        // 6 bytes Vehicle size
        unsigned short _size_length;
        unsigned short _size_width;
        unsigned short _size_height;
        
        //disable default constructor
        message();

    public:
        // constants
        static const unsigned int MESSAGE_SIZE = 46;
        static const unsigned char EEBL_TTL = 5;
        static const unsigned char TYPE_BEACON = 0;
        static const unsigned char TYPE_EEBL = 1;

        // builders
        static message * create_beacon(const float & gps_coordinate_x, const float & gps_coordinate_y, const float & gps_coordinate_z, const unsigned short & speed, const short & acceleration);
        static message * create_EEBL(const float & gps_coordinate_x, const float & gps_coordinate_y, const float & gps_coordinate_z, const unsigned short & speed, const short & deceleration);
        static message * create_rebroadcasted_EEBL(const message * original_EEBL);

        // constructing from serialized data received from network
        static message * from_bytes(const unsigned char data[]);

        // serialize message into data transmittable through socket
        void to_bytes(unsigned char buffer[]) const; // buffer size should be at lease 46 bytes.


        // fields getters
        unsigned char type() const;               // 1 byte Type
        bool is_beacon() const;
        bool is_EEBL() const;
        unsigned int packet_id() const;           // 4 bytes packet id
        unsigned int originator_id() const;       // 4 bytes originator id
        unsigned char ttl() const;                // 1 byte TTL
        unsigned int sender_id() const;           // 4 bytes sender id;
        float gps_coordinate_x() const;    // 12 bytes GPS coordinates - x
        float gps_coordinate_y() const;    // 12 bytes GPS coordinates - y
        float gps_coordinate_z() const;    // 12 bytes GPS coordinates - z
        time_t timestamp() const;                 // 8 bytes Timestamp
        unsigned short speed() const;             // 2 bytes Vehicle speed
        short acceleration() const;               // 2 bytes Vehicle acceleration
        short deceleration() const;               // 2 bytes Vehicle decceleration
        unsigned short heading() const;           // 2 bytes Vehicle heading
        unsigned short size_length() const;       // 6 bytes Vehicle size - length
        unsigned short size_width() const;        // 6 bytes Vehicle size - width
        unsigned short size_height() const;       // 6 bytes Vehicle size - height
        vehicle get_originator() const;         // get vehicle object representing originator
        
};

#endif
