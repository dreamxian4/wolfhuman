#include "spaceform.h"
#include "ui_spaceform.h"
#include<QDebug>

spaceForm::spaceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spaceForm)
{
    ui->setupUi(this);
    userid     =0;
    spaceid    =0;
    tui        =0;
    good       =0;
    comment    =0;
    isgood=false;
    istui=false;
}

spaceForm::~spaceForm()
{
    delete ui;
}

void spaceForm::setInfo(STRU_SPACE_RS *rs)
{
    userid=rs->userid;
    spaceid=rs->spaceid;
    tui=rs->tui;
    good=rs->good;
    comment=rs->comment;
    isgood=rs->isgood;
    istui=rs->istui;
    icon=rs->icon;
    name=QString::fromStdString(rs->name);
    time=QString::fromStdString(rs->time);
    content=QString::fromStdString(rs->content);
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(rs->icon)));
    ui->pb_icon->setIconSize(QSize(60,60));
    ui->lb_name->setText(name);
    ui->lb_time->setText(time);
    ui->lb_content->setText(content);
    if(!rs->istui)ui->pb_tui->setText(QString("踩（%1）").arg(rs->tui));
    else ui->pb_tui->setText(QString("已踩（%1）").arg(rs->tui));
    ui->pb_comment->setText(QString("评论（%1）").arg(rs->comment));
    if(!rs->isgood)ui->pb_good->setText(QString("赞（%1）").arg(rs->good));
    else ui->pb_good->setText(QString("已赞（%1）").arg(rs->good));
}

void spaceForm::on_pb_icon_clicked()
{
    //获取用户的详细信息
    Q_EMIT SIG_Getdetail(userid);
}


void spaceForm::on_pb_good_clicked()
{
    if(isgood){
        isgood=0;
        good--;
        ui->pb_good->setText(QString("赞（%1）").arg(good));
        Q_EMIT SIG_SpaceOpt(spaceid,userid,1,0);
    }else{
        isgood=1;
        good++;
        ui->pb_good->setText(QString("已赞（%1）").arg(good));
        Q_EMIT SIG_SpaceOpt(spaceid,userid,1,1);
    }
}


void spaceForm::on_pb_tui_clicked()
{
    if(istui){
        istui=0;
        tui--;
        ui->pb_tui->setText(QString("踩（%1）").arg(tui));
        Q_EMIT SIG_SpaceOpt(spaceid,userid,2,0);
    }else{
        istui=1;
        tui++;
        ui->pb_tui->setText(QString("已踩（%1）").arg(tui));
        Q_EMIT SIG_SpaceOpt(spaceid,userid,2,1);
    }
}


void spaceForm::on_pb_comment_clicked()
{
    Q_EMIT SIG_GetComment(spaceid,icon,name,time,content,spaceid);
}

