import QtQuick 2.12
import QtQuick.Controls 2.12


Popup {

    signal signalContinueBtnClicked()
    signal signalQuitBtnClicked()

    property string errorString: "硬件设备出现异常, 请及时调试"


    id: m_popup
    width: 1262 * factor
    height: 648 * factor
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose

    background: Rectangle{
        anchors.fill: parent
        radius: 30 * factor
        color: "#768CF7"
    }

    contentItem:Item {
        anchors.fill: parent
        Rectangle{
            id: topArea
            width: m_popup.width
            height: 197 * factor
            color: "#726DED"
            radius: 30 * factor
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                text: "提示"
                color: "white"
                font{ pixelSize: 78 * factor; family: "Microsoft YaHei"; bold: true }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 60 * factor
            }
            Image {
                width: 62 * factor
                height: width
                source: "qrc:/pic/22-CloseBtn.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 60 * factor
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        m_popup.close()
                    }
                }
            }
            Rectangle{
                width: parent.width
                height: 40 * factor
                color: parent.color
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
        Text {
            text: errorString
            color: "white"
            font{ pixelSize: 70 * factor; family: "Microsoft YaHei" }
            anchors.centerIn: parent
        }

        Rectangle{
            id: btnContiune
            width: 422 * factor
            height: 140 * factor
            radius: 70 * factor
            color: "#2B41D0"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 81 * factor
            anchors.left: parent.left
            anchors.leftMargin: 180 * factor
            Text {
                text: "继续测试"
                color: "white"
                font{ pixelSize: 62 * factor; family: "Microsoft YaHei" }
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    signalContinueBtnClicked()
                }
            }
        }

        Rectangle{
            id: btnQuit
            width: btnContiune.width
            height: btnContiune.height
            radius: btnContiune.radius
            color: "#2B41D0"
            anchors.verticalCenter: btnContiune.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 180 * factor
            Text {
                text: "退出测试"
                color: "white"
                font{ pixelSize: 62 * factor; family: "Microsoft YaHei" }
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    signalQuitBtnClicked()
                }
            }
        }
    }

    function show(){
        m_popup.open()
    }

    function hiden(){
        m_popup.close()
    }
}
