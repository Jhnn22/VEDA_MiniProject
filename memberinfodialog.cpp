#include "memberinfodialog.h"
#include "ui_memberinfodialog.h"
#include <QString>
#include <QFile>
#include <QStringListModel>
#include <QTextStream>
#include <QMap>

MemberInfoDialog::MemberInfoDialog(QString &fileName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MemberInfoDialog), model(new QStringListModel(this))
{
    ui->setupUi(this);
    ui->nickNameList->setModel(model);

    // 읽기 전용 처리
    ui->searchedMemberInfo->setReadOnly(true);
    ui->nickNameList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 입력 전 기본 세팅
    ui->inputNickName->setPlaceholderText("닉네임");

    loadMemberInfo(fileName, member);
    connect(ui->search, &QPushButton::clicked, this, &MemberInfoDialog::searchButtonClicked);
    connect(ui->nickNameList, &QListView::clicked, this, &MemberInfoDialog::NickNameListClicked);
}


MemberInfoDialog::~MemberInfoDialog()
{
    delete ui;
}

bool MemberInfoDialog::loadMemberInfo(QString &fileName, QMap<QString, QMap<QString, QString>> &member){
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            in.readLine();  // 헤더 라인 건너뛰기
            member.clear();  // 기존 QMap초기화로 중복 방지
            while(!in.atEnd()){
                QString line = in.readLine();
                QStringList fields = line.split(' ');
                if(fields.size() == 3){
                    QString str1 = fields[0];
                    QString str2 = fields[1];
                    QString str3 = fields[2];
                    member[str2] = {{"nickname", str1}, {"password", str3}};
                }
                else{
                    qDebug() << "invalid line format:" << line;
                }
            }
            file.close();
            qDebug() << "loaded members:" << member.size();
            return true;
        }
        else {
            qDebug() << "failed to open file:" << fileName;
        }
    }
    else{
        qDebug() << "file name is empty";
    }
    return false;
}

void MemberInfoDialog::updateNickNameListView(){
    QStringList nicknames;
    for(auto it = member.begin(); it != member.end(); it++){
        nicknames << it.value()["nickname"];
    }
    model->setStringList(nicknames);

}

void MemberInfoDialog::memberUpdate(QMap<QString, QMap<QString, QString>> &updatedMember){
    member = updatedMember;
    updateNickNameListView();
}

void MemberInfoDialog::searchButtonClicked(){
    QString searchNickName = ui->inputNickName->text();
    bool find = false;
    for(auto it = member.begin(); it != member.end(); it++){
        if(it.value()["nickname"] == searchNickName){
            QString info = QString("닉네임 : %1\n아이디 : %2\n비밀번호 : %3").arg(it.value()["nickname"], it.key(), it.value()["password"]);
            ui->searchedMemberInfo->setText(info);
            find = true;
            break;
        }
    }
    if(!find){
        ui->searchedMemberInfo->setText("유저 정보를 찾을 수 없습니다.");
    }

}

void MemberInfoDialog::NickNameListClicked(const QModelIndex &index)
{
    //NickName list에 표시된 data를 string으로 변환해서 QString에 저장하기.
    QString selectedNickName = index.data(Qt::DisplayRole).toString();

    for(auto it = member.begin(); it != member.end(); it++){
        if(it.value()["nickname"] == selectedNickName){
            QString info = QString("닉네임 : %1\n아이디 : %2\n비밀번호 : %3").arg(it.value()["nickname"], it.key(), it.value()["password"]);
            ui->searchedMemberInfo->setText(info);
            break;
        }
    }
}

