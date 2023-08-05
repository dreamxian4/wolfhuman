#include "commentdialog.h"
#include "ui_commentdialog.h"
#include<QIcon>

commentDialog::commentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::commentDialog)
{
    ui->setupUi(this);
    m_layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);  //控件上下左右的间距
    m_layout->setSpacing(3);        //设置每个控件彼此间的距离
    ui->wdg_2_list_2->setLayout(m_layout);
}

commentDialog::~commentDialog()
{
    delete ui;
}

void commentDialog::slot_setInfo(int icon, QString name, QString time, QString content,int id)
{
    spaceid=id;
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));
    ui->pb_icon->setIconSize(QSize(60,60));
    ui->lb_name->setText(name);
    ui->lb_time->setText(time);
    ui->lb_content->setText(content);
}

void commentDialog::slot_addwidget(commentForm* form)
{
    m_layout->insertWidget(0,form);
//    m_layout_space->addWidget(spaceform);
    lst.push_back(form);
}

void commentDialog::slot_removewidget(commentForm* form)
{
    form->hide();
    m_layout->removeWidget(form);
}

void commentDialog::slot_clearwidget()
{
    while(!lst.empty()){
        slot_removewidget(lst.front());
        commentForm* form=lst.front();
        lst.pop_front();
        delete form;
        form=nullptr;
    }
}

void commentDialog::on_pb_sendComment_clicked()
{
    if(ui->le_input->text().size()==0)return;
    Q_EMIT SIG_sendComment(spaceid,ui->le_input->text());
    ui->le_input->setText("");
}

