#ifndef CLOGIC_H
#define CLOGIC_H

#include"TCPKernel.h"
#include<list>

class CLogic
{
public:
    CLogic( TcpKernel* pkernel )
    {
        m_pKernel = pkernel;
        m_sql = pkernel->m_sql;
        m_tcp = pkernel->m_tcp;
    }
public:
    //设置协议映射
    void setNetPackMap();
    /************** 发送数据*********************/
    void SendData( sock_fd clientfd, char*szbuf, int nlen )
    {
        m_pKernel->SendData( clientfd ,szbuf , nlen );
    }
    /************** 网络处理 *********************/
    void RegisterRq( sock_fd clientfd, char*szbuf, int nlen );
    void LoginRq( sock_fd clientfd, char*szbuf, int nlen );
    void QuitLogin( sock_fd clientfd, char*szbuf, int nlen );
    void CreateRoom( sock_fd clientfd, char*szbuf, int nlen );
    void RoomList( sock_fd clientfd, char*szbuf, int nlen );
    void JoinRoom( sock_fd clientfd, char*szbuf, int nlen );
    void LeaveRoom( sock_fd clientfd, char*szbuf, int nlen );
    void BeginGameTest( sock_fd clientfd, char*szbuf, int nlen );
    void BeginGame( sock_fd clientfd, char*szbuf, int nlen );
    void Skyblack( sock_fd clientfd, char*szbuf, int nlen );
    void SkyblackEnd( sock_fd clientfd, char*szbuf, int nlen );
    void NWSilverWater( sock_fd clientfd, char*szbuf, int nlen );
    void ToBePolice( sock_fd clientfd, char*szbuf, int nlen );
    void PoliceEnd( sock_fd clientfd, char*szbuf, int nlen );
    void SpeakEnd( sock_fd clientfd, char*szbuf, int nlen );
    void BePolice( sock_fd clientfd, char*szbuf, int nlen );
    void SpeakStateEnd( sock_fd clientfd, char*szbuf, int nlen );
    void Vote( sock_fd clientfd, char*szbuf, int nlen );
    void VoteEnd( sock_fd clientfd, char*szbuf, int nlen );
    void SpeakOrder( sock_fd clientfd, char*szbuf, int nlen );
    void DayExile( sock_fd clientfd, char*szbuf, int nlen );
    void AudioData( sock_fd clientfd, char*szbuf, int nlen );
    void SpeakPause( sock_fd clientfd, char*szbuf, int nlen );
    void LrKillSelf( sock_fd clientfd, char*szbuf, int nlen );
    void ChatRq( sock_fd clientfd, char*szbuf, int nlen );
    void FriendZiLiao( sock_fd clientfd, char*szbuf, int nlen );
    void Space( sock_fd clientfd, char*szbuf, int nlen );
    void SpaceOpt( sock_fd clientfd, char*szbuf, int nlen );
    void SpaceComment( sock_fd clientfd, char*szbuf, int nlen );
    void SpaceAdd( sock_fd clientfd, char*szbuf, int nlen );
    void UserFind( sock_fd clientfd, char*szbuf, int nlen );
    void Email( sock_fd clientfd, char*szbuf, int nlen );
    void EmailClear( sock_fd clientfd, char*szbuf, int nlen );
    void AddFriend( sock_fd clientfd, char*szbuf, int nlen );
    void AddFriendRs( sock_fd clientfd, char*szbuf, int nlen );
    void DeleteFriend( sock_fd clientfd, char*szbuf, int nlen );
    void ChatMsg( sock_fd clientfd, char*szbuf, int nlen );
    void YaoQing( sock_fd clientfd, char*szbuf, int nlen );
    void AudioRq( sock_fd clientfd, char*szbuf, int nlen );
    void AudioRs( sock_fd clientfd, char*szbuf, int nlen );
    void VideoRq( sock_fd clientfd, char*szbuf, int nlen );
    void VideoRs( sock_fd clientfd, char*szbuf, int nlen );
    void AudioWithFrame( sock_fd clientfd, char*szbuf, int nlen );
    void VideoWithFrame( sock_fd clientfd, char*szbuf, int nlen );
    void AuViQuit( sock_fd clientfd, char*szbuf, int nlen );
    /*******************************************/

private:
    TcpKernel* m_pKernel;
    CMysql * m_sql;
    Block_Epoll_Net * m_tcp;
    MyMap<int,UserInfo*>m_mapIdToUserInfo;//id->socket的映射，保存在线用户
    MyMap<int,RoomInfo*>m_mapRoomidToRoomInfo;//roomid->roominfo的映射，保存房间
    std::list<int>m_roomList;//房间号列表
    MyMap<int,list<int>>m_mapRoomidToMemberlist;//roomid->房间成员列表的映射
    MyMap<int,MyMap<int,int>>m_mapRoomidToSeatidToSockfd;//房间号-》座位号-》sockfd
};

#endif // CLOGIC_H
