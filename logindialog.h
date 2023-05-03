#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include"uiapi/customwidget.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public CustomMoveDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void SIG_register();
    void SIG_login(QString,QString);

private slots:
    void on_pb_register_clicked();

    void on_pb_login_clicked();

    void on_pb_feedback_clicked();

    void on_pb_close_clicked();

    void on_pb_min_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
