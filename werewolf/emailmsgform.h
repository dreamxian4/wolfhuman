#ifndef EMAILMSGFORM_H
#define EMAILMSGFORM_H

#include <QWidget>

namespace Ui {
class emailMsgForm;
}

class emailMsgForm : public QWidget
{
    Q_OBJECT

signals:
    void SIG_AddResult(int,bool);//userid,add

public:
    explicit emailMsgForm(QWidget *parent = nullptr);
    ~emailMsgForm();
    void slot_setInfo(int icon,QString name,int userid,
                      int which,QString spaceCnt,QString msg,bool result);

private slots:
    void on_pb_no_clicked();

    void on_pb_yes_clicked();

private:
    Ui::emailMsgForm *ui;
    int m_userid;
};

#endif // EMAILMSGFORM_H
