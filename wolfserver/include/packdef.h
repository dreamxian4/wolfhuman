#ifndef _PACKDEF_H
#define _PACKDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "err_str.h"
#include <malloc.h>

#include<iostream>
#include<map>
#include<list>


//边界值
#define _DEF_SIZE 45
#define _DEF_BUFFERSIZE 1000
#define _DEF_PORT 8080
#define _DEF_SERVERIP "0.0.0.0"  //TODO

/*-------------数据库信息-----------------*/
#define _DEF_DB_NAME    "wolfwomen"
#define _DEF_DB_IP      "localhost"
#define _DEF_DB_USER    "root"
#define _DEF_DB_PWD     "123456"
/*
    room表属性：房间id（6位数），房主，开始人数，当前人数，房间状态，等级，是否加密，密码

*/
/*--------------------------------------*/

/*-------------玩法----------------------*/
//视频模式
#define _GAME_MODE_VIDEO (0)
//语音模式
#define _GAME_MODE_AUDIO (1)
//基础玩法
#define _GAME_BASE (0)
/*--------------------------------------*/

#define TRUE true
#define FALSE false


#define _DEF_LISTEN 128
#define _DEF_EPOLLSIZE 4096
#define _DEF_IPSIZE 16
#define _DEF_COUNT 10
#define _DEF_TIMEOUT 10
#define _DEF_SQLIEN 400



#define BOOL bool
#define DEF_PACK_BASE  (10000)
#define DEF_PACK_COUNT  (100)

//注册
#define  DEF_PACK_REGISTER_RQ    (DEF_PACK_BASE + 0)
#define  DEF_PACK_REGISTER_RS    (DEF_PACK_BASE + 1)
//登录
#define  DEF_PACK_LOGIN_RQ    (DEF_PACK_BASE + 2)
#define  DEF_PACK_LOGIN_RS    (DEF_PACK_BASE + 3)
//强制退出登录
#define DEF_PACK_QUIT_LOGIN_RQ (DEF_PACK_BASE + 4)
//用户信息 用于设置和登录时获取状态
#define DEF_PACK_USER_INFO      (DEF_PACK_BASE + 5)
//创建房间
#define DEF_PACK_CREATEROOM_RQ  (DEF_PACK_BASE + 6)
#define DEF_PACK_CREATEROOM_RS  (DEF_PACK_BASE + 7)
//客户端退出
#define DEF_PACK_CLIENTQUITLOGIN_RQ  (DEF_PACK_BASE + 8)
//房间列表请求
#define DEF_PACK_ROOMLIST_RQ    (DEF_PACK_BASE + 9)
#define DEF_PACK_ROOMLIST_RS    (DEF_PACK_BASE + 10)
//加入房间
#define DEF_PACK_JOINROOM_RQ  (DEF_PACK_BASE + 11)
#define DEF_PACK_JOINROOM_RS  (DEF_PACK_BASE + 12)
//房间成员信息
#define DEF_PACK_ROOM_MEMBER    (DEF_PACK_BASE + 13)
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ   (DEF_PACK_BASE + 14)
//退出房间回复
#define DEF_PACK_LEAVEROOM_RS   (DEF_PACK_BASE + 15)
//开始游戏
#define DEF_PACK_BEGINGAMETEST_RQ   (DEF_PACK_BASE + 16)
#define DEF_PACK_BEGINGAMETEST_RS   (DEF_PACK_BASE + 17)
#define DEF_PACK_BEGINGAME_RQ   (DEF_PACK_BASE + 18)
#define DEF_PACK_BEGINGAME_RS   (DEF_PACK_BASE + 19)
////音频数据
//#define DEF_PACK_AUDIO_FRAME    (DEF_PACK_BASE + 9)
////视频数据
//#define DEF_PACK_VIDEO_FRAME    (DEF_PACK_BASE + 10)

////音频注册
//#define DEF_PACK_AUDIO_REGISTER (DEF_PACK_BASE + 13)
////视频注册
//#define DEF_PACK_VIDEO_REGISTER (DEF_PACK_BASE + 14)


//注册请求结果
#define username_is_exist   (0)
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


#define MAX_PATH            (260 )
#define MAX_SIZE            (40  )
#define DEF_HOBBY_COUNT     (8  )
#define MAX_CONTENT_LEN     (4096 )


/////////////////////网络//////////////////////////////////////


#define DEF_MAX_BUF	  1024
#define DEF_BUFF_SIZE	  4096


typedef int PackType;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// id -> userInfo
typedef struct UserInfo
{
    UserInfo()
    {
         m_sockfd = 0;
         m_id = 0;
         m_roomid = 0;
         memset(m_userName, 0 , MAX_SIZE);
//        m_videofd = 0;
//        m_audiofd = 0;
         m_seat=0;
    }
//    struct bufferevent*  m_sockfd;
    int m_sockfd;
    int  m_id;
    int  m_roomid;//方便下线的时候从房间移出
    char m_userName[MAX_SIZE];
    int  m_seat;
//    int  m_videofd;
//    int  m_audiofd;

}UserInfo;

// roomid->RoomInfo
typedef struct RoomInfo
{
    RoomInfo()
    {
        m_roomid  =0;
        m_beginNum  =0;
        m_currentNum=0;
        m_playing   =false;
        m_minLevel  =0;
        m_lock      =false;
        m_mode=0;
        m_playMethod=0;
        memset(m_passwd,0,MAX_SIZE);
        memset(m_seat,0,13*sizeof(bool));
    }
    //房间id（6位数），开始人数，当前人数，房间状态，等级，是否加密，密码，模式，玩法
    int  m_roomid   ;
    int  m_beginNum   ;
    int  m_currentNum ;
    bool m_playing    ;    //0准备阶段 1游戏中
    int  m_minLevel   ;
    bool m_lock       ;
    char m_passwd[MAX_SIZE];
    int  m_mode;//0语音 1视频
    int  m_playMethod;//0基础玩法
    bool m_seat[13];
}RoomInfo;

//登录
typedef struct STRU_LOGIN_RQ
{
    //登录需要: 手机号 密码
    STRU_LOGIN_RQ():type(DEF_PACK_LOGIN_RQ)
    {
        memset( username , 0, sizeof(username) );
        memset( password , 0, sizeof(password) );
    }
    PackType type;
    char username[MAX_SIZE];
    char password[MAX_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS
{
    // 需要 结果 , 用户的id
    STRU_LOGIN_RS(): type(DEF_PACK_LOGIN_RS) , result(login_success),userid(0)
    {
    }
    PackType type;
    int userid;
    int result;

}STRU_LOGIN_RS;

//强制退出请求
typedef struct STRU_QUITLOGIN_RQ
{
    STRU_QUITLOGIN_RQ()
    {
        m_nType= DEF_PACK_QUIT_LOGIN_RQ;
    }
    PackType m_nType;   //包类型

}STRU_QUITLOGIN_RQ;

//用户信息请求，登录之后状态同步,设置用户信息
typedef struct STRU_USER_INFO_RQ
{
    STRU_USER_INFO_RQ()
    {
        m_nType= DEF_PACK_USER_INFO;
        m_UserID =0;
        m_iconid=0;
        m_level=1;
        memset(m_username,0,MAX_SIZE);
        memset(m_sex,0,MAX_SIZE);
        memset(m_name,0,MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int m_UserID;
    int m_iconid;
    int m_level;
    char m_username[MAX_SIZE];
    char m_sex[MAX_SIZE];
    char m_name[MAX_SIZE];

}STRU_USER_INFO_RQ;

//注册请求
typedef struct STRU_REGISTER_RQ
{
    //用户名唯一
    STRU_REGISTER_RQ():type(DEF_PACK_REGISTER_RQ)
    {
        memset( username  , 0, sizeof(username));
        memset( password , 0, sizeof(password) );
        memset( sex , 0, sizeof(sex) );
        memset( name , 0, sizeof(name) );
    }
    //用户名、密码、性别、昵称、出生日月
    PackType type;
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char sex[MAX_SIZE];
    char name[MAX_SIZE];
//    int month;
//    int day;

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
    //回复结果
    STRU_REGISTER_RS(): type(DEF_PACK_REGISTER_RS) , result(register_success)
    {
        memset( sex , 0, sizeof(sex) );
        memset( name , 0, sizeof(name) );
    }
    //昵称、性别
    PackType type;
    int result;
    char sex[MAX_SIZE];
    char name[MAX_SIZE];

}STRU_REGISTER_RS;



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
        memset(passwd,0,MAX_SIZE);
    }

    //模式（语音、视频）、玩法（基础、进阶）、人数、等级限制（无限制、xx级）、加密（是、否）、密码
    PackType m_nType;   //包类型
    int m_UserID;
    int  mode       ;
    int  playMethod ;
    int  level      ;
    bool lock       ;
    int maxcount;
    char passwd[MAX_SIZE];

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
        memset(passwd,0,MAX_SIZE);
    }
    //房间号、模式、人数、加密、密码
    PackType m_nType;   //包类型
    int  m_lResult ;    //创建结果
    int  m_RoomId;
    int  mode       ;
    bool lock       ;
    int maxcount;
    char passwd[MAX_SIZE];

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
        memset(roomName,0,MAX_SIZE);
        memset(passwd,0,MAX_SIZE);
        state=false;
        level=0;
    }
    PackType m_nType;   //包类型
    int     mode        ;//模式
    int     method      ;//玩法
    int     roomid      ;
    char    roomName[MAX_SIZE]    ;
    int     count       ;
    int     currentCou  ;
    bool    pass        ;
    char    passwd[MAX_SIZE];
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
        memset(passwd,0,MAX_SIZE);
        look      =false;
    }
    PackType m_nType;   //包类型
    int  m_lResult  ;    //注册结果
    int  m_RoomID    ;
    int  mode       ;
    bool lock       ;
    int  maxcount    ;
    char passwd         [MAX_SIZE];
    bool look       ;          //是否观战
}STRU_JOINROOM_RS;




//房间成员请求
typedef struct STRU_ROOM_MEMBER_RQ
{
    STRU_ROOM_MEMBER_RQ()
    {
        m_nType= DEF_PACK_ROOM_MEMBER;
        m_UserID =0;
        memset(m_szUser,0,MAX_SIZE);
        memset(m_sex,0,MAX_SIZE);
        m_icon=0;
        m_level=0;
        m_seat=0;
    }
    //头像、昵称、id、等级、性别、座位号
    PackType m_nType;   //包类型
    int m_UserID;
    char m_szUser[MAX_SIZE];
    int     m_icon;
    int     m_level;
    char    m_sex[MAX_SIZE];
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

///音频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int min;
/// int sec;
/// int msec;
/// QByteArray audioFrame;  --> char frame[]; 柔性数组
///


///视频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int hour;
/// int min;
/// int sec;
/// int msec;
/// QByteArray videoFrame;
///



#endif


