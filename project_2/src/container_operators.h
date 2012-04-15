#ifndef _H_container_operators
#define _H_container_operators

#include <unordered_set>

#define _USET_TEMP template<class Key, class Hash, class KeyEqual, class Allocator>
#define _USET_TYPE std::unordered_set<Key, Hash, KeyEqual, Allocator>

_USET_TEMP
void operator += ( _USET_TYPE & lo, const Key & ro ) {
    lo.insert(ro);
}

_USET_TEMP
void operator += ( _USET_TYPE & lo, const _USET_TYPE & ro ) {
    for(auto i = ro.begin(); i != ro.end(); ++i)
        lo += *i;
}

_USET_TEMP
void  operator -= ( _USET_TYPE & lo, const Key & ro ){
    lo.erase(ro);
}

_USET_TEMP
void  operator -= ( _USET_TYPE & lo, const _USET_TYPE & ro ){
    for(auto i = ro.begin(); i != ro.end(); ++i)
        lo -= *i;
}

_USET_TEMP
_USET_TYPE operator +  ( const _USET_TYPE & lo, const _USET_TYPE & ro ) {
    _USET_TYPE r;
    for(auto i = lo.begin(); i != lo.end(); ++i)
        r.insert(*i);
    for(auto i = ro.begin(); i != ro.end(); ++i)
        r.insert(*i);
    return r;
}

_USET_TEMP
_USET_TYPE operator -  ( const _USET_TYPE & lo, const _USET_TYPE & ro ) {
    _USET_TYPE r;
    for(auto i = lo.begin(); i != lo.end(); ++i)
        if(ro.end() == ro.find(*i))
            r.insert(*i);
    return r;
}

#endif
