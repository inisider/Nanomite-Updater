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
    uupdatewidget.cpp \
    usettingsreader.cpp \
    ucheckupdateswidget.cpp \
    uupdatewidget1.cpp \
    updateitem.cpp \
    uupdatesmodel.cpp \
    uupdatestableview.cpp \
    ufiledownloader.cpp

HEADERS  += \
    uupdatewidget.h \
    usettingsreader.h \
    ucheckupdateswidget.h \
    uupdatewidget1.h \
    updateitem.h \
    uupdatesmodel.h \
    uupdatestableview.h \
    ufiledownloader.h

FORMS    += \
    uupdatewidget.ui \
    ucheckupdateswidget.ui \
    uupdatewidget1.ui
