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

private:
    Ui::roomPlayerform *ui;
};

#endif // ROOMPLAYERFORM_H
