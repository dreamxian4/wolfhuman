#include "maindialog.h"
#include "ui_maindialog.h"
#include<QIcon>
#include<QMessageBox>
#include<QDebug>

mainDialog::mainDialog(QWidget *parent)
    : CustomMoveDialog(parent),m_userid(0),currentState(0),page(1),pageNum(0)
    ,onlineNum(0)/*,offLineNum(0)*/
    , ui(new Ui::mainDialog)
{
    ui->setupUi(this);
    //初始化QVBoxLayout
    m_layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);  //控件上下左右的间距
    m_layout->setSpacing(3);        //设置每个控件彼此间的距离

    m_layout_chat=new QVBoxLayout;
    m_layout_chat->setContentsMargins(0,0,0,0);
    m_layout_chat->setSpacing(3);

    m_layout_space=new QVBoxLayout;
    m_layout_space->setContentsMargins(0,0,0,0);
    m_layout_space->setSpacing(3);


    //设置控件的层
    ui->wdg_3_list->setLayout(m_layout);
    ui->wdg_2_list->setLayout(m_layout_chat);
    ui->wdg_2_list_2->setLayout(m_layout_space);
}

mainDialog::~mainDialog()
{
    delete ui;
}

void mainDialog::slot_setInfo(int userid, int icon, QString name,
                              QString sex, QString username,int level)
{
    //用户名设置 我的->个人信息
    m_userid=userid;
    ui->lb_0_name->setText(name);
    ui->lb_0_level->setText(QString("LV%1").arg(level));
    //头像和性别都是设置图片
    ui->pb_0_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    QString sexx=sex=="男"?"boy":"girl";
    ui->pb_0_sex->setIcon(QIcon(QString(":/tupian/%1.png").arg(sexx)));
    ui->pb_0_icon->setIconSize(QSize(50,50));
    ui->pb_0_sex->setIconSize(QSize(20,20));

    onlineNum=0;
//    offLineNum=0;
}

void mainDialog::slot_addFriend(UserItem *userItem)
{
    //往层上添加控件
    if(userItem->m_state!=2){//不离线
        m_layout->insertWidget(0,userItem);
        onlineNum++;
    }else {
        m_layout->addWidget(userItem);
//        offLineNum++;
    }
}

void mainDialog::slot_removeFriend(UserItem *useritem)
{
    useritem->hide();
    m_layout->removeWidget(useritem);
}

void mainDialog::slot_onlineFriend(UserItem *useritem)
{
    m_layout->removeWidget(useritem);
    m_layout->insertWidget(onlineNum,useritem);
    onlineNum++;
}

void mainDialog::slot_offlineFriend(UserItem *useritem)
{
    m_layout->removeWidget(useritem);
    onlineNum--;
    m_layout->insertWidget(onlineNum,useritem);
}

void mainDialog::slot_addChat(ChatItem *chatItem)
{
    //layout->insertwidght(0,item);
//    m_layout_chat->insertWidget(0,chatItem);
    m_layout_chat->addWidget(chatItem);
}

void mainDialog::slot_removeChat(ChatItem *chatitem)
{
    chatitem->hide();
    m_layout_chat->removeWidget(chatitem);
}

void mainDialog::slot_upChat(ChatItem *chatitem)
{
    m_layout_chat->insertWidget(0,chatitem);
}

void mainDialog::slot_setNoreadSpace(bool noread)
{
    if(noread)ui->pb_2_noread->setStyleSheet("background-color: rgb(255, 0, 0);");
    else ui->pb_2_noread->setStyleSheet("background:transparent");
}

void mainDialog::slot_setNoreadFriend(bool noread)
{
    if(noread)ui->pb_3_noread->setStyleSheet("background-color: rgb(255, 0, 0);");
    else ui->pb_3_noread->setStyleSheet("background:transparent");
}

void mainDialog::slot_setSpace(STRU_SPACE_RS *rs)
{
    pageNum=rs->spaceNum/10+1;
    ui->lb_2_pageNum->setText(QString("%1/%2页").arg(page).arg(pageNum));
}

void mainDialog::slot_addSpace(spaceForm *spaceform)
{
//    m_layout_space->insertWidget(0,spaceform);
    m_layout_space->addWidget(spaceform);
    spaceLst.push_back(spaceform);
}

void mainDialog::slot_removeSpace(spaceForm *spaceform)
{
    spaceform->hide();
    m_layout_space->removeWidget(spaceform);
}

void mainDialog::slot_clearSpace()
{
    while(!spaceLst.empty()){
        slot_removeSpace(spaceLst.front());
        spaceForm* space=spaceLst.front();
        spaceLst.pop_front();
        delete space;
        space=nullptr;
    }
}


void mainDialog::on_pb_1_createRoom_clicked()
{
    //发送信号、弹出房间配置界面
    Q_EMIT SIG_createRoomButton();
}


void mainDialog::on_pb_min_clicked()
{
    this->showMinimized();
}


void mainDialog::on_pb_close_clicked()
{
    if( QMessageBox::question( this , "退出提示","确定退出游戏?" ) == QMessageBox::Yes )
    {
         //发送退出登录信号
        Q_EMIT SIG_QUIT();
    }
//    this->slot_close();
}


void mainDialog::on_pb_1_roomList_clicked()
{
    //发送信号，切换到房间列表界面
    Q_EMIT SIG_roomListButton(-1,-1,0);
}


void mainDialog::on_pb_5_quiLLogin_clicked()
{
    //切换到登录界面
    Q_EMIT SIG_QUITlogin();
}


void mainDialog::on_pb_5_setinfo_clicked()
{
    //个人信息
}


void mainDialog::on_pb_3_findUser_clicked()
{
    //获取输入框的内容
    QString user=ui->le_3_findUser->text();
    if(user==""){
        QMessageBox::about(this,"提示","输入为空");
        return;
    }
    //判断是用户名查找还是昵称查找
    //通过信号发送给核心处理类
    Q_EMIT SIG_FindUser(user,ui->cmb_3_findUser->currentIndex(),1);
    //核心类发送包给服务器
    //服务器查询后将结果返回给客户端
}


void mainDialog::on_tb_tabBarClicked(int index)
{
    page=1;
    //种类（1好友 2推荐 3我的），是否为搜索，按（用户，内容）搜索，昵称/内容，页数
    if(index==1){
        //当前是什么类型的动态1：搜索 2：好友 3：推荐 4：我的
        currentState=3;
        ui->pb_2_friend->setEnabled(true);
        ui->pb_2_suggest->setEnabled(false);
        ui->pb_2_mine->setEnabled(true);
        Q_EMIT SIG_getSpace(2,false,0,"",1);
    }
}


void mainDialog::on_pb_2_friend_clicked()
{
    page=1;
    //当前是什么类型的动态1：搜索 2：好友 3：推荐 4：我的
    currentState=2;
    ui->pb_2_friend->setEnabled(false);
    ui->pb_2_suggest->setEnabled(true);
    ui->pb_2_mine->setEnabled(true);
    Q_EMIT SIG_getSpace(1,false,0,"",1);
}


void mainDialog::on_pb_2_suggest_clicked()
{
    page=1;
    //当前是什么类型的动态1：搜索 2：好友 3：推荐 4：我的
    currentState=3;
    ui->pb_2_friend->setEnabled(true);
    ui->pb_2_suggest->setEnabled(false);
    ui->pb_2_mine->setEnabled(true);
    Q_EMIT SIG_getSpace(2,false,0,"",1);
}


void mainDialog::on_pb_2_mine_clicked()
{
    page=1;
    //当前是什么类型的动态1：搜索 2：好友 3：推荐 4：我的
    currentState=4;
    ui->pb_2_friend->setEnabled(true);
    ui->pb_2_suggest->setEnabled(true);
    ui->pb_2_mine->setEnabled(false);
    Q_EMIT SIG_getSpace(3,false,0,"",1);
}


void mainDialog::on_pb_2_findSpace_clicked()
{
    page=1;
    //当前是什么类型的动态1：搜索 2：好友 3：推荐 4：我的
    currentState=1;
    int kind=ui->cmb_2_findSpace->currentIndex();
    QString str=ui->le_2_findstr->text();
    ui->pb_2_friend->setEnabled(true);
    ui->pb_2_suggest->setEnabled(true);
    ui->pb_2_mine->setEnabled(true);
    Q_EMIT SIG_getSpace(0,true,kind,str,1);
}


void mainDialog::on_pb_2_nextPage_clicked()
{
    page++;
    if(page>pageNum){
        QMessageBox::about(this,"提示","已经是最后一页");
        page--;
        return;
    }
    int kind;
    QString str;
    switch(currentState){
    case 1://搜索
        kind=ui->cmb_2_findSpace->currentIndex();
        str=ui->le_2_findstr->text();
        Q_EMIT SIG_getSpace(0,true,kind,str,page);
        break;
    case 2://好友
        Q_EMIT SIG_getSpace(1,false,0,"",page);
        break;
    case 3://推荐
        Q_EMIT SIG_getSpace(2,false,0,"",page);
        break;
    case 4://我的
        Q_EMIT SIG_getSpace(3,false,0,"",page);
        break;
    }
}


void mainDialog::on_pb_2_lastPage_clicked()
{
    page--;
    if(page<=0){
        QMessageBox::about(this,"提示","已经是第一页");
        page++;
        return;
    }
    int kind;
    QString str;
    switch(currentState){
    case 1://搜索
        kind=ui->cmb_2_findSpace->currentIndex();
        str=ui->le_2_findstr->text();
        Q_EMIT SIG_getSpace(0,true,kind,str,page);
        break;
    case 2://好友
        Q_EMIT SIG_getSpace(1,false,0,"",page);
        break;
    case 3://推荐
        Q_EMIT SIG_getSpace(2,false,0,"",page);
        break;
    case 4://我的
        Q_EMIT SIG_getSpace(3,false,0,"",page);
        break;
    }
}


void mainDialog::on_pb_2_addSpace_clicked()
{
    //发布帖子
    Q_EMIT SIG_sendSpace();
}


void mainDialog::on_pb_2_refresh_clicked()
{
    page=1;
    int kind;
    QString str;
    switch(currentState){
    case 1://搜索
        kind=ui->cmb_2_findSpace->currentIndex();
        str=ui->le_2_findstr->text();
        Q_EMIT SIG_getSpace(0,true,kind,str,1);
        break;
    case 2://好友
        Q_EMIT SIG_getSpace(1,false,0,"",1);
        break;
    case 3://推荐
        Q_EMIT SIG_getSpace(2,false,0,"",1);
        break;
    case 4://我的
        Q_EMIT SIG_getSpace(3,false,0,"",1);
        break;
    }
}


//void mainDialog::on_pb_2_findUser_clicked()
//{
//    //获取输入框的内容
//    QString user=ui->le_33_findUser->text();
//    //通过信号发送给核心处理类
//    Q_EMIT SIG_FindFriend(user);
//}


void mainDialog::on_pushButton_clicked()
{
    //发送获取消息信号
    Q_EMIT SIG_getSpaceEmail();
    slot_setNoreadSpace(false);
}


void mainDialog::on_pb_3_friendRq_clicked()
{
    Q_EMIT SIG_getFriEmail();
    slot_setNoreadFriend(false);
}

