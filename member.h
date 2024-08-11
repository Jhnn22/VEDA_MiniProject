#ifndef MEMBER_H
#define MEMBER_H

#include <QDebug>
using namespace std;

class Member
{
    string name;
    int mileage;

public:
    Member():name(""), mileage(0){}
    Member(string name):name(name), mileage(0){}
    ~Member(){}
    void addmileage(int amount){mileage += amount;}
    int getmileage(){return mileage;}
};

#endif // MEMBER_H

