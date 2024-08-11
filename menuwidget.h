#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

class SignInDialog;
class MemberInfoDialog;
class WordInfoDialog;
class GameWidget;

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
    // void loadMemberInfo();

private slots:
    void gameStartClicked();

    void memberListClicked();

    void wordListClicked();

private:
    Ui::MenuWidget *ui;
    QMap<QString, QMap<QString, QString>> member;
    QString memberFileName;
    SignInDialog *signInDialog;
    MemberInfoDialog *memberInfoDialog;
    WordInfoDialog *wordInfoDialog;
    GameWidget *gameWidget;
};

#endif // MENUWIDGET_H
