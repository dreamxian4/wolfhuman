#include "userspacedialog.h"
#include "ui_userspacedialog.h"

UserSpaceDialog::UserSpaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSpaceDialog)
{
    ui->setupUi(this);
    m_layout_space=new QVBoxLayout;
    m_layout_space->setContentsMargins(0,0,0,0);
    m_layout_space->setSpacing(3);
    ui->wdg_2_list_2->setLayout(m_layout_space);
}

UserSpaceDialog::~UserSpaceDialog()
{
    delete ui;
}


void UserSpaceDialog::slot_addSpace(spaceForm *spaceform)
{
//    m_layout_space->insertWidget(0,spaceform);
    m_layout_space->addWidget(spaceform);
    spaceLst.push_back(spaceform);
}

void UserSpaceDialog::slot_removeSpace(spaceForm *spaceform)
{
    spaceform->hide();
    m_layout_space->removeWidget(spaceform);
}

void UserSpaceDialog::slot_clearSpace()
{
    while(!spaceLst.empty()){
        slot_removeSpace(spaceLst.front());
        spaceForm* space=spaceLst.front();
        spaceLst.pop_front();
        delete space;
        space=nullptr;
    }
}
