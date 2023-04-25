import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../../Moudues"
import "../../Popups"
import "../../"
import "."
Item{
    id: root

    //是否测试完成
    property bool bTestFinished: false

    //是否可提交
    property bool bCommitable: false

    Component.onCompleted: {
        //2022/0707, 修改: 进入异步测试界面后，首先将提交方式强制改为带签字版的手动提交
        GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_Signature
    }

    //蓝色背景
    Rectangle{
        anchors.fill: parent
        color: "#4756EB"
    }

    //帮助按钮
    OtmHelpBtn{
        id: helpBtn
        z:5
        anchors.left: parent.left
        anchors.leftMargin: -30 * factor
        anchors.top: parent.top
        anchors.topMargin: 520 * factor
    }

    //录入用户按钮
    OtmAddUserBtn{
        z:5
        anchors.verticalCenter: helpBtn.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: -30 * factor
    }

    //人脸识别框
    OtmFaceRecogView{
        id: faceRecog
        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: root.top
        anchors.topMargin: 109 * factor
    }

    //正在测试表格的背景
    OtmTableBackground{
        id: testingTableArea
        width: 1760 * factor
        height: 1216 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: faceRecog.bottom
        anchors.topMargin: 170 * factor
        titleString: "测试学生列表"
        CustStuTableView{
            id: asyncTestTable
            width: 1700 * factor
            height: 1041 * factor
            tableType: GlobalProperty.asyncTestTable
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
        }
    }

    /*
    //完成测试表格的背景
    OtmTableBackground{
        id: finishedTableArea
        width: 1760 * factor
        height: 1216 * factor
        titleString: "已完成测试学生列表"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: testingTableArea.bottom
        anchors.topMargin: 131 * factor
        CustStuTableView{
            id: studentTable
            width: 1700 * factor
            height: 1041 * factor
            dataModel: studentDataModel
            tableType: GlobalProperty.asyncStudentTable
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
            tableCount : GlobalProperty.isDisplayScore ? 6 : 5
            widthArray : GlobalProperty.isDisplayScore ? [0.15, 0.175, 0.2, 0.1625, 0.1625, 0.15] : [0.2, 0.2, 0.3, 0.3, 0, 0]
            headName : ["设备号", "姓名", "准考证号", "左眼", "右眼", "状态", ""]
            cellTypeArray : [GlobalProperty.normalCell, GlobalProperty.normalCell, GlobalProperty.normalCell,
                             GlobalProperty.normalCell, GlobalProperty.normalCell, GlobalProperty.deviceCell]
        }
    }
    */

    //提交按钮
    OtmCommitBtn{
        id: commitBtn
        state: "inactive"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 65 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //带签字板确认提交窗
    OtmConfirmPopup{
        id: confirmPopup
        anchors.centerIn: parent
    }

    //不带签字版的弹窗
    OtmConfirmPopup_NX{
        id: confirmPopup_NX
        anchors.centerIn: parent
    }

    //停止测试弹窗
    StopAndCleanPopup{
        id: stopPopup
        anchors.centerIn: parent
    }

    Connections {
        target: stopPopup
        onSignalQuitBtnClicked: {
            stopPopup.hiden()
            //点击事件
            InterAction.procBtnClicked(Enums.OTM_ASYNC_CANCEL_BTN)
        }
    }

    Connections {
        target: commitBtn
        onSigBtnClicked: {
            //点击事件
            InterAction.procBtnClicked(Enums.OTM_ASYNC_COMMIT_BTN)

            if(bTestFinished)
            {
                //测试完成、提交全部
                if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_Signature )
                {
                    confirmPopup.bTestFinished = true;
                    confirmPopup.open()
                }
                else
                {
                    confirmPopup_NX.bTestFinished = true;
                    confirmPopup_NX.open()
                }
            }
            else
            {
                //测试未完成，提交部分
                if(bCommitable)
                {
                    if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_Signature )
                    {
                        confirmPopup.bTestFinished = false;
                        confirmPopup.open()
                    }
                    else
                    {
                        confirmPopup_NX.bTestFinished = false;
                        confirmPopup_NX.open()
                    }
                }
                //不存在成绩数据，清空并停止
                else
                {
                    stopPopup.visible = true;
                    stopPopup.show();
                }
            }
        }
    }

    Connections {
        target: InterAction

        //已有成绩可提交 参数：是否完成测试
        onSignalStuTestStatus: {
            //存在数据-可提交
            bCommitable = commitFlag;
            //是否完成测试
            bTestFinished = testFinishedFlag
        }

        //后端控制返回按钮是否可点
        onSigSetBtnEnable: {
            if(btnType === Enums.OTM_ASYNC_COMMIT_BTN)
            {
                commitBtn.state =  bEnable ? "active" : "inactive"
            }
        }
    }

    UnbindConfirmPopup {
        id: unBindCPoup
        anchors.centerIn: parent
    }

    Connections{
        target: asyncTestTable
        onSigUnbindBtnClicked: {
            unBindCPoup.open(index)
        }
    }

    Connections {
        target: unBindCPoup
        onSigOKBtnClicked: {
            InterAction.unBindStudent(index)
            if(commonDataModel.rowCount()<= 0)
            {
                commitBtn.setInactive()
            }
        }
    }
}
