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
#include"VideoApi/videoread.h"
#include"useritem.h"
#include"chatdialog.h"
#include"chatitem.h"
#include"ziliaodialog.h"
#include"spaceform.h"
#include"commentdialog.h"
#include"commentform.h"
#include"addspacedialog.h"
#include"finddialog.h"
#include"emaildialog.h"
#include"emailmsgform.h"
#include"chatmsgdialog.h"
#include"yaoqingfridialog.h"
#include"yaoqingform.h"
#include"userspacedialog.h"
#include"videoaudiodialog.h"

//////////////////////////////////////////////////////
///
///数据库:
///时间：timestamp     表结构：desc    当前时间：current_time
///
/// t_user 用户信息表：id username password sex icon name level gameNum
///                 用户id 用户名 密码 性别 头像 昵称 等级 游戏场数
///
/// t_space 动态表：id time content userid good tui commentNum
///                 动态id 发布时间 内容 发布人id 点赞数 点踩数 评论个数
///
/// t_friend 好友表：id_a id_b
///                 用户a 用户b
///
/// t_spaceOpt 动态操作表：spaceid userid good tui
///                    动态id 用户id 赞 踩
///
/// t_comment 评论表：spaceid userid content time
///                 动态id 用户id 评论内容 时间
///
/// offLineOpt 离线操作表：userid opt sendid  addContent addResult goodSpaceid spaceComment
///                      接收用户id 操作 操作人id  验证信息 验证结果 点赞动态id 评论信息
///
/// t_historyOpt 历史操作表：同上 chatMsg 离线信息
///
/// t_chatMsg 聊天记录表：sendid recvid time content
///                     发送人id 接收人id 时间 内容
//////////////////////////////////////////////////////

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
    //点击聊天界面的[发消息]->聊天窗口
    void slot_qie_chatItemSend(int id);
    //退出聊天窗口
    void slot_qie_quitChat(int id);
    //从详细资料跳转到发送消息
    void slot_qie_ziliaoToSendMag(int id);
    //显示发布帖子窗口
    void slot_qie_sendSpace();
    //查找好友
//    void slot_qie_findFriend(QString find);
    //邀请好友
    void slot_qie_yaoqingFri(int roomid);



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
    void slot_SendChatMsg(int friendID, QString content);
    void slot_sendUserZiLiaoRq(int id);
    void slot_sendGetSpace(int kind,bool find,int which,QString str,int page);
    void slot_sendSpaceOpt(int spaceid,int userid,int kind,int opt);
    void slot_sendGetComment(int spaceid,int icon,QString name,QString time,QString content,int id);
    void slot_sendSpaceComment(int spaceid,QString content,int masterid);
    void slot_sendAddSpace(QString content);
    void slot_sendFindUser(QString user,int index,int page);
    void slot_sendGetSpaceEmail();
    void slot_sendGetFriEmail();
    void slot_sendClearEmail(bool isspace);
    void slot_sendAddFriend(int id);
    void slot_sendAddResult(int userid,bool add);
    void slot_sendFriendDel(int id);
    void slot_sendGetChatMsg(int id);
    void slot_sendDeleteMsg(int id);
    void slot_sendYaoQingFri(int id,int roomid);
    void slot_sendAudioWith(int friid);
    void slot_sendVideoWith(int friid);
    void slot_sendgetUserSpace(int friid);
    void slot_sendAudioWithFrame(QByteArray& frame);
    void slot_refreshVideoImage(QImage &img);
    void slot_sendVideoFrameData(QByteArray &ba);
    void slot_sendQuitAu(int id);




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
    //发言
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
    //聊天请求
    void slot_DealChatRq( unsigned int lSendIP , char* buf , int nlen );
    //聊天回复
    void slot_DealChatRs( unsigned int lSendIP , char* buf , int nlen );
    //好友资料
    void slot_DealFriendZiLiao( unsigned int lSendIP , char* buf , int nlen );
    //动态信息
    void slot_DealSpace( unsigned int lSendIP , char* buf , int nlen );
    //动态评论
    void slot_DealSpaceComment( unsigned int lSendIP , char* buf , int nlen );
    //查找用户
    void slot_DealUserFind( unsigned int lSendIP , char* buf , int nlen );
    //邮件
    void slot_DealEmailRs( unsigned int lSendIP , char* buf , int nlen );
    //删除好友
    void slot_DealFriendDel( unsigned int lSendIP , char* buf , int nlen );
    //离线或历史聊天记录
    void slot_DealHOChatMsg( unsigned int lSendIP , char* buf , int nlen );
    //邀请
    void slot_DealYaoQing( unsigned int lSendIP , char* buf , int nlen );
    //语音邀请
    void slot_DealAudioRq( unsigned int lSendIP , char* buf , int nlen );
    //视频邀请
    void slot_DealVideoRq( unsigned int lSendIP , char* buf , int nlen );
    //邀请结果
    void slot_DealAudioRs( unsigned int lSendIP , char* buf , int nlen );
    void slot_DealVideoRs( unsigned int lSendIP , char* buf , int nlen );
    //语音聊天数据
    void slot_DealAudioWithFrame( unsigned int lSendIP , char* buf , int nlen );
    void slot_DealVideoWithFrame( unsigned int lSendIP , char* buf , int nlen );
    //挂断
    void slot_DealAuViQuit( unsigned int lSendIP , char* buf , int nlen );




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

    void slot_audioOpen();
    void slot_videoOpen();
    void slot_audioClose();
    void slot_videoClose();

private:
    startDialog*        m_startDialog       ;//启动窗口
    LoginDialog*        m_loginDialog       ;//登录窗口
    RegisterDialog*     m_registerDialog    ;//注册窗口
    mainDialog*         m_mainDialog        ;//主窗口
    createRoomForm*     m_createRoomDialog  ;//创建房间窗口
    roomDialog*         m_roomDialog        ;//房间窗口
    roomListDialog*     m_roomListDialog    ;//房间列表窗口
 TcpClientMediator*     m_client            ;//网络
    ZiLiaoDialog*       m_ziliao            ;//好友详细资料
    commentDialog*      m_comment           ;//评论窗口
    addSpaceDialog*     m_addSpace          ;//发布帖子窗口
    findDialog*         m_find              ;//查找窗口
    emailDialog*        m_spaceEmail        ;//社区邮件窗口
    emailDialog*        m_friendEmail       ;//好友邮件窗口
    ChatMsgDialog*      m_chatmsg           ;//聊天记录窗口
    YaoQingFriDialog*   m_yaoqing           ;//邀请好友窗口
    UserSpaceDialog*    m_userSpace         ;//用户动态窗口
    VideoAudioDialog*   m_viau              ;//语音视频聊天



    PFUN m_netMap[_DEF_PROTOCOL_COUNT];//协议映射表

    QString m_serverIp;//服务端ip地址
    int m_id;//用户id
    int m_roomid;//房间id
    int m_zuowei;//座位号

    //游戏内声音收集
    AudioRead* audioRead;
    //每个玩家一个播放对象
    std::map<int,AudioWrite*>m_mapSeatToWrite;//座位号对应的播放器

    //好友的音频播放器
    AudioRead* m_audioRead;
    AudioWrite* m_audioWrite;
    //视频采集
    VideoRead* m_videoRead;

    std::list<int> m_lstFriend;//好友id列表
    std::map<int,UserItem*>m_mapIdToUserItem;//id对应的好友控件
    std::map<int,ChatDialog*>m_mapIdToChatDialog;//id对应的聊天窗口
    std::map<int,ChatItem*>m_mapIdToChatItem;//id对应的聊天控件
    std::map<int,QString>m_mapIdToName;//id对应的昵称
    bool m_wolf;//是否为狼人发言

    bool getFriEmail;//是否好友获取邮件
    bool getSpaEmail;//是否获取社区邮件
    int withUser;//语音或视频的聊天对象
};

#endif // CKERNEL_H
