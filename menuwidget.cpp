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
    signInDialog = new SignInDialog(member, this);
    if (signInDialog->exec() == QDialog::Accepted) {
        GameWidget * gameWidget = new GameWidget(nullptr, this);
        gameWidget->show();
        this->hide();
    }
    delete signInDialog;
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
    QString dirPath = QFileDialog::getExistingDirectory(this, "select directory");
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


