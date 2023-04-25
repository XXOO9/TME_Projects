import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import "."
import ".."
Item {
    id: root
    width: 1920 * factor
    height: 170 * factor

    property string strCurTime: ""
    property int    quitCount:1

    property int startMouseX: 0
    property int startMouseY: 0

    Component.onCompleted: {
        upDateTimeTimer.start()
    }

    Rectangle{
        id: backGround
        width: parent.width
        height: parent.height
        color: "transparent"
        anchors.fill: parent
    }

    Image {
        id: imgLogo
        width: 110 * factor
        height: width
        source: "qrc:/pic/4-LOGO.png"
        fillMode: Image.PreserveAspectCrop
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 135 * factor
    }

    Text {
        id: curTime
        width: 326 * factor
        height: 66 * factor
        text: strCurTime
        color: "white"
        font{ pixelSize: 100 * factor; family: "Digital-7 Mono" }
        anchors.right: parent.right
        anchors.rightMargin: 165 * factor
        anchors.verticalCenter: parent.verticalCenter

        Timer{
            id: upDateTimeTimer
            interval: 1000
            repeat: true
            triggeredOnStart: true
            onTriggered: {
                strCurTime = getCurrentTime()
            }
        }
    }

    Button{
        z:5
        text: '点不得'
        visible: InterAction.isDeveloper
        onClicked: turnSystemSetting()
    }

    //软件退出隐藏开关
    MouseArea{
        anchors.fill: parent
        enabled: InterAction.isDeveloper

        onDoubleClicked: {
            console.log( "ret = " + InterAction.isDeveloper )
            InterAction.quit()
        }

        onPressed: onMousePressed( mouse )
        onReleased: onMouseReleased( mouse )
        onPositionChanged: onMousePostionChanged( mouse )
    }


    function getCurrentTime(){
        return Qt.formatDateTime( new Date(), "hh:mm:ss" )
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
