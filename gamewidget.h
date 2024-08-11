#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "gamemanage.h"
#include "menuwidget.h"
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
};

#endif // GAMEWIDGET_H
