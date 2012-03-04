#ifndef _location_h
#define _location_h
#include <math.h>
class location{
//2d location;
public:
int x;
int y;
//set methods
public:

void setX(int xIn) {
x = xIn;}
void setY(int yIn) {
y = yIn;}

double distance(const location& l2){
return sqrt(pow((x-l2.x),2) +pow((y-l2.y),2));
}

};
#endif 
