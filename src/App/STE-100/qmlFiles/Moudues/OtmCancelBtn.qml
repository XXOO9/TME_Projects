import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Item {
    id: root
    width: 564 * factor
    height: 168 * factor

    property string displayText: "取消测试"
    property string btnColor: "#2B41D0"
    property bool   mouseEnable: true

    signal sigBtnClicked()

    Rectangle{
        id: cancelBtn
        anchors.fill: parent
        radius: 84 * factor
        color:btnColor
        opacity: mouseEnable ? 1.0 : 0.5
        Text {
            text: displayText
            font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
            color: "white"
            anchors.centerIn: parent
        }
    }
    MouseArea{
        anchors.fill: parent
        enabled: mouseEnable
        onClicked: {
            sigBtnClicked()
        }
    }
}
