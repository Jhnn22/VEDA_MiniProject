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

    // 입력 전 기본 세팅
    ui->inputValue->setPlaceholderText("단어");

    loadWordInfo(dirPath, word);
    updateThemeListView();
    connect(ui->search2, &QPushButton::clicked, this, &WordInfoDialog::search2ButtonClicked);
    connect(ui->themeList, &QListView::clicked, this, &WordInfoDialog::themeListClicked);
}

WordInfoDialog::~WordInfoDialog()
{
    delete ui;
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
        else{
            qDebug() << "failed to open word file:" << it->absoluteFilePath();
        }
    }
    if(word.isEmpty()) {
        qDebug() << "no word files loaded from directory:" << dirPath;
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
    QString value = ui->inputValue->text(); //inputValue에 입력된 text를 value에 저장
    // it으로 가리키기만 하면 됨.
    ui->searchedWordList->clear();
    QStringList stringList3; //stringlist에 map(theme) value 값 빼서 저장하기
    for(auto it = word.begin(); it != word.end(); it++){
        QStringList wordList = it.value().split(" ", Qt::SkipEmptyParts);
        if(wordList.contains(value)){
            stringList3 <<it.key();
        }
    }

    if(stringList3.isEmpty()){
        ui->searchedWordList->setText("해당 단어가 존재하지 않습니다.");
    }else{
        for(const QString &fileName : stringList3){
            QString result = QString("검색한 단어: %1 \n단어가 있는 주제: %2").arg(value, fileName);
            ui->searchedWordList->append(result);
        }
    }

}

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

