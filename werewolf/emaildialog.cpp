#include "emaildialog.h"
#include "ui_emaildialog.h"
#include<QMessageBox>

emailDialog::emailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::emailDialog)
{
    ui->setupUi(this);
    m_layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);  //控件上下左右的间距
    m_layout->setSpacing(3);        //设置每个控件彼此间的距离
    ui->wdg_list->setLayout(m_layout);
}



emailDialog::~emailDialog()
{
    delete ui;
}

void emailDialog::slot_setInfo(bool space)
{
    if(space){
        ui->pb_clear_friend->setText("");
        ui->pb_clear_friend->setStyleSheet("background:transparent");
        ui->pb_clear_friend->setEnabled(false);
    }else{
        ui->pb_clear->setText("");
        ui->pb_clear->setStyleSheet("background:transparent");
        ui->pb_clear->setEnabled(false);
    }
}

void emailDialog::slot_addwidget(emailMsgForm *form)
{
    m_layout->insertWidget(0,form);
    lst.push_back(form);
}

void emailDialog::slot_removewidget(emailMsgForm *form)
{
    form->hide();
    m_layout->removeWidget(form);
}

void emailDialog::slot_clearwidget()
{
    while(!lst.empty()){
        slot_removewidget(lst.front());
        emailMsgForm* form=lst.front();
        lst.pop_front();
        delete form;
        form=nullptr;
    }
}

void emailDialog::on_pb_clear_clicked()
{
    if(QMessageBox::question(this,"提示","是否确认清空？")==QMessageBox::Yes){
        Q_EMIT SIG_clearSpaceEmail(true);
    }
}


void emailDialog::on_pb_clear_friend_clicked()
{
    if(QMessageBox::question(this,"提示","是否确认清空？")==QMessageBox::Yes){
        Q_EMIT SIG_clearFriendEmail(false);
    }
}

