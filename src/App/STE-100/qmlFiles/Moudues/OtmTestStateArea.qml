import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Item {
    id: root
    width: 1760 * factor
    height: 530 * factor

    state: "ready"

    property string tips: ""

    signal testFinished()

    Rectangle{
        id: stateArea
        anchors.fill: parent
        color: "#5C8EF3"
        visible: false

        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 333 * factor
            spacing: 38 * factor
            Repeater{
                id: statePoints
                model: 1
                Rectangle{
                    width: 30 * factor
                    height: width
                    radius: width / 2
                    border.color: "white"
                    border.width: 5 * factor
                    color: "#009944"
                }
            }
        }

        Rectangle{
            width: parent.width
            height: 100 * factor
            color: "#4D82EC"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: tipsText
                text: tips
                font {family: "苹方 中等"; pixelSize: 44 * factor }
                color: "#FFF45C"
                anchors.centerIn: parent
            }
        }
    }

    Rectangle{
        id: mask
        width: 1650 * factor
        height: 500 * factor
        radius: 30 * factor
        visible: false
    }

    OpacityMask{
        maskSource: mask
        source: stateArea
        anchors.fill: stateArea
    }

    Image {
        id: voiceImg
        width: 105 * factor
        height: 88 * factor
        source: "qrc:/pic/15-sound.png"
        anchors.left: parent.left
        anchors.leftMargin: 30 * factor
        anchors.top: parent.top
        anchors.topMargin: 30 * factor
    }

    Text {
        id: testStateText
        width: 500 * factor
        height: 120 * factor
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 140 * factor
        font{ family: "苹方 粗体"; pixelSize: 120 * factor }
    }

    states: [
        State {
            name: "ready"
            PropertyChanges {
                target: testStateText
                text: "准备测试"
            }
            PropertyChanges {
                target: statePoints
                model: 1
            }
        },
        State {
            name: "testing"
            PropertyChanges {
                target: testStateText
                text: "测试中"
            }
            PropertyChanges {
                target: statePoints
                model: 2
            }
        },
        State {
            name: "testFinished"
            PropertyChanges {
                target: testStateText
                text: "测试完成"
            }
            PropertyChanges {
                target: statePoints
                model: 3
            }
        }
    ]

//    MouseArea {
//        anchors.fill: parent
//        onDoubleClicked: {
//            setState("testFinished");
//            testFinished();
//        }

//        onClicked: {
//            setState("testing")
//        }
//    }

    function setState( val ){
        root.state = val
    }

    Connections{
        target: InterAction

        //更新测试状态
        onSendTestStateChangeSignal:{
            if(InterAction.testState === "请准备")
            {
                setState("ready");
            }
            else if(InterAction.testState === "测试中")
            {
                setState("testing");
            }
            else if(InterAction.testState === "测试完成")
            {
                setState("testFinished");
                testFinished();
            }
        }

        //更新提示语
        onSiganUpdateTestTip: {
            tips = qsTip;
        }

        //更新计时时间
        onSigUpdateTestCountTime: {
            testStateText.text = "测试中: " + String(time) + "s"
        }
    }
}
