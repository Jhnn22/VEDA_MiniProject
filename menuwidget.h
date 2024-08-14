#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

class SignInDialog;
class SignUpDialog;
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

private slots:
    void gameStartButtonClicked();
    void memberListButtonClicked();
    void wordListButtonClicked();
    void signUpToMemberButtonClicked();
    void quitButtonClicked();

private:
    Ui::MenuWidget *ui;
    QMap<QString, QMap<QString, QString>> member;   // member 맵 원본, 일반 변수로 선언
    QString memberFileName;

    SignInDialog *signInDialog;
    SignUpDialog *signUpDialog;
    MemberInfoDialog *memberInfoDialog;
    WordInfoDialog *wordInfoDialog;
    GameWidget *gameWidget;
};

#endif // MENUWIDGET_H
