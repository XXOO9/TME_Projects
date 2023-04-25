import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ".."


/********************双矩形测试结果显示框，适用于立定跳远，坐位体前屈，引体向上，仰卧起坐            */

Item {
    id: root

    state: "normal"
    //必初始化项...
    property string strRetMeasureType: "int"
    property string strDevType: "unKnown"


    //非必初始化项
    property string strColorWhite: "#FEFEFE"
    property string strColorGray: "#AAC5F4"
    property string strColorYellow: "#FFF100"

    property string pointColor: "#FFF100"

    //测试状态
    property string testState: GlobalProperty.strPrepareState

    property string strDefaultRet: "--"

    property int testItem: 0

    property int countTime: GlobalProperty.devType === "YWQZ" ? 60 : 10

    signal signalTouristSingleTestFinished()
    signal sigTimeout()

    width: 1583 * factor
    height: 864 * factor

    Component.onCompleted: {
        if( GlobalProperty.testUnit === "次" ){
            //GlobalProperty.testUnit = "个"
        }

        if( GlobalProperty.devType === "ROPEGPIP" ){
            GlobalProperty.testUnit = "kg"
        }

        if( GlobalProperty.devType === "PUSHUP" ){
            GlobalProperty.testUnit = "次"
        }

        if( GlobalProperty.checkDisplayCountTime() ){
//            countTimeLoader.sourceComponent = comCountTimeText
        }

        circles.itemAt(0).width = 46 * factor
        circles.itemAt(0).border.width = 5 * factor
    }

    function getTextPropety(text)
    {
        var strFoulKeyWords = "FOUL:"
        var strPrepareKeyWords = "PREPARE:"
        var strInvalidResultKeyWords = "INVALIDRESULT:"

        //犯规情况
        if(0 === text.indexOf(strFoulKeyWords))
        {
            strStateText.text = text.substring(strFoulKeyWords.length, text.length);
            strStateText.color = "red";
        }

        //设备测试开始异常情况
        else if(0 === text.indexOf(strPrepareKeyWords))
        {
            strStateText.text = text.substring(strPrepareKeyWords.length, text.length);
            strStateText.color = "red";
        }

        //无效测试结果异常情况
        else if(0 === text.indexOf(strInvalidResultKeyWords))
        {
            strStateText.text = text.substring(strInvalidResultKeyWords.length, text.length);
            strStateText.color = "red";
        }

        //正常测试过程中
        else
        {
            strStateText.color = "white";
        }
    }

    //测试状态( 矩形框 )
    Rectangle{
        id : rectTestState
        width: 594 * factor
        height: 824 * factor
        radius: 30 * factor
        color: "#568BEA"
        anchors.left: root.left
        anchors.top: root.top
        Image {
            id: imgSound
            width: 104 * factor
            height: 87 * factor
            source: "qrc:/pic/15-sound.png"
            anchors.top: parent.top
            anchors.topMargin: 56 * factor
            anchors.left: parent.left
            anchors.leftMargin: 56 * factor
        }
        Text {
            id: strStateText
            width: parent.width
            text: InterAction.testState
            font.bold: true
            font.pixelSize: 100 * factor
            font.family: "PingFang SC"
            color: strColorWhite
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            onTextChanged: {
                getTextPropety(InterAction.testState);
            }
        }
        //横向排列的三个圆圈
        Row{
            spacing: 40 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 90 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            Repeater{
                id : circles
                model: 3
                delegate: Rectangle{
                    id : rectCircle
                    anchors.verticalCenter: parent.verticalCenter
                    width: 40 * factor
                    height: width
                    radius: width / 2
                    color: "#F39800"
                    border.width: 8 * factor
                    border.color: "white"
                }
            }
        }

        //倒计时显示框(引体向上 仰卧起坐, 俯卧撑)
        Loader{
            id: countTimeLoader
            anchors.top: parent.top
            anchors.topMargin: 15 * factor
            anchors.right: parent.right
            anchors.rightMargin: 15 * factor
            //sourceComponent : GlobalProperty.checkDisplayCountTime() ? comCountTimeText : ""
        }
    }

    //测试结果显示框
    Rectangle{
        id : rectTestRet
        visible: GlobalProperty.checkVisible( strDevType )
        width: 960 * factor
        height: 824 * factor
        radius: 30 * factor
        color: rectTestState.color
        anchors.top: root.top
        anchors.right: root.right
        //文字 -> "当前测试结果"
        Text {
            text: qsTr( "当前测试结果" )
            font.pixelSize: 62 * factor
            font.family: "Microsoft YaHei"
            font.bold: true
            color: strColorYellow
            anchors.top: parent.top
            anchors.topMargin: 86 * factor
            anchors.horizontalCenter: parent.horizontalCenter
        }
        //文字  ( 测试结果的值 )
        Text {
            id: strRet
            width: parent.width
            fontSizeMode: Text.Fit
            // text: GlobalProperty.strCurRet === "0" ? strDefaultRet : GlobalProperty.strCurRet
            text: GlobalProperty.strCurRet
            font.pixelSize: 282 * factor
            font.family: "Rosenrot"
            font.bold: true
            color: strColorYellow
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        //视力测试时显示的数据
        Rectangle{
            id: visionRect
            visible: false
            width: parent.width * 0.8
            height: parent.height * 0.6
            color: parent.color
            anchors.centerIn: parent

            //中间的分割线
            Rectangle{
                id: splitLine
                width: 2 * factor
                height: parent.height * 0.6
                color: strColorYellow
                anchors.centerIn: parent
            }


            //左眼视力值
            Text {
                id: leftEyeVision
                text: "--"
                color: strColorYellow
                font{ family: "Microsoft YaHei"; bold: true; pixelSize: 160 * factor }
                anchors.verticalCenter: visionRect.verticalCenter
                anchors.right: splitLine.left
                anchors.rightMargin: 50 * factor
            }

            //文字  "左眼"
            Text {
                text: "左眼"
                color: strColorYellow
                font{ family: "Microsoft YaHei"; bold: true; pixelSize: 60 * factor }
                anchors.horizontalCenter: leftEyeVision.horizontalCenter
                anchors.top: leftEyeVision.bottom
                anchors.topMargin: 50 * factor
            }

            //右眼视力值
            Text {
                id: rightEyeVision
                text: "--"
                color: strColorYellow
                font{ family: "Microsoft YaHei"; bold: true; pixelSize: 160 * factor }
                anchors.verticalCenter: visionRect.verticalCenter
                anchors.left: splitLine.right
                anchors.leftMargin: 50 * factor
            }

            //文字  "右眼"
            Text {
                text: "右眼"
                color: strColorYellow
                font{ family: "Microsoft YaHei"; bold: true; pixelSize: 60 * factor }
                anchors.horizontalCenter: rightEyeVision.horizontalCenter
                anchors.top: rightEyeVision.bottom
                anchors.topMargin: 50 * factor
            }
        }

        //文字  单位 -> "m"
        Text {
            id: strTestRetMeasure
            text:  GlobalProperty.checkTY() ? "cm" : GlobalProperty.testUnit
            font.pixelSize: 62 * factor
            font.family: "Microsoft YaHei"
            color: strColorYellow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 90 * factor
        }

//        MouseArea{
//            anchors.fill: parent
//            onClicked: {
//                debugTimer.start()
//            }

//            Timer{
//                id: debugTimer
//                interval: 1000
//                repeat: true
//                onTriggered: {
//                    InterAction.setCurTestRet( "-19.5", true )
//                    console.log( "send 0 " )
//                }
//            }
//        }
    }

    Connections{
        target: InterAction

        //实时刷新当前这次的测试结果, 并且关闭超时定时器
        onSignalCurrentTestRetAvailable:{

            if(ret !== "")
            {
                strRet.text = InterAction.getCurTestRet();
            }
        }

        onSendTestStateChangeSignal:{
            strStateText.text = InterAction.testState
            if(InterAction.testState === "请准备")
            {
                circles.itemAt(0).width = 46 * factor
                circles.itemAt(0).border.width = 5 * factor
                circles.itemAt(1).width = 40 * factor
                circles.itemAt(1).border.width = 8 * factor
                circles.itemAt(2).width = 40 * factor
                circles.itemAt(2).border.width = 8 * factor
                stopCountTimer()
            }
            else if(InterAction.testState === "测试中")
            {
                //                if( GlobalProperty.checkDisplayCountTime() ){
                //                    startCountTime();
                //                }
                circles.itemAt(1).width = 45 * factor
                circles.itemAt(1).border.width = 5 * factor
                circles.itemAt(0).width = 40 * factor
                circles.itemAt(0).border.width = 8 * factor
                circles.itemAt(2).width = 40 * factor
                circles.itemAt(2).border.width = 8 * factor
            }
            else if(InterAction.testState === "测试完成")
            {
                circles.itemAt(2).width = 45 * factor
                circles.itemAt(2).border.width = 5 * factor
                circles.itemAt(0).width = 40 * factor
                circles.itemAt(0).border.width = 8 * factor
                circles.itemAt(1).width = 40 * factor
                circles.itemAt(1).border.width = 8 * factor
                signalTouristSingleTestFinished()
            }
        }
    }

    Component{
        id: comCountTimeText
        Text {
            id: timeCount
            text: countTime
            color: "white"
            font{ pixelSize: 100 * factor; family: "Digital-7 Mono" }
            Timer{
                id: countTimer
                interval: 1000
                repeat: true
                onTriggered: {
                    if( countTime > 0 ){
                        countTime--
                    }
                    else
                    {
                        countTimer.stop()
                        InterAction.testState === "测试完成"
                        //开启界面超时定时器
                        sigTimeout()
                    }
                }
            }

            //仰卧起坐专用
            function startTimersitUp(){
                if( countTimer.running ){
                    return
                }
                countTimer.start()
            }

            //引体向上专用
            function startTimerPullUp(){
//                countTimer.stop()
//                countTime = 10
//                countTimer.start()
            }

            function stopCountTimer(){
                if( countTimer.running ){
                    countTimer.stop()
                }
            }
        }
    }

    function startCountTime1(){
        countTimeLoader.item.startTimersitUp()
    }

    function startCountTime2(){
//        countTimeLoader.item.startTimerPullUp()
    }

    function stopCountTimer(){
        countTimeLoader.item.stopCountTimer()
    }


    function setEyeSightRet( bothEyeSight ){
        if( bothEyeSight === "--" ){
            leftEyeVision.text = "--";
            rightEyeVision.text = "--";
            return
        }

        let visionArray = bothEyeSight.split( "#" );
        leftEyeVision.text = visionArray[0];
        rightEyeVision.text = visionArray[1];
    }



    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: strRet
                visible: true
            }

            PropertyChanges {
                target: visionRect
                visible: false
            }
        },
        State{
            name: "dev_vision"
            PropertyChanges {
                target: strRet
                visible: false
            }

            PropertyChanges {
                target: visionRect
                visible: true
            }
        }
    ]
}
