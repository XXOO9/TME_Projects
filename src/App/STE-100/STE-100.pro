QT += texttospeech
QT += qml core quick serialport multimedia network charts printsupport

QT += virtualkeyboard

CONFIG += c++17
CONFIG += resources_big
#CONFIG += console



TARGET = STE-100

static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_LFLAGS_RELEASE += /MAP
QMAKE_CFLAGS_RELEASE += /Zi
QMAKE_LFLAGS_RELEASE += /debug /opt:ref
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/CProjectEnums.pri)
include($$PWD/SystemSettingConfig.pri)


INCLUDEPATH +=  ../../../thirdparty/RFID1356/include \
                ../../../thirdparty/Litesql/include \
                ../../Common \
                ../../Common/HttpClient \
                ../../Common/Config \
                ../../Common/Logger \
                ../../../thirdparty/opencv/include \
                ../../Communication \
                ../../Core/VirtualDevice\
                ./CustomControl \
                ./UserIdentification \
                ./SteDevManager \

CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../../../thirdparty/opencv/lib -lopencv_world452d
    LIBS += -L$$PWD/../../../thirdparty/RFID1356/lib/x64 -lRFID1356
    LIBS += -L$$PWD/../../../thirdparty/Litesql/lib/static/win_x64 -llitesqld
    LIBS += -L$$PWD/../../../thirdparty/SeetaFace/lib/debug -lFaceFeatureDetectord
    LIBS += -L$$PWD/../../debug/ -lCommon
    LIBS += -L$$PWD/../../debug/ -lCore
    LIBS += -L$$PWD/../../debug/ -lCommunication

    DESTDIR = $$PWD/../../debug
} else {
    LIBS += -L$$PWD/../../../thirdparty/opencv/lib -lopencv_world452
    LIBS += -L$$PWD/../../../thirdparty/RFID1356/lib/x64 -lRFID1356
    LIBS += -L$$PWD/../../../thirdparty/Litesql/lib/static/win_x64 -llitesql
    LIBS += -L$$PWD/../../../thirdparty/SeetaFace/lib/release -lFaceFeatureDetector
    LIBS += -L$$PWD/../../release/ -lCommon
    LIBS += -L$$PWD/../../release/ -lCore
    LIBS += -L$$PWD/../../release/ -lCommunication

    DESTDIR = $$PWD/../../release


}

SOURCES += \
        Analysis/CAlgorithm.cpp \
        Analysis/CAnalysisHBC.cpp \
        Analysis/CAnalysisLungFunc.cpp \
        Analysis/CoefFactor.c \
        AssistDevManager/CAssistDevManage.cpp \
        AssistDevManager/CConfirmDevRun.cpp \
        AssistDevManager/CDataParseRunNFC.cpp \
        AssistDevManager/CDataParseStartingGun.cpp \
        AssistDevManager/CDevStartingGun.cpp \
        AssistDevManager/CIdentificDevNFC.cpp \
        AssistDevManager/FoulControl/CConfirmDevFoulControl.cpp \
        AssistDevManager/FoulControl/CDataParseFoulControl.cpp \
        AssistDevManager/FoulControl/CDevFoulControl.cpp \
        AssistDevManager/PowerUart/CConfirmDevPowerUart.cpp \
        AssistDevManager/PowerUart/CDataParsePowerUart.cpp \
        AssistDevManager/PowerUart/CDevPowerUart.cpp \
        AssistDevManager/PowerUart/PowerUartApplication.c \
        AssistDevManager/PowerUart/PowerUartBaseDefine.c \
        AssistDevManager/PowerUart/PowerUartDataLink.c \
        AssistDevManager/PowerUart/PowerUartFifo8.c \
        AssistDevManager/RunNFC/CDevRunNFC.cpp \
        CCamerInfoForTest.cpp \
        CCommonDataModel.cpp \
        CConfirmDevBase.cpp \
        CCurStudentTestRet.cpp \
        CDataParseBase.cpp \
        CDevDataCfg.cpp \
        CDeviceBase.cpp \
        CEnumSerialPort.cpp \
        CGenerationCommand.cpp \
        CHightPerformanceTimeCounter.cpp \
        CImageProvider.cpp \
        CInterAction.cpp \
        CMainFrame.cpp \
        CPhysicalMeasureDataManage.cpp \
        CResoureModule.cpp \
        CScoreRuleInfo.cpp \
        CServerCommunication.cpp \
        CSingleApplication.cpp \
        CSoftMachineMode.cpp \
        CTestStateManage.cpp \
        CustomControl/CCustFVChartView.cpp \
        CustomControl/CCustRTChartView.cpp \
        CustomControl/CCustVTChartView.cpp \
        CustomControl/CCustomCurveView.cpp \
        CustomControl/COpencvVideoControl.cpp \
        CustomControl/COpencvVideoPlayer.cpp \
        CustomControl/qcustomplot.cpp \
        SteDevManager/CDevSteFactory.cpp \
        SteDevManager/CSteDeviceManager.cpp \
        SteDevManager/SteDevice/CDevHardwareBase.cpp \
        SteDevManager/SteDevice/CDevSteBase.cpp \
        SteDevManager/SteDevice/CDevSteBasketball.cpp \
        SteDevManager/SteDevice/CDevSteBody.cpp \
        SteDevManager/SteDevice/CDevSteEyesight.cpp \
        SteDevManager/SteDevice/CDevSteFootball.cpp \
        SteDevManager/SteDevice/CDevSteGrip.cpp \
        SteDevManager/SteDevice/CDevStePUO.cpp \
        SteDevManager/SteDevice/CDevStePVV.cpp \
        SteDevManager/SteDevice/CDevStePullUp.cpp \
        SteDevManager/SteDevice/CDevStePushup.cpp \
        SteDevManager/SteDevice/CDevSteRunMid.cpp \
        SteDevManager/SteDevice/CDevSteRunShort.cpp \
        SteDevManager/SteDevice/CDevSteSBJ.cpp \
        SteDevManager/SteDevice/CDevSteSFB.cpp \
        SteDevManager/SteDevice/CDevSteSitUp.cpp \
        SteDevManager/SteDevice/CDevSteSkip.cpp \
        SteDevManager/SteDevice/CDevSteSolidball.cpp \
        SteDevManager/SteDevice/CDevSteVC.cpp \
        SteDevManager/SteDevice/CDevSteVertivalJump.cpp \
        SteDevManager/SteDevice/CDevSteVolleyball.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementBase.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementBasketball.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementBody.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementEyesight.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementFootball.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementGrip.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementPUO.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementPVV.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementPullUp.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementPushup.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementRopeSkip.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementRunMid.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementRunShort.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementSBJ.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementSFB.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementSitUp.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementSolidball.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementVC.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementVerticalJump.cpp \
        SteDevManager/SteDevice/SteSubElement/CSubElementVolleyball.cpp \
        SteDevManager/SteDevice/SteSubElement/Error/DevExcept.cpp \
        SteTestGuide/CEFV.cpp \
        SteTestGuide/CTestGuideBase.cpp \
        SteTestGuide/CTestGuideBasketball.cpp \
        SteTestGuide/CTestGuideBody.cpp \
        SteTestGuide/CTestGuideEyesight.cpp \
        SteTestGuide/CTestGuideFootball.cpp \
        SteTestGuide/CTestGuideGrip.cpp \
        SteTestGuide/CTestGuideManager.cpp \
        SteTestGuide/CTestGuidePUO.cpp \
        SteTestGuide/CTestGuidePVV.cpp \
        SteTestGuide/CTestGuidePullup.cpp \
        SteTestGuide/CTestGuidePushup.cpp \
        SteTestGuide/CTestGuideRunMid.cpp \
        SteTestGuide/CTestGuideRunShort.cpp \
        SteTestGuide/CTestGuideSBJ.cpp \
        SteTestGuide/CTestGuideSFB.cpp \
        SteTestGuide/CTestGuideSitUp.cpp \
        SteTestGuide/CTestGuideSkip.cpp \
        SteTestGuide/CTestGuideSolidball.cpp \
        SteTestGuide/CTestGuideVC.cpp \
        SteTestGuide/CTestGuideVerticalJump.cpp \
        SteTestGuide/CTestGuideVolleyball.cpp \
        TestProc/CTestProcBCI.cpp \
        TestProc/CTestProcBasic.cpp \
        TestProc/CTestProcBasketball.cpp \
        TestProc/CTestProcEyesight.cpp \
        TestProc/CTestProcFootball.cpp \
        TestProc/CTestProcGrip.cpp \
        TestProc/CTestProcHeight.cpp \
        TestProc/CTestProcManage.cpp \
        TestProc/CTestProcPUO.cpp \
        TestProc/CTestProcPVV.cpp \
        TestProc/CTestProcPullUp.cpp \
        TestProc/CTestProcPushup.cpp \
        TestProc/CTestProcRun1000.cpp \
        TestProc/CTestProcRun50.cpp \
        TestProc/CTestProcRun800.cpp \
        TestProc/CTestProcSBJ.cpp \
        TestProc/CTestProcSFB.cpp \
        TestProc/CTestProcSitUp.cpp \
        TestProc/CTestProcSkip.cpp \
        TestProc/CTestProcSolidball.cpp \
        TestProc/CTestProcVC.cpp \
        TestProc/CTestProcVerticalJump.cpp \
        TestProc/CTestProcVolleyball.cpp \
        TestProc/CTestProcWeight.cpp \
        TestProcAntiCheating/CTestProcAntiCheatingBasic.cpp \
        TestProcAntiCheating/CTestProcAntiCheatingPullUp.cpp \
        TestProcAntiCheating/CTestProcAntiCheatingSitSBJ.cpp \
        TestProcAntiCheating/CTestProcAntiCheatingSitSFB.cpp \
        TestProcAntiCheating/CTestProcAntiCheatingSitUp.cpp \
        TestProcAntiCheating/CTestProcAntiCheatingVC.cpp \
        UserIdentification/CFaceRecognition.cpp \
        UserIdentification/CFindUserBasic.cpp \
        UserIdentification/CFindUserByFaceFeature.cpp \
        UserIdentification/CFindUserByMagneticCard.cpp \
        UserIdentification/CFindUserByStudentID.cpp \
        UserIdentification/CIdentifyUser.cpp \
        UserManger/CUserManage.cpp \
        UserManger/CUserRoleManage.cpp \
        main.cpp

RESOURCES += qml.qrc \
    keyBoardStyle/virtualkeyboard_default_style.qrc \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Analysis/CAlgorithm.h \
    Analysis/CAnalysisHBC.h \
    Analysis/CAnalysisLungFunc.h \
    Analysis/CoefFactor.h \
    AssistDevManager/CAssistDevManage.h \
    AssistDevManager/CConfirmDevRun.h \
    AssistDevManager/CDataParseRunNFC.h \
    AssistDevManager/CDataParseStartingGun.h \
    AssistDevManager/CDevStartingGun.h \
    AssistDevManager/CIdentificDevNFC.h \
    AssistDevManager/FoulControl/CConfirmDevFoulControl.h \
    AssistDevManager/FoulControl/CDataParseFoulControl.h \
    AssistDevManager/FoulControl/CDevFoulControl.h \
    AssistDevManager/PowerUart/CConfirmDevPowerUart.h \
    AssistDevManager/PowerUart/CDataParsePowerUart.h \
    AssistDevManager/PowerUart/CDevPowerUart.h \
    AssistDevManager/PowerUart/PowerUartApplication.h \
    AssistDevManager/PowerUart/PowerUartBaseDefine.h \
    AssistDevManager/PowerUart/PowerUartDataLink.h \
    AssistDevManager/PowerUart/PowerUartFifo8.h \
    AssistDevManager/RunNFC/CDevRunNFC.h \
    CCamerInfoForTest.h \
    CCommonDataModel.h \
    CConfirmDevBase.h \
    CCurStudentTestRet.h \
    CDataParseBase.h \
    CDevDataCfg.h \
    CDeviceBase.h \
    CEnumSerialPort.h \
    CGenerationCommand.h \
    CHightPerformanceTimeCounter.h \
    CImageProvider.h \
    CInterAction.h \
    CMainFrame.h \
    CPhysicalMeasureDataManage.h \
    CResoureModule.h \
    CScoreRuleInfo.h \
    CServerCommunication.h \
    CSingleApplication.h \
    CSoftMachineMode.h \
    CTestStateManage.h \
    CustomControl/CCustFVChartView.h \
    CustomControl/CCustRTChartView.h \
    CustomControl/CCustVTChartView.h \
    CustomControl/CCustomCurveView.h \
    CustomControl/COpencvVideoControl.h \
    CustomControl/COpencvVideoPlayer.h \
    CustomControl/qcustomplot.h \
    SteDevManager/CDevSteFactory.h \
    SteDevManager/CSteDeviceManager.h \
    SteDevManager/SteDevice/CDevHardwareBase.h \
    SteDevManager/SteDevice/CDevSteBase.h \
    SteDevManager/SteDevice/CDevSteBasketball.h \
    SteDevManager/SteDevice/CDevSteBody.h \
    SteDevManager/SteDevice/CDevSteEyesight.h \
    SteDevManager/SteDevice/CDevSteFootball.h \
    SteDevManager/SteDevice/CDevSteGrip.h \
    SteDevManager/SteDevice/CDevStePUO.h \
    SteDevManager/SteDevice/CDevStePVV.h \
    SteDevManager/SteDevice/CDevStePullUp.h \
    SteDevManager/SteDevice/CDevStePushup.h \
    SteDevManager/SteDevice/CDevSteRunMid.h \
    SteDevManager/SteDevice/CDevSteRunShort.h \
    SteDevManager/SteDevice/CDevSteSBJ.h \
    SteDevManager/SteDevice/CDevSteSFB.h \
    SteDevManager/SteDevice/CDevSteSitUp.h \
    SteDevManager/SteDevice/CDevSteSkip.h \
    SteDevManager/SteDevice/CDevSteSolidball.h \
    SteDevManager/SteDevice/CDevSteVC.h \
    SteDevManager/SteDevice/CDevSteVertivalJump.h \
    SteDevManager/SteDevice/CDevSteVolleyball.h \
    SteDevManager/SteDevice/CSteCommonDefin.h \
    SteDevManager/SteDevice/SteSubElement/CElementStruct.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementBase.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementBasketball.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementBody.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementEyesight.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementFootball.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementGrip.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementPUO.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementPVV.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementPullUp.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementPushup.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementRopeSkip.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementRunMid.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementRunShort.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementSBJ.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementSFB.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementSitUp.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementSolidball.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementVC.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementVerticalJump.h \
    SteDevManager/SteDevice/SteSubElement/CSubElementVolleyball.h \
    SteDevManager/SteDevice/SteSubElement/Error/DevExcept.h \
    SteDevManager/SteDevice/SteSubElement/Error/IDevExcept.h \
    SteTestGuide/CEFV.h \
    SteTestGuide/CTestGuideBase.h \
    SteTestGuide/CTestGuideBasketball.h \
    SteTestGuide/CTestGuideBody.h \
    SteTestGuide/CTestGuideEyesight.h \
    SteTestGuide/CTestGuideFootball.h \
    SteTestGuide/CTestGuideGrip.h \
    SteTestGuide/CTestGuideManager.h \
    SteTestGuide/CTestGuidePUO.h \
    SteTestGuide/CTestGuidePVV.h \
    SteTestGuide/CTestGuidePullup.h \
    SteTestGuide/CTestGuidePushup.h \
    SteTestGuide/CTestGuideRunMid.h \
    SteTestGuide/CTestGuideRunShort.h \
    SteTestGuide/CTestGuideSBJ.h \
    SteTestGuide/CTestGuideSFB.h \
    SteTestGuide/CTestGuideSitUp.h \
    SteTestGuide/CTestGuideSkip.h \
    SteTestGuide/CTestGuideSolidball.h \
    SteTestGuide/CTestGuideVC.h \
    SteTestGuide/CTestGuideVerticalJump.h \
    SteTestGuide/CTestGuideVolleyball.h \
    SteTestGuide/TestMode.h \
    TestProc/CTestProcBCI.h \
    TestProc/CTestProcBasic.h \
    TestProc/CTestProcBasketball.h \
    TestProc/CTestProcEyesight.h \
    TestProc/CTestProcFootball.h \
    TestProc/CTestProcGrip.h \
    TestProc/CTestProcHeight.h \
    TestProc/CTestProcManage.h \
    TestProc/CTestProcPUO.h \
    TestProc/CTestProcPVV.h \
    TestProc/CTestProcPullUp.h \
    TestProc/CTestProcPushup.h \
    TestProc/CTestProcRun1000.h \
    TestProc/CTestProcRun50.h \
    TestProc/CTestProcRun800.h \
    TestProc/CTestProcSBJ.h \
    TestProc/CTestProcSFB.h \
    TestProc/CTestProcSitUp.h \
    TestProc/CTestProcSkip.h \
    TestProc/CTestProcSolidball.h \
    TestProc/CTestProcVC.h \
    TestProc/CTestProcVerticalJump.h \
    TestProc/CTestProcVolleyball.h \
    TestProc/CTestProcWeight.h \
    TestProcAntiCheating/CTestProcAntiCheatingBasic.h \
    TestProcAntiCheating/CTestProcAntiCheatingPullUp.h \
    TestProcAntiCheating/CTestProcAntiCheatingSitSBJ.h \
    TestProcAntiCheating/CTestProcAntiCheatingSitSFB.h \
    TestProcAntiCheating/CTestProcAntiCheatingSitUp.h \
    TestProcAntiCheating/CTestProcAntiCheatingVC.h \
    UserIdentification/CFaceRecognition.h \
    UserIdentification/CFindUserBasic.h \
    UserIdentification/CFindUserByFaceFeature.h \
    UserIdentification/CFindUserByMagneticCard.h \
    UserIdentification/CFindUserByStudentID.h \
    UserIdentification/CIdentifyUser.h \
    UserIdentification/IDetector.h \
    UserManger/CUserManage.h \
    UserManger/UserRoleManage.h

DISTFILES += \
    AssistDevManager/PowerUart/Readme.txt \
 \#    qmlFiles/qmldir

OTHER_FILES += myapp.rc
RC_FILE += myapp.rc
