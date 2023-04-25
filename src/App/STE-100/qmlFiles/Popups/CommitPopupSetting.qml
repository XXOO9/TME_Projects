import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import ProjectEnums 1.0
import "../"
import "../Moudues"

Item {
    id: root
    width: 1582 * factor
    height: 1400 * factor

    Popup{
        id: m_popup
        width: 1582 * factor
        height: 1400 * factor
        padding: 0
        modal: true

        background: Rectangle{
            anchors.fill: parent
            color: "#6A76F5"
        }

        onOpened: {
            onPopupOpened()
        }

        Rectangle{
            id: tipsRect
            width: parent.width
            height: 198 * factor
            color: "#4D39DB"
            Text {
                text: "提交配置"
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 60 * factor
            }

            Image {
                id: closeImg
                width: 70 * factor
                height: width
                source: "qrc:/pic/22-CloseBtn.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 30 * factor
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        close()
                    }
                }
            }
        }

        Text {
            id: delScoreText
            text: "是否可以取消成绩:"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 60 * factor; bold: true }
            anchors.top: tipsRect.bottom
            anchors.topMargin: 50 * factor
            anchors.left: tipsRect.left
            anchors.leftMargin: 60 * factor
        }

        CustHorizontalRadioButtons{
            id: delScoreModelSelect
            contentArray: [ "是          ", "否" ]
            anchors.top: delScoreText.bottom
            anchors.topMargin: 20 * factor
            anchors.left: commitModelText.right
            anchors.leftMargin: 50 * factor
            curIndex: GlobalProperty.isDelBtnEnable === 1 ? 0 : 1
        }

        Text {
            id: commitModelText
            text: "提交方式:"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 60 * factor; bold: true }
            anchors.top: delScoreModelSelect.bottom
            anchors.topMargin: 100 * factor
            anchors.left: tipsRect.left
            anchors.leftMargin: 60 * factor
        }

        CustHorizontalRadioButtons{
            id: commitModelSelect
            contentArray: [ "手动提交", "自动提交" ]
            anchors.top: commitModelText.bottom
            anchors.topMargin: 20 * factor
            anchors.left: commitModelText.right
            anchors.leftMargin: 50 * factor
        }

        Text {
            id: signatureExistedText
            visible: false
            text: "是否需要签字:"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 60 * factor; bold: true }
            anchors.top: tipsRect.bottom
            anchors.topMargin: 300 * factor
            anchors.left: tipsRect.left
            anchors.leftMargin: 60 * factor
        }

        CustHorizontalRadioButtons{
            id: signatureExistedlSelect
            visible: false
            contentArray: [ "需要签字", "不需要签字" ]
            checkAble: false
            anchors.top: signatureExistedText.bottom
            anchors.topMargin: 20 * factor
            anchors.left: commitModelSelect.left
        }

        Text {
            id: autoUploadCountTimeText
            text: "选择自动提交倒计时:"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 60 * factor; bold: true }
            anchors.top: commitModelSelect.bottom
//            anchors.topMargin: 550 * factor
            anchors.topMargin: 120 * factor
            anchors.left: tipsRect.left
            anchors.leftMargin: 60 * factor
        }

        CustValueRanger{
            id: autoUploadCountTimeSelecte
            anchors.top: autoUploadCountTimeText.bottom
            anchors.topMargin: 20 * factor
            anchors.left: commitModelSelect.left
        }

        OtmOperationBtn{
            id: cancelBtn
            width: 350 * factor
            height: 150 * factor
            displayText: "取消"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 120 * factor
            anchors.left: parent.left
            anchors.leftMargin: 350 * factor
            onSigBtnClicked: {
                close()
            }
        }

        OtmOperationBtn{
            id: confirmBtn
            width: 350 * factor
            height: 150 * factor
            anchors.verticalCenter: cancelBtn.verticalCenter
            anchors.left: cancelBtn.right
            anchors.leftMargin: 50 * factor
            onSigBtnClicked: {
                saveCommitSet()
            }
        }

    }

    Connections{
        target: commitModelSelect
        onSigBtnClicked:{
//            GlobalProperty.currentCommitPopup_type = btnIndex
            //手动提交, 就不需用设置自动提交的时间
            if( btnIndex === 0 ){
                signatureExistedlSelect.setCurrentIndex( 0 )
                hideAutoUploadCmp()
            }else if( btnIndex === 1){  //自动提交
                showAutoUploadCmp()
            }
        }
    }

    Connections{
        target: signatureExistedlSelect
        onSigBtnClicked:{
            //如果当前是自动提交,则强行设置为不签字
            if( commitModelSelect.curIndex === GlobalProperty.commitPopup_value ){
                signatureExistedlSelect.setCurrentIndex( GlobalProperty.commitPopup_value )
                return
            }

            //如果是自动提交模式,则必定为不带签字版
            signatureExistedlSelect.setCurrentIndex( btnIndex )
        }
    }

    function open(){
        m_popup.open()
        delScoreModelSelect.setCurrentIndex( Math.abs(GlobalProperty.isDelBtnEnable - 1))
        commitModelSelect.setCurrentIndex(GlobalProperty.currentCommitPopup_type)
        InterAction.curPage = Enums.PAGE_SETTING
    }

    function close(){
        m_popup.close()
        InterAction.curPage = GlobalProperty.pageBeforoPopups
    }

    function saveCommitSet(){
        var delScoreBtnEnable = delScoreModelSelect.curIndex == 0 ? 1 : 0
        var commitModel = commitModelSelect.curIndex
        var signatureExist = signatureExistedlSelect.curIndex
        var autoUploadCountTime = autoUploadCountTimeSelecte.curValue

        console.log( "commit model = " + commitModel )
        console.log( "signatureExist = " + signatureExist )
        console.log( "autoUploadCountTime = " + autoUploadCountTime )

        //保存配置时,先判断是否需要签字版
        if( signatureExist === 0 ){
            GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_Signature
        }else if( signatureExist === 1 ){
            GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_value
        }

        //再判断是否自动提交

        //如果是自动提交,就不会有签字版
        if( commitModel === 1 ){
            GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_value
            GlobalProperty.autoCommitCountTime = autoUploadCountTime
        }

        InterAction.saveCommitPopupParamToDataBase(delScoreBtnEnable, commitModel, signatureExist, autoUploadCountTime )

        GlobalProperty.isDelBtnEnable = delScoreBtnEnable

        close()
    }

    function hideAutoUploadCmp(){
        autoUploadCountTimeText.visible = false
        autoUploadCountTimeSelecte.visible = false
        signatureExistedlSelect.checkAble = true
        signatureExistedlSelect.setCurrentIndex( 0 )
    }

    function  showAutoUploadCmp(){
        autoUploadCountTimeText.visible = true
        autoUploadCountTimeSelecte.visible = true
        signatureExistedlSelect.checkAble = false
        signatureExistedlSelect.setCurrentIndex( 1 )
    }

    function onPopupOpened(){
        //如果是带有签字版的弹窗, 则只能是手动提交
        if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_Signature ){
            commitModelSelect.setCurrentIndex( 0 )
            hideAutoUploadCmp()
        }

        //如果是不带签字版的弹窗, 则只能是自动提交
        if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_value ){
            commitModelSelect.setCurrentIndex( 1 )
            signatureExistedlSelect.setCurrentIndex( 1 )
            showAutoUploadCmp()
        }

        autoUploadCountTimeSelecte.setValue( GlobalProperty.autoCommitCountTime )
    }
}
