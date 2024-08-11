#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>
// #include <QMap> // 템플릿 클래스는 전방 선언으로는 충분한 정보를 전달하지 못한다!, 왜지...?

class QString;
class QLineEdit;

namespace Ui {
class SignUpDialog;
}

class SignUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignUpDialog(QMap<QString, QMap<QString, QString>>& member, QWidget *parent = nullptr);
    ~SignUpDialog();
    void updateSignUpButtonState();
    void saveMemberInfoToCsv();

private slots:
    void nickNameDuplicateCheckButtonClicked();
    void idDuplicateCheckButtonClicked();

    void signUpButtonClicked();

private:
    Ui::SignUpDialog *ui;
    QMap<QString, QMap<QString, QString>>& member;
    QString nickName, id_2, pw_2;
    bool isNickNameChecked;
    bool isIdChecked;
};

#endif // SIGNUPDIALOG_H
