#include "menuwidget.h"
#include "ui_menuwidget.h"
#include "gamewidget.h"
#include "memberinfodialog.h"
#include "signindialog.h"
#include "signupdialog.h"
#include "wordinfodialog.h"
#include <QApplication>
#include <QFileDialog>
#include <QTextStream>
#include <QString>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWidget), member()
{
    ui->setupUi(this);
    connect(ui->gameStart, &QPushButton::clicked, this, &MenuWidget::gameStartButtonClicked);
    connect(ui->memberList, &QPushButton::clicked, this, &MenuWidget::memberListButtonClicked);
    connect(ui->wordList, &QPushButton::clicked, this, &MenuWidget::wordListButtonClicked);
    connect(ui->quit, &QPushButton::clicked, qApp, &QApplication::quit);

    memberFileName = "member.txt";
    MemberInfoDialog::loadMemberInfo(memberFileName, member);

}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::gameStartButtonClicked()
{
    // 회원 로그인 정보가 없으면 로그인 창 생성 후 게임시작
    if(!SignInDialog::isLoggedIn){
        signInDialog = new SignInDialog(member, this);
        signInDialog->memberUpdate(member);
        if (signInDialog->exec() == QDialog::Accepted) {
            gameWidget = new GameWidget(member, this, nullptr);
            gameWidget->show();
            this->hide();
        }
        else{
            qDebug() << "signIn cancelled";
        }
        delete signInDialog;
    }

    // 회원 로그인 정보가 있으면 바로 게임시작
    else{
        gameWidget = new GameWidget(member, this, nullptr);
        gameWidget->show();
        this->hide();
    }

}


void MenuWidget::memberListButtonClicked()
{
    memberInfoDialog = new MemberInfoDialog(memberFileName, this);
    memberInfoDialog->memberUpdate(member); // 정보 갱신
    memberInfoDialog->exec();
    delete memberInfoDialog;
}


void MenuWidget::wordListButtonClicked()
{
    QString dirPath = "./theme/";
    if(!dirPath.isEmpty()){
        wordInfoDialog = new WordInfoDialog(dirPath, this);
        wordInfoDialog->exec();
        delete wordInfoDialog;
    }
}

void MenuWidget::signUpToMemberButtonClicked(){
    signUpDialog = new SignUpDialog(member, this);
    signUpDialog->exec();
    signUpDialog->close();
}

void MenuWidget::quitButtonClicked(){
    qApp->quit();
}



