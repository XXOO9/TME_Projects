import QtQuick 2.0
import QtQuick.Controls 2.5
import FVCurveChartView 1.0
import VTCurveChartView 1.0
import RTCurveChartView 1.0
import ProjectEnums 1.0

Rectangle {
    id: root
    width: 1583 * factor
    height: 864 * factor
    radius: 30 * factor
    color: "#568BEA"

    SwipeView {
        id: view
        currentIndex: 0
        anchors.fill: parent
        anchors.centerIn: parent
        interactive: true
        clip: true
        Item {
            Text {
                id: vtText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 20 * factor
                text: qsTr("After bronchoddator use")
                font.pixelSize: 54 * factor
                font.family: "PingFang SC"
                font.bold: true
                color: "white"
            }
            Rectangle {
                width: parent.width
                height: 735 * factor
                radius: 20 * factor
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 25 * factor
                VTCurveChartView {
                    id: vtCurve
                    height: parent.height - 40 * factor
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20 * factor
                    Component.onCompleted: initCustomPlot()
                }
                Text {
                    text: qsTr("Time")
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 10
                }
                Text {
                    text: qsTr("Volume")
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 10
                }
            }
        }
        Item {
            Text {
                id: fvText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 20 * factor
                text: qsTr("Before bronchoddator use")
                font.pixelSize: 54 * factor
                font.family: "PingFang SC"
                font.bold: true
                color: "white"
            }
            Rectangle {
                width: parent.width
                height: 735 * factor
                radius: 20 * factor
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 25 * factor

                FVCurveChartView {
                    id: fvCurve
                    height: parent.height - 40 * factor
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20 * factor
                    Component.onCompleted: initCustomPlot()
                }
                Connections {
                    target: fvCurve
                    onSigStartInspiration: {
                        //insLabel.visible = true
                    }
                    onSigStartExpiration: {
                        //expLabel.visible = true
                    }
                }
                Text {
                    text: qsTr("Volume")
                    anchors.right: parent.right
                    anchors.rightMargin: 40 * factor
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 80 * factor
                }

                Text {
                    text: qsTr("Flow")
                    anchors.left: parent.left
                    anchors.leftMargin: 100 * factor
                    anchors.top: parent.top
                    anchors.topMargin: 10
                }

                Text {
                    id: expLabel
                    text: qsTr("Expiration")
                    anchors.right: parent.right
                    anchors.rightMargin: 100
                    anchors.top: parent.top
                    anchors.topMargin: 100
                    visible: false
                }

                Text {
                    id: insLabel
                    text: qsTr("Inspiration")
                    anchors.right: parent.right
                    anchors.rightMargin: 100
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 100
                    visible: false
                }
            }
        }

        Item {
            RTCurveChartView {
                id: rtCurve
                width: parent.width - 40 * factor
                height: parent.height - 40 * factor
                anchors.centerIn: parent
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                Component.onCompleted: initCustomPlot()
            }
        }
    }

    Connections {
        target: InterAction
        onSigAddPointsToPVVChart: {
            switch(chartType)
            {
            case Enums.VT_Chart:
                vtCurve.recvData(yVal)
                break
            case Enums.FV_Chart:
                fvCurve.recvData(stageType, xVal, yVal)
                break
            case Enums.RT_Chart:
                rtCurve.recvData(yVal)
                break
            default:
                InterAction.qmlLogout("肺通气量点数据类型未知")
            }
        }
        onSigChangePVVSampleRate: {
            vtCurve.setSampleRate(iSR)
            rtCurve.setSampleRate(iSR)
        }
    }

    PageIndicator {
      id: indicator
      count: view.count
      currentIndex: view.currentIndex
      anchors.bottom: view.bottom
      anchors.horizontalCenter: parent.horizontalCenter
      visible: false
    }
}
