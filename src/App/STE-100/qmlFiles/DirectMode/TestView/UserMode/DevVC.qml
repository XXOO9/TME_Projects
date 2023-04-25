import QtQuick 2.7
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

    //提示界面倒计时 10s后跳转页面
    property int pageTurnCount: GlobalProperty.tipPopupCountTime

    //界面跳转倒计时 30s无操作跳转
    property int autoSwitchCount: GlobalProperty.pageSwitchCountTime

    ListModel{
        id: processPic
    }

    //设备异常弹窗
    CustAbNormalPopup{
        id: errorPopup
        anchors.centerIn: parent
    }

    //返回按钮
    CustReturnBtn{
        id:rectReturnBtn
        anchors.top: parent.top
        anchors.topMargin: 190 * factor
        anchors.left: parent.left
        anchors.leftMargin: 113 * factor
        onSignalBtnClicked: onReturnBtnClicked()
    }

    //文字 "测试"
    Text {
        id: strcenterTopText
        text: qsTr( "测试" )
        font.pixelSize: 124 * factor
        font.family: "Microsoft YaHei"
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 172 * factor
    }

    //姓名旁边的图标
    Image {
        id: imgNameIcon
        width: 70 * factor
        height: 70 * factor
        source: "qrc:/pic/10-1name.png"
        anchors.right: parent.right
        anchors.rightMargin: 323 * factor
        anchors.verticalCenter: rectReturnBtn.verticalCenter
    }

    //当前测试学生的名字
    Text {
        id : strCurTestStuName
        width: 260 * factor
        elide: Text.ElideMiddle
        text: GlobalProperty.strCurStudentName
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        font.bold: true
        color: "white"
        anchors.verticalCenter: imgNameIcon.verticalCenter
        anchors.left: imgNameIcon.right
        anchors.leftMargin: 32 * factor
    }

    //测试框
    CustSingleRectRetDisplay{
        id: singleRectRetDisplay
        anchors.left: parent.left
        anchors.leftMargin: 170 * factor
        anchors.top: parent.top
        anchors.topMargin: 371 * factor
    }

    //测试成绩总览显示栏
    Rectangle{
        id : rectTestGrade
        width: 1582 * factor
        height: 633 * factor
        color: "#3989EE"
        radius: 30 * factor
        anchors.top: parent.top
        anchors.topMargin: 1305 * factor
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
                font.family: "苹方 粗体"
                font.bold: true
                style: Text.Sunken
                color: "white"
                anchors.centerIn: parent
            }
        }

        //成绩显示表格
        CustStuTableView{
            id:gradeTableView
            width: 1526 * factor
            height: 468 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            tableType: GlobalProperty.directTestTable
            tableCount: GlobalProperty.isDisplayScore? 3 : 2
            widthArray: GlobalProperty.isDisplayScore? [0.3, 0.35, 0.35, 0, 0, 0] : [0.5, 0.5, 0, 0, 0, 0]
            headName: ["序次", "结果(ml)", "成绩", "", "", ""]
        }
    }

    //过程照片框
    Rectangle{
        id : rectProcessPic
        width: 1582 * factor
        height: 423 * factor
        color: "#3989EE"
        radius: 30 * factor
        anchors.top: rectTestGrade.bottom
        anchors.topMargin: 87 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            width: 1166 * factor
            height: 141 * factor
            source: "qrc:/pic/17.png"
            anchors.top: parent.top
            anchors.topMargin: -31 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                text: "过程照片"
                font.pixelSize: 84 * factor
                font.family: "苹方 粗体"
                style: Text.Sunken
                color: "white"
                anchors.centerIn: parent
            }
        }

        ListView{
            id: processPicListView
            width: 1320 * factor
            height: 260 * factor
            spacing: 150 * factor
            orientation: ListView.Horizontal
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            model: processPic
            clip: true
            snapMode: ListView.SnapToItem
            delegate: Image {
                width: 340 * factor
                height: 256 * factor
                source: path
                Text {
                    text: index + 1
                    font.pixelSize: 50 * factor
                    font.family: "苹方 粗体"
                    style: Text.Sunken
                    color: "#D8D6F0"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 28 * factor
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    //提交按钮
    OtoCommitBtn {
        id : rectCommitBtn
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 180 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        onSignalBtnClicked: {
            onCommitBtnClicked()
        }
    }

    Timer {
        id: btnTimer
    }

    //避免短时期快速点击
    function showSignature()
    {
        //根据弹窗类型，弹窗出响应的弹窗
        openCommitPopup()
        //解绑函数
        btnTimer.triggered.disconnect(showSignature)
        //恢复按钮可点击
        rectCommitBtn.btnEnable = true;
        //关闭超时弹窗
        timeoutTimer.stop();
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

    //页面存在操作，定时器刷新
    MouseArea {
        id: itemMouseArea
        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true
        z: -1
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
            console.log( "count time = " + autoSwitchCount )

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

            //页面无操作30s后跳转界面
            if(autoSwitchCount===0)
            {
                //暂停跳转定时器
                timeoutTimer.stop();
                //跳转至首页
                mainRoot.turnToHomePage()
                //超时模拟提交点击
                InterAction.procBtnClicked( Enums.OTO_COMMIT_BTN )
                //超时模拟签字提交练习成绩
                InterAction.procBtnClicked( Enums.OTO_VERIFY_SIGN_BTN )
            }
        }
    }

    //带有签字版的提交弹窗
    CustPopupSignatureConfirm{
        id: popConfirmPopup_signature
        z:10
        anchors.centerIn: parent
    }

    //不带签字版的提交弹窗
    CustPopupConfirmWithoutSignPainter{
        id: popConfirmPopup_value
        z:10
        anchors.centerIn: parent
    }

    //处理来自确认提交弹窗的信号
    Connections{
        target: popConfirmPopup_value
        //成绩上传结果，成功返回主界面
        onSigUpdateRetReulst: {
            if(bSuccess)
            {
                //跳转至学生信息界面
                mainRoot.turnToHomePage()
            }
        }
        onSignalCloseBtnClicked: {
            //关闭提交页面,开启超时计时器
            reStartTimeoutTimer();
        }
    }

    //处理来自异常弹窗的信号
    Connections{
        target: errorPopup
        //响应继续测试按钮
        onSignalContinueBtnClicked:{
            reStartTimeoutTimer();
            errorPopup.hiden()
        }

        //响应退出测试按钮
        onSignalQuitBtnClicked:{
            errorPopup.hiden()
            //跳转至学生信息界面
            mainRoot.turnToStudentInfoPage()
            //点击事件
            InterAction.procBtnClicked(Enums.OTO_TEST_BACK_INFO_BTN)
        }
    }

    //处理来自C++  InterACtion类的信号
    Connections{
        target: InterAction

        //实时刷新当前这次的测试结果, 并且刷新超时定时器
        onSignalCurrentTestRetAvailable:{
            //如果数据没有变化,或者数据为空, 则直接退出
            if( GlobalProperty.strCurRet === InterAction.getCurTestRet() ||
                    InterAction.getCurTestRet() === "" )
            {
                return
            }


            //获取当前测试结果
            GlobalProperty.strCurRet = InterAction.getCurTestRet()
            let tmpOffset = Number( GlobalProperty.strCurRet )
            if( tmpOffset <= 7000 ){
                singleRectRetDisplay.offsetPix = tmpOffset * 0.09 * factor
            }
            InterAction.qmlLogout( "refresh qml view testW ret = " + GlobalProperty.strCurRet )
            var value =  GlobalProperty.strCurRet * 1;
            if(value)
            {
                //有数据刷新时，就刷新超时定时器的计数时间, 并且开启倒计时显示(仰卧起坐，引体向上),有数据才关闭该定时器
                reStartTimeoutTimer()
            }
        }

        //设置过程照片
        onSignalSetTestingPicture:{
            processPic.append( { path: "file:///" + filePath } )
            if(processPic.count - 1)
            {
                processPicListView.currentIndex = processPic.count - 1;
            }
        }

        //弹出异常窗口
        onSignalShowAbNormalPopup:{
            errorPopup.show()
            timeoutTimer.stop()
        }

        //测试完成后, 自动弹出提交成绩的弹窗，只有在currentCommitPopup_type设置为commitPopup_value时才使用
        onSigOpenCommitPopup:{
            if( GlobalProperty.currentCommitPopup_type !== GlobalProperty.commitPopup_value ){
                console.log( "当前设置的不是自动提交弹窗" )
                return
            }

            //自动弹出的弹窗只可能是不带签字版的弹窗
            popConfirmPopup_value.show()
            InterAction.procBtnClicked( Enums.OTO_COMMIT_BTN )
        }
    }

    function openCommitPopup(){
        //带有签字版的弹窗
        if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_Signature ){
            InterAction.startSignature( "1" )
            popConfirmPopup_signature.show()
        }

        //不带有签字版的弹窗
        else if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_value ){
            popConfirmPopup_value.show()
        }
    }

    function onCommitBtnClicked(){
        //绑定定时器，避免快速点击
        rectCommitBtn.btnEnable = false
        btnTimer.interval = 500
        btnTimer.triggered.connect(showSignature)
        btnTimer.start()
    }

    function onReturnBtnClicked(){

        if( commonDataModel.rowCount() > 0 ){
            openRootPopup( "unFinished" )
            return
        }

        InterAction.startElapsedTimer()

        //暂停跳转定时器
//        timeoutTimer.stop();
        //跳转至学生信息界面
        mainRoot.turnToStudentInfoPage()
        //点击
        InterAction.procBtnClicked(Enums.OTO_TEST_BACK_INFO_BTN)
    }
}
