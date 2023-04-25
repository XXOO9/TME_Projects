/*************************************************
  <文件名称>    BindDeviceTipPopup.qml
  <功能描述>    设备绑定示意弹窗

  <作    者>    tcy
  <时    间>    2022-04-02
**************************************************/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../"
import "."

Item {
    id : root
    width: 1920 * factor
    height: 2880 * factor

    //nfc识别倒计时
    property int nfcRecogSecond: 10
    property alias popupVisible: pop.visible
    property int imgNum: 1

    //nfc刷卡倒计时
    Timer {
        id: runMidNFCRecogTimer
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            if(nfcRecogSecond-1 >= 0)
            {
                nfcRecogSecond--;
            }
            else
            {
                //绑定超时
                InterAction.nfcValidTimeOut()
                hide()
            }
        }
    }

    Popup{
        id: pop
        modal: true
        visible: false
        width: root.width
        height: root.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnPressOutside
        onClosed: {
            runMidNFCRecogTimer.stop()
        }

        background:  Item {
            width: parent.width
            height: parent.height + 100 * factor
            anchors.centerIn: parent

            Rectangle{
                anchors.fill: parent
                color: "black"
                opacity: 0.8
                anchors.centerIn: parent

                Text {
                    width: parent.width * 0.7
                    wrapMode: Text.WrapAnywhere
                    id: tipText
                    text: qsTr(InterAction.curRecogUserName + "同学，请进行设备配对")
                    font.bold: true
                    font.family: "PingFang SC"
                    color: "white"
                    font.pixelSize: 100 * factor
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: nfcTimerCount.top
                    anchors.bottomMargin: 50 * factor
                }

                Text {
                    id: nfcTimerCount
                    text: nfcRecogSecond
                    font.pixelSize: 160 * factor
                    font.family: "Digital-7 Mono"
                    font.bold: true
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 350 * factor
                }

                Text {
                    id: timeUnit
                    text: "s"
                    font.bold: true
                    color: "white"
                    font.pixelSize: 124 * factor
                    anchors.left: nfcTimerCount.right
                    anchors.leftMargin: 20 * factor
                    anchors.bottom: nfcTimerCount.bottom
                }

                Rectangle {
                    id: closeBtn
                    width: 170 * factor
                    height: width
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 175 * factor
                    radius: width / 2
                    color: "#DAD4D1"
                    Image {
                        width: 62 * factor
                        height: width
                        source: "qrc:/pic/22-CloseBtn.png"
                        anchors.centerIn: parent
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            //取消绑定
                            InterAction.closeBindPopup()
                            hide()
                        }
                    }
                }
            }

            Image {
                id: runGudieImg
                width: parent.width - 200 * factor
                height: parent.height - 200 * factor
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -200 * factor
                anchors.horizontalCenter: parent.horizontalCenter
                source: GlobalProperty.iDevType === GlobalProperty.devRopeSkip? "qrc:/pic/skipGuide/14.png" : "qrc:/pic/runGuide/14.png"
                z: 1
            }

            Image {
                id: ropeItem
                width: 122 * factor
                height: 277 * factor

                opacity: 1
                property int maxWidth: runGudieImg.width / 2
                property int minWidth: runGudieImg.width / 4

                visible: GlobalProperty.iDevType === GlobalProperty.devRopeSkip? true : false
                anchors.verticalCenter: parent.verticalCenter
                x: minWidth
                source: "qrc:/pic/skipGuide/rope.png"
                z: 10

                SequentialAnimation on x {
                    loops: Animation.Infinite

                    NumberAnimation {
                        from: ringItem.minWidth; to: ringItem.maxWidth
                        easing.type: Easing.OutExpo; duration: 1500
                    }

                    // Then pause for 500ms
                    PauseAnimation { duration: 500 }
                }
            }

            Image {
                id: ringItem
                width: 191 * factor
                height: 199 * factor
                opacity: 1
                property int maxWidth: (runGudieImg.width - 200* factor) / 2
                property int minWidth: runGudieImg.width / 6

                visible: GlobalProperty.iDevType !== GlobalProperty.devRopeSkip? true : false
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -50 * factor
                x: minWidth
                source: "qrc:/pic/runGuide/ring.png"
                z: 10

                SequentialAnimation on x {
                    loops: Animation.Infinite

                    NumberAnimation {
                        from: ringItem.minWidth; to: ringItem.maxWidth
                        easing.type: Easing.OutCubic ; duration: 1500
                    }

                    // Then pause for 500ms
                    PauseAnimation { duration: 500 }
                }
            }
        }
    }

    function show(){
        runMidNFCRecogTimer.start()
        InterAction.curPage = Enums.PAGE_POPUP
        popupVisible = true;
    }

    function hide(){
        runMidNFCRecogTimer.stop()
        InterAction.curPage = GlobalProperty.pageBeforoPopups
        popupVisible = false;
    }
}
