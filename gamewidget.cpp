#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "memberinfodialog.h"
#include "wordinfodialog.h"
#include "randomnumbergenerator.h"
#include "gamemanage.h"
#include "menuwidget.h"
#include <QCoreApplication>
#include <QDir>
#include <QVector>
#include <QMessageBox>

GameWidget::GameWidget(QWidget *parent, MenuWidget *menuWidget)
    : QWidget(parent)
    , ui(new Ui::GameWidget), gameManage(), menuWidget(menuWidget)
    , currentAttempts(0), maxAttempts(10)
    , anyChecked(false), isWordAdded(false)
{
    ui->setupUi(this);
    QString currentDir = QDir::currentPath();
    QString dirPath2 = QDir(currentDir).filePath("theme");
    WordInfoDialog wordInfoDialog(dirPath2, this);
    if(wordInfoDialog.loadWordInfo(dirPath2, word)){
        updateThemeCheckBox();
    }
    else{
        qDebug() << "Failed to load word information";
    }

    updateAddButtonState();
    ui->confirm->setEnabled(false);
    connect(ui->add, &QPushButton::clicked, this, &GameWidget::addButtonClicked);
    connect(ui->confirm, &QPushButton::clicked, this, &GameWidget::confirmButtonClicked);

}

void GameWidget::updateThemeCheckBox(){
    // 기존 데이터 초기화
    // for(auto it = checkBoxes.begin(); it != checkBoxes.end(); it++){
    //     QCheckBox *checkBox = *it;
    //     ui->verticalLayout_2->removeWidget(checkBox);
    //     delete checkBox;
    // }
    // checkBoxes.clear();
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

void GameWidget::updateAddButtonState(){
    for (auto it = checkBoxes.begin(); it != checkBoxes.end(); it++){
        if ((*it)->isChecked()){
            anyChecked = true;
            break;
        }
    }
    ui->add->setEnabled(anyChecked);
}

void GameWidget::updateNickNameAndId(){

}

void GameWidget::updateAttemptsAndMileage(){
    // 시도 횟수 & 누적 마일리지 설정
    int remainingAttempts = maxAttempts - currentAttempts;
    QString accumulatedMileage =  mileage.displayMileage();
    QString labelText = QString("남은 시도 횟수 : %1\n마일리지 : %2").arg(remainingAttempts).arg(accumulatedMileage);
    ui->attemptsAndMileage->setText(labelText);
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

    // 랜덤 난수 생성기 호출
    RandomNumberGenerator rng;
    int randomIndex = rng.randomNumberGenerator(0, selectedWords.size()-1);
    selectedWord = selectedWords[randomIndex];

    qDebug() << selectedWord;
    // ui->result->setText(selectedWord);
    QString resultTopRowSetText = QString("%1개의 음절로 이루어진 단어를 입력해 주세요.").arg(selectedWord.length());
    ui->result->setText(resultTopRowSetText);

    // 시도 횟수 초기화
    gameManage.resetAttempts(currentAttempts, maxAttempts);

    // // label_3 초기화
    // // 추후에 labe_3 함수 따로 파서 간단하게 호출, 지금은 임시방편
    // ui->attemptsAndMileage->setText("남은 시도 횟수 : 10\n마일리지 : 0");

    // 시도 횟수 & 누적 마일리지 설정
    updateAttemptsAndMileage();

    // 최초 단어를 추가해야 확인버튼 활성화
    isWordAdded = true;
    ui->confirm->setEnabled(true);
}

void GameWidget::confirmButtonClicked(){
    // result textbrowser 설정
    answer = ui->inputAnswer->text();
    qDebug() << answer;
    QString resultString;
    GameManage::gameResult gameResult = gameManage.run(selectedWord, answer, resultString, currentAttempts, maxAttempts);
    QString input1 = QString("=====%1번째 시도=====").arg(currentAttempts);
    QString input2 = QString("입력>> %1").arg(answer);
    ui->result->append(input1);
    ui->result->append(input2);
    ui->result->append(resultString);

    // 마일리지 설정
    int earnedMileage = mileage.calculateMileage(1, 3); // 매 시도마다 1회분의 마일리지만 추가
    mileage.addMileage(earnedMileage);

    // 시도 횟수 & 누적 마일리지 설정
    updateAttemptsAndMileage();

    // 게임 결과, gameResult는 멤버변수로 선언해서 함수를 나눠도 상관없게 하자
    switch(gameResult) {
    case GameManage::gameResult::correct:{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "정답!", "정답입니다! 한번 더 하시겠습니까?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            resetGame();
        } else {
            goToMenu();
        }
    }
    break;
    case GameManage::gameResult::inCorrect: break;
    case GameManage::gameResult::gameOver:{
        int extraAttempts = mileage.getExtraAttemptsBasedOnMileage();
        if(extraAttempts > 0){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "게임 오버"
                                            , QString("시도 횟수를 모두 사용했습니다.\n마일리지를 사용하여 %1번의 추가 시도를 하시겠습니까?").arg(extraAttempts)
                                            , QMessageBox::Yes | QMessageBox::No);

            if(reply == QMessageBox::Yes){
                maxAttempts += extraAttempts;
                mileage.usedMileage(extraAttempts);
                updateAttemptsAndMileage();
                return;
            } else {
                goToMenu();
                break;
            }
        }

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "게임 오버", "다시 시작하시겠습니까?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            resetGame();
        }
        else{
            goToMenu();
        }

    }
    break;
    }

    // 게임 끝나기 전 까지는 add 버튼 비활성화?
    ui->add->setEnabled(false);
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
    updateAttemptsAndMileage();
}

void GameWidget::goToMenu(){
    if (menuWidget) {
        menuWidget->show();
        this->hide();
    } else {
        qDebug() << "Error: menuWidget is null";
    }
}

GameWidget::~GameWidget()
{
    delete ui;
}
