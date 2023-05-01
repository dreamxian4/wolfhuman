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

public:
    explicit roomPlayerform(QWidget *parent = nullptr);
    ~roomPlayerform();
//    void paintEvent(QPaintEvent *event);
    void setInfo(int id,int identify,int jing);
    void setImage(int icon);
    void setZiLiao(int level,QString sex,QString name,int userid);
    int getUserid();

private:
    Ui::roomPlayerform *ui;
    int         m_level  ;
    QString     m_sex    ;
    QString     m_name   ;
    int         m_userid ;
};

#endif // ROOMPLAYERFORM_H
