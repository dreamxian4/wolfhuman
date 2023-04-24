#include "ckernel.h"
#include<QSettings>
#include<QApplication>
#include<QFileInfo>
#include<QMessageBox>
#include<qDebug>
#include<QDate>

#define netMap(a) m_netMap[a-_DEF_PACK_BASE]

ckernel::ckernel(QObject *parent) : QObject(parent)
{
    initConfig();
    setNetMap();
    m_startDialog=new startDialog;
    m_startDialog->show();
    m_client=new TcpClientMediator;
    m_loginDialog=new LoginDialog;
    m_registerDialog=new RegisterDialog;
    m_mainDialog=new mainDialog;

    connect(m_startDialog,SIGNAL(SIG_joinGame()),
            this,SLOT(slot_joinGame()));

    connect(m_client,SIGNAL(SIG_ReadyData(uint,char*,int)),
            this,SLOT(dealData(uint,char*,int)));

    connect(m_loginDialog,SIGNAL(SIG_register()),
            this,SLOT(slot_register()));
    connect(m_loginDialog,SIGNAL(SIG_login(QString,QString)),
            this,SLOT(slot_sendLoginRq(QString,QString)));

    connect(m_registerDialog,SIGNAL(SIG_submit(QString,QString,QString,QString,QDate)),
            this,SLOT(slot_sendRegisterRq(QString,QString,QString,QString,QDate)));
    connect(m_registerDialog,SIGNAL(SIG_return()),
            this,SLOT(slot_returnLogin()));
}

ckernel::~ckernel()
{
    qDebug()<<"退出";
    if(m_startDialog){
        m_startDialog->hide();
        delete m_startDialog;
        m_startDialog=nullptr;
    }
    if(m_client){
        m_client->CloseNet();
        delete m_client;
        m_client=nullptr;
    }
    if(m_loginDialog){
        m_loginDialog->hide();
        delete m_loginDialog;
        m_loginDialog=nullptr;
    }
    if(m_registerDialog){
        m_registerDialog->hide();
        delete m_registerDialog;
        m_registerDialog=nullptr;
    }
    if(m_mainDialog){
        m_mainDialog->hide();
        delete m_mainDialog;
        m_mainDialog=nullptr;
    }
}

void ckernel::slot_joinGame()
{
    //连接网络
    if(!m_client->OpenNet(m_serverIp.toStdString().c_str(),8080)){
        QMessageBox::information(m_startDialog,"提示","服务器连接失败,请稍后再试~~~");
        return;
    }
    //隐藏启动窗口
    m_startDialog->hide();
    //显示登录窗口
    m_loginDialog->showNormal();
}

void ckernel::slot_register()
{
    //隐藏登录界面
    //显示注册界面
    m_loginDialog->hide();
    m_registerDialog->showNormal();
}

void ckernel::slot_returnLogin()
{
    //隐藏注册界面
    m_registerDialog->hide();
    //显示登录界面
    m_loginDialog->showNormal();
}

void ckernel::slot_sendRegisterRq(QString username, QString passwd, QString name, QString sex, QDate date)
{
    //封装成数据包，发送给服务端
    STRU_REGISTER_RQ rq;
    strcpy(rq.username,username.toStdString().c_str());
    strcpy(rq.name,name.toStdString().c_str());
    strcpy(rq.password,passwd.toStdString().c_str());
    strcpy(rq.sex,sex.toStdString().c_str());
    //生日
    //    QString year=QString::number(date.year());
    //    QString month=QString::number(date.month());
    //    QString day=QString::number(date.day());
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendLoginRq(QString name, QString passwd)
{
    //封装成数据包，发送给服务端
    STRU_LOGIN_RQ rq;
    strcpy(rq.username,name.toStdString().c_str());
    strcpy(rq.password,passwd.toStdString().c_str());
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::dealData(unsigned int lSendIP, char *buf, int nlen)
{
    int type=*(int*)buf;
    if(type>=_DEF_PACK_BASE&&type<_DEF_PACK_BASE+_DEF_PROTOCOL_COUNT){
        PFUN p=netMap(type);
        if(p) (this->*p)(lSendIP,buf,nlen);
    }
    delete []buf;
}

void ckernel::slot_DealRegisterRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_REGISTER_RS* rs=(STRU_REGISTER_RS*)buf;
    if(rs->result==username_is_exist){//失败，返回注册界面
        QMessageBox::about(m_registerDialog,"提示","用户名已存在~~");
        return;
    }else if(rs->result==register_success){
        //成功，直接进入主界面
        QMessageBox::about(m_registerDialog,"提示","注册成功！！");
        m_registerDialog->hide();
        m_mainDialog->showNormal();
    }
}

void ckernel::slot_DealLoginRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_LOGIN_RS* rs=(STRU_LOGIN_RS*)buf;\
    //登录失败，提示
    if(rs->result==user_not_exist){
        QMessageBox::about(m_loginDialog,"提示","用户不存在");
        return;
    }
    if(rs->result==password_error){
        QMessageBox::about(m_loginDialog,"提示","密码错误");
        return;
    }
    //登录成功，隐藏登录窗口，显示主界面
    if(rs->result==login_success){
        QMessageBox::about(m_loginDialog,"提示","登录成功！！！");
        m_loginDialog->hide();
        m_mainDialog->showNormal();
        //设置成员信息
        m_id=rs->userid;
    }
}

void ckernel::slot_DealQuitLoginRs(unsigned int lSendIP, char *buf, int nlen)
{
    //如果正在游戏中。。。TODO
    QMessageBox::about(m_mainDialog,"提示","本账号已在其他客户端登录");
    m_mainDialog->hide();
    m_loginDialog->showNormal();
}

void ckernel::initConfig()
{
    m_serverIp=_DEF_SERVER_IP;
    //路径设置 exe同级目录下->applicationDiePath(获取.exe所在文件夹的绝对路径绝对路径) config.ini
    QString path=QApplication::applicationDirPath()+"/config.ini";
    //判断配置文件是否存在
    QFileInfo info(path);
    //打开配置文件:存在打开，不存在创建
    QSettings settings(path,QSettings::IniFormat,NULL);
    if(info.exists()){
        //文件存在 加载配置文件 获取其中的ip
        //移动到Net组
        settings.beginGroup("Net");
        //读取ip->addr-->赋值
        QVariant ip=settings.value("ip");
        QString strIP=ip.toString();
        //结束
        settings.endGroup();
        if(!strIP.isEmpty()){
            m_serverIp=strIP;
        }
    }else{
        //没有配置文件，写入默认的ip
        settings.beginGroup("Net");
        settings.setValue("ip",m_serverIp);
        settings.endGroup();
    }
    qDebug()<<m_serverIp;
}

void ckernel::SendData(unsigned int lSendIP, char *buf, int nlen)
{
    m_client->SendData(lSendIP,buf,nlen);
}

void ckernel::setNetMap()
{
    memset(m_netMap,0,sizeof(PFUN));
    netMap(_DEF_PACK_REGISTER_RS)=&ckernel::slot_DealRegisterRs;
    netMap(_DEF_PACK_LOGIN_RS)=&ckernel::slot_DealLoginRs;
    netMap(DEF_PACK_QUIT_LOGIN_RQ)=&ckernel::slot_DealQuitLoginRs;
}
