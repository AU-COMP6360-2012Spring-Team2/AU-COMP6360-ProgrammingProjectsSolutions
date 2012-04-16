/*
 *  This headers file defines operators for stl containers.
 *
 *  Operators 
 *      + - += -=
 *  are supported in following containers:
 *      std::unordered_set
 *      std::unordered_map
 *      std::set
 *      std::map
 *
 *
 *  Notes:
 *
 *  1. + and - operators for std::unordered_set and std::set
 *  support both container type and value type for right
 *  operand, while std::unordered_map and std::map only support
 *  container type as right operand.
 *
 *  2.1 Operators on std::unordered_set std::set are quite
 *  straightforward. They are simply collection operations,
 *  and do NOT change actual values in the containers,
 *  i.e., insertion, removal, merge.
 *
 *  2.2 For std::unordered_map and std::map, when merging (+, +=), 
 *  these operators checks if the element to be inserted exists
 *  in the container. If existing, a plus operation ( + ) is
 *  applied to the two values with the same key, and the new
 *  value replace the old one. For example:
 *      std::map<int, int> m1;
 *      std::map<int, int> m2;
 *      m1[1] = 10;
 *      m1[2] = 20;
 *      m2[2] = 2;
 *      auto m3 = m1 + m2; // m3 == { 1->10, 2->22 }
 *
 *      // Similar for container value type:
 *      set<int> s1;
 *      set<int> s2;
 *      set<int> s3;
 *      s1 += 10;
 *      s1 += 11;
 *      s3 += 20;
 *      s3 += 21;
 *      s2 += 30;
 *      map<int,set<int>> m1;
 *      map<int,set<int>> m2;
 *
 *      m1[1] = s1;
 *      m1[2] = s2;
 *      m2[1] = s3;
 *  
 *      m3 = m1 + m2;
 *      // m3 == { 1->[10,11,20,21] 2->[30] }
 */

#ifndef _H_container_operators
#define _H_container_operators

#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>

#define SET_OPERATORS(TEMP, TYPE)\
TEMP \
void operator += ( TYPE & lo, const Key & ro ) {\
    lo.insert(ro);\
}\
\
TEMP \
void operator += ( TYPE & lo, const TYPE & ro ) {\
    for(auto i = ro.begin(); i != ro.end(); ++i)\
        lo += *i;\
}\
\
TEMP \
void  operator -= ( TYPE & lo, const Key & ro ){\
    lo.erase(ro);\
}\
\
TEMP \
void  operator -= ( TYPE & lo, const TYPE & ro ){\
    for(auto i = ro.begin(); i != ro.end(); ++i)\
        lo -= *i;\
}\
\
TEMP \
TYPE operator +  ( const TYPE & lo, const TYPE & ro ) {\
    TYPE r;\
    for(auto i = lo.begin(); i != lo.end(); ++i)\
        r.insert(*i);\
    for(auto i = ro.begin(); i != ro.end(); ++i)\
        r.insert(*i);\
    return r;\
}\
\
TEMP \
TYPE operator -  ( const TYPE & lo, const TYPE & ro ) {\
    TYPE r;\
    for(auto i = lo.begin(); i != lo.end(); ++i)\
        if(ro.end() == ro.find(*i))\
            r.insert(*i);\
    return r;\
}


#define MAP_OPERATORS(TEMP, TYPE) \
TEMP \
void operator += ( TYPE & lo, const TYPE & ro ) { \
    for(auto i = ro.begin(); i != ro.end(); ++i) \
        if(lo.end() != lo.find(i->first)) \
            lo[i->first] += i->second; \
        else \
            lo[i->first] = i->second; \
} \
 \
TEMP \
void  operator -= ( TYPE & lo, const TYPE & ro ){ \
    for(auto i = ro.begin(); i != ro.end(); ++i) \
        lo.erase(i->first); \
} \
 \
TEMP \
TYPE operator +  ( const TYPE & lo, const TYPE & ro ) { \
    TYPE r; \
    for(auto i = lo.begin(); i != lo.end(); ++i) \
        r[i->first] = i->second; \
    for(auto i = ro.begin(); i != ro.end(); ++i) \
        if(r.end() != r.find(i->first)) \
            r[i->first] += i->second; \
        else \
            r[i->first] = i->second; \
    return r; \
} \
 \
TEMP \
TYPE operator -  ( const TYPE & lo, const TYPE & ro ) { \
    TYPE r; \
    for(auto i = lo.begin(); i != lo.end(); ++i) \
        if(ro.end() == ro.find(i->first)) \
            r[i->first] = i->second; \
    return r; \
}


#define UNORDERED_SET_TEMP template<class Key, class Hash, class KeyEqual, class Allocator>
#define UNORDERED_SET_TYPE std::unordered_set<Key, Hash, KeyEqual, Allocator>
SET_OPERATORS(UNORDERED_SET_TEMP, UNORDERED_SET_TYPE)

#define SET_TEMP template<class Key, class Compare, class Allocator>
#define SET_TYPE std::set<Key, Compare, Allocator>
SET_OPERATORS(SET_TEMP, SET_TYPE)

#define UNORDERED_MAP_TEMP template < class Key, class T, class Hash, class KeyEqual, class Allocator >
#define UNORDERED_MAP_TYPE std::unordered_map< Key, T, Hash, KeyEqual, Allocator >
MAP_OPERATORS(UNORDERED_MAP_TEMP, UNORDERED_MAP_TYPE)

#define MAP_TEMP template < class Key, class T, class Compare, class Allocator >
#define MAP_TYPE std::map< Key, T, Compare, Allocator >
MAP_OPERATORS(MAP_TEMP, MAP_TYPE)

#endif
