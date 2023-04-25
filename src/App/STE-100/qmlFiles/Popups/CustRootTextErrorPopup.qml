import QtQuick 2.12
import QtQuick.Controls 2.12
import "../JavaScript/ProjectValue.js" as Assistant

Popup {
    id: root

    implicitWidth: 1200 * factor
    implicitHeight: 800 * factor

    padding: 0
    closePolicy: Popup.NoAutoClose

    onOpened:{
        displayText.text = Assistant.rootErrorPopup.textErrorInfo
        autoCloseTimer.start()
    }


    background: Rectangle{
        anchors.fill: parent
        color: "skyblue"
    }

    Timer{
        id: autoCloseTimer
        interval: 1500
        repeat: false
        onTriggered: {
            closeRootPopup()
        }
    }

    Text {
        id: displayText
        text: "null"
        color: "white"
        anchors.centerIn: parent
        font{ family: "Microsoft YaHei"; pixelSize: 150 * factor; bold: true }
    }
}
