#include "maindialog.h"
#include "ui_maindialog.h"

mainDialog::mainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::mainDialog)
{
    ui->setupUi(this);
}

mainDialog::~mainDialog()
{
    delete ui;
}

void mainDialog::slot_setInfo(int userid, int icon, QString name, QString sex, QString username)
{
    //用户名设置 我的->个人信息
    m_userid=userid;
    ui->lb_name->setText(name);
    //头像和性别都是设置图片
}


void mainDialog::on_pb_createRoom_clicked()
{
    //发送信号、弹出房间配置界面
    Q_EMIT SIG_createRoomButton();
}

