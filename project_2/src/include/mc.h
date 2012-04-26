#ifndef H_mc
#define H_mc

#include <libmemcached/memcached.hpp>

class mc {
    private:
        static memcache::Memcache * m;
        mc () { }
        mc operator = (const mc & m) { throw ; }
        mc ( const mc & m ) { throw ; }
        ~ mc () {
            delete m;
        }
    public:
        static mc & get () {
            static mc instance;
            if(!m)
                throw ;
            return instance;
        }
        static void initialize( std::string & hostname, in_port_t port ) {
            m = new memcache::Memcache ( hostname, port );
        }
        void set ( std::string & key, std::vector<char> & value ) {
            time_t expiry = 0;
            uint32_t flags = 0;
            m->set(key, value, expiry, flags);
        }
        void set_all ( std::vector<std::string> & keys, std::vector<std::vector<char> > & values) {
            time_t expiry = 0;
            uint32_t flags = 0;
            std::vector<std::vector<char> * > v;
            for(auto i = values.begin(); i != values.end(); ++i)
                v.push_back(&(*i));
            m->setAll( keys, v, expiry, flags );
        }
        void get ( std::string & key, std::vector<char> & value ) {
            m->get(key, value);
        }
        std::vector<char> get ( const std::string & key ) {
            std::vector<char> r;
            m->get(key, r);
            return r;
        }
};

#endif
