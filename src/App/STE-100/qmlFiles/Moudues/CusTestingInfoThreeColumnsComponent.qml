import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"
Item {
    id: root
    width: 1525 * factor
    height: 115 * factor

    property real perColumnWidth: 507 * factor
    property real perRowHeight: 115 * factor

    readonly property font headerFont: Qt.font( { family: "Microsoft YaHei", pixelSize: 160 * factor, "bold": true } )
    property font contentFont: Qt.font( { family: "Microsoft YaHei", pixelSize: 90 * factor } )

    property string headerRectColor: "#2839B5"
    property string contentRectColor: "#3241CC"


    Row{
        anchors.fill: parent
        spacing: 3 * factor

        Rectangle{
            id: orderRect
            width: perColumnWidth
            height: perRowHeight
            color: contentRectColor
            Text {
                text: order
                font { family: "Microsoft YaHei"; pixelSize: 62 * factor }
                color: "white"
                anchors.centerIn: parent
            }
        }

        Rectangle{
            id: resultRect
            width: perColumnWidth
            height: perRowHeight
            color: contentRectColor
            Text {
                text: result
                font { family: "Microsoft YaHei"; pixelSize: 62 * factor }
                color: "white"
                anchors.centerIn: parent
            }
        }

        Rectangle{
            id: scoreRect
            width: perColumnWidth
            height: perRowHeight
            color: contentRectColor
            Text {
                text: score
                font { family: "Microsoft YaHei"; pixelSize: 62 * factor }
                color: "white"
                anchors.centerIn: parent
            }
        }
    }
}
