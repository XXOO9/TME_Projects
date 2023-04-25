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

    signal  sigStartBtnClicked()

    Component.onCompleted: {
        GlobalProperty.bDirectMode = false
    }

    //蓝色背景
    Rectangle{
        anchors.fill: parent
        color: "#4756EB"
    }

    OtmHelpBtn{
        id: helpBtn
        z:5
        anchors.left: parent.left
        anchors.leftMargin: -30 * factor
        anchors.top: parent.top
        anchors.topMargin: 520 * factor
    }

    OtmAddUserBtn{
        id: userAddBtn
        z:5
        anchors.verticalCenter: helpBtn.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: -30 * factor
    }

    OtmFaceRecogView{
        id: faceRecog
        width: 1002 * factor
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 109 * factor
    }

    OtmTableBackground{
        id: recogedUserTableRect
        width: 1760 * factor
        height: 990 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:faceRecog.bottom
        anchors.topMargin: 170 * factor
        rectColor: "#9585F7"
        titleBackgroundColor: "qrc:/pic/16.png"
        titleString: qsTr("已识别学生列表")
        CustStuTableView{
            tableType: GlobalProperty.syncStudentTable
            width: 1700 * factor
            height: 820 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
        }
    }

    OtmCommitBtn{
        id: startTestBtn
        displayText: "开始测试"
        anchors.top: recogedUserTableRect.bottom
        anchors.topMargin: 100 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Connections{
        target: startTestBtn
        onSigBtnClicked:{
            turnToPageTestingPage()
        }
    }
}
