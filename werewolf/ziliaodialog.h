#ifndef ZILIAODIALOG_H
#define ZILIAODIALOG_H

#include <QDialog>
#include"packdef.h"
#include"uiapi/customwidget.h"

namespace Ui {
class ZiLiaoDialog;
}

class ZiLiaoDialog : public CustomMoveDialog
{
    Q_OBJECT

signals:
    void SIG_sendMsg(int id);
    void SIG_joinRoom(int id);
    void SIG_deleteFriend(int);//firendid
    void SIG_getSpace(int);
    void SIG_addFri(int);

public:
    explicit ZiLiaoDialog(QWidget *parent = nullptr);
    ~ZiLiaoDialog();
    void slot_setInfo(STRU_FRIEND_ZILIAO_RS& rs,bool fri,bool game);

private slots:
    void on_pb_min_clicked();

    void on_pb_close_clicked();

    void on_pb_sendMess_clicked();

    void on_pb_followRoom_clicked();

    void on_pb_space_clicked();

    void on_pb_delete_clicked();

private:
    Ui::ZiLiaoDialog *ui;
    int friendId;
    int roomid;
    bool isfri;
};

#endif // ZILIAODIALOG_H
