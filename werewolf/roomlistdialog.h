#ifndef ROOMLISTDIALOG_H
#define ROOMLISTDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"roomlistform.h"
#include<QList>
#include"uiapi/customwidget.h"

namespace Ui {
class roomListDialog;
}

class roomListDialog : public CustomMoveDialog
{
    Q_OBJECT

signals:
    void SIG_REFRESH(int,int,int);
    void SIG_joinroom(int);

public:
    explicit roomListDialog(QWidget *parent = nullptr);
    ~roomListDialog();

    void slot_addRoom(QWidget* room);
    void slot_removeRoom(QWidget* room);
    void slot_setlist(int mode, int method, int roomid, int count,
                               int current, bool pass, QString passwd, bool state, int level);

private slots:
    void on_pb_refresh_clicked();

    void on_pb_joinRoom_clicked();
    void slot_clearList();

    void on_pb_min_clicked();

    void on_pb_close_clicked();
    void slot_dealJoinRoom(int roomid);

private:
    Ui::roomListDialog *ui;
    QVBoxLayout* m_userLayout;
    QList<QWidget*>lst;
};

#endif // ROOMLISTDIALOG_H
