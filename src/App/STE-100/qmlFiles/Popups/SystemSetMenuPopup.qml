import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import ProjectEnums 1.0
import "../JavaScript/ProjectValue.js" as Common
import "../Moudues"
import "../"

Item {
    id: root
    width: 1582 * factor
    height: 1018 * factor

    signal sigOpenDevNumberPopup()
    signal sigOpenChannelConfigPopup()
    signal sigOpenCommitConfigPopup()
    signal sigOpenTestInfoConfigPopup()
    signal sigOpenCorrectDevPop()
    signal sigOpenOtherSettingsPop()

    Popup{
        id: m_popup
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        modal: true
        closePolicy: Popup.NoAutoClose
        padding: 0

        background: Rectangle{
            anchors.fill: parent
            color: "#6A76F5"
        }

        Rectangle{
            id: tipsRect
            width: parent.width
            height: 198 * factor
            color: "#4D39DB"
            Text {
                text: "系统设置"
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 60 * factor
            }

            Image {
                id: closeImg
                width: 70 * factor
                height: width
                source: "qrc:/pic/22-CloseBtn.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 30 * factor
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        close()
                    }
                }
            }
        }

        Grid{
            spacing: 50 * factor
            anchors.top: tipsRect.bottom
            anchors.topMargin: 50 * factor
            anchors.horizontalCenter: tipsRect.horizontalCenter
            columns: 3

            Component.onCompleted: {
                /*
                if( GlobalProperty.bDirectMode ){
                    configChannelPopupBtn.visible = false
                }else{
                    configDevNumberBtn.visible = false
                }
                */
            }

            OtmOperationBtn{
                id: configDevNumberBtn
                width: 400 * factor
                height: 250 * factor
                displayText: "设置分机编号"
                btnColor: "#2B41D0"
                textSize: 60
                textBold: true
                visible: GlobalProperty.iDevType !== GlobalProperty.devRunMid
                         && GlobalProperty.iDevType !== GlobalProperty.devRunShort
                         && GlobalProperty.iDevType !== GlobalProperty.devRopeSkip
                onSigBtnClicked: {
                    sigOpenDevNumberPopup()
                }
            }

            OtmOperationBtn{
                id: configChannelPopupBtn
                width: 400 * factor
                height: 250 * factor
                displayText: "信道配置"
                btnColor: "#2B41D0"
                textSize: 60
                textBold: true
                mouseAreaEnable: ! (GlobalProperty.bDirectMode && GlobalProperty.bDevConnect)
                onSigBtnClicked: {
                    sigOpenChannelConfigPopup()
                }
            }

            OtmOperationBtn{
                id: configCommitPopup
                width: 400 * factor
                height: 250 * factor
                displayText: "提交配置"
                btnColor: "#2B41D0"
                textSize: 60
                textBold: true
                onSigBtnClicked: {
                    sigOpenCommitConfigPopup()
                }
            }

            OtmOperationBtn{
                id: configTestInfoPopup
                width: 400 * factor
                height: 250 * factor
                displayText: "测试配置"
                btnColor: "#2B41D0"
                textSize: 60
                textBold: true
                visible: GlobalProperty.bDevConnect
                onSigBtnClicked: {
                    sigOpenTestInfoConfigPopup()
                }
            }

            OtmOperationBtn{
                id: correctDev
                width: 400 * factor
                height: 250 * factor
                displayText: "设备校准"
                btnColor: "#2B41D0"
                textSize: 60
                textBold: true
                visible: GlobalProperty.bDevConnect
                onSigBtnClicked: {
                    sigOpenCorrectDevPop()
                }
            }

            OtmOperationBtn{
                id: otherSettingBtn
                width: 400 * factor
                height: 250 * factor
                displayText: "其他设置"
                btnColor: "#2B41D0"
                textSize: 60
                textBold: true
                visible: GlobalProperty.bDevConnect
                onSigBtnClicked: {
                    sigOpenOtherSettingsPop()
                }
            }
        }
    }

    function open(){
        m_popup.open()
        InterAction.curPage = Enums.PAGE_SETTING
    }

    function close(){
        m_popup.close()
        InterAction.curPage = GlobalProperty.pageBeforoPopups
    }
}
