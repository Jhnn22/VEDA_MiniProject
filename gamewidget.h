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
    void updateAttemptsAndMileage();
    void updateUserInfo();

private slots:
    void updateAddButtonState();
    void addButtonClicked();
    void confirmButtonClicked();
    void resetGame();
    void goToMenu();

private:
    Ui::GameWidget *ui;
    QMap<QString, QMap<QString, QString>> member;
    QMap<QString, QString> word;
    QVector<QCheckBox*> checkBoxes;
    QString selectedWord, answer;
    GameManage gameManage;
    MenuWidget *menuWidget;
    SignInDialog *signInDialog;
    Mileage mileage;
    int maxAttempts;
    int currentAttempts;
    bool anyChecked;
    bool isWordAdded;
};

#endif // GAMEWIDGET_H
