#include "maindialog.h"
#include "ui_maindialog.h"
#include<QIcon>

mainDialog::mainDialog(QWidget *parent)
    : CustomMoveDialog(parent)
    , ui(new Ui::mainDialog)
{
    ui->setupUi(this);
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
    ui->lb_name->setText(name);
    ui->lb_level->setText(QString("等级：LV%1").arg(level));
    //头像和性别都是设置图片
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    QString sexx=sex=="男"?"boy":"girl";
    ui->pb_sex->setIcon(QIcon(QString(":/tupian/%1.png").arg(sexx)));
    ui->pb_icon->setIconSize(QSize(50,50));
    ui->pb_sex->setIconSize(QSize(20,20));
}


void mainDialog::on_pb_createRoom_clicked()
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
    this->slot_close();
}


void mainDialog::on_pb_roomList_clicked()
{
    //发送信号，切换到房间列表界面
    Q_EMIT SIG_roomListButton(-1,-1,0);
}

