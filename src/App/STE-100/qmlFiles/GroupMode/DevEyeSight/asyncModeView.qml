import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../../"
import "../../Moudues"
import "../../Popups"
import "."
Item{
    id: root
    //蓝色背景
    Rectangle{
        anchors.fill: parent
        color: "#4756EB"
    }

    Component.onCompleted: {
        GlobalProperty.bDirectMode = false
        GlobalProperty.devType = 'VISION'
        GlobalProperty.iDevType = GlobalProperty.devVision
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
        height: 516 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: faceRecog.bottom
        anchors.topMargin: 170 * factor
        CustStuTableView{
            width: 1700 * factor
            height: 345 * factor
            tableType: GlobalProperty.asyncTestTable
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
        }
    }

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
            id: studentTable
            width: 1700 * factor
            height: 345 * factor
            tableType: GlobalProperty.asyncStudentTable
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
        }
    }

    //提交按钮
    OtmCommitBtn{
        id: commitBtn
        anchors.top: finishedTableArea.bottom
        anchors.topMargin: 100 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
