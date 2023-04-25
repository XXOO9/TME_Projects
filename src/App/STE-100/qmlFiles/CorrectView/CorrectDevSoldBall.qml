import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../"
import "../Moudues/"
import "../JavaScript/ProjectValue.js" as Common

Item {

    id: root

    width: 1245 * factor * 0.95
    height: 1952 * factor * 0.95

    Rectangle{
        id: bkg
        anchors.fill: parent
        radius: 5 * factor
        color: "#4D39DB"
    }

    Text {
        width: parent.width
        wrapMode: Text.WrapAnywhere
        text: qsTr("请依次执行实心球校准步骤( 总共1步 )")
        color: "white"
        font.pixelSize: 50 * factor
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        anchors{ horizontalCenter: parent.horizontalCenter;  top: parent.top; topMargin: 150 * factor }
    }

    Column{
        spacing: 10 * factor
        anchors{ horizontalCenter: parent.horizontalCenter;  top: parent.top; topMargin: 250 * factor }
        CorrectValInput{
            width: 600 * factor
            height: 300 * factor
            correctName: "0~100 校准实心球(单位: cm)"
            correctVal: "0"
            regExpObj: inputRegExp
            tipsText: "单位: cm"
            correctBtnName: "定标"
            clickeEnable: true
            onSigClicked: {
                InterAction.setCurCorrectItem( Enums.ITEM_SOLIDBALL )
                InterAction.setCorrectProgress( 0, Common.curCorrectDevNumber, correctVal )
            }
        }
    }

    RegExpValidator{
        id: inputRegExp
        regExp: /^(100|[1-9][0-9]|[0-9])$/
    }
}
