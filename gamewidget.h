#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "gamemanage.h"
#include "mileage.h"
#include <QWidget>
#include <QMap>

class QCheckBox;
class MenuWidget;


namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr, MenuWidget *menuWidget = nullptr);
    ~GameWidget();
    void updateThemeCheckBox();
    void updateAttemptsAndMileage();
    void updateNickNameAndId();

private slots:
    void updateAddButtonState();
    void addButtonClicked();
    void confirmButtonClicked();
    void resetGame();
    void goToMenu();

private:
    Ui::GameWidget *ui;
    QMap<QString, QString> word;
    QVector<QCheckBox*> checkBoxes;
    QString selectedWord, answer;
    GameManage gameManage;
    MenuWidget *menuWidget;
    Mileage mileage;
    int maxAttempts;
    int currentAttempts;
    bool anyChecked;
    bool isWordAdded;
};

#endif // GAMEWIDGET_H
