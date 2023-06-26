QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ckernel.cpp \
    createroomform.cpp \
    logindialog.cpp \
    main.cpp \
    maindialog.cpp \
    registerdialog.cpp \
    roomdialog.cpp \
    roomlistdialog.cpp \
    roomlistform.cpp \
    roomplayerform.cpp \
    startdialog.cpp

HEADERS += \
    ckernel.h \
    createroomform.h \
    logindialog.h \
    maindialog.h \
    registerdialog.h \
    roomdialog.h \
    roomlistdialog.h \
    roomlistform.h \
    roomplayerform.h \
    startdialog.h

FORMS += \
    createroomform.ui \
    logindialog.ui \
    maindialog.ui \
    registerdialog.ui \
    roomdialog.ui \
    roomlistdialog.ui \
    roomlistform.ui \
    roomplayerform.ui \
    startdialog.ui

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
