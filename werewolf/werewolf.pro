QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addspacedialog.cpp \
    chatdialog.cpp \
    chatitem.cpp \
    chatmsgdialog.cpp \
    ckernel.cpp \
    commentdialog.cpp \
    commentform.cpp \
    createroomform.cpp \
    emaildialog.cpp \
    emailmsgform.cpp \
    finddialog.cpp \
    logindialog.cpp \
    main.cpp \
    maindialog.cpp \
    mychatdialog.cpp \
    registerdialog.cpp \
    roomdialog.cpp \
    roomlistdialog.cpp \
    roomlistform.cpp \
    roomplayerform.cpp \
    spaceform.cpp \
    startdialog.cpp \
    useritem.cpp \
    usershow.cpp \
    userspacedialog.cpp \
    videoaudiodialog.cpp \
    yaoqingform.cpp \
    yaoqingfridialog.cpp \
    ziliaodialog.cpp

HEADERS += \
    addspacedialog.h \
    chatdialog.h \
    chatitem.h \
    chatmsgdialog.h \
    ckernel.h \
    commentdialog.h \
    commentform.h \
    createroomform.h \
    emaildialog.h \
    emailmsgform.h \
    finddialog.h \
    logindialog.h \
    maindialog.h \
    mychatdialog.h \
    registerdialog.h \
    roomdialog.h \
    roomlistdialog.h \
    roomlistform.h \
    roomplayerform.h \
    spaceform.h \
    startdialog.h \
    useritem.h \
    usershow.h \
    userspacedialog.h \
    videoaudiodialog.h \
    yaoqingform.h \
    yaoqingfridialog.h \
    ziliaodialog.h

FORMS += \
    addspacedialog.ui \
    chatdialog.ui \
    chatitem.ui \
    chatmsgdialog.ui \
    commentdialog.ui \
    commentform.ui \
    createroomform.ui \
    emaildialog.ui \
    emailmsgform.ui \
    finddialog.ui \
    logindialog.ui \
    maindialog.ui \
    mychatdialog.ui \
    registerdialog.ui \
    roomdialog.ui \
    roomlistdialog.ui \
    roomlistform.ui \
    roomplayerform.ui \
    spaceform.ui \
    startdialog.ui \
    useritem.ui \
    usershow.ui \
    userspacedialog.ui \
    videoaudiodialog.ui \
    yaoqingform.ui \
    yaoqingfridialog.ui \
    ziliaodialog.ui

include(./netapi/netapi.pri)
include(./uiapi/uiapi.pri)
include(./MD5/md5.pri)
include(./AudioApi/audioapi.pri)
include(./VideoApi/videoapi.pri)

INCLUDE += \
    ./netapi \
    ./uiapi \
    ./MD5 \
    ./identify\
    ./AudioApi\
    ./VideoApi

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

QT +=sql
