QT += core
QT += serialport
#concurrent
TARGET = Communication
TEMPLATE = lib
CONFIG += staticlib

QMAKE_LFLAGS_RELEASE += /MAP
QMAKE_CFLAGS_RELEASE += /Zi
QMAKE_LFLAGS_RELEASE += /debug /opt:ref
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

CONFIG(debug,debug|release){
#指定生成的应用程序放置的目录
DESTDIR = ../debug
}

CONFIG(release,debug|release){
DESTDIR = ../release
}

CONFIG(debug, debug|release){
    LIBS += -L../../thirdparty/glog/glog-extension/Qt/lib/x64 -lglogd
    LIBS += -L../debug -lCommon
} else {
    LIBS += -L../../thirdparty/glog/glog-extension/Qt/lib/x64 -lglog
    LIBS += -L../release -lCommon
}

INCLUDEPATH +=  ../Common
INCLUDEPATH +=  ../Common/Logger

HEADERS += \
    CBluetoothCom.h \
    CCycleQueue.h \
    CDataVerify.h \
    CNFCCom.h \
    CNetworkCom.h \
    CSerialPortCom.h \
    CSerialPortHelper.h \
    CUSBCom.h \
    Communication_global.h \
    HardwareConstantDefine.h \
    IComHelper.h \
    ICommunicate.h \
    IProductRegistrar.h \
    ProductFactory.h \
    ProductRegistrar.h

SOURCES += \
    CBluetoothCom.cpp \
    CComFactory.cpp \
    CCycleQueue.cpp \
    CDataVerify.cpp \
    CNFCCom.cpp \
    CNetworkCom.cpp \
    CSerialPortCom.cpp \
    CSerialPortHelper.cpp \
    CUSBCom.cpp
