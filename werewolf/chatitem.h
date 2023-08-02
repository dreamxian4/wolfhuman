#ifndef CHATITEM_H
#define CHATITEM_H

#include <QWidget>

namespace Ui {
class ChatItem;
}

class ChatItem : public QWidget
{
    Q_OBJECT

signals:
    void SIG_chatItemSend(int);
public:
    explicit ChatItem(QWidget *parent = nullptr);
    ~ChatItem();
    void slot_setChatMsg(QString content);
    void slot_setInfo(QString name,int state,int icon,QString sex,int id);
    QString getname();

private slots:
    void on_pb_chat_clicked();

private:
    Ui::ChatItem *ui;
    int userid;
};

#endif // CHATITEM_H
