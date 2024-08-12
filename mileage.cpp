#include "mileage.h"
#include <QString>
#define SIZE 10

void Mileage::addMileage(int amount){
    mileage += amount;
}

int Mileage::getMileage(){
    return mileage;
}

QString Mileage::displayMileage(){
    return QString::number(mileage);
}

int Mileage::getExtraAttemptsBasedOnMileage(){
    return mileage / SIZE;
}

void  Mileage::usedMileage(int amount){
    mileage -= amount * SIZE;
}

int Mileage::calculateMileage(int attempts, int amount){
    return attempts * amount;
}
