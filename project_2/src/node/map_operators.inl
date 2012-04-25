/* This file contains operators for maps.
 * This is included by container_operators.h
 */

TEMP 
void operator += ( TYPE & lo, const TYPE & ro ) { 
    for(auto i = ro.begin(); i != ro.end(); ++i) 
        if(lo.end() != lo.find(i->first)) 
            lo[i->first] += i->second; 
        else 
            lo[i->first] = i->second; 
} 
 
TEMP 
void  operator -= ( TYPE & lo, const TYPE & ro ){ 
    for(auto i = ro.begin(); i != ro.end(); ++i) 
        lo.erase(i->first); 
} 
 
TEMP 
TYPE operator +  ( const TYPE & lo, const TYPE & ro ) { 
    TYPE r; 
    for(auto i = lo.begin(); i != lo.end(); ++i) 
        r[i->first] = i->second; 
    for(auto i = ro.begin(); i != ro.end(); ++i) 
        if(r.end() != r.find(i->first)) 
            r[i->first] += i->second; 
        else 
            r[i->first] = i->second; 
    return r; 
} 
 
TEMP 
TYPE operator -  ( const TYPE & lo, const TYPE & ro ) { 
    TYPE r; 
    for(auto i = lo.begin(); i != lo.end(); ++i) 
        if(ro.end() == ro.find(i->first)) 
            r[i->first] = i->second; 
    return r; 
}

TEMP
TYPE operator & ( const TYPE & lo, const TYPE & ro ) {
    TYPE r;
    for(auto i = lo.begin(); i != lo.end(); ++i)
        if(ro.end() != ro.find(i->first))
            r[i->first] = i->second;
    return r;
}
