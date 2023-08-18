#ifndef USERSPACEDIALOG_H
#define USERSPACEDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"spaceform.h"


namespace Ui {
class UserSpaceDialog;
}

class UserSpaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserSpaceDialog(QWidget *parent = nullptr);
    ~UserSpaceDialog();
    void slot_clearSpace();
    void slot_addSpace(spaceForm* spaceform);
    void slot_removeSpace(spaceForm* spaceform);

private:
    Ui::UserSpaceDialog *ui;
    QVBoxLayout* m_layout_space;
    QList<spaceForm*>spaceLst;
};

#endif // USERSPACEDIALOG_H
