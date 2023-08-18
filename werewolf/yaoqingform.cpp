#include "yaoqingform.h"
#include "ui_yaoqingform.h"
#include<QIcon>


YaoQingForm::YaoQingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::YaoQingForm)
{
    m_id=0;
    ui->setupUi(this);
}

YaoQingForm::~YaoQingForm()
{
    delete ui;
}

void YaoQingForm::setinfo(int icon, QString name, int id,int roomid)
{
    m_id=id;
    QString iconPath=QString(":/tx/%1.png").arg(icon);
    ui->pb_icon->setIcon(QIcon(iconPath));
    ui->pb_icon->setIconSize(QSize(50,50));
    ui->lb_name->setText(name);
    m_roomid=roomid;
}

void YaoQingForm::on_pb_yao_clicked()
{
    Q_EMIT SIG_yaoqing(m_id,m_roomid);
}

