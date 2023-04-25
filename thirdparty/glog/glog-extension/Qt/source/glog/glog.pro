# 指定构建模板
TEMPLATE = lib

# 指定目标文件名
CONFIG(debug, debug|release){
   TARGET = glogd
} else {
   TARGET = glog
}

# 配置信息
CONFIG += staticlib

# 指定目标生成路径
DESTDIR = ../lib

INCLUDEPATH += ./windows

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS HAVE_SNPRINTF HAVE_SNPRINTF GLOG_NO_ABBREVIATED_SEVERITIES GOOGLE_GLOG_DLL_DECL=

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
           base/mutex.h \
           base/commandlineflags.h  \
           base/googleinit.h        \
           windows/config.h \
           windows/port.h \
           windows/glog/log_severity.h \
           windows/glog/logging.h \
           windows/glog/raw_logging.h \
           windows/glog/stl_logging.h \
           windows/glog/vlog_is_on.h \
           symbolize.h \
           utilities.h \
           demangle.h

SOURCES += demangle.cc \
           raw_logging.cc \
           logging.cc \
           signalhandler.cc \
           symbolize.cc \
           utilities.cc \
           windows/port.cc \
           vlog_is_on.cc


# Default rules for deployment.
unix {
    target.path = ../lib
	INSTALLS += target
}
!isEmpty(target.path): INSTALLS += target
