#ifndef ADDSPACEDIALOG_H
#define ADDSPACEDIALOG_H

#include <QDialog>

namespace Ui {
class addSpaceDialog;
}

class addSpaceDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_sendSpace(QString);

public:
    explicit addSpaceDialog(QWidget *parent = nullptr);
    ~addSpaceDialog();
    void slot_clear();

private slots:
    void on_pb_send_clicked();

    void on_pb_cancel_clicked();

private:
    Ui::addSpaceDialog *ui;
};

#endif // ADDSPACEDIALOG_H
