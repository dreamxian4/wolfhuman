#include "clogic.h"
#include<string>
#include<random>

void CLogic::setNetPackMap()
{
    NetPackMap(DEF_PACK_REGISTER_RQ)    = &CLogic::RegisterRq;
    NetPackMap(DEF_PACK_LOGIN_RQ)    = &CLogic::LoginRq;
    NetPackMap(DEF_PACK_CLIENTQUITLOGIN_RQ)    = &CLogic::QuitLogin;
    NetPackMap(DEF_PACK_CREATEROOM_RQ)    = &CLogic::CreateRoom;
    NetPackMap(DEF_PACK_ROOMLIST_RQ)    = &CLogic::RoomList;
    NetPackMap(DEF_PACK_JOINROOM_RQ)    = &CLogic::JoinRoom;
    NetPackMap(DEF_PACK_LEAVEROOM_RQ)    = &CLogic::LeaveRoom;
    NetPackMap(DEF_PACK_BEGINGAMETEST_RQ)    = &CLogic::BeginGameTest;
}


void CLogic::RegisterRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("RegisterRq:%d\n",clientfd);
    //拆包
    STRU_REGISTER_RQ* rq=(STRU_REGISTER_RQ*)szbuf;
    //在数据库中查找用户名是否存在
    list<string>lstRes;
    char strsql[1024];
    sprintf(strsql,"select username from t_user where username='%s'",rq->username);
    m_sql->SelectMysql(strsql,1,lstRes);

    STRU_REGISTER_RS rs;
    if(lstRes.size()>0){//存在，返回注册失败
        rs.result=username_is_exist;
    }else{//不存在，写入数据库，返回注册成功
        rs.result=register_success;
        sprintf(strsql,
                "insert into t_user(username,password,sex,icon,name,level)values('%s','%s','%s',1,'%s',1)"
                ,rq->username,rq->password,rq->sex,rq->name);
        if(!m_sql->UpdataMysql(strsql)){
            printf("update fail:%s\n",strsql);
        }
    }
    SendData(clientfd,(char*)&rs,sizeof(rs));
}

void CLogic::LoginRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("LoginRq:%d\n",clientfd);
    //拆包
    STRU_LOGIN_RQ* rq=(STRU_LOGIN_RQ*)szbuf;
    //查找数据库
    list<string>lst;
    char sql[1024];
    sprintf(sql,"select id,password from t_user where username='%s'",rq->username);
    m_sql->SelectMysql(sql,2,lst);
    //用户名不存在，返回登陆失败
    STRU_LOGIN_RS rs;
    if(lst.empty())rs.result=user_not_exist;
    //用户名存在，判断密码是否正确
    else {
        int id=atoi(lst.front().c_str());
        lst.pop_front();
        string passwd=lst.front();
        lst.pop_front();
        if(strcmp(passwd.c_str(),rq->password)==0){//正确
            rs.userid=id;
            rs.result=login_success;
            UserInfo* user=nullptr;
            if(m_mapIdToUserInfo.find(id,user)){
                m_mapIdToUserInfo.erase(id);
                //如果用户已经在线，给上一个客户端发送离线申请
                STRU_QUITLOGIN_RQ quitRq;
                SendData(user->m_sockfd,(char*)&quitRq,sizeof(quitRq));
                //判断是否在房间内
                if(user->m_roomid>0){
                    //判断是不是房主
                    list<int>memlst;
                    if(!m_mapRoomidToMemberlist.find(user->m_roomid,memlst))return;
                    STRU_LEAVEROOM_RS lers;
                    if(user->m_seat==1){
                        //给房间的每个成员发送房间不存在包
                        lers.m_roomisExist=false;
                        //如果是，将房间销毁，从三个映射表中将该房间删除
                        m_mapRoomidToRoomInfo.erase(user->m_roomid);
                        m_mapRoomidToMemberlist.erase(user->m_roomid);
                        m_roomList.remove(user->m_roomid);
                        memlst.remove(user->m_roomid);
                    }else{
                        //如果不是，在房间信息中更新当前人数、空闲座位
                        RoomInfo* room=nullptr;
                        if(!m_mapRoomidToRoomInfo.find(user->m_roomid,room))return;
                        room->m_currentNum--;
                        room->m_seat[user->m_seat]=0;
                        //将该用户从房间成员列表中删除，给该房间的其他成员发送退出包
                        lers.m_roomisExist=true;
                        lers.m_id=user->m_seat;
                        memlst.remove(user->m_roomid);
                        m_mapRoomidToMemberlist.insert(user->m_roomid,memlst);
                    }
                    for(auto ite=memlst.begin();ite!=memlst.end();ite++){
                        UserInfo* muser;
                        if(!m_mapIdToUserInfo.find(*ite,muser))continue;
                        SendData(muser->m_sockfd,(char*)&lers,sizeof(lers));
                    }
                }
                delete user;
                return;
            }
            user=new UserInfo;
            user->m_id=id;
            user->m_sockfd=clientfd;
            strcpy(user->m_userName,rq->username);
            m_mapIdToUserInfo.insert(id,user);
            //给客户发送个人信息
            STRU_USER_INFO_RQ userRq;
            sprintf(sql,"select id,username,name,sex,icon,level from t_user where username='%s'",
                    rq->username);
            m_sql->SelectMysql(sql,6,lst);
            userRq.m_UserID=atoi(lst.front().c_str());
            lst.pop_front();
            strcpy(userRq.m_username,lst.front().c_str());
            lst.pop_front();
            strcpy(userRq.m_name,lst.front().c_str());
            lst.pop_front();
            strcpy(userRq.m_sex,lst.front().c_str());
            lst.pop_front();
            userRq.m_iconid=atoi(lst.front().c_str());
            lst.pop_front();
            userRq.m_level=atoi(lst.front().c_str());
            lst.pop_front();
            SendData(clientfd,(char*)&userRq,sizeof(userRq));
        }else rs.result=password_error;
    }
    //返回结果
    SendData(clientfd,(char*)&rs,sizeof(rs));
}

void CLogic::QuitLogin(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("QuitLogin:%d\n",clientfd);
    STRU_CLIENTQUITLOGIN_RQ* rq=(STRU_CLIENTQUITLOGIN_RQ*)szbuf;
    int id=rq->m_UserID;
    if(m_mapIdToUserInfo.IsExist(id))m_mapIdToUserInfo.erase(id);
    //TODO:给好友发送离线申请
}

void CLogic::CreateRoom(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("CreateRoom:%d\n",clientfd);
    STRU_CREATEROOM_RQ* rq=(STRU_CREATEROOM_RQ*)szbuf;
    //保存映射
    RoomInfo* room=new RoomInfo;
    int id=(rand()%899999)+100000;
    while(m_mapRoomidToRoomInfo.IsExist(id))id=(rand()%899999)+100000;
    room->m_roomid=id;
    room->m_lock=rq->lock;
    if(room->m_lock)strcpy(room->m_passwd,rq->passwd);
    room->m_playing=false;
    room->m_beginNum=rq->maxcount;
    room->m_currentNum++;
    room->m_minLevel=rq->level;
    room->m_mode=rq->mode;
    room->m_playMethod=rq->playMethod;

    UserInfo* user=nullptr;
    if(!m_mapIdToUserInfo.find(rq->m_UserID,user))return;
    user->m_seat=1;
    room->m_seat[1]=1;
    user->m_roomid=id;
    m_mapRoomidToRoomInfo.insert(room->m_roomid,room);
    m_roomList.push_back(room->m_roomid);
    list<int>lst;
    lst.push_back(rq->m_UserID);
    m_mapRoomidToMemberlist.insert(room->m_roomid,lst);

    //回复
    STRU_CREATEROOM_RS rs;
    rs.m_RoomId=room->m_roomid;
    rs.lock=room->m_lock;
    rs.mode=room->m_mode;
    strcpy(rs.passwd,room->m_passwd);
    rs.maxcount=room->m_beginNum;
    rs.m_lResult=create_success;
    SendData(clientfd,(char*)&rs,sizeof(rs));

    STRU_ROOM_MEMBER_RQ myrq;
    list<string>infolst;
    char mysql[1024];
    sprintf(mysql,"select icon,name,id,level,sex from t_user where id=%d",rq->m_UserID);
    m_sql->SelectMysql(mysql,5,infolst);
    myrq.m_icon=atoi(infolst.front().c_str());
    infolst.pop_front();
    strcpy(myrq.m_szUser,infolst.front().c_str());
    infolst.pop_front();
    myrq.m_UserID=atoi(infolst.front().c_str());
    infolst.pop_front();
    myrq.m_level=atoi(infolst.front().c_str());
    infolst.pop_front();
    strcpy(myrq.m_sex,infolst.front().c_str());
    infolst.pop_front();
    myrq.m_seat=user->m_seat;
    SendData(clientfd,(char*)&myrq,sizeof(myrq));

}

void CLogic::RoomList(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("RoomList:%d\n",clientfd);
    //拆包
    STRU_ROOMLIST_RQ* rq=(STRU_ROOMLIST_RQ*)szbuf;
    //判断房间id是否有值，如果有，按照房间号在映射中获取房间信息
    STRU_ROOMLIST_RS rs;
    RoomInfo* room=nullptr;
    if(rq->roomid>0){
        m_mapRoomidToRoomInfo.find(rq->roomid,room);
        //发送
        if(room){
            rs.mode=room->m_mode;
            rs.pass=room->m_lock;
            rs.count=room->m_beginNum;
            rs.method=room->m_playMethod;
            strcpy(rs.passwd,room->m_passwd);
            rs.roomid=room->m_roomid;
            rs.level=room->m_minLevel;
            rs.state=room->m_playing;
            rs.currentCou=room->m_currentNum;
            SendData(clientfd,(char*)&rs,sizeof(rs));
        }
        return;
    }

    //如果没有，按照模式和玩法发送房间信息
    for(auto ite=m_roomList.begin();ite!=m_roomList.end();ite++){
        m_mapRoomidToRoomInfo.find(*ite,room);
        if(room&&(room->m_mode==rq->mode||rq->mode==-1)&&
                (room->m_playMethod==rq->method||rq->method==-1)){
            rs.mode=room->m_mode;
            rs.pass=room->m_lock;
            rs.count=room->m_beginNum;
            rs.method=room->m_playMethod;
            strcpy(rs.passwd,room->m_passwd);
            rs.roomid=room->m_roomid;
            rs.level=room->m_minLevel;
            rs.state=room->m_playing;
            rs.currentCou=room->m_currentNum;
            SendData(clientfd,(char*)&rs,sizeof(rs));
        }
    }
}

void CLogic::JoinRoom(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("JoinRoom:%d\n",clientfd);
    //拆包
    STRU_JOINROOM_RQ* rq=(STRU_JOINROOM_RQ*)szbuf;
    STRU_JOINROOM_RS rs;
    //查找个人信息
    UserInfo* user=nullptr;
    if(!m_mapIdToUserInfo.find(rq->m_UserID,user))return;
    //查找房间信息
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rq->m_RoomID,room)){
        rs.m_lResult=room_no_exist;
        SendData(clientfd,(char*)&rs,sizeof(rs));
        return;
    }
    else{
        //判断等级是否符合
        list<string>levellst;
        char levelsql[1024];
        sprintf(levelsql,"select level from t_user where id=%d",rq->m_UserID);
        m_sql->SelectMysql(levelsql,1,levellst);
        if(atoi(levellst.front().c_str())<room->m_minLevel){
            rs.m_lResult=level_unqualified;
            SendData(clientfd,(char*)&rs,sizeof(rs));
        }
        else{
            list<int>memerlst;
            m_mapRoomidToMemberlist.find(rq->m_RoomID,memerlst);
            //写入房间成员
            for(int i=1;i<13;i++){
                if(room->m_seat[i]==0){
                    user->m_seat=i;
                    room->m_seat[i]=1;
                    user->m_roomid=room->m_roomid;
                    break;
                }
            }
            memerlst.push_back(rq->m_UserID);
            m_mapRoomidToMemberlist.insert(room->m_roomid,memerlst);

            //加入房间回复
            rs.m_lResult=join_success;
            rs.lock=room->m_lock;
            rs.look=room->m_playing||++(room->m_currentNum)>room->m_beginNum?true:false;
            rs.mode=room->m_mode;
            strcpy(rs.passwd,room->m_passwd);
            rs.maxcount=room->m_beginNum;
            rs.m_RoomID=room->m_roomid;
            SendData(clientfd,(char*)&rs,sizeof(rs));
            //TODO:观战模式写了一办，如果是观战的话，有些映射不能加，有些成员信息不能发

            //新成员信息包
            STRU_ROOM_MEMBER_RQ myrq;
            list<string>infolst;
            char mysql[1024];
            sprintf(mysql,"select icon,name,id,level,sex from t_user where id=%d",rq->m_UserID);
            m_sql->SelectMysql(mysql,5,infolst);
            myrq.m_icon=atoi(infolst.front().c_str());
            infolst.pop_front();
            strcpy(myrq.m_szUser,infolst.front().c_str());
            infolst.pop_front();
            myrq.m_UserID=atoi(infolst.front().c_str());
            infolst.pop_front();
            myrq.m_level=atoi(infolst.front().c_str());
            infolst.pop_front();
            strcpy(myrq.m_sex,infolst.front().c_str());
            infolst.pop_front();
            myrq.m_seat=user->m_seat;
            SendData(clientfd,(char*)&myrq,sizeof(myrq));



            //原有成员信息包
            for(auto ite=memerlst.begin();ite!=memerlst.end();ite++){
                if(*ite!=rq->m_UserID){
                    //头像、昵称、id、等级、性别、座位号
                    char sql[1024];
                    sprintf(sql,"select icon,name,id,level,sex from t_user where id=%d",*ite);
                    m_sql->SelectMysql(sql,5,infolst);
                    STRU_ROOM_MEMBER_RQ memrq;
                    memrq.m_icon=atoi(infolst.front().c_str());
                    infolst.pop_front();
                    strcpy(memrq.m_szUser,infolst.front().c_str());
                    infolst.pop_front();
                    memrq.m_UserID=atoi(infolst.front().c_str());
                    infolst.pop_front();
                    memrq.m_level=atoi(infolst.front().c_str());
                    infolst.pop_front();
                    strcpy(memrq.m_sex,infolst.front().c_str());
                    infolst.pop_front();
                    UserInfo* memuser=nullptr;
                    if(!m_mapIdToUserInfo.find(*ite,memuser))break;
                    memrq.m_seat=memuser->m_seat;
                    //给改用户发送其他成员信息
                    SendData(clientfd,(char*)&memrq,sizeof(memrq));
                    //给其他成员发送该用户信息
                    SendData(memuser->m_sockfd,(char*)&myrq,sizeof(myrq));
                }
            }
        }
    }
}

void CLogic::LeaveRoom(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("LeaveRoom:%d\n",clientfd);
    //判断是不是房主
    STRU_LEAVEROOM_RQ* rq=(STRU_LEAVEROOM_RQ*)szbuf;
    UserInfo* user=nullptr;
    if(!m_mapIdToUserInfo.find(rq->m_nUserId,user))return;
    user->m_roomid=0;
    STRU_LEAVEROOM_RS rs;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(rq->m_RoomId,lst))return;
    if(user->m_seat==1){
        //给房间的每个成员发送房间不存在包
        rs.m_roomisExist=false;
        //如果是，将房间销毁，从三个映射表中将该房间删除
        m_mapRoomidToRoomInfo.erase(rq->m_RoomId);
        m_mapRoomidToMemberlist.erase(rq->m_RoomId);
        m_roomList.remove(rq->m_RoomId);
        lst.remove(rq->m_nUserId);
    }else{
        //如果不是，在房间信息中更新当前人数、空闲座位
        RoomInfo* room=nullptr;
        if(!m_mapRoomidToRoomInfo.find(rq->m_RoomId,room))return;
        room->m_currentNum--;
        room->m_seat[user->m_seat]=0;
        //将该用户从房间成员列表中删除，给该房间的其他成员发送退出包
        rs.m_roomisExist=true;
        rs.m_id=user->m_seat;
        lst.remove(rq->m_nUserId);
        m_mapRoomidToMemberlist.insert(rq->m_RoomId,lst);
    }

    for(auto ite=lst.begin();ite!=lst.end();ite++){
        UserInfo* muser;
        if(!m_mapIdToUserInfo.find(*ite,muser))continue;
        SendData(muser->m_sockfd,(char*)&rs,sizeof(rs));
    }
}

void CLogic::BeginGameTest(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("BeginGameTest:%d\n",clientfd);
    //拆包
    STRU_BEGINGAMETEST_RQ* rq=(STRU_BEGINGAMETEST_RQ*)szbuf;
    //找到房间
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rq->m_RoomId,room))return;
    //给房间的每个成员发送准备开始游戏包
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(rq->m_RoomId,lst))return;
    STRU_BEGINGAMETEST_RS rs;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        UserInfo* user=nullptr;
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,(char*)&rs,sizeof(rs));
    }
}
