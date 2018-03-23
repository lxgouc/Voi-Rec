#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T11:43:23
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Voi-Rec
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    speech_recognizer.cpp \
    linuxrec.cpp \
    iatwork.cpp \
    ttswork.cpp

HEADERS  += widget.h \
    speech_recognizer.h \
    formats.h \
    linuxrec.h \
    iatwork.h \
    ttswork.h

FORMS    += widget.ui

INCLUDEPATH    += ./include

LIBS    += -L/home/lxg/workspace/Voi-Rec/libs/x64 -lmsc \
           -lasound
