import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import ".."

Item {
    id: root
    width: 300 * factor
    height: 140 * factor

    property bool btnEnable: true
    property string enableColor: "#EA825B"
    property string disAbleColor: "#999999"

    signal signalBtnClicked()

    //返回按钮
    Rectangle{
        id : rectReturnBtn
        width: 140 * factor
        height: width
        radius: width / 2
        color: btnEnable ? enableColor : disAbleColor
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        Image {
            width: 44 * factor
            height: 76 * factor
            source: "qrc:/pic/18-returnBtn.png"
            anchors.centerIn: parent
        }
        MouseArea{
            id: btnMouseArea
            anchors.fill: parent
            enabled: btnEnable
            onClicked: {
                //点击返回按钮
                signalBtnClicked()
            }
        }
    }

    //文字 "返回"
    Text {
        text: qsTr( "返回" )
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        color: "#F9DB54"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }

    Connections{
        target: InterAction
        //后端控制返回按钮是否可点
        onSigSetBtnEnable: {
            if(btnType === Enums.OTO_TEST_BACK_INFO_BTN)
            {
                btnEnable = bEnable
            }
        }
    }
}
