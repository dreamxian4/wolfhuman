#include "ckernel.h"
#include<QSettings>
#include<QApplication>
#include<QFileInfo>
#include<QMessageBox>
#include<qDebug>
#include<QDate>
#include"MD5/md5.h"
#include<QTime>

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

ckernel::ckernel(QObject *parent) : QObject(parent),m_id(0),m_roomid(0),m_wolf(false)
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
    audioRead=new AudioRead;
    m_ziliao=new ZiLiaoDialog;
    m_comment=new commentDialog;


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
    connect(m_mainDialog,SIGNAL(SIG_getSpace(int,bool,int,QString,int)),
            this,SLOT(slot_sendGetSpace(int,bool,int,QString,int)));


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
    //    connect(m_roomDialog,SIGNAL(SIG_imDie(int)),
    //            this,SLOT(slot_sendImDie(int)));
    connect(m_roomDialog,SIGNAL(SIG_police(int,bool)),
            this,SLOT(slot_sendPolice(int,bool)));
    connect(m_roomDialog,SIGNAL(SIG_PoliceEnd()),
            this,SLOT(slot_sendPoliceEnd()));
    connect(m_roomDialog,SIGNAL(SIG_SpeakEnd(int,int,int)),
            this,SLOT(slot_sendSpeakEnd(int,int,int)));
    connect(m_roomDialog,SIGNAL(SIG_imPolice(int,int)),
            this,SLOT(slot_sendImPolice(int,int)));
    connect(m_roomDialog,SIGNAL(SIG_SpeakStateEnd(int)),
            this,SLOT(slot_sendSpeakStateEnd(int)));
    connect(m_roomDialog,SIGNAL(SIG_vote(int,int,int)),
            this,SLOT(slot_sendVote(int,int,int)));
    connect(m_roomDialog,SIGNAL(SIG_VoteEnd(int)),
            this,SLOT(slot_sendVoteEnd(int)));
    connect(m_roomDialog,SIGNAL(SIG_speakOrder(int,int)),
            this,SLOT(slot_sendSpeakOrder(int,int)));
    connect(m_roomDialog,SIGNAL(SIG_dayExile(int)),
            this,SLOT(slot_sendDayExile(int)));
    connect(m_roomDialog,SIGNAL(SIG_Audio(bool,bool,bool)),
            this,SLOT(slot_Audio(bool,bool,bool)));
    connect(m_roomDialog,SIGNAL(SIG_lrKillSelf()),
            this,SLOT(slot_sendLrKillSelf()));



    connect(m_roomListDialog,SIGNAL(SIG_REFRESH(int,int,int)),
            this,SLOT(slot_sendroomListRQ(int,int,int)));
    connect(m_roomListDialog,SIGNAL(SIG_joinroom(int)),
            this,SLOT(slot_sendJoinRoomRq(int)));
    connect(m_roomListDialog,SIGNAL(SIG_QUIT()),
            this,SLOT(slot_quitLogin()));
    connect(m_roomListDialog,SIGNAL(SIG_returnMain()),
            this,SLOT(slot_qie_listMain()));


    connect(audioRead,SIGNAL(SIG_audioFrame(QByteArray&)),
            this,SLOT(slot_sendAudio(QByteArray&)));

    connect(m_ziliao,SIGNAL(SIG_sendMsg(int)),
            this,SLOT(slot_qie_ziliaoToSendMag(int)));
    connect(m_ziliao,SIGNAL(SIG_joinRoom(int)),
            this,SLOT(slot_sendJoinRoomRq(int)));

    connect(m_comment,SIGNAL(SIG_sendComment(int,QString)),
            this,SLOT(slot_sendSpaceComment(int,QString)));
}




ckernel::~ckernel()
{
    qDebug()<<"退出";
    if(m_startDialog){
        m_startDialog->hide();
        delete m_startDialog;
        m_startDialog=nullptr;
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
    if(audioRead){
        delete audioRead;
        audioRead=nullptr;
    }
    if(m_client){
        slot_quitLogin();
        m_client->CloseNet();
        delete m_client;
        m_client=nullptr;
    }
}



void ckernel::slot_qie_joinGame()
{
    //连接网络
    if(!m_client->OpenNet(m_serverIp.toStdString().c_str(),8080)){
        QMessageBox::information(m_startDialog,"提示","服务器连接失败,请稍后再试~~~");
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
//        m_roomListDialog->showNormal();
        slot_sendroomListRQ(-1,-1,0);
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

void ckernel::slot_sendUserZiLiaoRq(int id)
{
    //发送好友详细信息请求
    STRU_FRIEND_ZILIAO_RQ rq;
    rq.friendid=id;
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendGetSpace(int kind,bool find,int which,QString str,int page)
{
    STRU_SPACE_RQ rq;
    rq.id=m_id;
    rq.kind=kind;
    rq.find=find;
    rq.page=page;
    strcpy(rq.str,str.toStdString().c_str());
    rq.which=which;
    m_mainDialog->slot_clearSpace();
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendSpaceOpt(int spaceid, int userid,int kind, int opt)
{
    STRU_SPACE_OPT sopt;
    sopt.spaceid=spaceid;
    sopt.userid=m_id;
    sopt.kind=kind;
    sopt.opt=opt;
    sopt.masterid=userid;
    SendData(0,(char*)&sopt,sizeof(sopt));
}

void ckernel::slot_sendGetComment(int spaceid, int icon, QString name, QString time, QString content,int id)
{
    STRU_SPACE_COMMENT_RQ rq;
    rq.spaceid=spaceid;
    m_comment->slot_setInfo(icon,name,time,content,id);
    m_comment->slot_clearwidget();
    m_comment->showNormal();
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendSpaceComment(int spaceid, QString content)
{
    STRU_SPACE_OPT sopt;
    sopt.spaceid=spaceid;
    sopt.userid=m_id;
    sopt.kind=3;
    strcpy(sopt.comment,content.toStdString().c_str());
    SendData(0,(char*)&sopt,sizeof(sopt));
}


void ckernel::slot_qie_chatItemSend(int id)
{
    //显示聊天窗口·
    if(m_mapIdToChatDialog.find(id)!=m_mapIdToChatDialog.end()){
        ChatDialog* chat=m_mapIdToChatDialog[id];
        chat->showNormal();
    }
}

void ckernel::slot_qie_quitChat(int id)
{
    //关闭聊天窗口
    if(m_mapIdToChatDialog.find(id)!=m_mapIdToChatDialog.end()){
        ChatDialog* chat=m_mapIdToChatDialog[id];
        chat->close();
    }
}

void ckernel::slot_qie_ziliaoToSendMag(int id)
{
    //显示聊天窗口
    if(m_mapIdToChatDialog.find(id)!=m_mapIdToChatDialog.end()){
        ChatDialog* chat=m_mapIdToChatDialog[id];
        chat->showNormal();
    }
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

//void ckernel::slot_sendImDie(int iden)
//{
//    STRU_SKYWHT_RS rs;
//    rs.iden=iden;
//    rs.roomid=m_roomid;
//    SendData(0,(char*)&rs,sizeof(rs));
//}

void ckernel::slot_sendPolice(int seat,bool raise)
{
    STRU_TOBEPOLICE_RS rs;
    rs.roomid=m_roomid;
    rs.seat=seat;
    rs.raise=raise;
    SendData(0,(char*)&rs,sizeof(rs));
}

void ckernel::slot_sendPoliceEnd()
{
    STRU_POLICE_END end;
    end.roomid=m_roomid;
    SendData(0,(char*)&end,sizeof(end));
}

void ckernel::slot_sendSpeakEnd(int seat,int next,int state)
{
    audioRead->pause();
    STRU_SPEAK_RS rs;
    rs.seat=seat;
    rs.roomid=m_roomid;
    rs.next=next;
    rs.state=state;
    SendData(0,(char*)&rs,sizeof(rs));
}

void ckernel::slot_sendImPolice(int seat,int state)
{
    STRU_BEPOLICE_RS rs;
    rs.roomid=m_roomid;
    rs.seat=seat;
    rs.state=state;
    SendData(0,(char*)&rs,sizeof(rs));
}

void ckernel::slot_sendSpeakStateEnd(int state)
{
    STRU_SPEAKSTATE_END end;
    end.roomid=m_roomid;
    end.state=state;
    SendData(0,(char*)&end,sizeof(end));
}

void ckernel::slot_sendVote(int seat, int toseat,int state)
{
    STRU_VOTE_RQ rq;
    rq.roomid=m_roomid;
    rq.seat=seat;
    rq.toseat=toseat;
    rq.state=state;
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendVoteEnd(int state)
{
    STRU_VOTE_END end;
    end.roomid=m_roomid;
    end.state=state;
    SendData(0,(char*)&end,sizeof(end));
}

void ckernel::slot_sendSpeakOrder(int seat, int next)
{
    STRU_SPEAK_ORDER order;
    order.roomid=m_roomid;
    order.seat=seat;
    order.next=next;
    SendData(0,(char*)&order,sizeof(order));
}

void ckernel::slot_sendDayExile(int seat)
{
    STRU_DAY_EXILE exile;
    exile.roomid=m_roomid;
    exile.die=seat;
    SendData(0,(char*)&exile,sizeof(exile));
}

void ckernel::slot_sendAudio(QByteArray &frame)
{
    ///音频数据帧
    /// 成员描述
    /// int type;
    /// int zuowei;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int msec;
    /// int hour;
    /// int wolf;
    /// QByteArray audioFrame;  --> char frame[]; 柔性数组
    int type=DEF_PACK_AUDIO_FRAME;
    int zuowei=m_zuowei;
    int roomid=m_roomid;
    QTime tm=QTime::currentTime();
    int min=tm.minute();
    int sec=tm.second();
    int msec=tm.msec();
    int hour=tm.hour();
    int wolf=m_wolf?1:0;
    char* audioData=frame.data();
    int len=frame.size();
    char* buf=new char[sizeof(int)*8+len];
    //序列化数据
    char* tmp=buf;
    *(int*)tmp=type;
    tmp+=sizeof(int);
    *(int*)tmp=zuowei;
    tmp+=sizeof(int);
    *(int*)tmp=roomid;
    tmp+=sizeof(int);
    *(int*)tmp=hour;
    tmp+=sizeof(int);
    *(int*)tmp=min;
    tmp+=sizeof(int);
    *(int*)tmp=sec;
    tmp+=sizeof(int);
    *(int*)tmp=msec;
    tmp+=sizeof(int);
    *(int*)tmp=wolf;
    tmp+=sizeof(int);
    memcpy(tmp,audioData,len);
    SendData(0,buf,sizeof(int)*8+len);
    delete []buf;
}

void ckernel::slot_sendLrKillSelf()
{
    STRU_LR_KILLSELF kill;
    kill.roomid=m_roomid;
    kill.seat=m_zuowei;
    SendData(0,(char*)&kill,sizeof(kill));
}

void ckernel::slot_SendChatMsg(int friendID, QString content)
{
    //1.拆包
    STRU_CHAT_RQ rq;
    rq.userid=m_id;
    rq.friendid=friendID;

    //2.兼容中文
    std::string strContent=content.toStdString();
    strcpy(rq.content,strContent.c_str());

    //3.聊天内容发给服务器
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_Audio(bool begin,bool sent,bool wolf)
{
    m_wolf=wolf;
    if(begin){//开始采集音频
        audioRead->start();
    }else{//停止采集音频
        audioRead->pause();
        if(sent){
            STRU_SPEAKPAUSE pause;
            pause.roomid=m_roomid;
            pause.seat=m_zuowei;
            pause.wolf=wolf;
            SendData(0,(char*)&pause,sizeof(pause));
        }
    }
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
    if(rq->m_UserID==m_id){m_zuowei=rq->m_seat;}
    else {
        if(m_mapSeatToWrite.count(rq->m_seat)>0)m_mapSeatToWrite.erase(rq->m_seat);
        m_mapSeatToWrite[rq->m_seat]=new AudioWrite;
    }
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
    //加入成功，隐藏房间列表界面，显示房间界面(隐藏主界面，好友资料界面，聊天界面)
    m_mapSeatToWrite.clear();
    m_roomid=rs->m_RoomID;
    m_roomDialog->slot_setInfo(rs->m_RoomID,rs->mode,0,rs->lock,
                               QString::fromStdString(rs->passwd),rs->maxcount,m_id);
    m_roomListDialog->hide();
    m_mainDialog->hide();
    m_ziliao->hide();
    m_roomDialog->showNormal();
}

void ckernel::slot_DealLeaveRoomRs(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_LEAVEROOM_RS* rs=(STRU_LEAVEROOM_RS*)buf;
    if(rs->m_roomisExist){
        //判断房间还在不在，如果在，从房间中删掉对应座位号的人
        m_roomDialog->slot_setPlayer(rs->m_id,0,0,"","",0);
        //删掉对应的音频播放器
        m_mapSeatToWrite.erase(rs->m_id);
    }else{
        //如果不在，提示房间已解散，返回房间列表
        QMessageBox::about(m_roomDialog,"提示","房间已解散");
        m_roomDialog->hide();
        m_roomListDialog->showNormal();
        m_roomDialog->slot_destroyRoom();
        m_roomid=0;
        m_mapSeatToWrite.clear();
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
    //语音
    m_roomDialog->slot_speak(*(STRU_SPEAK_RQ*)buf);
}

void ckernel::slot_DealPoliceRq(unsigned int lSendIP, char *buf, int nlen)
{
    //竞选警长
    m_roomDialog->slot_police();
}

void ckernel::slot_DealBePoliceRq(unsigned int lSendIP, char *buf, int nlen)
{
    //成为警长
    m_roomDialog->slot_bePolice();
}

void ckernel::slot_DealBePoliceRs(unsigned int lSendIP, char *buf, int nlen)
{
    //设置警长
    m_roomDialog->slot_setPolice(*(STRU_BEPOLICE_RS*)buf);
}

void ckernel::slot_DealBeginVote(unsigned int lSendIP, char *buf, int nlen)
{
    //开始投票
    m_roomDialog->slot_beginVote(*(STRU_SPEAKSTATE_END*)buf);
}

void ckernel::slot_DeaVoteRs(unsigned int lSendIP, char *buf, int nlen)
{
    //处理投票结果
    m_roomDialog->slot_VoteRs(*(STRU_VOTE_RS*)buf);
}

void ckernel::slot_DealSpeakOrder(unsigned int lSendIP, char *buf, int nlen)
{
    //发言顺序：如果是空，说明是要警长选择，如果有值，说明是通知房间成员警长的选择
    m_roomDialog->slot_SpeakOrder(*(STRU_SPEAK_ORDER*)buf);
}

void ckernel::slot_DealSpeakStateBegin(unsigned int lSendIP, char *buf, int nlen)
{
    //发言阶段开始，客户端房间内显示发言开始信息
    m_roomDialog->slot_SpeakStateBegin();
}

void ckernel::slot_DealDayExile(unsigned int lSendIP, char *buf, int nlen)
{
    //将放逐玩家头像设置为出局，更新状态信息
    m_roomDialog->slot_dayExile(*(STRU_DAY_EXILE*)buf);
}

void ckernel::slot_DealGameOver(unsigned int lSendIP, char *buf, int nlen)
{
    //游戏结束，显示游戏结果
    m_roomDialog->slot_gameOver();
}

void ckernel::slot_DealAudioFrame(unsigned int lSendIP, char *buf, int nlen)
{
    ///音频数据帧
    /// 成员描述
    /// int type;
    /// int zuowei;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int msec;
    /// int hour;
    /// int wolf;
    /// QByteArray audioFrame;  --> char frame[]; 柔性数组
    //反序列化
    int type;
    int zuowei;
    int roomid;
    int min;
    int sec;
    int msec;
    int hour;
    int wolf;

    //反序列化数据
    char* tmp=buf;
    type=*(int*)tmp;
    tmp+=sizeof(int);
    zuowei=*(int*)tmp;
    tmp+=sizeof(int);
    roomid=*(int*)tmp;
    tmp+=sizeof(int);
    min=*(int*)tmp;
    tmp+=sizeof(int);
    sec=*(int*)tmp;
    tmp+=sizeof(int);
    msec=*(int*)tmp;
    tmp+=sizeof(int);
    hour=*(int*)tmp;
    tmp+=sizeof(int);
    wolf=*(int*)tmp;
    tmp+=sizeof(int);

    int audiolen=nlen-8*sizeof(int);
    //音频数据
    if(!wolf||m_roomDialog->slot_getIden()==3){
        if(zuowei!=m_zuowei){
            QByteArray ba(tmp,audiolen);
            if(m_mapSeatToWrite.count(zuowei)>0){
                AudioWrite* aw=m_mapSeatToWrite[zuowei];
                aw->slot_net_rx(ba);
            }
        }
        m_roomDialog->slot_playerSpeak(zuowei,true);
    }
}

void ckernel::slot_DealSpeakPause(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_SPEAKPAUSE* pause=(STRU_SPEAKPAUSE*)buf;
    if(!pause->wolf)m_roomDialog->slot_playerSpeak(pause->seat,false);
}

void ckernel::slot_DealSpeakEnd(unsigned int lSendIP, char *buf, int nlen)
{
    //提示某玩家结束发言
    m_roomDialog->slot_speakEnd(*(STRU_SPEAK_RS*)buf);
}

void ckernel::slot_DealFriendInfo(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_FRIEND_INFO* info=(STRU_FRIEND_INFO*)buf;
    //存储好友信息
    //显示好友信息
    //4.不是自己，查看当前窗口是否已经有这个好友了
    if(m_mapIdToUserItem.find(info->userid)==m_mapIdToUserItem.end()){
        //4.1 如果没有，把好友添加到控件上
        //4.1.1 创建一个useritem和chatitem
        UserItem* item=new UserItem;
        ChatItem* citem=new ChatItem;

        //4.1.2 给控件赋值
        item->slot_setInfo(info->userid,QString::fromStdString(info->name),info->state,info->icon,QString::fromStdString(info->sex),
                           info->level,QString::fromStdString(info->username));
        citem->slot_setInfo(QString::fromStdString(info->name),info->state,info->icon,QString::fromStdString(info->sex),info->userid);

        //4.1.3 绑定控件点击事件的信号和槽TODO
        connect(item,SIGNAL(SIG_UserItemClicked(int)),
                this,SLOT(slot_sendUserZiLiaoRq(int)));
        connect(citem,SIGNAL(SIG_chatItemSend(int)),
                this,SLOT(slot_qie_chatItemSend(int)));

        //4.1.4 创建聊天窗口
        ChatDialog* chat=new ChatDialog;
        chat->setInfo(info->name,info->userid);


        //4.1.5 绑定聊天窗口发送数据的信号和槽函数
        connect(chat,SIGNAL(SIG_SendChatMsg(int , QString )),
                this,SLOT(slot_SendChatMsg(int , QString )));
        connect(chat,SIGNAL(SIG_quitChat(int)),
                this,SLOT(slot_qie_quitChat(int)));

        //4.1.6 把聊天窗口放在map中管理
        m_mapIdToChatDialog[info->userid]=chat;

        //4.1.7 把useritem添加到好友列表里/把chatitem放在聊天列表里
        m_mainDialog->slot_addFriend(item);
        m_mainDialog->slot_addChat(citem);

        //4.1.8 把useritem放在map中管理/把chatitem放在map中管理
        m_mapIdToUserItem[info->userid]=item;
        m_mapIdToChatItem[info->userid]=citem;
    }else{
        //4.2 如果有，更新控件信息
        UserItem* item=m_mapIdToUserItem[info->userid];
        if(item&&item->m_state==2&&info->state==1){//之前是离线，现在是上线
            //可以弹窗提示用户xxx已上线
            qDebug()<<QString("用户[%1]已上线").arg(info->name);
        }
        ChatItem* citem=m_mapIdToChatItem[info->userid];
        //更新控件
        item->slot_setInfo(info->userid,QString::fromStdString(info->name),info->state,info->icon,QString::fromStdString(info->sex),
                           info->level,QString::fromStdString(info->username));
        citem->slot_setInfo(QString::fromStdString(info->name),info->state,info->icon,QString::fromStdString(info->sex),info->userid);
    }
}

void ckernel::slot_DealChatRq(unsigned int lSendIP, char *buf, int nlen)
{
    //1.拆包
    STRU_CHAT_RQ* rq=(STRU_CHAT_RQ*)buf;

    //2.查看聊天窗口是否存在
    if(m_mapIdToChatDialog.find(rq->userid)!=m_mapIdToChatDialog.end()){
        //3.如果存在，根据id查找窗口并显示
        ChatDialog* chat=m_mapIdToChatDialog[rq->userid];

        //4.设置聊天内容
        chat->slot_setChatMsg(rq->content);
    }
    //5.查看聊天控件是否存在
    if(m_mapIdToChatItem.find(rq->userid)!=m_mapIdToChatItem.end()){
        //6.存在，更新最新消息,将该用户的控件移到最上面
        ChatItem* item=m_mapIdToChatItem[rq->userid];
        item->slot_setChatMsg(rq->content);
    }
}

void ckernel::slot_DealChatRs(unsigned int lSendIP, char *buf, int nlen)
{
    //1.拆包
    STRU_CHAT_RS* rs=(STRU_CHAT_RS*)buf;

    //2.查看聊天窗口是否存在
    if(m_mapIdToChatDialog.find(rs->friendid)!=m_mapIdToChatDialog.end()){
        //3.如果存在，根据id查找窗口并显示
        ChatDialog* chat=m_mapIdToChatDialog[rs->friendid];

        //4.设置聊天内容
        chat->slot_UserOffline();
    }
}

void ckernel::slot_DealFriendZiLiao(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_FRIEND_ZILIAO_RS* rs=(STRU_FRIEND_ZILIAO_RS*)buf;
    m_ziliao->slot_setInfo(*rs);
    m_ziliao->show();
}

void ckernel::slot_DealSpace(unsigned int lSendIP, char *buf, int nlen)
{
    m_mainDialog->slot_setSpace((STRU_SPACE_RS*)buf);

    spaceForm *space=new spaceForm;
    connect(space,SIGNAL(SIG_Getdetail(int)),
            this,SLOT(slot_sendUserZiLiaoRq(int)));
    connect(space,SIGNAL(SIG_SpaceOpt(int,int,int,int)),
            this,SLOT(slot_sendSpaceOpt(int,int,int,int)));
    connect(space,SIGNAL(SIG_GetComment(int,int,QString,QString,QString,int)),
            this,SLOT(slot_sendGetComment(int,int,QString,QString,QString,int)));

    space->setInfo((STRU_SPACE_RS*)buf);
    m_mainDialog->slot_addSpace(space);
}

void ckernel::slot_DealSpaceComment(unsigned int lSendIP, char *buf, int nlen)
{
    //在评论窗口添加组件并显示
    STRU_SPACE_COMMENT_RS* rs=(STRU_SPACE_COMMENT_RS*)buf;
    commentForm* form=new commentForm;

    form->slot_setInfo(QString::fromStdString(rs->name),
                       QString::fromStdString(rs->time),QString::fromStdString(rs->comment));
    m_comment->slot_addwidget(form);
}


void ckernel::slot_DealPolicePlayerRs(unsigned int lSendIP, char *buf, int nlen)
{
    //设置竞选警长的玩家
    m_roomDialog->slot_setPolicePlayer(*(STRU_TOBEPOLICE_RS*)buf);
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
    netMap(DEF_PACK_TOBEPOLICE_RQ)=&ckernel::slot_DealPoliceRq;
    netMap(DEF_PACK_TOBEPOLICE_RS)=&ckernel::slot_DealPolicePlayerRs;
    netMap(DEF_PACK_BEPOLICE_RQ)=&ckernel::slot_DealBePoliceRq;
    netMap(DEF_PACK_BEPOLICE_RS)=&ckernel::slot_DealBePoliceRs;
    netMap(DEF_PACK_SPEAKSTATE_END)=&ckernel::slot_DealBeginVote;
    netMap(DEF_PACK_VOTE_RS)=&ckernel::slot_DeaVoteRs;
    netMap(DEF_PACK_SPEAK_ORDER)=&ckernel::slot_DealSpeakOrder;
    netMap(DEF_PACK_SPEAKSTATE_BEGIN)=&ckernel::slot_DealSpeakStateBegin;
    netMap(DEF_PACK_DAY_EXILE)=&ckernel::slot_DealDayExile;
    netMap(DEF_PACK_GAMEOVER)=&ckernel::slot_DealGameOver;
    netMap(DEF_PACK_AUDIO_FRAME)=&ckernel::slot_DealAudioFrame;
    netMap(DEF_PACK_SPEAK_PAUSE)=&ckernel::slot_DealSpeakPause;
    netMap(DEF_PACK_SPEAK_RS)=&ckernel::slot_DealSpeakEnd;
    netMap(DEF_PACK_FRIEND_INFO)=&ckernel::slot_DealFriendInfo;
    netMap(DEF_PACK_CHAT_RQ)=&ckernel::slot_DealChatRq;
    netMap(DEF_PACK_CHAT_RS)=&ckernel::slot_DealChatRs;
    netMap(DEF_PACK_FRIEND_ZILIAO_RS)=&ckernel::slot_DealFriendZiLiao;
    netMap(DEF_PACK_SPACE_RS)=&ckernel::slot_DealSpace;
    netMap(DEF_PACK_SPACE_COMMENT_RS)=&ckernel::slot_DealSpaceComment;
}

void ckernel::slot_quitLogin()
{
    //退出登录，退出客户端
    STRU_CLIENTQUITLOGIN_RQ rq;
    rq.m_UserID=m_id;
    rq.roomid=m_roomid;
    SendData(0,(char*)&rq,sizeof(rq));
    exit(0);
}

void ckernel::slot_quitClogin()
{
    //退出登录，返回主界面
    STRU_CLIENTQUITLOGIN_RQ rq;
    rq.m_UserID=m_id;
    rq.roomid=m_roomid;
    SendData(0,(char*)&rq,sizeof(rq));
    m_mainDialog->hide();
    m_loginDialog->showNormal();
}
