#ifndef SPQCEFORM_H
#define SPQCEFORM_H

#include <QWidget>

namespace Ui {
class spqceForm;
}

class spqceForm : public QWidget
{
    Q_OBJECT

public:
    explicit spqceForm(QWidget *parent = nullptr);
    ~spqceForm();

private:
    Ui::spqceForm *ui;
};

#endif // SPQCEFORM_H
