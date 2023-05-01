#include "roomdialog.h"
#include "ui_roomdialog.h"
#include<QMessageBox>


roomDialog::roomDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::roomDialog)
{
    ui->setupUi(this);
    m_playerLayoutLeft=new QVBoxLayout;
    m_playerLayoutRight=new QVBoxLayout;
    ui->wdg_left->setLayout(m_playerLayoutLeft);
    ui->wdg_right->setLayout(m_playerLayoutRight);
    m_playerLayoutLeft->setContentsMargins(0,0,0,0);
    m_playerLayoutLeft->setSpacing(5);
    m_playerLayoutRight->setContentsMargins(0,0,0,0);
    m_playerLayoutRight->setSpacing(5);
    //通过addwidget添加控件
    //m_userLayout->addWidget();

}

void roomDialog::slot_addPlayer(QWidget *player,int id)
{
    if(id<=6)m_playerLayoutLeft->addWidget(player);
    else m_playerLayoutRight->addWidget(player);
}

void roomDialog::slot_removePlayer(QWidget *player,int id)
{
    player->hide();
    if(id<=6)m_playerLayoutLeft->removeWidget(player);
    else m_playerLayoutRight->removeWidget(player);
}

void roomDialog::slot_setInfo(int roomid, int mode, int method,
                             bool lock, QString password,int num)
{
    ui->lb_roomNum->setText(QString("%1").arg(roomid));
    ui->lb_roomType->setText(QString("%1人基础板").arg(num));
    ui->pb_day->setText("准备阶段");
    //加12个默认的
    for(int i=1;i<13;i++){
        //头像：小于人数的编号：+ 大于人数的编号：锁
        roomPlayerform* player=new roomPlayerform;
        player->setInfo(i,0,0);
        //锁
        if(i>num)player->setImage(1);
        m_mapIdToPlayer[i]=player;
        slot_addPlayer(player,i);
    }
    //设置TODO
}

void roomDialog::slot_destroyRoom(){
    for(int i=1;i<13;i++){
        roomPlayerform* player= m_mapIdToPlayer[i];
        slot_removePlayer(player,i);
    }
}

void roomDialog::slot_setPlayer(int id, int icon, int level, QString sex, QString name, int userid)
{
    //设置成员信息
    m_mapIdToPlayer[id]->setImage(icon);
    m_mapIdToPlayer[id]->setZiLiao(level,sex,name,userid);
}

roomDialog::~roomDialog()
{
    delete ui;
}

void roomDialog::on_pb_min_clicked()
{
    this->showMinimized();
}


void roomDialog::on_pb_close_clicked()
{
    if( QMessageBox::question( this , "退出提示","确定退出游戏?" ) == QMessageBox::Yes )
    {
        //发送退出房间信号
        on_pb_quitroom_clicked();
         //发送退出登录信号
        Q_EMIT SIG_QUIT();
    }
}


void roomDialog::on_pb_quitroom_clicked()
{
    //发送退出房间信号，如果是房主，就退出到主界面，如果不是，退出到房间列表
    Q_EMIT SIG_quitRoom(m_mapIdToPlayer[1]->getUserid());
}

