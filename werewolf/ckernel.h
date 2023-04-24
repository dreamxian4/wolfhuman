#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include"packdef.h"
#include"startdialog.h"
#include"TcpClientMediator.h"
#include"logindialog.h"
#include"registerdialog.h"
#include"maindialog.h"

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
    void slot_joinGame();
    //注册
    void slot_register();
    //从注册界面切回登录界面
    void slot_returnLogin();



    //网络发送槽
    void slot_sendRegisterRq(QString username,QString passwd,
                             QString name,QString sex,QDate date);
    void slot_sendLoginRq(QString name,QString passwd);



    //网络接收处理槽
    //处理数据
    void dealData( unsigned int lSendIP , char* buf , int nlen );
    //注册
    void slot_DealRegisterRs( unsigned int lSendIP , char* buf , int nlen );
    //登录
    void slot_DealLoginRs( unsigned int lSendIP , char* buf , int nlen );
    //强制退出登录
    void slot_DealQuitLoginRs( unsigned int lSendIP , char* buf , int nlen );



    //其他
    //初始化配置
    void initConfig();
    //发送数据
    void SendData( unsigned int lSendIP , char* buf , int nlen );
    //设置协议
    void setNetMap();

private:
    startDialog* m_startDialog;//启动窗口
    LoginDialog* m_loginDialog;//登录窗口
    RegisterDialog* m_registerDialog;//注册窗口
    mainDialog* m_mainDialog;//主窗口
    TcpClientMediator* m_client;//网络
    PFUN m_netMap[_DEF_PROTOCOL_COUNT];//协议映射表
    QString m_serverIp;//服务端ip地址
    int m_id;//用户id
};

#endif // CKERNEL_H
