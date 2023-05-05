#include "ckernel.h"
#include<QSettings>
#include<QApplication>
#include<QFileInfo>
#include<QMessageBox>
#include<qDebug>
#include<QDate>
#include"MD5/md5.h"

#define netMap(a) m_netMap[a-_DEF_PACK_BASE]
#define MD5_KEY 1234

/// 拼凑规则：
/// passwd_1234
/// \brief GetMD5
/// \param str
/// \return

static std::string GetMD5(QString str){
    std::string tmp=QString("%1_%2").arg(str).arg(MD5_KEY).toStdString();
    MD5 md5(tmp);
    return md5.toString();
}

ckernel::ckernel(QObject *parent) : QObject(parent),m_id(0),m_roomid(0)
{
    initConfig();
    setNetMap();
    m_startDialog=new startDialog;
    m_startDialog->show();
    m_client=new TcpClientMediator;
    m_loginDialog=new LoginDialog;
    m_registerDialog=new RegisterDialog;
    m_mainDialog=new mainDialog;
    m_createRoomDialog=new createRoomForm;
    m_roomDialog=new roomDialog;
    m_roomListDialog=new roomListDialog;


    connect(m_startDialog,SIGNAL(SIG_joinGame()),
            this,SLOT(slot_qie_joinGame()));


    connect(m_client,SIGNAL(SIG_ReadyData(uint,char*,int)),
            this,SLOT(dealData(uint,char*,int)));


    connect(m_loginDialog,SIGNAL(SIG_register()),
            this,SLOT(slot_qie_register()));
    connect(m_loginDialog,SIGNAL(SIG_login(QString,QString)),
            this,SLOT(slot_sendLoginRq(QString,QString)));


    connect(m_registerDialog,SIGNAL(SIG_submit(QString,QString,QString,QString,QDate)),
            this,SLOT(slot_sendRegisterRq(QString,QString,QString,QString,QDate)));
    connect(m_registerDialog,SIGNAL(SIG_return()),
            this,SLOT(slot_qie_returnLogin()));


    connect(m_mainDialog,SIGNAL(SIG_createRoomButton()),
            this,SLOT(slot_qie_createRoomButton()));
    connect(m_mainDialog,SIGNAL(SIG_roomListButton(int,int,int)),
            this,SLOT(slot_sendroomListRQ(int,int,int)));
    connect(m_mainDialog,SIGNAL(SIG_QUIT()),
            this,SLOT(slot_quitLogin()));
    connect(m_mainDialog,SIGNAL(SIG_QUITlogin()),
            this,SLOT(slot_quitClogin()));


    connect(m_createRoomDialog,SIGNAL(SIG_CANCEL()),
            this,SLOT(slot_qie_cancel()));
    connect(m_createRoomDialog,SIGNAL(SIG_createRoom(int,int,int,int,bool,QString)),
            this,SLOT(slot_sendCreateRoomRQ(int,int,int,int,bool,QString)));


    connect(m_roomDialog,SIGNAL(SIG_quitRoom(int)),
            this,SLOT(slot_qie_quitRoom(int)));
    connect(m_roomDialog,SIGNAL(SIG_QUIT()),
            this,SLOT(slot_quitLogin()));
    connect(m_roomDialog,SIGNAL(SIG_ReadybeginGame()),
            this,SLOT(slot_sendBeginGameTestRq()));
    connect(m_roomDialog,SIGNAL(SIG_beginGame()),
            this,SLOT(slot_sendBeginGameRq()));
    connect(m_roomDialog,SIGNAL(SIG_skyBlkRs(int,int,int,int)),
            this,SLOT(slot_sendSkyBlkRs(int,int,int,int)));
    connect(m_roomDialog,SIGNAL(SIG_skyBlk15(bool)),
            this,SLOT(slot_sendskyBlk15(bool)));
    connect(m_roomDialog,SIGNAL(SIG_nvSilverWater()),
            this,SLOT(slot_sendNvSW()));
    connect(m_roomDialog,SIGNAL(SIG_imDie(int)),
            this,SLOT(slot_sendImDie(int)));


    connect(m_roomListDialog,SIGNAL(SIG_REFRESH(int,int,int)),
            this,SLOT(slot_sendroomListRQ(int,int,int)));
    connect(m_roomListDialog,SIGNAL(SIG_joinroom(int)),
            this,SLOT(slot_sendJoinRoomRq(int)));
    connect(m_roomListDialog,SIGNAL(SIG_QUIT()),
            this,SLOT(slot_quitLogin()));
    connect(m_roomListDialog,SIGNAL(SIG_returnMain()),
            this,SLOT(slot_qie_listMain()));
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
        slot_quitLogin();
        if(m_roomid)slot_qie_quitRoom(m_zuowei);
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
    if(m_createRoomDialog){
        m_createRoomDialog->hide();
        delete m_createRoomDialog;
        m_createRoomDialog=nullptr;
    }
    if(m_roomDialog){
        m_roomDialog->hide();
        delete m_roomDialog;
        m_roomDialog=nullptr;
    }
    if(m_roomListDialog){
        m_roomListDialog->hide();
        delete m_roomListDialog;
        m_roomListDialog==nullptr;
    }
}

void ckernel::slot_qie_joinGame()
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

void ckernel::slot_qie_register()
{
    //隐藏登录界面
    //显示注册界面
    m_loginDialog->hide();
    m_registerDialog->showNormal();
}

void ckernel::slot_qie_returnLogin()
{
    //隐藏注册界面
    m_registerDialog->hide();
    //显示登录界面
    m_loginDialog->showNormal();
}

void ckernel::slot_qie_createRoomButton()
{
    //隐藏主界面
    m_mainDialog->hide();
    //显示创建房间界面
    m_createRoomDialog->showNormal();
}

void ckernel::slot_qie_cancel()
{
    //隐藏创建房间界面
    m_createRoomDialog->hide();
    //显示主界面
    m_mainDialog->showNormal();
}

void ckernel::slot_qie_quitRoom(int id)
{
    //发送退出房间包
    STRU_LEAVEROOM_RQ rq;
    rq.m_nUserId=m_id;
    rq.m_RoomId=m_roomid;
    SendData(0,(char*)&rq,sizeof(rq));
    //判断是不是房主，如果是，返回到主界面，如果不是，返回到房间列表（参数id是房主的id）
    if(m_id==id){
        //隐藏房间界面
        m_roomDialog->hide();
        //显示主界面
        m_mainDialog->showNormal();
    }else{
        m_roomDialog->hide();
        //显示房间列表
        m_roomListDialog->showNormal();
    }
    m_roomDialog->slot_destroyRoom();
    m_roomid=0;
}

void ckernel::slot_qie_listMain()
{
    //隐藏房间列表
    m_roomListDialog->hide();
    //显示主界面
    m_mainDialog->showNormal();
}



void ckernel::slot_sendRegisterRq(QString username, QString passwd, QString name, QString sex, QDate date)
{
    //封装成数据包，发送给服务端
    STRU_REGISTER_RQ rq;
    strcpy(rq.username,username.toStdString().c_str());
    strcpy(rq.name,name.toStdString().c_str());
    strcpy(rq.password,GetMD5(passwd).c_str());
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
    strcpy(rq.password,GetMD5(passwd).c_str());
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendCreateRoomRQ(int mode, int method, int playerNum, int level, bool pass, QString password)
{
    STRU_CREATEROOM_RQ rq;
    rq.m_UserID=m_id;
    rq.level=level;
    rq.lock=pass;
    rq.maxcount=playerNum;
    rq.mode=mode;
    strcpy(rq.passwd,password.toStdString().c_str());
    rq.playMethod=method;
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendroomListRQ(int method,int mode,int roomid)
{
    //发送包
    STRU_ROOMLIST_RQ rq;
    rq.method=method;
    rq.mode=mode;
    rq.roomid=roomid;
    SendData(0,(char*)&rq,sizeof(rq));
    //隐藏主窗口
    m_mainDialog->hide();
    //显示房间列表（正在获取....）
    m_roomListDialog->showNormal();
}

void ckernel::slot_sendJoinRoomRq(int roomid)
{
    //发送加入房间包
    STRU_JOINROOM_RQ rq;
    rq.m_UserID=m_id;
    rq.m_RoomID=roomid;
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendBeginGameTestRq()
{
    //发送准备开始游戏包
    STRU_BEGINGAMETEST_RQ rq;
    rq.m_RoomId=m_roomid;
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendBeginGameRq()
{
    //发送开始游戏包
    STRU_BEGINGAME_RQ rq;
    rq.m_RoomId=m_roomid;
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendSkyBlkRs(int iden, int seat, int operate, int toseat)
{
    //发送天黑操作包
    STRU_SKYBLACK_RS rs;
    rs.m_iden=iden;
    rs.m_operate=operate;
    rs.m_seat=seat;
    rs.m_toseat=toseat;
    rs.m_roomid=m_roomid;
    SendData(0,(char*)&rs,sizeof(rs));
}

void ckernel::slot_sendskyBlk15(bool mid)
{
    STRU_SKYBLK_END end;
    end.roomid=m_roomid;
    if(mid){
        //发送夜晚前十五秒结束
        end.state=0;
    }else{
        //发送夜晚后十五秒结束
        end.state=1;
    }
    SendData(0,(char*)&end,sizeof(end));
}

void ckernel::slot_sendNvSW()
{
    STRU_LRTONW_SKYBLK_RS rs;
    rs.roomid=m_roomid;
    SendData(0,(char*)&rs,sizeof(rs));
}

void ckernel::slot_sendImDie(int iden)
{
    STRU_SKYWHT_RS rs;
    rs.iden=iden;
    rs.roomid=m_roomid;
    SendData(0,(char*)&rs,sizeof(rs));
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
        //成功,返回登录界面（如果直接登录的话，服务端不会走登录模块，没有做在线设置和发送个人信息包）

        QMessageBox::about(m_registerDialog,"提示","注册成功！！");
        m_registerDialog->hide();
        m_loginDialog->showNormal();
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
        m_loginDialog->hide();
        m_mainDialog->showNormal();
        //设置成员信息
        m_id=rs->userid;
    }
}

void ckernel::slot_DealQuitLoginRs(unsigned int lSendIP, char *buf, int nlen)
{
    //如果在游戏中...TODO
    //如果在房间中
    if(m_roomid>0){
        m_roomDialog->hide();
        m_roomDialog->slot_destroyRoom();
        m_roomid=0;
    }
    else m_mainDialog->hide();
    m_loginDialog->showNormal();
    QMessageBox::about(m_loginDialog,"提示","本账号已在其他客户端登录");
}

void ckernel::slot_DealUserInfoRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_USER_INFO_RQ* rq=(STRU_USER_INFO_RQ*)buf;
    int icon=rq->m_iconid;
    QString name=QString::fromStdString(rq->m_name);
    QString sex=QString::fromStdString(rq->m_sex);
    int userid=rq->m_UserID;
    QString username=QString::fromStdString(rq->m_username);
    int level=rq->m_level;
    //设置主界面的个人信息
    m_mainDialog->slot_setInfo(userid,icon,name,sex,username,level);
}

void ckernel::slot_DealCreateRoomRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_CREATEROOM_RS* rs=(STRU_CREATEROOM_RS*)buf;
    m_roomid=rs->m_RoomId;
    //给房间界面传递信息
    m_roomDialog->slot_setInfo(rs->m_RoomId,rs->mode,0,rs->lock,
                               QString::fromStdString(rs->passwd),rs->maxcount,m_id);
    m_createRoomDialog->hide();
    m_roomDialog->showNormal();
}

void ckernel::slot_DealRoomListRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_ROOMLIST_RS* rs=(STRU_ROOMLIST_RS*)buf;
    //设置列表信息
    int mode   =rs->mode;
    int method =rs->method;
    int roomid =rs->roomid;
    int count =rs->count;
    int current=rs->currentCou;
    bool pass   =rs->pass;
    QString passwd =QString::fromStdString(rs->passwd);
    bool state  =rs->state;
    int level  =rs->level;
    m_roomListDialog->slot_setlist(mode,method,roomid,count,current,pass,passwd,state,level);
}

void ckernel::slot_DealRoomMemberRq(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_ROOM_MEMBER_RQ* rq=(STRU_ROOM_MEMBER_RQ*)buf;
    //将该成员添加到对应的控件上
    m_roomDialog->slot_setPlayer(rq->m_seat,rq->m_icon,rq->m_level,QString::fromStdString(rq->m_sex)
                                 ,QString::fromStdString(rq->m_szUser),rq->m_UserID);
    if(rq->m_UserID==m_id)m_zuowei=rq->m_seat;
}

void ckernel::slot_DealJoinRoomRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    //判断结果，如果加入失败，提示信息
    STRU_JOINROOM_RS* rs=(STRU_JOINROOM_RS*)buf;
    if(rs->m_lResult==room_no_exist){
        QMessageBox::about(m_roomListDialog,"提示","房间不存在，加入失败");
        return;
    }
    if(rs->m_lResult==level_unqualified){
        QMessageBox::about(m_roomListDialog,"提示","不满足等级条件，加入失败");
        return;
    }
    //加入成功，隐藏房间列表界面，显示房间界面
    m_roomid=rs->m_RoomID;
    m_roomDialog->slot_setInfo(rs->m_RoomID,rs->mode,0,rs->lock,
                               QString::fromStdString(rs->passwd),rs->maxcount,m_id);
    m_roomListDialog->hide();
    m_roomDialog->showNormal();
}

void ckernel::slot_DealLeaveRoomRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_LEAVEROOM_RS* rs=(STRU_LEAVEROOM_RS*)buf;
    if(rs->m_roomisExist){
        //判断房间还在不在，如果在，从房间中删掉对应座位号的人
        m_roomDialog->slot_setPlayer(rs->m_id,0,0,"","",0);
    }else{
        //如果不在，提示房间已解散，返回房间列表
        QMessageBox::about(m_roomDialog,"提示","房间已解散");
        m_roomDialog->hide();
        m_roomListDialog->showNormal();
        m_roomDialog->slot_destroyRoom();
        m_roomid=0;
    }
}

void ckernel::slot_DealBeginGameTestRs(unsigned int lSendIP, char *buf, int nlen)
{
    //准备开始，5s倒计时
    m_roomDialog->slot_ready();
}

void ckernel::slot_DealBeginGameRs(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_BEGINGAME_RS* rs=(STRU_BEGINGAME_RS*)buf;
    //显示身份信息
    m_roomDialog->slot_setIden(rs->m_iden);
}

void ckernel::slot_DealSkyBlackRq(unsigned int lSendIP, char *buf, int nlen)
{
    m_roomDialog->slot_skyBlack();
}

void ckernel::slot_DealYYJSkyBlk(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_YYJ_SKYBLK* yyj=(STRU_YYJ_SKYBLK*)buf;
    m_roomDialog->slot_yyj(yyj->id,yyj->iden);
}

void ckernel::slot_DealLRSkyBlk(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_LR_SKYBLK* lr=(STRU_LR_SKYBLK*)buf;
    m_roomDialog->slot_lr(lr->id,lr->toid);
}

void ckernel::slot_DealLRKillSkyBlk(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_LRTONW_SKYBLK* kl=(STRU_LRTONW_SKYBLK*)buf;
    m_roomDialog->slot_nw(kl->kill);
}

void ckernel::slot_DealSkyWhiteRq(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_SKYWHT_RQ* rq=(STRU_SKYWHT_RQ*)buf;
    m_roomDialog->slot_skyWhite(rq->die);
}

void ckernel::slot_DealSpeakRq(unsigned int lSendIP, char *buf, int nlen)
{
    //语音TODO
    m_roomDialog->slot_speak();
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
    netMap(DEF_PACK_USER_INFO)=&ckernel::slot_DealUserInfoRs;
    netMap(DEF_PACK_CREATEROOM_RS)=&ckernel::slot_DealCreateRoomRs;
    netMap(DEF_PACK_ROOMLIST_RS)=&ckernel::slot_DealRoomListRs;
    netMap(DEF_PACK_ROOM_MEMBER)=&ckernel::slot_DealRoomMemberRq;
    netMap(DEF_PACK_JOINROOM_RS)=&ckernel::slot_DealJoinRoomRs;
    netMap(DEF_PACK_LEAVEROOM_RS)=&ckernel::slot_DealLeaveRoomRs;
    netMap(DEF_PACK_BEGINGAMETEST_RS)=&ckernel::slot_DealBeginGameTestRs;
    netMap(DEF_PACK_BEGINGAME_RS)=&ckernel::slot_DealBeginGameRs;
    netMap(DEF_PACK_SKYBLACK_RQ)=&ckernel::slot_DealSkyBlackRq;
    netMap(DEF_PACK_YYJ_SKYBLK)=&ckernel::slot_DealYYJSkyBlk;
    netMap(DEF_PACK_LR_SKYBLK)=&ckernel::slot_DealLRSkyBlk;
    netMap(DEF_PACK_LRTONW_SKYBLK)=&ckernel::slot_DealLRKillSkyBlk;
    netMap(DEF_PACK_SKYWHT_RQ)=&ckernel::slot_DealSkyWhiteRq;
    netMap(DEF_PACK_SPEAK_RQ)=&ckernel::slot_DealSpeakRq;
}

void ckernel::slot_quitLogin()
{
    STRU_CLIENTQUITLOGIN_RQ rq;
    rq.m_UserID=m_id;
    SendData(0,(char*)&rq,sizeof(rq));
    exit(0);
}

void ckernel::slot_quitClogin()
{
    STRU_CLIENTQUITLOGIN_RQ rq;
    rq.m_UserID=m_id;
    SendData(0,(char*)&rq,sizeof(rq));
    m_mainDialog->hide();
    m_loginDialog->showNormal();
}
