import QtQuick 2.12
import QtQuick.Controls 2.12
import "../ConfigObject/"
import "./"
import "../"

Popup{
    id: m_pop
    width: 800 * factor
    height: width

    property string curBtnName: ""
    property bool   display: true
    property int    startPosX: 0
    property int    startPosY: 0

    closePolicy: Popup.NoAutoClose

//    onOpened: InterAction.startSignature( "1" )

    onClosed: onPopClosed()

    ListModel{
        id: btnList
        ListElement{ name: "wid+";  type: "size" }
        ListElement{ name: "up";    type: "pos" }
        ListElement{ name: "wid-";  type: "size" }

        ListElement{ name: "left";  type: "pos"}
        ListElement{ name: "close"; type: "control"}
        ListElement{ name: "right"; type: "pos"}

        ListElement{ name: "hei+";  type: "size"}
        ListElement{ name: "down";  type: "pos"}
        ListElement{ name: "hei-";  type: "size"}
    }

    Timer{
        id: pressTimer
        interval: 50
        repeat: true
        onTriggered: onTimerTimeout()
    }

    Grid{
        rows: 3
        columns: 3
        spacing: 10 * factor
        anchors.centerIn: parent

        Repeater{
            id: btnRepeater
            model: btnList
            delegate: Button{
                id: clickBtn
                width: 250 * factor
                height: width
                text: name
                font.pixelSize: 50 * factor
                onClicked: onBtnClicked( name )
                onDoubleClicked: onBtnDoubleClicked( name )
                onPressAndHold: onMousePressedAndHold( name )
                onPressed: opacity = 0.5
                onReleased: {
                    clickBtn.opacity = 1
                    pressTimer.stop()
                }

                background: Rectangle{
                    color: btnColorProvider( type )
                    Text {
                        text: btnTextProvider( index )
                        color: "white"
                        anchors.bottom: parent.bottom
                    }
                }
            }
        }
    }

    function btnTextProvider( index ){
        let ret = ""
        let wid = 0
        let hei = 0
        let x = 0
        let y = 0

        if( index === 0 ){
            wid = SignPainterConfig.otoDisplayWidth
            hei = SignPainterConfig.otoDisplayHeight
            x   = SignPainterConfig.otoXpos
            y   = SignPainterConfig.otoYpos

            ret = wid + "\n" + hei + "\n" + x + "\n" + y
        }

        if( index === 1 ){
            wid = SignPainterConfig.otmDisplayWidth
            hei = SignPainterConfig.otmDisplayHeight
            x   = SignPainterConfig.otmXpos
            y   = SignPainterConfig.otmYpos

            ret = wid + "\n" + hei + "\n" + x + "\n" + y
        }

        return ret
    }


    function btnColorProvider( type ){
        if( type === "size" ){
            return "lightgreen"
        }

        if( type === "pos" ){
            return "blue"
        }

        if( type === "control" ){
            return "gray"
        }
    }

    function onBtnClicked( btnName ){
        switch( btnName ){
        case "wid+":
            widChange( true )
            break
        case "wid-":
            widChange( false )
            break
        case "hei+":
            heiChange( true )
            break
        case "hei-":
            heiChange( false )
            break
        case "close":
            onControlSignPainter()
            break
        case "up":
            onUpmove()
            break
        case "down":
            onDownMove()
            break
        case "left":
            onLeftMove()
            break
        case "right":
            onRightMove()
            break

        default:
            return
        }
    }

    function onBtnDoubleClicked( name ){
        if( name === "close" ){
            m_pop.close()
        }
    }

    function onControlSignPainter(){
        if( display ){
            InterAction.startSignature( "1" )
        }else{
            InterAction.startSignature( "0" )
        }

        display = !display
    }

    function widChange( isAdd ){
        if( GlobalProperty.bDirectMode ){
            if( isAdd ){
                SignPainterConfig.otoDisplayWidth += 10
            }else{
                SignPainterConfig.otoDisplayWidth -= 10
            }
            SignPainterConfig.changeOtoSignPainter()
        }else{
            if( isAdd ){
                SignPainterConfig.otmDisplayWidth += 10
            }else{
                SignPainterConfig.otmDisplayWidth -= 10
            }
            SignPainterConfig.changeOtmSignPainter()
        }
    }

    function heiChange( isAdd ){
        if( GlobalProperty.bDirectMode ){
            if( isAdd ){
                SignPainterConfig.otoDisplayHeight += 10
            }else{
                SignPainterConfig.otoDisplayHeight -= 10
            }
            SignPainterConfig.changeOtoSignPainter()
        }else{
            if( isAdd ){
                SignPainterConfig.otmDisplayHeight += 10
            }else{
                SignPainterConfig.otmDisplayHeight -= 10
            }
            SignPainterConfig.changeOtmSignPainter()
        }
    }

    function onUpmove(){
        if( GlobalProperty.bDirectMode ){
            SignPainterConfig.otoYpos -= 5
            SignPainterConfig.changeOtoSignPainter()
        }else{
            SignPainterConfig.otmYpos -= 5
            SignPainterConfig.changeOtmSignPainter()
        }
    }

    function onDownMove(){
        if( GlobalProperty.bDirectMode ){
            SignPainterConfig.otoYpos += 5
            SignPainterConfig.changeOtoSignPainter()
        }else{
            SignPainterConfig.otmYpos += 5
            SignPainterConfig.changeOtmSignPainter()
        }
    }

    function onLeftMove(){
        if( GlobalProperty.bDirectMode ){
            SignPainterConfig.otoXpos -= 5
            SignPainterConfig.changeOtoSignPainter()
        }else{
            SignPainterConfig.otmXpos -= 5
            SignPainterConfig.changeOtmSignPainter()
        }
    }

    function onRightMove(){
        if( GlobalProperty.bDirectMode ){
            SignPainterConfig.otoXpos += 5
            SignPainterConfig.changeOtoSignPainter()
        }else{
            SignPainterConfig.otmXpos += 5
            SignPainterConfig.changeOtmSignPainter()
        }
    }

    function onMousePressedAndHold( name ){

        if( name === "close" ){
            m_pop.close()
        }

        if( pressTimer.running ){
            return
        }

        curBtnName = name
        pressTimer.start()
    }



    function onTimerTimeout(){
        onBtnClicked( curBtnName )
        console.log( "name = " + curBtnName )
    }

    function onPopClosed(){
        pressTimer.stop()
        toolLoader.source = ""
    }

}
