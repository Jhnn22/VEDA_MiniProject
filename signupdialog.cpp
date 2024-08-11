#include "signupdialog.h"
#include "ui_signupdialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QMap>


SignUpDialog::SignUpDialog(QMap<QString, QMap<QString, QString>>& member, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUpDialog)
    , member(member)
    , isNickNameChecked(false), isIdChecked(false)
{
    ui->setupUi(this);

    // 입력 전까지 나타나는 문장
    ui->makeNickName->setPlaceholderText("닉네임");
    ui->makeId->setPlaceholderText("아이디");
    ui->makePw->setPlaceholderText("비밀번호");

    // 가입하기 버튼은 최초 비활성화
    ui->signUp->setEnabled(false);

    connect(ui->duplicateCheck1, &QPushButton::clicked, this, &SignUpDialog::nickNameDuplicateCheckButtonClicked);
    connect(ui->duplicateCheck2, &QPushButton::clicked, this, &SignUpDialog::idDuplicateCheckButtonClicked);
    connect(ui->signUp, &QPushButton::clicked, this, &SignUpDialog::signUpButtonClicked);

    // LineEdit의 상태에 따라 가입하기 버튼의 활성화/비활성화를 결정
    connect(ui->makeNickName, &QLineEdit::textChanged, this, [this]() {
        isNickNameChecked = false;
        updateSignUpButtonState();
    });
    connect(ui->makeId, &QLineEdit::textChanged, this, [this]() {
        isIdChecked = false;
        updateSignUpButtonState();
    });
    connect(ui->makePw, &QLineEdit::textChanged, this, [this]() {
        updateSignUpButtonState();
    });

    qDebug() << "SignUpDialog created. Current members:" << member.size();

}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}\

void SignUpDialog::nickNameDuplicateCheckButtonClicked()
{
    //빈칸 예외처리
    nickName = ui->makeNickName->text();
    if(nickName.isEmpty()){
        QMessageBox::warning(this, "경고", "닉네임을 입력해주세요.");
    }

    bool nickNameDuplicated = false;
    for(auto it = member.begin(); it != member.end(); it++){
        if(it.value().value("nickname") == nickName){
            nickNameDuplicated = true;
            break;
        }
    }
    if(nickNameDuplicated){
        QMessageBox::warning(this, "경고", "닉네임이 이미 존재합니다.");
    }
    else{
        QMessageBox::information(this, "확인", "사용 가능한 닉네임입니다.");
        isNickNameChecked = true;
        updateSignUpButtonState();
    }
}

void SignUpDialog::idDuplicateCheckButtonClicked()
{
    id_2 = ui->makeId->text();
    if(id_2.isEmpty()){
        QMessageBox::warning(this, "경고", "아이디를 입력해주세요.");
    }
    else if(member.contains(id_2)){
        QMessageBox::warning(this, "경고", "아이디가 이미 존재합니다.");
    }
    else{
        QMessageBox::information(this, "확인", "사용 가능한 아이디입니다.");
        isIdChecked = true;
        updateSignUpButtonState();
    }
}


void SignUpDialog::signUpButtonClicked()
{
    pw_2 = ui->makePw->text();
    member[id_2] = {{"nickname", nickName}, {"password", pw_2}};
    QMessageBox::information(this, "성공", QString("%1님 환영합니다!").arg(nickName));
    saveMemberInfoToCsv();
    accept();
}

// 입력칸이 하나라도 비워져 있으면 가입하기 버튼 비활성화 처리
void SignUpDialog::updateSignUpButtonState(){
    ui->signUp->setEnabled(isNickNameChecked && isIdChecked && !(ui->makePw->text().isEmpty()));
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
    else return;
}
