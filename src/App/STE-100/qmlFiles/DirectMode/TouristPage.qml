import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../Moudues"
import "../Popups"
import "../"
import "."
Item {
    id : root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    property bool bClicked: true

    property int pageTurnCount: GlobalProperty.tipPopupCountTime         //提示界面倒计时
    property int autoSwitchCount: GlobalProperty.pageSwitchCountTime     //界面跳转倒计时

    signal sigShowVideoPlayerView(bool bShow)

    Component.onCompleted: {
        InterAction.stopElapsedTimer( "跳转到游客界面" )
    }

    //怡康标题栏
    DeviceTitle {
        id: rectDeviceName
        anchors.top: parent.top
        anchors.topMargin: 180 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //矩形-> "当前为游客模式"
    Rectangle{
        id : rectCurrentTestModelTips
        width: 1582 * factor
        height: 355 * factor
        radius: 30 * factor
        color: "transparent"
        border.color: "#B5BBFF"
        anchors.top: parent.top
        anchors.topMargin: 382 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            id: imgTouristIcon
            width: 171 * factor
            height: 171 * factor
            source: "qrc:/pic/8-tourist.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 180 * factor
        }
        Text {
            id: strModelTips
            text: qsTr( "当前为游客模式" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: imgTouristIcon.right
            anchors.leftMargin: 55 * factor
        }
    }

    //开始测试按钮
    CustStartTestBtn{
        id : startTestBtn
        bClicked: bClicked
        anchors.horizontalCenter: rectCurrentTestModelTips.horizontalCenter
        anchors.top: rectCurrentTestModelTips.bottom
        anchors.topMargin: 85 * factor
        onSignalTouristTestModel: {
            timeoutTimer.stop();
            //跳转至游客测试页面
            mainRoot.turnToTouristTestModelPage()
            //点击游客模式开始测试
            InterAction.procBtnClicked(Enums.OTO_START_TUR_TEST_BTN)
        }
    }

    //指导视频
    Rectangle{
        id : rectGuideVideoBtn
        width: 1582 * factor
        height: 726 * factor
        radius: 30 * factor
        anchors.top: startTestBtn.bottom
        anchors.topMargin: 105 * factor
        anchors.horizontalCenter: startTestBtn.horizontalCenter
        Image {
            smooth: true
            anchors.fill: parent
            source: GlobalProperty.getSuitableVideoCover2()
            //            source: "qrc:/pic/videoCovers/SBJ_3.png"
        }
        Image {
            width: 238 * factor
            height: 238 * factor
            source: "qrc:/pic/9-playBtn.png"
            anchors.centerIn: parent
            opacity: 0.9
        }

        Text {
            id: strGuideVideo
            text: GlobalProperty.sportName + qsTr( "指导视频" )
            font.pixelSize: 62 * factor
            font.family: "Microsoft YaHei"
            color: "white"
            anchors.left: parent.left
            anchors.leftMargin: 50 * factor
            anchors.bottom: rectGuideVideoBtn.bottom
            anchors.bottomMargin: 30 * factor
        }
        MouseArea{
            id: guideVideoBtnMouse
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //暂时停止倒计时
                timeoutTimer.stop();
                sigShowVideoPlayerView( true )
            }
        }
    }

    //返回按钮
    Rectangle{
        id : returnBtn
        width: 563 * factor
        height: 170 * factor
        color: "#EA825B"
        radius: 85 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 198 * factor
        Text {
            id: strReturn
            text: qsTr( "返回" )
            font.pixelSize: 78 * factor
            font.family: "Microsoft YaHei"
            font.bold: true
            color: "white"
            anchors.centerIn: parent
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                returnBtn.enabled = false;
                btnTimer.interval = 500
                btnTimer.triggered.connect(returnHomePage)
                btnTimer.start()
            }
        }
    }

    Timer {
        id: btnTimer
    }

    //避免短时期快速点击
    function returnHomePage()
    {
        InterAction.startElapsedTimer()
        //解绑函数
        btnTimer.triggered.disconnect(returnHomePage)
        //恢复按钮可点击
        returnBtn.enabled = true;
        //点击返回按钮
        mainRoot.turnToHomePage()
        //关闭超时弹窗
        timeoutTimer.stop();
    }

    PageSwitchingPopup {
        id: pageSwitchPopup
        switchCount: pageTurnCount
        anchors.centerIn: parent
        popupVisible: false
        onPopClose: {
            autoSwitchCount = GlobalProperty.pageSwitchCountTime;
            pageTurnCount = GlobalProperty.tipPopupCountTime;
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true
        z: -1
        onEntered: {
            //页面存在操作，定时器刷新
            if(InterAction.curPage !== Enums.PAGE_VIDEO)
            {
                autoSwitchCount = GlobalProperty.pageSwitchCountTime;
                pageTurnCount = GlobalProperty.tipPopupCountTime;
                pageSwitchPopup.popupVisible = false;
                console.log("点击界面, 刷新定时器");
            }
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
                mainRoot.turnToHomePage()
                console.log( "test page timeout, turn to student info page" )
            }
        }
    }

    Connections {
        target: mainRoot
        onCloseVideoPopup: {
            //视频窗口关闭-重新启动倒计时
            autoSwitchCount = GlobalProperty.pageSwitchCountTime;
            pageTurnCount = GlobalProperty.tipPopupCountTime;
            timeoutTimer.start();
            bClicked = true
        }
    }
}
