#include "cache.h"

cache::cache(){
    this->_last_removing_outdated_messages_timestamp = time(NULL);
    this->_last_removing_outdated_eebl_timestamp = time(NULL);
}

void cache::new_message(message * msg){
    this->_message_cache[msg->originator_id()] = msg;
    this->_message_cache_index.insert(pair<time_t, message * >(time(NULL), msg));
    this->_eebl_cache_index.insert(pair<time_t, _type_eebl_hash_set::iterator>( time(NULL),
          this->_eebl_cache.insert(_type_vehicle_id_packet_id_pair(msg->originator_id(), msg->packet_id())).first
        ));

    time_t now = time(NULL);
    if(cache::CACHED_MESSAGE_LIVE_TIME < now - this->_last_removing_outdated_messages_timestamp)
        this->_remove_outdated_messages();
    if(cache::CACHED_EEBL_ID_LIVE_TIME < now - this->_last_removing_outdated_eebl_timestamp)
        this->_remove_outdated_eebl();
}

void cache::_remove_outdated_messages() {
    time_t now = time(NULL);
    auto i = this->_message_cache_index.begin();
    for( ; cache::CACHED_MESSAGE_LIVE_TIME < now - i->first; ++i) {
        if(this->_message_cache[i->second->originator_id()] == i->second)
            this->_message_cache.erase(i->second->originator_id());
        delete i->second;
    }
    this->_message_cache_index.erase(this->_message_cache_index.begin(), i);
}

void cache::_remove_outdated_eebl() {
    time_t now = time(NULL);
    auto i = this->_eebl_cache_index.begin();
    for( ; cache::CACHED_EEBL_ID_LIVE_TIME < now - i->first; ++i)
        this->_eebl_cache.erase(i->second);
    this->_eebl_cache_index.erase(this->_eebl_cache_index.begin(), i);
}

bool cache::has_recently_seen_eebl_of(unsigned int vehicle_id, unsigned int packet_id) {
    return this->_eebl_cache.end() != this->_eebl_cache.find(_type_vehicle_id_packet_id_pair(vehicle_id, packet_id));
}

