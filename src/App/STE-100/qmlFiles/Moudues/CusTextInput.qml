import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"
Item {
    id: root
    width: 200 * factor
    height: 100 * factor

    property alias tips: tipsText.text

    signal sigClicked()


    Text {
        id: tipsText
        text: "标题:"
        color: "white"
        font.pixelSize: 48 * factor
        anchors{ top: parent.top; left: parent.left; leftMargin: 5 * factor }
    }

    TextField{
        id: textInput
        width: parent.width * 0.7
        height: parent.height * 0.6
        anchors{ top: tipsText.bottom; topMargin: 10 * factor; left: parent.left; leftMargin: 5 * factor }
    }

    Rectangle{
        id: confirmBtn
        width: parent.width * 0.2
        height: parent.height * 0.8
        border{ color: "lightskyblue"; width: 2 * factor }
        anchors{ verticalCenter: parent.verticalCenter }

        Text {
            text: "确定"
            font.pixelSize: 48 * factor
            color: "white"
            anchors{ verticalCenter: textInput.verticalCenter; right: parent.right; rightMargin: 5 * factor }
        }

        MouseArea{
            anchors.fill: parent
            onClicked: sigClicked()
        }
    }
}
