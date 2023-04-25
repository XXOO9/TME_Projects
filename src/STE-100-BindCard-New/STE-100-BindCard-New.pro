QT += quick core gui network
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/../Common/
INCLUDEPATH += $$PWD/../Common/HttpClient/
INCLUDEPATH += $$PWD/../../thirdparty/RFID1356/inclulde/

CONFIG( debug, debug | release ){
    LIBS += -L$$PWD/../debug -lCommon
    LIBS += -L$$PWD/../../thirdparty/RFID1356/lib/x64/ -lRFID1356
    LIBS += -L$$PWD/../../thirdparty/opencv/lib -lopencv_world452d
    LIBS += -L$$PWD/../../thirdparty/glog/glog-extension/Qt/lib/x64 -lglogd
    LIBS += -L$$PWD/../../thirdparty/Litesql/lib/static/win_x64 -llitesqld
    LIBS += -L$$PWD/../../thirdparty/SeetaFace/lib/debug -lFaceFeatureDetectord

    DESTDIR = $$PWD/bin/debug
}

CONFIG( release, debug | release ){
    LIBS += -L$$PWD/../release -lCommon
    LIBS += -L$$PWD/../../thirdparty/RFID1356/lib/x64/ -lRFID1356
    LIBS += -L$$PWD/../../thirdparty/RFID1356/lib/x64/ -lRFID1356
    LIBS += -L$$PWD/../../thirdparty/opencv/lib -lopencv_world452
    LIBS += -L$$PWD/../../thirdparty/glog/glog-extension/Qt/lib/x64 -lglog
    LIBS += -L$$PWD/../../thirdparty/Litesql/lib/static/win_x64 -llitesql
    LIBS += -L$$PWD/../../thirdparty/SeetaFace/lib/release -lFaceFeatureDetector

    DESTDIR = $$PWD/bin/release
}

HEADERS += \
    CInterAction.h \
    CMainFrame.h \
    DataModel/CUserFilterModel.h \
    DataModel/CUserInfos.h \ \
    NFC/CIdentificDevNFC.h \
    NFC/CNFCThread.h


SOURCES += \
        CInterAction.cpp \
        CMainFrame.cpp \
        DataModel/CUserFilterModel.cpp \
        DataModel/CUserInfos.cpp \
        NFC/CIdentificDevNFC.cpp \
        NFC/CNFCThread.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


