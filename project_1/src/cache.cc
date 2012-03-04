#include "cache.h"

cache::cache(){
}

void cache::new_message(message * msg){
    // update message cache
    this->_message_cache[msg->originator_id()] = msg;
    this->_message_cache_index.insert(pair<time_t, message * >(time(NULL), msg));

    // update eebl cache
    this->_eebl_cache_index.insert(pair<time_t, _type_eebl_hash_set::iterator>( time(NULL),
          this->_eebl_cache.insert(_type_vehicle_id_packet_id_pair(msg->originator_id(), msg->packet_id())).first
        ));

    // remove outdated items.
    this->_remove_outdated_messages();
    this->_remove_outdated_eebl();
}

void cache::_remove_outdated_messages() {
    time_t now = time(NULL);
    auto i = this->_message_cache_index.begin();
    for( ; cache::CACHED_MESSAGE_LIVE_TIME < now - i->first; ++i) {
        if(this->_message_cache[i->second->originator_id()] == i->second) // if the message is the one stored in the cache, meaning that theitem should be removed from cache as well
            this->_message_cache.erase(i->second->originator_id());
        delete i->second; // here is the exclusive place to destroy the message memory space
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

message * cache::get_latest_message_from(unsigned int vehicle_id){
    // remove outdated messages if necessary
    this->_remove_outdated_messages();

    // return NULL or pointer to required message
    auto r = this->_message_cache.find(vehicle_id);
    if(this->_message_cache.end() == r)
        return NULL;
    else
        return r->second;
}

bool cache::has_recently_seen_eebl_of(unsigned int vehicle_id, unsigned int packet_id) {
    // remove outdated eebl if necessary
    this->_remove_outdated_eebl();

    return this->_eebl_cache.end() != this->_eebl_cache.find(_type_vehicle_id_packet_id_pair(vehicle_id, packet_id));
}

