#include "maindialog.h"
#include "ui_maindialog.h"
#include<QIcon>
#include<QMessageBox>
#include<QDebug>

mainDialog::mainDialog(QWidget *parent)
    : CustomMoveDialog(parent)
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

    slot_addSpace(new spqceForm);
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
}

void mainDialog::slot_addFriend(UserItem *userItem)
{
    //往层上添加控件
    m_layout->addWidget(userItem);
}

void mainDialog::slot_addChat(ChatItem *chatItem)
{
    //layout->insertwidght(0,item);
//    m_layout_chat->insertWidget(0,chatItem);
    m_layout_chat->addWidget(chatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
    m_layout_chat->addWidget(new ChatItem);
}

void mainDialog::slot_addSpace(spqceForm *spaceform)
{
    m_layout_space->addWidget(spaceform);
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
    //判断是用户名查找还是昵称查找
    //通过信号发送给核心处理类
    Q_EMIT SIG_FindUser(user,ui->cmb_3_findUser->currentIndex());
    //核心类发送包给服务器
    //服务器查询后将结果返回给客户端
}

