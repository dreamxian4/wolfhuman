#include "addspacedialog.h"
#include "ui_addspacedialog.h"
#include<QMessageBox>

addSpaceDialog::addSpaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSpaceDialog)
{
    ui->setupUi(this);
}

addSpaceDialog::~addSpaceDialog()
{
    delete ui;
}

void addSpaceDialog::slot_clear()
{
    ui->te_space->setText("");
}

void addSpaceDialog::on_pb_send_clicked()
{
    QString str=ui->te_space->toPlainText();
    if(str.length()!=0){
        Q_EMIT SIG_sendSpace(str);
        this->hide();
    }
    else QMessageBox::about(this,"提示","输入为空");
    ui->te_space->setText("");
}


void addSpaceDialog::on_pb_cancel_clicked()
{
    ui->te_space->setText("");
}

