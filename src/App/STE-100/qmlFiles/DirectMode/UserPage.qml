import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import ".."
import "../Moudues/"
import "../Popups"

Item {
    id: root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    property string strStudentName: GlobalProperty.strCurStudentName
    property string strStudentAge: GlobalProperty.strCurStudentAge
    property string strStudentGrade: GlobalProperty.strCurStudentGrade
    property string strStudentClass: GlobalProperty.strCurStudentClass
    property string strStudentNumber: GlobalProperty.strCurStudentNumber
    property int pageTurnCount: GlobalProperty.tipPopupCountTime        //提示界面倒计时
    property int autoSwitchCount: GlobalProperty.pageSwitchCountTime    //界面跳转倒计时

    property int countDown: 5

    property int rectOpacity: 0

    property bool bCommingTimerWork: false

    signal signalGuideVideoBtnClicked( var videoName )
    signal sigShowVideoPlayerView(bool bShow)

    Component.onCompleted: {
        GlobalProperty.reFreshCurStuInfo()
        if( !GlobalProperty.bDevConnect ){
            rectOpacity = 0.5
            InterAction.qmlLogout( "Identification.qml -> device is disConnected" )
        }

        imgGuideVideoCover.source = ""
        imgGuideVideoCover.source = GlobalProperty.getSuitableVideoCover()
        timeoutTimer.start()

        if( bCommingTimerWork )
        {
            commingInTimer.start()
        }
        else
        {
            rectCircle.visible = false
            console.log( "不显示倒计时框" )
        }
        InterAction.stopElapsedTimer( "跳转到用户信息界面" )

        //2023.01.12， 如果不开启用户信息页面，则直接跳转到测试界面
        if( !GlobalProperty.userInfoPageExited && !GlobalProperty.curUserEnteredTestView ){
            mainRoot.turnToStudentTestModelPage()
            //点击开始测试
            InterAction.procBtnClicked(Enums.OTO_START_STU_TEST_BTN)
        }
    }

    Component.onDestruction: {
        bCommingTimerWork = false
    }

    //怡康标题栏
    DeviceTitle {
        id: rectDeviceName
        anchors.top: parent.top
        anchors.topMargin: 180 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //学生信息框
    Rectangle {
        id: rectStuInfos
        width: 1582 * factor
        height: 563 * factor
        radius: 30 * factor
        color: "transparent"
        border.color: "#BCC2FF"
        anchors.top: parent.top
        anchors.topMargin: 381 * factor
        anchors.horizontalCenter: parent.horizontalCenter

        //头像
        Rectangle {
            id: studentIconBG
            width: 309 * factor
            height: 309 * factor
            radius: width / 2
            color: "#5161E5"
            anchors.horizontalCenter: imgStudentIcon.horizontalCenter
            anchors.bottom: imgStudentIcon.bottom
        }

        Image {
            id: imgStudentIcon
            width: 234 * factor
            height: 394 * factor
            source: GlobalProperty.bCurStudentSex === true ? "qrc:/pic/10-1man.png" : "qrc:/pic/10-1-2woman.png"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 113 * factor
            anchors.left: parent.left
            anchors.leftMargin: 134 * factor
        }

        //姓名
        CustImgWithText {
            id: displayStuName
            imgPath: "qrc:/pic/10-1name.png"
            imgWidth: 90
            imgHeight: 90
            strDisplay: strStudentName
            anchors.left: parent.left
            anchors.leftMargin: 452 * factor
            anchors.top: parent.top
            anchors.topMargin: 160 * factor
        }

        //年龄
        CustImgWithText {
            id: displayStuAge
            imgPath: "qrc:/pic/10-2birthday.png"
            imgWidth: 90
            imgHeight: 90
            strDisplay: strStudentAge
            anchors.left: parent.left
            anchors.leftMargin: 987 * factor
            anchors.verticalCenter: displayStuName.verticalCenter
        }
        //年级
        CustImgWithText {
            id: displayStuGrade
            imgPath: "qrc:/pic/gradeAndClass.png"
            imgWidth: 90
            imgHeight: 90
            strDisplay: strStudentGrade + qsTr("年级") /*+ strStudentClass + qsTr("班")*/
            //strDisplay: qsTr(strStudentClass)
            fontSize: 68
            anchors.left: displayStuName.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 153 * factor
        }
        //学号
        CustImgWithText {
            id: displayStuNumber
            imgPath: "qrc:/pic/10-4stuId.png"
            imgWidth: 90
            imgHeight: 90
            strDisplay: strStudentNumber
            anchors.left: displayStuAge.left
            anchors.verticalCenter: displayStuGrade.verticalCenter
        }
    }

    //开始测试按钮
    CustStartTestBtn {
        id: rectStartTestBtn
        anchors.left: rectStuInfos.left
        anchors.top: rectStuInfos.bottom
        Rectangle{
            id: rectMask
            width: parent.width
            height: parent.height
            radius: 60 * factor
            color: "#6A6A6A"
            opacity: rectOpacity
            anchors.centerIn: parent
        }
        //倒计时圆圈
        Rectangle{
            id: rectCircle
            width: 300 * factor
            height: width
            radius: width / 2
            color: "transparent"
            border.color: "white"
            border.width: 10 * factor
            anchors.left: parent.left
            anchors.leftMargin: 80 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 150 * factor
            Text {
                id:countDownNUmber
                text: countDown
                font{ pixelSize: 90 * factor; family: "Microsoft YaHei"; bold: true }
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset:  - 20 * factor
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -40  * factor
            }
            Text {
                text: "s"
                font{ pixelSize: 50 * factor; family: "Microsoft YaHei"; bold: true }
                color: "white"
                anchors.left: countDownNUmber.right
                anchors.leftMargin: 10 * factor
                anchors.bottom: countDownNUmber.bottom
                anchors.bottomMargin: 10 * factor

            }
            Text {
                text: "倒计时"
                font{ pixelSize: 50 * factor; family: "Microsoft YaHei"; bold: true }
                color: "white"
                anchors.top: countDownNUmber.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Timer{
                id: commingInTimer
                repeat: true
                interval: 1000
                onTriggered: {
                    if( countDown > 0 ){
                        countDown--
                    }else{
                        stop()
                        //页面跳转

                        mainRoot.turnToStudentTestModelPage()
                        //点击开始测试
                        InterAction.procBtnClicked(Enums.OTO_START_STU_TEST_BTN)

                    }
                }
            }
        }


        //点击学生界面开始测试按钮
        anchors.topMargin: 85 * factor
        onSignalStudentTestModel: {
            //关闭跳转页面定时器
            InterAction.startElapsedTimer()
            timeoutTimer.stop()

            //页面跳转
            mainRoot.turnToStudentTestModelPage()

            //点击开始测试
            InterAction.procBtnClicked(Enums.OTO_START_STU_TEST_BTN)
        }
    }

    //指导视频按钮
    Rectangle {
        id: rectGuideVideo
        width: 750 * factor
        height: 518 * factor
        radius: 30 * factor
        anchors.left: rectStartTestBtn.left
        anchors.top: rectStartTestBtn.bottom
        anchors.topMargin: 112 * factor
        Image {
            id: imgGuideVideoCover
            source: "qrc:/pic/GuideVideo.png"
            anchors.fill: parent
            Image {
                width: parent.width
                height: 180 * factor
                smooth: true
                source: "qrc:/pic/picBottom.png"
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    text: GlobalProperty.sportName + qsTr( "指导视频" )
                    font.family: "Microsoft YaHei"
                    font.pixelSize: 56 * factor
                    color: "white"
                    horizontalAlignment: Text.horizontalCenter
                    elide: Text.ElideRight
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 24 * factor
                }
            }
        }
        Image {
            width: 238 * factor
            height: 238 * factor
            source: "qrc:/pic/9-playBtn.png"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                //点击指导视频
                //关闭人脸识别成功后的5秒倒计时自动跳转
                //                stopCommingTimer()
                //关闭跳转页面定时器
                timeoutTimer.stop()
                //关闭5s定时器
                commingInTimer.stop()
                sigShowVideoPlayerView( true )
            }
        }
    }

    //成绩查询按钮
    Rectangle {
        width: 750 * factor
        height: 518 * factor
        radius: 30 * factor
        color: "#BB82FD"
        anchors.verticalCenter: rectGuideVideo.verticalCenter
        anchors.right: rectStartTestBtn.right
        Image {
            width: 427 * factor
            height: 310 * factor
            source: "qrc:/pic/queryGrade.png"
            anchors.top: parent.top
            anchors.topMargin: 57 * factor
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            text: qsTr("成绩查询")
            font.pixelSize: 68 * factor
            font.family: "MF DianHei (Noncommercial)"
            font.bold: true
            color: "white"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 55 * factor
            anchors.horizontalCenter: parent.horizontalCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                InterAction.startElapsedTimer()
                //关闭跳转页面定时器
                timeoutTimer.stop()
                //关闭人脸识别成功后的5秒倒计时自动跳转
                stopCommingTimer()
                //点击成绩查询按钮
                mainRoot.turnToScoreQueryPage()
            }
        }
    }

    //退出按钮
    Rectangle {
        id: returnBtn
        width: 563 * factor
        height: 170 * factor
        color: "#EA825B"
        radius: 85 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rectGuideVideo.bottom
        anchors.topMargin: 90 * factor
        Text {
            id: strReturn
            text: qsTr("退出")
            font.pixelSize: 78 * factor
            font.family: "Microsoft YaHei"
            font.bold: true
            color: "white"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                returnBtn.enabled = false
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
        //解绑函数
        btnTimer.triggered.disconnect(returnHomePage)
        //关闭人脸识别成功后的5秒倒计时自动跳转
        stopCommingTimer()
        //恢复按钮可点击
        returnBtn.enabled = true;
        //关闭超时弹窗
        timeoutTimer.stop();
        //点击返回按钮
        mainRoot.turnToHomePage()
    }

    //刷新超时跳转定时器
    /*
    function reStartTimeoutTimer(){
        if(InterAction.curPage !== Enums.PAGE_VIDEO)
        {
            autoSwitchCount = GlobalProperty.pageSwitchCountTime
            pageTurnCount = GlobalProperty.tipPopupCountTime
            pageSwitchPopup.popupVisible = false;
            timeoutTimer.restart()
        }
    }
    */

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
                pageSwitchPopup.switchCount = pageTurnCount--
            }

            //页面无操作20s后加载提示界面
            if(autoSwitchCount === pageTurnCount)
            {
                pageSwitchPopup.popupVisible = true
                pageSwitchPopup.switchCount = pageTurnCount--
            }

            //页面无操作30s后跳转界面
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
            //隐藏5s倒计时区域
            rectCircle.visible = false
        }
    }

    function stopCommingTimer(){
        if( commingInTimer.running ){
            commingInTimer.stop()
        }
    }
}
