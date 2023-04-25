import QtQuick 2.12
import QtQuick.Controls 2.12
Item{
    id: root
    width: 1760 * factor
    height: 550 * factor
    property string titleString: "正在测试的学生列表"
    property string rectColor:  "#3989EE"
    property string titleBackgroundColor: "qrc:/pic/17.png"

    Component.onCompleted: {
        console.log( "width = " + rect.width + " " + "height = " + rect.height )
    }

    Rectangle{
        id: rect
        anchors.fill: parent
        color: rectColor
        radius: 30 * factor
    }

    Image {
        width: 1131 * factor
        height: 140 * factor
        source: titleBackgroundColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: -20 * factor
        Text {
            id: titleText
            text: titleString
            anchors.centerIn: parent
            color: "white"
            font{family: "苹方 粗体"; pixelSize: 84 * factor}
        }
    }
}
