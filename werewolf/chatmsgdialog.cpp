#include "chatmsgdialog.h"
#include "ui_chatmsgdialog.h"
#include<QMessageBox>

ChatMsgDialog::ChatMsgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatMsgDialog)
{
    ui->setupUi(this);
}

ChatMsgDialog::~ChatMsgDialog()
{
    delete ui;
}

void ChatMsgDialog::setinfo(QString name, int id)
{
    ui->lb_title->setText(QString("和【%1】的聊天记录").arg(name));
    m_id=id;
    m_name=name;
    ui->tb_msg->clear();
}

void ChatMsgDialog::slot_setMsg(bool me, QString time, QString content)
{
    if(me){
        ui->tb_msg->append(QString("[我] %1").arg(time));
        ui->tb_msg->append(content);
    }else{
        ui->tb_msg->append(QString("[%1] %2").arg(m_name).arg(time));
        ui->tb_msg->append(content);
    }
}

void ChatMsgDialog::on_pb_clear_clicked()
{
    if(QMessageBox::question(this,"提示","是否确认删除")==QMessageBox::Yes){
        Q_EMIT SIG_deleteMsg(m_id);
        ui->tb_msg->clear();
    }
}

