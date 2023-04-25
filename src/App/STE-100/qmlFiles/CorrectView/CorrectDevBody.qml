import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../"
import "../Moudues/"
import "../JavaScript/ProjectValue.js" as Common

Item{
    id: root
    property int  curWeightStepIndex: 0
    property int  devNumber: -1
    property bool isCorrectHumanHeight: false

    width: 1245 * factor * 0.95
    height: 1952 * factor * 0.95

    Rectangle{
        id: bkg
        anchors.fill: parent
        radius: 5 * factor
        color: "#4D39DB"
    }


    Text {
        text: qsTr("依次执行身高校准步骤(总共1步)")
        color: "white"
        font.pixelSize: 35 * factor
        font.bold: true
        anchors{ left: parent.left; leftMargin: 40 * factor;  top: parent.top; topMargin: 150 * factor }
    }

    CorrectValInput{
        width: 400 * factor
        height: 300 * factor
        anchors{ left: parent.left; top: parent.top; leftMargin: 50 * factor; topMargin: 250 * factor }
        correctName: "校准身高(单位: cm)"
        correctVal: "100"
        tipsText: "单位: cm"
        correctBtnName: "定标"
        clickeEnable: true
        onSigClicked: {
            InterAction.setCurCorrectItem( Enums.ITEM_HEIGHT )
            InterAction.setCorrectProgress( 0, Common.curCorrectDevNumber, correctVal )
        }
    }

    Text {
        text: qsTr("依次执行体重校准步骤(总共2步)")
        color: "white"
        font.pixelSize: 35 * factor
        font.bold: true
        anchors{ right: parent.right; rightMargin: 90 * factor; top: parent.top; topMargin: 150 * factor }
    }

    Column{
        spacing: 20 * factor
        anchors{ right: parent.right; top: parent.top; rightMargin: 90 * factor; topMargin: 250 * factor }
        CorrectValInput{
            width: 500 * factor
            height: 300 * factor
            correctName: "校准体重第一步(单位:kg)"
            correctVal: "0"
            tipsText: "单位: kg"
            correctBtnName: "确定"
            clickeEnable: curWeightStepIndex === 0
            onSigClicked: {
                curWeightStepIndex = 1
                InterAction.setCurCorrectItem( Enums.ITEM_WEIGHT )
                InterAction.setCorrectProgress( 0, Common.curCorrectDevNumber, correctVal )
            }
        }

        CorrectValInput{
            width: 500 * factor
            height: 300 * factor
            correctName: "校准体重第二步(单位:kg)"
            correctVal: "50"
            tipsText: "单位: kg"
            correctBtnName: "定标"
            clickeEnable: curWeightStepIndex === 1
            onSigClicked: {
                curWeightStepIndex = 0
                InterAction.setCurCorrectItem( Enums.ITEM_WEIGHT )
                InterAction.setCorrectProgress( 1, Common.curCorrectDevNumber, correctVal )
            }
        }
    }

    Row{
        visible: false
        spacing: 5 * factor
        anchors.centerIn: parent
        Button{
            width: 400 * factor
            height: width
            text: "校准身高为100cm"
            onClicked: InterAction.correctHeight()
        }

        Button{
            width: 400 * factor
            height: width
            text: "校准体重为 0kg"
            onClicked: InterAction.correctWidth( 0 )
        }

        Button{
            width: 400 * factor
            height: width
            text: "校准体重为 50kg"
            onClicked: InterAction.correctWidth( 50 )
        }
    }




}
