#include "menuwidget.h"
#include "ui_menuwidget.h"
#include "signindialog.h"
#include "memberinfodialog.h"
#include "wordinfodialog.h"
#include "gamewidget.h"
#include <QFileDialog>
#include <QTextStream>
#include <QString>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWidget)
{
    ui->setupUi(this);
    connect(ui->gameStart, &QPushButton::clicked, this, &MenuWidget::gameStartClicked);
    connect(ui->memberList, &QPushButton::clicked, this, &MenuWidget::memberListClicked);
    connect(ui->wordList, &QPushButton::clicked, this, &MenuWidget::wordListClicked);

    memberFileName = "member.txt";
    //디버그 확인용
    if (MemberInfoDialog::loadMemberInfo(memberFileName, member)) {
        qDebug() << "Loaded members in MenuWidget:" << member.size();
    } else {
        qDebug() << "Failed to load member information";
    }
}

void MenuWidget::gameStartClicked()
{
    // 회원 로그인 정보가 없으면 로그인 창 생성 후 게임시작
    if(!SignInDialog::isLoggedIn){
        signInDialog = new SignInDialog(member, this);
        if (signInDialog->exec() == QDialog::Accepted) {
            GameWidget * gameWidget = new GameWidget(nullptr, this);
            gameWidget->show();
            this->hide();
        }
        delete signInDialog;
    }

    // 회원 로그인 정보가 있으면 바로 게임시작
    else{
        GameWidget * gameWidget = new GameWidget(nullptr, this);
        gameWidget->show();
        this->hide();
    }

}


void MenuWidget::memberListClicked()
{
    memberInfoDialog = new MemberInfoDialog(memberFileName, this);
    memberInfoDialog->setMemberInfo(member);  // 새로운 함수를 추가해야 합니다
    memberInfoDialog->exec();
    delete memberInfoDialog;
}


void MenuWidget::wordListClicked()
{

    // QString dirPath = QFileDialog::getExistingDirectory(this, "select directory"); //select 하지 않고, 바로 켤 수 있도록 만들기
    QString dirPath = "./theme/"; //수정함
    if(!dirPath.isEmpty()){
        wordInfoDialog = new WordInfoDialog(dirPath, this);
        wordInfoDialog->exec();
        delete wordInfoDialog;
    }

}

MenuWidget::~MenuWidget()
{
    delete ui;
    delete gameWidget;
}


