import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ".."

//身高体重测试框
Item {
    id: root
    //非必初始化项
    property string strColorWhite: "#FEFEFE"
    property string strColorGray: "#AAC5F4"
    property string strColorYellow: "#FFF100"

    //测试状态
    property string testState: GlobalProperty.strPrepareState
    property int testItem: 0

    //测试结果
    property string weightVal: "0"
    property string heightVal: "0"

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

        MouseArea{
            anchors.fill: parent
            onClicked: {

            }
        }
    }

    //测试结果显示框
    Rectangle{
        id : rectTestRet
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
            anchors.topMargin: 56 * factor
            anchors.left: parent.left
            anchors.leftMargin: 50 * factor
        }
        //文字  ( 测试结果的值 )
        Text {
            id: strHeight
            text: weightVal
            font.pixelSize: 180 * factor
            font.family: "Rosenrot"
            color: strColorYellow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: partLine.top
            anchors.bottomMargin: 60 * factor
        }

        Text {
            id: strHeightUnit
            text: "kg"
            font.pixelSize: 100 * factor
            font.family: "Microsoft YaHei"
            color: strColorYellow
            anchors.bottom: strHeight.bottom
            anchors.left: strHeight.right
            anchors.leftMargin: 20 * factor
        }

        Rectangle {
            id: partLine
            width: parent.width - 50 * factor
            height: 5 * factor
            color: "#D7D5F0"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 60 * factor
        }

        //文字  单位 -> "m"
        Text {
            id: strWeight
            text: heightVal
            font.pixelSize: 180 * factor
            font.family: "Rosenrot"
            color: strColorYellow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: partLine.top
            anchors.topMargin: 80 * factor
        }

        Text {
            id: strWeightUnit
            text: "cm"
            font.pixelSize: 100 * factor
            font.family: "Microsoft YaHei"
            color: strColorYellow
            anchors.bottom: strWeight.bottom
            anchors.left: strWeight.right
            anchors.leftMargin: 20 * factor
        }
    }

    Connections{
        target: InterAction

        //实时刷新当前这次的测试结果, 并且关闭超时定时器
        onSignalCurrentTestRetAvailable:{
            if(ret === "")
            {
                strRet.text = "--";
            }
            else
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
            }
            else if(InterAction.testState === "测试中")
            {
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
            }
        }
    }
}
