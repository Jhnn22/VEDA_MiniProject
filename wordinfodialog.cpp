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

    loadWordInfo(dirPath, word);
    updateThemeListView();
    connect(ui->search2, &QPushButton::clicked, this, &WordInfoDialog::search2ButtonClicked);
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
    QString theme = ui->inputTheme->text();
    if(word.contains(theme)){
        QString content2 = word[theme];
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
