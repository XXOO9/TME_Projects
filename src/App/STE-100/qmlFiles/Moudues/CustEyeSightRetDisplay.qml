import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root

    property string leftEyeSightValue: "0.0"
    property string rightEyeSightValue: "0.0"

    width: 1582  * factor
    height: 864 * factor


    Rectangle{
        width: parent.width
        height: parent.height
        radius: 30 * factor
        color: "#568BEA"
        anchors.centerIn: parent


        //中间的细线
        Rectangle{
            id: scaleLine
            width: 2 * factor
            height: parent.height * 0.8
            color: "white"
            radius: 1 * factor
            anchors.centerIn: parent
        }

        //左眼标题
        Text {
            id: leftEyeSightRetText
            text: "左眼:"
            color: "white"
            font.pixelSize: 80 * factor
            font.family: "Microsoft YaHei"
            font.bold: true

            anchors.top: parent.top
            anchors.topMargin: 20 * factor
            anchors.left: parent.left
            anchors.leftMargin: 40 * factor
        }

        //左眼视力结果
        Text {
            id: leftEyesightRet
            text: leftEyeSightValue
            color: "white"
            font{ pixelSize: 80 * factor; family: "Microsoft YaHei" }
            anchors.left: leftEyeSightRetText.left
            anchors.top: leftEyeSightRetText.bottom
            anchors.topMargin: 50 * factor
        }

        //右眼标题
        Text {
            id: rightEyeSightRetText
            text: "右眼:"
            color: "white"
            font.pixelSize: 80 * factor
            font.family: "Microsoft YaHei"
            font.bold: true

            anchors.verticalCenter: leftEyeSightRetText.verticalCenter
            anchors.left: scaleLine.right
            anchors.leftMargin: 40 * factor
        }
    }
}
