QT += core

INCLUDEPATH = $$PWD/SteDevManager/

HEADERS += \
    $$PWD/CAbstractSettingElement.h \
    $$PWD/CDevSettingConfig.h \
    $$PWD/CHostSettingConfig.h \
    $$PWD/CSystemSettingMgr.h \
    $$PWD/CUsingSettingConfig.h

SOURCES += \
    $$PWD/CAbstractSettingElement.cpp \
    $$PWD/CDevSettingConfig.cpp \
    $$PWD/CHostSettingConfig.cpp \
    $$PWD/CSystemSettingMgr.cpp \
    $$PWD/CUsingSettingConfig.cpp
