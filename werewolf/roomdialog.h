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
#include"packdef.h"

/**************************************/
#define USERINFO 0
#define SKYBLK_YYJ 1
#define SKYBLK_LR 2
#define SKYBLK_NW 3
#define SKYBLK_SW 4
#define VOTE_POLICE 5
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
    void SIG_police(int,bool);//是否竞选警长,是：举手 否：放手
    void SIG_PoliceEnd();//竞选阶段结束
    void SIG_SpeakEnd(int,int,int);//发言结束
    void SIG_imPolice(int);//我是警长
    void SIG_SpeakStateEnd(int state);//发言阶段结束
    void SIG_votePolice(int,int,int);//警长投票
    void SIG_VoteEnd(int);//投票结束
    void SIG_speakOrder(int,int);//发言顺序


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
    void slot_speak(STRU_SPEAK_RQ& rq);
    void slot_police();
    void slot_bePolice();
    void slot_setPolicePlayer(STRU_TOBEPOLICE_RS& rs);
    void slot_setPolice(STRU_BEPOLICE_RS& rs);
    void slot_beginVote(STRU_SPEAKSTATE_END& end);
    void slot_VoteRs(STRU_VOTE_RS& rs);
    void slot_SpeakOrder(STRU_SPEAK_ORDER& order);


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
    void slot_OverTimerPolice();
    void slot_OverTimerVote();


    void on_pb_0_end_clicked();

    void on_pb_operate_clicked();

    void on_pb_order_clicked();

    void on_pb_deorder_clicked();

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
    QTimer* m_timer_police;//竞选警长
    QTimer* m_timer_vote;//投票

    //用于倒计时
    int num;//5秒
    bool state;//判断是否在准备开始倒计时中
    int blk;//30s
    int police;//10s
    int vote;//10s

    //用于控制组件
    int m_pb_icon;//控制头像点击信号


    int m_d_kill;//被杀的人
    bool m_d_antidote;//女巫解药
    bool m_d_poison;//女巫毒药
    int m_d_protect;//守卫守的人
    bool m_d_police;//自己是否竞选
    int m_d_state;//当前阶段，用于控制发言
    int m_d_speak;//发言阶段，用于控制结束发言：如果当前阶段已经发过言，发送阶段结束包
    int m_d_vote;//投票阶段，用于发送投票结束包 1：上警
    bool m_d_bePolice;//自己是否为警长
    int m_d_nextSpeak;//发言顺序

    //其他
    QString Text_upPolice;//上警玩家提示信息
};

#endif // ROOMDIALOG_H
