import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../"
import "../Moudues/"
import "../JavaScript/ProjectValue.js" as Common

Item{
    id: root
    property var stateArray: []
    property int curStateIndex: 0

    width: 1245 * factor * 0.95
    height: 1952 * factor * 0.95

    Rectangle{
        id: bkg
        anchors.fill: parent
        radius: 5 * factor
        color: "#4D39DB"
    }

    Text {
        text: qsTr("请依次执行握力校准步骤( 总共4步 )")
        color: "white"
        font.pixelSize: 70 * factor
        font.bold: true
        anchors{ horizontalCenter: parent.horizontalCenter;  top: parent.top; topMargin: 150 * factor }
    }

    Column{
        spacing: 10 * factor
        anchors{ horizontalCenter: parent.horizontalCenter;  top: parent.top; topMargin: 250 * factor }
        CorrectValInput{
            width: 600 * factor
            height: 300 * factor
            correctName: "校准握力(单位: kg)"
            correctVal: "0"
            tipsText: "单位: kg"
            correctBtnName: "确定"
            clickeEnable: curStateIndex === 0
            onSigClicked: {
                curStateIndex = 1
                InterAction.setCurCorrectItem( Enums.ITEM_GRIP )
                InterAction.setCorrectProgress( 0, Common.curCorrectDevNumber, correctVal )
            }
        }

        CorrectValInput{
            width: 600 * factor
            height: 300 * factor
            correctName: "校准握力(单位: kg)"
            correctVal: "5"
            tipsText: "单位: kg"
            correctBtnName: "确定"
            clickeEnable: curStateIndex === 1
            onSigClicked: {
                curStateIndex = 2
                InterAction.setCurCorrectItem( Enums.ITEM_GRIP )
                InterAction.setCorrectProgress( 1, Common.curCorrectDevNumber, correctVal )
            }
        }

        CorrectValInput{
            width: 600 * factor
            height: 300 * factor
            correctName: "校准握力(单位: kg)"
            correctVal: "10"
            tipsText: "单位: kg"
            correctBtnName: "定标"
            clickeEnable: curStateIndex === 2
            onSigClicked: {
                curStateIndex = 3
                InterAction.setCurCorrectItem( Enums.ITEM_GRIP )
                InterAction.setCorrectProgress( 2, Common.curCorrectDevNumber, correctVal )
            }
        }

        CorrectValInput{
            width: 600 * factor
            height: 300 * factor
            correctName: "校准握力(单位: kg)"
            correctVal: "15"
            tipsText: "单位: kg"
            correctBtnName: "定标"
            clickeEnable: curStateIndex === 3
            onSigClicked: {
                curStateIndex = 0
                InterAction.setCurCorrectItem( Enums.ITEM_GRIP )
                InterAction.setCorrectProgress( 2, Common.curCorrectDevNumber, correctVal )
            }
        }
    }

}
