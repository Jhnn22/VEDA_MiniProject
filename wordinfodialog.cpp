#include "wordinfodialog.h"
#include "ui_wordinfodialog.h"
#include <QString>
#include <QFile>
#include <QStringListModel>
#include <QTextStream>
#include <QDir>

WordInfoDialog::WordInfoDialog(QString &dirPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WordInfoDialog), model2(new QStringListModel(this))
{
    ui->setupUi(this);
    ui->themeList->setModel(model2);

    // 읽기 전용 처리
    ui->searchedWordList->setReadOnly(true);
    ui->themeList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    loadWordInfo(dirPath, word); //word load함
    updateThemeListView();
    connect(ui->search2, &QPushButton::clicked, this, &WordInfoDialog::search2ButtonClicked);

    //wordinfodialog에 넣어주기
    connect(ui->themeList, &QListView::clicked, this, &WordInfoDialog::themeListClicked);

}


bool WordInfoDialog::loadWordInfo(QString &dirPath, QMap<QString, QString> &word){
    QDir dir(dirPath);
    QStringList stringList;
    stringList << "*.txt";
    QFileInfoList fileInfoList = dir.entryInfoList(stringList, QDir::Files);
    for(auto it = fileInfoList.begin(); it != fileInfoList.end(); it++){
        QString name = it->baseName();
        QFile file(it->absoluteFilePath());
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            QString content = in.readAll();
            word[name] = content;
            file.close();
        }
    }
    return !word.isEmpty();
}

void WordInfoDialog::updateThemeListView(){
    QStringList stringList2;
    for(auto it = word.begin(); it != word.end(); it++){
        stringList2 << it.key();
    }
    model2->setStringList(stringList2);

}

void WordInfoDialog::search2ButtonClicked(){
    QString theme = ui->inputTheme->text(); //inputTheme에 입력된 text를 theme에 저장
    if(word.contains(theme)){
        QString content2 = word[theme]; //word에 theme이 포함되어있으면, 해당키에 연결된 값을 가져오고, content2에 저장한다.
        QStringList stringList3 = content2.split(" ", Qt::SkipEmptyParts);
        std::sort(stringList3.begin(), stringList3.end());  //key값에 해당하는 value의 정렬
        ui->searchedWordList->clear();                     //중복 방지 초기화
        for(auto it = stringList3.begin(); it != stringList3.end(); it++){
            ui->searchedWordList->append(*it);
        }

    }
    else{
        ui->searchedWordList->setText("해당 테마를 찾을 수 없습니다.");
    }

}

WordInfoDialog::~WordInfoDialog()
{
    delete ui;
}

//txt 파일 선택했을 때, info 창에 뜨도록 설정하기 /*수정중*/

void WordInfoDialog::themeListClicked(const QModelIndex &index)
{
    //theme list에 표시된 data를 string으로 변환해서 QString에 저장하기.
    QString selectedTheme = index.data(Qt::DisplayRole).toString();

    //수정만 좀 하면 될듯
    if(word.contains(selectedTheme)){
        QString content3 = word[selectedTheme]; //word에 theme이 포함되어있으면, 해당키에 연결된 값을 가져오고, content2에 저장한다.
        QStringList stringList4 = content3.split(" ", Qt::SkipEmptyParts);
        std::sort(stringList4.begin(), stringList4.end());  //key값에 해당하는 value의 정렬
        ui->searchedWordList->clear();                     //중복 방지 초기화
        for(auto it = stringList4.begin(); it != stringList4.end(); it++){
            ui->searchedWordList->append(*it);
        }

    }
    else{
        ui->searchedWordList->setText("해당 테마를 찾을 수 없습니다.");
    }
}


