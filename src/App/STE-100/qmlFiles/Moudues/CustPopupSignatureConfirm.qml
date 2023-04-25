import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ".."

Item {
    id : popItem
    width: 1582 * factor
    height: 1182 * factor

    property alias popupVisible: pop.visible

    signal signalCloseBtnClicked()

    signal signalConfirmBtnClicked( bool bIsSigned )

    signal sigUpdateRetReulst(bool bSuccess);

    property string strFailedPath: "qrc:/pic/failed.png"
    property string strSuccessPath: "qrc:/pic/success.png"

    property bool iflag : false

    property int timeOutCount: 3       //25 * 200ms = 5s

    Component.onCompleted: {
        okBtnMouseArea.enabled = true
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
            InterAction.clearSignPainter()
            InterAction.startSignature("0")
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
                    text: qsTr( "签名确认成绩" )
                    font.pixelSize: 78 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 60 * factor
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
                            /*
                            //清除签字板内容
                            custPaint.clean();
                            //关闭当前弹窗
                            hide()
                            */

                            //清除并关闭widget签字板
//                            InterAction.clearSignPainter()
                            InterAction.startSignature("0")

                            hide()

                            okBtnMouseArea.enabled = true
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
                //color: "white"
                color: "transparent"
                radius: 30 * factor
                anchors.top: rectTopText.bottom
                anchors.topMargin: 84 * factor
                anchors.horizontalCenter: parent.horizontalCenter
//                // 签字板
//                CustPaint{
//                    id: custPaint
//                    anchors.fill: parent
//                }
            }

            Popup{
                id: upLoadscoreRetPop
                width: 500 * factor
                height: 340 * factor
                padding: 0 * factor
                anchors.centerIn: parent
                modal: true
                closePolicy: Popup.NoAutoClose
                contentItem: Rectangle{
                    id:rectRet
                    width: 500 * factor
                    height: 340 * factor
                    radius: 30 * factor
                    color: "#3241CC"
                    anchors.centerIn: parent
//                    visible: false
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

            //            Rectangle{
            //                id:rectRet
            //                width: 500 * factor
            //                height: 340 * factor
            //                radius: 30 * factor
            //                color: "#3241CC"
            //                anchors.centerIn: parent
            //                visible: false
            //                Image {
            //                    id: imgRet
            //                    width: 120 * factor
            //                    height: width
            //                    anchors.horizontalCenter: parent.horizontalCenter
            //                    anchors.top: parent.top
            //                    anchors.topMargin: 60 * factor
            //                }
            //                Text {
            //                    id: upDateRetTips
            //                    width: 267 * factor
            //                    height: 59 * factor
            //                    color: "white "
            //                    font{ pixelSize: 62 * factor; bold: true; family: "Microsoft YaHei" }
            //                    anchors.horizontalCenter: parent.horizontalCenter
            //                    anchors.top: imgRet.bottom
            //                    anchors.topMargin: 41 * factor
            //                }
            //            }

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

                        InterAction.clearSignPainter()
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
                    interval: 500
                    repeat: false
                    onTriggered: {
                        console.log( "coming in delayTimer" )
                        //检查widget签字版是否成功保存签字
                        var bSignExit = InterAction.checkSignFileExit()

                        //清除签字版
                        InterAction.clearSignPainter()
                        InterAction.startSignature("0")
                        //InterAction.checkSignFileExit()
                        if( bSignExit ){
                            //签字版不为空上传为测试成绩
                            console.log("签字版不为空,即将上传考试成绩\n")
                            InterAction.qmlLogout( "\nsignPainter is not empty, going to update socre as exam type\n" )
                            InterAction.upDateCurTestRet( true )
                        }
                        else{
                            //签字版为空上传为练习成绩
                            console.log("签字版为空,即将上传练习成绩\n")
                            InterAction.qmlLogout( "\nsignPainter is empty, going to update socre as exercise type\n" )
                            InterAction.upDateCurTestRet( false )
                        }
                    }
                }

                MouseArea{
                    id: okBtnMouseArea
                    anchors.fill: parent
                    onClicked: {
                        okBtnMouseArea.enabled = false
                        /*
                        imgRet.source = strSuccessPath
                        upDateRetTips.text = "提交成功"
                        if( !custPaint.bIsBlank ){
                            custPaint.save()
                        }
                        rectRet.visible = true
                        delayTimer.start()
                        */

                        //开启超时失败判断
                        updateRetTimer.start();
                        //显示状态提示
                        upLoadscoreRetPop.open()
//                        rectRet.visible = true

                        /*
                        //有签名则保存
                        if( custPaint.getIsBlank() ){
                            custPaint.save()
                        }
                        */
                        //开始保存签名照片
                        InterAction.saveWidgetSignPainter()

                        //上传成绩
                        delayTimer.start()
                    }
                }
            }
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
                imgRet.source = strSuccessPath
                iflag = true;
            }
            else
            {
                console.log("SignalGetUpdateTestRet：提交失败")
                imgRet.source = strFailedPath
                upDateRetTips.text = "提交失败"
                okBtnMouseArea.enabled = true
                iflag = false;
            }

            //关闭窗口
            closePopTimer.start();
        }
    }

    Timer{
        id: updateRetTimer
        interval: 200
        repeat: true
        onTriggered: {
            console.log("提交中",timeOutCount," ",iflag)
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
        interval: 1000
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
            //custPaint.clean();
            //signalCloseBtnClicked()
            //InterAction.cleanDataModel()
            //hideMask()
        }
    }


    function show(){
        popupVisible = true
        okBtnMouseArea.enabled = true
    }

    function hide(){
        popupVisible = false
    }
}
