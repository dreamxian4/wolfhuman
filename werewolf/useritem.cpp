#include "useritem.h"
#include "ui_useritem.h"
#include<QIcon>
#include<QBitmap>
#include<QDebug>
#include<QCursor>
#include<QPoint>

UserItem::UserItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserItem)
{
    ui->setupUi(this);
}

UserItem::~UserItem()
{
    delete ui;
}

//设置用户信息
void UserItem::slot_setInfo(int userid, QString name, int state, int iconid, QString sex,int level,QString username)
{
    //1.保存用户信息
    m_id=userid;
    m_name=name;
    m_state=state;
    m_iconid=iconid;
    m_sex=sex;
    m_level=level;
    m_username=username;

    //2.设置个人信息
    ui->lb_name->setText(m_name);
    ui->lb_level->setText(QString("Level.%1").arg(m_level));
    ui->lb_sex->setText(m_sex);
    QString str_state="";
    switch(m_state){//1：在线 2：离线 3：游戏中 4：房间中
    case 1:
        str_state="在线";
        break;
    case 2:
        str_state="离线";
        break;
    case 3:
        str_state="游戏中";
        break;
    case 4:
        str_state="房间中";
        break;
    default:
        str_state="行为异常";
        break;
    }
    ui->lb_state->setText(str_state);

    //3.设置头像
    QString iconPath=QString(":/tx/%1.png").arg(iconid);

    //4.判断用户状态
    if(m_state==1){//在线,彩色图标
        ui->pb_icon->setIcon(QIcon(iconPath));
    }else{//不在线，灰色图标
        QBitmap bmp;
        bmp.load(iconPath);
        ui->pb_icon->setIcon(bmp);
    }

    //5.重绘
    this->repaint();
}

//设置用户下线
void UserItem::slot_setOffline()
{
    //1.设置用户状态为下线
    m_state=0;

    //2.设置头像为灰色
    QString iconPath=QString(":/tx/%1.png").arg(m_iconid);
    QBitmap bmp;
    bmp.load(iconPath);
    ui->pb_icon->setIcon(bmp);

    //3.重绘
    this->repaint();
}

void UserItem::on_pb_icon_clicked()
{
    //发送信号，通知kernel类，发送用户id
    Q_EMIT SIG_UserItemClicked(m_id);
}


