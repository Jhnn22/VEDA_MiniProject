#ifndef GAMEMANAGE_H
#define GAMEMANAGE_H

class QString;

class GameManage
{
private:
    int maxAttempts;
    int currentAttempts;
public:
    GameManage():maxAttempts(0), currentAttempts(0){}
    GameManage(int maxAttempts):maxAttempts(maxAttempts), currentAttempts(0){}
    // void gameStart();
    bool run(QString &question, QString &answer, QString &resultString);
    bool isGameOver();
    void resetAttempts();
};

#endif // GAMEMANAGE_H
