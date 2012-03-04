#ifndef CLASS_cache
#define CLASS_cache

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <ctime>

#include "message.h"

using namespace std;

class cache {
    private:
        typedef std::pair<unsigned int, unsigned int> _type_vehicle_id_packet_id_pair;
        typedef unordered_set<_type_vehicle_id_packet_id_pair> _type_eebl_hash_set;
        typedef unordered_map<unsigned int, message * > _type_message_hash_map;

        _type_eebl_hash_set _eebl_cache;
        _type_message_hash_map _message_cache;

        multimap<time_t, _type_eebl_hash_set::iterator> _eebl_cache_index;
        multimap<time_t, message * > _message_cache_index;

        time_t _last_removing_outdated_messages_timestamp;
        time_t _last_removing_outdated_eebl_timestamp;
        void _remove_outdated_messages();
        void _remove_outdated_eebl();

    public:
        static const unsigned int CACHED_MESSAGE_LIVE_TIME = 8;
        static const unsigned int CACHED_EEBL_ID_LIVE_TIME = 32;

        cache();
        void new_message(message * msg);
        message * get_latest_message_from(unsigned int vehicle_id);
        bool has_recently_seen_eebl_of(unsigned int vehicle_id, unsigned int packet_id);
};

#endif
