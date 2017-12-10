#-------------------------------------------------
#
# Project created by QtCreator 2015-06-01T17:57:53
#
#-------------------------------------------------

QT += core gui xml
CONFIG += c++11
QT += webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReportCompare
TEMPLATE = app

INCLUDEPATH += C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/include/

MOC_DIR = .

SOURCES += main.cpp\
            dropfilebutton.cpp \
            comparedialog.cpp \
            menuwindow.cpp \

HEADERS  += \
    dropfilebutton.h \
    comparedialog.h \
    menuwindow.h

FORMS    += \
    comparedialog.ui \
    menuwindow.ui
