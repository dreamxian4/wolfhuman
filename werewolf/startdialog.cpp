#include "startdialog.h"
#include "ui_startdialog.h"
#include<QMessageBox>

startDialog::startDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startDialog)
{
    ui->setupUi(this);
}

startDialog::~startDialog()
{
    delete ui;
}

void startDialog::on_pb_notice_clicked()
{
    //TODO:发送信号：弹出公告栏
//    Q_EMIT SIG_showNotice();
    QMessageBox::about(this,"公告","i'm 公告");
}


void startDialog::on_pb_joinGame_clicked()
{
    //发送信号，ckernel连接网络
    Q_EMIT SIG_joinGame();
}

