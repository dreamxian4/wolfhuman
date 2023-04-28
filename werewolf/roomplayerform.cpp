#include "roomplayerform.h"
#include "ui_roomplayerform.h"
#include<QPainter>
#include<qDebug>
#include<QSize>

roomPlayerform::roomPlayerform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roomPlayerform)
{
    ui->setupUi(this);
    setImage(0);
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

void roomPlayerform::setInfo(int id, int identify, int jing)
{
    ui->lb_num->setText(QString("%1").arg(id));
    //设置警徽和身份TODO
}

void roomPlayerform::setImage(int icon)
{
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    ui->pb_icon->setIconSize(QSize(50,40));
}

