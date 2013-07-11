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

TARGET = updater1
TEMPLATE = app


SOURCES += main.cpp\
    usettingsreader.cpp \
    ucheckupdateswidget.cpp \
    updateitem.cpp \
    uupdatesmodel.cpp \
    uupdatestableview.cpp \
    ufiledownloader.cpp \
    uupdatewidget.cpp

HEADERS  += \
    usettingsreader.h \
    ucheckupdateswidget.h \
    updateitem.h \
    uupdatesmodel.h \
    uupdatestableview.h \
    ufiledownloader.h \
    uupdatewidget.h

FORMS    += \
    ucheckupdateswidget.ui \
    uupdatewidget.ui
