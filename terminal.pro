QT += widgets serialport

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    kenwooddvr.cpp \
    dvrprotocol.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    kenwooddvr.h \
    dvrprotocol.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc

DISTFILES += \
    dvr.log
