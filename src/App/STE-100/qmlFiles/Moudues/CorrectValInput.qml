import QtQuick 2.0
import QtQuick.Controls 2.12
import "../JavaScript/ProjectValue.js" as Common
import "../"

Item {
    id: root
    width: 300 * factor
    height: 100 * factor

    property alias correctName: correctNameText.text
    property alias correctVal: valInput.text
    property alias correctBtnName: btnText.text
    property alias tipsText: valInput.placeholderText
    property alias clickeEnable: mouseArea.enabled
    property alias regExpObj: valInput.validator

    signal sigClicked()


    Text {
        id: correctNameText
        width: parent.width
        height: parent.height * 0.2
        text: qsTr("text")
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        color: "white"
        font.pixelSize: 50 * factor
        anchors.top: parent.top
        anchors.topMargin: 10 * factor
        anchors.left: parent.left
        anchors.leftMargin: 10 * factor
    }


    TextField{
        id: valInput
        width: parent.width * 0.7
        height: parent.height * 0.4
        placeholderText: ""
        font{ pixelSize: 40 * factor; bold: true }
        anchors{ left: correctNameText.left; top: correctNameText.bottom; topMargin: 10 * factor }
    }

    Rectangle{
        id: btn
        width: parent.width * 0.2
        height: valInput.height
        anchors{ left: valInput.right; verticalCenter: valInput.verticalCenter }
        color: mouseArea.enabled ? "green" : "gray"
        border.width: 5 * factor
        border.color: "black"

        Text {
            id: btnText
            text: qsTr("确定")
            color: "white"
            font.pixelSize: width * 0.5
            anchors.centerIn: parent
        }

        MouseArea{
            id: mouseArea
            anchors.fill: parent
            onPressed: parent.opacity = 0.7
            onReleased: parent.opacity = 1
            onClicked: sigClicked()
        }
    }
}
