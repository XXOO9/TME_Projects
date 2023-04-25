QT += testlib
QT += gui network core widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += qt
TEMPLATE = app

INCLUDEPATH += $$PWD/../Common/HttpClient/

CONFIG( debug, debug | release ){
        DESTDIR = $$PWD/../debug/
        LIBS += -L../../thirdparty/opencv/lib -lopencv_world452d
        LIBS += -L../../thirdparty/glog/glog-extension/Qt/lib/x64 -lglogd
        LIBS += -L../../thirdparty/Litesql/lib/static/win_x64 -llitesqld
        LIBS += -L../../thirdparty/SeetaFace/lib/debug -lFaceFeatureDetectord
        LIBS += -L$$PWD/../debug/ -lCommon
}

CONFIG( release, debug | release ){
        DESTDIR = $$PWD/../release/
        LIBS += -L$$PWD/../../thirdparty/opencv/lib -lopencv_world452
        LIBS += -L$$PWD/../../thirdparty/RFID1356/lib/x64 -lRFID1356
        LIBS += -L$$PWD/../../thirdparty/Litesql/lib/static/win_x64 -llitesql
        LIBS += -L$$PWD/../../thirdparty/SeetaFace/lib/release -lFaceFeatureDetector
        LIBS += -L$$PWD/../release/ -lCommon
}

SOURCES += CHttpTest.cpp \
    main.cpp

HEADERS += \
    CHttpTest.h
