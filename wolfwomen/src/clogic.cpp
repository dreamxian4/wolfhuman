#include "clogic.h"
#include<string>

void CLogic::setNetPackMap()
{
    NetPackMap(DEF_PACK_REGISTER_RQ)    = &CLogic::RegisterRq;
    NetPackMap(DEF_PACK_LOGIN_RQ)    = &CLogic::LoginRq;
    NetPackMap(DEF_PACK_CLIENTQUITLOGIN_RQ)    = &CLogic::QuitLogin;
    NetPackMap(DEF_PACK_CREATEROOM_RQ)    = &CLogic::CreateRoom;
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
                "insert into t_user(username,password,sex,icon,name)values('%s','%s','%s',1,'%s')"
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
            sprintf(sql,"select id,username,name,sex,icon from t_user where username='%s'",
                    rq->username);
            m_sql->SelectMysql(sql,5,lst);
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
    //语音房基础模式表
    //视频房基础模式表
    //表属性：房间id（6位数），房主，开始人数，当前人数，房间状态，等级，是否加密，密码
}
