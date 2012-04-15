#include <unordered_set>
#include <iostream>
#include <string>

#include "container_operators.h"

using namespace std;

void printOut(unordered_set<int> & s, string name){
    cout<<name<<": ";
    for(auto i = s.begin(); i != s.end(); ++i)
        cout<<*i<<" ";
    cout<<endl;
}

int test_container_operators () {
    unordered_set<int> s1;
    unordered_set<int> s2;

    s1 += 1;
    s1 += 2;
    s1 += 3;
    s1 += 4;

    s2 += -1;
    s2 += -2;
    s2 += -3;

    printOut(s1, "s1");
    printOut(s2, "s2");

    auto s3 = s1 + s2;
    
    printOut(s3, "s3=s1+s2, s3");

    s1+=s2;

    printOut(s1, "s1+=s2, s1");

    s3 -= s2;

    printOut(s3, "s3-=s2, s3");

    return 0;

}

int main(int argc, char ** argv) {
    test_container_operators();
    return 0;
}
