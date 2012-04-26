#include "vehicle.h"

vehicle::vehicle(unsigned int id, unsigned short heading, unsigned short size_length, unsigned short size_width, unsigned short size_height){
    this->_id = id;
    this->_heading = heading;
    this->_size_length = size_length;
    this->_size_width = size_width;
    this->_size_height = size_height;
}

unsigned int vehicle::id() const { return this->_id; }
unsigned short vehicle::heading() const { return this->_heading; }
unsigned short vehicle::size_length() const { return this->_size_length; }
unsigned short vehicle::size_width() const { return this->_size_width; }
unsigned short vehicle::size_height() const { return this->_size_height; }
