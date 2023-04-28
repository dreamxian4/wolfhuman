#include "createroomform.h"
#include "ui_createroomform.h"
#include<qDebug>
#include"packdef.h"

createRoomForm::createRoomForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createRoomForm)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    ui->le_passwd->setReadOnly(true);
    ui->le_passwd->setPlaceholderText("xxxxxxx");
    ui->rb_audio->setChecked(true);
    ui->sb_playerNum->setMinimum(6);
    ui->sb_playerNum->setMaximum(12);
}

createRoomForm::~createRoomForm()
{
    delete ui;
}

void createRoomForm::on_cb_passwd_clicked()
{
    if(ui->cb_passwd->isChecked()){
        ui->le_passwd->setReadOnly(false);
        ui->le_passwd->setPlaceholderText("xxx");
    }else{
        ui->le_passwd->setReadOnly(true);
    }
}


void createRoomForm::on_pb_cancel_clicked()
{
    //发送信号，进行页面切换
    Q_EMIT SIG_CANCEL();
}


void createRoomForm::on_pb_commit_clicked()
{
    //将房间信息以信号的形式发送出去
    //下拉框的索引从0开始
    int mode=ui->rb_audio->isChecked()?_GAME_MODE_AUDIO:_GAME_MODE_VIDEO;
    int method=ui->cmb_playMethod->currentIndex();
    int playerNum=ui->sb_playerNum->value();
    int level=ui->cmb_level->currentIndex();
    bool pass=ui->cb_passwd->isChecked();
    QString password="";
    if(pass)password=ui->le_passwd->text();
    Q_EMIT SIG_createRoom(mode,method,playerNum,level,pass,password);
}

