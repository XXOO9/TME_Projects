import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "."

Item {
    id: root
    width: 1582  * factor
    height: 864 * factor
    Rectangle{
        id: bakgRect
        anchors.fill: parent
        radius: 100 * factor
        color: "#568BEA"
    }

    Rectangle{
        id: leftVisionRect
        width: 500 * factor
        height: 500 * factor
        radius: 100 * factor
        color: "#4756EB"
        anchors.verticalCenter: bakgRect.verticalCenter
        anchors.left: bakgRect.left
        anchors.leftMargin: 150 * factor
        Text {
            text: "左眼:"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 100 * factor }
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 50 * factor
        }
        Text {
            text: InterAction.leftVision
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 80 * factor }
            anchors.centerIn: parent
        }
    }

    Rectangle{
        id: rightVisionRect
        width: 500 * factor
        height: 500 * factor
        radius: 100 * factor
        color: "#4756EB"
        anchors.verticalCenter: bakgRect.verticalCenter
        anchors.right: bakgRect.right
        anchors.rightMargin: 150 * factor
        Text {
            text: "右眼:"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 100 * factor }
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 50 * factor
        }
        Text {
            text: InterAction.rightVision
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 80 * factor }
            anchors.centerIn: parent
        }
    }
}
