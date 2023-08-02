#include "spqceform.h"
#include "ui_spaceform.h"

spqceForm::spqceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spqceForm)
{
    ui->setupUi(this);
}

spqceForm::~spqceForm()
{
    delete ui;
}
