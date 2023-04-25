import QtQuick 2.0
import QtQuick.Controls 2.12
import "../"
Item {
    id: root
    property alias val: displayText.text

    width: 800 * factor
    height: 400 * factor

    Button{
        text: "上移"
        onClicked: GlobalProperty.scanfLineStartPos -= 10
        anchors.right: displayText.left
        anchors.verticalCenter: displayText.verticalCenter
    }

    Text {
        id: displayText
        anchors.centerIn: parent
        text: GlobalProperty.scanfLineStartPos + "\n" + GlobalProperty.scanfHorizontalPos.toFixed( 0 )
        font.pixelSize: 80 * factor
        color: "white"
    }

    Button{
        text: "下移"
        onClicked: GlobalProperty.scanfLineStartPos += 10
        anchors.left: displayText.right
        anchors.verticalCenter: displayText.verticalCenter
    }

    Button{
        text: "左"
        anchors.bottom: displayText.top
        anchors.horizontalCenter: displayText.horizontalCenter
        onClicked: GlobalProperty.scanfHorizontalPos -= 10
    }

    Button{
        text: "右"
        anchors.top: displayText.bottom
        anchors.horizontalCenter: displayText.horizontalCenter
        onClicked: GlobalProperty.scanfHorizontalPos += 10
    }


}
