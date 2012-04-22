#include <unordered_set>
#include <set>
#include <iostream>
#include <string>

#include "container_operators.h"

using namespace std;

void printOut(unordered_set<int> & s, string name){
    cout<<name<<": [ ";
    for(auto i = s.begin(); i != s.end(); ++i)
        cout<<*i<<" ";
    cout<<" ]"<<endl;
}

void printOut(set<int> & s, string name){
    cout<<name<<": [ ";
    for(auto i = s.begin(); i != s.end(); ++i)
        cout<<*i<<" ";
    cout<<" ]"<<endl;
}

void printOut(unordered_map<int,int> & s, string name){
    cout<<name<<": { ";
    for(auto i = s.begin(); i != s.end(); ++i)
        cout<<i->first<<"->"<<i->second<<" ";
    cout<<" }"<<endl;
}

void printOut(map<int,int> & s, string name){
    cout<<name<<": { ";
    for(auto i = s.begin(); i != s.end(); ++i)
        cout<<i->first<<"->"<<i->second<<" ";
    cout<<" }"<<endl;
}

void printOut(map<int,set<int>> & s, string name){
    cout<<name<<": { ";
    for(auto i = s.begin(); i != s.end(); ++i)
    {
        cout<<i->first<<"->[";
        for(auto j = i->second.begin(); j != i->second.end(); ++j)
            cout<<*j<<", ";
        cout<<"] ";
    }
    cout<<" }"<<endl;
}

int test_set_operators () {

    set<int> s1;
    set<int> s2;

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

int test_unordered_set_operators () {
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

int test_unordered_map_operators () {
    unordered_map<int,int> m1;
    unordered_map<int,int> m2;

    m1[1]=10;
    m1[2]=20;
    m1[3]=30;
    m1[4]=40;

    m2[1]=100;
    m2[2]=200;
    m2[5]=500;

    printOut(m1, "m1");
    printOut(m2, "m2");

    auto m3 = m1 + m2;

    printOut(m3, "m3=m1+m2, m3");

    m1+=m2;

    printOut(m1, "m1+=m2, m1");

    m3 -= m2;

    printOut(m3, "m3-=m2, m3");

    return 0;

}

int test_map_operators () {
    set<int> s1;
    set<int> s2;
    set<int> s3;
    s1 += 10;
    s1 += 11;
    s3 += 20;
    s3 += 21;
    s2 += 30;
    map<int,set<int>> m1;
    map<int,set<int>> m2;

    m1[1]=s1;
    m1[2]=s2;

    m2[1]=s3;

    printOut(m1, "m1");
    printOut(m2, "m2");

    auto m3 = m1 + m2;

    printOut(m3, "m3=m1+m2, m3");

    m1+=m2;

    printOut(m1, "m1+=m2, m1");

    m3 -= m2;

    printOut(m3, "m3-=m2, m3");

    return 0;

}

int main(int argc, char ** argv) {
    cout<<"testing std::unordered_set"<<endl;
    test_unordered_set_operators();
    cout<<endl;
    cout<<"testing std::set"<<endl;
    test_set_operators();
    cout<<endl;
    cout<<"testing std::unordered_map"<<endl;
    test_unordered_map_operators();
    cout<<endl;
    cout<<"testing std::map"<<endl;
    test_map_operators();
    cout<<endl;
    return 0;
}
