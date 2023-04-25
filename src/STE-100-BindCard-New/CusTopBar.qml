import QtQuick 2.12
import QtQuick.Controls 2.12
Item {
    id: root
    width: 1920 * factorX
    height: 110 * factorY

    Rectangle{
        anchors.fill: parent
        color: '#31466A'
    }

    Image {
        width: 57 * factorX
        height: 64 * factorY
        source: 'qrc:/pics/Logo.png'
        fillMode: Image.PreserveAspectFit
        anchors{ left: parent.left; leftMargin: 30 * factorX; verticalCenter: parent.verticalCenter }
    }


    Item{
        id: currentDateTimeArea
        width: 175 * factorX
        height: parent.height
        anchors{ right: parent.right; verticalCenter: parent.verticalCenter; rightMargin: 30 * factorX }

        Text {
            id: dateArea
            text: '--'
            font{ family: "Microsoft YaHei"; pixelSize: 16 * factorX }
            color: '#FFFFFF'
            anchors{ top: parent.top; topMargin: 27 * factorY; right: parent.right; rightMargin: 30 * factorX}
        }

        Text {
            id: timeArea
            text: '--'
            font{ family: "Microsoft YaHei"; pixelSize: 30 * factorX; bold: true }
            color: '#FFFFFF'
            anchors{ top: parent.top; topMargin: 56 * factorY; right: parent.right; rightMargin: 30 * factorX}
        }

        Timer{
            interval: 1000
            repeat: true
            running: true

            onTriggered: getCurrentTime()
        }
    }

    MouseArea{
        anchors.fill: parent
        onDoubleClicked: Qt.quit()
    }

    function getCurrentTime(){
        let currentDateTime = Qt.formatDateTime( new Date(), 'yyyy-MM-dd ddd+hh:mm:ss ' )
        let tmpArray = currentDateTime.split( '+' )
        dateArea.text = tmpArray[ 0 ]
        timeArea.text = tmpArray[ 1 ]
    }
}
