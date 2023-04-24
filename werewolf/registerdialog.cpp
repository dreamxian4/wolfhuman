#include "registerdialog.h"
#include "ui_registerdialog.h"
#include<qDebug>
#include<QRegExp>
#include<QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    //时间设置
    ui->de_born->setCalendarPopup(true);
    ui->de_born->setDate(QDate::currentDate());
    ui->de_born->setMaximumDate(QDate::currentDate());
    //默认性别：男
    ui->rb_boy->setChecked(true);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::slot_setInfo(QString username, QString passwd, QString name, QString sex)
{
    ui->le_name->setText(name);
    ui->le_passwd->setText(passwd);
    ui->le_passagain->setText(passwd);
    ui->le_username->setText(username);
    if(sex=="男")ui->rb_boy->setChecked(true);
    else ui->rb_girl->setChecked(true);
    ui->de_born->setDate(QDate::currentDate());
}

void RegisterDialog::on_pb_clear_clicked()
{
    //清空
    ui->le_name->setText("");
    ui->le_passwd->setText("");
    ui->le_passagain->setText("");
    ui->le_username->setText("");
    ui->de_born->setDate(QDate::currentDate());
}


void RegisterDialog::on_pb_register_clicked()
{
    QString username    =ui->le_username->text();
    QString passwd      =ui->le_passwd->text();
    QString again       =ui->le_passagain->text();
    QString name        =ui->le_name->text();
    QString sex         =ui->rb_boy->isChecked()?"男":"女";
    QDate   date          =ui->de_born->date();
//    QString year=QString::number(date.year());
//    QString month=QString::number(date.month());
//    QString day=QString::number(date.day());

    if(username.isEmpty()||passwd.isEmpty()||again.isEmpty()||name.isEmpty()){
        QMessageBox::about(this,"提示","输入不可以为空哦~");
        return;
    }
    //用户名：由字母数字下划线组成，大于6位小于10位
    QRegExp reg1("^[a-zA-Z0-9_]{6,10}$");
    bool res=reg1.exactMatch(username);
    if(!res){
        QMessageBox::about(this,"提示","用户名不合法~");
        return;
    }
    //密码：由字母数字组成，不超过20位
    QRegExp reg2("^[a-zA-Z0-9]{1,20}$");
    res=reg2.exactMatch(username);
    if(!res){
        QMessageBox::about(this,"提示","密码不合法~");
        return;
    }
    if(passwd!=again){
        QMessageBox::about(this,"提示","两次密码要一致！");
        return;
    }
    //昵称：不超过15位
    if(name.length()>15){
        QMessageBox::about(this,"提示","名字太长啦！");
        return;
    }
    //全部合格，发送信号
    Q_EMIT SIG_submit(username,passwd,name,sex,date);
}


void RegisterDialog::on_pb_return_clicked()
{
    //发送信号，回到登录界面
    Q_EMIT SIG_return();
}

