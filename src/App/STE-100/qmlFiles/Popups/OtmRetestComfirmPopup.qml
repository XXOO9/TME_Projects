import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../Moudues"

Item {
    id: root

    signal sigRetestBtnClicked()
    signal sigCancelRetestBtnClicked()

    Popup{
        id: m_popup
        width: 1262 * factor
        height: 648 * factor
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose
        anchors.centerIn: parent
        background: Rectangle{
            anchors.fill: parent
            radius: 30 * factor
            color: "#768CF7"
        }


        contentItem: Item {
            anchors.fill: parent
            Rectangle{
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
                //填充满底部的矩形
                Rectangle{
                    width: parent.width
                    height: 40 * factor
                    color: parent.color
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
            Text {
                text: "是否重新开始测试？"
                color: "white"
                font{ pixelSize: 70 * factor; family: "Microsoft YaHei" ; bold: true}
                anchors.centerIn: parent
            }

            //确定重测按钮
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
                    text: "是"
                    color: "white"
                    font{ pixelSize: 62 * factor; family: "Microsoft YaHei" }
                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        sigRetestBtnClicked()
                    }
                }
            }

            //取消重测按钮
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
                    text: "否"
                    color: "white"
                    font{ pixelSize: 62 * factor; family: "Microsoft YaHei" }
                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        close()
                    }
                }
            }
        }
    }

    function open(){
        m_popup.open()
    }

    function close(){
        m_popup.close()
    }
}
