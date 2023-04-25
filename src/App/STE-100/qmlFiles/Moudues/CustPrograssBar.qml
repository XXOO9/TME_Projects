import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "."
import "../"
Item {
    id: root

    readonly property real totalLength: 1440 * factor
    property int barHeight: 28 * factor
    property real loadedLength: 0.0
    property string baseColor: "#6B78EF"
    property string loadedColor: "#21BBA9"
    property real loadedPersent: 0.01//rectLoadedProgressBar.width / totalLength
    property real targetLength: 0.0

    signal sigLoadFinished()

    signal sigLoadError()

    Component.onCompleted: {
        autoAddLength( 0.99 )
    }

    width:  totalLength
    height: barHeight

    //进度条总长度
    Rectangle{
        id : rectBaseProgressBar
        width: totalLength
        height: barHeight
        color: baseColor
        radius:  14 * factor
        anchors.fill: parent
        clip: true
        layer.enabled: true
        layer.effect: OpacityMask{
            maskSource: Rectangle{
                width: totalLength
                height: barHeight
                radius: 14 * factor
            }
        }

        //已加载的部分
        Rectangle{
            id : rectLoadedProgressBar
            width: totalLength
            height: barHeight
            color: loadedColor
            radius: 14 * factor
            x: -totalLength
            anchors.left: rectBaseProgressBar.left
            anchors.verticalCenter: rectBaseProgressBar.verticalCenter

            XAnimator on x{ //水平方向移动的动画
                id: animaltion
                from: -totalLength
                to: loadedLength
                duration: 3000
                onFinished: {
                    if( targetLength >= totalLength ){
                        sigLoadFinished()
                    }
                }
            }

            /*
            PropertyAnimation on width{
                id: animaltion
                running: false
                from: loadedLength
                to: targetLength
                duration: 3000
                onFinished: {
                    loadedLength = rectLoadedProgressBar.width
                    if( loadedLength >= totalLength ){
                        sigLoadFinished()
                    }
                }
            }
            */

        }
    }

    Timer {
        id: persentTimer
        interval: 30
        repeat: true
        running: true
        onTriggered: {
            if(animaltion.running)
            {
                if(loadedPersent + 0.01 <1.0)
                    loadedPersent += 0.01
            }
            else
            {
                loadedPersent = 1
                persentTimer.stop()
            }
        }
    }


    Timer{
        id: m_timer
        repeat: false
        interval: 5000
        onTriggered: {
            if( "unKnown" === GlobalProperty.devType ){
                sigLoadError()
            }else{
                sigLoadFinished()
            }
        }
    }

    function autoAddLength( selfCheckPersent ){
        if( selfCheckPersent >= 0.99 ){
            selfCheckPersent = 0.99
        }

        if( selfCheckPersent === 0.99 ){
            m_timer.start()
        }

        targetLength = selfCheckPersent * totalLength
        //animaltion.start()
    }


    Connections{
        target: InterAction
        onUpdateProcessVal:{
            autoAddLength( selfCheckPersent )
        }
    }
}
