#ifndef YAOQINGFRIDIALOG_H
#define YAOQINGFRIDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"yaoqingform.h"

namespace Ui {
class YaoQingFriDialog;
}

class YaoQingFriDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YaoQingFriDialog(QWidget *parent = nullptr);
    ~YaoQingFriDialog();
    void slot_setInfo(int roomid);
    void slot_addwidget(YaoQingForm* item);
    void slot_removewidget(YaoQingForm* item);
    void slot_clearwidget();

private:
    Ui::YaoQingFriDialog *ui;
    QVBoxLayout* m_layout;
    QList<YaoQingForm*>lst;
};

#endif // YAOQINGFRIDIALOG_H
