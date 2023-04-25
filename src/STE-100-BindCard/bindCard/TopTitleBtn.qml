import QtQuick 2.12
import QtQuick.Controls 2.12
import "."

Rectangle{
    id:btn
    property var imgPath: []
    property string displayText: "null"
    property bool   isSelected: false

    signal sigClicked()

    implicitWidth: 170 * factor
    implicitHeight: 110 * factor
    state: isSelected ? "selected" : "unSelected"

    Image {
        id: img
        width: 40 * factor
        height: 26 * factor
        fillMode: Image.Stretch
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -30
        source: isSelected ? imgPath[0] : imgPath[1]
    }

    Text {
        id:display
        text: displayText
        font{ pixelSize: 18 * factor; family: "微软雅黑" }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30 * factor
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            if( !isSelected ){
                isSelected = true
                sigClicked()
            }
        }
    }

    states: [
        State {
            name: "selected"
            PropertyChanges {
                target: btn
                color: "#0068E1"
            }
            PropertyChanges {
                target: display
                color: "#BDE1F8"
            }
        },
        State{
            name: "unSelected"
            PropertyChanges {
                target: btn
                color: "transparent"
            }
            PropertyChanges {
                target: display
                color: "#5A8BA9"
            }
        }

    ]

}
