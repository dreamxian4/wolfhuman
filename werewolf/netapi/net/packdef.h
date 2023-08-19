#pragma once
#include<QString>

#define _DEF_BUFFER  ( 4096 )
#define _DEF_CONTENT_SIZE	(1024)
#define _MAX_SIZE	(40)

//ok删除好友：客户端发请求，服务端收到后在好友表中删除好友关系，客户端发送后删除好友组件，
//          服务端判断被删人在线的话，发送好友组件删除的请求,客户端收到后，删除该好友
//ok离线信息：在用户登录的时候，服务端判断有没有要发给给用户的离线信息
//     （添加进离线信息就不能添加在历史记录了，在离线信息发送出去并从表中删除的时候，再添加进历史信息）
//      如果有离线信息，先发送历史信息，再发送离线信息，客户端只要收到邮件，就不用再发送邮件请求了
//ok动态消息：点赞评论都要判断主人是否在线，在线发送点赞信息，不在线将信息添加到离线消息，其他同上
//ok动态回复：客户端收到动态的时候，判断主人是否为好友，如果是好友，去掉加好友按钮
//ok动态加好友：在动态页面点击加好友之后，获取验证信息，按钮文本变为已申请，发送信号
//ok聊天：发送聊天信息，判断对方是否在线，在线发送信息、添加信息到聊天记录表；
//                                不在线将信息添加到离线信息表
//      用户登录的时候，如果某好友有离线信息，直接发离线信息；如果没有，从聊天记录表获取十条发过去
//ok聊天记录：客户端有聊天记录按钮，点击可以获取所有聊天记录，其内有一键清除按钮
//游戏内邀请好友：判断头像状态，如果是邀请好友状态，弹出邀请列表，列出所有在线好友，
//             点击邀请，给好友发送邀请信息，好友接收后，给房间发送进入申请
//ok游戏内获取用户信息：判断头像状态，发送资料请求
//ok进入游戏后，关闭所有其他窗口
//服务端保存每局游戏的身份信息，在游戏结束后发送给房内成员
//客户端收到用户资料后，判断是不是好友，是好友显示发消息，不是好友显示加 好友，
//          如果本人在游戏中的话，不能点击发消息(可以弹出动态，写界面)
//把跟举报有关的删掉
//用户个人信息修改
//okbug:同意好友申请后，重新登录，好友申请信息还是有同意拒绝符号
//快速加入房间
//好友进入房间或游戏中给好友发送

//赢了的玩家加20经验 输了的玩家加10经验
//100经验2级，250经验3级，450，700，1000，1350...满级30级：（最后写吧）

////玩法
//视频模式
#define _GAME_MODE_VIDEO (1)
//语音模式
#define _GAME_MODE_AUDIO (0)
//基础玩法
#define _GAME_BASE (0)


//身份牌
//预言家 女巫 平民 狼人 猎人 守卫
//typedef enum {YUYANJIA,NVWU,PINGMIN,LANGREN,LIEREN,SHOUWEI} BASEIDENTIFY;




//自定义协议   先写协议头 再写协议结构
//登录 注册 获取好友信息 添加好友 聊天 发文件 下线请求
#define _DEF_PACK_BASE	(10000)
#define _DEF_PROTOCOL_COUNT (100)

//注册
#define  _DEF_PACK_REGISTER_RQ    (_DEF_PACK_BASE + 0)
#define  _DEF_PACK_REGISTER_RS    (_DEF_PACK_BASE + 1)
//登录
#define  _DEF_PACK_LOGIN_RQ    (_DEF_PACK_BASE + 2)
#define  _DEF_PACK_LOGIN_RS    (_DEF_PACK_BASE + 3)
//强制退出登录请求（服务端发来的）
#define DEF_PACK_QUIT_LOGIN_RQ (_DEF_PACK_BASE + 4)
//用户信息 用于设置和登录时获取状态
#define DEF_PACK_USER_INFO (_DEF_PACK_BASE + 5)
//创建房间
#define DEF_PACK_CREATEROOM_RQ  (_DEF_PACK_BASE + 6)
#define DEF_PACK_CREATEROOM_RS  (_DEF_PACK_BASE + 7)
//客户端退出
#define DEF_PACK_CLIENTQUITLOGIN_RQ  (_DEF_PACK_BASE + 8)
//房间列表请求
#define DEF_PACK_ROOMLIST_RQ    (_DEF_PACK_BASE + 9)
#define DEF_PACK_ROOMLIST_RS    (_DEF_PACK_BASE + 10)
//加入房间
#define DEF_PACK_JOINROOM_RQ  (_DEF_PACK_BASE + 11)
#define DEF_PACK_JOINROOM_RS  (_DEF_PACK_BASE + 12)
//房间成员信息
#define DEF_PACK_ROOM_MEMBER    (_DEF_PACK_BASE + 13)
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ   (_DEF_PACK_BASE + 14)
#define DEF_PACK_LEAVEROOM_RS   (_DEF_PACK_BASE + 15)
//开始游戏
#define DEF_PACK_BEGINGAMETEST_RQ   (_DEF_PACK_BASE + 16)
#define DEF_PACK_BEGINGAMETEST_RS   (_DEF_PACK_BASE + 17)
#define DEF_PACK_BEGINGAME_RQ   (_DEF_PACK_BASE + 18)
#define DEF_PACK_BEGINGAME_RS   (_DEF_PACK_BASE + 19)
//天黑
#define DEF_PACK_SKYBLACK_RQ     (_DEF_PACK_BASE + 20)
#define DEF_PACK_SKYBLACK_RS     (_DEF_PACK_BASE + 21)
//预言信息
#define DEF_PACK_YYJ_SKYBLK     (_DEF_PACK_BASE + 22)
//杀人信息
#define DEF_PACK_LR_SKYBLK     (_DEF_PACK_BASE + 23)
//夜晚结束
#define DEF_PACK_SKYBLACK_END     (_DEF_PACK_BASE + 24)
//发给女巫的杀人信息
#define DEF_PACK_LRTONW_SKYBLK       (_DEF_PACK_BASE + 25)
#define DEF_PACK_LRTONW_SKYBLK_RS       (_DEF_PACK_BASE + 26)
//天亮
#define DEF_PACK_SKYWHT_RQ       (_DEF_PACK_BASE + 27)
#define DEF_PACK_SKYWHT_RS       (_DEF_PACK_BASE + 28)
//发言
#define DEF_PACK_SPEAK_RQ       (_DEF_PACK_BASE + 29)
#define DEF_PACK_SPEAK_RS       (_DEF_PACK_BASE + 30)
//竞选警长
#define DEF_PACK_TOBEPOLICE_RQ     (_DEF_PACK_BASE + 31)
#define DEF_PACK_TOBEPOLICE_RS     (_DEF_PACK_BASE + 32)
//成为警长
#define DEF_PACK_BEPOLICE_RQ        (_DEF_PACK_BASE + 33)
#define DEF_PACK_BEPOLICE_RS        (_DEF_PACK_BASE + 34)
//竞选阶段结束
#define DEF_PACK_POLICEEND     (_DEF_PACK_BASE + 35)
//发言阶段开始
#define DEF_PACK_SPEAKSTATE_BEGIN       (_DEF_PACK_BASE + 36)
//发言阶段结束（开始投票）
#define DEF_PACK_SPEAKSTATE_END     (_DEF_PACK_BASE + 37)
//玩家投票
#define DEF_PACK_VOTE_RQ    (_DEF_PACK_BASE + 38)
//投票结果
#define DEF_PACK_VOTE_RS     (_DEF_PACK_BASE + 39)
//投票阶段结束
#define DEF_PACK_VOTE_END     (_DEF_PACK_BASE + 40)
//警长选择发言顺序
# define DEF_PACK_SPEAK_ORDER       (_DEF_PACK_BASE + 41)
//白天投票放逐信息
#define DEF_PACK_DAY_EXILE         (_DEF_PACK_BASE + 42)
//游戏结束
#define DEF_PACK_GAMEOVER           (_DEF_PACK_BASE + 43)
//音频数据
#define DEF_PACK_AUDIO_FRAME    (_DEF_PACK_BASE + 44)
////视频数据
//#define DEF_PACK_VIDEO_FRAME    (_DEF_PACK_BASE + 10)
//音频注册
#define DEF_PACK_AUDIO_REGISTER (_DEF_PACK_BASE + 45)
////视频注册
//#define DEF_PACK_VIDEO_REGISTER (_DEF_PACK_BASE + 14)
//暂停发言
#define DEF_PACK_SPEAK_PAUSE       (_DEF_PACK_BASE + 46)
//狼人自爆
#define DEF_PACK_LR_KILLSELF       (_DEF_PACK_BASE + 47)
//好友信息
#define DEF_PACK_FRIEND_INFO        (_DEF_PACK_BASE + 48)
//聊天请求
#define DEF_PACK_CHAT_RQ       (_DEF_PACK_BASE + 49)
#define DEF_PACK_CHAT_RS       (_DEF_PACK_BASE + 50)
//好友详细信息
#define DEF_PACK_FRIEND_ZILIAO_RQ       (_DEF_PACK_BASE + 51)
#define DEF_PACK_FRIEND_ZILIAO_RS       (_DEF_PACK_BASE + 52)
//动态
#define DEF_PACK_SPACE_RQ       (_DEF_PACK_BASE+53)
#define DEF_PACK_SPACE_RS       (_DEF_PACK_BASE+54)
#define DEF_PACK_SPACE_OPT      (_DEF_PACK_BASE+55)
#define DEF_PACK_SPACE_COMMENT_RQ  (_DEF_PACK_BASE+56)
#define DEF_PACK_SPACE_COMMENT_RS  (_DEF_PACK_BASE+57)
#define DEF_PACK_SPACE_ADD      (_DEF_PACK_BASE+58)
//查找用户
#define DEF_PACK_USER_FIND_RQ      (_DEF_PACK_BASE+59)
#define DEF_PACK_USER_FIND_RS      (_DEF_PACK_BASE+60)
//邮件
#define DEF_PACK_EMAIL_RQ       (_DEF_PACK_BASE+61)
#define DEF_PACK_EMAIL_RS       (_DEF_PACK_BASE+62)
#define DEF_PACK_EMAIL_CLEAR    (_DEF_PACK_BASE+63)
//添加好友
#define DEF_PACK_FRIEND_ADD_RQ     (_DEF_PACK_BASE+64)
#define DEF_PACK_FRIEND_ADD_RS     (_DEF_PACK_BASE+65)
//删除好友
#define DEF_PACK_FRIEND_DELETE      (_DEF_PACK_BASE+66)
//离线信息/历史信息
#define DEF_PACK_CHAT_HISorOFF      (_DEF_PACK_BASE+67)
//聊天记录
#define DEF_PACK_CHAT_MSG      (_DEF_PACK_BASE+68)
//邀请进入房间
#define DEF_PACK_YAOQING        (_DEF_PACK_BASE+69)
//视频/语音邀请
#define DEF_PACK_AUDIO_RQ     (_DEF_PACK_BASE+70)
#define DEF_PACK_AUDIO_RS     (_DEF_PACK_BASE+71)
#define DEF_PACK_VIDEO_RQ     (_DEF_PACK_BASE+72)
#define DEF_PACK_VIDEO_RS     (_DEF_PACK_BASE+73)
//音频数据
#define DEF_PACK_AUDIO_WITH_FRAME   (_DEF_PACK_BASE+74)
#define DEF_PACK_VIDEO_WITH_FRAME   (_DEF_PACK_BASE+75)
//挂断
#define DEF_PACK_AUVI_QUIT          (_DEF_PACK_BASE+76)




//注册请求结果
#define username_is_exist		(0)
#define register_success	(1)
//#define name_is_exist       (2)

//登录请求结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)
//#define user_online         (3)

//创建房间结果
#define room_is_maxed        0
#define create_success       1

//加入房间结果
#define room_no_exist        0
#define join_success         1
#define level_unqualified    2

//信息发送结果
#define send_success 0
#define send_error 1

//好友状态 1：在线 2：离线 3：游戏中 4：房间中
#define online 1
#define offline 2
#define playing 3
#define inroom 4

typedef int PackType;

//协议结构
//注册
typedef struct STRU_REGISTER_RQ
{
    //用户名唯一
    STRU_REGISTER_RQ():type(_DEF_PACK_REGISTER_RQ)
    {
        memset( username  , 0, sizeof(username));
        memset( password , 0, sizeof(password) );
        memset( sex , 0, sizeof(sex) );
        memset( name , 0, sizeof(name) );
    }
    //用户名、密码、性别、昵称、出生日月
    PackType type;
    char username[_MAX_SIZE];
    char password[_MAX_SIZE];
    char sex[_MAX_SIZE];
    char name[_MAX_SIZE];
    //    int month;
    //    int day;

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
    //回复结果
    STRU_REGISTER_RS(): type(_DEF_PACK_REGISTER_RS) , result(register_success)
    {
        memset( sex , 0, sizeof(sex) );
        memset( name , 0, sizeof(name) );
    }
    //
    //昵称、性别
    PackType type;
    int result;
    char sex[_MAX_SIZE];
    char name[_MAX_SIZE];

}STRU_REGISTER_RS;

//登录
typedef struct STRU_LOGIN_RQ
{
    //登录需要: 手机号 密码
    STRU_LOGIN_RQ():type(_DEF_PACK_LOGIN_RQ)
    {
        memset( username , 0, sizeof(username) );
        memset( password , 0, sizeof(password) );
    }
    PackType type;
    char username[_MAX_SIZE];
    char password[_MAX_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS
{
    // 需要 结果 , 用户的id
    STRU_LOGIN_RS(): type(_DEF_PACK_LOGIN_RS) , result(login_success),userid(0)
    {
    }
    PackType type;
    int userid;
    int result;

}STRU_LOGIN_RS;


//用户信息请求，登录之后状态同步,设置用户信息
typedef struct STRU_USER_INFO_RQ
{
    STRU_USER_INFO_RQ()
    {
        m_nType= DEF_PACK_USER_INFO;
        m_UserID =0;
        m_iconid=0;
        m_level=0;
        memset(m_username,0,_MAX_SIZE);
        memset(m_sex,0,_MAX_SIZE);
        memset(m_name,0,_MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int m_UserID;
    int m_iconid;
    int m_level;
    char m_username[_MAX_SIZE];
    char m_sex[_MAX_SIZE];
    char m_name[_MAX_SIZE];

}STRU_USER_INFO_RQ;

//强制退出请求
typedef struct STRU_QUITLOGIN_RQ
{
    STRU_QUITLOGIN_RQ()
    {
        m_nType= DEF_PACK_QUIT_LOGIN_RQ;
    }
    PackType m_nType;   //包类型

}STRU_QUITLOGIN_RQ;

//创建房间请求
typedef struct STRU_CREATEROOM_RQ
{
    STRU_CREATEROOM_RQ()
    {
        m_nType = DEF_PACK_CREATEROOM_RQ;
        m_UserID = 0;
        mode       =0;
        playMethod =0;
        level      =0;
        lock       =0;
        maxcount      =0;
        memset(passwd,0,_MAX_SIZE);
    }

    //模式（语音、视频）、玩法（基础、进阶）、人数、等级限制（无限制、xx级）、加密（是、否）、密码
    //语音的基础一个表、视频的基础一个表、语音的进阶一个表、...
    PackType m_nType;   //包类型
    int m_UserID;
    int  mode       ;
    int  playMethod ;
    int  level      ;
    bool lock       ;
    int maxcount;
    char passwd[_MAX_SIZE];

}STRU_CREATEROOM_RQ;

//创建房间回复
typedef struct STRU_CREATEROOM_RS
{
    STRU_CREATEROOM_RS()
    {
        m_nType= DEF_PACK_CREATEROOM_RS;
        m_lResult = 0;
        m_RoomId = 0;
        mode       =0;
        lock       =0;
        maxcount      =0;
        memset(passwd,0,_MAX_SIZE);
    }
    //房间号、模式、人数、加密、密码
    PackType m_nType;   //包类型
    int  m_lResult ;    //创建结果
    int  m_RoomId;
    int  mode       ;
    bool lock       ;
    int maxcount;
    char passwd[_MAX_SIZE];

}STRU_CREATEROOM_RS;


//退出登录
typedef struct STRU_CLIENTQUITLOGIN_RQ
{
    STRU_CLIENTQUITLOGIN_RQ()
    {
        m_nType = DEF_PACK_CLIENTQUITLOGIN_RQ;
        m_UserID = 0;
        roomid=0;
    }
    PackType m_nType;   //包类型
    int m_UserID;
    int roomid;

}STRU_CLIENTQUITLOGIN_RQ;


//房间列表请求
typedef struct STRU_ROOMLIST_RQ
{
    STRU_ROOMLIST_RQ()
    {
        m_nType = DEF_PACK_ROOMLIST_RQ;
        m_UserID = 0;
        mode=0;
        method=0;
        roomid=0;
    }
    PackType m_nType;   //包类型
    int mode;//模式
    int method;//玩法
    int roomid;//点搜索给roomid赋值，点刷新roomid为0
    int m_UserID;

}STRU_ROOMLIST_RQ;

//房间列表回复
typedef struct STRU_ROOMLIST_RS
{
    STRU_ROOMLIST_RS()
    {
        m_nType = DEF_PACK_ROOMLIST_RS;
        mode=0;
        method=0;
        roomid=0;
        count      =0;
        currentCou =0;
        pass       =false;
        memset(roomName,0,_MAX_SIZE);
        memset(passwd,0,_MAX_SIZE);
        state=false;
        level=0;
    }
    PackType m_nType;   //包类型
    int     mode        ;//模式
    int     method      ;//玩法
    int     roomid      ;
    char    roomName[_MAX_SIZE]    ;
    int     count       ;
    int     currentCou  ;
    bool    pass        ;
    char    passwd[_MAX_SIZE];
    bool state;
    int level;

}STRU_ROOMLIST_RS;


//加入房间请求
typedef struct STRU_JOINROOM_RQ
{
    STRU_JOINROOM_RQ()
    {
        m_nType = DEF_PACK_JOINROOM_RQ;
        m_UserID = 0;
        m_RoomID = 0;
    }

    PackType m_nType;   //包类型
    int m_UserID;
    int m_RoomID;

}STRU_JOINROOM_RQ;

//加入房间回复
typedef struct STRU_JOINROOM_RS
{
    STRU_JOINROOM_RS()
    {
        m_nType= DEF_PACK_JOINROOM_RS;
        m_lResult = 0;
        m_RoomID = 0;
        m_lResult =0;
        m_RoomID  =0;
        mode      =0;
        lock      =false;
        maxcount  =0;
        memset(passwd,0,_MAX_SIZE);
        look      =false;
    }
    PackType m_nType;   //包类型
    int  m_lResult  ;    //注册结果
    int  m_RoomID    ;
    int  mode       ;
    bool lock       ;
    int  maxcount    ;
    char passwd         [_MAX_SIZE];
    bool look       ;          //是否观战
}STRU_JOINROOM_RS;

//房间成员请求
typedef struct STRU_ROOM_MEMBER_RQ
{
    STRU_ROOM_MEMBER_RQ()
    {
        m_nType= DEF_PACK_ROOM_MEMBER;
        m_UserID =0;
        memset(m_szUser,0,_MAX_SIZE);
        memset(m_sex,0,_MAX_SIZE);
        m_icon=0;
        m_level=0;
        m_seat=0;
    }
    //头像、昵称、id、等级、性别、座位号
    PackType m_nType;   //包类型
    int m_UserID;
    char m_szUser[_MAX_SIZE];
    int     m_icon;
    int     m_level;
    char    m_sex[_MAX_SIZE];
    int     m_seat;
}STRU_ROOM_MEMBER_RQ;


//离开房间请求
typedef struct STRU_LEAVEROOM_RQ
{
    STRU_LEAVEROOM_RQ()
    {
        m_nType = DEF_PACK_LEAVEROOM_RQ;
        m_nUserId = 0;
        m_RoomId = 0;
    }
    PackType   m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_RoomId;
}STRU_LEAVEROOM_RQ;

typedef struct STRU_LEAVEROOM_RS
{
    STRU_LEAVEROOM_RS()
    {
        m_nType = DEF_PACK_LEAVEROOM_RS;
        m_roomisExist = true;
        m_id = 0;
    }
    PackType   m_nType;   //包类型
    bool m_roomisExist;
    int m_id;
}STRU_LEAVEROOM_RS;


//准备开始游戏
typedef struct STRU_BEGINGAMETEST_RQ
{
    STRU_BEGINGAMETEST_RQ()
    {
        m_nType = DEF_PACK_BEGINGAMETEST_RQ;
        m_RoomId = 0;
    }
    PackType   m_nType;   //包类型
    int    m_RoomId;
}STRU_BEGINGAMETEST_RQ;


typedef struct STRU_BEGINGAMETEST_RS
{
    STRU_BEGINGAMETEST_RS()
    {
        m_nType = DEF_PACK_BEGINGAMETEST_RS;
    }
    PackType   m_nType;   //包类型
}STRU_BEGINGAMETEST_RS;


//正式开始游戏
typedef struct STRU_BEGINGAME_RQ
{
    STRU_BEGINGAME_RQ()
    {
        m_nType = DEF_PACK_BEGINGAME_RQ;
        m_RoomId = 0;
    }
    PackType   m_nType;   //包类型
    int    m_RoomId;
}STRU_BEGINGAME_RQ;


typedef struct STRU_BEGINGAME_RS
{
    STRU_BEGINGAME_RS()
    {
        m_nType = DEF_PACK_BEGINGAME_RS;
        m_iden=0;
    }
    PackType   m_nType;   //包类型
    int m_iden;
}STRU_BEGINGAME_RS;


//天黑
typedef struct STRU_SKYBLACK_RQ
{
    STRU_SKYBLACK_RQ()
    {
        m_nType = DEF_PACK_SKYBLACK_RQ;
    }
    PackType   m_nType;   //包类型
}STRU_SKYBLACK_RQ;


typedef struct STRU_SKYBLACK_RS
{
    STRU_SKYBLACK_RS()
    {
        m_nType = DEF_PACK_SKYBLACK_RS;
        m_iden=0;
        m_roomid=0;
        m_iden     =0;
        m_seat     =0;
        m_operate  =0;
        m_toseat   =0;
    }
    PackType   m_nType;   //包类型
    int m_roomid;//房间号
    int m_iden      ;//我的身份
    int m_seat      ;//我的座位号
    int m_operate   ;//我进行的操作 1预言家 2狼人  3女巫毒 4守卫 0无操作
    int m_toseat    ;//我对谁操作
}STRU_SKYBLACK_RS;


//预言家预言信息
typedef struct STRU_YYJ_SKYBLK
{
    STRU_YYJ_SKYBLK()
    {
        m_nType = DEF_PACK_YYJ_SKYBLK;
        id=0;
        iden=0;
    }
    PackType   m_nType;   //包类型
    int id;
    int iden;
}STRU_YYJ_SKYBLK;

//狼人杀人信息
typedef struct STRU_LR_SKYBLK
{
    STRU_LR_SKYBLK()
    {
        m_nType = DEF_PACK_LR_SKYBLK;
        id=0;
        toid=0;
    }
    PackType   m_nType;   //包类型
    int id;
    int toid;
}STRU_LR_SKYBLK;



//夜晚结束
typedef struct STRU_SKYBLK_END
{
    STRU_SKYBLK_END()
    {
        m_nType = DEF_PACK_SKYBLACK_END;
        state=0;
        roomid=0;
    }
    PackType   m_nType;   //包类型
    int state;//0:前15s  1:夜晚结束
    int roomid;
}STRU_SKYBLK_END;


//狼人的确切杀人信息
typedef struct STRU_LRTONW_SKYBLK
{
    STRU_LRTONW_SKYBLK()
    {
        m_nType = DEF_PACK_LRTONW_SKYBLK;
        kill=0;
    }
    PackType   m_nType;   //包类型
    int kill;
}STRU_LRTONW_SKYBLK;

//杀人信息回复：女巫救人
typedef struct STRU_LRTONW_SKYBLK_RS
{
    STRU_LRTONW_SKYBLK_RS()
    {
        m_nType = DEF_PACK_LRTONW_SKYBLK_RS;
        roomid=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
}STRU_LRTONW_SKYBLK_RS;


//天亮了，包含死亡信息
typedef struct STRU_SKYWHT_RQ
{
    STRU_SKYWHT_RQ()
    {
        m_nType = DEF_PACK_SKYWHT_RQ;
        memset(die,0,8);
    }
    PackType   m_nType;   //包类型
    int die[2];
}STRU_SKYWHT_RQ;

typedef struct STRU_SKYWHT_RS
{
    STRU_SKYWHT_RS()
    {
        m_nType = DEF_PACK_SKYWHT_RS;
        roomid=0;
        iden=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int iden;
}STRU_SKYWHT_RS;


//发言
typedef struct STRU_SPEAK_RQ
{
    STRU_SPEAK_RQ()
    {
        m_nType = DEF_PACK_SPEAK_RQ;
        state=0;
        seat=0;
        next=0;
    }
    PackType   m_nType;   //包类型
    int state;//1:uppolice 2:nopolice 3:normal 4:exile
    int seat;
    int next;
}STRU_SPEAK_RQ;

typedef struct STRU_SPEAK_RS
{
    STRU_SPEAK_RS()
    {
        m_nType = DEF_PACK_SPEAK_RS;
        roomid=0;
        seat=0;
        next=0;
        state=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int seat;
    int next;
    int state;//1:uppolice 2:nopolice 3:normal
}STRU_SPEAK_RS;


//竞选警长
typedef struct STRU_TOBEPOLICE_RQ
{
    STRU_TOBEPOLICE_RQ()
    {
        m_nType = DEF_PACK_TOBEPOLICE_RQ;
    }
    PackType   m_nType;   //包类型
}STRU_TOBEPOLICE_RQ;

typedef struct STRU_TOBEPOLICE_RS
{
    STRU_TOBEPOLICE_RS()
    {
        m_nType = DEF_PACK_TOBEPOLICE_RS;
        roomid=0;
        seat=0;
        raise=true;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int seat;
    bool raise;//是举手还是放手
}STRU_TOBEPOLICE_RS;


//成为警长
typedef struct STRU_BEPOLICE_RQ
{
    STRU_BEPOLICE_RQ()
    {
        m_nType = DEF_PACK_BEPOLICE_RQ;
    }
    PackType   m_nType;   //包类型
}STRU_BEPOLICE_RQ;

typedef struct STRU_BEPOLICE_RS
{
    STRU_BEPOLICE_RS()
    {
        m_nType = DEF_PACK_BEPOLICE_RS;
        roomid=0;
        seat=0;
        state=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int seat;
    int state;//阶段：1当选（接下来选择发言顺序）/2移交（接下来进入黑夜）
}STRU_BEPOLICE_RS;



//竞选警长阶段结束(选择完毕)
typedef struct STRU_POLICE_END
{
    STRU_POLICE_END()
    {
        m_nType = DEF_PACK_POLICEEND;
        roomid=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
}STRU_POLICE_END;


//发言阶段开始
typedef struct STRU_SPEAKSTATE_BEGIN
{
    STRU_SPEAKSTATE_BEGIN()
    {
        m_nType = DEF_PACK_SPEAKSTATE_BEGIN;
    }
    PackType   m_nType;   //包类型
}STRU_SPEAKSTATE_BEGIN;


//发言阶段结束（开始投票包）
typedef struct STRU_SPEAKSTATE_END
{
    STRU_SPEAKSTATE_END()
    {
        m_nType = DEF_PACK_SPEAKSTATE_END;
        roomid=0;
        state=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int state;//1:上警 2:白天 3:平票
}STRU_SPEAKSTATE_END;


//警长选择发言顺序
typedef struct STRU_SPEAK_ORDER
{
    STRU_SPEAK_ORDER()
    {
        m_nType = DEF_PACK_SPEAK_ORDER;
        roomid=0;
        seat=0;
        next=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int seat;//警长座位号
    int next;//发言顺序
}STRU_SPEAK_ORDER;


//玩家投票包
typedef struct STRU_VOTE_RQ
{
    STRU_VOTE_RQ()
    {
        m_nType = DEF_PACK_VOTE_RQ;
        roomid=0;
        state=0;
        seat=0;
        toseat=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int state;//1:票选警长 2:放逐投票
    int seat;
    int toseat;
}STRU_VOTE_RQ;

//投票结果包
typedef struct STRU_VOTE_RS
{
    STRU_VOTE_RS()
    {
        m_nType = DEF_PACK_VOTE_RS;
        state=0;
        memset(result,0,sizeof(int)*12);
    }
    PackType   m_nType;   //包类型
    int state;//1:上警
    int result[13];
}STRU_VOTE_RS;


//投票阶段结束
typedef struct STRU_VOTE_END
{
    STRU_VOTE_END()
    {
        m_nType = DEF_PACK_VOTE_END;
        roomid=0;
        state=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int state;//1:上警
}STRU_VOTE_END;



//白天投票放逐信息
typedef struct STRU_DAY_EXILE
{
    STRU_DAY_EXILE()
    {
        m_nType = DEF_PACK_DAY_EXILE;
        roomid=0;
        die=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int die;
}STRU_DAY_EXILE;



//游戏结束
typedef struct STRU_GAMEOVER
{
    STRU_GAMEOVER()
    {
        m_nType = DEF_PACK_GAMEOVER;
    }
    PackType   m_nType;   //包类型
}STRU_GAMEOVER;



//暂停发言
typedef struct STRU_SPEAKPAUSE
{
    STRU_SPEAKPAUSE()
    {
        m_nType = DEF_PACK_SPEAK_PAUSE;
        roomid=0;
        seat=0;
        wolf=false;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int seat;
    bool wolf;//是否为狼人夜间发言
}STRU_SPEAKPAUSE;


//狼人自爆
typedef struct STRU_LR_KILLSELF
{
    STRU_LR_KILLSELF()
    {
        m_nType = DEF_PACK_LR_KILLSELF;
        roomid=0;
        seat=0;
    }
    PackType   m_nType;   //包类型
    int roomid;
    int seat;
}STRU_LR_KILLSELF;


//好友信息
typedef struct STRU_FRIEND_INFO {
    //协议头 好友id 头像 状态 手机号 昵称 签名
    STRU_FRIEND_INFO() :type(DEF_PACK_FRIEND_INFO), userid(0), icon(0), state(0) {
        memset(username, 0, _MAX_SIZE);
        memset(name, 0, _MAX_SIZE);
        memset(sex, 0, _MAX_SIZE);
        level=0;
    }
    PackType type;
    int userid;
    int icon;
    int state;//1：在线 2：离线 3：游戏中 4：房间中
    char username[_MAX_SIZE];
    char name[_MAX_SIZE];
    char level;
    char sex[_MAX_SIZE];
}STRU_FRIEND_INFO;


//聊天
typedef struct STRU_CHAT_RQ {
    //协议头  内容 发送人id 接收人id
    STRU_CHAT_RQ() :type(DEF_PACK_CHAT_RQ), userid(0), friendid(0) {
        memset(content, 0, _DEF_CONTENT_SIZE);
    }
    PackType type;
    int userid;
    int friendid;
    char content[_DEF_CONTENT_SIZE];
}STRU_CHAT_RQ;

typedef struct STRU_CHAT_RS {
    //协议头 结果
    STRU_CHAT_RS() :type(DEF_PACK_CHAT_RS), userid(0), friendid(0), result(send_success) {}
    PackType type;
    int userid;
    int friendid;
    int result;
}STRU_CHAT_RS;



//好友详细资料
typedef struct STRU_FRIEND_ZILIAO_RQ {
    //协议头  内容 发送人id 接收人id
    STRU_FRIEND_ZILIAO_RQ() :type(DEF_PACK_FRIEND_ZILIAO_RQ),friendid(0) {
    }
    PackType type;
    int friendid;
}STRU_FRIEND_ZILIAO_RQ;

typedef struct STRU_FRIEND_ZILIAO_RS {
    //协议头 结果
    STRU_FRIEND_ZILIAO_RS() :type(DEF_PACK_FRIEND_ZILIAO_RS), friendid(0) {
        icon=0;
        roomid=0;
        level=0;
        gameNum=0;
//        space=false;
        state=0;
        memset(name, 0, _MAX_SIZE);
        memset(username, 0, _MAX_SIZE);
        memset(sex, 0, _MAX_SIZE);
//        memset(content, 0, _DEF_CONTENT_SIZE);
//        memset(time, 0, _MAX_SIZE);
    }
    PackType type;
    int friendid;
    int icon;
    char name[_MAX_SIZE];
    int state;//状态 //1：在线 2：离线 3：游戏中 4：房间中
    int roomid;
    char username[_MAX_SIZE];
    char sex[_MAX_SIZE];
    int level;
    int gameNum;//游戏次数
//    bool space;//是否有动态
//    char content[_DEF_CONTENT_SIZE];//动态内容
//    char time[_MAX_SIZE];
}STRU_FRIEND_ZILIAO_RS;


//动态
typedef struct STRU_SPACE_RQ {
    STRU_SPACE_RQ() :type(DEF_PACK_SPACE_RQ) {
        kind=0;
        id=0;
        page=0;
        memset(str, 0, _MAX_SIZE);
        find=false;
        which=0;
        user=false;
        beiid=0;
    }
    PackType type;
    int kind;
    int id;
    int page;
    char str[_MAX_SIZE];
    bool find;
    bool user;
    int which;
    int beiid;
}STRU_SPACE_RQ;

typedef struct STRU_SPACE_RS {
    //协议头 结果
    STRU_SPACE_RS() :type(DEF_PACK_SPACE_RS){
        icon=0;
        userid=0;
        spaceid=0;
        tui=0;
        good=0;
        comment=0;
        spaceNum=0;
        isgood=0;
        istui=0;
        memset(name, 0, _MAX_SIZE);
        memset(content, 0, _DEF_CONTENT_SIZE);
        memset(time, 0, _MAX_SIZE);
        user=false;
    }
    PackType type;
    int icon;
    int userid;
    int spaceid;
    char name[_MAX_SIZE];
    int tui;
    int good;
    int comment;
    int spaceNum;
    int isgood;
    int istui;
    char content[_DEF_CONTENT_SIZE];//动态内容
    char time[_MAX_SIZE];
    bool user;
}STRU_SPACE_RS;

//对动态的操作
typedef struct STRU_SPACE_OPT {
    //协议头 结果
    STRU_SPACE_OPT() :type(DEF_PACK_SPACE_OPT){
        spaceid=0;
        userid=0;
        masterid=0;
        kind=0;
        opt=0;
        memset(comment, 0, _DEF_CONTENT_SIZE);
    }
    PackType type;
    int spaceid;
    int userid;
    int masterid;
    int kind;
    int opt;
    char comment[_DEF_CONTENT_SIZE];
}STRU_SPACE_OPT;



//获取评论
typedef struct STRU_SPACE_COMMENT_RQ {
    //协议头 结果
    STRU_SPACE_COMMENT_RQ() :type(DEF_PACK_SPACE_COMMENT_RQ){
        spaceid=0;
    }
    PackType type;
    int spaceid;
}STRU_SPACE_COMMENT_RQ;

typedef struct STRU_SPACE_COMMENT_RS {
    //协议头 结果
    STRU_SPACE_COMMENT_RS() :type(DEF_PACK_SPACE_COMMENT_RS){
//        commentid=0;
        memset(name,0,_MAX_SIZE);
        memset(time,0,_MAX_SIZE);
        memset(comment,0,_DEF_CONTENT_SIZE);
    }
    PackType type;
//    int commentid;
    char name[_MAX_SIZE];
    char time[_MAX_SIZE];
    char comment[_DEF_CONTENT_SIZE];
}STRU_SPACE_COMMENT_RS;



//发布动态
typedef struct STRU_SPACE_ADD {
    //协议头 结果
    STRU_SPACE_ADD() :type(DEF_PACK_SPACE_ADD){
        userid=0;
        memset(content, 0, _DEF_CONTENT_SIZE);
    }
    PackType type;
    int userid;
    char content[_DEF_CONTENT_SIZE];
}STRU_SPACE_ADD;


//查找用户
typedef struct STRU_USER_FIND_RQ {
    STRU_USER_FIND_RQ() :type(DEF_PACK_USER_FIND_RQ) {
        id=0;
        page=0;
        memset(str, 0, _MAX_SIZE);
        which=0;
    }
    PackType type;
    int id;
    int page;
    char str[_MAX_SIZE];
    int which;
}STRU_USER_FIND_RQ;

typedef struct STRU_USER_FIND_RS {
    STRU_USER_FIND_RS() :type(DEF_PACK_USER_FIND_RS), userid(0), icon(0) {
        memset(username, 0, _MAX_SIZE);
        memset(name, 0, _MAX_SIZE);
        memset(sex, 0, _MAX_SIZE);
        level=0;
    }
    PackType type;
    int userid;
    int icon;
//    int state;//1：在线 2：离线 3：游戏中 4：房间中
    char username[_MAX_SIZE];
    char name[_MAX_SIZE];
    char level;
    char sex[_MAX_SIZE];
}STRU_USER_FIND_RS;


//邮件
typedef struct STRU_EMAIL_RQ {
    STRU_EMAIL_RQ() :type(DEF_PACK_EMAIL_RQ) {
        id=0;
        space=false;
    }
    PackType type;
    int id;
    bool space;
}STRU_EMAIL_RQ;

typedef struct STRU_EMAIL_RS {
    STRU_EMAIL_RS() :type(DEF_PACK_EMAIL_RS), userid(0), icon(0) {
        which=0;
        result=false;
        NEW=false;
        memset(name, 0, _MAX_SIZE);
        memset(msg, 0, _DEF_CONTENT_SIZE);
        memset(spaceCnt, 0, _DEF_CONTENT_SIZE);
    }
    PackType type;
    int which;//2好友申请信息 3申请结果 4点赞动态 5评论动态
    int icon;
//    int state;//1：在线 2：离线 3：游戏中 4：房间中
    char name[_MAX_SIZE];
    int userid;
    bool result;
    bool NEW;
    char spaceCnt[_DEF_CONTENT_SIZE];
    char msg[_DEF_CONTENT_SIZE];
}STRU_EMAIL_RS;

//清空邮件
typedef struct STRU_EMAIL_CLEAR {
    //协议头 结果
    STRU_EMAIL_CLEAR() :type(DEF_PACK_EMAIL_CLEAR){
        userid=0;
        space=false;
    }
    PackType type;
    int userid;
    bool space;
}STRU_EMAIL_CLEAR;


//添加好友
typedef struct STRU_FRIEND_ADD_RQ {
    STRU_FRIEND_ADD_RQ() :type(DEF_PACK_FRIEND_ADD_RQ) {
        id=0;
        beiid=0;
        memset(content,0,_DEF_CONTENT_SIZE);
    }
    PackType type;
    int id;
    int beiid;
    char content[_DEF_CONTENT_SIZE];
}STRU_FRIEND_ADD_RQ;

typedef struct STRU_FRIEND_ADD_RS {
    STRU_FRIEND_ADD_RS() :type(DEF_PACK_FRIEND_ADD_RS) {
        id=0;
        beiid=0;
        bool result;
    }
    PackType type;
    int id;
    int beiid;
    bool result;
}STRU_FRIEND_ADD_RS;


//离线或历史信息
typedef struct STRU_CHAT_HO {
    //协议头  内容 发送人id 接收人id
    STRU_CHAT_HO() :type(DEF_PACK_CHAT_HISorOFF), userid(0){
        memset(content, 0, _DEF_CONTENT_SIZE);
        me=false;
        his=false;
        msg=false;
        memset(time,0,_MAX_SIZE);
    }
    PackType type;
    int userid;
    bool me;
    bool his;
    bool msg;
    char content[_DEF_CONTENT_SIZE];
    char time[_MAX_SIZE];
}STRU_CHAT_HO;


//删除好友
typedef struct STRU_FRIEND_DELETE {
    //协议头 结果
    STRU_FRIEND_DELETE() :type(DEF_PACK_FRIEND_DELETE){
        userid=0;
        beiid=0;
    }
    PackType type;
    int userid;
    int beiid;
}STRU_FRIEND_DELETE;


//聊天记录
typedef struct STRU_CHAT_MSG {
    //协议头 结果
    STRU_CHAT_MSG() :type(DEF_PACK_CHAT_MSG){
        userid=0;
        beiid=0;
        del=false;
    }
    PackType type;
    int userid;
    int beiid;
    bool del;
}STRU_CHAT_MSG;


//邀请进入房间
typedef struct STRU_YAOQING {
    //协议头 结果
    STRU_YAOQING() :type(DEF_PACK_YAOQING){
        userid=0;
        beiid=0;
        roomid=0;
    }
    PackType type;
    int userid;
    int beiid;
    int roomid;
}STRU_YAOQING;


//语音视频邀请
typedef struct STRU_AUDIO_RQ {
    //协议头 结果
    STRU_AUDIO_RQ() :type(DEF_PACK_AUDIO_RQ){
        userid=0;
        beiid=0;
    }
    PackType type;
    int userid;
    int beiid;
}STRU_AUDIO_RQ;

typedef struct STRU_AUDIO_RS {
    //协议头 结果
    STRU_AUDIO_RS() :type(DEF_PACK_AUDIO_RS){
        userid=0;
        beiid=0;
        result=false;
    }
    PackType type;
    int userid;
    int beiid;
    bool result;
}STRU_AUDIO_RS;

typedef struct STRU_VIDEO_RQ {
    //协议头 结果
    STRU_VIDEO_RQ() :type(DEF_PACK_VIDEO_RQ){
        userid=0;
        beiid=0;
    }
    PackType type;
    int userid;
    int beiid;
}STRU_VIDEO_RQ;

typedef struct STRU_VIDEO_RS {
    //协议头 结果
    STRU_VIDEO_RS() :type(DEF_PACK_VIDEO_RS){
        userid=0;
        beiid=0;
        result=false;
    }
    PackType type;
    int userid;
    int beiid;
    bool result;
}STRU_VIDEO_RS;

typedef struct STRU_AUVI_QUIT {
    //协议头 结果
    STRU_AUVI_QUIT() :type(DEF_PACK_AUVI_QUIT){
        userid=0;
        beiid=0;
    }
    PackType type;
    int userid;
    int beiid;
}STRU_AUVI_QUIT;



////注册音频
//struct STRU_AUDIO_REGISTER
//{
//    STRU_AUDIO_REGISTER():m_nType(DEF_PACK_AUDIO_REGISTER){
//        m_userid = 0;
//    }

//    PackType m_nType;   //包类型
//    int m_userid;
//};

////注册视频
//struct STRU_VIDEO_REGISTER
//{
//    STRU_VIDEO_REGISTER():m_nType(DEF_PACK_VIDEO_REGISTER){
//        m_userid = 0;
//    }

//    PackType m_nType;   //包类型
//    int m_userid;
//};

//// 数据库
///*
//	表1 用户表 id->唯一标识 tel  password  name  iconid   feeling 也可以写成两张表 登录和用户信息
//	列:  id  tel password  name  iconid  feeling

//	表2 好友  idA  idB
//	列 idA  idB
//	操作: 好友关系是双向关系  关注是单向关系  , 添加好友时 , 需要写入两行 即 A->B B->A 方便查询
//	(添加好友 查找一般比插入多)

////	表3 离线消息

//	用户数据表t_user
//	create table t_user ( id bigint unsigned  AUTO_INCREMENT primary key, tel varchar (260) ,
//	password varchar(260)  , name varchar (260) , icon int , feeling varchar(260));

//	可以添加测试数据
//	insert into t_user ( tel,password , name , icon , feeling  )
//	values ('12345','test', '12345' ,1,'比较懒,什么也没写')

//	好友表 t_friend
//	create table t_friend ( idA bigint unsigned  , idB bigint unsigned);

//*/








