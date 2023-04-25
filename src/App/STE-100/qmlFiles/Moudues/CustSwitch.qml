import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"
Item {
    id: root
    width: 300 * factor
    height: 300 * factor

    property alias displayText: displayText.text
    property alias  isChecked: control.checked
    property alias mouseEnable: control.checkable

    property string checkedTrueText: '测量'
    property string checkedFalseText: '不测量'
    property string checkedTrueColor: '#65CE67'
    property string checkedFalseColor: "#ffffff"

    property alias switchWidth: control.width
    property alias switchHeight: control.height

    signal sigSwitchChanged( var isSwitch )
    signal sigClickedWhileMouseDisable()


    Text {
        id: displayText
        width: parent.width
        height: parent.height - control.height
        text: "null"
        color: "white"

        font{ family: "Microsoft YaHei"; pixelSize: 60 * factor }
        anchors.left: parent.left
        anchors.top: parent.top
        padding: 0
    }


    Switch {
        id: control
        text: qsTr("Switch")
        width: 200 * factor
        height: 100 * factor
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 30 * factor
        padding: 0

        indicator: Rectangle {
            id: rect
            implicitWidth: 200 * factor
            implicitHeight: 80 * factor
            x: control.leftPadding
            y: parent.height / 2 - height / 2

            radius: parent.height
            color: control.checked ? checkedTrueColor : checkedFalseColor
            border.color: control.checked ? "green" : "#cccccc"

            Rectangle {
                id : smallRect
                width: parent.height
                height: width
                radius: width / 2
                color: control.down ? "#cccccc" : "#ffffff"
                border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"

                NumberAnimation on x{
                    to: rect.width - smallRect.width
                    running: control.checked ? true : false
                    duration: 200
                }

                NumberAnimation on x{
                    to: 0
                    running: control.checked ? false : true
                    duration: 200
                }
            }
        }

        //要显示的文本
        contentItem: Text {
            text: control.checked ? checkedTrueText : checkedFalseText
            font.pixelSize: 60 * factor
            font.family: "Microsoft YaHei"
            //鼠标按下时  control.down
            color: "white"
            verticalAlignment: Text.AlignVCenter
            anchors.left: control.right
            anchors.leftMargin: 50 * factor
        }

        onPositionChanged: {
            sigSwitchChanged( checked )
        }
    }

    states: [
        State{
            name: 'noText'
            PropertyChanges{ target: displayText; visible: false }
            AnchorChanges{ target: control; anchors.horizontalCenter: root.horizontalCenter; anchors.verticalCenter: root.verticalCenter }
        }

    ]

    function setSwitchState( isSwitch ){
        control.checked = isSwitch
    }

    function set_switch_on(){
        control.checked = true
    }

    function set_switch_off(){
        control.checked = false
    }
}

