#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include<QDate>
#include"uiapi/customwidget.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public CustomMoveDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
    void slot_setInfo(QString username,QString passwd,QString name,QString sex);

signals:
    void SIG_submit(QString,QString,QString,QString,QDate);
    void SIG_return();

private slots:
    void on_pb_clear_clicked();

    void on_pb_register_clicked();

    void on_pb_return_clicked();

    void on_pb_min_clicked();

    void on_pb_close_clicked();

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
