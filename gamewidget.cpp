#include "gamewidget.h"
#include "ui_gamewidget.h"
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
    , ui(new Ui::GameWidget), gameManage(10), menuWidget(menuWidget)    //maxAttempts 10으로 설정
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
    bool anyChecked = false;
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

    // 랜덤 난수 생성기 호출
    RandomNumberGenerator rng;
    int randomIndex = rng.randomNumberGenerator(0, selectedWords.size()-1);
    selectedWord = selectedWords[randomIndex];

    qDebug() << selectedWord;
    // ui->result->setText(selectedWord);
    QString resultTopRowSetText = QString("%1개의 음절로 이루어진 단어를 입력해 주세요.").arg(selectedWord.length());
    ui->result->setText(resultTopRowSetText);

    // 시도 횟수 초기화
    gameManage.resetAttempts();
}

void GameWidget::confirmButtonClicked(){
    answer = ui->inputAnswer->text();
    qDebug() << answer;
    QString resultString;
    bool isCorrect = gameManage.run(selectedWord, answer, resultString);
    QString input = QString("입력>> %1").arg(answer);
    ui->result->append(input);
    ui->result->append(resultString);
    if(isCorrect){
        qDebug() << "한번 더?";
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "정답!", "정답입니다! 한번 더 하시겠습니까?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            resetGame();
        }
        else if(reply == QMessageBox::No){
            goToMenu();
        }
    }
    else{
        // 정답 이외?
    }
}

void GameWidget::resetGame(){
    for(auto it = checkBoxes.begin(); it != checkBoxes.end(); it++){
        (*it)->setChecked(false);
    }
    ui->add->setEnabled(false);
    ui->result->clear();
    ui->inputAnswer->clear();
    selectedWord.clear();
    gameManage.resetAttempts();
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
