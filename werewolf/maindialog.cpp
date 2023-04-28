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
                              QString sex, QString username)
{
    //用户名设置 我的->个人信息
    m_userid=userid;
    ui->lb_name->setText(name);
    //头像和性别都是设置图片
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    ui->pb_sex->setIcon(QIcon(QString(":/tupian/%1.png").arg(sex)));
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

