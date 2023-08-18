#include "finddialog.h"
#include "ui_finddialog.h"
#include<QMessageBox>

findDialog::findDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findDialog)
{
    ui->setupUi(this);
    m_layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);  //控件上下左右的间距
    m_layout->setSpacing(3);        //设置每个控件彼此间的距离
    ui->wdg_list->setLayout(m_layout);
//    m_page=1;
}

findDialog::~findDialog()
{
    delete ui;
}

void findDialog::slot_setInfo(int page, int index, QString str)
{
//    m_page=page;
    m_index=index;
    m_str=str;
    /*m_pageNum=pageNum;*/
}

void findDialog::slot_addwidget(UserItem *item,bool isfri)
{
    if(isfri)m_layout->insertWidget(0,item);
    else m_layout->addWidget(item);
    lst.push_back(item);
}

void findDialog::slot_removewidget(UserItem *item)
{
    item->hide();
    m_layout->removeWidget(item);
}

void findDialog::slot_clearwidget()
{
    while(!lst.empty()){
        slot_removewidget(lst.front());
        UserItem* form=lst.front();
        lst.pop_front();
        delete form;
        form=nullptr;
    }
}

void findDialog::on_pb_findUser_clicked()
{
    QString str=ui->le_findUser->text();
    if(str==""){
        QMessageBox::about(this,"提示","输入为空");
        return;
    }
    m_str=str;
    m_index=ui->cmb_findUser->currentIndex();
//    m_page=1;
    Q_EMIT SIG_FindUser(m_str,m_index,1);
}


//void findDialog::on_pb_lastPage_clicked()
//{
//    m_page--;
//    if(m_page<1){
//        QMessageBox::about(this,"提示","已经是第一页");
//        m_page++;
//        return;
//    }
//    Q_EMIT SIG_FindUser(m_str,m_index,m_page);
//}


//void findDialog::on_pb_nextPage_clicked()
//{
//    m_page++;
//    if(m_page>m_pageNum){
//        QMessageBox::about(this,"提示","已经是最后一页");
//        m_page--;
//        return;
//    }
//    Q_EMIT SIG_FindUser(m_str,m_index,m_page);
//}

