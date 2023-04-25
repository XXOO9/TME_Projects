import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../"
import "."
// 一对一使用的
Item {
    id : root
    width: 1582 * factor
    height: 142 * factor
    Component.onCompleted: {
    }

    //文字 "STE-100仰卧起坐测试仪"
    Text {
        id: strDeviceName
        text: GlobalProperty.deviceName
        font.pixelSize: 124 * factor
        font.family: "MF YueYuan (Noncommercial)"
        horizontalAlignment: Text.AlignHCenter
        color: "white"
        fontSizeMode: Text.Fit
        width: 1421 * factor
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {

        }
    }
}
