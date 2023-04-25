import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../../"
import "../../Moudues"
import "../../Popups"
import "."


Item {
    id: root
    width: 1920 * factor
    height: 2630 * factor

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
    }

    //取消按钮
    OtmCancelBtn{
        id: cancelBtn
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100 * factor
        anchors.left: parent.left
        anchors.leftMargin: 382 * factor
    }

    //提交按钮
    OtmCommitBtn{
        id: commitBtn
        anchors.verticalCenter: cancelBtn.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 380 * factor
    }

    OtmConfirmPopup{
        id: confirmPopup
        anchors.centerIn: parent
    }

    Connections{
        target: commitBtn
        onSigBtnClicked:{
            confirmPopup.open()
        }
    }
    Connections{
        target: cancelBtn
        onSigBtnClicked:{
            turnToPageSyncHomePage()
        }
    }

}
