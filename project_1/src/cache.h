#ifndef CLASS_cache
#define CLASS_cache

#include <map>
#include <set>
#include <ctime>

#include "message.h"

using namespace std;

class cache{
    private:
        typedef std::pair<unsigned int, unsigned int> _type_vehicle_id_packet_id_pair;
        typedef std::pair<time_t, _type_vehicle_id_packet_id_pair> _type_eebl_cache_pair;
        struct _type_eebl_cache_comparer {
            bool operator () (const _type_eebl_cache_pair & lhs, const _type_eebl_cache_pair & rhs) const {
                return lhs.first < rhs.first;
            }
        };

        // dynamically updated map consisting messages from each vehicle. outdated vehicles will be removed.
        map<unsigned int, message * > _message_cache;

        // dynamically updated set for EEBL IDs. outdated ones will be removed.
        set<_type_eebl_cache_pair, _type_eebl_cache_comparer> _eebl_id_cache;

        unsigned int _message_counter;
        void _drop_outdated_items();

    public:
        static const unsigned int OUTDATED_ITEM_CHECK_INTERVAL = 1024;

        cache();
        void new_message(message * msg);
};


#endif
