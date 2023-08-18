#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"useritem.h"

namespace Ui {
class findDialog;
}

class findDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_FindUser(QString,int,int);

public:
    explicit findDialog(QWidget *parent = nullptr);
    ~findDialog();
    void slot_setInfo(int page,int index,QString str);
    void slot_addwidget(UserItem* item,bool isfri);
    void slot_removewidget(UserItem* item);
    void slot_clearwidget();

private slots:
    void on_pb_findUser_clicked();

//    void on_pb_lastPage_clicked();

//    void on_pb_nextPage_clicked();

private:
    Ui::findDialog *ui;
    QVBoxLayout* m_layout;
    QList<UserItem*>lst;
//    int m_page;
//    int m_pageNum;//总页数
    int m_index;
    QString m_str;
};

#endif // FINDDIALOG_H
