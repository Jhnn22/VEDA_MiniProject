#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <QDialog>
#include <QMap>

class QString;
class SignUpDialog;


namespace Ui {
class SignInDialog;
}

class SignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignInDialog(QMap<QString, QMap<QString, QString>> &member, QWidget *parent = nullptr);
    ~SignInDialog();

private slots:
    void memberSignInButtonClicked();

    void nonMemberSignInButtonClicked();

    void signUpToMemberButtonClicked();



private:
    Ui::SignInDialog *ui;
    QString id, pw, name;
    QMap<QString, QMap<QString, QString>> member;
    SignUpDialog *SUD;


};

#endif // SIGNINDIALOG_H
