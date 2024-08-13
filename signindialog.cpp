#include "signindialog.h"
#include "ui_signindialog.h"
#include "signupdialog.h"
#include "memberinfodialog.h"
#include <QString>
#include <QMessageBox>
#include <QMap>

bool SignInDialog::isLoggedIn = false;
QString SignInDialog::currentId = "";
QString SignInDialog::currentName = "";

SignInDialog::SignInDialog(QMap<QString, QMap<QString, QString>> &member, QWidget *parent)
    : QDialog(parent)
    , member(member), ui(new Ui::SignInDialog)
{
    ui->setupUi(this);

    // 입력 전까지 나타나는 문장
    ui->inputId->setPlaceholderText("아이디");
    ui->inputPw->setPlaceholderText("비밀번호");
    ui->inputName->setPlaceholderText("이름");

    connect(ui->memberSignIn, &QPushButton::clicked, this, &SignInDialog::memberSignInButtonClicked);
    connect(ui->nonMemberSignIn, &QPushButton::clicked, this, &SignInDialog::nonMemberSignInButtonClicked);
    connect(ui->memberSignUp, &QPushButton::clicked, this, &SignInDialog::signUpToMemberButtonClicked);
    connect(ui->nonMemberSignUp, &QPushButton::clicked, this, &SignInDialog::signUpToMemberButtonClicked);   
}

SignInDialog::~SignInDialog()
{
    delete ui;
}

QString SignInDialog::userInfo(){
    if(isLoggedIn){
        return QString("%1님\n%2").arg(QString(member[currentId]["nickname"])).arg(currentId);
    }
    else{
        return QString("%1님").arg(currentName);
    }
}

void SignInDialog::memberSignInButtonClicked()
{
    // // 회원 정보 실시간 연동
    // QString memberFileName = "member.txt";
    // MemberInfoDialog::loadMemberInfo(memberFileName, member);

    id = ui->inputId->text();
    pw = ui->inputPw->text();

    // 빈칸 예외처리
    if(id.isEmpty() || pw.isEmpty()){
        QMessageBox::warning(this, "경고", "입력칸을 모두 채워주세요.");
        return;
    }

    if(member.contains(id) && member[id]["password"] == pw){
        isLoggedIn = true;
        currentId = id;
        qDebug() << member[id]["nickname"];
        //테스트
        userInfo();
        accept();
    }
    else{
        QMessageBox::warning(this, "경고", "아이디 또는 비밀번호가 일치하지 않습니다.");
    }

}

void SignInDialog::nonMemberSignInButtonClicked()
{
    name = ui->inputName->text();

    // 빈칸 예외처리
    if(name.isEmpty()){
        QMessageBox::warning(this, "경고", "이름을 입력해주세요.");
        return;
    }

    currentName = name;
    accept();
}

void SignInDialog::signUpToMemberButtonClicked()
{
    signUpDialog = new SignUpDialog(member, this);
    signUpDialog->exec();
}




