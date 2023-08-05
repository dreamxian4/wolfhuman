#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include"uiapi/customwidget.h"
#include<QVBoxLayout>
#include"useritem.h"
#include<QMenu>
#include<QCloseEvent>
#include"chatitem.h"
#include"spaceform.h"
#include"packdef.h"

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
    //获取动态
    void SIG_getSpace(int,bool,int,QString,int);//种类（1好友 2推荐 3我的），是否为搜索，按（用户，内容）搜索，昵称/内容，页数

public:
    mainDialog(QWidget *parent = nullptr);
    ~mainDialog();
    void slot_setInfo(int userid,int icon,QString name,QString sex,QString username,int level);
    //添加好友
    void slot_setSpace(STRU_SPACE_RS* rs);
    void slot_clearSpace();
    void slot_addFriend(UserItem* userItem);
    void slot_addChat(ChatItem* chatItem);
    void slot_addSpace(spaceForm* spaceform);
    void slot_removeSpace(spaceForm* spaceform);

    void slot_getDetail(int userid);

private slots:
    void on_pb_1_createRoom_clicked();

    void on_pb_min_clicked();

    void on_pb_close_clicked();

    void on_pb_1_roomList_clicked();

    void on_pb_5_quiLLogin_clicked();

    void on_pb_5_setinfo_clicked();

    void on_pb_3_findUser_clicked();

    void on_tb_tabBarClicked(int index);

    void on_pb_2_friend_clicked();

    void on_pb_2_suggest_clicked();

    void on_pb_2_mine_clicked();

    void on_pb_2_findSpace_clicked();

    void on_pb_2_nextPage_clicked();

    void on_pb_2_lastPage_clicked();

private:
    Ui::mainDialog *ui;
    int m_userid;
    //QVBoxLayout是一个垂直布局的层
    QVBoxLayout* m_layout;
    QVBoxLayout* m_layout_chat;
    QVBoxLayout* m_layout_space;
    int currentState;//当前是什么类型的动态1：搜索 2：好友 3：推荐 4：我的
    int page;//当前页数
    int pageNum;//总页数
    QList<spaceForm*>spaceLst;
};
#endif // MAINDIALOG_H
