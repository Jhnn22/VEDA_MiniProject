#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

#include <QDebug>
#include <random>
using namespace std;

class RandomNumberGenerator{
private:
    random_device dev;
    mt19937 gen;
public:
    RandomNumberGenerator():gen(dev()){}
    int randomNumberGenerator(int min, int max){
        uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

};

#endif // RANDOMNUMBERGENERATOR_H
