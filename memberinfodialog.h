#ifndef MEMBERINFODIALOG_H
#define MEMBERINFODIALOG_H

#include <QDialog>
#include <QMap>

class QString;
class QStringListModel;

namespace Ui {
class MemberInfoDialog;
}

class MemberInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MemberInfoDialog(QString &fileName, QWidget *parent = nullptr);
    ~MemberInfoDialog();
    static bool loadMemberInfo(QString &fileName, QMap<QString, QMap<QString, QString>> &member);
    void updateNickNameListView();
    void setMemberInfo(QMap<QString, QMap<QString, QString>> &memberInfo);

private slots:
    void searchButtonClicked();
    void NickNameListClicked(const QModelIndex &index);


private:
    Ui::MemberInfoDialog *ui;
    QMap<QString, QMap<QString, QString>> member;
    QStringListModel *model;
};

#endif // MEMBERINFODIALOG_H
