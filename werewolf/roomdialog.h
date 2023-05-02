#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"roomplayerform.h"
#include<map>
#include"uiapi/customwidget.h"
#include<QTimer>
#include<QTimerEvent>

namespace Ui {
class roomDialog;
}

class roomDialog : public CustomMoveDialog
{
    Q_OBJECT

signals:
    void SIG_quitRoom(int);//id是房主的userid
    void SIG_QUIT();
    void SIG_beginGame();

public:
    explicit roomDialog(QWidget *parent = nullptr);
    ~roomDialog();
    void slot_setInfo(int roomid,int mode,int method,bool lock,
                     QString password,int num,int userid);
    void slot_setPlayer(int id,int icon,int level,QString sex,QString name,int userid);
    void slot_destroyRoom();
    void slot_ready();

private slots:
    void on_pb_min_clicked();

    void on_pb_close_clicked();

    void on_pb_quitroom_clicked();
    void slot_addPlayer(QWidget* player,int id);
    void slot_removePlayer(QWidget* player,int id);

    void on_pb_0_begin_clicked();

public:
    void timerEvent(QTimerEvent *e);

private:
    Ui::roomDialog *ui;
    QVBoxLayout* m_playerLayoutRight;
    QVBoxLayout* m_playerLayoutLeft;
    std::map<int,roomPlayerform*>m_mapIdToPlayer;
    int m_seat;
    int m_userid;
    int m_mode        ;//模式
    int m_method      ;//玩法
    int m_roomid      ;
//    char    roomName[_MAX_SIZE]    ;
    int m_count       ;
    int m_currentCou  ;
    bool m_pass        ;
    QString  m_passwd;

    //定时器
    int m_timer_tips;
    int m_timer_ready;

    //用于倒计时
    int num;//5秒
    bool state;//判断是否在倒计时中
};

#endif // ROOMDIALOG_H
