import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"

Item {
    id: root

    width: 1920 * factor
    height: 170 * factor

    property string bakgColor: "#4756EB"
    property string systemDateTime: "00000"

    property int startMouseX: 0
    property int startMouseY: 0

    Rectangle{
        anchors.fill: parent
        color: bakgColor
    }

    //怡康logo
    Image {
        id: imgLogo
        width: 110 * factor
        height: width
        source: "qrc:/pic/4-LOGO.png"

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 80 * factor
    }

    //测试类型名
    Text {
        id: testingTypeName
        width: 324 * factor
        height: 92 * factor
        text: GlobalProperty.deviceName
        color: "white"
        font{ family: "MF YueYuan (Noncommercial)"; pixelSize: 100 * factor}
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        anchors.left: imgLogo.right
        anchors.leftMargin: 27 * factor
        anchors.verticalCenter: imgLogo.verticalCenter
    }

    //系统时间
    Text {
        id: systemTime
        width: 326 * factor
        height: 66 * factor
        text:systemDateTime
        color: "white"
        font{family: "Digital-7 Mono"; pixelSize: 100 * factor}
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        anchors.right: parent.right
        anchors.rightMargin: 76 * factor
        anchors.verticalCenter: imgLogo.verticalCenter

        Timer{
            interval: 1000
            repeat: true
            running: true
            triggeredOnStart: true
            onTriggered: {
                systemDateTime = curSystemDateTime()
            }
        }
    }

    MouseArea{
        anchors.fill: parent
        enabled: InterAction.isDeveloper
        onDoubleClicked: {
            Qt.quit()
        }

        onPressed: onMousePressed( mouse )
        onReleased: onMouseReleased( mouse )
        onPositionChanged: onMousePostionChanged( mouse )
    }

    function curSystemDateTime(){
        var tmp = Qt.formatDateTime( new Date, "hh:mm:ss" )
        return tmp
    }

    function onMousePressed( mouse ){
        startMouseX = mouse.x
        startMouseY = mouse.y
    }

    function onMouseReleased( mouse ){
        startMouseX = mouse.x
        startMouseY = mouse.y
    }

    function onMousePostionChanged( mouse ){
        if( !InterAction.isDeveloper ){
            return
        }

        let offsetX = mouse.x - startMouseX
        let offsetY = mouse.y - startMouseY

        moveWindow( offsetX, offsetY )
    }
}
