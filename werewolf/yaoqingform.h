#ifndef YAOQINGFORM_H
#define YAOQINGFORM_H

#include <QWidget>

namespace Ui {
class YaoQingForm;
}

class YaoQingForm : public QWidget
{
    Q_OBJECT

signals:
    void SIG_yaoqing(int,int);

public:
    explicit YaoQingForm(QWidget *parent = nullptr);
    ~YaoQingForm();
    void setinfo(int icon,QString name,int id,int roomid);

private slots:
    void on_pb_yao_clicked();

private:
    Ui::YaoQingForm *ui;
    int m_id;
    int m_roomid;
};

#endif // YAOQINGFORM_H
