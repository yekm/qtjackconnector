#-------------------------------------------------
#
# Project created by QtCreator 2019-07-18T14:48:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtjackconnector
TEMPLATE = app

LIBS += -ljack

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        booleaneditor.cpp \
        jackmodel.cpp \
        jackportstate.cpp \
        main.cpp \
        mainwindow.cpp \
        verticaltextheader.cpp

HEADERS += \
        booleaneditor.h \
        jackmodel.h \
        jackportstate.h \
        mainwindow.h \
        verticaltextheader.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
