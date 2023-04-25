import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Popups/"
import "../Moudues/"
import "../ConfigObject/"
import "../JavaScript/ProjectValue.js" as Common


Item {
    id: root
    width: 1200 * factor
    height: 800 * factor
    property var curPop

    Popup{
        id: loadingPop

        // 是否开启等待框超时
        property alias enableTimeoutTimer: loading.waitlongTimeAutoClose

        anchors.centerIn: parent
        closePolicy: Popup.NoAutoClose
        padding: 0
        modal: true

        onOpened: loading.startTimeoutTimer()

        background: Rectangle{
            anchors.fill: parent
            color: Common.colorTransparent
        }

        CustLoading{
            id: loading
            anchors.centerIn: parent
            onSigTimeout: closePopup()
        }
    }

    CustRootTextErrorPopup{
        id: textErrorPopup
        anchors.centerIn: parent
    }

    CusWaringPop{
        id: waringPop
        anchors.centerIn: parent
    }

    ToolAdjustSignPainter{
        id: adjustSignPainter
    }

    states: [
        State {
            name: "loadingPopup"
            PropertyChanges {
                target: popTypeLoader
                source: "qrc:/qmlFiles/Moudues/CustLoading.qml"
            }

            PropertyChanges {
                target: m_popup
                visible: true
            }
        },

        State{
            name: "userConfirmFailed"
            PropertyChanges {
                target: popTypeLoader
                source: "qrc:/qmlFiles/Popups/CustRootTextErrorPopup.qml"
            }

            PropertyChanges {
                target: m_popup
                visible: true
            }
        },

        State{
            name: "unFinished"
            PropertyChanges {
                target: popTypeLoader
                source: "qrc:/qmlFiles/Popups/CusWaringPop.qml"
            }

            PropertyChanges {
                target:  m_popup
                visible: true
            }
        },
        State{
            name: "adjustSign"
            PropertyChanges {
                target: adjustSignPainter
                anchors{ top: parent.top }
            }

            PropertyChanges {
                target:  m_popup
                visible: true
            }
        }

    ]

    function openSpecificPop( type ){
        switch( type ){
        case "loadingPopup":
            loadingPop.open()
            loading.waitlongTimeAutoClose = false
            return
        case "userConfirmFailed":
            textErrorPopup.open()
            return
        case "unFinished":
            waringPop.open()
            return
        case "adjustSign":
            adjustSignPainter.open()
            return

        case "loadingPopWithoutTimer":
            openLoadingPopWithoutAutoClose()
            return
        }
    }

    //开启没有超时自动关闭的弹窗
    function openLoadingPopWithoutAutoClose(){
        loading.waitlongTimeAutoClose = true
        loadingPop.open()
    }


    function closePopup(){
        loadingPop.close()
        textErrorPopup.close()
        waringPop.close()
        adjustSignPainter.close()
    }
}
