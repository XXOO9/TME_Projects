import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "."
import "./Moudues"
import "./Popups"
import "./ConfigObject/"

ApplicationWindow{
    id : mainRoot
    visible: true
    flags: Qt.FramelessWindowHint | Qt.Window

    property real factor: Screen.height === 2880.0 ? Screen.height / 2880.0 : ( Screen.height - 100 )/ 2880.0
    property bool bSelfCheckFinish: false

    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    signal closeVideoPopup()

    //软件启动之后，首先获取设备类型
    Component.onCompleted: {
        InterAction.startSignature( "1" )
        SignPainterConfig.initSignPainter( false )
        InterAction.curPage = Enums.PAGE_SYSTEM_SELFCHECK
        GlobalProperty.setTestDeviceType()

        //获取数据库中配置
        InterAction.sigGetSpecificSettingViewInfo(-1)

        //InterAction.readCommitPopupConfigFromDataBase()
        InterAction.startSignature( "0" )


    }

    //背景"蓝紫色"
    Rectangle {
        id: backGround
        color: GlobalProperty.bDirectMode ? "#384DD5" : "#4756EB"
        anchors.fill: parent
    }

    //一对一：顶部显示时间
    CustCurrentTimeDisplay {
        id: curTime
        anchors.top: parent.top
        visible: GlobalProperty.bDirectMode
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //一对一：底部信息栏
    TopSystemStateInfos {
        id: rectTipsInfo
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: GlobalProperty.bDirectMode
        onSignalClicked: {
            tipsArea.visible = !tipsArea.visible
            if(InterAction.curPage === Enums.PAGE_DIRECT_HOME || InterAction.curPage === Enums.PAGE_SYSTEM_ABNORMAL)
            {
                adminAuthtiCationPopup.open()
            }
        }
    }

    //一对多：顶部信息标题栏
    OtmTopTitle{
        id: topTitle
        anchors.top: parent.top
        visible: !GlobalProperty.bDirectMode
        anchors.horizontalCenter: parent.horizontalCenter
        Button{
            z:5
            text: '点不得'
            visible: InterAction.isDeveloper
            onClicked: turnSystemSetting()
        }
    }

    //一对多：底部状态栏
    OtmBottomStateArea{
        id: bottomStateArea
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: !GlobalProperty.bDirectMode
        z: 1
        onSignalClicked: {
            if(InterAction.curPage === Enums.PAGE_GROUP_HOME || InterAction.curPage === Enums.PAGE_SYSTEM_ABNORMAL)
            {
                adminAuthtiCationPopup.open()
            }
        }
    }

    //系统设置弹窗
    AuthenticationPopup {
        id: adminAuthtiCationPopup
        anchors.centerIn: parent
    }

    Loader{
        id : mainLoader
        source: "qrc:/qmlFiles/StartUpPage.qml"
        //source: "qrc:/qmlFiles/HomePageLocal.qml"

        Connections{
            target: mainLoader.item
            ignoreUnknownSignals: true

            //响应自检结果信号
            onSelfCheckFinished:{
                if(success){
                    //自检成功
                    turnToHomePage()
                }
                else{
                    //自检失败
                    turnToHomePageLocal()
                }
            }

            onSetSelfCheckFlag: {
                bSelfCheckFinish = true;
            }

            onSigShowVideoPlayerView:{
                if( bShow ){
                    showVideoPlayer()
                }
            }
        }
    }

    CustRootPopup{
        id: rootPopup
        anchors.centerIn: parent
    }

    CustTipsInfoArea{
        id: tipsArea
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 500 * factor
    }

    Loader{
        id: toolLoader
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Loader{
        id: sysSettingLoader
        anchors.centerIn: parent
    }

    Connections {
        target: InterAction
        //设备状态更新
        onSignalUpdateDevStatus: {
            if(devType !== GlobalProperty.devUnknow){
                //修改设备状态, false代表直连
                GlobalProperty.bDirectMode = isDirectConnect;
                GlobalProperty.testType = testMode;
                GlobalProperty.iDevType = devType;
                GlobalProperty.bDevConnect = true;
                let tmp = { "curDevTypeValue": devType }
//                InterAction.setSystemConfigInfo( Enums.PAGE_HOST, tmp, 5 )

                //设置设备类型
                GlobalProperty.setTestDeviceType()
                //设置测试配置
                GlobalProperty.setTestConfig()

                //修改显示布局
                curTime.visible = GlobalProperty.bDirectMode
                rectTipsInfo.visible = GlobalProperty.bDirectMode

                topTitle.visible = !GlobalProperty.bDirectMode
                bottomStateArea.visible = !GlobalProperty.bDirectMode

                backGround.color = GlobalProperty.bDirectMode ? "#384DD5" : "#4756EB"

                //跳转至首页
                InterAction.qmlLogout( "设备状态更新，重新跳转至首页" )
                turnToHomePage();
            }
            else{
                GlobalProperty.bDevConnect = false;
                InterAction.qmlLogout("设备断连")
                turnToHomePageLocal()
            }
            //}
        }

        onSignalDevTestError: {
            //设备异常，返回首页
            turnToHomePage()
        }

        //设置提交方式
        onSigCommitPopupParamChanged:{
            //目前只存在1或者0的情况
            //自动提交
            if( signatureExist === 0 ){
                GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_value
            }

            //手动提交
            if( signatureExist === 1 ){
                GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_Signature
            }

            GlobalProperty.autoCommitCountTime     = autoCountTime
            GlobalProperty.isDelBtnEnable          = delScoreBtnEnable
            GlobalProperty.userInfoPageExited      = userInfoPage
        }

        onSigCloseRootPopup:{
            rootPopup.closePopup()
        }

        onSigDisplaySignpainterParam: displaySignpainterInfo( info )

        onSigTestItemChanged: GlobalProperty.updateDevTestItem()
    }

    function displaySignpainterInfo( info ){
        GlobalProperty.signInfos = info
    }

    //自检失败跳转到首页
    function turnToHomePageLocal()
    {
        mainLoader.source = "";
        mainLoader.source = "qrc:/qmlFiles/HomePageLocal.qml"
        GlobalProperty.pageBeforoPopups = Enums.PAGE_SYSTEM_ABNORMAL
        InterAction.setCurrentPage( Enums.PAGE_SYSTEM_ABNORMAL)
    }

    //跳转到学生信息界面( 非人脸识别成功,(用户识别成功后，点击返回等按钮时进入)进入 )
    function turnToStudentInfoPage(){
        console.log( "main.qml turn to page stuInfo" )
        InterAction.resetTest()
        mainLoader.source = ""
        mainLoader.setSource( "qrc:/qmlFiles/DirectMode/UserPage.qml", { "bCommingTimerWork": false } )
        InterAction.curPage = Enums.PAGE_USER_INFO
        GlobalProperty.pageBeforoPopups = Enums.PAGE_USER_INFO
        console.log( "main.qml -> 即将跳转到学生信息界面   " + Enums.PAGE_USER_INFO )
        console.log( "----------------------------------------------------------" )
    }

    //跳转到学生信息界面( 人脸识别成功进入使用 )
    function recogSuccessTurnToStudentInfoPage(){
        console.log( "recog success coming in indextify" )
        InterAction.resetTest()
        mainLoader.source = ""
        mainLoader.setSource( "qrc:/qmlFiles/DirectMode/UserPage.qml", { "bCommingTimerWork": true } )
        InterAction.curPage = Enums.PAGE_USER_INFO
        GlobalProperty.pageBeforoPopups = Enums.PAGE_USER_INFO
        InterAction.qmlLogout( "main.qml -> 即将跳转到学生信息界面   " + Enums.PAGE_USER_INFO )
        console.log( "----------------------------------------------------------" )
    }

    //跳转到游客界面
    function turnToTouristPage(){
        InterAction.resetTest()
        mainLoader.source = ""
        mainLoader.source = "qrc:/qmlFiles/DirectMode/TouristPage.qml"
        //InterAction.stopTest();
        InterAction.curPage = Enums.PAGE_TOURIST_HOME
        console.log( "main.qml -> 即将跳转到游客主界面界面    " + Enums.PAGE_TOURIST_HOME )
        console.log( "----------------------------------------------------------" )
    }

    //跳转到首页
    function turnToHomePage(){
        InterAction.startElapsedTimer()
        //关闭弹窗
        closeRootPopup()
        //关闭系统设置页面
        closeSystemSetting()
        /* 跳转至首页 */
        InterAction.resetTest()
        InterAction.qmlLogout( "++++++++++++++coming into func turnToHomePage" )
        var source = "qrc:/qmlFiles/" + GlobalProperty.getDevTypeFileName() + "/HomePage.qml";
        mainLoader.source = ""
        mainLoader.setSource(source);
        InterAction.qmlLogout("main.qml -> 即将跳转到首页界面     " + Enums.PAGE_DIRECT_HOME)
        if(GlobalProperty.bDirectMode)
        {
            InterAction.curPage = Enums.PAGE_DIRECT_HOME
            GlobalProperty.pageBeforoPopups = Enums.PAGE_DIRECT_HOME
        }
        else
        {
            InterAction.curPage = Enums.PAGE_GROUP_HOME
            GlobalProperty.pageBeforoPopups = Enums.PAGE_GROUP_HOME
        }

        console.log("+++++++++++++GlobalProperty.pageBeforoPopups", GlobalProperty.pageBeforoPopups)

        keyFocus.forceActiveFocus()
    }

    //跳转到同步测试页面
    function turnToGroupHomePage()
    {
        InterAction.curPage = Enums.PAGE_GROUP_HOME
        GlobalProperty.pageBeforoPopups = Enums.PAGE_GROUP_HOME
        console.log( "main.qml -> 即将跳转到组连同步测试界面    " +  Enums.PAGE_GROUP_HOME )
        console.log( "----------------------------------------------------------" )
        keyFocus.forceActiveFocus()
    }

    //跳转到同步测试页面
    function turnToGroupSyncTestPage(){
        InterAction.curPage = Enums.PAGE_GROUP_TEST
        console.log( "main.qml -> 即将跳转到组连同步测试界面    " +  Enums.PAGE_GROUP_TEST )
        console.log( "----------------------------------------------------------" )
    }

    //跳转到学生测试界面
    function turnToStudentTestModelPage(){
        GlobalProperty.curUserEnteredTestView = true
        GlobalProperty.reFreshCurStuInfo()
        GlobalProperty.strCurRet = "--"
        var qmlFileName = GlobalProperty.getDevQMLFileName();
        mainLoader.source = ""
        mainLoader.source = "qrc:/qmlFiles/DirectMode/TestView/UserMode/"+ qmlFileName + ".qml"
        InterAction.curPage = Enums.PAGE_USER_TEST
        InterAction.qmlLogout( "main.qml -> 即将跳转到测试界面    " +  Enums.PAGE_USER_TEST )
        console.log( "----------------------------------------------------------" )
    }

    //跳转到游客测试界面
    function turnToTouristTestModelPage(){
        //初始化显示
        GlobalProperty.strCurRet = "--"
        var qmlFileName = GlobalProperty.getDevQMLFileName();
        mainLoader.source = ""
        mainLoader.source = "qrc:/qmlFiles/DirectMode/TestView/TouristMode/"+ qmlFileName +".qml"
        InterAction.curPage = Enums.PAGE_TOURIST_TEST
        console.log( "main.qml -> 即将跳转到游客测试界面    " + Enums.PAGE_TOURIST_TEST )
        console.log( "----------------------------------------------------------" )
    }

    //跳转到成绩查询界面
    function turnToScoreQueryPage(){
        mainLoader.source = ""
        mainLoader.source = "qrc:/qmlFiles/DirectMode/ScorePage.qml"
        InterAction.curPage = Enums.PAGE_USER_SCOREQUERY
        openRootPopup( "loadingPopup" )
        console.log( "main.qml -> 即将跳转到成绩查询界面    " + Enums.PAGE_USER_SCOREQUERY )
        console.log( "----------------------------------------------------------" )
    }

    //打开根部弹窗
    function openRootPopup( popupType ){
        rootPopup.openSpecificPop( popupType )
    }

    //打开调试菜单
    function openDebugTool(){
        toolLoader.source = "qrc:/qmlFiles/Moudues/ToolMenu.qml"
    }

    //打开调试工具
    function openSignPainterTool(){
        SignPainterConfig.changeOtmSignPainter()
        toolLoader.source = "qrc:/qmlFiles/Popups/ToolAdjustSignPainter.qml"
        toolLoader.item.open()
    }

    //关闭根部弹窗
    function closeRootPopup(){
        rootPopup.closePopup()
        InterAction.qmlLogout( "####关闭所有弹窗..." )
    }

    //系统设置界面
    function turnSystemSetting(){
        InterAction.curPage = Enums.PAGE_SETTING
        sysSettingLoader.source = 'qrc:/qmlFiles/Settings/SettingHome.qml'
    }

    //关闭系统设置页面
    function closeSystemSetting(){
        sysSettingLoader.source = ""
        InterAction.curPage = GlobalProperty.pageBeforoPopups
    }


    Loader{
        id: videoPlayerLoader
        anchors.fill: parent
        z: 5
    }

    Connections{
        target: videoPlayerLoader.item
        onSignalCloseBtnClicked: {
            console.log( "close btn clicked" )
            if(GlobalProperty.pageBeforoPopups === Enums.PAGE_DIRECT_HOME || GlobalProperty.pageBeforoPopups === Enums.PAGE_GROUP_HOME)
            {
                InterAction.curPage = GlobalProperty.pageBeforoPopups
                console.log("InterAction.curPage1", InterAction.curPage)
            }
            videoPlayerLoader.source = ""
            closeVideoPopup()
        }
    }

    Connections{
        target: toolLoader.item
        ignoreUnknownSignals: true
        onSigClicked:{
            switch( index ){
            case 0:
                openSignPainterTool()
                return
            case 1:
                toolLoader.setSource( "qrc:/qmlFiles/Moudues/ToolAdjustScanfLineStartPos.qml" )
                return
            default:
            }
        }
    }

    //打开播放视频弹窗
    function showVideoPlayer(){
        InterAction.curPage = Enums.PAGE_VIDEO
        videoPlayerLoader.setSource("qrc:/qmlFiles/Popups/CustVideoDisplayPage.qml")
    }


    //捕捉扫描枪事件
    FocusScope {
        id:keyFocus
        anchors.fill: parent
        focus: true
        z: 20
        visible: ( InterAction.curPage === Enums.PAGE_DIRECT_HOME || InterAction.curPage === Enums.PAGE_GROUP_HOME)
        property string scanCode: ""
        Keys.onPressed: {
            //扫码枪结束标志
            if(event.key === Qt.Key_Return)
            {
                InterAction.catchScanCodeInfo(scanCode)
                console.log("scam", scanCode)
                scanCode = ""
            }

            else
            {
                if(event.key >= Qt.Key_0 && event.key <= Qt.Key_Z)
                {
                    scanCode += event.text
                    console.log("scam", event.key, event.text)
                }
            }
        }
    }

    function moveWindow( x, y ){
        mainRoot.x += x
        mainRoot.y += y
    }
}
