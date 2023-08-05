#include "commentform.h"
#include "ui_commentform.h"

commentForm::commentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::commentForm)
{
    ui->setupUi(this);
}

commentForm::~commentForm()
{
    delete ui;
}

void commentForm::slot_setInfo(QString name, QString time, QString content)
{
    ui->lb_name->setText(name);
    ui->lb_time->setText(time);
    ui->lb_comment->setText(content);
}
