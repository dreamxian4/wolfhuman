#include "maindialog.h"

#include <QApplication>
#include"ckernel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    mainDialog w;
//    w.show();
    ckernel* ck=ckernel::getCkernel();
    return a.exec();
}
