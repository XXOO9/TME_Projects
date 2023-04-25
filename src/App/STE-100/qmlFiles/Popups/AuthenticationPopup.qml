//身份认证弹窗
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "./"
import "../Moudues"
import "../"

Item {
    id: root
    width: 1760 * factor
    height: 2530 * factor

    Popup{
        id: m_popup
        width: parent.width
        height: parent.height
        modal: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: parent

        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
        }

        OtmKeyBord{
            id: keyBordView
            titleText: "身份验证"
            tipText:   "请输入管理员密码"
            isPasswdInput: true
            anchors.centerIn: parent
        }
    }


    DevciceNumSettingPopup{
        id: devicePopup
        anchors.fill: parent
    }

    ChannelConfigPopup {
        id: channelPopup
        anchors.fill: parent
    }

    CommitPopupSetting{
        id: commitPopup
        anchors.centerIn: parent
    }

    SystemSetMenuPopup{
        id: systemSetPopup
        anchors.centerIn: parent
    }

    TestInfoConfigPopup {
        id: testInfoPopup
        anchors.centerIn: parent
    }

    CusCorrectDevPop{
        id: correctDevPop
        anchors.centerIn: parent
    }

    OtherSettingsConfigPop{
        id: otherSettingsPop
        anchors.centerIn: parent
        onClosed: {
            otherSettingsPop.clearSettingMenu()
//            root.close()
        }
    }



    Connections{
        target: keyBordView
        onSigConfirmBtnClicked: {
            onKeyBordInputFinished( content )
        }
    }



    Connections{
        target: systemSetPopup

        //响应打开设置分级编号
        onSigOpenDevNumberPopup:{
            //systemSetPopup.close()
            devicePopup.open()
        }

        //响应打开设置信道
        onSigOpenChannelConfigPopup:{
            //systemSetPopup.close()
            channelPopup.open()
        }

        //响应设置提交弹窗的方式
        onSigOpenCommitConfigPopup:{
            //systemSetPopup.close()
            commitPopup.open()
        }

        //响应设置测试配置
        onSigOpenTestInfoConfigPopup: {
            //systemSetPopup.close()
            testInfoPopup.open()
        }

        //打开设备校准弹窗
        onSigOpenCorrectDevPop: {
            onOpenCorrectDevPop()
        }

        //打开其他设置弹窗
        onSigOpenOtherSettingsPop:{
            otherSettingsPop.open()
        }
    }

    function open(){
        m_popup.open()
        InterAction.curPage = Enums.PAGE_SETTING
    }

    function close(){
        keyBordView.clear()
        m_popup.close()
        InterAction.curPage = GlobalProperty.pageBeforoPopups
    }

    function onKeyBordInputFinished( content ){
        if( content === "admin123" || InterAction.isDeveloper ){
            close()
            turnSystemSetting()
//            systemSetPopup.open()
        }
    }

    function onOpenCorrectDevPop( ){
        correctDevPop.openCorrectDevPop()
    }
}
