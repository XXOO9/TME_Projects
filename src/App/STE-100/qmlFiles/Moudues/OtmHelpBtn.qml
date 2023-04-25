import QtQuick 2.12
import QtQuick.Controls 2.12
Item {
    id: root
    width: 140 * factor
    height: 300 * factor

    signal sigClicked()

    Rectangle{
        anchors.fill: parent
        radius: 40 * factor
        color: "#1F71CF"
        border.color: "#1ADDFC"

        Image {
            id: img
            width: 54 * factor
            height: 76 * factor
            source: "qrc:/pic/help.png"
            anchors.right: parent.right
            anchors.rightMargin: 30 * factor
            anchors.top: parent.top
            anchors.topMargin: 40 * factor
        }

        Text {
            text: "帮\n助"
            color: "white"
            font{family: "Microsoft YaHei"; pixelSize: 54 * factor}
            anchors.right: parent.right
            anchors.rightMargin: 30 * factor
            anchors.top: img.bottom
            anchors.topMargin: 25 * factor
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                onHelpBtnClicked()
            }
        }
    }

    function onHelpBtnClicked(){
        sigClicked()
        mainRoot.showVideoPlayer()
    }
}
