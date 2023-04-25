import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//import QtQuick.VirtualKeyboard 2.4
//import QtQuick.VirtualKeyboard.Settings 2.2

Item {
    id: root
    width: 1760 * factor
    height: 1700 * factor

    property string titleText: "录入信息"
    property string tipText: "请输入个人编号"
    property bool   isPasswdInput: false
    signal sigConfirmBtnClicked( var content )

    Rectangle{
        width: 1580 * factor
        height: 780 * factor
        anchors.centerIn: parent
        color: "#6A76F5"

        Rectangle{
            id: topTitleRect
            width: parent.width
            height: 200 * factor
            color: "#5049E5"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                text: titleText
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 60 * factor
            }

            Image {
                id: closeImg
                width: 100 * factor
                height: width
                source: "qrc:/pic/22-CloseBtn.png"
                anchors.right: parent.right
                anchors.rightMargin: 50 * factor
                anchors.verticalCenter: parent.verticalCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        close()
                    }
                }
            }
        }

        TextField{

            id: userIdInput
            width: 1100 * factor
            height: 170 * factor
            color: "white"
            placeholderText: tipText
            echoMode: isPasswdInput ? TextInput.Password : TextInput.Normal
            placeholderTextColor: "gray"
            font{ family: "Microsoft YaHei";pixelSize: 60 * factor }
            anchors.left: parent.left
            anchors.leftMargin: 40 * factor
            anchors.verticalCenter: parent.verticalCenter
            background: Rectangle{
                id: rect
                width: 1100 * factor
                height: 170 * factor
                radius: 30 * factor
                color: "#0C3DA4"
                border.color: "#1ADDFC"
            }
        }

        Button{
            id: sureBtn
            width: 340 * factor
            height: 170 * factor
            anchors.verticalCenter: userIdInput.verticalCenter
            anchors.left: userIdInput.right
            anchors.leftMargin: 41 * factor
            background: Rectangle{
                anchors.fill: parent
                color: "#1F71CF"
                border.color: "#1ADDFC"
                radius: 30 * factor
                Text {
                    text: "确定"
                    color: "white"
                    font{ family: "Microsoft YaHei"; pixelSize: 60 * factor }
                    anchors.centerIn: parent
                }
            }
            onClicked: {
                sigConfirmBtnClicked( userIdInput.text )
            }
        }
    }

    /*

    Rectangle{
        id: keyBordArea
        width: 1760 * factor
        height: 970 * factor
        radius: 30 * factor
        color: "gray"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 250 * factor
        visible: false
    }

    Rectangle {
        id: closeBtn
        width: 170 * factor
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: keyBordArea.top
        anchors.topMargin: 100 * factor
        radius: width / 2
        color: "#DAD4D1"
        Image {
            width: 62 * factor
            height: width
            source: "qrc:/pic/22-CloseBtn.png"
            anchors.centerIn: parent
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                close()
            }
        }
    }
    */

    /*
    InputPanel {
        id: inputPanel
        z: 99
        x: keyBordArea.x
        y: -parent.height//keyBordArea.y
        width: keyBordArea.width
        visible: false
        Component.onCompleted: {
            VirtualKeyboardSettings.activeLocales = ["en_GB","zh_CN"]
            VirtualKeyboardSettings.locale = "en_GB"
        }

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: keyBordArea.y
            }
            PropertyChanges {
                target: closeBtn
                anchors.top: keyBordArea.bottom
            }
        }
    }
    */

    function clear(){
        userIdInput.clear()
    }
}
