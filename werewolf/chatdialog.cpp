#include "chatdialog.h"
#include "ui_chatdialog.h"
#include<QTime>
#include<QDebug>

ChatDialog::ChatDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);

    //创建菜单对象，指定父控件，父控件负责子控件的回收
    m_menu=new QMenu(this);

    //添加菜单项
    m_menu->addAction("详细信息");
    m_menu->addAction("删除好友");

    //绑定菜单项的点击事件和处理函数
    connect(m_menu,SIGNAL(triggered(QAction*)),
            this,SLOT(slot_dealMenu(QAction*)));
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::setInfo(QString name,int id)
{
    m_name=name;
    m_id=id;
    //设置窗口标题：与【IP】的聊天
    ui->lb_title_2->setText(QString("与%1的聊天").arg(name));
}

//在控件上显示收到的聊天内容
void ChatDialog::slot_setChatMsg(QString content)
{
    ui->tb_chat->append(QString("[%1] %2").arg(m_name).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);
}

//处理聊天对象不在线
void ChatDialog::slot_UserOffline()
{
    //格式：[用户名] 时间 用户不在线
    QString str=QString("[%1] %2 用户不在线").arg(m_name).arg(QTime::currentTime().toString("hh:mm:ss"));
    ui->tb_chat->append(str);
}

void ChatDialog::on_pb_send_clicked()
{
    //1.获取输入的内容QString
    QString content=ui->te_chat->toPlainText();
    if(content.isEmpty()){
        return;
    }
    //2.清空te控件
    ui->te_chat->clear();
    //3.显示到上面tb控件
    //append换行追加
    ui->tb_chat->append(QString("[我] %1").arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);
    //4.发送数据到ckernal类
    Q_EMIT SIG_SendChatMsg(m_id,content);
}

void ChatDialog::on_pb_show_clicked()
{
    //按照鼠标点击的位置显示菜单
    QPoint p=QCursor::pos();
    //获取菜单的绝对高度
    QSize size=m_menu->sizeHint();
    m_menu->exec(QPoint(p.x(),p.y()-size.height()));
}

//点击菜单项的处理函数
void ChatDialog::slot_dealMenu(QAction *action)
{
    if(action->text()=="详细信息"){

    }else if(action->text()=="删除好友"){
        qDebug()<<"删除好友！！";
    }
}

void ChatDialog::slot_getMsg(int id)
{

}

void ChatDialog::slot_HO(bool me, QString time, QString content)
{
    if(me){
        ui->tb_chat->append(QString("[我] %1").arg(time));
        ui->tb_chat->append(content);
    }else{
        ui->tb_chat->append(QString("[%1] %2").arg(m_name).arg(time));
        ui->tb_chat->append(content);
    }
}

void ChatDialog::slot_deleteMsg()
{
    ui->tb_chat->clear();
}

void ChatDialog::on_pb_min_2_clicked()
{
    this->showMinimized();
}


void ChatDialog::on_pb_close_2_clicked()
{
    Q_EMIT SIG_quitChat(m_id);
}


void ChatDialog::on_pb_chatMsg_clicked()
{
    Q_EMIT SIG_GetChatMsg(m_id);
}


void ChatDialog::on_pb_audio_clicked()
{
    Q_EMIT SIG_audioFri(m_id);
}


void ChatDialog::on_pb_video_clicked()
{
    Q_EMIT SIG_videoFri(m_id);
}

