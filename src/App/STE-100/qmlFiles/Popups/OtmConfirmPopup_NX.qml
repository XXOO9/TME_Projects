import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../Moudues"
import "../Popups/"
import ".."

Item {
    id: root
    width: 1760 * factor
    height: 2530 * factor

    property bool bTestFinished: false
    property int  commitCount: 0
    property int  commitFinished: 0

    //提交成功
    readonly property int commitSuccess: 3

    //提交失败
    readonly property int commitFailed: 4

    //提交异常下标
    readonly property int commitAbnormal: 5

    //是否提交成功， 用于控制成功后关闭弹窗， 提交失败后，不关闭弹窗
    property bool isSuccess: false

    //考试模式，弹出弹窗后,过三秒自动提交
    property int examModelCountTime: 3

    signal sigCancelBtnClicked()
    signal sigConfirmBtnClicked()

    //提交成功信号, 用户界面跳转
    signal sigCommitSuccess()

    Popup{
        id: popup
        width: 1760 * factor
        height: 2530 * factor
        modal: true
        anchors.centerIn: parent
        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
        }

        OtmTableBackground{
            id: tableBackGround
            width: 1760 * factor
            height: 2000 * factor
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            titleString: qsTr("确认测试成绩")
            CustStuTableView{
                id: scoreDataTable
                width: 1700 * factor
                height: bTestFinished ? 1600 * factor : 1400 * factor
                tableType: GlobalProperty.groupScoreTable
                anchors.top: parent.top
                anchors.topMargin: 160  * factor
                anchors.horizontalCenter: parent.horizontalCenter
                dataModel: scoreDataModel
            }
            Rectangle {
                width: 1700 * factor
                height: 290 * factor
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30 * factor
                color: "#6E50E3"
                visible: !bTestFinished

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 30 * factor
                    Image {
                        id: errorImg
                        width: tiptext.height
                        height: tiptext.height
                        source: "qrc:/pic/error.png"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        id: tiptext
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("仍有学生正在测试，未存在有效成绩！")
                        color: "white"
                        font{family: "苹方 粗体"; pixelSize: 74 * factor}
                    }
                }
            }

            Text {
                id: autoUploadCountTimeText
                visible: GlobalProperty.testType === GlobalProperty.testSyncMode ? true : false
                text: "即将在 " + examModelCountTime + " 秒后提交"
                color: "white"
                font{family: "苹方 粗体"; pixelSize: 74 * factor}
                z:50
                anchors.top: scoreDataTable.bottom
                anchors.topMargin: 20 * factor
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        //清除签字按钮
        OtmOperationBtn{
            id: cancelBtn
            displayText: "取消"
            anchors.top: tableBackGround.bottom
            anchors.topMargin: 57 * factor
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: 30 * factor
        }

        //确认提交按钮
        OtmOperationBtn{
            id: confirmBtn
            displayText: "确认"
            anchors.verticalCenter: cancelBtn.verticalCenter
            anchors.left: parent.horizontalCenter
            anchors.leftMargin: 30 * factor
        }

        //提交结果动态弹窗
        Popup{
            id: retPop
            visible: false

            onOpened: {
                commitAnimation.start()
            }

            background: Rectangle{
                anchors.fill: parent
                color: "skyblue"
                radius: 30 * factor
            }

            property var status: [ "提交中.", "提交中..", "提交中...", "提交成功", "提交失败", "提交异常" ]
            property int curIndex: 0
            width: 540 * factor
            height: 250 * factor
            modal: true
            closePolicy: Popup.NoAutoClose
            anchors.centerIn: parent

            Text {
                id: commitState
                text: retPop.status[retPop.curIndex]
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                anchors.centerIn: parent
            }

            //提交进度，展现形式为 12 / 15, 暂未实现， 故可见性置为false
            Text {
                id: commitPrograss
                visible: false
                text:  InterAction.commitPer
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: commitState.bottom
                anchors.topMargin: 20 * factor
            }

            //提交失败时，展示错误码
            Text {
                id: errorCodeText
                text: InterAction.errorString
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: commitState.bottom
                anchors.topMargin: 20 * factor
            }

            PropertyAnimation{
                id: commitAnimation
                running: false
                target: retPop
                property: "curIndex"
                from: 0
                to: 3
                loops: Animation.Infinite
                duration: 500
            }
        }

        //关闭按钮
        Rectangle {
            id: closeBtn
            width: 170 * factor
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100 * factor
            radius: width / 2
            color: "#DAD4D1"
            Image {
                width: 62 * factor
                height: width
                source: "qrc:/pic/22-CloseBtn.png"
                anchors.centerIn: parent
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    close()
                }
            }
        }

        onOpened: {
            examModelCountTime = GlobalProperty.autoCommitCountTime
            //只有同步模式下才用自动提交
            if( GlobalProperty.testType === GlobalProperty.testSyncMode ){
                examModelAutoUploadTimer.start()
            }
        }

        onClosed: {
            close()
        }
    }

    DelScoreConfirmPopup {
        id: delPopup
        anchors.centerIn: parent
    }

    Timer{
        id: examModelAutoUploadTimer
        repeat: true
        interval: 1000
        onTriggered: {
            if( examModelCountTime > 0 ){
                examModelCountTime--
            }else{
                examModelAutoUploadTimer.stop()
                InterAction.procBtnClicked( Enums.OTO_VERIFY_SIGN_BTN )
                startCommitAnimaiton()
            }
        }
    }

    Timer{
        id: delayTimer
        interval: 2000
        repeat: false
        running: false
        onTriggered: {
            retPop.visible = false
            if( isSuccess ){
                close()
                InterAction.startElapsedTimer()
                sigCommitSuccess()
            }
        }
    }

    Connections{
        target: cancelBtn
        onSigBtnClicked: {
            close()
        }
    }

    Connections{
        target: confirmBtn
        onSigBtnClicked:{
            //点击提交按钮过后,在得到提交结果之前,将按钮置为不可点击状态
            if( examModelAutoUploadTimer.running ){
                examModelAutoUploadTimer.stop()
                confirmBtn.mouseAreaEnable = false
                InterAction.saveWidgetSignPainter()
                InterAction.procBtnClicked( Enums.OTO_VERIFY_SIGN_BTN )
                startCommitAnimaiton()
            }
            //极限情况, 点的时候已经触发提交,则啥也不干
            else{

            }
        }
    }



    Connections {
        target: delPopup
        onSigOKBtnClicked: {
            InterAction.delStudentScore(index)
            examModelCountTime = GlobalProperty.autoCommitCountTime
            examModelAutoUploadTimer.restart()
        }

        onSigNOBtnClicked: {

        }
    }

    Connections{
        target: scoreDataTable
        onSignBtnClicked: {
            InterAction.startSignature( "0" )
            delPopup.open(index)
        }
    }

    Connections{
        target: InterAction
        //接收的到提交成绩的结果后，开启显示提交的结果(delayTimer控制显示两秒)
        onSignalGetUpdateTestRet:{
            //2022.07.18 另外一个弹窗由于没有屏蔽，也会接收到这个信号,用提交类型来区分
            if( GlobalProperty.currentCommitPopup_type !== GlobalProperty.commitPopup_value ){
                return
            }

            confirmBtn.mouseAreaEnable = true
            commitAnimation.stop()
            delayTimer.start()
            isSuccess = bSuccess
            if( !bSuccess ){
                retPop.curIndex = commitFailed
            }else{
                retPop.curIndex = commitSuccess
            }
        }

        //异常情况下, 关闭弹窗,并显示提交异常
        onSigCloseCommitStatePopup:{
            confirmBtn.mouseAreaEnable = true
            commitAnimation.stop()
            delayTimer.start()
            InterAction.errorString = "请确保存在有效成绩"
            retPop.curIndex = commitAbnormal
        }

        //接受信号，关闭弹窗（取消成绩后）
        onSigCloseConfirmPopup:{
            close()
        }

        onSignalStuTestStatus: {
            bTestFinished = testFinishedFlag
        }
    }

    function open(){
        popup.open()
        //刷新表格配置
        scoreDataTable.initTableConfig()
        InterAction.curPage = Enums.PAGE_POPUP
    }

    function close(){
        if(GlobalProperty.testType === GlobalProperty.testSyncMode)
        {
            InterAction.curPage = Enums.PAGE_GROUP_TEST
        }
        else
        {
            InterAction.curPage = Enums.PAGE_GROUP_HOME
        }
        InterAction.startSignature( "0" )
        examModelAutoUploadTimer.stop()
        popup.close()
    }

    function onCancelBtnClicked(){
        sigCancelBtnClicked()
        InterAction.clearSignPainter()
    }

    function onConfirmBtnClicked(){
        sigConfirmBtnClicked()
        InterAction.saveWidgetSignPainter()
    }

    function startCommitAnimaiton(){
        retPop.visible = true
        commitAnimation.start()
    }

    //显示一对多提交界面的签字版(和一对一的签字版是同一个签字版，只是更改了尺寸和坐标)
    function displayOtmSignPainter(){
        //        20220715 屏蔽
        /*
//        var wid = 0.0
//        var hei = 0.0
//        var posX = 0
//        var posY = 0.0
//        if( factor > 0.4 ){
//            InterAction.qmlLogout( "display on Pad" )
//            wid = (3400 * factor ).toFixed(0).toString()
//            hei = (1000 * factor).toFixed(0).toString()
//            posX = 110
//            posY = 1710
//        }else{
//            InterAction.qmlLogout( "display on PC" )
//            wid = (1650 * factor ).toFixed(0).toString()
//            hei = (480 * factor).toFixed(0).toString()
//            posX = 40
//            posY = 590
//        }
//        InterAction.changedSignPainterPos( wid, hei, posX, posY )
////        InterAction.startSignature( "1" )
//        InterAction.qmlLogout( "display signature Otm Auto Confirm" )
*/
    }
}
