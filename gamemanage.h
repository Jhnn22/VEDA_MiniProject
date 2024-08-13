#ifndef GAMEMANAGE_H
#define GAMEMANAGE_H

#include <QMap>
class QString;

class GameManage
{
private:
    QMap<QString, QMap<QString, QString>> member;
public: 
    GameManage(){}
    ~GameManage(){}

    enum class gameResult{
        correct,
        inCorrect,
        invalidInput,
        gameOver
    };

    void gameStart();
    gameResult run(QString &question, QString &answer, QString &resultString, int &currentAttempts, int &maxAttempts);
    bool isGameOver(int &currentAttempts, int &maxAttempts);
    void resetAttempts(int &currentAttempts, int &maxAttempts);
};

#endif // GAMEMANAGE_H
