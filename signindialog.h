#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <QDialog>
#include <QMap>

class QString;
class SignUpDialog;
class MemberInfoDialog;

namespace Ui {
class SignInDialog;
}

class SignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignInDialog(QMap<QString, QMap<QString, QString>> &member, QWidget *parent = nullptr);
    ~SignInDialog();

    static bool isLoggedIn;
    static QString currentId;
    static QString currentName;
    QString userInfo();
    void memberUpdate(QMap<QString, QMap<QString, QString>> &updatedMember);


private slots:
    void memberSignInButtonClicked();
    void nonMemberSignInButtonClicked();
    void signUpToMemberButtonClicked();

private:
    Ui::SignInDialog *ui;
    QString id, pw, name;
    QMap<QString, QMap<QString, QString>> &member;
    SignUpDialog *signUpDialog;
};

#endif // SIGNINDIALOG_H
