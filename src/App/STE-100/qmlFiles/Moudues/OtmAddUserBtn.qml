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

        Text {
            text: "录\n入\n信\n息"
            color: "white"
            font{family: "Microsoft YaHei"; pixelSize: 54 * factor}
            anchors.centerIn: parent
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                openKeyBord()
            }
        }
    }
}
