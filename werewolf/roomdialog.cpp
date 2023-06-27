#include "roomdialog.h"
#include "ui_roomdialog.h"
#include<QMessageBox>
#include<qDebug>



roomDialog::roomDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::roomDialog),m_seat(0),m_userid(0),
    m_mode(0),m_method(0),m_roomid(0),m_count(0),
    m_currentCou(0),m_pass(false),num(0),state(false),
    m_playing(false),m_day(0),m_user_iden(-1),m_pb_icon(USERINFO),m_d_kill(0),
    m_d_antidote(false),m_d_poison(false),m_d_protect(0),m_d_speak(0),
    m_d_vote(0),Text_upPolice("上警玩家:"),m_d_bePolice(false),m_d_state(0),
    m_d_nextSpeak(0),m_pb_oper(0),m_d_alive(true),m_pb_begin(0),speakOrder(0),
    m_d_policer(0),speak(0),m_speak(0)
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
    m_timer_vote= new QTimer;
    connect(m_timer_vote,&QTimer::timeout,this,&roomDialog::slot_OverTimerVote);
    m_timer_speakOrder=new QTimer;
    connect(m_timer_speakOrder,&QTimer::timeout,this,&roomDialog::slot_OverTimerSpeakOrder);
    m_timer_speak=new QTimer;
    connect(m_timer_speak,&QTimer::timeout,this,&roomDialog::slot_OverTimerSpeak);
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
    //设置房间信息
    m_roomid=roomid;
    m_mode=mode;
    m_method=method;
    m_pass=lock;
    m_passwd=password;
    m_count=num;
    m_userid=userid;
    m_seat=0;
    m_currentCou=0;

    //房间类数据初始化
    state=false;
    m_playing=false;
    m_day=0;
    m_user_iden=-1;
    m_pb_icon=USERINFO;
    m_d_kill=0;
    m_d_antidote=false;
    m_d_poison=false;
    m_d_protect=0;
    m_d_vote=0;
    Text_upPolice="上警玩家:";
    m_d_bePolice=false;
    m_d_state=0;
    m_d_nextSpeak=0;
    m_pb_oper=0;
    m_d_alive=true;
    m_d_speak=0;
    m_pb_begin=0;
    m_d_policer=0;
    m_speak=0;

    //倒计时初始化
    m_timer_police->stop();
    m_timer_ready->stop();
    m_timer_skyBlk->stop();
    m_timer_tips->stop();
    m_timer_vote->stop();
    m_timer_speakOrder->stop();

    //房间内组件初始化
    ui->lb_roomNum->setText(QString("%1").arg(roomid));
    ui->lb_roomType->setText(QString("%1人基础板").arg(num));
    ui->pb_day->setText("准备阶段");
    ui->pb_operate->setEnabled(false);
    ui->pb_order->setEnabled(false);
    ui->pb_deorder->setEnabled(false);
    ui->pb_0_end->setEnabled(false);
    ui->tb_message->setText("");
    ui->lb_time->setText("");
    ui->pb_0_begin->setText("开始游戏");
    ui->pb_0_begin->setEnabled(true);
    ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
    ui->pb_operate_up->setEnabled(false);

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

    Q_EMIT SIG_Audio(false,false,false);
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
//    ui->lb_operate->setText("");
    ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
    ui->pb_operate_up->setIconSize(QSize(280,40));
}

void roomDialog::slot_ready()
{
    //倒计时，如果有玩家退出，停止计时
    num=5;
    m_timer_ready->start(1000);
    state=true;
    //    ui->lb_ready->setText(QString("游戏即将开始"));
    ui->tb_message->append((QString("游戏即将开始")));
    ui->pb_0_begin->setText("");
    ui->pb_0_begin->setEnabled(false);
}

void roomDialog::slot_setIden(int iden)
{
    ui->pb_0_begin->setText("点击发言");
    m_user_iden=iden;
    if(iden==1){//女巫的解药和毒药
        m_d_antidote=true;
        m_d_poison=true;
    }
    m_playing=true;
    m_mapIdToPlayer[m_seat]->setIden(iden);
    ui->lb_ready->setText(QString("游戏开始~~你的身份是：%1").arg(BASEIDENTIFY[iden]));
    ui->tb_message->append(QString("游戏开始~~你的身份是：%1").arg(BASEIDENTIFY[iden]));
}

int roomDialog::slot_getIden()
{
    return m_user_iden;
}

void roomDialog::slot_skyBlack()
{
    ui->tb_message->append("天黑啦！！！！");
    m_day++;
    ui->pb_day->setText(QString("第%1天").arg(m_day));
    ui->pb_day->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
//    ui->lb_operate->setStyleSheet("color: rgb(0, 255, 127);font: 8pt \"隶书\";");
    if(m_d_alive){
        switch(m_user_iden){//根据身份做不同的操作
        case 0://预言家
            //        ui->lb_operate->setText("请选择一位玩家，验证其身份");
//            ui->tb_message->append("请选择一位玩家，验证其身份");
            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/yyjSkyBlack.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            for(int i=1;i<=m_count;i++){
                if(m_mapIdToPlayer[i]->m_id!=m_seat&&m_mapIdToPlayer[i]->alive
                        &&!m_mapIdToPlayer[i]->beKnown){//不是自己，没被验过，还活着
                    m_mapIdToPlayer[i]->setAbleToVoted(true);
                }else m_mapIdToPlayer[i]->setAbleToVoted(false);
            }
            m_pb_icon=SKYBLK_YYJ;
            break;
        case 1://女巫
            //        ui->lb_operate->setText("等待操作");
//            ui->tb_message->append("稍作等待，完成操作");

            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/nv0SkyBlack.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            m_pb_icon=USERINFO;
            break;
        case 2://平民
            //        ui->lb_operate->setText("等待天亮");
//            ui->tb_message->append("等待天亮");
            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/waitDayLight.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            m_pb_icon=USERINFO;
            break;
        case 3://狼人
            //        ui->lb_operate->setText("请选择一位玩家，杀掉他");
//            ui->tb_message->append("请选择一位玩家，杀掉他");
            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/lrSkyBlack.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            for(int i=1;i<=m_count;i++){
                if(m_mapIdToPlayer[i]->alive){//还活着
                    m_mapIdToPlayer[i]->setAbleToVoted(true);
                }else m_mapIdToPlayer[i]->setAbleToVoted(false);
            }
            m_pb_icon=SKYBLK_LR;
            //可以和队友沟通
            m_pb_begin=WOLF_SPEAK;
            ui->pb_0_begin->setEnabled(true);
            ui->pb_0_begin->setText("点击发言");
            break;
        case 4://猎人
            //        ui->lb_operate->setText("等待天亮");
//            ui->tb_message->append("等待天亮");
            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/waitDayLight.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            m_pb_icon=USERINFO;
            break;
        case 5://守卫TODO
            //        ui->lb_operate->setText("请选择一位玩家，守护他不被杀害");
            ui->tb_message->append("请选择一位玩家，守护其不被杀害");
            m_pb_icon=SKYBLK_SW;
            break;
        }
    }
    blk=30;
    m_timer_skyBlk->start(1000);
}

void roomDialog::slot_yyj(int id, int iden)
{
    //    ui->lb_operate->setText(QString("您选择的玩家是%1号，他的身份是:%2").arg(id).arg(BASEIDENTIFY[iden]));
    if(m_d_alive){
        QString idenstr="";
        if(iden!=3)idenstr="好人";
        else idenstr="坏人";
        ui->tb_message->append(QString("您选择的玩家是%1号，他的身份是:%2").arg(id).arg(idenstr));
        m_mapIdToPlayer[id]->setYYJIden(iden);
        m_mapIdToPlayer[id]->beKnown=true;
    }
}

void roomDialog::slot_lr(int id, int toid)
{
    if(m_d_alive){
        if(m_user_iden!=3)return;
        ui->tb_message->append(QString("杀人信息：%1号 选择杀掉 %2号").arg(id).arg(toid));
    }
}

void roomDialog::slot_nw(int kill)
{
    if(m_d_alive){
        //判断自己是不是女巫
        if(m_user_iden==1){
            //是
            //判断还有没有解药
            if(m_d_antidote){
                //有，弹出框显示杀人信息，询问是否救人（不能是弹出框，弹出框是阻塞执行且不能按时退出）
                if(kill==0){
                    //QMessageBox::about(this,"死亡信息","昨夜无人死亡");
                    slot_operate_button(NW_NODIE,0);
                }
    //            else if(QMessageBox::question(this,"死亡信息",QString("昨夜死亡的玩家是%1号，是否救他？").arg(kill))==QMessageBox::Yes){
    //                //救，发送救人信息
    //                Q_EMIT SIG_nvSilverWater();
    //                m_d_antidote=false;
    //                return;
    //            }
                else slot_operate_button(NW_RESCUE,kill);
            }
            //不救，没有解药，没有杀人
            //判断有没有毒药
    //        if(m_d_poison){
    //            //询问是否毒人（将按钮状态设置成女巫），发送操作信息
    //            if(QMessageBox::question(this,"提示",QString("你有一瓶毒药，是否毒人？"))==QMessageBox::Yes){
    //                //毒
    //                //ui->lb_operate->setText("选择一位玩家，毒死他");
    //                ui->tb_message->append("选择一位玩家，毒死他");
    //                for(int i=1;i<=m_count;i++){
    //                    if(&m_mapIdToPlayer[i]->alive){//活着
    //                        m_mapIdToPlayer[i]->setAbleToVoted(true);
    //                    }else m_mapIdToPlayer[i]->setAbleToVoted(false);
    //                }
    //                m_pb_icon=SKYBLK_NW;
    //                return;
    //            }
    //            slot_operate_button(1,0);
    //        }
    //        //        ui->lb_operate->setText("等待天亮");
    //        ui->tb_message->append("等待天亮");
    //        m_pb_icon=USERINFO;
    //
        }
        //不是忽略
    }
}

void roomDialog::slot_skyWhite(int *die)
{
    for(int i=1;i<=m_count;i++){
        if(m_mapIdToPlayer[i]->m_id!=m_seat){
            m_mapIdToPlayer[i]->resumeVoted();
        }else m_mapIdToPlayer[i]->setInfo(i,true);
    }
    ui->pb_0_begin->setText("点击发言");
    ui->pb_0_begin->setEnabled(false);
    //判断死的是不是自己
    QString show="昨晚死亡的玩家是：";
    for(int i=0;i<2;i++){
        //是自己,不能在进行操作TODO
        if(die[i]==m_seat){
            if(m_d_bePolice){
                //自己是警长，选择是否移交警徽TODO
                //操作栏询问是否移交警徽
                ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/turnPolice.png")));
                ui->pb_operate_up->setIconSize(QSize(280,40));
                //设置定时装置，超时撕警徽
                //移交：存活玩家设置可被选择
                //不移交：没有警徽
                //设置头像选择阶段
            }
            ui->tb_message->append("你死了");
            m_d_alive=false;
            m_mapIdToPlayer[m_seat]->setImage("000");
            m_mapIdToPlayer[m_seat]->alive=false;
        }
        //显示死亡信息
        if(die[i]!=0){
            show+=QString("%1号 ").arg(die[i]);
            m_mapIdToPlayer[die[i]]->setImage("000");
            m_mapIdToPlayer[die[i]]->alive=false;
        }
    }
    ui->tb_message->append("天亮了！！！！");
    ui->tb_message->append(show);
}

void roomDialog::slot_speak(STRU_SPEAK_RQ& rq)
{
    m_speak=rq.seat;
    speak=120;
    m_timer_speak->start(1000);
    ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
    ui->pb_operate_up->setIconSize(QSize(280,40));
    //判断发言人是不是自己
    switch(rq.state){
    case 3://白天正常发言
        ui->pb_day->setText("1白天");
        m_d_state=3;
        if(rq.seat==m_seat){
            //如果已经发过言了，发言阶段结束
            if(m_d_speak==1){
                Q_EMIT SIG_SpeakStateEnd(2);
                m_d_speak=0;
                return;
            }
        }
        ui->tb_message->append(QString("请%1号玩家开始发言").arg(rq.seat));
        if(rq.seat==m_seat){
            m_d_speak=1;
//            ui->tb_message->append("开始发言");
            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/beginSpeak.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            ui->pb_0_begin->setText("点击发言");
            m_pb_begin=BEGIN_SPEAK;
            ui->pb_0_begin->setEnabled(true);
            ui->pb_0_end->setEnabled(true);
        }
        break;
    case 1://上警玩家发言
        m_d_nextSpeak=1;
        m_d_state=1;
        if(m_d_police)ui->pb_operate->setEnabled(true);//可以放手
        if(rq.seat==m_seat){
            //如果已经发过言了，发言阶段结束
            if(m_d_speak==1){
                Q_EMIT SIG_SpeakStateEnd(1);
                m_d_speak=0;
                return;
            }
        }
        if(rq.seat==m_seat){
            m_d_speak=1;
            //判断自己有没有上警
            if(m_d_police){
                //如果上警：发言
                //ui->lb_operate->setText("开始发言");
//                ui->tb_message->append("开始发言");
                ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/beginSpeak.png")));
                ui->pb_operate_up->setIconSize(QSize(280,40));
                ui->pb_0_begin->setText("点击发言");
                m_pb_begin=BEGIN_SPEAK;
                ui->pb_0_begin->setEnabled(true);
                ui->pb_0_end->setEnabled(true);
            }else{
                //没上警：发送发言结束包，下一个人是后面的人
                Q_EMIT SIG_SpeakEnd(m_seat,1,rq.state);
            }
        }
        break;
    case 2://没有警长，依次发言
        ui->pb_day->setText("1白天");
        m_d_state=2;
        m_d_nextSpeak=1;
        if(rq.seat==m_seat){
            //如果已经发过言了，发言阶段结束
            if(m_d_speak==1){
                Q_EMIT SIG_SpeakStateEnd(2);
                m_d_speak=0;
                return;
            }
        }
        ui->tb_message->append(QString("请%1号玩家开始发言").arg(rq.seat));
        if(rq.seat==m_seat){
            m_d_speak=1;
//            ui->tb_message->append("开始发言");
            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/beginSpeak.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            ui->pb_0_begin->setText("点击发言");
            m_pb_begin=BEGIN_SPEAK;
            ui->pb_0_begin->setEnabled(true);
            ui->pb_0_end->setEnabled(true);
        }
        break;
    case 4://淘汰发言
        if(m_seat==rq.seat)ui->tb_message->append("请您进行淘汰发言");
        else ui->tb_message->append(QString("【%1】号进行淘汰发言").arg(rq.seat));
        if(m_seat==rq.seat){
            m_d_state=4;
            ui->pb_0_end->setEnabled(true);
            ui->pb_0_begin->setText("点击发言");
            m_pb_begin=BEGIN_SPEAK;
            ui->pb_0_begin->setEnabled(true);
        }
        break;
    }
}


void roomDialog::slot_police()
{
    for(int i=1;i<=m_count;i++){
        if(m_mapIdToPlayer[i]->m_id!=m_seat){
            m_mapIdToPlayer[i]->resumeVoted();
        }else m_mapIdToPlayer[i]->setInfo(i,true);
    }
    ui->pb_0_begin->setText("点击发言");
    ui->pb_0_begin->setEnabled(false);
    police=10;
    m_timer_police->start(1000);
    //询问是否竞选警长
//    if(QMessageBox::question(this,"提示","是否竞选警长")==QMessageBox::Yes){
//        //是：回复竞选
//        m_d_police=true;
//        Q_EMIT SIG_police(m_seat,true);
//    }else m_d_police=false;
    slot_operate_button(WANT_BEPOLICE,0);
    ui->pb_day->setText("竞选阶段");
}

void roomDialog::slot_bePolice()
{
    //判断自己有没有竞选
    if(m_d_police){
        //竞选了，自己是警长，发送警长回复包
        Q_EMIT SIG_imPolice(m_seat,1);
        m_d_bePolice=true;
    }
    //没竞选，自己不是警长
}

//上警
void roomDialog::slot_setPolicePlayer(STRU_TOBEPOLICE_RS& rs)
{
    //将对应位置的玩家警徽图标设为上警1:上警 2:放手 3:警长
    if(rs.raise){
        m_mapIdToPlayer[rs.seat]->setJing(1);
        Text_upPolice+=QString("%1号 ").arg(rs.seat);
    }
    else{
        m_mapIdToPlayer[rs.seat]->setJing(2);
        ui->tb_message->append(QString("%1号放手了").arg(rs.seat));
    }
}

//当选
void roomDialog::slot_setPolice(STRU_BEPOLICE_RS &rs)
{
    //将对应位置的玩家警徽图标设为上警1:上警 2:放手 3:警长
    for(int i=1;i<=m_count;i++){
        if(m_mapIdToPlayer[i]->alive)m_mapIdToPlayer[i]->setJing(0);
    }
    m_mapIdToPlayer[rs.seat]->setJing(3);
    m_d_policer=rs.seat;
    ui->tb_message->append(QString("%1号当选警长").arg(rs.seat));
    if(rs.seat==m_seat)m_d_bePolice=true;
}

void roomDialog::slot_beginVote(STRU_SPEAKSTATE_END &end)
{
    vote=10;
    m_timer_vote->start(1000);
    //判断是哪个阶段的投票
    //根据阶段查找可以被投票的人
    switch(end.state){
    case 1://竞选阶段发言结束，开始投票
        //        ui->lb_operate->setText("开始投票,选出警长");
//        ui->tb_message->append("开始投票,选出警长");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/voteToPolice.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        m_d_vote=1;
        if(m_mapIdToPlayer[m_seat]->getJing()==""){
            m_pb_icon=VOTE_POLICE;
            for(int i=1;i<=m_count;i++){
                if(m_mapIdToPlayer[i]->getJing()=="上"){
                    m_mapIdToPlayer[i]->setAbleToVoted(true);
                }else m_mapIdToPlayer[i]->setAbleToVoted(false);
            }
        }
        break;
    case 2://白天发言结束，开始投票
        ui->tb_message->append("开始放逐投票，点击头像选择你要放逐的对象");
        m_d_vote=2;
        m_pb_icon=VOTE_DAY;
        for(int i=1;i<=m_count;i++){
            if(m_mapIdToPlayer[i]->alive){
                m_mapIdToPlayer[i]->setAbleToVoted(true);
            }else m_mapIdToPlayer[i]->setAbleToVoted(false);
        }
        break;
    }
}

void roomDialog::slot_VoteRs(STRU_VOTE_RS &rs)
{
    //根据阶段进行操作
    //显示投票结果
    QString res[13];
    for(int i=1;i<=12;i++)res[i]=QString("%1号:").arg(i);
    int num[13]={0};
    for(int i=1;i<=12;i++){
        if(rs.result[i]){
            num[rs.result[i]]++;
            res[rs.result[i]]+=QString("%1号 ").arg(i);
        }
    }
    if(rs.state==1)ui->tb_message->append("警上竞选投票结果：");
    else if(rs.state==2)ui->tb_message->append("放逐投票结果：");
    int num1[12]={1};
    int flag=0;
    for(int i=1;i<13;i++)if(num[i]){
        //记录最大票数的玩家
        /*
         * 数组num1用来记录玩家座位号
         * 如果遍历num的票数大于num1的第一个元素票数，说明当前遍历的这个票数多，更新第一个元素
         * 如果遍历num的票数小于第一个元素票数，不用更新
         * 如果遍历的票数和最大票数相等，将其保存在num1的后一个位置
         * 在计算票数的时候，只需要看第二个位置的票数和第一个位置是不是相等，即可判断是否平票
         */
        if(num[i]>num[num1[0]]){
            num1[0]=i;
            flag=1;
        }else if(num[i]<num[num1[0]]){}
        else{
            while(num[i]==num[num1[flag]]){
                flag++;
            }
            num1[flag]=i;
            flag++;
        }
        ui->tb_message->append(res[i]);
    }
    switch(rs.state){
    case 1://上警
        //根据结果发送下一步操作包
        if(m_seat==1){
            if(num[num1[0]]==0||num[num1[0]]==num[num1[1]]){
                //没有票，没有警长；平票：没有警长
                Q_EMIT SIG_imPolice(0,1);
            }else{
                //票数最多的当选警长，发送警长包
                Q_EMIT SIG_imPolice(num1[0],1);
            }
        }
        break;
    case 2://白天放逐
        //先显示是谁被驱逐（计算出票型后，显示再公告栏，待被驱逐玩家发言结束后（收到服务端发来的死亡玩家信息），更新其状态
        //根据结果发送下一步操作包
        if(num[num1[0]]==0||num[num1[0]]==num[num1[1]]){
            //没有票或平票，没人被放逐
            if(m_seat==1)Q_EMIT SIG_dayExile(0);
        }else{
            //票数最多的被放逐
            if(m_seat==1)Q_EMIT SIG_dayExile(num1[0]);
            ui->tb_message->append(QString("【%1号】玩家被放逐").arg(num1[0]));
        }
        break;
    }
}

void roomDialog::slot_SpeakOrder(STRU_SPEAK_ORDER &order)
{
    if(order.next==0){
        //如果next=0，说明发言顺序还没定，要警长选择发言顺序
        speakOrder=10;
        m_timer_speakOrder->start(1000);
        if(m_d_bePolice){
//            ui->lb_operate->setText("请选择发言顺序");
            ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/selectSpeakOrder.png")));
            ui->pb_operate_up->setIconSize(QSize(280,40));
            ui->pb_order->setText("顺序发言");
            ui->pb_deorder->setText("逆序发言");
            ui->pb_order->setEnabled(true);
            ui->pb_deorder->setEnabled(true);
            m_pb_oper=SPEAK_ORDER;
        }
    }else{
        //如果next有值，说明发言顺序已经定好，设置自己的发言顺序
        QString speakOrder=order.next==1?"顺序":"逆序";
        ui->tb_message->append(QString("房主选择【%1】发言").arg(speakOrder));
        m_d_nextSpeak=order.next;
    }
}

//发言阶段开始
void roomDialog::slot_SpeakStateBegin()
{
    ui->tb_message->append("---------进入发言阶段---------");
}

void roomDialog::slot_dayExile(STRU_DAY_EXILE& exile)
{
    if(m_seat==exile.die){
        m_d_alive=false;
        ui->tb_message->append("你死了");
    }
    m_mapIdToPlayer[exile.die]->setImage("000");
    m_mapIdToPlayer[exile.die]->alive=false;
}

void roomDialog::slot_gameOver()
{
    ui->tb_message->append("游戏结束");
}

void roomDialog:: slot_playerSpeak(int seat,bool speak)
{
    if(m_mapIdToPlayer.count(seat)>0)m_mapIdToPlayer[seat]->setSpeak(speak);
}

void roomDialog::slot_speakEnd(STRU_SPEAK_RS &end)
{
    ui->tb_message->append(QString("[%1]号结束发言").arg(end.seat));
    if(m_mapIdToPlayer.count(end.seat)>0)m_mapIdToPlayer[end.seat]->resumeVoted();
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
            ui->pb_0_begin->setText("开始游戏");
            ui->pb_0_begin->setEnabled(true);
        }
    }
    //设置成员信息
    if(userid==m_userid){//判断该信息是不是自己
        m_seat=id;
        m_mapIdToPlayer[id]->setInfo(id,true);
        m_mapIdToPlayer[id]->setMe(true);
    }
    m_mapIdToPlayer[id]->setImage(QString("%1").arg(icon));
    m_mapIdToPlayer[id]->setZiLiao(level,sex,name,userid,m_count);
}

roomDialog::~roomDialog()
{
    delete ui;
}


/////////////////////////////////////////////////////////
//////////////////////组合类槽函数/////////////////////////
/////////////////////////////////////////////////////////

//玩家头像被点击
void roomDialog::slot_click_icon(int id)
{
    //判断当前是什么状态
    switch(m_pb_icon){
    case USERINFO://玩家信息
        //显示玩家资料TODO
        QMessageBox::about(this,"资料","xxxxx");
        break;
    case SKYBLK_YYJ://预言家夜晚
        //直接发送选择，TODO:等待计时结束，将最后一个选择的发送出去
        Q_EMIT SIG_skyBlkRs(m_user_iden,m_seat,1,id);
        //操作完，将状态恢复成默认
        m_pb_icon=USERINFO;
        for(int i=1;i<=m_count;i++){
            if(m_mapIdToPlayer[i]->m_id!=m_seat){
                m_mapIdToPlayer[i]->resumeVoted();
            }else m_mapIdToPlayer[i]->setInfo(i,true);
        }
        break;
    case SKYBLK_LR://狼人夜晚
        //直接发送杀人信息
        Q_EMIT SIG_skyBlkRs(m_user_iden,m_seat,2,id);
        for(int i=1;i<=m_count;i++){
            if(m_mapIdToPlayer[i]->m_id!=m_seat){
                m_mapIdToPlayer[i]->resumeVoted();
            }else m_mapIdToPlayer[i]->setInfo(i,true);
        }
        m_pb_icon=USERINFO;
        break;
    case SKYBLK_NW://女巫夜晚
        //在收到杀人信息时进行的操作：弹出框显示杀人信息，询问是否救人，如果救，发送救人信息，如果不救，询问是否毒人（将按钮状态设置成女巫），发送操作信息
        //毒人
        Q_EMIT SIG_skyBlkRs(m_user_iden,m_seat,3,id);
//        ui->lb_operate->setText("等待天亮");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/waitDayLight.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        m_d_poison=false;
        ui->tb_message->append(QString("您选择毒死的玩家是%1号").arg(id));
        for(int i=1;i<=m_count;i++){
            if(m_mapIdToPlayer[i]->m_id!=m_seat){
                m_mapIdToPlayer[i]->resumeVoted();
            }else m_mapIdToPlayer[i]->setInfo(i,true);
        }
        m_pb_icon=USERINFO;
        break;
    case SKYBLK_SW://守卫夜晚
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
        Q_EMIT SIG_vote(m_seat,id,1);
        m_pb_icon=USERINFO;
        for(int i=1;i<=m_count;i++){
            if(m_mapIdToPlayer[i]->m_id!=m_seat){
                m_mapIdToPlayer[i]->resumeVoted();
            }
        }
        break;
    case VOTE_DAY://白天投票放逐
        Q_EMIT SIG_vote(m_seat,id,2);
        m_pb_icon=USERINFO;
        for(int i=1;i<=m_count;i++){
            if(m_mapIdToPlayer[i]->m_id!=m_seat){
                m_mapIdToPlayer[i]->resumeVoted();
            }else m_mapIdToPlayer[i]->setInfo(i,true);
        }
    }
}

//操作文本框的显示信息
void roomDialog::slot_operate_button(int state,int kill)
{
    //0:女巫救人 1：女巫毒人 2：没死人
    switch(state){
    case NW_RESCUE://女巫救人
//        ui->lb_operate->setText(QString("昨夜死亡的玩家是%1号，是否救他？").arg(kill));
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/%1die.png").arg(kill)));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        m_pb_oper=NW_RESCUE;
        ui->pb_order->setEnabled(true);
        ui->pb_deorder->setEnabled(true);
        ui->pb_order->setText("是");
        ui->pb_deorder->setText("否");
        break;
    case NW_POISON://女巫毒人
//        ui->lb_operate->setText(QString("你有一瓶毒药，是否毒人？"));
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/nvPosion.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        m_pb_oper=NW_POISON;
        ui->pb_order->setEnabled(true);
        ui->pb_deorder->setEnabled(true);
        ui->pb_order->setText("是");
        ui->pb_deorder->setText("否");
        break;
    case NW_NODIE://无人死亡
//        ui->lb_operate->setText(QString("昨夜无人死亡"));
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/nvNoDie.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setEnabled(true);
        ui->pb_order->setText("知道了");
        m_pb_oper=NW_NODIE;
        break;
    case WANT_BEPOLICE:
//        ui->lb_operate->setText(QString("是否竞选警长？"));
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/policeYesOrNo.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setEnabled(true);
        ui->pb_deorder->setEnabled(true);
        ui->pb_order->setText("是");
        ui->pb_deorder->setText("否");
        m_pb_oper=WANT_BEPOLICE;
        break;
    }
}

/////////////////////////////////////////////////////////
//////////////////////组件槽函数///////////////////////////
/////////////////////////////////////////////////////////

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
    Q_EMIT SIG_quitRoom(m_mapIdToPlayer[1]->getUserid());//信号中为房主的id，用于在ckernel类中判断退出房间的玩家是不是房主
    Q_EMIT SIG_Audio(false,false,false);
}


void roomDialog::on_pb_0_begin_clicked()
{
    switch(m_pb_begin){
    case BEGIN_GAME:
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
        break;
    case BEGIN_SPEAK:
        //按钮转换成停止发言
        ui->pb_0_begin->setText("停止发言");
        m_pb_begin=END_SPEAK;
        //开始获取麦克风数据
        Q_EMIT SIG_Audio(true,true,false);
        break;
    case END_SPEAK:
        //转换按钮成开始发言
        ui->pb_0_begin->setText("点击发言");
        m_pb_begin=BEGIN_SPEAK;
        //通知获取麦克风数据
        Q_EMIT SIG_Audio(false,true,false);
        break;
    case WOLF_SPEAK:
        //开启麦克风，发送给所有狼人
        ui->pb_0_begin->setText("停止发言");
        m_pb_begin=WOLF_SPEAK_END;
        Q_EMIT SIG_Audio(true,true,true);
        break;
    case WOLF_SPEAK_END:
        //关闭麦克风，发送给所有狼人
        ui->pb_0_begin->setText("开始发言");
        m_pb_begin=WOLF_SPEAK;
        Q_EMIT SIG_Audio(false,true,true);
        break;
    }
}




void roomDialog::on_pb_0_end_clicked()
{
    //结束发言
    Q_EMIT SIG_SpeakEnd(m_seat,m_d_nextSpeak,m_d_state);
    ui->pb_0_begin->setText("点击发言");
    ui->pb_0_begin->setEnabled(false);
    ui->pb_0_end->setEnabled(false);
}


void roomDialog::on_pb_operate_clicked()
{
    //放手
    m_d_police=false;
    Q_EMIT SIG_police(m_seat,false);
}


void roomDialog::on_pb_order_clicked()
{//救，毒，顺序,知道了,竞选
    switch(m_pb_oper){
    case NW_RESCUE://救人
        Q_EMIT SIG_nvSilverWater();
        m_d_antidote=false;
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        m_pb_icon=USERINFO;
        break;
    case NW_POISON://毒人
        for(int i=1;i<=m_count;i++){
            if(&m_mapIdToPlayer[i]->alive){//活着
                m_mapIdToPlayer[i]->setAbleToVoted(true);
            }else m_mapIdToPlayer[i]->setAbleToVoted(false);
        }
        m_pb_icon=SKYBLK_NW;
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        break;
    case NW_NODIE://如果有毒药，询问是否毒人（知道了）
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        if(m_d_poison){
            slot_operate_button(NW_POISON,0);
        }
        break;
    case WANT_BEPOLICE://竞选
        m_d_police=true;
        Q_EMIT SIG_police(m_seat,true);
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        break;
    case SPEAK_ORDER://顺序
        Q_EMIT SIG_speakOrder(m_seat,1);
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        break;
    }
}


void roomDialog::on_pb_deorder_clicked()
{//不救，不毒，逆序，不竞选
    switch(m_pb_oper){
    case NW_RESCUE:
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        if(m_d_poison){
            slot_operate_button(NW_POISON,0);
        }
        break;
    case NW_POISON:
        ui->tb_message->append("等待天亮");
        m_pb_icon=USERINFO;
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        break;
    case WANT_BEPOLICE://不竞选
        m_d_police=false;
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        break;
    case SPEAK_ORDER://逆序
        Q_EMIT SIG_speakOrder(m_seat,-1);
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        break;
    }

}



/////////////////////////////////////////////////////////
//////////////////////计时器函数///////////////////////////
/////////////////////////////////////////////////////////

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
    if(blk==15){
        //是房主发送结束包
        if(m_seat==1)Q_EMIT SIG_skyBlk15(true);
        //狼人不能行动了
        if(m_user_iden==3){
            for(int i=1;i<=m_count;i++){
                if(m_mapIdToPlayer[i]->m_id!=m_seat){
                    m_mapIdToPlayer[i]->resumeVoted();
                }else m_mapIdToPlayer[i]->setInfo(i,true);
            }
            m_pb_icon=USERINFO;
        }
    }
    if(blk==0){
        //房主发送夜晚结束包
        if(m_seat==1)Q_EMIT SIG_skyBlk15(false);
        //所有玩家不能行动
        for(int i=1;i<=m_count;i++){
            if(m_mapIdToPlayer[i]->m_id!=m_seat){
                m_mapIdToPlayer[i]->resumeVoted();
            }else m_mapIdToPlayer[i]->setInfo(i,true);
        }
        m_pb_icon=USERINFO;
        m_timer_skyBlk->stop();
    }
}

void roomDialog::slot_OverTimerPolice()
{
    ui->lb_time->setText(QString("%1").arg(police));
    police--;
    //判断是不是房主
    if(police==0){
        if(m_seat==1)Q_EMIT SIG_PoliceEnd();//是房主发送竞选阶段结束包
//        ui->lb_operate->setText("");
        ui->pb_operate_up->setIcon(QIcon(QString(":/tupian/kong.png")));
        ui->pb_operate_up->setIconSize(QSize(280,40));
        ui->pb_order->setText("");
        ui->pb_deorder->setText("");
        ui->pb_order->setEnabled(false);
        ui->pb_deorder->setEnabled(false);
        m_timer_police->stop();
        ui->lb_time->setText("");
    }
}

void roomDialog::slot_OverTimerVote()
{
    ui->lb_time->setText(QString("%1").arg(vote));
    vote--;
    //判断是不是房主
    if(vote==0){
        if(m_seat==1)Q_EMIT SIG_VoteEnd(m_d_vote);//是房主发送竞选投票阶段结束包
        m_timer_vote->stop();
        ui->lb_time->setText("");
    }
}

void roomDialog::slot_OverTimerSpeakOrder()
{
    ui->lb_time->setText(QString("%1").arg(speakOrder));
    speakOrder--;
    if(speakOrder==0){
        if(m_seat==1)Q_EMIT SIG_speakOrder(m_d_policer,1);//是房主发送顺序选择结束包
        m_timer_speakOrder->stop();
        ui->lb_time->setText("");
    }
}

void roomDialog::slot_OverTimerSpeak()
{
    ui->lb_time->setText(QString("%1").arg(speak));
    speak--;
    if(speak==0){
        if(m_seat==1)Q_EMIT SIG_SpeakEnd(m_speak,m_d_nextSpeak,m_d_state);//是房主发送发言结束包
        m_timer_speak->stop();
        ui->lb_time->setText("");
        ui->pb_0_begin->setText("点击发言");
        ui->pb_0_begin->setEnabled(false);
        ui->pb_0_end->setEnabled(false);
    }
}
