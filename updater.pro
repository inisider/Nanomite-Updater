#-------------------------------------------------
#
# Project created by QtCreator 2013-05-25T10:03:46
#
#-------------------------------------------------

QT       += core gui
QT += network

MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
OBJECTS_DIR = .obj
DESTDIR = bin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = updater
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    udownloader.cpp

HEADERS  += mainwindow.h \
    settings.h \
    udownloader.h

FORMS    += mainwindow.ui
