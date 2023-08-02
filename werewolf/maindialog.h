#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include"uiapi/customwidget.h"
#include<QVBoxLayout>
#include"useritem.h"
#include<QMenu>
#include<QCloseEvent>
#include"chatitem.h"
#include"spqceform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainDialog; }
QT_END_NAMESPACE

class mainDialog : public CustomMoveDialog
{
    Q_OBJECT

signals:
    void SIG_createRoomButton();
    void SIG_roomListButton(int,int,int);
    void SIG_QUIT();
    void SIG_QUITlogin();
    //添加好友的信号
    void SIG_FindUser(QString,int);//0 用户名 1 昵称

public:
    mainDialog(QWidget *parent = nullptr);
    ~mainDialog();
    void slot_setInfo(int userid,int icon,QString name,QString sex,QString username,int level);
    //添加好友
    void slot_addFriend(UserItem* userItem);
    void slot_addChat(ChatItem* chatItem);
    void slot_addSpace(spqceForm* spqceform);

private slots:
    void on_pb_1_createRoom_clicked();

    void on_pb_min_clicked();

    void on_pb_close_clicked();

    void on_pb_1_roomList_clicked();

    void on_pb_5_quiLLogin_clicked();

    void on_pb_5_setinfo_clicked();

    void on_pb_3_findUser_clicked();

private:
    Ui::mainDialog *ui;
    int m_userid;
    //QVBoxLayout是一个垂直布局的层
    QVBoxLayout* m_layout;
    QVBoxLayout* m_layout_chat;
    QVBoxLayout* m_layout_space;
};
#endif // MAINDIALOG_H
