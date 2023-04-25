import QtQuick 2.0
import QtQuick.Controls 2.12
import "../JavaScript/ProjectValue.js" as Common
import "../"
Rectangle {
    id: root
    property alias text: content.text
    property alias clickEnable: mouseArea.enabled

    signal sigClicked()

    width: 400 * factor
    height: 100 * factor

    radius: 50 * factor

    color: mouseArea.enabled ? "lightgreen" : "gray"

    Text {
        id: content
        text: "下一步"
        color: "white"
        font{ family: Common.fontYaHei; pixelSize: 40 * factor; bold: true }
        anchors.centerIn: parent
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            sigClicked()
        }
    }
}
