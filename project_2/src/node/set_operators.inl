/* This file contains operators for sets.
 * This is included by container_operators.h
 */
TEMP 
void operator += ( TYPE & lo, const Key & ro ) {
    lo.insert(ro);
}

TEMP 
void operator += ( TYPE & lo, const TYPE & ro ) {
    for(auto i = ro.begin(); i != ro.end(); ++i)
        lo += *i;
}

TEMP 
void  operator -= ( TYPE & lo, const Key & ro ){
    lo.erase(ro);
}

TEMP 
void  operator -= ( TYPE & lo, const TYPE & ro ){
    for(auto i = ro.begin(); i != ro.end(); ++i)
        lo -= *i;
}

TEMP 
TYPE operator +  ( const TYPE & lo, const TYPE & ro ) {
    TYPE r;
    for(auto i = lo.begin(); i != lo.end(); ++i)
        r.insert(*i);
    for(auto i = ro.begin(); i != ro.end(); ++i)
        r.insert(*i);
    return r;
}

TEMP 
TYPE operator -  ( const TYPE & lo, const TYPE & ro ) {
    TYPE r;
    for(auto i = lo.begin(); i != lo.end(); ++i)
        if(ro.end() == ro.find(*i))
            r.insert(*i);
    return r;
}

TEMP
TYPE operator & ( const TYPE & lo, const TYPE & ro ) {
    TYPE r;
    for(auto i = lo.begin(); i != lo.end(); ++i)
        if(ro.end() != ro.find(*i))
            r.insert(*i);
    return r;
}

TEMP
bool operator == ( const TYPE & lo, const TYPE & ro ) {
    if(lo.size() != ro.size())
        return false;
    for(auto i = lo.begin(); i != lo.end(); ++i)
        if(ro.end() == ro.find(*i))
            return false;
    return true;
}
