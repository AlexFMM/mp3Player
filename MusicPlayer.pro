#-------------------------------------------------
#
# Project created by QtCreator 2016-03-30T11:18:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        musica.cpp \
    album.cpp

HEADERS  += mainwindow.h\
            musica.h \
    album.h

FORMS    += mainwindow.ui

QT += core gui multimedia
