#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include"packdef.h"
#include"startdialog.h"
#include"TcpClientMediator.h"
#include"logindialog.h"
#include"registerdialog.h"
#include"maindialog.h"
#include"createroomform.h"
#include"roomdialog.h"
#include"roomplayerform.h"
#include"roomlistdialog.h"
#include"AudioApi/audioread.h"
#include"AudioApi/audiowrite.h"

class ckernel;
typedef void (ckernel::*PFUN)(unsigned int,char*,int);

class ckernel : public QObject
{
    Q_OBJECT
private:
    explicit ckernel(QObject *parent = nullptr);
    ckernel(const ckernel& ck){}
    ~ckernel();

public:
    static ckernel* getCkernel(){
        static ckernel ck;
        return &ck;
    }


signals:


public slots:

    //页面之间的槽
    //启动界面->加入房间
    void slot_qie_joinGame();
    //启动界面->注册
    void slot_qie_register();
    //注册界面->登录界面
    void slot_qie_returnLogin();
    //主界面->创建房间界面
    void slot_qie_createRoomButton();
    //创建房间->主界面
    void slot_qie_cancel();
    //房间->主界面sendQuitRoom
    void slot_qie_quitRoom(int id);
    //房间列表->主界面
    void slot_qie_listMain();



    //网络发送槽
    void slot_sendRegisterRq(QString username,QString passwd,
                             QString name,QString sex,QDate date);
    void slot_sendLoginRq(QString name,QString passwd);
    void slot_sendCreateRoomRQ(int mode,int method,int playerNum,
                               int level,bool pass,QString password);
    void slot_sendroomListRQ(int method,int mode,int roomid);
    void slot_sendJoinRoomRq(int roomid);
    void slot_sendBeginGameTestRq();
    void slot_sendBeginGameRq();
    void slot_sendSkyBlkRs(int iden,int seat,int operate,int toseat);
    void slot_sendskyBlk15(bool mid);
    void slot_sendNvSW();
//    void slot_sendImDie(int iden);
    void slot_sendPolice(int seat,bool raise);
    void slot_sendPoliceEnd();
    void slot_sendSpeakEnd(int seat,int next,int state);
    void slot_sendImPolice(int seat,int state);
    void slot_sendSpeakStateEnd(int state);
    void slot_sendVote(int seat,int toseat,int state);
    void slot_sendVoteEnd(int state);
    void slot_sendSpeakOrder(int seat,int next);
    void slot_sendDayExile(int seat);
    void slot_Audio(bool begin,bool sent,bool wolf);
    void slot_sendAudio(QByteArray& frame);
    void slot_sendLrKillSelf();



    //网络接收处理槽
    //处理数据
    void dealData( unsigned int lSendIP , char* buf , int nlen );
    //注册
    void slot_DealRegisterRs( unsigned int lSendIP , char* buf , int nlen );
    //登录
    void slot_DealLoginRs( unsigned int lSendIP , char* buf , int nlen );
    //强制退出登录
    void slot_DealQuitLoginRs( unsigned int lSendIP , char* buf , int nlen );
    //自己的个人信息
    void slot_DealUserInfoRs( unsigned int lSendIP , char* buf , int nlen );
    //创建房间回复
    void slot_DealCreateRoomRs( unsigned int lSendIP , char* buf , int nlen );
    //房间列表
    void slot_DealRoomListRs( unsigned int lSendIP , char* buf , int nlen );
    //房间成员信息
    void slot_DealRoomMemberRq( unsigned int lSendIP , char* buf , int nlen );
    //加入房间
    void slot_DealJoinRoomRs( unsigned int lSendIP , char* buf , int nlen );
    //离开房间
    void slot_DealLeaveRoomRs( unsigned int lSendIP , char* buf , int nlen );
    //准备开始
    void slot_DealBeginGameTestRs( unsigned int lSendIP , char* buf , int nlen );
    //正式开始
    void slot_DealBeginGameRs( unsigned int lSendIP , char* buf , int nlen );
    //天黑
    void slot_DealSkyBlackRq( unsigned int lSendIP , char* buf , int nlen );
    //预言信息
    void slot_DealYYJSkyBlk( unsigned int lSendIP , char* buf , int nlen );
    //狼人杀人信息
    void slot_DealLRSkyBlk( unsigned int lSendIP , char* buf , int nlen );
    //狼人的确切杀人信息
    void slot_DealLRKillSkyBlk( unsigned int lSendIP , char* buf , int nlen );
    //天亮了,包含死亡信息
    void slot_DealSkyWhiteRq( unsigned int lSendIP , char* buf , int nlen );
    //天亮发言
    void slot_DealSpeakRq( unsigned int lSendIP , char* buf , int nlen );
    //竞选警长
    void slot_DealPoliceRq( unsigned int lSendIP , char* buf , int nlen );
    //成为警长
    void slot_DealBePoliceRq( unsigned int lSendIP , char* buf , int nlen );
    //竞选警长的玩家
    void slot_DealPolicePlayerRs( unsigned int lSendIP , char* buf , int nlen );
    //设置警长
    void slot_DealBePoliceRs( unsigned int lSendIP , char* buf , int nlen );
    //开始投票
    void slot_DealBeginVote( unsigned int lSendIP , char* buf , int nlen );
    //投票结果
    void slot_DeaVoteRs( unsigned int lSendIP , char* buf , int nlen );
    //警长选择发言顺序
    void slot_DealSpeakOrder( unsigned int lSendIP , char* buf , int nlen );
    //发言阶段开始
    void slot_DealSpeakStateBegin( unsigned int lSendIP , char* buf , int nlen );
    //白天放逐玩家
    void slot_DealDayExile( unsigned int lSendIP , char* buf , int nlen );
    //游戏结束
    void slot_DealGameOver( unsigned int lSendIP , char* buf , int nlen );
    //音频包
    void slot_DealAudioFrame( unsigned int lSendIP , char* buf , int nlen );
    //暂停发言
    void slot_DealSpeakPause( unsigned int lSendIP , char* buf , int nlen );
    //玩家结束发言
    void slot_DealSpeakEnd( unsigned int lSendIP , char* buf , int nlen );
    //好友信息
    void slot_DealFriendInfo( unsigned int lSendIP , char* buf , int nlen );




    //其他
    //初始化配置
    void initConfig();
    //发送数据
    void SendData( unsigned int lSendIP , char* buf , int nlen );
    //设置协议
    void setNetMap();
    //退出客户端
    void slot_quitLogin();
    //退出登录
    void slot_quitClogin();

private:
    startDialog* m_startDialog;//启动窗口
    LoginDialog* m_loginDialog;//登录窗口
    RegisterDialog* m_registerDialog;//注册窗口
    mainDialog* m_mainDialog;//主窗口
    createRoomForm* m_createRoomDialog;//创建房间窗口
    roomDialog* m_roomDialog;//房间窗口
    roomListDialog* m_roomListDialog;//房间列表窗口
    TcpClientMediator* m_client;//网络

    PFUN m_netMap[_DEF_PROTOCOL_COUNT];//协议映射表

    QString m_serverIp;//服务端ip地址
    int m_id;//用户id
    int m_roomid;//房间id
    int m_zuowei;//座位号
    AudioRead* audioRead;
    //每个玩家一个播放对象
    std::map<int,AudioWrite*>m_mapSeatToWrite;
    bool m_wolf;//是否为狼人发言
};

#endif // CKERNEL_H
