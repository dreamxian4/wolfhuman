#ifndef CHATMSGDIALOG_H
#define CHATMSGDIALOG_H

#include <QDialog>

namespace Ui {
class ChatMsgDialog;
}

class ChatMsgDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_deleteMsg(int);

public:
    explicit ChatMsgDialog(QWidget *parent = nullptr);
    ~ChatMsgDialog();
    void setinfo(QString name,int id);
    void slot_setMsg(bool me, QString time, QString content);

private slots:
    void on_pb_clear_clicked();

private:
    Ui::ChatMsgDialog *ui;
    int m_id;
    QString m_name;
};

#endif // CHATMSGDIALOG_H
