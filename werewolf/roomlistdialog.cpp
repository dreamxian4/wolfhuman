 #include "roomlistdialog.h"
#include "ui_roomlistdialog.h"
#include<qDebug>
#include<QMessageBox>

roomListDialog::roomListDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::roomListDialog)
{
    ui->setupUi(this);
    m_userLayout=new QVBoxLayout;
    //用户列表垂直布局
    ui->wdg_roomList->setLayout(m_userLayout);
    m_userLayout->setContentsMargins(0,0,0,0);
    m_userLayout->setSpacing(5);
}


void roomListDialog::slot_addRoom(QWidget *room)
{
    m_userLayout->addWidget(room);
    lst.push_back(room);
}

void roomListDialog::slot_removeRoom(QWidget *room)
{
    room->hide();
    m_userLayout->removeWidget(room);
}

void roomListDialog::slot_setlist(int mode, int method, int roomid, int count, int current,
                                  bool pass, QString passwd, bool state, int level)
{
    roomListForm* room=new roomListForm;
    room->setinfo(mode,method,roomid,count,current,pass,passwd,state,level);
    connect(room,SIGNAL(SIG_joinRoom(int)),
            this,SLOT(slot_dealJoinRoom(int)));
    slot_addRoom(room);
}

roomListDialog::~roomListDialog()
{
    delete ui;
}

void roomListDialog::on_pb_refresh_clicked()
{
    slot_clearList();
    int method=ui->cmb_method->currentIndex();
    int mode=ui->cmb_mode->currentIndex();
    Q_EMIT SIG_REFRESH(method-1,mode-1,0);//-1表示全部，为了和服务端的视频音频序号对应
}


void roomListDialog::on_pb_joinRoom_clicked()
{
    slot_clearList();
    int roomid=ui->le_roomNum->text().toInt();
    Q_EMIT SIG_REFRESH(0,0,roomid);
}

void roomListDialog::slot_clearList()
{
    while(!lst.empty()){
        slot_removeRoom(lst.front());
        lst.pop_front();
    }
}


void roomListDialog::on_pb_min_clicked()
{
    this->showMinimized();
}


void roomListDialog::on_pb_close_clicked()
{
    if( QMessageBox::question( this , "退出提示","确定退出游戏?" ) == QMessageBox::Yes )
    {
         //发送退出登录信号
        Q_EMIT SIG_QUIT();
    }
//    this->slot_close();
}

void roomListDialog::slot_dealJoinRoom(int roomid)
{
    Q_EMIT SIG_joinroom(roomid);
}


void roomListDialog::on_pb_return_clicked()
{
    slot_clearList();
    Q_EMIT SIG_returnMain();
}

