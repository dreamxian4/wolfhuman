#ifndef COMMENTFORM_H
#define COMMENTFORM_H

#include <QWidget>

namespace Ui {
class commentForm;
}

class commentForm : public QWidget
{
    Q_OBJECT

public:
    explicit commentForm(QWidget *parent = nullptr);
    ~commentForm();
    void slot_setInfo(QString name,QString time,QString content);

private:
    Ui::commentForm *ui;
};

#endif // COMMENTFORM_H
