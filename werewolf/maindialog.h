#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class mainDialog; }
QT_END_NAMESPACE

class mainDialog : public QDialog
{
    Q_OBJECT

public:
    mainDialog(QWidget *parent = nullptr);
    ~mainDialog();
    void slot_setInfo(int userid,int icon,QString name,QString sex,QString username);

private:
    Ui::mainDialog *ui;
    int m_userid;
};
#endif // MAINDIALOG_H
