import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    id: root
    width: 960 * factor
    height: 330 * factor

    property string tipString: '设置成功'
    property alias  fontSize: contentText.font.pixelSize

    background: Rectangle{
        color: 'skyblue'
        anchors.fill: parent
        radius: 30 * factor
    }

    Text {
        id: contentText
        text: tipString
        color: 'white'
        font{ family: "Microsoft YaHei"; pixelSize: 90 * factor; bold: true }
        anchors{ top: parent.top; topMargin: 50 * factor; horizontalCenter: parent.horizontalCenter }
    }

    Timer{
        id: autoCloseTimer
        interval: 2000
        repeat: false
        onTriggered: root.close()
    }


    // publibc:
    function openAutoClosePop( displayTipsString = '' ){

        if( displayTipsString !== '' ){
            tipString = displayTipsString
        }

        autoCloseTimer.start()
        root.open()
    }
}
