#pragma once
#include<QString>

#define _DEF_BUFFER  ( 4096 )
#define _DEF_CONTENT_SIZE	(1024)
#define _MAX_SIZE	(40)

////玩法
//视频模式
#define _GAME_MODE_VIDEO (1)
//语音模式
#define _GAME_MODE_AUDIO (0)
//基础玩法
#define _GAME_BASE (0)




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
////音频数据
//#define DEF_PACK_AUDIO_FRAME    (_DEF_PACK_BASE + 9)
////视频数据
//#define DEF_PACK_VIDEO_FRAME    (_DEF_PACK_BASE + 10)

////音频注册
//#define DEF_PACK_AUDIO_REGISTER (_DEF_PACK_BASE + 13)
////视频注册
//#define DEF_PACK_VIDEO_REGISTER (_DEF_PACK_BASE + 14)



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
    }
    PackType m_nType;   //包类型
    int m_UserID;

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








