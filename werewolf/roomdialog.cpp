#include "roomdialog.h"
#include "ui_roomdialog.h"
#include<QMessageBox>
#include<qDebug>



roomDialog::roomDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::roomDialog),m_seat(0),m_userid(0),
    m_mode(0),m_method(0),m_roomid(0),m_count(0),
    m_currentCou(0),m_pass(false),num(0),state(false),
    m_playing(false),m_day(0),m_pb_icon(USERINFO),m_d_kill(0),
    m_d_antidote(false),m_d_poison(false),m_d_protect(0),m_d_speak(0)
{

    //身份信息初始化
    BASEIDENTIFY={"预言家","女巫","平民","狼人","猎人","守卫"};

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

    //定时器
    m_timer_ready=new QTimer;
    connect(m_timer_ready,&QTimer::timeout,this,&roomDialog::slot_OverTimerReady);
    m_timer_skyBlk=new QTimer;
    connect(m_timer_skyBlk,&QTimer::timeout,this,&roomDialog::slot_OverTimerskyBlk);
    m_timer_tips=new QTimer;
    connect(m_timer_tips,&QTimer::timeout,this,&roomDialog::slot_OverTimerTips);
    m_timer_police=new QTimer;
    connect(m_timer_police,&QTimer::timeout,this,&roomDialog::slot_OverTimerPolice);
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
    ui->pb_operate->setEnabled(false);
    //加12个默认的
    for(int i=1;i<13;i++){
        //头像：小于人数的编号：+ 大于人数的编号：锁
        roomPlayerform* player=new roomPlayerform;
        connect(player,SIGNAL(SIG_click_icon(int)),
                this,SLOT(slot_click_icon(int)));
        player->setInfo(i,false);
        player->setIden(-1);
        player->setJing(-1);
        //锁
        if(i>num)player->setImage("00");
        m_mapIdToPlayer[i]=player;
        slot_addPlayer(player,i);
    }
    //设置TODO
}

void roomDialog::slot_destroyRoom(){
    m_currentCou=0;
    m_playing=false;
    if(state){
        state=false;
        m_timer_ready->stop();
        ui->lb_ready->setText("");
    }
    for(int i=1;i<13;i++){
        roomPlayerform* player= m_mapIdToPlayer[i];
        slot_removePlayer(player,i);
    }
    ui->lb_ready->setText("");
    ui->lb_operate->setText("");
}

void roomDialog::slot_ready()
{
    //倒计时，如果有玩家退出，停止计时
    m_timer_ready->start(1000);
    num=5;
    state=true;
    ui->lb_ready->setText(QString("游戏即将开始"));
}

void roomDialog::slot_setIden(int iden)
{
    m_user_iden=iden;
    if(iden==1){
        m_d_antidote=true;
        m_d_poison=true;
    }
    m_playing=true;
    m_mapIdToPlayer[m_seat]->setIden(iden);
    ui->lb_ready->setText(QString("游戏开始~~你的身份是：%1").arg(BASEIDENTIFY[iden]));
}

void roomDialog::slot_skyBlack()
{
    ui->tb_message->append("天黑啦！！！！");
    m_day++;
    ui->pb_day->setText(QString("第%1天").arg(m_day));
    ui->pb_day->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    ui->lb_operate->setStyleSheet("color: rgb(0, 255, 127);font: 8pt \"隶书\";");
    switch(m_user_iden){
    case 0://预言家
        ui->lb_operate->setText("请选择一位玩家，验证其身份");
        m_pb_icon=SKYBLK_YYJ;
        break;
    case 1://女巫
        ui->lb_operate->setText("等待操作");
        m_pb_icon=USERINFO;
        break;
    case 2://平民
        ui->lb_operate->setText("等待天亮");
        m_pb_icon=USERINFO;
        break;
    case 3://狼人
        ui->lb_operate->setText("请选择一位玩家，杀掉他");
        m_pb_icon=SKYBLK_LR;
        break;
    case 4://猎人
        ui->lb_operate->setText("等待天亮");
        m_pb_icon=USERINFO;
        break;
    case 5://守卫
        ui->lb_operate->setText("请选择一位玩家，守护他不被杀害");
        m_pb_icon=SKYBLK_SW;
        break;
    }
    blk=30;
    m_timer_skyBlk->start(1000);

}

void roomDialog::slot_yyj(int id, int iden)
{
    ui->lb_operate->setText(QString("您选择的玩家是%1号，他的身份是:%2").arg(id).arg(BASEIDENTIFY[iden]));
}

void roomDialog::slot_lr(int id, int toid)
{
    if(m_user_iden!=3)return;
    //TODO
    ui->tb_message->append(QString("杀人信息：%1号 选择杀掉 %2号").arg(id).arg(toid));
}

void roomDialog::slot_nw(int kill)
{
    //判断自己是不是女巫
    if(m_user_iden==1){
        //是
        //判断还有没有解药
        if(m_d_antidote){
            //有，弹出框显示杀人信息，询问是否救人
            if(kill==0)QMessageBox::about(this,"死亡信息","昨夜无人死亡");
            else if(QMessageBox::question(this,"死亡信息",QString("昨夜死亡的玩家是%1号，是否救他？").arg(kill))==QMessageBox::Yes){
                //救，发送救人信息
                Q_EMIT SIG_nvSilverWater();
                m_d_antidote=false;
                return;
            }
        }
        //不救，没有解药，没有杀人
        //判断有没有毒药
        if(m_d_poison){
            //询问是否毒人（将按钮状态设置成女巫），发送操作信息
            if(QMessageBox::question(this,"提示",QString("你有一瓶毒药，是否毒人？"))==QMessageBox::Yes){
                //毒TODO
                ui->lb_operate->setText("选择一位玩家，毒死他");
                m_pb_icon=SKYBLK_NW;
                return;
            }
        }
        ui->lb_operate->setText("等待天亮");
        m_pb_icon=USERINFO;
    }
    //不是忽略
}

void roomDialog::slot_skyWhite(int *die)
{
    //判断死的是不是自己
    QString show="昨晚死亡的玩家是：";
    for(int i=0;i<2;i++){
        //是，发送自己的身份
        if(die[i]==m_seat)Q_EMIT SIG_imDie(m_user_iden);
        //显示死亡信息
        if(die[i]!=0)show+=QString("%1号 ").arg(die[i]);
    }
    ui->tb_message->append("天亮了！！！！");
    ui->tb_message->append(show);
}

void roomDialog::slot_speak(STRU_SPEAK_RQ& rq)
{
    //判断发言人是不是自己
    switch(rq.state){
    case 3://白天正常发言
        break;
    case 1://上警玩家发言
        ui->pb_day->setText("竞选阶段");
        m_d_state=1;
        if(m_d_police)ui->pb_operate->setEnabled(true);
        if(rq.seat==m_seat){
            //判断自己有没有上警
            if(m_d_police){
                //如果上警：发言
                //如果已经发过言了，发言阶段结束
                if(m_d_speak==1){
                    Q_EMIT SIG_SpeakStateEnd(1);
                    m_d_speak=0;
                }
                ui->lb_operate->setText("开始发言");
                m_d_speak=1;
            }else{
                //没上警：发送发言结束包，下一个人是后面的人
                Q_EMIT SIG_SpeakEnd(m_seat,1,rq.state);
            }
        }
        break;
    case 2://没有警长，依次发言
        break;
    }
}

void roomDialog::slot_police()
{
    police=10;
    m_timer_police->start(1000);
    //询问是否竞选警长
    if(QMessageBox::question(this,"提示","是否竞选警长")==QMessageBox::Yes){
        //是：回复竞选
        m_d_police=true;
        Q_EMIT SIG_police(m_seat,true);
    }else m_d_police=false;
}

void roomDialog::slot_bePolice()
{
    //判断自己有没有竞选
    if(m_d_police){
        //竞选了，自己是警长，发送警长回复包
        Q_EMIT SIG_imPolice(m_seat);
    }
    //没竞选，自己不是警长
}

void roomDialog::slot_setPolicePlayer(STRU_TOBEPOLICE_RS& rs)
{
    //将对应位置的玩家警徽图标设为上警1:上警 2:放手 3:警长
    if(rs.raise) m_mapIdToPlayer[rs.seat]->setJing(1);
    else m_mapIdToPlayer[rs.seat]->setJing(2);
}

void roomDialog::slot_setPolice(STRU_BEPOLICE_RS &rs)
{
    //将对应位置的玩家警徽图标设为上警1:上警 2:放手 3:警长
    m_mapIdToPlayer[rs.seat]->setJing(3);
}

void roomDialog::slot_beginVote(STRU_SPEAKSTATE_END &end)
{
    //判断是哪个阶段的投票
    //根据阶段查找可以被投票的人
    switch(end.state){
    case 1://竞选阶段发言结束，开始投票
        ui->lb_operate->setText("开始投票,选出警长");
        if(m_mapIdToPlayer[m_seat]->getJing()==""){
            m_pb_icon=VOTE_POLICE;
            for(int i=1;i<=m_count;i++){
                if(m_mapIdToPlayer[i]->getJing()=="上"){
                    m_mapIdToPlayer[i]->setAbleToVoted(true);
                }else m_mapIdToPlayer[i]->setAbleToVoted(false);
            }
        }
        break;
    case 2:

        break;
    }
}


void roomDialog::slot_setPlayer(int id, int icon, int level, QString sex, QString name, int userid)
{
    //新成员加入
    if(userid>0)m_currentCou++;
    else {//有人退出
        m_currentCou--;
        if(state){//如果正在准备开始游戏，结束计时
            state=false;
            m_timer_ready->stop();
            ui->lb_ready->setText("");
        }
    }
    //设置成员信息
    if(userid==m_userid){//判断该信息是不是自己
        m_seat=id;
        m_mapIdToPlayer[id]->setInfo(id,true);
    }
    m_mapIdToPlayer[id]->setImage(QString("%1").arg(icon));
    m_mapIdToPlayer[id]->setZiLiao(level,sex,name,userid,m_count);
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
    if(m_playing&&QMessageBox::question( this , "退出提示","游戏已开始，是否确定退出?" ) != QMessageBox::Yes)return;
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
            Q_EMIT SIG_ReadybeginGame();
        }else{
            //没满员，提示人数不足，不能开始游戏
            ui->lb_tip->setText("人数不足，不能开始游戏");
            ui->lb_tip->setStyleSheet("background-color: rgb(129, 129, 129);color: rgb(255, 255, 255);");
            //定时回复
            m_timer_tips->start(1500);
        }
    }else{
        //不是房主，提示只有房主可以开始游戏
        ui->lb_tip->setText("只有房主可以开始游戏");
        ui->lb_tip->setStyleSheet("background-color: rgb(129, 129, 129);color: rgb(255, 255, 255);");
        m_timer_tips->start(1500);
    }
}

//玩家头像被点击
void roomDialog::slot_click_icon(int id)
{
    //1预言家 2狼人  3女巫毒 4守卫 0无操作
    //判断当前是什么状态
    switch(m_pb_icon){
    case USERINFO:
        //显示玩家资料TODO
        QMessageBox::about(this,"资料","xxxxx");
        break;
    case SKYBLK_YYJ:
        //直接发送选择，TODO:等待计时结束，将最后一个选择的发送出去
        Q_EMIT SIG_skyBlkRs(m_user_iden,m_seat,1,id);
        //操作完，将状态恢复成默认
        m_pb_icon=USERINFO;
        break;
    case SKYBLK_LR:
        //直接发送杀人信息
        Q_EMIT SIG_skyBlkRs(m_user_iden,m_seat,2,id);
        m_pb_icon=USERINFO;
        break;
    case SKYBLK_NW:
        //在收到杀人信息时进行的操作：弹出框显示杀人信息，询问是否救人，如果救，发送救人信息，如果不救，询问是否毒人（将按钮状态设置成女巫），发送操作信息
        //毒人
        Q_EMIT SIG_skyBlkRs(m_user_iden,m_seat,3,id);
        ui->lb_operate->setText("等待天亮");
        m_d_poison=false;
        m_pb_icon=USERINFO;
        break;
    case SKYBLK_SW:
        //发送守人信息，TODO:等待计时结束，将最后一个选择的发送出去
        if(m_d_protect==id)QMessageBox::about(this,"提示","不能连续两晚守护同一个人");
        else{
            Q_EMIT SIG_skyBlkRs(m_user_iden,m_seat,4,id);
            m_d_protect=id;
            m_pb_icon=USERINFO;
        }
        break;
    case VOTE_POLICE://投票竞选警长
        //发送投票信息
        Q_EMIT SIG_votePolice(m_seat,id,1);
        m_pb_icon=USERINFO;
        break;
    }
}

void roomDialog::slot_OverTimerReady()
{
    //5秒倒计时
    num--;
    ui->lb_time->setText(QString("%1").arg(num));
    ui->lb_ready->setText(QString("游戏即将开始"));
    if(num==0){
        //倒计时结束，判断自己是不是房主，如果是房主，给服务端发送正式开始包
        m_timer_ready->stop();
        state=false;
        ui->lb_ready->setText("");
        ui->lb_time->setText("");
        if(m_seat==1)Q_EMIT SIG_beginGame();
    }
}

void roomDialog::slot_OverTimerTips()
{
    //开始按钮提醒恢复
    ui->lb_tip->setText("");
    ui->lb_tip->setStyleSheet("background:transparent");
    m_timer_tips->stop();
}

void roomDialog::slot_OverTimerskyBlk()
{
    ui->lb_time->setText(QString("%1").arg(blk));
    blk--;
    //判断是不是房主
    if(m_seat==1&&blk==15){
        //是房主发送结束包
        Q_EMIT SIG_skyBlk15(true);
    }else if(m_seat==1&&blk==0){
        Q_EMIT SIG_skyBlk15(false);
        m_timer_skyBlk->stop();
    }else if(blk==0) m_timer_skyBlk->stop();
}

void roomDialog::slot_OverTimerPolice()
{
    ui->lb_time->setText(QString("%1").arg(police));
    police--;
    //判断是不是房主
    if(m_seat==1&&police==0){
        //是房主发送竞选阶段结束包
        Q_EMIT SIG_PoliceEnd();
        m_timer_police->stop();
        ui->lb_time->setText("");

    }else if(police==0){
        m_timer_police->stop();
        ui->lb_time->setText("");
    }
    //不是忽略
}


void roomDialog::on_pb_0_end_clicked()
{
    //结束发言
    Q_EMIT SIG_SpeakEnd(m_seat,1,m_d_state);
}


void roomDialog::on_pb_operate_clicked()
{
    //放手
    m_d_police=false;
    Q_EMIT SIG_police(m_seat,false);
}

