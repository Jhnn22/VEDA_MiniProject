#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "gamemanage.h"
#include "memberinfodialog.h"
#include "menuwidget.h"
#include "mileage.h"
#include "randomnumbergenerator.h"
#include "signindialog.h"
#include "signupdialog.h"
#include "wordinfodialog.h"
#include <QCoreApplication>
#include <QDir>
#include <QVector>
#include <QMessageBox>
#include <QShortcut>

GameWidget::GameWidget(QMap<QString, QMap<QString, QString>> &member ,MenuWidget *menuWidget, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget), member(member), menuWidget(menuWidget), gameManage()
    , currentAttempts(0), maxAttempts(10)
    , anyChecked(false), isWordAdded(false)
{
    ui->setupUi(this);
    signInDialog = new SignInDialog(member, this);
    QString currentDir = QDir::currentPath();
    QString dirPath2 = QDir(currentDir).filePath("theme");
    WordInfoDialog wordInfoDialog(dirPath2, this);
    if(wordInfoDialog.loadWordInfo(dirPath2, word)){
        updateThemeCheckBox();
    }
    else{
        qDebug() << "error: failed to load word information from path" << dirPath2;
    }


    displayUserInfo();
    updateAddButtonState();
    ui->confirm->setEnabled(false);

    connect(ui->add, &QPushButton::clicked, this, &GameWidget::addButtonClicked);
    connect(ui->confirm, &QPushButton::clicked, this, &GameWidget::confirmButtonClicked);
    connect(ui->logOut, &QPushButton::clicked, this, &GameWidget::logOutButtonClicked);
    connect(ui->quit, &QPushButton::clicked, this, &GameWidget::quitButtonClicked);
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::updateThemeCheckBox(){
    while (QLayoutItem* item = ui->verticalLayout_2->takeAt(0)) {
        delete item->widget();  // 위젯을 삭제
        delete item;            // 레이아웃 항목 삭제
    }

    // 새 데이터 추가
    for(auto it = word.begin(); it != word.end(); it++){
        QCheckBox *checkBox2 = new QCheckBox(it.key(), this);
        ui->verticalLayout_2->addWidget(checkBox2);
        checkBoxes.push_back(checkBox2);
        connect(checkBox2, &QCheckBox::stateChanged, this, &GameWidget::updateAddButtonState);
    }
}

void GameWidget::displayAttemptsAndMileage(){
    // 남은 횟수 표시
    int remainingAttempts = maxAttempts - currentAttempts;
    QString accumulatedMileage = mileage.getMileage();
    QString display = QString("남은 시도 횟수 : %1\n누적 마일리지 : %2").arg(remainingAttempts).arg(accumulatedMileage);
    ui->attemptsAndMileage->setText(display);
    qDebug() << "Remaining Attempts:" << remainingAttempts;
    qDebug() << "Accumulated Mileage:" << accumulatedMileage;
}

void GameWidget::displayUserInfo(){
    if(signInDialog){
        ui->userInfo->setText(signInDialog->userInfo());
    }
    else{
        qDebug() << "error: signInDialog is null";
    }
}

void GameWidget::displayResult(GameManage::gameResult gameResult, QString & resultString){
    if(gameResult != GameManage::gameResult::inValidInput){
        QString input1 = QString("=====%1번째 시도=====").arg(currentAttempts);
        QString input2 = QString("입력>> %1").arg(answer);
        ui->result->append(input1);
        ui->result->append(input2);
        ui->result->append(resultString);

    }
}

void GameWidget::correct(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "정답!", "정답입니다! 한번 더 하시겠습니까?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        resetGame();
    } else {
        goToMenu();
    }
}

void GameWidget::gameOver(){
    int extraAttempts = mileage.getExtraAttemptsBasedOnMileage();
    if(extraAttempts > 0){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "게임 오버"
                                      , QString("시도 횟수를 모두 사용했습니다.\n마일리지를 사용하여 %1번의 추가 시도를 하시겠습니까?").arg(extraAttempts)
                                      , QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes){
            maxAttempts += extraAttempts;
            mileage.usedMileage(extraAttempts);
            displayAttemptsAndMileage();
        } else {
            finishGame();
        }
    }
    else{
        finishGame();
    }
}

void GameWidget::finishGame(){
    QMessageBox::information(this, "게임 오버", "원하는 주제를 추가하면 새로운 게임을 시작합니다.");
    resetGame();
}

void GameWidget::updateAddButtonState(){
    for (auto it = checkBoxes.begin(); it != checkBoxes.end(); it++){
        if ((*it)->isChecked()){
            anyChecked = true;
            break;
        }
    }
    ui->add->setEnabled(anyChecked);
}

void GameWidget::addButtonClicked(){
    // 선택된 테마에 해당되는 단어들을 벡터에 추가
    QVector<QString> selectedWords;
    for(int i = 0; i<checkBoxes.size(); i++){
        if(checkBoxes[i]->isChecked()){
            QString theme = checkBoxes[i]->text();
            QStringList words = word[theme].split(" ", Qt::SkipEmptyParts);
            selectedWords.append(words);
        }
    }

    if(selectedWords.isEmpty()){
        qDebug() << "error: no words selected";
        return;
    }

    // 랜덤 난수 생성기 호출
    RandomNumberGenerator rng;
    int randomIndex = rng.randomNumberGenerator(0, selectedWords.size()-1);
    selectedWord = selectedWords[randomIndex];

    qDebug() << selectedWord;    //  정답 표시용 디버그 입니다. 문제가 어려우시면 주석을 풀고 Applicaiton Output를 보시면 됩니다!
    QString resultTopRowSetText = QString("%1개의 음절로 이루어진 단어를 입력해 주세요.").arg(selectedWord.length());
    ui->result->setText(resultTopRowSetText);

    // 시도 횟수 초기화
    gameManage.resetAttempts(currentAttempts, maxAttempts);

    // 시도 횟수 & 누적 마일리지 설정
    displayAttemptsAndMileage();

    // 최초 단어를 추가해야 확인버튼 활성화
    isWordAdded = true;
    ui->confirm->setEnabled(true);
}

void GameWidget::confirmButtonClicked(){
    // 결과창 설정
    answer = ui->inputAnswer->text();
    QString resultString;
    GameManage::gameResult gameResult = gameManage.run(selectedWord, answer, resultString, currentAttempts, maxAttempts);
    if(gameResult != GameManage::gameResult::gameOver || currentAttempts < maxAttempts){
        displayResult(gameResult, resultString);
    }

    // 게임 결과
    switch(gameResult){
    case GameManage::gameResult::correct: correct(); break;
    case GameManage::gameResult::inCorrect: mileage.addMileage(3); break;
    case GameManage::gameResult::inValidInput: return;
    case GameManage::gameResult::gameOver: gameOver(); break;
    }
    // 남은 횟수 & 마일리지 설정
    displayAttemptsAndMileage();


    // 게임 끝나기 전 까지는 add 버튼 비활성화
    ui->add->setEnabled(false);

}

void GameWidget::logOutButtonClicked(){
    SignInDialog::isLoggedIn = false;
    goToMenu();
}

void GameWidget::quitButtonClicked(){
    goToMenu();
}

void GameWidget::resetGame(){
    for(auto it = checkBoxes.begin(); it != checkBoxes.end(); it++){
        (*it)->setChecked(false);
    }
    ui->add->setEnabled(false);
    ui->confirm->setEnabled(false);
    ui->result->clear();
    ui->inputAnswer->clear();
    selectedWord.clear();
    gameManage.resetAttempts(currentAttempts, maxAttempts);
    displayAttemptsAndMileage();
}

void GameWidget::goToMenu(){
    if(menuWidget){
        menuWidget->show();
        this->hide();
    }else{
        qDebug() << "error: menuWidget is null";
    }
}

