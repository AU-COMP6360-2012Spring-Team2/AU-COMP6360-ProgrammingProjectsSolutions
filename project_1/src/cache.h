#ifndef CLASS_cache
#define CLASS_cache

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <mutex>
#include <ctime>

#include "message.h"

using namespace std;

class cache {
    private:
        typedef std::pair<unsigned int, unsigned int> _type_vehicle_id_packet_id_pair;
        class _hasher_type_vehicle_id_packet_id_pair {
            public:
                size_t operator()(const _type_vehicle_id_packet_id_pair & s ) const {
                    std::hash<unsigned int> hasher;
                    return hasher(s.first) + hasher(s.second);
                }
        };
        typedef unordered_set<_type_vehicle_id_packet_id_pair, _hasher_type_vehicle_id_packet_id_pair> _type_eebl_hash_set;
        typedef unordered_map<unsigned int, message * > _type_message_hash_map;

        // cache for messages and eebl ids. using hash to ensure fast access
        _type_eebl_hash_set _eebl_cache;
        _type_message_hash_map _message_cache;

        // two ordered multimap's with keys being timestamp, used as index for fast removing outdated items
        multimap<time_t, _type_eebl_hash_set::iterator> _eebl_cache_index;
        multimap<time_t, message * > _message_cache_index;

        void _remove_outdated_messages();
        void _remove_outdated_eebl();

        mutex _mutex_message_cache;
        mutex _mutex_eebl_cache;

    public:
        // TODO: probably should read from configuration file.
        static const unsigned int CACHED_MESSAGE_LIVE_TIME = 8;
        static const unsigned int CACHED_EEBL_ID_LIVE_TIME = 4;

        cache();
        void new_message(message * msg);
        message * get_latest_message_from(unsigned int vehicle_id);
        bool has_recently_seen_eebl_of(unsigned int vehicle_id, unsigned int packet_id);
};

#endif
