import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "."

Item {
    id : root
    width: 450 * factor
    height: 85 * factor

    property string imgPath: ""
    property string strDisplay: ""
    property int imgWidth: 90
    property int imgHeight: 90
    property int fontSize: 68

    Image {
        id: imgIocn
        width: imgWidth * factor
        height: imgHeight * factor
        source: imgPath
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: displayText
        width: 420 * factor
        text: strDisplay
        font.pixelSize: fontSize * factor
        font.family: "Microsoft YaHei"
//        font.bold: true
        elide: Text.ElideMiddle
        color: "white"
        anchors.left: imgIocn.right
        anchors.leftMargin: 30 * factor
        anchors.verticalCenter: parent.verticalCenter
    }
}
