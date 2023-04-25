import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../"

Item {
    id: root
    width: 1000 * factor
    height: 300 * factor

    property alias value: contentInput.text

    signal  sigOnOkBtnClicked()

    Text {
        id: ropeSkipDelayTimeText
        text: "超时等待(s)"
        color: "white"
        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
    }

    TextField{
        id: contentInput
        width: 860 * factor
        height: 140 * factor
        color: "white"
        placeholderText: "配置跳绳超时等待(5-30)s"
        placeholderTextColor: "gray"
        font{ family: "Microsoft YaHei";pixelSize: 60 * factor }
        validator: RegExpValidator{regExp: /([5-9]|[1-2][0-9]|30)$\d*/ }//RegExpValidator{regExp: /^[0-90000/]+$/}
        maximumLength: 5
        text: GlobalProperty.devRopeSkipRecvDelay
        anchors.verticalCenter: ropeSkipDelayTimeText.verticalCenter
        anchors.left: ropeSkipDelayTimeText.right
        anchors.leftMargin: 50 * factor
        background: Rectangle{
            id: rect
            width: 860 * factor
            height: 140 * factor
            radius: 30 * factor
            color: "#0C3DA4"
            border.color: "#1ADDFC"
        }
    }

    Button{
        id: sureBtn
        width: 200 * factor
        height: 140 * factor
        visible: GlobalProperty.devRopeSkipRecvDelay != contentInput.text * 1 ? true:false
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: contentInput.right
        anchors.leftMargin: 10 * factor
        background: Rectangle{
            anchors.fill: parent
            color: "#1F71CF"
            border.color: "#1ADDFC"
            radius: 30 * factor
            Text {
                text: "确定"
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 60 * factor }
                anchors.centerIn: parent
            }
        }

        onClicked: {
            GlobalProperty.devRopeSkipRecvDelay = contentInput.text * 1;
            onOkBtnClicked()
        }
    }

    function onOkBtnClicked(){
        sigOnOkBtnClicked()
    }
}
