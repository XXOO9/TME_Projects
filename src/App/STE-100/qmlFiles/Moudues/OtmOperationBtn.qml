import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../"

Item {
    id: root
    width: 422 * factor
    height: 140 * factor

    property string displayText: "确认"
    property bool mouseAreaEnable: true
    property string btnColor: "#2B41D0"
    property int textSize: 62
    property bool textBold: false

    signal sigBtnClicked()

    Rectangle{
        id: rect
        anchors.fill: parent
        radius: 70 * factor
        color: mouseAreaEnable ? btnColor : "gray"

        Text{
            width: parent.width
            wrapMode: Text.WrapAnywhere
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: displayText
            font{ family: "Microsoft YaHei"; pixelSize: textSize * factor; bold: textBold }
            color: "white"
        }

        MouseArea{
            id: btnMouseArea
            enabled: mouseAreaEnable
            anchors.fill: parent
            onClicked: {
                sigBtnClicked()
            }
        }
    }
}
