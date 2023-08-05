#include "ziliaodialog.h"
#include "ui_ziliaodialog.h"
#include<QIcon>
#include<QSize>
#include<QDebug>
#include<QMessageBox>

ZiLiaoDialog::ZiLiaoDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::ZiLiaoDialog),friendId(0),roomid(0)
{
    ui->setupUi(this);
}

ZiLiaoDialog::~ZiLiaoDialog()
{
    delete ui;
}

void ZiLiaoDialog::slot_setInfo(STRU_FRIEND_ZILIAO_RS &rs)
{
    friendId=rs.friendid;

    //头像
    ui->pb_icon->setIconSize(QSize(60,60));
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(rs.icon)));
    //昵称
    ui->lb_name->setText(QString::fromStdString(rs.name));

    //状态
    switch(rs.state){
    case 1://在线
        ui->lb_state->setStyleSheet("color: rgb(85, 255, 0);");
        ui->lb_state->setText("在线");
        roomid=0;
        break;
    case 2://离线
        ui->lb_state->setStyleSheet("color: rgb(117, 117, 117);");
        ui->lb_state->setText("离线");
        roomid=0;
        break;
    case 3://游戏中
        ui->lb_state->setStyleSheet("color: rgb(255, 85, 0);");
        ui->lb_state->setText("游戏中");
        roomid=rs.roomid;
        break;
    case 4://房间中
        ui->lb_state->setStyleSheet("color: rgb(85, 255, 0);");
        ui->lb_state->setText(QString("房间号：%1").arg(rs.roomid));
        roomid=rs.roomid;
        break;
    }

    //用户名
    ui->tb_username->setText(QString::fromStdString(rs.username));
    //性别
    ui->tb_sex->setText(QString::fromStdString(rs.sex));
    //等级
    ui->tb_level->setText(QString("level.%1").arg(rs.level));
    //游戏场数
    ui->tb_gameNum->setText(QString("%1场").arg(rs.gameNum));
}

void ZiLiaoDialog::on_pb_min_clicked()
{
    this->showMinimized();
}


void ZiLiaoDialog::on_pb_close_clicked()
{
    this->close();
}


void ZiLiaoDialog::on_pb_sendMess_clicked()
{
    Q_EMIT SIG_sendMsg(friendId);
}


void ZiLiaoDialog::on_pb_followRoom_clicked()
{
    //发送加入房间
    if(roomid)Q_EMIT SIG_joinRoom(roomid);
    else QMessageBox::about(this,"提示","该用户不在房间中");
}


void ZiLiaoDialog::on_pb_space_clicked()
{

}

