import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../../../Moudues"
import "../../../Popups"
import "../../../"

Item {
    id : root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    property int pageTurnCount: GlobalProperty.tipPopupCountTime         //提示界面倒计时
    property int autoSwitchCount: GlobalProperty.pageSwitchCountTime     //界面跳转倒计时

    //返回按钮
    CustReturnBtn{
        id:rectReturnBtn
        anchors.top: parent.top
        anchors.topMargin: 227 * factor
        anchors.left: parent.left
        anchors.leftMargin: 113 * factor
        onSignalBtnClicked: {
            //暂停跳转定时器
            timeoutTimer.stop();

            //跳转至游客主界面
            mainRoot.turnToTouristPage();

            //点击
            InterAction.procBtnClicked(Enums.OTO_TEST_BACK_INFO_BTN)
        }
    }

    //文字 "测试"
    Text {
        id: strcenterTopText
        text: qsTr( "测试" )
        font.pixelSize: 124 * factor
        font.family: "Microsoft YaHei"
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: rectReturnBtn.verticalCenter
    }

    //返回首页按钮
    TurBackHomeBtn {
        id: homeBtnItem
        anchors.right: parent.right
        anchors.rightMargin: 168 * factor
        anchors.verticalCenter: rectReturnBtn.verticalCenter
        onSignalBtnClicked: {
            //暂停跳转定时器
            timeoutTimer.stop();

            //跳转至首页
            mainRoot.turnToHomePage();

            //点击
            InterAction.procBtnClicked(Enums.OTO_TEST_BACK_HOME_BTN)
        }
    }

    //常规仪器测试框
    CustDoubleRectRetDisplay{
        id: doubleRectTestRet
        visible: GlobalProperty.checkDoubleRect()
        strDevType: GlobalProperty.devType
        testState: InterAction.testState
        anchors.left: parent.left
        anchors.leftMargin: 170 * factor
        anchors.top: parent.top
        anchors.topMargin: 420 * factor
    }

    //测试成绩总览显示栏
    Rectangle{
        id : rectTestGrade
        width: 1582 * factor
        height: 1143 * factor
        color: "#3989EE"
        radius: 30 * factor
        anchors.top: doubleRectTestRet.bottom
        anchors.topMargin: 116 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            width: 1166 * factor
            height: 141 * factor
            source: "qrc:/pic/17.png"
            anchors.top: parent.top
            anchors.topMargin: -31 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                text: qsTr("测试成绩")
                font.pixelSize: 84 * factor
                font.family: "PingFang SC"
                font.bold: true
                color: "white"
                anchors.centerIn: parent
            }
        }
        //成绩显示表格
        CustStuTableView{
            id:gradeTableView
            width: 1526 * factor
            height: 938 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 52.5 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            tableType: GlobalProperty.directTouristTable
            tableCount: 2
            widthArray: [0.5, 0.5, 0, 0, 0, 0]
            headName: ["序次", "结果(次)", "", "", "", ""]
        }
    }

    Rectangle {
        id: tipRect
        width: 1582 * factor
        height: 168 * factor
        color: "#3042CF"
        radius: 30 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 130 * factor

        Rectangle {
            id: pointRect
            width: 23 * factor
            height: 23 * factor
            radius: width / 2
            color: "#FFB784"
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 5 * factor
            anchors.left: parent.left
            anchors.leftMargin: 56 * factor
        }

        Text {
            id: titleText
            text: qsTr( "当前为游客模式" )
            font.pixelSize: 62 * factor
            font.family: "Microsoft YaHei"
            color: "white"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 110 * factor
        }
    }

    CustAbNormalPopup{
        id: errorPopup
        anchors.centerIn: parent
    }

    //刷新超时跳转定时器
    function reStartTimeoutTimer(){
        autoSwitchCount = GlobalProperty.pageSwitchCountTime
        pageTurnCount = GlobalProperty.tipPopupCountTime
        pageSwitchPopup.popupVisible = false;
        timeoutTimer.restart()
    }

    PageSwitchingPopup {
        id: pageSwitchPopup
        switchCount: pageTurnCount
        anchors.centerIn: parent
        popupVisible: false
        onPopClose: {
            reStartTimeoutTimer();
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true
        z: -1
        //页面存在操作，定时器刷新
        onEntered: {
            reStartTimeoutTimer()
        }
        onReleased: {
            reStartTimeoutTimer()
        }
    }

    //超时定时器, 进入测试界面30s内没有测试成绩就自动退出到学生信息界面
    Timer{
        id: timeoutTimer
        repeat: true
        running: true
        interval: 1000
        onTriggered: {
            autoSwitchCount--

            InterAction.qmlLogout( "Tourist page timeout timer is working, remaining time = " + autoSwitchCount )
            //提示10s切换
            if(pageSwitchPopup.popupVisible)
            {
                pageSwitchPopup.switchCount = pageTurnCount--;
            }

            //页面无操作20s后加载提示界面
            if(autoSwitchCount === pageTurnCount)
            {
                pageSwitchPopup.popupVisible = true;
                pageSwitchPopup.switchCount = pageTurnCount--;
            }

            //页面误操作30s后跳转界面
            if(autoSwitchCount===0)
            {
                mainRoot.turnToHomePage();
                InterAction.qmlLogout( "tourist test page timeout, turn to home page" )
            }
        }
    }

    //处理来自C++  InterACtion类的信号
    Connections{
        target: InterAction
        //实时刷新当前这次的测试结果, 并且关闭超时定时器
        onSignalCurrentTestRetAvailable:{
            //如果数据没有变化,则直接退出
            InterAction.qmlLogout( "curRet = " + GlobalProperty.strCurRet )
            InterAction.qmlLogout( "recvRet = " + InterAction.getCurTestRet() )
            if( GlobalProperty.strCurRet === InterAction.getCurTestRet() )
            {
                InterAction.qmlLogout( "++++++++++++++++++++++++++++++++++++++++++++" )
                return
            }
            else{
                InterAction.qmlLogout( "------------------------------------------------" )
                GlobalProperty.strCurRet = InterAction.getCurTestRet()
            }

            var value =  GlobalProperty.strCurRet * 1;
            if(value)
            {
                //有数据刷新时，就结束超时定时器
                reStartTimeoutTimer();

                //如果需要有倒计时的项目,则开始倒计时(引体向上）
                doubleRectTestRet.startCountTime2()
            }

            InterAction.qmlLogout( "refresh qml view testW ret = " + GlobalProperty.strCurRet);
        }

        //弹出异常窗口
        onSignalShowAbNormalPopup:{
            timeoutTimer.stop()
            errorPopup.show()
        }
    }

    //处理来自常规结果框的信号
    Connections{
        target: doubleRectTestRet
        onSignalTouristSingleTestFinished:{
            doubleRectTestRet.stopCountTimer()
            //数据十秒间隔计时
            doubleRectTestRet.countTime = 10
        }
    }

    //处理来自异常弹窗的信号
    Connections{
        target: errorPopup
        //响应继续测试按钮
        onSignalContinueBtnClicked:{
            reStartTimeoutTimer()
            errorPopup.hiden()
        }

        //响应退出测试按钮
        onSignalQuitBtnClicked:{
            errorPopup.hiden()

            //跳转至游客主界面
            mainRoot.turnToTouristPage();

            //点击事件
            InterAction.procBtnClicked(Enums.OTO_TEST_BACK_INFO_BTN)
        }
    }
}
