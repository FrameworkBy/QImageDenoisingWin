#-------------------------------------------------
#
# Project created by QtCreator 2015-05-27T02:59:12
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QImageDenoisingWin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nlm_filter_gray.cpp \
    nlm_filter_gray2.cpp \
    utils.cpp \
    awgn.cpp \
    noise.cpp \
    nlm_filter_gray3.cpp

HEADERS  += mainwindow.h \
    nlm_filter_gray.h \
    nlm_filter_gray2.h \
    utils.h \
    awgn.h \
    noise.h \
    nlm_filter_gray3.h

FORMS    += mainwindow.ui
