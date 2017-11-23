#-------------------------------------------------
#
# Project created by QtCreator 2015-06-01T17:57:53
#
#-------------------------------------------------

QT += core gui xml
QT += webkitwidgets
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReportCompare
TEMPLATE = app

MOC_DIR = .

SOURCES += main.cpp\
            dropfilebutton.cpp \
            genbecreport.cpp \
            comparedialog.cpp \
            menuwindow.cpp \
    idoc.cpp \
    tableelement.cpp \
    opetableelement.cpp \
    enegyplusdoc.cpp \
    icompareelement.cpp \
    htmltutil.cpp \
    becdoc.cpp \
    openstudiodoc.cpp \
    opsgraphelement.cpp

HEADERS  += \
    dropfilebutton.h \
    genbecreport.h \
    comparedialog.h \
    menuwindow.h \
    idoc.h \
    tableelement.h \
    opetableelement.h \
    enegyplusdoc.h \
    icompareelement.h \
    htmltutil.h \
    becdoc.h \
    openstudiodoc.h \
    opsgraphelement.h

FORMS    += \
    comparedialog.ui \
    menuwindow.ui
