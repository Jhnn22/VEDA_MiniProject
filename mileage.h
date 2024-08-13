#ifndef MILEAGE_H
#define MILEAGE_H

class QString;

class Mileage
{
    int mileage;
public:
    Mileage():mileage(0){}
    ~Mileage(){}

    void addMileage(int amount);
    QString getMileage();
    // QString displayMileage();
    int getExtraAttemptsBasedOnMileage();
    void usedMileage(int amount);
    int calculateMileage(int attempts, int amount);
    void resetMileageIfNotLoggedIn();    // 비회원은 마일리지X

};

#endif // MILEAGE_H
