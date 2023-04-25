import QtQuick 2.0
import ProjectEnums 1.0

Rectangle {
    id: homeBtn
    width: 282 * factor
    height: 140 * factor
    radius: 30 * factor
    color: "#1F71CF"
    border.width: 3 * factor
    border.color: "#1ADDFC"

    property bool btnEnable: true
    signal signalBtnClicked()

    Image {
        id: btnImg
        width: 69 * factor
        height: 63 * factor
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 40 * factor
        source: "qrc:/pic/21-home.png"
    }

    Text {
        id: btnText
        text: qsTr("首页")
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        font.bold: true
        color: "white"
        anchors.left: btnImg.right
        anchors.leftMargin: 10 * factor
        anchors.verticalCenter: btnImg.verticalCenter
    }

    MouseArea {
        anchors.fill: parent
        enabled: btnEnable
        onClicked: {
            signalBtnClicked()
        }
    }
    Connections{
        target: InterAction
        //后端控制返回按钮是否可点
        onSigSetBtnEnable: {
            if(btnType === Enums.OTO_TEST_BACK_INFO_BTN)
            {
                btnEnable = bEnable
                if(btnEnable)
                {
                    homeBtn.color = "#1F71CF"
                }
                else
                {
                    homeBtn.color = "#999999"
                }
            }
        }
    }
}
