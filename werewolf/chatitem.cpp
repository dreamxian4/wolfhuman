#include "chatitem.h"
#include "ui_chatitem.h"
#include<QBitmap>
#include<QTime>

ChatItem::ChatItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatItem),userid(0)
{
    ui->setupUi(this);
}

ChatItem::~ChatItem()
{
    delete ui;
}

void ChatItem::slot_setChatMsg(QString content,bool isme)
{
    ui->lb_content->setText(QString("[%1]%2")
                            .arg(QTime::currentTime().toString("hh:mm")).arg(content));
    if(isme)ui->lb_content->setStyleSheet("color: rgb(63, 188, 0);");
    else ui->lb_content->setStyleSheet("color: rgb(0, 0, 0);");
}

void ChatItem::setNoRead(bool no){
    if(no) ui->lb_noread->setStyleSheet("background-color: rgb(255, 63, 46);");
    else ui->lb_noread->setStyleSheet("background:transparent");
}

void ChatItem::slot_deleteMsg()
{
    ui->lb_content->setText("");
}

void ChatItem::slot_setInfo(QString name, int state, int icon, QString sex,int id)
{
    userid=id;
    ui->lb_name->setText(name);
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    QString sexx=sex=="男"?"boy":"girl";
    ui->pb_sex->setIcon(QIcon(QString(":/tupian/%1.png").arg(sexx)));
//    ui->pb_sex->setIconSize(QSize(50,50));
    ui->pb_sex->setIconSize(QSize(20,20));

    QString str_state="";
    switch(state){//1：在线 2：离线 3：游戏中 4：房间中
    case 1:
        str_state="在线";
        break;
    case 2:
        str_state="离线";
        break;
    case 3:
        str_state="游戏";
        break;
    case 4:
        str_state="房间";
        break;
    default:
        str_state="异常";
        break;
    }
    ui->lb_state->setText(str_state);

    //3.设置头像
    QString iconPath=QString(":/tx/%1.png").arg(icon);

    //4.判断用户状态
    if(state!=2){//在线,彩色图标
        ui->pb_icon->setIcon(QIcon(iconPath));
    }else{//不在线，灰色图标
        QBitmap bmp;
        bmp.load(iconPath);
        ui->pb_icon->setIcon(bmp);
    }

    //5.重绘
    this->repaint();
}

QString ChatItem::getname()
{
    return ui->lb_name->text();
}

void ChatItem::on_pb_chat_clicked()
{
    setNoRead(false);
    Q_EMIT SIG_chatItemSend(userid);
}

