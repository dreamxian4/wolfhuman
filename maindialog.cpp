#include "maindialog.h"
#include "ui_maindialog.h"
#include<QIcon>
#include<QMessageBox>

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
    ui->lb_0_name->setText(name);
    ui->lb_0_level->setText(QString("LV%1").arg(level));
    //头像和性别都是设置图片
    ui->pb_0_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    QString sexx=sex=="男"?"boy":"girl";
    ui->pb_0_sex->setIcon(QIcon(QString(":/tupian/%1.png").arg(sexx)));
    ui->pb_0_icon->setIconSize(QSize(50,50));
    ui->pb_0_sex->setIconSize(QSize(20,20));
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

}

