#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui {
class startDialog;
}

class startDialog : public QDialog
{
    Q_OBJECT

public:
    explicit startDialog(QWidget *parent = nullptr);
    ~startDialog();

signals:
    void SIG_showNotice();
    void SIG_joinGame();

private slots:
    void on_pb_notice_clicked();

    void on_pb_joinGame_clicked();

private:
    Ui::startDialog *ui;
};

#endif // STARTDIALOG_H
