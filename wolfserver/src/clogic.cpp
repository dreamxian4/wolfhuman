#include "clogic.h"
#include<string>
#include<random>
#include<vector>

void CLogic::setNetPackMap()
{
    NetPackMap(DEF_PACK_REGISTER_RQ)        = &CLogic::RegisterRq;
    NetPackMap(DEF_PACK_LOGIN_RQ)           = &CLogic::LoginRq;
    NetPackMap(DEF_PACK_CLIENTQUITLOGIN_RQ) = &CLogic::QuitLogin;
    NetPackMap(DEF_PACK_CREATEROOM_RQ)      = &CLogic::CreateRoom;
    NetPackMap(DEF_PACK_ROOMLIST_RQ)        = &CLogic::RoomList;
    NetPackMap(DEF_PACK_JOINROOM_RQ)        = &CLogic::JoinRoom;
    NetPackMap(DEF_PACK_LEAVEROOM_RQ)       = &CLogic::LeaveRoom;
    NetPackMap(DEF_PACK_BEGINGAMETEST_RQ)   = &CLogic::BeginGameTest;
    NetPackMap(DEF_PACK_BEGINGAME_RQ)       = &CLogic::BeginGame;
    NetPackMap(DEF_PACK_SKYBLACK_RS)        = &CLogic::Skyblack;
    NetPackMap(DEF_PACK_SKYBLACK_END)       = &CLogic::SkyblackEnd;
    NetPackMap(DEF_PACK_LRTONW_SKYBLK_RS)   = &CLogic::NWSilverWater;
    NetPackMap(DEF_PACK_TOBEPOLICE_RS)      = &CLogic::ToBePolice;
    NetPackMap(DEF_PACK_POLICEEND)          = &CLogic::PoliceEnd;
    NetPackMap(DEF_PACK_SPEAK_RS)           = &CLogic::SpeakEnd;
    NetPackMap(DEF_PACK_BEPOLICE_RS)        = &CLogic::BePolice;
    NetPackMap(DEF_PACK_SPEAKSTATE_END)     = &CLogic::SpeakStateEnd;
    NetPackMap(DEF_PACK_VOTE_RQ)            = &CLogic::Vote;
    NetPackMap(DEF_PACK_VOTE_END)           = &CLogic::VoteEnd;
    NetPackMap(DEF_PACK_SPEAK_ORDER)        = &CLogic::SpeakOrder;
    NetPackMap(DEF_PACK_DAY_EXILE)          = &CLogic::DayExile;
    NetPackMap(DEF_PACK_AUDIO_FRAME)        = &CLogic::AudioData;
    NetPackMap(DEF_PACK_SPEAK_PAUSE)        = &CLogic::SpeakPause;
    NetPackMap(DEF_PACK_LR_KILLSELF)        = &CLogic::LrKillSelf;
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
                        memlst.remove(user->m_id);
                    }else{
                        //如果不是，在房间信息中更新当前人数、空闲座位
                        RoomInfo* room=nullptr;
                        if(!m_mapRoomidToRoomInfo.find(user->m_roomid,room))return;
                        room->m_currentNum--;
                        room->m_seat[user->m_seat]=0;
                        //将该用户从房间成员列表中删除，给该房间的其他成员发送退出包
                        lers.m_roomisExist=true;
                        lers.m_id=user->m_seat;
                        memlst.remove(user->m_id);
                        m_mapRoomidToMemberlist.insert(user->m_roomid,memlst);
                    }
                    UserInfo* muser;
                    for(auto ite=memlst.begin();ite!=memlst.end();ite++){
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
            STRU_TCP_FRIEND_INFO loginInfo;
            sprintf(sql,"select id,username,name,sex,icon,level from t_user where username='%s'",
                    rq->username);
            m_sql->SelectMysql(sql,6,lst);
            userRq.m_UserID=atoi(lst.front().c_str());
            loginInfo.userid=userRq.m_UserID;
            lst.pop_front();
            strcpy(userRq.m_username,lst.front().c_str());
            strcpy(loginInfo.username,userRq.m_username);
            lst.pop_front();
            strcpy(userRq.m_name,lst.front().c_str());
            strcpy(loginInfo.name,userRq.m_name);
            lst.pop_front();
            strcpy(userRq.m_sex,lst.front().c_str());
            strcpy(loginInfo.sex,userRq.m_sex);
            lst.pop_front();
            userRq.m_iconid=atoi(lst.front().c_str());
            loginInfo.icon=userRq.m_iconid;
            lst.pop_front();
            userRq.m_level=atoi(lst.front().c_str());
            loginInfo.level=userRq.m_level;
            lst.pop_front();
            loginInfo.state=1;
            SendData(clientfd,(char*)&userRq,sizeof(userRq));
            //给用户发送好友信息
            //4.根据用户id查找所有的好友id
            list<string> resultList;
            char sqlBuf[1024] = "";
            sprintf(sqlBuf, "select id_b from t_friend where id_a = %d;", user->m_id);
            if (!m_sql->SelectMysql(sqlBuf, 1, resultList)) {
                cout << "selectMysql error:" << sqlBuf << endl;
                return;
            }

            //5.遍历查找结果
            int friendId = 0;
            STRU_TCP_FRIEND_INFO friendInfo;
            UserInfo* friendIf=nullptr;
            while (!resultList.empty()) {
                //6.取出好友id，根据好友id查找好友信息
                friendId = atoi(resultList.front().c_str());
                resultList.pop_front();
                //1.保存id
                friendInfo.userid = friendId;

                //2.根据id查信息
                list<string> resultList;
                char sqlBuf[1024] = "";
                sprintf(sqlBuf, "select username,sex,icon,name,level from t_user where id=%d;", friendId);
                if (!m_sql->SelectMysql(sqlBuf, 5, resultList)) {
                    cout << "selectMysql error:" << sqlBuf << endl;
                    return;
                }

                //3.遍历查询结果
                if (resultList.size() == 5) {
                    strcpy(friendInfo.username, resultList.front().c_str());
                    resultList.pop_front();

                    strcpy(friendInfo.sex, resultList.front().c_str());
                    resultList.pop_front();

                    friendInfo.icon = atoi(resultList.front().c_str());
                    resultList.pop_front();

                    strcpy(friendInfo.name, resultList.front().c_str());
                    resultList.pop_front();

                    friendInfo.level = atoi(resultList.front().c_str());
                    resultList.pop_front();
                }

                //4.判断用户是否存在
                if (m_mapIdToUserInfo.find(friendId,friendIf)) {
                    friendInfo.state = 1;		//在线
                }
                else {
                    friendInfo.state = 2;		//不在线
                }


                //7.把好友信息发给客户端
                SendData(clientfd, (char*)&friendInfo, sizeof(friendInfo));

                //8.如果好友在线，通知好友登陆人上线
                if (!friendIf) {
                    continue;//好友不在线，进行下一次循环
                }
                //9.如果好友在线，取出好友的套接字，给好友发送登陆人的上线通知
                SendData(friendIf->m_sockfd, (char*)&loginInfo, sizeof(loginInfo));
            }
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
    UserInfo* user=nullptr;
    if(!m_mapIdToUserInfo.find(id,user))return;
    if(user->m_roomid){
        //判断是不是房主
        STRU_LEAVEROOM_RS rs;
        list<int>lst;
        if(!m_mapRoomidToMemberlist.find(rq->roomid,lst))return;
        if(user->m_seat==1){
            //给房间的每个成员发送房间不存在包
            rs.m_roomisExist=false;
            //如果是，将房间销毁，从三个映射表中将该房间删除
            m_mapRoomidToRoomInfo.erase(rq->roomid);
            m_mapRoomidToMemberlist.erase(rq->roomid);
            m_roomList.remove(rq->roomid);
            lst.remove(rq->roomid);
        }else{
            //如果不是，在房间信息中更新当前人数、空闲座位
            RoomInfo* room=nullptr;
            if(!m_mapRoomidToRoomInfo.find(rq->roomid,room))return;
            room->m_currentNum--;
            room->m_seat[user->m_seat]=0;
            //将该用户从房间成员列表中删除，给该房间的其他成员发送退出包
            rs.m_roomisExist=true;
            rs.m_id=user->m_seat;
            lst.remove(rq->roomid);
            m_mapRoomidToMemberlist.insert(rq->roomid,lst);
            //判断房间是否已经开始游戏并且推出的玩家还活着
            if(room->m_playing&&user->m_alive){
                if(room->m_identify[user->m_seat]==2)room->i_farmerNum--;
                else if(room->m_identify[user->m_seat]==3)room->i_wolfNum--;
                else room->i_godNum--;
            }
        }

        user->m_roomid=0;

        for(auto ite=lst.begin();ite!=lst.end();ite++){
            UserInfo* muser;
            if(!m_mapIdToUserInfo.find(*ite,muser))continue;
            SendData(muser->m_sockfd,(char*)&rs,sizeof(rs));
        }
    }
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
            user->m_alive=true;
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
        //判断房间是否已经开始游戏并且推出的玩家还活着
        if(room->m_playing&&user->m_alive){
            if(room->m_identify[user->m_seat]==2)room->i_farmerNum--;
            else if(room->m_identify[user->m_seat]==3)room->i_wolfNum--;
            else room->i_godNum--;
        }
    }

    user->m_roomid=0;
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

void CLogic::BeginGame(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("BeginGame:%d\n",clientfd);
    //拆包
    STRU_BEGINGAME_RQ* rq=(STRU_BEGINGAME_RQ*)szbuf;
    //找到房间信息
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rq->m_RoomId,room))return;
    room->m_playing=true;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(rq->m_RoomId,lst))return;
    STRU_BEGINGAME_RS rs;
    STRU_SKYBLACK_RQ skyrq;

    //为每个玩家随机生成身份牌
    //预言家,女巫,平民,狼人,猎人,守卫
    vector<int>pai(6);
    switch(room->m_beginNum){
    case 6:
        pai={1,1,2,2,0,0};
        break;
    case 7:
        pai={1,1,3,2,0,0};
        break;
    case 8:
        pai={1,1,2,3,1,0};
        break;
    case 9:
        pai={1,1,3,3,1,0};
        break;
    case 10:
        pai={1,1,4,3,1,0};
        break;
    case 11:
        pai={1,1,3,4,1,1};
        break;
    case 12:
        pai={1,1,4,4,1,1};
        break;
    }
    room->i_godNum=pai[0]+pai[1]+pai[4]+pai[5];
    room->i_wolfNum=pai[3];
    room->i_farmerNum=pai[2];
    MyMap<int,int>map;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        UserInfo* user=nullptr;
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        int iden=rand()%6;
        while(pai[iden]==0)iden=rand()%6;
        //将身份信息发送给玩家
        rs.m_iden=iden;
        room->m_identify[user->m_seat]=iden;
        room->i_day=1;
        pai[iden]--;
        SendData(user->m_sockfd,(char*)&rs,sizeof(rs));
        //发送天黑包
        SendData(user->m_sockfd,(char*)&skyrq,sizeof(skyrq));
        map.insert(user->m_seat,user->m_sockfd);
    }
    m_mapRoomidToSeatidToSockfd.insert(rq->m_RoomId,map);
}

void CLogic::Skyblack(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("Skyblack:%d\n",clientfd);
    //拆包
    STRU_SKYBLACK_RS* rs=(STRU_SKYBLACK_RS*)szbuf;
    //通过房间号找到房间
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rs->m_roomid,room))return;
    STRU_YYJ_SKYBLK yyj;
    list<int>lst;
    switch(rs->m_operate){//1预言家 2狼人  3女巫毒 4守卫 0无操作
    case 0:
        break;
    case 1://处理预言家
        yyj.iden=room->m_identify[rs->m_toseat];
        yyj.id=rs->m_toseat;
        SendData(clientfd,(char*)&yyj,sizeof(yyj));
        break;
    case 2://处理狼人
        if(!m_mapRoomidToMemberlist.find(rs->m_roomid,lst))return;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            UserInfo* user=nullptr;
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            STRU_LR_SKYBLK lr;
            lr.id=rs->m_seat;
            lr.toid=rs->m_toseat;
            SendData(user->m_sockfd,(char*)&lr,sizeof(lr));
            for(int i=0;i<4;i++)if(room->i_kill[i]==0){
                room->i_kill[i]=rs->m_toseat;
                break;
            }
        }
        break;
    case 3://处理女巫
        room->i_die[1]=rs->m_toseat;
        break;
    case 4://处理守卫
        if(room->i_die[0]==rs->m_toseat)room->i_die[0]=0;
        break;
    }
}

void CLogic::SkyblackEnd(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("SkyblackEnd:%d\n",clientfd);
    //拆包
    STRU_SKYBLK_END* end=(STRU_SKYBLK_END*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(end->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(end->roomid,lst))return;
    //判断是半夜还是整夜
    if(end->state==0){
        //如果是半夜，将狼人的杀人信息发送给女巫
        STRU_LRTONW_SKYBLK die;
        int num=0;
        for(int i=0;i<4;i++)if(room->i_kill[i]!=0)num++;
        int kill=num==0?0:rand()%num;
        die.kill=room->i_kill[kill];
        room->i_die[0]=die.kill;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            UserInfo* user=nullptr;
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&die,sizeof(die));
        }
    }else{//整夜
        //判断是不是第一天
        if(room->i_day==1){
            //是：发送竞选警长
            UserInfo* user=nullptr;

            STRU_TOBEPOLICE_RQ rq;
            for(auto ite=lst.begin();ite!=lst.end();ite++){
                if(!m_mapIdToUserInfo.find(*ite,user))continue;
                SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
            }
        }else{
            //不是：发送死亡信息;如果没有人死，不用判断玩家身份，直接发送发言包
            //发送天亮包，包含死亡信息
            STRU_SKYWHT_RQ rq;
            rq.die[0]=room->i_die[0];
            rq.die[1]=room->i_die[1];
            UserInfo* user=nullptr;
            for(auto ite=lst.begin();ite!=lst.end();ite++){
                if(!m_mapIdToUserInfo.find(*ite,user))continue;
                SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
            }
            //更新房间信息
            for(int i=0;i<2;i++){
                if(room->i_die[i]){
                    if(room->m_identify[room->i_die[i]]==2)room->i_farmerNum--;
                    else if(room->m_identify[room->i_die[i]]==3)room->i_wolfNum--;
                    else room->i_godNum--;
                    for(auto ite=lst.begin();ite!=lst.end();ite++){
                        if(!m_mapIdToUserInfo.find(*ite,user))continue;
                        if(user->m_seat==room->i_die[i]){
                            user->m_alive=false;
                            break;
                        }
                    }
                }
            }

            //判断是否结束游戏
            if(room->i_godNum==0||room->i_wolfNum==0||room->i_farmerNum==0){
                //结束：给每个人发送结束包TODO
                STRU_GAMEOVER over;
                for(auto ite=lst.begin();ite!=lst.end();ite++){
                    if(!m_mapIdToUserInfo.find(*ite,user))continue;
                    SendData(user->m_sockfd,(char*)&over,sizeof(over));
                }
            }else{
                //不结束：
                //发送发言信息
                if(room->i_police){
                    //发送给警长，让警长选择发言顺序（顺序或逆序）
                    //警长选择后，服务器将次序发送给客户端
                    //客户端在发送发言结束包的时候，将次序一并发过来，服务端根据次序判断下一个人是谁
                    //发送给所有人，客户端自行判断自己是不是警长
                    STRU_SPEAK_ORDER orderRq;
                    for(auto ite=lst.begin();ite!=lst.end();ite++){
                        if(!m_mapIdToUserInfo.find(*ite,user))continue;
                        SendData(user->m_sockfd,(char*)&orderRq,sizeof(orderRq));
                    }
                }else{
                    //没有警长，从房主开始顺序发言
                    STRU_SPEAKSTATE_BEGIN begin;
                    for(auto ite=lst.begin();ite!=lst.end();ite++){
                        if(!m_mapIdToUserInfo.find(*ite,user))continue;
                        SendData(user->m_sockfd,(char*)&begin,sizeof(begin));
                    }
                    STRU_SPEAK_RQ speakRq;
                    speakRq.state=3;
                    MyMap<int,int>seat;
                    if(!m_mapRoomidToSeatidToSockfd.find(room->m_roomid,seat))return;
                    bool key=true;
                    for(int a=1;a<13;a++){
                        if(seat.IsExist(a)){
                            if(key){
                                speakRq.seat=a;
                                key=false;
                            }else {
                                speakRq.next=a;
                                break;
                            }
                        }
                    }
                    for(auto ite=lst.begin();ite!=lst.end();ite++){
                        if(!m_mapIdToUserInfo.find(*ite,user))continue;
                        SendData(user->m_sockfd,(char*)&speakRq,sizeof(speakRq));
                    }
                }
            }
            memset(room->i_die,0,8);
            memset(room->i_kill,0,16);
        }
    }
}

void CLogic::NWSilverWater(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("NWSilverWater:%d\n",clientfd);
    //女巫救人了
    //找到房间
    STRU_LRTONW_SKYBLK_RS* rs=(STRU_LRTONW_SKYBLK_RS*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rs->roomid,room))return;
    //将杀人清空
    room->i_die[0]=0;
}

void CLogic::ToBePolice(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("BePolice:%d\n",clientfd);
    //拆包
    STRU_TOBEPOLICE_RS* rs=(STRU_TOBEPOLICE_RS*)szbuf;
    RoomInfo* room=nullptr;
    //上警人数+1
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(rs->roomid,lst))return;
    UserInfo* user=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rs->roomid,room))return;
    if(rs->raise)room->i_policeNum++;
    else {
        // 有人放手，判断警上人数，如果只有一个，直接当选警长
        room->i_policeNum--;
        STRU_BEPOLICE_RQ rq;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
        }
    }
    //将上警信息发送给每个玩家
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,szbuf,nlen);
    }
}

void CLogic::PoliceEnd(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("PoliceEnd:%d\n",clientfd);
    //判断上警人数，如果为0或为游戏人数，直接发言，没有警长
    STRU_POLICE_END* end=(STRU_POLICE_END*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(end->roomid,room))return;
    STRU_SPEAK_RQ speakRq;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(end->roomid,lst))return;
    UserInfo* user=nullptr;
    if(room->i_policeNum==0||room->i_policeNum==room->m_beginNum){
        //发送死亡信息
        STRU_SKYWHT_RQ rq;
        rq.die[0]=room->i_die[0];
        rq.die[1]=room->i_die[1];
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
        }

        for(int i=0;i<2;i++){
            if(room->i_die[i]){
                if(room->m_identify[room->i_die[i]]==2)room->i_farmerNum--;
                else if(room->m_identify[room->i_die[i]]==3)room->i_wolfNum--;
                else room->i_godNum--;
            }
        }

        //判断是否结束游戏
        if(room->i_godNum==0||room->i_wolfNum==0||room->i_farmerNum==0){
            //结束：给每个人发送结束包TODO
            STRU_GAMEOVER over;
            for(auto ite=lst.begin();ite!=lst.end();ite++){
                if(!m_mapIdToUserInfo.find(*ite,user))continue;
                SendData(user->m_sockfd,(char*)&over,sizeof(over));
            }
        }else{
            //没有警长，从房主开始顺序发言
            //给所有人发送发言阶段开始包
            //给发言人发送开始发言包
            speakRq.state=2;
            MyMap<int,int>seat;
            if(!m_mapRoomidToSeatidToSockfd.find(room->m_roomid,seat))return;
            bool key=true;
            for(int a=1;a<13;a++){
                if(seat.IsExist(a)){
                    if(key){
                        speakRq.seat=a;
                        key=false;
                    }else {
                        speakRq.next=a;
                        break;
                    }
                }
            }
        }
        memset(room->i_die,0,8);
        memset(room->i_kill,0,16);
    }else if(room->i_policeNum==1){
        //如果只有一个人上警，直接成为警长，发送一个警长包给所有玩家
        STRU_BEPOLICE_RQ rq;
        STRU_SPEAK_ORDER orderRq;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
            //警长选择发言顺序
            SendData(user->m_sockfd,(char*)&orderRq,sizeof(orderRq));
        }
    }else{
        //发送上警玩家发言包
        speakRq.state=1;
        speakRq.seat=1;
    }
    STRU_SPEAKSTATE_BEGIN begin;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,(char*)&begin,sizeof(begin));
        SendData(user->m_sockfd,(char*)&speakRq,sizeof(speakRq));
    }
}

void CLogic::SpeakEnd(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("SpeakEnd:%d\n",clientfd);
    //给所有人发送发言包
    STRU_SPEAK_RS* rs=(STRU_SPEAK_RS*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rs->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(rs->roomid,lst))return;
    UserInfo* user=nullptr;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,szbuf,nlen);
    }
    if(rs->state==4){
        //淘汰玩家发言完毕，发送天黑包和淘汰包
        STRU_DAY_EXILE exile;
        exile.die=rs->seat;
        STRU_SKYBLACK_RQ rq;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&exile,sizeof(exile));
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
        }
    }else{
        STRU_SPEAK_RQ rq;
        MyMap<int,int>seat;
        if(!m_mapRoomidToSeatidToSockfd.find(room->m_roomid,seat))return;
        int next=(rs->seat+rs->next)%room->m_beginNum;
        int a;
        bool key=true;
        rq.seat=rs->seat;
        for(a=1;a<13;a++){
            if(seat.IsExist(next)){
                if(key){
                    rq.seat=next;
                    key=false;
                    next=(next+rs->next)%room->m_beginNum;
                }else {
                    rq.next=next;
                    break;
                }
            }else next=(next+rs->next)%room->m_beginNum;
        }
        if(a==13)rq.next=0;
        rq.state=rs->state;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
        }
    }
}

void CLogic::BePolice(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("BePolice:%d\n",clientfd);
    STRU_BEPOLICE_RS* rs=(STRU_BEPOLICE_RS*)szbuf;
    //设置警长信息
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rs->roomid,room))return;list<int>lst;
    if(!m_mapRoomidToMemberlist.find(rs->roomid,lst));
    UserInfo* user=nullptr;
    room->i_police=rs->seat;
    //当选警长，将警长信息发送给玩家
    if(rs->seat){
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,szbuf,nlen);
        }
    }
    if(room->i_day==1){
        //是第一天，发送死亡信息
        STRU_SKYWHT_RQ rq;
        rq.die[0]=room->i_die[0];
        rq.die[1]=room->i_die[1];
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
        }

        for(int i=0;i<2;i++){
            if(room->i_die[i]){
                if(room->m_identify[room->i_die[i]]==2)room->i_farmerNum--;
                else if(room->m_identify[room->i_die[i]]==3)room->i_wolfNum--;
                else room->i_godNum--;
            }
        }

        //判断是否结束游戏
        if(room->i_godNum==0||room->i_wolfNum==0||room->i_farmerNum==0){
            //结束：给每个人发送结束包TODO
            STRU_GAMEOVER over;
            for(auto ite=lst.begin();ite!=lst.end();ite++){
                if(!m_mapIdToUserInfo.find(*ite,user))continue;
                SendData(user->m_sockfd,(char*)&over,sizeof(over));
            }
        }else{
            //不结束：
            //发送发言信息
            if(rs->seat){
                //发送给警长，让警长选择发言顺序（顺序或逆序）
                //警长选择后，服务器将次序发送给客户端
                //客户端在发送发言结束包的时候，将次序一并发过来，服务端根据次序判断下一个人是谁
                //发送给所有人，客户端自行判断自己是不是警长
                STRU_SPEAK_ORDER orderRq;
                for(auto ite=lst.begin();ite!=lst.end();ite++){
                    if(!m_mapIdToUserInfo.find(*ite,user))continue;
                    SendData(user->m_sockfd,(char*)&orderRq,sizeof(orderRq));
                }
            }else{
                //没有警长，从房主开始顺序发言
                STRU_SPEAK_RQ speakRq;
                STRU_SPEAKSTATE_BEGIN begin;
                speakRq.state=2;
                MyMap<int,int>seat;
                if(!m_mapRoomidToSeatidToSockfd.find(room->m_roomid,seat))return;
                bool key=true;
                for(int a=1;a<13;a++){
                    if(seat.IsExist(a)){
                        if(key){
                            speakRq.seat=a;
                            key=false;
                        }else {
                            speakRq.next=a;
                            break;
                        }
                        break;
                    }
                }
                for(auto ite=lst.begin();ite!=lst.end();ite++){
                    if(!m_mapIdToUserInfo.find(*ite,user))continue;
                    SendData(user->m_sockfd,(char*)&begin,sizeof(begin));
                    SendData(user->m_sockfd,(char*)&speakRq,sizeof(speakRq));
                }
            }
        }
        memset(room->i_die,0,8);
        memset(room->i_kill,0,16);
    }else{
        //不是第一天，说明是撕警徽或者移交警徽，发生在白天结束，不用发送死亡信息，应该发送进入夜晚TODO
        STRU_SKYBLACK_RQ SkyRq;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&SkyRq,sizeof(SkyRq));
        }
    }
}

void CLogic::SpeakStateEnd(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("SpeakStateEnd:%d\n",clientfd);
    STRU_SPEAKSTATE_END* end=(STRU_SPEAKSTATE_END*)szbuf;
    //给玩家发送投票包
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(end->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(end->roomid,lst))return;
    UserInfo* user=nullptr;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,szbuf,nlen);
    }
}

void CLogic::Vote(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("Vote:%d\n",clientfd);
    //保存投票结果
    STRU_VOTE_RQ* rq=(STRU_VOTE_RQ*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(rq->roomid,room))return;
    room->i_vote[rq->seat]=rq->toseat;
}

void CLogic::VoteEnd(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("VoteEnd:%d\n",clientfd);
    //将投票结果发送给房间成员
    STRU_VOTE_END* end=(STRU_VOTE_END*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(end->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(end->roomid,lst))return;
    UserInfo* user=nullptr;
    STRU_VOTE_RS rs;
    rs.state=end->state;
    memcpy(rs.result,room->i_vote,sizeof(int)*13);
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,(char*)&rs,sizeof(rs));
    }
    //清空结果数组
    memset(room->i_vote,0,sizeof(int)*13);
}

void CLogic::SpeakOrder(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("VoteEnd:%d\n",clientfd);
    //转发给所有玩家
    STRU_SPEAK_ORDER* order=(STRU_SPEAK_ORDER*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(order->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(order->roomid,lst))return;
    UserInfo* user=nullptr;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,szbuf,nlen);
    }
    STRU_SPEAK_RQ speakRq;
    speakRq.state=3;
    MyMap<int,int>seat;
    if(!m_mapRoomidToSeatidToSockfd.find(room->m_roomid,seat))return;
    int next=(order->seat+order->next)%room->m_beginNum;
    int a;
    bool key=true;
    speakRq.seat=order->seat;
    for(a=1;a<13;a++){
        if(seat.IsExist(next)){
            if(key){
                speakRq.seat=next;
                next=(next+order->next)%room->m_beginNum;
                key=false;
            }else {
                speakRq.next=next;
                break;
            }
        }else next=(next+order->next)%room->m_beginNum;
    }
    if(a==13)speakRq.next=0;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,(char*)&speakRq,sizeof(speakRq));
    }
}

void CLogic::DayExile(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("DayExile:%d\n",clientfd);
    STRU_DAY_EXILE* exile=(STRU_DAY_EXILE*)szbuf;
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(exile->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(exile->roomid,lst))return;
    UserInfo *user=nullptr;
    //更新房间状态
    if(room->m_identify[exile->die]==2)room->i_farmerNum--;
    else if(room->m_identify[exile->die]==3)room->i_wolfNum--;
    else room->i_godNum--;

    //判断是否结束游戏
    if(room->i_godNum==0||room->i_wolfNum==0||room->i_farmerNum==0){
        //如果游戏结束：发送游戏结束包
        STRU_GAMEOVER over;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&over,sizeof(over));
        }
    }else{
        //不结束：给淘汰玩家发送发言包；结束发言后，发送天黑包
        MyMap<int,int>ma;
        if(!m_mapRoomidToSeatidToSockfd.find(room->m_roomid,ma))return;
        STRU_SPEAK_RQ rq;
        rq.state=4;
        rq.seat=exile->die;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
        }
        //将淘汰玩家从存活玩家中删除;
        ma.erase(exile->die);
        m_mapRoomidToSeatidToSockfd.insert(exile->roomid,ma);
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            if(user->m_seat==exile->die){
                user->m_alive=false;
                break;
            }
        }
    }
}

void CLogic::AudioData(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    char* tmp=szbuf;
    //反序列化
    tmp+=sizeof(int);
    int zuowei=*(int*)tmp;//按照四个字节读
    tmp+=sizeof(int);
    int roomid=*(int*)tmp;
    //roomid->列表
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(roomid,lst))return;
    UserInfo* user=nullptr;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        SendData(user->m_sockfd,szbuf,nlen);
    }
}

void CLogic::SpeakPause(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("SpeakPause:%d\n",clientfd);
    //拆包
    STRU_SPEAKPAUSE* pause=(STRU_SPEAKPAUSE*)szbuf;
    //找到房间成员列表
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(pause->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(pause->roomid,lst))return;
    UserInfo* user;
    for(auto ite=lst.begin();ite!=lst.end();ite++){
        if(!m_mapIdToUserInfo.find(*ite,user))continue;
        //发送暂停发言包
        SendData(user->m_sockfd,szbuf,nlen);
    }
}

void CLogic::LrKillSelf(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("LrKillSelf:%d\n",clientfd);
    //拆包
    STRU_LR_KILLSELF* kill=(STRU_LR_KILLSELF*)szbuf;
    //找到房间成员列表
    RoomInfo* room=nullptr;
    if(!m_mapRoomidToRoomInfo.find(kill->roomid,room))return;
    list<int>lst;
    if(!m_mapRoomidToMemberlist.find(kill->roomid,lst))return;
    UserInfo* user;
    //更新房间状态
    if(room->m_identify[kill->seat]==2)room->i_farmerNum--;
    else if(room->m_identify[kill->seat]==3)room->i_wolfNum--;
    else room->i_godNum--;

    //判断是否结束游戏
    if(room->i_godNum==0||room->i_wolfNum==0||room->i_farmerNum==0){
        //如果游戏结束：发送游戏结束包
        STRU_GAMEOVER over;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&over,sizeof(over));
        }
    }else{
        //不结束：给淘汰玩家发送发言包；结束发言后，发送天黑包
        MyMap<int,int>ma;
        if(!m_mapRoomidToSeatidToSockfd.find(room->m_roomid,ma))return;
        STRU_SPEAK_RQ rq;
        rq.state=4;
        rq.seat=kill->seat;
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            SendData(user->m_sockfd,(char*)&rq,sizeof(rq));
        }
        //将淘汰玩家从存活玩家中删除;
        ma.erase(kill->seat);
        m_mapRoomidToSeatidToSockfd.insert(kill->roomid,ma);
        for(auto ite=lst.begin();ite!=lst.end();ite++){
            if(!m_mapIdToUserInfo.find(*ite,user))continue;
            if(user->m_seat==kill->seat){
                user->m_alive=false;
                break;
            }
        }
    }
}
