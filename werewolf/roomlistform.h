#ifndef ROOMLISTFORM_H
#define ROOMLISTFORM_H

#include <QWidget>

namespace Ui {
class roomListForm;
}

class roomListForm : public QWidget
{
    Q_OBJECT

signals:
    void SIG_joinRoom(int roomid);

public:
    explicit roomListForm(QWidget *parent = nullptr);
    ~roomListForm();
    void setinfo(int mode,int method,int roomid,int count,int current,bool pass,QString passwd,bool state,int level);

private slots:
    void on_pb_joinRoom_clicked();

private:
    void up();

private:
    Ui::roomListForm *ui;
    int     m_mode        ;//模式
    int     m_method      ;//玩法
    int     m_roomid      ;
//    char    roomName[_MAX_SIZE]    ;
    int     m_count       ;
    int     m_currentCou  ;
    bool    m_pass        ;
    QString  m_passwd;
    bool m_state;
    int m_level;
};

#endif // ROOMLISTFORM_H
