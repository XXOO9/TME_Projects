/*
     @ FileName:     CustPopupSignatureConfirm.qml
     @ Describe:     带签字版的一对一提交成绩弹窗
     @ Author:       xutianci
     @ Attentions:
     @ Create Date:  2022-08-17  09:08
*/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../JavaScript/ProjectValue.js" as Assistant
import "../"

Item {
    id : popItem
    objectName: "commitPopSign"
    visible: false
    width: 1582 * factor
    height: 1182 * factor

    property alias popupVisible: pop.visible
    property alias displayErrorString: errorString.text

    signal signalCloseBtnClicked()

    signal sigUpdateRetReulst(bool bSuccess);

    property string strFailedPath: "qrc:/pic/failed.png"
    property string strSuccessPath: "qrc:/pic/success.png"

    property bool iflag : false

    property int timeOutCount: 3       //25 * 200ms = 5s

    property int examModelCountTime: 3  //宁夏考试模式自动提交倒计时

    Component.onCompleted: {
        //        okBtnMouseArea.enabled = true
    }

    function getCurDisplayInfo(){
        //        //加载识别文字
        //        if(GlobalProperty.iDevType === GlobalProperty.devBody)
        //        {
        //            uploadRet.font.pixelSize = 120 * factor
        //            uploadRet.text = "身高:"+InterAction.getOtoBodyHeight() + "cm" + " 体重:" + InterAction.getOtoBodyWeight() + "kg"
        //        }
        //        else if(GlobalProperty.iDevType === GlobalProperty.devVision)
        //        {
        //            uploadRet.font.pixelSize = 250 * factor
        //            uploadRet.text = "是否提交成绩?"
        //        }
        //        else
        //        {
        //            uploadRet.font.pixelSize = 150 * factor
        //            uploadRet.text = InterAction.otoMaxRet + GlobalProperty.getUnitByDevType()
        //        }
    }

    Popup{
        id: pop
        //        visible: false
        modal: true
        focus: true

        width: popItem.width
        height: popItem.height
        anchors.centerIn: parent
        closePolicy: Popup.NoAutoClose

        onClosed:{
            //清除并关闭widget签字板
//            InterAction.clearSignPainter()
            //InterAction.startSignature("0")
        }

        onOpened: {
            SignPainterConfig.changeOtoSignPainter()
            okBtnMouseArea.enabled = true
        }


        background: Rectangle{
            id: backGrd
            width: popItem.width
            height: popItem.height
            radius: 30 * factor
            color: "#747DF2"
            //顶部文字 和 按钮
            Rectangle{
                id: rectTopText
                width: parent.width
                height: 197 * factor
                radius: 30 * factor
                color: "#4D3DDD"
                anchors.top: backGrd.top
                anchors.horizontalCenter:backGrd.horizontalCenter
                Text {
                    //宁夏版本不需要签字
                    id: tipsText
                    text: qsTr( "签名确认成绩" )
                    font.pixelSize: 78 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 60 * factor
                }

                Button{
                    id: debugBtn
                    visible:  InterAction.isDeveloper
                    width: 250 * factor
                    height: parent.height
                    onClicked: openSignPainterTool()
                }

                //关闭按钮
                Image {
                    width: 62 * factor
                    height: 62 * factor
                    source: "qrc:/pic/22-CloseBtn.png"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 60 * factor
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            hide()
                            signalCloseBtnClicked()
                        }
                    }
                }
                //底部填充矩形
                Rectangle{
                    width: parent.width
                    height: 20 * factor
                    color: parent.color
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
            //签字区域
            Rectangle{
                id:rectSignature
                width: 1470 * factor
                height: 620 * factor
                color: "white"
                //                            color: "transparent"
                radius: 30 * factor
                anchors.top: rectTopText.bottom
                anchors.topMargin: 84 * factor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            //提交结果提示窗
            Popup{
                id: upLoadscoreRetPop
                width: 600 * factor
                height: 450 * factor
                padding: 0 * factor
                anchors.centerIn: parent
                modal: true
                closePolicy: Popup.NoAutoClose
                contentItem: Rectangle{
                    id:rectRet
                    width: 700 * factor
                    height: 540 * factor
                    radius: 30 * factor
                    color: "#3241CC"
                    anchors.centerIn: parent
                    //visible: false
                    Image {
                        id: imgRet
                        width: 120 * factor
                        height: width
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 60 * factor
                    }
                    Text {
                        id: upDateRetTips
                        width: 267 * factor
                        height: 59 * factor
                        color: "white "
                        font{ pixelSize: 62 * factor; bold: true; family: "Microsoft YaHei" }
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: imgRet.bottom
                        anchors.topMargin: 41 * factor
                    }

                    Text {
                        id: errorString
                        visible: false
                        text: InterAction.errorString
                        width: parent.width
                        wrapMode: Text.Wrap
                        color: "white"
                        font{ pixelSize: 50 * factor; bold: true; family: "Microsoft YaHei" }
                        anchors.top: upDateRetTips.bottom
                        anchors.topMargin: 40 * factor
                        anchors.horizontalCenter: upDateRetTips.horizontalCenter
                    }
                }
                background: Rectangle{
                    anchors.fill: parent
                    color: "transparent"
                }
                onClosed: {
                    updateRetTimer.stop()
                    console.log( "uoload score ret popup is closed..." )
                }
            }

            //清除按钮
            Rectangle{
                id:rectCleanBtn
                width: 422 * factor
                height: 141 * factor
                radius: 70 * factor
                color: "#2B41D0"
                anchors.top: rectSignature.bottom
                anchors.topMargin: 57 * factor
                anchors.left: parent.left
                anchors.leftMargin: 339 * factor
                Text {
                    text: qsTr( "清除签字" )

                    font.pixelSize: 62 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        //点击清除按钮
                        //custPaint.clean()
                        //InterAction.clearSignPainter()
                        InterAction.procBtnClicked(Enums.OTO_CLEAR_SIGN_BTN)

                    }
                }
            }
            //确认按钮
            Rectangle{
                id:rectConfirmBtn
                width: 422 * factor
                height: 141 * factor
                radius: 70 * factor
                color: "#2B41D0"
                anchors.top: rectSignature.bottom
                anchors.topMargin: 57 * factor
                anchors.right: parent.right
                anchors.rightMargin: 339 * factor
                Text {
                    text: qsTr( "确认" )
                    font.pixelSize: 62 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.centerIn: parent
                }

                Timer{
                    id: delayTimer
                    interval: 2000
                    repeat: false
                    onTriggered: {
                        closeRootPopup()
                        InterAction.qmlLogout("点击签字确认按钮")
                        //交互类通知后台开始提交成绩(通用)
//                        InterAction.startUploadCommonScore()
                        InterAction.procBtnClicked( Enums.OTO_VERIFY_SIGN_BTN )     //onClickBtnVerifySubmitResult

                        ////开启超时失败判断
                        updateRetTimer.start()
                        //显示状态提示
                        upLoadscoreRetPop.open()
                    }
                }

                MouseArea{
                    id: okBtnMouseArea
                    anchors.fill: parent
                    onClicked: {
                        //点了之后，立马变为不可点击状态, 直到下次弹出来的时候才可以点
                        enabled = false

                        //开启保存等待弹窗
                        openRootPopup( "loadingPopup" )

                        //保存签名
                        InterAction.saveWidgetSignPainter()

                        InterAction.startSignature( "0" )
                        InterAction.clearSignPainter()
                        delayTimer.start()
                    }
                }
            }
        }
    }


    //用于动态显示    提交中....
    Timer{
        id: updateRetTimer
        interval: 200
        repeat: true
        onTriggered: {
            console.log("提交中",timeOutCount," ",iflag)
            imgRet.source = ""
            imgRet.source = strSuccessPath;
            if(timeOutCount > 0)
            {
                timeOutCount--;
                var remiander = timeOutCount % 3;
                if(remiander===0)
                {
                    upDateRetTips.text = "提交中."
                }
                else if(remiander===1)
                {
                    upDateRetTips.text = "提交中.."
                }
                else
                {
                    upDateRetTips.text = "提交中..."
                }
            }
            else
            {
                timeOutCount = 3;
            }
        }
    }

    //延迟关闭窗口
    Timer{
        id: closePopTimer
        interval: 2500
        repeat: false
        onTriggered:
        {
            timeOutCount = 3;
            upDateRetTips.text = "";
            upLoadscoreRetPop.close()
            //            rectRet.visible = false;
            imgRet.source = "";
            sigUpdateRetReulst(iflag);
            hide()

            okBtnMouseArea.enabled = true
            //custPaint.clean();
            //signalCloseBtnClicked()
            //InterAction.cleanDataModel()
            //hideMask()
        }
    }

    Connections{
        target: InterAction

        //接收成绩提交结果
        onSignalGetUpdateTestRet:{
            updateRetTimer.stop();

            if(bSuccess)
            {
                console.log("SignalGetUpdateTestRet：提交成功")
                upDateRetTips.text = "提交成功"
                imgRet.source = ""
                imgRet.source = strSuccessPath
                iflag = true;
            }else
            {
                console.log( "js ret = " + InterAction.errorString )
                imgRet.source = ""
                imgRet.source = strFailedPath
                upDateRetTips.text = "提交失败"
                if( GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_Signature ){
                    InterAction.startSignature( "1" )
                }
                iflag = false
            }

            //关闭窗口
            closePopTimer.start();
        }
    }


    //给后台调用的接口, 设置显示的错误信息
    function setUploadErrorString( errString ){
        displayErrorString = errString
    }


    function show(){
        getCurDisplayInfo()
        popupVisible = true
        InterAction.startSignature( "1" )
    }

    function hide(){
        InterAction.startSignature( "0" )
        popupVisible = false
    }
}
