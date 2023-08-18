#include "emailmsgform.h"
#include "ui_emailmsgform.h"
#include<QIcon>
#include<QSize>
#include<QDebug>

emailMsgForm::emailMsgForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emailMsgForm)
{
    ui->setupUi(this);
}

emailMsgForm::~emailMsgForm()
{
    delete ui;
}

void emailMsgForm::slot_setInfo(int icon, QString name, int userid, int which
                                , QString spaceCnt, QString msg,bool result)
{
    m_userid=userid;
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    ui->pb_icon->setIconSize(QSize(50,50));
    QString up="";
    QString down="";
    switch (which) {
    case 2://好友申请
        up+=QString("%1 申请添加你为好友").arg(name);
        down+=QString("申请信息：%1").arg(msg);
        break;
    case 3://申请结果
        if(result)up+=QString("%1 已成为你的好友").arg(name);
        else up+=QString("%1 拒绝添加你为好友").arg(name);
        ui->pb_no->setEnabled(false);
        ui->pb_no->setStyleSheet("background:transparent");
        ui->pb_yes->setEnabled(false);
        ui->pb_yes->setStyleSheet("background:transparent");
        ui->pb_yes->setText("");
        ui->pb_no->setText("");
        break;
    case 4://点赞动态
        up+=QString("%1 点赞了你的动态").arg(name);
        down+=QString("动态内容：%1").arg(spaceCnt);
        ui->pb_no->setEnabled(false);
        ui->pb_no->setStyleSheet("background:transparent");
        ui->pb_yes->setEnabled(false);
        ui->pb_yes->setStyleSheet("background:transparent");
        ui->pb_yes->setText("");
        ui->pb_no->setText("");

        break;
    case 5://评论动态
        up+=QString("%1 评论了你的动态：%2").arg(name).arg(msg);
        down+=QString("动态内容：%1").arg(spaceCnt);
        ui->pb_no->setEnabled(false);
        ui->pb_no->setStyleSheet("background:transparent");
        ui->pb_yes->setEnabled(false);
        ui->pb_yes->setStyleSheet("background:transparent");
        ui->pb_yes->setText("");
        ui->pb_no->setText("");

        break;
    }
    ui->lb_up->setText(up);
    ui->lb_down->setText(down);
}


void emailMsgForm::on_pb_no_clicked()
{
    ui->lb_opt->setText("已拒绝");
    ui->lb_opt->setStyleSheet("background-color: rgb(229, 254, 255);");
    ui->pb_no->setText("");
    ui->pb_yes->setText("");
    ui->pb_no->setEnabled(false);
    ui->pb_no->setStyleSheet("background:transparent");
    ui->pb_yes->setEnabled(false);
    ui->pb_yes->setStyleSheet("background:transparent");
    Q_EMIT SIG_AddResult(m_userid,false);
}


void emailMsgForm::on_pb_yes_clicked()
{
    ui->lb_opt->setText("已同意");
    ui->lb_opt->setStyleSheet("background-color: rgb(229, 254, 255);");
    ui->pb_no->setText("");
    ui->pb_yes->setText("");
    ui->pb_no->setEnabled(false);
    ui->pb_no->setStyleSheet("background:transparent");
    ui->pb_yes->setEnabled(false);
    ui->pb_yes->setStyleSheet("background:transparent");
    Q_EMIT SIG_AddResult(m_userid,true);
}

