#ifndef CREATEROOMFORM_H
#define CREATEROOMFORM_H

#include <QWidget>

namespace Ui {
class createRoomForm;
}

class createRoomForm : public QWidget
{
    Q_OBJECT

signals:
    void SIG_CANCEL();
    void SIG_createRoom(int,int,int,int,bool,QString);

public:
    explicit createRoomForm(QWidget *parent = nullptr);
    ~createRoomForm();

private slots:
    void on_cb_passwd_clicked();

    void on_pb_cancel_clicked();

    void on_pb_commit_clicked();

private:
    Ui::createRoomForm *ui;
};

#endif // CREATEROOMFORM_H
