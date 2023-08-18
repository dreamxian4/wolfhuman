#include "yaoqingfridialog.h"
#include "ui_yaoqingfridialog.h"

YaoQingFriDialog::YaoQingFriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YaoQingFriDialog)
{
    ui->setupUi(this);
    ui->setupUi(this);
    m_layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);  //控件上下左右的间距
    m_layout->setSpacing(3);        //设置每个控件彼此间的距离
    ui->wdg_list->setLayout(m_layout);
}

YaoQingFriDialog::~YaoQingFriDialog()
{
    delete ui;
}

void YaoQingFriDialog::slot_setInfo(int roomid)
{
    ui->lb_title->setText(QString("房间号：%1").arg(roomid));
}

void YaoQingFriDialog::slot_addwidget(YaoQingForm *item)
{
    m_layout->addWidget(item);
    lst.push_back(item);
}

void YaoQingFriDialog::slot_removewidget(YaoQingForm *item)
{
    item->hide();
    m_layout->removeWidget(item);
}

void YaoQingFriDialog::slot_clearwidget()
{
    while(!lst.empty()){
        slot_removewidget(lst.front());
        YaoQingForm* form=lst.front();
        lst.pop_front();
        delete form;
        form=nullptr;
    }
}
