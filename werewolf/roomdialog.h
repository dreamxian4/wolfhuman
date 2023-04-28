#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"roomplayerform.h"
#include<map>
#include"uiapi/customwidget.h"

namespace Ui {
class roomDialog;
}

class roomDialog : public CustomMoveDialog
{
    Q_OBJECT

signals:
    void SIG_quitRoom();

public:
    explicit roomDialog(QWidget *parent = nullptr);
    ~roomDialog();
    void slot_addPlayer(QWidget* player,int id);
    void slot_removePlayer(QWidget* player,int id);
    void slot_setInfo(int roomid,int mode,int method,bool lock,
                     QString password,int num);
    void slot_setPlayer(int id,int icon);

private slots:
    void on_pb_min_clicked();

    void on_pb_close_clicked();

    void on_pb_quitroom_clicked();

private:
    Ui::roomDialog *ui;
    QVBoxLayout* m_playerLayoutRight;
    QVBoxLayout* m_playerLayoutLeft;
    std::map<int,roomPlayerform*>m_mapIdToPlayer;
};

#endif // ROOMDIALOG_H
