#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"roomplayerform.h"
#include<map>
#include"uiapi/customwidget.h"
#include<QTimer>
#include<QTime>
#include<vector>

/**************************************/
#define USERINFO 0
#define SKYBLK_YYJ 1
#define SKYBLK_LR 2
#define SKYBLK_NW 3
#define SKYBLK_SW 4
/***************************************/


namespace Ui {
class roomDialog;
}

class roomDialog : public CustomMoveDialog
{
    Q_OBJECT

signals:
    void SIG_quitRoom(int);//id是房主的userid
    void SIG_QUIT();//退出游戏
    void SIG_ReadybeginGame();//准备开始游戏
    void SIG_beginGame();//正式开始游戏
    void SIG_skyBlkRs(int,int,int,int);//夜晚的操作
    void SIG_skyBlk15(bool);//是：前半夜结束  否：后半夜结束
    void SIG_nvSilverWater();//女巫救人
    void SIG_imDie(int);//我死了，发送我的身份到服务端


public:
    explicit roomDialog(QWidget *parent = nullptr);
    ~roomDialog();
    void slot_setInfo(int roomid,int mode,int method,bool lock,
                     QString password,int num,int userid);
    void slot_setPlayer(int id,int icon,int level,QString sex,QString name,int userid);
    void slot_destroyRoom();
    void slot_ready();
    void slot_setIden(int iden);
    void slot_skyBlack();
    void slot_yyj(int id,int iden);
    void slot_lr(int id,int toid);
    void slot_nw(int kill);
    void slot_skyWhite(int* die);
    void slot_speak();

private slots:
    void on_pb_min_clicked();
    void on_pb_close_clicked();
    void on_pb_quitroom_clicked();
    void on_pb_0_begin_clicked();

    void slot_addPlayer(QWidget* player,int id);
    void slot_removePlayer(QWidget* player,int id);

    void slot_click_icon(int id);

    void slot_OverTimerReady();
    void slot_OverTimerTips();
    void slot_OverTimerskyBlk();


private:
    Ui::roomDialog *ui;
    QVBoxLayout* m_playerLayoutRight;
    QVBoxLayout* m_playerLayoutLeft;
    std::map<int,roomPlayerform*>m_mapIdToPlayer;
    //房间信息
    int m_seat;
    int m_userid;
    int m_mode        ;//模式
    int m_method      ;//玩法
    int m_roomid      ;
//    char    roomName[_MAX_SIZE]    ;
    int m_count       ;
    int m_currentCou  ;
    bool m_pass        ;
    QString  m_passwd;
    bool m_playing;
    int m_day;

    //身份信息
    std::vector<QString>BASEIDENTIFY;

    //本人信息
    int m_user_iden;

    //定时器
    QTimer* m_timer_tips;//提示消失
    QTimer* m_timer_ready;//5秒准备
    QTimer* m_timer_skyBlk;//夜晚

    //用于倒计时
    int num;//5秒
    bool state;//判断是否在准备开始倒计时中

    //用于控制组件
    int m_pb_icon;


    int m_d_kill;//被杀的人
    bool m_d_antidote;//女巫解药
    bool m_d_poison;//女巫毒药
    int m_d_protect;//守卫守的人
    bool m_d_midnight;//半夜还是整夜，用于房主发送夜晚包
};

#endif // ROOMDIALOG_H
