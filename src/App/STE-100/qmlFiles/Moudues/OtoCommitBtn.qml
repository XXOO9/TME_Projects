import QtQuick 2.0
import ProjectEnums 1.0

Item {
    width: 564 * factor
    height: 168 * factor

    property bool btnEnable: true
    property string enableColor: "#EA825B"
    property string disAbleColor: "#B06244"

    signal signalBtnClicked()

    //提交按钮
    Rectangle{
        id : rectCommitBtn
        anchors.fill: parent
        radius: 84 * factor
        color: btnEnable ? enableColor: disAbleColor
        Text {
            text: "提交"
            font.pixelSize: 80 * factor
            font.family: "Microsoft YaHei"
            font.bold: true
            color: btnEnable ? "white" : "gray"
            anchors.centerIn: parent
        }

        MouseArea{
            id: btnMouseArea
            anchors.fill: parent
            enabled: btnEnable
            onClicked: {
                signalBtnClicked()
            }
        }
    }

    Connections{
        target: InterAction
        //后端控制提交按钮是否可点
        onSigSetBtnEnable: {
            if(btnType === Enums.OTO_COMMIT_BTN)
            {
                btnEnable = bEnable
            }
        }
    }
}

