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

    //읽기 전용 처리
    ui->searchedMemberInfo->setReadOnly(true);
    ui->nickNameList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    loadMemberInfo(fileName, member);
    connect(ui->search, &QPushButton::clicked, this, &MemberInfoDialog::searchButtonClicked);

    //클릭했을 때, SearchedMemberInfo에 뜨게 설정
    connect(ui->nickNameList, &QListView::clicked, this, &MemberInfoDialog::NickNameListClicked);
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
                QStringList fields = line.split(" ");
                if(fields.size() == 3){
                    QString str1 = fields[0];
                    QString str2 = fields[1];
                    QString str3 = fields[2];
                    member[str2] = {{"nickname", str1}, {"password", str3}};
                }
            }
            file.close();
            return true;
            // updateNickNameListView();
            qDebug() << "Loaded members:" << member.size();
        }
        else {
            qDebug() << "Failed to open file:" << fileName; // 디버깅용
        }
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

void MemberInfoDialog::setMemberInfo(QMap<QString, QMap<QString, QString>> &memberInfo){
    member = memberInfo;
    updateNickNameListView();
}

void MemberInfoDialog::searchButtonClicked(){
    QString searchNickName = ui->inputNickName->text();
    bool find = false;
    for(auto it = member.begin(); it != member.end(); it++){
        if(it.value()["nickname"] == searchNickName){
            QString info = QString("NICKNAME : %1\n ID : %2\n PW : %3").arg(it.value()["nickname"], it.key(), it.value()["password"]);
            ui->searchedMemberInfo->setText(info);
            find = true;
            break;
        }
    }
    if(!find){
        ui->searchedMemberInfo->setText("유저 정보를 찾을 수 없습니다.");
    }

}

MemberInfoDialog::~MemberInfoDialog()
{
    delete ui;
}

//닉네임리스트 클릭하면 info 띄움
void MemberInfoDialog::NickNameListClicked(const QModelIndex &index)
{
    //NickName list에 표시된 data를 string으로 변환해서 QString에 저장하기.
    QString selectedNickName = index.data(Qt::DisplayRole).toString();

    for(auto it = member.begin(); it != member.end(); it++){
        if(it.value()["nickname"] == selectedNickName){
            QString info = QString("NICKNAME : %1\n ID : %2\n PW : %3").arg(it.value()["nickname"], it.key(), it.value()["password"]);
            ui->searchedMemberInfo->setText(info);
            break;
        }
    }
}
