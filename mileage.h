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
    int getMileage();
    QString displayMileage();
    int getExtraAttemptsBasedOnMileage();
    void usedMileage(int amount);
    int calculateMileage(int attempts, int amount);

};

#endif // MILEAGE_H
