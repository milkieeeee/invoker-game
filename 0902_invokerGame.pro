#-------------------------------------------------
#
# Project created by QtCreator 2018-09-02T21:59:53
#
#-------------------------------------------------

QT       += core gui
RC_ICONS = td.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 0902_invokerGame
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    xlabel.cpp

HEADERS  += widget.h \
    defineAll.h \
    xlabel.h

FORMS    += widget.ui

RESOURCES += \
    pic.qrc

OTHER_FILES +=
