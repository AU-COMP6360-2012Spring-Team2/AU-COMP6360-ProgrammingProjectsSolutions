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


#define TEMP template<class Key, class Hash, class KeyEqual, class Allocator>
#define TYPE std::unordered_set<Key, Hash, KeyEqual, Allocator>
#include "set_operators.inl"
#undef TEMP
#undef TYPE

#define TEMP template<class Key, class Compare, class Allocator>
#define TYPE std::set<Key, Compare, Allocator>
#include "set_operators.inl"
#undef TEMP
#undef TYPE

#define TEMP template < class Key, class T, class Hash, class KeyEqual, class Allocator >
#define TYPE std::unordered_map< Key, T, Hash, KeyEqual, Allocator >
#include "map_operators.inl"
#undef TEMP
#undef TYPE

#define TEMP template < class Key, class T, class Compare, class Allocator >
#define TYPE std::map< Key, T, Compare, Allocator >
#include "map_operators.inl"
#undef TEMP
#undef TYPE

#endif
