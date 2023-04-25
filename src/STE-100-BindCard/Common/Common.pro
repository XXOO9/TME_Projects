#-------------------------------------------------
#
# Project created by QtCreator 2022-01-18T11:44:06
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Common
TEMPLATE = lib

DEFINES += COMMON_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG( debug, debug|release ){
        DESTDIR     = $$PWD/../bindCard/debug
}

CONFIG( release, debug|release ){
        DESTDIR     = $$PWD/../bindCard/release
}

SOURCES += \
        Common.cpp

HEADERS += \
        Common.h \
        common_global.h  \
        commondefin.h

INCLUDEPATH += ../bindCard/

include($$PWD/HttpClient/HttpClient.pri)
include($$PWD/DevNFC/DevNFC.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}
