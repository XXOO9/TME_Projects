import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common
import ".."
import "../Moudues/"
import "../Popups"

CusSettingElementArea{
    id: root

    titleString: '提交配置'

    property alias signSwitchState: signatureSwitch.isChecked
    property alias delBtnswitchState: delScoreBtnSwitch.isChecked
    property alias countTime: autoUploadCountTimeSelecte.curValue
    property alias autoCommitCheckable: signatureSwitch.mouseEnable

    Column {
        spacing: 20 * factor
        anchors{ left: root.left; leftMargin: root.leftOffset; top: root.top; topMargin: 152 * factor }

        Row{
            spacing: 80 * factor
            Text {
                text: '是否可以取消成绩: '
                color: 'white'
                font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: delScoreBtnSwitch.verticalCenter
            }

            CustSwitch{
                id: delScoreBtnSwitch
                width:  200 * factor
                height: 100 * factor
                switchWidth: width
                switchHeight: height
                checkedFalseText: ''
                checkedTrueText: ''
                state: 'noText'
                onSigSwitchChanged: sigConfigChanged()
            }
        }


        Row{
            spacing: 80 * factor
            Text {
                text: '是否自动提交成绩: '
                color: 'white'
                font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: signatureSwitch.verticalCenter
            }

            CustSwitch{
                id: signatureSwitch
                width: 200 * factor
                height: 100 * factor
                switchWidth: width
                switchHeight: height
                checkedFalseText: ''
                checkedTrueText: ''
                state: 'noText'
                onSigSwitchChanged: sigConfigChanged()
                onSigClickedWhileMouseDisable: function(){
                    autoCloseTipsPop.openAutoClosePop()
                }
            }
        }

        Rectangle{
            id: connectedModel
            width: 1050 * factor
            height: 200 * factor
            radius: 20 * factor
            color: '#4756EB'
            visible: signatureSwitch.isChecked
            anchors.left: parent.left
            anchors.leftMargin: -20 * factor

            Row{
                spacing: 40 * factor
                anchors.left: parent.left
                anchors.leftMargin: 20 * factor
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: '自动提交成绩倒计时(s): '
                    color: 'white'
                    font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: autoUploadCountTimeSelecte.verticalCenter
                }

                CustValueRanger{
                    id: autoUploadCountTimeSelecte
                    width: 500 * factor
                    height: 100 * factor
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    CusAutoCloseTipsPop{
        id: autoCloseTipsPop
        fontSize: 75 * factor
        tipString: '异步模式下仅可手动提交'
        anchors.centerIn: parent
        modal: true
    }

    //打开警告弹窗
    function openWaringPop(){
        autoCloseTipsPop.openAutoClosePop()
    }

    //获取当前自动提交状态 true -> 自动提交,   false -> 手动提交
    function getAutoCommitCheckedState(){
        return signatureSwitch.isChecked
    }

    function setAutoCommitChecked( checkedState ){
        signatureSwitch.isChecked = checkedState
    }

    //设置该按钮是否可点击
    function setAutoCommitCheckeAble( checkAble ){
        signatureSwitch.mouseEnable = checkAble
    }

    function setSigNatureSwitchChecked( checkedState ){
        signatureSwitch.isChecked = !checkedState
    }


    function setDelBtnSwitchChecked( checkedState ){
        delScoreBtnSwitch.isChecked = checkedState
    }

    function setAutoCommitTime( time ){
        console.log("+++++", time)
        autoUploadCountTimeSelecte.curValue = time
    }
}
