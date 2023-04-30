#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include"uiapi/customwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainDialog; }
QT_END_NAMESPACE

class mainDialog : public CustomMoveDialog
{
    Q_OBJECT

signals:
    void SIG_createRoomButton();
    void SIG_roomListButton(int,int,int);

public:
    mainDialog(QWidget *parent = nullptr);
    ~mainDialog();
    void slot_setInfo(int userid,int icon,QString name,QString sex,QString username,int level);

private slots:
    void on_pb_createRoom_clicked();

    void on_pb_min_clicked();

    void on_pb_close_clicked();

    void on_pb_roomList_clicked();

private:
    Ui::mainDialog *ui;
    int m_userid;
};
#endif // MAINDIALOG_H
