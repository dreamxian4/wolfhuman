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

private:
    Ui::mainDialog *ui;
};
#endif // MAINDIALOG_H
