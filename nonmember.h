#ifndef NONMEMBER_H
#define NONMEMBER_H

#include <QDebug>
using namespace std;

class NonMember
{
    string name;

public:
    NonMember():name(""){}
    NonMember(string name):name(name){}
    ~NonMember(){}
};

#endif // NONMEMBER_H
