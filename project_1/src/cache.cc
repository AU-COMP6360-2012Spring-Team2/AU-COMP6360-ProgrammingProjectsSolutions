#include "cache.h"

cache::cache() {
    this->_message_counter = 0;
}

void cache::new_message(message * msg){
    this->_message_cache[msg->originator_id()] = msg;
    if(msg->is_EEBL())
        this->_eebl_id_cache.insert(_type_eebl_cache_pair(msg->timestamp(), _type_vehicle_id_packet_id_pair(msg->originator_id(), msg->packet_id())));

    if( (++this->_message_counter) >= cache::OUTDATED_ITEM_CHECK_INTERVAL ) {
        this->_message_counter = 0;
        this->_drop_outdated_items();
    }
}

void cache::_drop_outdated_items() {
    // TODO: 
}
