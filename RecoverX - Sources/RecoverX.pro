#-------------------------------------------------
#
# Project created by QtCreator 2013-02-08T22:28:22
#
#-------------------------------------------------

QT       += core gui
QT       += core gui network
QT       += network
QT       += xml

TARGET = RecoverX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    license.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    license.h

FORMS    += mainwindow.ui \
    dialog.ui \
    license.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources.rc

RC_FILE = resources.rc
