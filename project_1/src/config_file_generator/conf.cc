#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "location.cpp"


using namespace std;

int main() {

ifstream in("carLocation.txt");
if(!in) {cout<<"File not found!";}
ofstream out("conf.txt", ios::app);
if(!out) {cout<<"File not found!";}

string node;
int x, y;
location l[15];
int i=0, j=0;
while(in>>node>>x>>y){
l[i].x = x;
l[i].y = y;
i++;
}

for(i=0; i<15;i++){
 out<<"Node"<<i+1<<" links "; 
for(j=0; j<15;j++){

    if(j!=i){
      if(l[i].distance(l[j])<100){
       out<<j+1<<" ";
      }
    }
}

out<<endl;
}

in.close();
out.close();
}
