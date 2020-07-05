#-------------------------------------------------
#
# Project created by QtCreator 2020-06-27T00:26:23
#
#-------------------------------------------------

QT       += core gui
QT += multimedia multimediawidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = infotainmentQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


target.path = /home/root/app
INSTALLS += target

SOURCES += \
        main.cpp \
        mainscreen.cpp \
    systeminterface.cpp

HEADERS += \
        mainscreen.h \
    systeminterface.h \
    availabletargets.h \
    targetconfiguration.h

FORMS += \
        mainscreen.ui

RESOURCES += \
    background.qrc \
    icons.qrc \
    musicfiles.qrc
