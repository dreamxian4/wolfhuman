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
    void setInfo(int id,bool isme);
    void setImage(QString icon);
    void setZiLiao(int level,QString sex,QString name,int userid,int count);
    void setJing(int jing);
    void setIden(int iden);
    int getUserid();
    QString getJing();
    void setAbleToVoted(bool vote);

private slots:
    void on_pb_icon_clicked();

private:
    Ui::roomPlayerform *ui;
    int         m_level  ;
    QString     m_sex    ;
    QString     m_name   ;
    int         m_userid ;
    int         m_id;
    int m_num;


};

#endif // ROOMPLAYERFORM_H
