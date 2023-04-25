QT += quick qml sql
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG( debug, debug|release ){
        DESTDIR     = $$PWD/debug
        LIBS       += -L$$PWD/debug -lCommon
        LIBS       += -L$$PWD/thirdPart/RFID1356/lib/x64 -lRFID1356
        #LIBS       += $$PWD/../../../thirdparty/opencv/3.4.1/lib -lopencv_world341d
}

CONFIG( release, debug|release ){
        DESTDIR     = $$PWD/release
        LIBS       += -L$$PWD/release -lCommon
}

SOURCES += \
        main.cpp

RESOURCES += qml.qrc

INCLUDEPATH += ../Common/HttpClient
INCLUDEPATH += ./
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

include($$PWD/ProjectEnumMetas.pri)
include($$PWD/AbStractDataModel.pri)
include($$PWD/InterAction.pri)
include($$PWD/CMainFrame.pri)
include($$PWD/DataBase.pri)
include($$PWD/NetWorkManager.pri)
include($$PWD/StudentInfoMgr.pri)
#include($$PWD/DevNFC.pri)

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


