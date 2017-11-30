#-------------------------------------------------
#
# Project created by QtCreator 2017-08-01T11:01:13
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DominoAPIPorter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dominoapi.cpp \
    third-party/http/httpbase.cpp \
    third-party/http/logger.cpp \
    third-party/http/json.cpp \
    third-party/http/httptool.cpp \
    third-party/http/httpparams.cpp \
    third-party/http/httpbase2.cpp \
    functionutils.cpp

HEADERS  += mainwindow.h \
    dominoapi.h \
    third-party/http/logger.h \
    third-party/http/json.h \
    third-party/http/httptool.h \
    third-party/http/httpparams.h \
    third-party/http/httpbase2.h \
    third-party/http/httpbase.h \
    third-party/json/json.hpp \
    functionutils.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/third-party/soap
INCLUDEPATH += $$PWD/third-party
