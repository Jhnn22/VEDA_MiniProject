#ifndef WORDINFODIALOG_H
#define WORDINFODIALOG_H

#include <QDialog>
#include <QMap>

class QStringListModel;

namespace Ui {
class WordInfoDialog;
}

class WordInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WordInfoDialog(QString &dirPath, QWidget *parent = nullptr);
    ~WordInfoDialog();
    bool loadWordInfo(QString &dirPath, QMap<QString, QString> &word);
    void updateThemeListView();

private slots:
    void search2ButtonClicked();
    void themeListClicked(const QModelIndex &index);


private:
    Ui::WordInfoDialog *ui;
    QMap<QString, QString> word;
    QStringListModel *model2;
};

#endif // WORDINFODIALOG_H
