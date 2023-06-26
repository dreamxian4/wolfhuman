#ifndef ROOMPLAYERFORM_H
#define ROOMPLAYERFORM_H

#include <QWidget>
//重写重绘事件
#include<QPaintEvent>
#include<QImage>



namespace Ui {
class roomPlayerform;
}

class roomPlayerform : public QWidget
{
    Q_OBJECT

signals:
    void SIG_click_icon(int id);

public:
    explicit roomPlayerform(QWidget *parent = nullptr);
    ~roomPlayerform();
//    void paintEvent(QPaintEvent *event);
    void setInfo(int id,bool isme);//设置个人信息
    void setImage(QString icon);//设置头像
    void setZiLiao(int level,QString sex,QString name,int userid,int count);//设置详细信息
    void setJing(int jing);//上警等操作
    void setIden(int iden);//设置身份信息
    int getUserid();//获得用户id
    QString getJing();//获得上警情况
    void setAbleToVoted(bool vote);//设置可以被投票
    void resumeVoted();//恢复非投票状态
    void setAbleToOper(int who);//设置可以
    void setSpeak(bool speak);//设置正在讲话

private slots:
    void on_pb_icon_clicked();

private:
    Ui::roomPlayerform *ui;
    int         m_level  ;
    QString     m_sex    ;
    QString     m_name   ;
    int         m_userid ;



public:
    int m_num;//总人数
    bool alive;//是否存活
    bool beKnown;//是否被验过
    int  m_id;//座位号
};

#endif // ROOMPLAYERFORM_H
