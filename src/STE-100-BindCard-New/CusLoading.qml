import QtQuick 2.12
import QtQuick.Controls 2.12
Item {
    id: control

    //item圆圈个数
    property int itemCount: 4
    //item圆圈直径
    property int itemSize: 25 * factorX
    //item圆圈颜色
    property color itemColor: "lightgreen"
    //转一次时长
    property int duration: 2500
    //
    property bool running: visible

    implicitHeight: 160 * factorY
    implicitWidth: 160 * factorX

    Item{
        id: content
        anchors.fill: parent
        anchors.margins: 5 + control.itemSize/2

        Repeater{
            id: repeater
            model: control.itemCount

            //旋转的小球
            Rectangle{
                id: item
                width: control.itemSize
                height: control.itemSize
                color: control.itemColor
                radius: width/2
                x: content.width/2 - control.itemSize/2 + content.width/2 * Math.sin(rotate/360*6.283185307179)
                y: content.height/2 - control.itemSize/2 - content.height/2 * Math.cos(rotate/360*6.283185307179)
                property real rotate: -index*20


                SequentialAnimation {
                    running: control.running
                    loops: Animation.Infinite
                    NumberAnimation{
                        duration: index*100
                    }
                    ParallelAnimation{
                        NumberAnimation{
                            target: item
                            property: "rotate"
                            from: -index*20
                            to: 360-index*20
                            duration: control.duration
                            easing.type: Easing.OutCubic
                        }
                        SequentialAnimation{
                            NumberAnimation{
                                target: item
                                property: "opacity"
                                from: 0
                                to: 1
                                duration: control.duration*1/8
                            }
                            NumberAnimation{
                                duration: control.duration*3/4
                            }
                        }
                    }
                    NumberAnimation{
                        duration: (control.itemCount-index)*100
                    }
                }
            }
        }
    }

    Text {
        text: "请稍后..."
        color: "white"
        font{ family: 'fontMicrosoft YaHei'; pixelSize: 16 * factorX }
        anchors.centerIn: parent
    }
}
