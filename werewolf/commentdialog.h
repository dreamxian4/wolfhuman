#ifndef COMMENTDIALOG_H
#define COMMENTDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"commentform.h"
#include<QList>

namespace Ui {
class commentDialog;
}

class commentDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_sendComment(int,QString);//spaceid,comment

public:
    explicit commentDialog(QWidget *parent = nullptr);
    ~commentDialog();
    void slot_setInfo(int icon,QString name,QString time,QString content,int id);
    void slot_addwidget(commentForm* form);
    void slot_removewidget(commentForm* form);
    void slot_clearwidget();

private slots:
    void on_pb_sendComment_clicked();

private:
    Ui::commentDialog *ui;
    QVBoxLayout* m_layout;
    QList<commentForm*>lst;
    int spaceid;
};

#endif // COMMENTDIALOG_H
