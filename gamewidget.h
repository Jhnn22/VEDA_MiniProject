#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "gamemanage.h"
#include "mileage.h"
#include <QWidget>
#include <QMap>

class QCheckBox;
class MenuWidget;
class SignInDialog;


namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QMap<QString, QMap<QString, QString>> &member, MenuWidget *menuWidget = nullptr, QWidget *parent = nullptr);
    ~GameWidget();

    void updateThemeCheckBox();
    void displayAttemptsAndMileage();
    void displayUserInfo();
    void displayResult(GameManage::gameResult gameResult, QString & resultString);
    void correct();
    void gameOver();
    void finishGame();


private slots:
    void updateAddButtonState();
    void addButtonClicked();
    void confirmButtonClicked();
    void logOutButtonClicked();
    void quitButtonClicked();
    void resetGame();
    void goToMenu();

private:
    Ui::GameWidget *ui;
    QMap<QString, QMap<QString, QString>> &member;
    QMap<QString, QString> word;
    QVector<QCheckBox*> checkBoxes;
    QString selectedWord, answer;
    GameManage gameManage;
    Mileage mileage;

    MenuWidget *menuWidget;
    SignInDialog *signInDialog;

    int maxAttempts;
    int currentAttempts;
    bool anyChecked;
    bool isWordAdded;
};

#endif // GAMEWIDGET_H
