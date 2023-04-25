import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"
Item {
    id: root
    width: 1525 * factor
    height: 115 * factor

    property var widthArray: [ 197 * factor, 380 * factor, 590 * factor, 350 * factor ]
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
            width: widthArray[ 0 ]
            height: perRowHeight
            color: contentRectColor
            Text {
                text: order
                font: headerFont
                color: "white"
                anchors.centerIn: parent
            }
        }

        Rectangle{
            id: resultRect
            width: widthArray[ 1 ]
            height: perRowHeight
            color: contentRectColor
            Text {
                text: result
                font { family: "Digital-7 Mono"; pixelSize: 68 * factor }
                color: "white"
                anchors.centerIn: parent
            }
        }

        Rectangle{
            id: ruleResultRect
            width: widthArray[ 2 ]
            height: perRowHeight
            color: contentRectColor
            Text {
                text: ruleResult
                font { family: "Microsoft YaHei"; pixelSize: 68 * factor }
                color: "white"
                anchors.centerIn: parent
            }
        }

        Rectangle{
            id: scoreRect
            width: widthArray[ 3 ]
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
