#-------------------------------------------------
#
# Project created by QtCreator 2016-03-30T11:18:09
#
#--------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        musica.cpp \
        album.cpp \
    	addalbumform.cpp \
        addmusicform.cpp \
        editinfo.cpp \
        config.cpp \
    playlist.cpp \
    addplaylistform.cpp

HEADERS  += mainwindow.h\
            musica.h \
            album.h \
    		addalbumform.h \
            addmusicform.h \
            editinfo.h \
            config.h \
    playlist.h \
    addplaylistform.h

FORMS    += mainwindow.ui \
            addalbumform.ui \
            addmusicform.ui \
            editinfo.ui \
            config.ui \
    addplaylistform.ui

QT += core gui multimedia sql

DISTFILES +=
