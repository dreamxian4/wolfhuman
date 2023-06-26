#include "roomplayerform.h"
#include "ui_roomplayerform.h"
#include<QPainter>
#include<qDebug>
#include<QSize>

roomPlayerform::roomPlayerform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roomPlayerform),m_id(0),
    alive(true),beKnown(false)
{
    ui->setupUi(this);
    setImage("0");
    ui->pb_edge->setEnabled(false);
}

roomPlayerform::~roomPlayerform()
{
    delete ui;
}

//void roomPlayerform::paintEvent(QPaintEvent *event)
//{
//    //画背景
//    QPainter painter(this);//在这个控件上绘画，需要这个对象指针
//    painter.setBrush((Qt::black));//设置黑色画刷
//    //画一个矩形，会按照画刷的颜色填充
//    painter.drawRect(0,0,this->width(),this->height());
//    //画图（人物或桌面）
//    event->accept();
//}

void roomPlayerform::setInfo(int id,bool isme)
{
    //判断是不是自己，设置醒目
    if(isme)ui->pb_edge->setStyleSheet("background-color: rgb(0, 255, 127);");
    ui->lb_num->setText(QString("%1").arg(id));
    m_id=id;
    ui->pb_icon->setEnabled(true);
}

void roomPlayerform::setImage(QString icon)
{
    //如果icon==00，即该位置是被锁住的，则按钮不能点击
    if(icon=="00"){
        //设置按钮不能点击
        ui->pb_icon->setEnabled(false);
    }
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    ui->pb_icon->setIconSize(QSize(50,40));
}

void roomPlayerform::setZiLiao(int level, QString sex, QString name, int userid,int count)
{
    m_level =level;
    m_sex   =sex;
    m_name  =name;
    m_userid=userid;
    m_num=count;
}

void roomPlayerform::setJing(int jing)
{
    //设置警徽图标
    switch(jing){
    case 1://上警
        ui->lb_badge->setText("上");
        break;
    case 2://放手
        ui->lb_badge->setText("放");
        break;
    case 3://警长
        ui->lb_badge->setText("警");
        break;
    default:
        ui->lb_badge->setText("");
        break;
    }
}

void roomPlayerform::setIden(int iden)
{
    //设置身份
    switch(iden){
    case 0://预言家
        break;
    case 1://女巫
    case 2://平民
    case 4://猎人
    case 5://守卫
        ui->lb_identify->setText("好");
        break;
    case 3://狼人
        ui->lb_identify->setText("坏");
        break;
    default:
        ui->lb_identify->setText("");
        break;
    }
}

int roomPlayerform::getUserid()
{
    return m_userid;
}

QString roomPlayerform::getJing()
{
    return ui->lb_badge->text();
}

void roomPlayerform::setAbleToVoted(bool ok)
{
    if(ok){//可以被投票
        ui->pb_edge->setStyleSheet("background-color: rgb(0, 85, 255);");
    }else{//不可以被投票
        ui->pb_icon->setEnabled(false);
    }
}

void roomPlayerform::resumeVoted()
{
    if(alive){
        ui->pb_icon->setEnabled(true);
        ui->pb_edge->setStyleSheet("background-color: rgb(255, 255, 255);");
    }
}

void roomPlayerform::setAbleToOper(int who)
{

}

void roomPlayerform::setSpeak(bool speak)
{
    if(speak)ui->pb_edge->setStyleSheet("background-color: rgb(255, 255, 127);");
    else ui->pb_edge->setStyleSheet("background-color: rgb(75,24,56);");
}




void roomPlayerform::on_pb_icon_clicked()
{
    Q_EMIT SIG_click_icon(m_id);
}

