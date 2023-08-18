#include "ckernel.h"
#include<QSettings>
#include<QApplication>
#include<QFileInfo>
#include<QMessageBox>
#include<qDebug>
#include<QDate>
#include"MD5/md5.h"

#include<QTime>
#include<QInputDialog>
#include<map>

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

    getFriEmail=false;
    getSpaEmail=false;
    withUser=0;
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
    m_audioRead=new AudioRead;
    m_audioWrite=new AudioWrite;
    m_videoRead=new VideoRead;
    m_ziliao=new ZiLiaoDialog;
    m_comment=new commentDialog;
    m_addSpace=new addSpaceDialog;
    m_find=new findDialog;
    m_spaceEmail=new emailDialog;
    m_friendEmail=new emailDialog;
    m_chatmsg=new ChatMsgDialog;
    m_yaoqing=new YaoQingFriDialog;
    m_userSpace=new UserSpaceDialog;
    m_viau=new VideoAudioDialog;

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
    connect(m_mainDialog,SIGNAL(SIG_sendSpace()),
            this,SLOT(slot_qie_sendSpace()));
    //    connect(m_mainDialog,SIGNAL(SIG_FindFriend(QString)),
    //            this,SLOT(slot_qie_findFriend(QString)));
    connect(m_mainDialog,SIGNAL(SIG_FindUser(QString,int,int)),
            this,SLOT(slot_sendFindUser(QString,int,int)));
    connect(m_mainDialog,SIGNAL(SIG_getSpaceEmail()),
            this,SLOT(slot_sendGetSpaceEmail()));
    connect(m_mainDialog,SIGNAL(SIG_getFriEmail()),
            this,SLOT(slot_sendGetFriEmail()));


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
    connect(m_roomDialog,SIGNAL(SIG_getPlayerZL(int)),
            this,SLOT(slot_sendUserZiLiaoRq(int)));
    connect(m_roomDialog,SIGNAL(SIG_YaoQingFri(int)),
            this,SLOT(slot_qie_yaoqingFri(int)));



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
    connect(m_audioRead,SIGNAL(SIG_audioFrame(QByteArray&)),
            this,SLOT(slot_sendAudioWithFrame(QByteArray&)));
    connect(m_videoRead,SIGNAL(SIG_videoFrame(QImage&)),
            this,SLOT(slot_refreshVideoImage(QImage&)));
    connect(m_videoRead,SIGNAL(SIG_videoFrameData(QByteArray&)),
            this,SLOT(slot_sendVideoFrameData(QByteArray&)));

    connect(m_ziliao,SIGNAL(SIG_sendMsg(int)),
            this,SLOT(slot_qie_ziliaoToSendMag(int)));
    connect(m_ziliao,SIGNAL(SIG_joinRoom(int)),
            this,SLOT(slot_sendJoinRoomRq(int)));
    connect(m_ziliao,SIGNAL(SIG_deleteFriend(int)),
            this,SLOT(slot_sendFriendDel(int)));
    connect(m_ziliao,SIGNAL(SIG_getSpace(int)),
            this,SLOT(slot_sendgetUserSpace(int)));
    connect(m_ziliao,SIGNAL(SIG_addFri(int)),
            this,SLOT(slot_sendAddFriend(int)));

    connect(m_comment,SIGNAL(SIG_sendComment(int,QString,int)),
            this,SLOT(slot_sendSpaceComment(int,QString,int)));

    connect(m_addSpace,SIGNAL(SIG_sendSpace(QString)),
            this,SLOT(slot_sendAddSpace(QString)));

    connect(m_find,SIGNAL(SIG_FindUser(QString,int,int)),
            this,SLOT(slot_sendFindUser(QString,int,int)));

    connect(m_spaceEmail,SIGNAL(SIG_clearSpaceEmail(bool)),
            this,SLOT(slot_sendClearEmail(bool)));

    connect(m_friendEmail,SIGNAL(SIG_clearFriendEmail(bool)),
            this,SLOT(slot_sendClearEmail(bool)));

    connect(m_chatmsg,SIGNAL(SIG_deleteMsg(int)),
            this,SLOT(slot_sendDeleteMsg(int)));

    connect(m_viau,SIGNAL(SIG_audioOpen()),
            this,SLOT(slot_audioOpen()));
    connect(m_viau,SIGNAL(SIG_audioClose()),
            this,SLOT(slot_audioClose()));
    connect(m_viau,SIGNAL(SIG_videoOpen()),
            this,SLOT(slot_videoOpen()));
    connect(m_viau,SIGNAL(SIG_videoClose()),
            this,SLOT(slot_videoClose()));
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
    m_comment->slot_setInfo(icon,name,time,content,spaceid,id);
    m_comment->slot_clearwidget();
    m_comment->showNormal();
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendSpaceComment(int spaceid, QString content,int masterid)
{
    STRU_SPACE_OPT sopt;
    sopt.spaceid=spaceid;
    sopt.userid=m_id;
    sopt.kind=3;
    sopt.masterid=masterid;
    strcpy(sopt.comment,content.toStdString().c_str());
    SendData(0,(char*)&sopt,sizeof(sopt));
}

void ckernel::slot_sendAddSpace(QString content)
{
    STRU_SPACE_ADD add;
    add.userid=m_id;
    strcpy(add.content,content.toStdString().c_str());
    SendData(0,(char*)&add,sizeof(add));
}

void ckernel::slot_sendFindUser(QString user, int index,int page)
{
    STRU_USER_FIND_RQ rq;
    rq.id=m_id;
    rq.page=page;
    rq.which=index;
    strcpy(rq.str,user.toStdString().c_str());
    m_find->slot_setInfo(page,index,user);
    m_find->slot_clearwidget();
    m_find->showNormal();
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendGetSpaceEmail()
{
    if(!getSpaEmail){
        STRU_EMAIL_RQ rq;
        rq.id=m_id;
        rq.space=true;
        m_spaceEmail->slot_clearwidget();
        m_spaceEmail->slot_setInfo(true);
        SendData(0,(char*)&rq,sizeof(rq));
        getSpaEmail=true;
    }
    m_spaceEmail->showNormal();
}

void ckernel::slot_sendGetFriEmail()
{
    if(!getFriEmail){
        STRU_EMAIL_RQ rq;
        rq.id=m_id;
        rq.space=false;
        m_friendEmail->slot_clearwidget();
        m_friendEmail->slot_setInfo(false);
        SendData(0,(char*)&rq,sizeof(rq));
        getFriEmail=true;
    }
    m_friendEmail->showNormal();
}

void ckernel::slot_sendClearEmail(bool isspace)
{
    if(isspace){
        m_spaceEmail->slot_clearwidget();
    }else m_friendEmail->slot_clearwidget();
    STRU_EMAIL_CLEAR clear;
    clear.userid=m_id;
    clear.space=isspace;
    SendData(0,(char*)&clear,sizeof(clear));
}

void ckernel::slot_sendAddFriend(int id)
{
    QString str=QInputDialog::getText(m_find,"添加好友","验证消息：");
    STRU_FRIEND_ADD_RQ rq;
    rq.id=m_id;
    rq.beiid=id;
    if(m_id==id){
        QMessageBox::about(m_find,"提示","不能添加自己为好友");
        return;
    }
    strcpy(rq.content,str.toStdString().c_str());
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendAddResult(int userid, bool add)
{
    STRU_FRIEND_ADD_RS rs;
    rs.id=m_id;
    rs.beiid=userid;
    rs.result=add;
    SendData(0,(char*)&rs,sizeof(rs));
}

void ckernel::slot_sendFriendDel(int id)
{
    ChatDialog* dia=m_mapIdToChatDialog[id];
    m_mapIdToChatDialog.erase(id);
    dia->hide();
    delete dia;
    dia=nullptr;

    UserItem* ite=m_mapIdToUserItem[id];
    m_mainDialog->slot_removeFriend(ite);
    m_mapIdToUserItem.erase(id);
    delete ite;
    ite=nullptr;

    ChatItem* cite=m_mapIdToChatItem[id];
    m_mainDialog->slot_removeChat(cite);
    m_mapIdToChatItem.erase(id);
    delete cite;
    cite=nullptr;

    m_lstFriend.remove(id);

    //本来是用作好友查找的，功能待开发
    m_mapIdToName.erase(id);

    //发送删除请求
    STRU_FRIEND_DELETE del;
    del.userid=m_id;
    del.beiid=id;
    SendData(0,(char*)&del,sizeof(del));
}

void ckernel::slot_sendGetChatMsg(int id)
{
    //显示聊天记录窗口
    m_chatmsg->setinfo(m_mapIdToName[id],id);
    m_chatmsg->showNormal();
    STRU_CHAT_MSG msg;
    msg.userid=m_id;
    msg.beiid=id;
    SendData(0,(char*)&msg,sizeof(msg));
}

void ckernel::slot_sendDeleteMsg(int id)
{
    m_mapIdToChatDialog[id]->slot_deleteMsg();
    m_mapIdToChatItem[id]->slot_deleteMsg();
    STRU_CHAT_MSG msg;
    msg.userid=m_id;
    msg.beiid=id;
    msg.del=true;
    SendData(0,(char*)&msg,sizeof(msg));
}

void ckernel::slot_sendYaoQingFri(int id, int roomid)
{
    STRU_YAOQING yq;
    yq.userid=m_id;
    yq.beiid=id;
    yq.roomid=roomid;
    SendData(0,(char*)&yq,sizeof(yq));
}

void ckernel::slot_sendAudioWith(int friid)
{
    STRU_AUDIO_RQ rq;
    rq.userid=m_id;
    rq.beiid=friid;
    withUser=friid;
    m_viau->setinfo(true);
    m_viau->showNormal();
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendVideoWith(int friid)
{
    STRU_VIDEO_RQ rq;
    rq.userid=m_id;
    rq.beiid=friid;
    withUser=friid;
    m_viau->setinfo(false);
    m_viau->showNormal();
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendgetUserSpace(int friid)
{
    STRU_SPACE_RQ rq;
    rq.id=m_id;
    rq.user=true;
    rq.beiid=friid;
    m_userSpace->slot_clearSpace();
    m_userSpace->showNormal();
    SendData(0,(char*)&rq,sizeof(rq));
}

void ckernel::slot_sendAudioWithFrame(QByteArray &frame)
{
    ///音频数据帧
    /// 成员描述
    /// int type;
    /// int id;
    /// int beiid;
    /// int min;
    /// int sec;
    /// int msec;
    /// int hour;
    /// QByteArray audioFrame;  --> char frame[]; 柔性数组
    int type=DEF_PACK_AUDIO_WITH_FRAME;
    int id=m_id;
    int beiid=withUser;
    QTime tm=QTime::currentTime();
    int min=tm.minute();
    int sec=tm.second();
    int msec=tm.msec();
    int hour=tm.hour();
    char* audioData=frame.data();
    int len=frame.size();
    char* buf=new char[sizeof(int)*7+len];
    //序列化数据
    char* tmp=buf;
    *(int*)tmp=type;
    tmp+=sizeof(int);
    *(int*)tmp=id;
    tmp+=sizeof(int);
    *(int*)tmp=beiid;
    tmp+=sizeof(int);
    *(int*)tmp=hour;
    tmp+=sizeof(int);
    *(int*)tmp=min;
    tmp+=sizeof(int);
    *(int*)tmp=sec;
    tmp+=sizeof(int);
    *(int*)tmp=msec;
    tmp+=sizeof(int);
    memcpy(tmp,audioData,len);
    SendData(0,buf,sizeof(int)*7+len);
    delete []buf;
}


void ckernel::slot_qie_chatItemSend(int id)
{
    //显示聊天窗口
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

void ckernel::slot_qie_sendSpace()
{
    m_addSpace->showNormal();
}

void ckernel::slot_qie_yaoqingFri(int roomid)
{
    m_yaoqing->slot_clearwidget();
    m_yaoqing->show();
    for(auto id:m_lstFriend){
        if(m_mapIdToUserItem[id]->m_state==1){
            //在线好友
            YaoQingForm* form=new YaoQingForm;

            connect(form,SIGNAL(SIG_yaoqing(int,int)),
                    this,SLOT(slot_sendYaoQingFri(int,int)));

            form->setinfo(m_mapIdToUserItem[id]->m_iconid,m_mapIdToUserItem[id]->m_name,
                          id,roomid);
            m_yaoqing->slot_addwidget(form);
        }
    }
}

//void ckernel::slot_qie_findFriend(QString find)
//{
//    for(auto id:m_lstFriend){
//        if(m_mapIdToName[id]==find){

//        }
//    }
//}





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
    //清除当前用户数据
    for(auto id:m_lstFriend){
        if(m_mapIdToChatItem.count(id)>0){
            m_mainDialog->slot_removeChat(m_mapIdToChatItem[id]);
            ChatItem* chat=m_mapIdToChatItem[id];
            m_mapIdToChatItem.erase(id);
            delete chat;
            chat=nullptr;
        }
        if(m_mapIdToUserItem.count(id)>0){
            m_mainDialog->slot_removeFriend(m_mapIdToUserItem[id]);
            UserItem* user=m_mapIdToUserItem[id];
            m_mapIdToUserItem.erase(id);
            delete user;
            user=nullptr;
        }
        if(m_mapIdToChatDialog.count(id)>0){
            ChatDialog* cd=m_mapIdToChatDialog[id];
            m_mapIdToChatDialog.erase(id);
            delete cd;
            cd=nullptr;
        }
    }
    m_lstFriend.clear();
    m_addSpace->slot_clear();
    getFriEmail=false;
    getSpaEmail=false;
    m_mainDialog->slot_setNoreadSpace(false);
    m_mainDialog->slot_setNoreadFriend(false);

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
    //拆包
    STRU_CHAT_RQ rq;
    rq.userid=m_id;
    rq.friendid=friendID;

    //兼容中文
    std::string strContent=content.toStdString();
    strcpy(rq.content,strContent.c_str());

    //查看聊天控件是否存在
    if(m_mapIdToChatItem.find(friendID)!=m_mapIdToChatItem.end()){
        //存在，更新最新消息,将该用户的控件移到最上面
        ChatItem* item=m_mapIdToChatItem[friendID];
        item->slot_setChatMsg(content,true);
        item->setNoRead(false);
        m_mainDialog->slot_upChat(item);
    }


    //聊天内容发给服务器
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
    m_createRoomDialog   ->hide();
    m_roomListDialog     ->hide();
    m_comment            ->hide();
    m_addSpace           ->hide();
    m_find               ->hide();
    m_spaceEmail         ->hide();
    m_friendEmail        ->hide();
    m_chatmsg            ->hide();
    m_userSpace->hide();
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
        connect(chat,SIGNAL(SIG_GetChatMsg(int)),
                this,SLOT(slot_sendGetChatMsg(int)));
        connect(chat,SIGNAL(SIG_audioFri(int)),
                this,SLOT(slot_sendAudioWith(int)));
        connect(chat,SIGNAL(SIG_videoFri(int)),
                this,SLOT(slot_sendVideoWith(int)));

        //4.1.6 把聊天窗口放在map中管理
        m_mapIdToChatDialog[info->userid]=chat;

        //4.1.7 把useritem添加到好友列表里/把chatitem放在聊天列表里
        m_mainDialog->slot_addFriend(item);
        m_mainDialog->slot_addChat(citem);

        //4.1.8 把useritem放在map中管理/把chatitem放在map中管理
        m_mapIdToUserItem[info->userid]=item;
        m_mapIdToChatItem[info->userid]=citem;
        m_lstFriend.push_back(info->userid);
        m_mapIdToName[info->userid]=info->name;
    }else{
        //4.2 如果有，更新控件信息
        UserItem* item=m_mapIdToUserItem[info->userid];
        ChatItem* citem=m_mapIdToChatItem[info->userid];
        int state=item->m_state;
        //更新控件
        item->slot_setInfo(info->userid,QString::fromStdString(info->name),info->state,info->icon,QString::fromStdString(info->sex),
                           info->level,QString::fromStdString(info->username));
        citem->slot_setInfo(QString::fromStdString(info->name),info->state,info->icon,QString::fromStdString(info->sex),info->userid);
        if(item&&state==2&&info->state==1){//之前是离线，现在是上线
            //            //可以弹窗提示用户xxx已上线
            //            qDebug()<<QString("用户[%1]已上线").arg(info->name);
            m_mainDialog->slot_onlineFriend(item);
        }else if(item&&state!=2&&info->state==2){//之前是在线，现在是离线
            m_mainDialog->slot_offlineFriend(item);
        }
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
        item->slot_setChatMsg(rq->content,false);
        item->setNoRead(true);
        m_mainDialog->slot_upChat(item);
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
    bool fri=false,game=false;
    if(m_mapIdToUserItem.count(rs->friendid)>0)fri=true;
    if(m_roomid>0)game=true;
    m_ziliao->slot_setInfo(*rs,fri,game);
    m_ziliao->show();
}

void ckernel::slot_DealSpace(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_SPACE_RS* rs=(STRU_SPACE_RS*)buf;
    if(!rs->user)m_mainDialog->slot_setSpace((STRU_SPACE_RS*)buf);

    spaceForm *space=new spaceForm;
    connect(space,SIGNAL(SIG_Getdetail(int)),
            this,SLOT(slot_sendUserZiLiaoRq(int)));
    connect(space,SIGNAL(SIG_SpaceOpt(int,int,int,int)),
            this,SLOT(slot_sendSpaceOpt(int,int,int,int)));
    connect(space,SIGNAL(SIG_GetComment(int,int,QString,QString,QString,int)),
            this,SLOT(slot_sendGetComment(int,int,QString,QString,QString,int)));
    connect(space,SIGNAL(SIG_AddFriend(int)),
            this,SLOT(slot_sendAddFriend(int)));

    int fri=0;
    if(m_mapIdToUserItem.count(rs->userid)>0)fri=1;
    if(rs->userid==m_id)fri=2;
    space->setInfo((STRU_SPACE_RS*)buf,fri);
    if(rs->user)m_userSpace->slot_addSpace(space);
    else m_mainDialog->slot_addSpace(space);
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

void ckernel::slot_DealUserFind(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_USER_FIND_RS* rs=(STRU_USER_FIND_RS*)buf;
    UserItem* item=new UserItem;

    connect(item,SIGNAL(SIG_UserItemClicked(int)),
            this,SLOT(slot_sendUserZiLiaoRq(int)));
    connect(item,SIGNAL(SIG_joinChatDia(int)),
            this,SLOT(slot_qie_chatItemSend(int)));
    connect(item,SIGNAL(SIG_addFriend(int)),
            this,SLOT(slot_sendAddFriend(int)));

    bool isFriend;
    if(m_mapIdToUserItem.count(rs->userid)>0)isFriend=true;
    else isFriend=false;
    item->slot_setFind(rs->userid,rs->icon,QString::fromStdString(rs->sex),
                       QString::fromStdString(rs->name),QString::fromStdString(rs->username),
                       rs->level,isFriend);
    m_find->slot_addwidget(item,isFriend);
}

void ckernel::slot_DealEmailRs(unsigned int lSendIP, char *buf, int nlen)
{
    //头插到邮件页面
    STRU_EMAIL_RS* rs=(STRU_EMAIL_RS*)buf;
    emailMsgForm* form=new emailMsgForm;
    form->slot_setInfo(rs->icon,QString::fromStdString(rs->name),rs->userid,
                       rs->which,QString::fromStdString(rs->spaceCnt),
                       QString::fromStdString(rs->msg),rs->result);

    connect(form,SIGNAL(SIG_AddResult(int,bool)),
            this,SLOT(slot_sendAddResult(int,bool)));

    if(rs->which==2||rs->which==3){
        m_friendEmail->slot_addwidget(form);
        if(rs->NEW){m_mainDialog->slot_setNoreadFriend(true);}
    }
    else {
        m_spaceEmail->slot_addwidget(form);
        if(rs->NEW)m_mainDialog->slot_setNoreadSpace(true);
    }
}

void ckernel::slot_DealFriendDel(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_FRIEND_DELETE* del=(STRU_FRIEND_DELETE*)buf;
    int id=del->userid;
    ChatDialog* dia=m_mapIdToChatDialog[id];
    m_mapIdToChatDialog.erase(id);
    dia->hide();
    delete dia;
    dia=nullptr;

    UserItem* ite=m_mapIdToUserItem[id];
    m_mainDialog->slot_removeFriend(ite);
    m_mapIdToUserItem.erase(id);
    delete ite;
    ite=nullptr;

    ChatItem* cite=m_mapIdToChatItem[id];
    m_mainDialog->slot_removeChat(cite);
    m_mapIdToChatItem.erase(id);
    delete cite;
    cite=nullptr;

    m_lstFriend.remove(id);

    //本来是用作好友查找的，功能待开发
    m_mapIdToName.erase(id);
}

void ckernel::slot_DealHOChatMsg(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_CHAT_HO* ho=(STRU_CHAT_HO*)buf;
    if(ho->msg){
        m_chatmsg->slot_setMsg(ho->me,QString::fromStdString(ho->time),QString::fromStdString(ho->content));
    }else if(m_mapIdToChatDialog.count(ho->userid)>0){
        ChatDialog* chat=m_mapIdToChatDialog[ho->userid];
        chat->slot_HO(ho->me,QString::fromStdString(ho->time)
                      ,QString::fromStdString(ho->content));
        ChatItem* item=m_mapIdToChatItem[ho->userid];
        if(ho->his){
            //历史信息
            item->slot_setChatMsg(QString::fromStdString(ho->content),ho->me);
            item->setNoRead(false);
        }else{
            //离线信息
            item->slot_setChatMsg(QString::fromStdString(ho->content),ho->me);
            item->setNoRead(true);
        }
        m_mainDialog->slot_upChat(item);
    }
}

void ckernel::slot_DealYaoQing(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_YAOQING* yq=(STRU_YAOQING*)buf;
    if(QMessageBox::question(m_mainDialog,"好友邀请",
                             QString("您的好友【%1】邀请您加入房间，是否同意").arg(m_mapIdToName[yq->userid]))==QMessageBox::Yes){
        //同意,发送加入房间请求
        slot_sendJoinRoomRq(yq->roomid);
    }
}

void ckernel::slot_DealAudioRq(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_AUDIO_RQ* rq=(STRU_AUDIO_RQ*)buf;
    if(QMessageBox::question(m_mainDialog,
                             "语音邀请",QString("【%1】邀请你语音聊天").arg(m_mapIdToName[rq->userid]))==QMessageBox::Yes){
        //接受邀请
        withUser=rq->userid;
        m_viau->setinfo(true);
        m_viau->showNormal();
    }else{
        //拒绝
        STRU_AUDIO_RS rs;
        rs.userid=m_id;
        rs.beiid=rq->userid;
        rs.result=false;
        SendData(0,(char*)&rs,sizeof(rs));
    }
}

void ckernel::slot_DealVideoRq(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_VIDEO_RQ* rq=(STRU_VIDEO_RQ*)buf;
    if(QMessageBox::question(m_mainDialog,
                             "语音邀请",QString("【%1】邀请你视频聊天").arg(m_mapIdToName[rq->userid]))==QMessageBox::Yes){
        //接受邀请
        withUser=rq->userid;
        m_viau->setinfo(false);
        m_viau->showNormal();
    }else{
        //拒绝
        STRU_VIDEO_RS rs;
        rs.userid=m_id;
        rs.beiid=rq->userid;
        rs.result=false;
        SendData(0,(char*)&rs,sizeof(rs));
    }
}

void ckernel::slot_DealAudioRs(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_AUDIO_RS* rs=(STRU_AUDIO_RS*)buf;
    if(rs->result){
        //接受

    }else{
        QMessageBox::about(m_mainDialog,"提示",QString("【%1】拒绝了你的邀请").arg(m_mapIdToName[rs->userid]));
        m_viau->hide();
    }
}

void ckernel::slot_DealVideoRs(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_VIDEO_RS* rs=(STRU_VIDEO_RS*)buf;
    if(rs->result){
        //接受
    }else{
        QMessageBox::about(m_mainDialog,"提示",QString("【%1】拒绝了你的邀请").arg(m_mapIdToName[rs->userid]));
        m_viau->hide();
    }
}

void ckernel::slot_DealAudioWithFrame(unsigned int lSendIP, char *buf, int nlen)
{
    ///音频数据帧
    /// 成员描述
    /// int type;
    /// int id;
    /// int beiid;
    /// int min;
    /// int sec;
    /// int msec;
    /// int hour;
    /// QByteArray audioFrame;  --> char frame[]; 柔性数组
    //反序列化
    int type;
    int id;
    int beiid;
    int min;
    int sec;
    int msec;
    int hour;

    //反序列化数据
    char* tmp=buf;
    type=*(int*)tmp;
    tmp+=sizeof(int);
    id=*(int*)tmp;
    tmp+=sizeof(int);
    beiid=*(int*)tmp;
    tmp+=sizeof(int);
    min=*(int*)tmp;
    tmp+=sizeof(int);
    sec=*(int*)tmp;
    tmp+=sizeof(int);
    msec=*(int*)tmp;
    tmp+=sizeof(int);
    hour=*(int*)tmp;
    tmp+=sizeof(int);

    int audiolen=nlen-7*sizeof(int);
    //音频数据
    QByteArray ba(tmp,audiolen);
    m_audioWrite->slot_net_rx(ba);
}

void ckernel::slot_DealVideoWithFrame(unsigned int lSendIP, char *buf, int nlen)
{
    //反序列化
    int type;
    int userid;
    int beiid;
    int hour;
    int min;
    int sec;
    int msec;

    //反序列化数据
    char* tmp=buf;
    type=*(int*)tmp;
    tmp+=sizeof(int);
    userid=*(int*)tmp;
    tmp+=sizeof(int);
    beiid=*(int*)tmp;
    tmp+=sizeof(int);
    tmp+=sizeof(int);
    tmp+=sizeof(int);
    tmp+=sizeof(int);
    tmp+=sizeof(int);

    int imagelen=nlen-7*sizeof(int);
    //视频数据
    QImage img;
    img.loadFromData((uchar*)tmp,imagelen);
    m_viau->setimage(img);
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
    netMap(DEF_PACK_USER_FIND_RS)=&ckernel::slot_DealUserFind;
    netMap(DEF_PACK_EMAIL_RS)=&ckernel::slot_DealEmailRs;
    netMap(DEF_PACK_FRIEND_DELETE)=&ckernel::slot_DealFriendDel;
    netMap(DEF_PACK_CHAT_HISorOFF)=&ckernel::slot_DealHOChatMsg;
    netMap(DEF_PACK_YAOQING)=&ckernel::slot_DealYaoQing;
    netMap(DEF_PACK_AUDIO_RQ)=&ckernel::slot_DealAudioRq;
    netMap(DEF_PACK_VIDEO_RQ)=&ckernel::slot_DealVideoRq;
    netMap(DEF_PACK_AUDIO_RS)=&ckernel::slot_DealAudioRs;
    netMap(DEF_PACK_VIDEO_RS)=&ckernel::slot_DealVideoRs;
    netMap(DEF_PACK_AUDIO_WITH_FRAME)=&ckernel::slot_DealAudioWithFrame;
    netMap(DEF_PACK_VIDEO_WITH_FRAME)=&ckernel::slot_DealVideoWithFrame;
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
    withUser=0;
    rq.m_UserID=m_id;
    rq.roomid=m_roomid;
    SendData(0,(char*)&rq,sizeof(rq));
    m_mainDialog->hide();
    m_createRoomDialog->hide();
    m_roomDialog      ->hide();
    m_roomListDialog  ->hide();
    m_ziliao          ->hide();
    m_comment         ->hide();
    m_addSpace        ->hide();
    m_find            ->hide();
    m_spaceEmail      ->hide();
    m_friendEmail     ->hide();
    m_chatmsg->hide();
    m_userSpace->hide();
    m_loginDialog->showNormal();
}

void ckernel::slot_audioOpen()
{
    m_audioRead->start();
}

void ckernel::slot_videoOpen()
{
    m_videoRead->start();
}

void ckernel::slot_audioClose()
{
    m_audioRead->pause();
}

void ckernel::slot_videoClose()
{
    m_videoRead->pause();
}

void ckernel::slot_refreshVideoImage(QImage &img)
{

}

void ckernel::slot_sendVideoFrameData(QByteArray &ba)
{
    //确定协议（服务器加个小时）
    //协议头
    //发送者id:服务器直到是谁发的，不给他转发
    //房间号：找到转发给谁
    //时间 小时 分钟 秒 毫秒 send延迟-》考虑丢弃帧，需要记录时间
    //音频长度
    //音频数据
    int type=DEF_PACK_VIDEO_WITH_FRAME;
    int userid=m_id;
    int beiid=withUser;
    QTime tm=QTime::currentTime();
    int hour=tm.hour();
    int min=tm.minute();
    int sec=tm.second();
    int msec=tm.msec();
    char* audioData=ba.data();
    int len=ba.size();

    //序列化数据
    char* buf=new char[sizeof(int)*7+len];
    char* tmp=buf;
    *(int*)tmp=type;
    tmp+=sizeof(int);
    *(int*)tmp=userid;
    tmp+=sizeof(int);
    *(int*)tmp=beiid;
    tmp+=sizeof(int);
    *(int*)tmp=hour;
    tmp+=sizeof(int);
    *(int*)tmp=min;
    tmp+=sizeof(int);
    *(int*)tmp=sec;
    tmp+=sizeof(int);
    *(int*)tmp=msec;
    tmp+=sizeof(int);
    memcpy(tmp,audioData,len);
    SendData(0,buf,sizeof(int)*7+len);
    delete[] buf;
//    //替换为发送信号
//    Q_EMIT SIG_sendVideoFrame(0,buf,sizeof(int)*7+len);
}
