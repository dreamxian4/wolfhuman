#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//注册
void LoginDialog::on_pb_register_clicked()
{
    //发送信号 核心类隐藏登录窗口，弹出注册窗口
    //如果注册成功，直接登录
    Q_EMIT SIG_register();
}

//登录
void LoginDialog::on_pb_login_clicked()
{
    //获取输入框中的数据，以信号的形式发送给核心类
    //核心类将其封装成数据包，发送给服务端
    QString username=ui->le_username->text();
    QString passwd=ui->le_passwd->text();
    //判断用户名密码是否符合规范
    if(username.isEmpty()||passwd.isEmpty()){
        QMessageBox::about(this,"提示","输入不可以为空哦~");
        return;
    }
    //用户名：由字母数字或下划线组成，不超过10位
    QRegExp reg1("^[a-zA-Z0-9_]{6,10}$");
    bool res=reg1.exactMatch(username);
    if(!res){
        QMessageBox::about(this,"提示","用户名不合法~");
        return;
    }
    //密码：不超过20位，由字母数字组成
    QRegExp reg2("^[a-zA-Z0-9]{1,20}$");
    res=reg2.exactMatch(username);
    if(!res){
        QMessageBox::about(this,"提示","密码不合法~");
        return;
    }
    //符合规范，发送信号
    Q_EMIT SIG_login(username,passwd);
}

//反馈按钮
void LoginDialog::on_pb_feedback_clicked()
{
    //TODO
    //弹出反馈框
    //获取用户输入的文字
    //以信号形式发送
}


void LoginDialog::on_pb_close_clicked()
{
    this->slot_close();
}


void LoginDialog::on_pb_min_clicked()
{
    this->showMinimized();
}

