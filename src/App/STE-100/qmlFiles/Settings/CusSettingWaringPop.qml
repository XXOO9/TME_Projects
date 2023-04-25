import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"
Popup{
    id: root
    width: 960 * factor
    height: 530 * factor

    signal sigOk()
    signal sigCancel()

    property real nextPageIndex: 0

    padding: 0

    background: Rectangle{
        color: '#0C3DA4'
        anchors.fill: parent
        radius: 30 * factor
    }

    Text {
        id: tips
        text: '是否保存当前设置?'
        color: 'white'
        font{ family: "Microsoft YaHei"; pixelSize: 90 * factor; bold: true }
        anchors{ top: parent.top; topMargin: 50 * factor; horizontalCenter: parent.horizontalCenter }
    }

    Row{
        width: 500 * factor
        spacing: 40 * factor
        anchors{ horizontalCenter: tips.horizontalCenter; top: tips.bottom; topMargin: 80 * factor }
        Repeater{
            model: [ '是', '否' ]
            delegate: cusClickedBtn
        }
    }

    Component{
        id: cusClickedBtn
        Rectangle{
            id: clickedBtn
            width: 230 * factor
            height: 134 * factor
            radius: 50 * factor
            color: index === 0 ? '#EA825B' : '#009E96'
            Text {
                text: modelData
                color: 'white'
                font{ family: 'Microsoft YaHei'; pixelSize: 70 * factor; bold: true }
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea{
                id: btnMouseArea
                anchors.fill: parent
                onClicked: index === 0 ? sigOk() : sigCancel()
            }

            states: [
                State{
                    when: btnMouseArea.pressed
                    PropertyChanges{ target: clickedBtn; opacity: 0.9 }
                }

            ]
        }
    }
}
