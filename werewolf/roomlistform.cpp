#include "roomlistform.h"
#include "ui_roomlistform.h"
#include<qDebug>
#include<QInputDialog>
#include<QMessageBox>


roomListForm::roomListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roomListForm)
{
    ui->setupUi(this);
}

roomListForm::~roomListForm()
{
    delete ui;
}

void roomListForm::setinfo(int mode, int method, int roomid, int count,
                           int current, bool pass, QString passwd, bool state, int level)
{
    m_mode=mode;
    m_method=method;
    m_roomid=roomid;
    m_count=count;
    m_currentCou=current;
    m_pass=pass;
    m_passwd=passwd;
    m_state=state;
    m_level=level;
    up();
}

void roomListForm::up()
{
    QString method="基础模式";
    //TODO设置音频视频符号图片
    //TODO根据是否有密码设置房间图片
    QString level=QString("LV.%1").arg(m_level);
    //TODO根据阶段设置按钮
    QString state=m_state?"已开始":"准备阶段";
    QString num=QString("%1/%2人").arg(m_currentCou).arg(m_count);
    //TODO设置进入图片
    ui->lb_method->setText(method);
    ui->lb_level->setText(level);
    ui->lb_num->setText(num);
    ui->lb_state->setText(state);
}

void roomListForm::on_pb_joinRoom_clicked()
{
    if(m_pass){
        QString str=QInputDialog::getText(this,"已上锁","密码：");
        if(str.isEmpty())return;
        else{
            if(str!=m_passwd){
                QMessageBox::about(this,"提示","密码错误，加入房间失败");
                return;
            }
        }
    }
    //state判断：已开始，观战模式；房间人数：已满，观战模式
    Q_EMIT SIG_joinRoom(m_roomid);
}

