import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: control

    //item圆圈个数
    property int itemCount: 5
    //item圆圈直径
    property int itemSize: 80 * factor
    //item圆圈颜色
    property color itemColor: "lightgreen"
    //转一次时长
    property int duration: 2500
    //
    property bool running: visible

    property alias timeout: timeoutTimer.interval


    //是否开启超时自动关闭弹窗
    property bool waitlongTimeAutoClose: false

    implicitHeight: 500 * factor
    implicitWidth: 500 * factor

    signal sigTimeout()

    Item{
        id: content
        anchors.fill: parent
        anchors.margins: 5 + control.itemSize / 2

        Repeater{
            id: repeater
            model: control.itemCount

            //旋转的小球
            Rectangle{
                id: item
                width: control.itemSize
                height: control.itemSize
                color: control.itemColor
                radius: width / 2
                x: content.width / 2 - control.itemSize / 2 + content.width / 2 * Math.sin( rotate / 360 * 6.283185307179 )
                y: content.height / 2 - control.itemSize / 2 - content.height / 2 * Math.cos( rotate / 360 * 6.283185307179 )
                property real rotate: -index * 20


                SequentialAnimation {
                    running: control.running
                    loops: Animation.Infinite
                    NumberAnimation{
                        duration: index * 100
                    }
                    ParallelAnimation{
                        NumberAnimation{
                            target: item
                            property: "rotate"
                            from: -index * 20
                            to: 360 - index * 20
                            duration: control.duration
                            easing.type: Easing.OutCubic
                        }
                        SequentialAnimation{
                            NumberAnimation{
                                target: item
                                property: "opacity"
                                from: 0
                                to: 1
                                duration: control.duration * 1/8
                            }
                            NumberAnimation{
                                duration: control.duration * 3/4
                            }
                        }
                    }

                    NumberAnimation{
                        duration: (control.itemCount-index)*100
                    }
                }
            }
        }

        Text {
            text: "请稍后"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 80 * factor }
            anchors.centerIn: parent
        }

        //用于防止意外，导致弹窗一直不能自动关闭的情况
        Timer{
            id: timeoutTimer
            interval: waitlongTimeAutoClose ? 60 * 1000 : 10 * 1000
            repeat: false
            onTriggered: {
                if( !control.waitlongTimeAutoClose ){
                    sigTimeout()
                }
            }
        }
    }

    function startTimeoutTimer(){
        timeoutTimer.start()
    }
}
