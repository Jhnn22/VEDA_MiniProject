#include "signupdialog.h"
#include "ui_signupdialog.h"
#include "signindialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QMap>


SignUpDialog::SignUpDialog(QMap<QString, QMap<QString, QString>>& member, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUpDialog)
    , member(member)
    , isNickNameChecked(false), isIdChecked(false), isPwVerifyed(false)
{
    ui->setupUi(this);

    // 입력 전 기본 세팅
    ui->makeNickName->setPlaceholderText("닉네임");
    ui->makeId->setPlaceholderText("아이디");
    ui->makePw->setPlaceholderText("비밀번호");
    ui->verifyPw->setPlaceholderText("비밀번호 확인");

    // 비밀번호는 안 보이게
    ui->makePw->setEchoMode(QLineEdit::Password);
    ui->verifyPw->setEchoMode(QLineEdit::Password);


    // 가입하기 버튼은 최초 비활성화
    ui->signUp->setEnabled(false);

    connect(ui->duplicateCheck1, &QPushButton::clicked, this, &SignUpDialog::nickNameDuplicateCheckButtonClicked);
    connect(ui->duplicateCheck2, &QPushButton::clicked, this, &SignUpDialog::idDuplicateCheckButtonClicked);
    connect(ui->signUp, &QPushButton::clicked, this, &SignUpDialog::signUpButtonClicked);
    connect(ui->verifyPw, &QLineEdit::textChanged, this, &SignUpDialog::displayVerifyPw);

    // LineEdit의 상태에 따라 가입하기 버튼의 활성화/비활성화를 결정
    connect(ui->makeNickName, &QLineEdit::textChanged, this, &SignUpDialog::textChanged);
    connect(ui->makeId, &QLineEdit::textChanged, this, &SignUpDialog::textChanged);
    connect(ui->makePw, &QLineEdit::textChanged, this, &SignUpDialog::textChanged);
    connect(ui->verifyPw, &QLineEdit::textChanged, this, &SignUpDialog::textChanged);
    qDebug() << "SignUpDialog created. Current members:" << member.size();
}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

// 회원가입을 한 유저 정보를 .txt로 저장
void SignUpDialog::saveMemberInfoToCsv(){
    QFile file("member.txt");
    if(file.open(QIODevice::Append | QIODevice::Text)){
        QTextStream out(&file);

        //파일이 비어 있으면 헤더 추가
        if(file.size() == 0){
            out << "nickname id password\n";
        }

        //새로운 정보 추가
        out << nickName << " " << id_2 << " " << pw_2 << "\n";
        file.close();
    }
    else{
        qDebug() << "failed to open file for writing memberInfo";
    }
}

// 닉네임, 아이디, 비밀번호 중 하나라도 통과 못하면 가입 버튼 비활성화
void SignUpDialog::updateSignUpButtonState(){
    ui->signUp->setEnabled(isNickNameChecked && isIdChecked && isPwVerifyed);
}


void SignUpDialog::nickNameDuplicateCheckButtonClicked()
{
    //빈칸 예외처리
    nickName = ui->makeNickName->text();
    if(nickName.isEmpty()){
        ui->label->setText("닉네임을 입력해주세요.");
        ui->label->setStyleSheet("color: red;");
        return;
    }

    bool nickNameDuplicated = false;
    for(auto it = member.begin(); it != member.end(); it++){
        if(it.value().value("nickname") == nickName){
            nickNameDuplicated = true;
            break;
        }
    }
    if(nickNameDuplicated){
        ui->label->setText("닉네임이 이미 존재합니다.");
        ui->label->setStyleSheet("color: red;");
        return;
    }
    else{
        ui->label->setText("사용 가능한 닉네임입니다.");
        ui->label->setStyleSheet("color: green;");
        isNickNameChecked = true;
        updateSignUpButtonState();
    }
}

void SignUpDialog::idDuplicateCheckButtonClicked()
{
    //빈칸 예외처리
    id_2 = ui->makeId->text();
    if(id_2.isEmpty()){
        ui->label_2->setText("아이디를 입력해주세요.");
        ui->label_2->setStyleSheet("color: red;");
        return;
    }

    if(member.contains(id_2)){
        ui->label_2->setText("아이디가 이미 존재합니다.");
        ui->label_2->setStyleSheet("color: red;");
        return;
    }
    else{
        ui->label_2->setText("사용 가능한 아이디입니다.");
        ui->label_2->setStyleSheet("color: green;");
        isIdChecked = true;
        updateSignUpButtonState();
    }
}

void SignUpDialog::displayVerifyPw(){
    pw_2 = ui->makePw->text();
    QString str = ui->verifyPw->text();
    if(pw_2 == str){
        ui->label_3->setText("비밀번호가 일치합니다.");
        ui->label_3->setStyleSheet("color: green;");
        isPwVerifyed = true;
    }
    else{
        ui->label_3->setText("비밀번호가 일치하지 않습니다.");
        ui->label_3->setStyleSheet("color: red;");
        isPwVerifyed = false;
    }
}

void SignUpDialog::signUpButtonClicked()
{
    member[id_2] = {{"nickname", nickName}, {"password", pw_2}};
    saveMemberInfoToCsv();
    QMessageBox::information(this, "성공", QString("%1님 환영합니다!").arg(nickName));
    accept();
}

void SignUpDialog::textChanged(){
    if(sender() == ui->makeNickName) isNickNameChecked = false;
    else if(sender() == ui->makeId) isIdChecked = false;
    else if(sender() == ui->makePw || sender() == ui->verifyPw) displayVerifyPw();
    updateSignUpButtonState();
}
