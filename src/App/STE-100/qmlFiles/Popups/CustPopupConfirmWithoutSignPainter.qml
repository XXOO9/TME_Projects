import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../"

Item {
    id : popItem
    visible: false
    width: 1582 * factor
    height: 1182 * factor

    property alias popupVisible: pop.visible

    signal signalCloseBtnClicked()

    signal sigUpdateRetReulst(bool bSuccess);

    property string strFailedPath: "qrc:/pic/failed.png"
    property string strSuccessPath: "qrc:/pic/success.png"

    property bool iflag : false

    property int timeOutCount: 3       //25 * 200ms = 5s

    property int examModelCountTime: 3  //宁夏考试模式自动提交倒计时

    Component.onCompleted: {
//        okBtnMouseArea.enabled = true
        examModelCountTime = GlobalProperty.autoCommitCountTime
    }

    Component.onDestruction: {
        upDateRetTips.text = ""
    }

    function getCurDisplayInfo(){
        //加载识别文字
        if(GlobalProperty.iDevType === GlobalProperty.devBody)
        {
            uploadRet.font.pixelSize = 120 * factor
            uploadRet.text = "身高:"+InterAction.getOtoBodyHeight() + "cm" + " 体重:" + InterAction.getOtoBodyWeight() + "kg"
        }
        else if(GlobalProperty.iDevType === GlobalProperty.devVision)
        {
            uploadRet.font.pixelSize = 250 * factor
            uploadRet.text = "是否提交成绩?"
        }
        else
        {
            uploadRet.font.pixelSize = 150 * factor
            uploadRet.text = InterAction.otoMaxRet + GlobalProperty.getUnitByDevType()
        }
    }

    Popup{
        id: pop
        modal: true
        focus: true

        width: popItem.width
        height: popItem.height
        anchors.centerIn: parent
        closePolicy: Popup.NoAutoClose

        onClosed:{
            hide()
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
                    id: tipsText
                    text: qsTr( "注意:" )
                    font.pixelSize: 78 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 60 * factor
                }

                Text {
                    id: examModelCountTimeText
                    text: "即将在" + examModelCountTime + "秒后自动提交"
                    font{ family: "Microsoft YaHei"; pixelSize: 60 * factor }
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: tipsText.right
                    anchors.leftMargin: 100 * factor
                    Timer{
                        id: examModelCountTimer
                        repeat: true
                        interval: 1000
                        onTriggered: {
                            //如果没到3秒就自减一， 5秒到了就自动提交成绩
                            if( examModelCountTime > 0 ){
                                examModelCountTime--
                            }else{
                                examModelCountTimer.stop()
                                ////开启超时失败判断
                                updateRetTimer.start()
                                //显示状态提示
                                upLoadscoreRetPop.open()

                                InterAction.procBtnClicked( Enums.OTO_VERIFY_SIGN_BTN )
                            }
                        }
                    }
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
                            InterAction.startSignature("0")

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
                //color: "white"
                color: "transparent"
                radius: 30 * factor
                anchors.top: rectTopText.bottom
                anchors.topMargin: 84 * factor
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    text: "即将提交的测试结果为:"
                    visible: (GlobalProperty.iDevType != GlobalProperty.devBody && GlobalProperty.iDevType != GlobalProperty.devVision) ? true : false
                    color: "white"
                    font.pixelSize: 84 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    anchors.top: parent.top
                    anchors.topMargin: 20 * factor
                    anchors.left: parent.left
                    anchors.leftMargin: 20 * factor
                }
                Text {
                    id: uploadRet
                    color: "white"
                    text: "是否提交成绩?"
                    font.pixelSize: 150 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    anchors.centerIn: parent
                }
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
                    //Text {
                    //    id: errorString
                    //    text: qsTr("text")
                    //}
                }
                background: Rectangle{
                    anchors.fill: parent
                    color: "transparent"
                }
                onClosed: {
                    hide()
                }
            }
        }

        //宁夏版本同步模式下,打开弹窗后就自动开始计时，五秒后自动提交成绩
        onOpened: {
            examModelCountTime = GlobalProperty.autoCommitCountTime
            //只有同步模式下才用自动提交
            if( GlobalProperty.testType === GlobalProperty.testSyncMode || GlobalProperty.currentCommitPopup_type === GlobalProperty.commitPopup_value ){
                examModelCountTimer.start()
            }
        }
    }

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
            imgRet.source = "";
            InterAction.qmlLogout( "+++++++++before emit sig, the flag = " + iflag )
            sigUpdateRetReulst(iflag)
            hide()
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
                console.log("SignalGetUpdateTestRet：提交失败")
                imgRet.source = ""
                imgRet.source = strFailedPath
                upDateRetTips.text = "提交失败"
                iflag = false
            }

            //关闭窗口
            closePopTimer.start();
        }
    }


    function show(){
        getCurDisplayInfo()
        popupVisible = true
//        okBtnMouseArea.enabled = true
    }


    //关闭弹窗的时候， 关闭自动提交定时器,并清除签字版
    function hide(){
        popupVisible = false
        //清除并关闭widget签字板
        InterAction.clearSignPainter()
        InterAction.startSignature("0")
        updateRetTimer.stop()
        examModelCountTimer.stop()

        console.log( "uoload score ret popup is closed..." )
    }
}
