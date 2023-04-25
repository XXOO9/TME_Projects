import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    width: 564 * factor
    height: 168 * factor
    state: "active"
    //state: mouseEnable ? "active" : "inactive"

    property string displayText: "提  交"
    property string btnColorNormal: "#EA825B"
    property string btnColorInvaild: "#B06244"
    property bool   mouseEnable: true

    signal sigBtnClicked()

    Rectangle{
        id: rect
        anchors.fill: parent
        radius: 84 * factor
        color: "#EA825B"

        Text {
            id: btnText
            text: displayText
            font{family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true; wordSpacing: 30 * factor}
            anchors.centerIn: parent
            color: "white"
        }

        MouseArea{
            id: btnMouse
            anchors.fill: parent
            enabled: mouseEnable
            onClicked: {
                sigBtnClicked()
            }
        }
    }

    states: [
        State {
            name: "active"
            PropertyChanges {
                target: btnMouse
                enabled: mouseEnable
            }
            PropertyChanges {
                target: rect
                color: btnColorNormal
            }
            PropertyChanges {
                target: btnText
                color: "white"
            }

        },
        State{
            name: "inactive"
            PropertyChanges {
                target: btnMouse
                enabled: false
            }
            PropertyChanges {
                target: rect
                color: btnColorInvaild
            }
            PropertyChanges {
                target: btnText
                color: "gray"
            }
        }
    ]

    function setActive(){
        root.state = "active"
    }

    function setInactive(){
        root.state = "inactive"
    }
}
