#include "mileage.h"
#include "signindialog.h"
#include <QString>
#define SIZE 10

void Mileage::addMileage(int amount){
    mileage += amount;
}

QString Mileage::getMileage(){
    return QString::number(mileage);
}

// QString Mileage::displayMileage(){
//     return QString::number(mileage);
// }

int Mileage::getExtraAttemptsBasedOnMileage(){
    return mileage / SIZE;
}

void  Mileage::usedMileage(int amount){
    mileage -= amount * SIZE;
}

int Mileage::calculateMileage(int attempts, int amount){
    return attempts * amount;
}

void Mileage::resetMileageIfNotLoggedIn(){
    if(!SignInDialog::isLoggedIn){
        mileage = 0;
    }
}
