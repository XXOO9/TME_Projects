﻿import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../../Moudues"
import "../../Popups"
import "../../"
import "."

Item{
    id: root
    width: 1920 * factor
    height: 2630 * factor

    property bool bTestFinish: false
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

    //绑卡提示窗
    BindDeviceTipPopup {
        id: bindGuidePopup
        anchors.centerIn: parent
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
            tableType: GlobalProperty.asyncTestTable
            width: 1700 * factor
            height: 1041 * factor
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
        height: 516 * factor
        titleString: "已完成测试学生列表"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: testingTableArea.bottom
        anchors.topMargin: 131 * factor
        CustStuTableView{
            dataModel: studentDataModel
            tableType: GlobalProperty.asyncStudentTable
            width: 1700 * factor
            height: 351 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
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

    //不带签字板确认提交窗
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

            if(bTestFinish)
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
        //人员识别成功 开启nfc刷卡倒计时
        onUpdateNFCRecogTime: {
            if(time>0)
            {
                bindGuidePopup.nfcRecogSecond = time / 1000
                bindGuidePopup.show()
            }
        }

        //结束刷卡倒计时
        onSigStopNFCRecogTimer: {
            bindGuidePopup.hide()
        }

        //后端控制返回按钮是否可点
        onSigSetBtnEnable: {
            if(btnType === Enums.OTM_ASYNC_COMMIT_BTN)
            {
                commitBtn.state =  bEnable ? "active" : "inactive"
            }
        }

        onSignalStuTestStatus: {
            //存在数据-可提交
            bCommitable = commitFlag;
            //是否完成测试
            bTestFinish = testFinishedFlag;
        }

        onSigTestItemChanged: {
            //响应短跑项目切换
            GlobalProperty.updateDevTestItem()
            //更新表格数据
            asyncTestTable.initTableConfig()
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
