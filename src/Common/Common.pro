#-------------------------------------------------
#
# Project created by QtCreator 2022-01-14T14:18:30
#
#-------------------------------------------------

QT       += core widgets texttospeech concurrent network multimedia serialport qml quick

TARGET = Common
TEMPLATE = lib

QMAKE_LFLAGS_RELEASE += /MAP
QMAKE_CFLAGS_RELEASE += /Zi
QMAKE_LFLAGS_RELEASE += /debug /opt:ref
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

DEFINES += COMMON_LIBRARY

#指定生成的应用程序放置的目录
CONFIG(debug,debug|release){
DESTDIR = ../debug
}

CONFIG(release,debug|release){
DESTDIR = ../release
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS GLOG_NO_ABBREVIATED_SEVERITIES GOOGLE_GLOG_DLL_DECL=

DISTFILES += \
    HttpClient/HttpClient.pri \
    HttpClient/readme.txt

include($$PWD/HttpClient/HttpClient.pri)

#包含库目录
INCLUDEPATH +=  ../../thirdparty/opencv/include \
                ../../thirdparty/glog/glog-extension/Qt/include \
                ../../thirdparty/Litesql/include \

#引入第三方库文件
CONFIG(debug, debug|release){
    LIBS += -L../../thirdparty/opencv/lib -lopencv_world452d
    LIBS += -L../../thirdparty/glog/glog-extension/Qt/lib/x64 -lglogd
    LIBS += -L../../thirdparty/Litesql/lib/static/win_x64 -llitesqld
    LIBS += -L../../thirdparty/SeetaFace/lib/debug -lFaceFeatureDetectord
} else {
    LIBS += -L../../thirdparty/opencv/lib -lopencv_world452
    LIBS += -L../../thirdparty/glog/glog-extension/Qt/lib/x64 -lglog
    LIBS += -L../../thirdparty/Litesql/lib/static/win_x64 -llitesql
    LIBS += -L../../thirdparty/SeetaFace/lib/release -lFaceFeatureDetector
}


SOURCES += \
        CCamerRegister.cpp \
        CDataSynchronize.cpp \
        CDevCamera.cpp \
        CFilePath.cpp \
        CHttpConnection.cpp \
        CHttpDownloadFromUrl.cpp \
        CHttpDownloadThread.cpp \
        CImage.cpp \
        CSignPainterCommandDefin.cpp \
        CStdString.cpp \
        CTextToSpeech.cpp \
        CThreadResourceCtrlBase.cpp \
        Common.cpp \
        Config/CAbstractConfigInfo.cpp \
        Config/CCameraConfig.cpp \
        Config/CClassInfo.cpp \
        Config/CCommitPopupConfig.cpp \
        Config/CCommonCfg.cpp \
        Config/CConfig_Version.cpp \
        Config/CDatabaseManage.cpp \
        Config/CDatabase_Checker.cpp \
        Config/CDevHWInfoConfig.cpp \
        Config/CDevSWInfoConfig.cpp \
        Config/CDevTestInfoConfig.cpp \
        Config/CGrade.cpp \
        Config/CScoreRule.cpp \
        Config/CServerFunConfig.cpp \
        Config/CTableClassInfo.cpp \
        Config/CTableCommitPopupConfig.cpp \
        Config/CTableDataSyncTime.cpp \
        Config/CTableScoreRules.cpp \
        Config/CTableTestItemInfo.cpp \
        Config/CTableTestItemResult.cpp \
        Config/CTableUserIdentityInfo.cpp \
        Config/CTestProcessVideoPathConfig.cpp \
        Config/CTestResultInfo.cpp \
        Config/CTestRunConfig.cpp \
        Config/CUserInfo.cpp \
        Config/ste100database.cpp \
        DshowCameraInfo.cpp \
        Logger/CGlog.cpp \
        Logger/CLogger.cpp \
        common_error.cpp

HEADERS += \
        CCamerRegister.h \
        CDataSynchronize.h \
        CDevCamera.h \
        CFilePath.h \
        CHttpConnection.h \
        CHttpDownloadFromUrl.h \
        CHttpDownloadThread.h \
        CImage.h \
        CSignPainterCommandDefin.h \
        CStdString.h \
        CTextToSpeech.h \
        CThreadResourceCtrlBase.h \
        Common.h \
        Config/CAbstractConfigInfo.h \
        Config/CCameraConfig.h \
        Config/CClassInfo.h \
        Config/CCommitPopupConfig.h \
        Config/CCommonCfg.h \
        Config/CConfig_Version.h \
        Config/CDatabaseManage.h \
        Config/CDatabase_Checker.h \
        Config/CDevHWInfoConfig.h \
        Config/CDevSWInfoConfig.h \
        Config/CDevTestInfoConfig.h \
        Config/CGrade.h \
        Config/CScoreRule.h \
        Config/CServerFunConfig.h \
        Config/CTableClassInfo.h \
        Config/CTableCommitPopupConfig.h \
        Config/CTableDataSyncTime.h \
        Config/CTableScoreRules.h \
        Config/CTableTestItemInfo.h \
        Config/CTableTestItemResult.h \
        Config/CTableUserIdentityInfo.h \
        Config/CTestProcessVideoPathConfig.h \
        Config/CTestResultInfo.h \
        Config/CTestRunConfig.h \
        Config/CUserInfo.h \
        Config/ste100database.hpp \
        DshowCameraInfo.h \
        IImageProBasic.h \
        Logger/CGlog.h \
        Logger/CLogger.h \
        Logger/ILog.h \
        common_error.h \
        common_global.h  \
        commondefin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
