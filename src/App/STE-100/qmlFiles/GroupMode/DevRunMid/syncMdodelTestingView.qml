import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../../Moudues"
import "../../Popups"
import "../../"
import "."

Item {
    id: root
    width: 1920 * factor
    height: 2630 * factor

    Component.onCompleted: {
        InterAction.stopElapsedTimer( "跳转到中长跑同步模式下的测试界面" )
    }

    function onCommitBtnClicked(){
        //点击事件
        InterAction.procBtnClicked(Enums.OTM_SYNC_COMMIT_BTN)

        //如果当前配置的是带有签字版的弹窗
        if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_Signature ){
            confirmPopup.open()
        }

        if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_value ){
            confirmPopup_NX.open()
        }
    }

    //蓝色背景
    Rectangle{
        anchors.fill: parent
        color: "#4756EB"
    }

    //测试状态框
    OtmTestStateArea{
        id: testStateArea
        anchors.top: parent.top
        anchors.topMargin: 104 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

     //正在测试表格的背景
    OtmTableBackground{
        id: testingTableArea
        width: 1760 * factor
        height: 1460 * factor
        anchors.top: testStateArea.bottom
        anchors.topMargin: 170 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        CustStuTableView{
            tableType: GlobalProperty.syncTestTable
            width: 1700 * factor
            height: 1290 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
            tableCount: 6
            widthArray: [0.15, 0.175, 0.2, 0.1, 0.2, 0.175]
            headName: ["设备号", "姓名", "准考证号", "圈数", "结果", "状态"]
            cellTypeArray: [GlobalProperty.normalCell, GlobalProperty.normalCell, GlobalProperty.normalCell,
                             GlobalProperty.normalCell, GlobalProperty.normalCell, GlobalProperty.testCell]
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 15 * factor

        //取消按钮
        OtmCancelBtn{
            id: cancelBtn
            anchors.verticalCenter: parent.verticalCenter
            onSigBtnClicked:{
                cancelTestPopup.show()
            }
        }

        //重测按钮
        OtmCommitBtn{
            id: reTestBtn
            //visible: false
            state: "active"
            anchors.verticalCenter: parent.verticalCenter
            displayText: "重  测"
            btnColorNormal: "#3333CC"
            btnColorInvaild: "#434399"
            onSigBtnClicked:{
                retestComfirmPopup.open()
            }
        }

        //提交按钮
        OtmCommitBtn{
            id: commitBtn
            state: "inactive"
            anchors.verticalCenter: parent.verticalCenter
            onSigBtnClicked:{
                onCommitBtnClicked()
            }
        }
    }

    //取消测试弹窗
    CancelTestConfirmPopup {
        id: cancelTestPopup
        anchors.centerIn: parent
    }

    //重测弹窗
    OtmRetestComfirmPopup{
        id: retestComfirmPopup
        anchors.centerIn: parent
    }

    //不带签字版的弹窗
    OtmConfirmPopup_NX{
        id: confirmPopup_NX
        anchors.centerIn: parent
    }

    //带签字版的提交弹窗
    OtmConfirmPopup{
       id: confirmPopup
       anchors.centerIn: parent
    }

    //处理取消测试弹窗事件
    Connections {
        target: cancelTestPopup
        onSignalQuitBtnClicked: {
            //页面跳转
            turnToGroupHomePage()
            //取消测试按钮点击事件
            InterAction.procBtnClicked(Enums.OTM_CANCEL_TEST_BTN)
        }
    }

    //响应重测按钮
    Connections{
        target: retestComfirmPopup
        onSigRetestBtnClicked:{
            retestComfirmPopup.close()
            //按钮失效
            reTestBtn.setInactive();
            //点击事件
            InterAction.procBtnClicked(Enums.OTM_RESTART_TEST_BTN)
        }
    }

    Connections{
        target: confirmPopup_NX
        onSigCommitSuccess:{
            turnToGroupHomePage()
        }
    }

    Connections{
        target: confirmPopup
        onSigCommitSuccess:{
            turnToGroupHomePage()
        }
    }

    Connections {
        target: testStateArea
        onTestFinished: {
            confirmPopup_NX.bTestFinished = true;
        }
    }

    Connections {
        target: InterAction

        //后端控制返回按钮是否可点
        onSigSetBtnEnable: {
            if(btnType === Enums.OTM_CANCEL_TEST_BTN)
            {
                cancelBtn.mouseEnable = bEnable
            }
            else if(btnType === Enums.OTM_RESTART_TEST_BTN)
            {
                reTestBtn.state = bEnable ? "active":"inactive"
            }
            else if(btnType === Enums.OTM_SYNC_COMMIT_BTN)
            {
                commitBtn.state = bEnable ? "active":"inactive"
            }
        }

        //是否所有人完成测试
        onSignalStuTestStatus: {
            confirmPopup.bTestFinished = testFinishedFlag
            confirmPopup_NX.bTestFinished = testFinishedFlag
        }

        onSigOpenCommitPopup:{
            //如果配置的弹窗不是带有签字版的弹窗,则直接return
            if( GlobalProperty.currentCommitPopup_type !== GlobalProperty.commitPopup_value ){
                return
            }
            confirmPopup_NX.open()

            //模拟点击测试界面提交按钮(不是弹窗里面的提交按钮)
            InterAction.procBtnClicked( Enums.OTO_COMMIT_BTN )
        }
    }
}
