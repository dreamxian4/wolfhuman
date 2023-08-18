#ifndef EMAILDIALOG_H
#define EMAILDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"emailmsgform.h"

namespace Ui {
class emailDialog;
}

class emailDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_clearSpaceEmail(bool isspace);
    void SIG_clearFriendEmail(bool isspace);

public:
    explicit emailDialog(QWidget *parent = nullptr);
    ~emailDialog();
    void slot_setInfo(bool space);
    void slot_addwidget(emailMsgForm* form);
    void slot_removewidget(emailMsgForm* form);
    void slot_clearwidget();

private slots:
    void on_pb_clear_clicked();

    void on_pb_clear_friend_clicked();

private:
    Ui::emailDialog *ui;
    QVBoxLayout* m_layout;
    QList<emailMsgForm*>lst;
};

#endif // EMAILDIALOG_H
