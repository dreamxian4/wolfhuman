#ifndef SPACEFORM_H
#define SPACEFORM_H

#include <QWidget>
#include"packdef.h"

namespace Ui {
class spaceForm;
}

class spaceForm : public QWidget
{
    Q_OBJECT

signals:
    void SIG_Getdetail(int);//userid
    void SIG_SpaceOpt(int,int,int,int);//spaceid userid 赞/踩 +1/-1
    void SIG_GetComment(int,int,QString,QString,QString,int);//spaceid,iaon,name,time,content,spaceid

public:
    explicit spaceForm(QWidget *parent = nullptr);
    ~spaceForm();
    void setInfo(STRU_SPACE_RS* rs);

private slots:
    void on_pb_icon_clicked();

    void on_pb_good_clicked();

    void on_pb_tui_clicked();

    void on_pb_comment_clicked();

private:
    Ui::spaceForm *ui;
    int userid  ;
    int spaceid ;
    int tui     ;
    int good    ;
    int comment ;
    int isgood;
    int istui;

    int icon;
    QString name;
    QString time;
    QString content;
};

#endif // SPACEFORM_H
