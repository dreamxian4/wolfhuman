#include "roomdialog.h"
#include "ui_roomdialog.h"
#include<QMessageBox>
#include<qDebug>


roomDialog::roomDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::roomDialog),m_seat(0),m_userid(0),
    m_mode(0),m_method(0),m_roomid(0),m_count(0),
    m_currentCou(0),m_pass(false),num(0),state(false)
{
    m_passwd="";
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
                             bool lock, QString password,int num,int userid)
{
    m_roomid=roomid;
    m_mode=mode;
    m_method=method;
    m_pass=lock;
    m_passwd=password;
    m_count=num;
    m_userid=userid;
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
    m_currentCou=0;
    for(int i=1;i<13;i++){
        roomPlayerform* player= m_mapIdToPlayer[i];
        slot_removePlayer(player,i);
    }
}

void roomDialog::slot_ready()
{
    //倒计时，如果有玩家退出，停止计时
    m_timer_ready=startTimer(1000);
    num=5;
    state=true;
    ui->lb_ready->setText(QString("%1秒后开始游戏").arg(num));
}

void roomDialog::slot_setPlayer(int id, int icon, int level, QString sex, QString name, int userid)
{
    if(userid>0)m_currentCou++;
    else {
        m_currentCou--;
        if(state){
            state=false;
            killTimer(m_timer_ready);
            ui->lb_ready->setText("");
        }
    }
    //设置成员信息
    if(userid==m_userid)m_seat=id;
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
    if(state&&m_seat==1)return;
    //发送退出房间信号，如果是房主，就退出到主界面，如果不是，退出到房间列表
    Q_EMIT SIG_quitRoom(m_mapIdToPlayer[1]->getUserid());
}


void roomDialog::on_pb_0_begin_clicked()
{
    if(state)return;
    //判断自己是不是房主
    if(m_seat==1){
        //是房主，判断房间内有没有满员
        if(1||m_currentCou==m_count){//测试
            //满员了，发送开始游戏包
            Q_EMIT SIG_beginGame();
        }else{
            //没满员，提示人数不足，不能开始游戏
            ui->lb_tip->setText("人数不足，不能开始游戏");
            ui->lb_tip->setStyleSheet("background-color: rgb(129, 129, 129);color: rgb(255, 255, 255);");
            //定时回复
            m_timer_tips=startTimer(1500);
        }
    }else{
        //不是房主，提示只有房主可以开始游戏
        ui->lb_tip->setText("只有房主可以开始游戏");
        ui->lb_tip->setStyleSheet("background-color: rgb(129, 129, 129);color: rgb(255, 255, 255);");
        m_timer_tips=startTimer(1500);
    }
}


void roomDialog::timerEvent(QTimerEvent *e)
{
    if(e->timerId()==m_timer_tips){
        //开始按钮提醒恢复
        ui->lb_tip->setText("");
        ui->lb_tip->setStyleSheet("background:transparent");
        killTimer(m_timer_tips);
    }else if(e->timerId()==m_timer_ready){
        //5秒倒计时
        num--;
        ui->lb_ready->setText(QString("%1秒后开始游戏").arg(num));
        killTimer(m_timer_ready);
        if(num>0)m_timer_ready=startTimer(1000);
        else{
            //倒计时结束，判断自己是不是房主，如果是房主，给服务端发送正式开始包
            state=false;
            ui->lb_ready->setText("");
        }
    }
}

