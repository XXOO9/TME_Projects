import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../"

Item {
    id: root
    width: 1580 * factor
    height: 1220 * factor

    Rectangle{
        anchors.fill: parent
        color: "#6A76F5"

        Rectangle{
            id: topTitleRect
            width: parent.width
            height: 200 * factor
            color: "#5049E5"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                text:  "系统设置"
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 60 * factor
            }

            Image {
                id: closeImg
                width: 100 * factor
                height: width
                source: "qrc:/pic/22-CloseBtn.png"
                anchors.right: parent.right
                anchors.rightMargin: 50 * factor
                anchors.verticalCenter: parent.verticalCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        closeSystem()
                    }
                }
            }
        }

        Text {
            id: testModelText
            text: "测试模式"
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
            anchors.top: topTitleRect.bottom
            anchors.topMargin: 140 * factor
            anchors.left: parent.left
            anchors.leftMargin: 195 * factor
        }

        CustComboBox{
            id: modelSelectComboBox
            width: 860 * factor
            height: 140 * factor
            comboBoxDisplayList: [ "异步模式", "同步模式" ]
            anchors.verticalCenter: testModelText.verticalCenter
            anchors.left: testModelText.right
            anchors.leftMargin: 27 * factor
            curComboBoxIndex: GlobalProperty.testType == GlobalProperty.testAsyncMode ? 0 : 1
        }

        Text {
            id: textIsDisplayScoreText
            text: qsTr("是否显示成绩")
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
            anchors.top: testModelText.bottom
            anchors.topMargin: 140 * factor
            anchors.right: testModelText.right
        }

        CustComboBox{
            id: isDisplayScoreComboBox
            width: 860 * factor
            height: 140 * factor
            comboBoxDisplayList: [ "是", "否" ]
            anchors.verticalCenter: textIsDisplayScoreText.verticalCenter
            anchors.left: modelSelectComboBox.left
            curComboBoxIndex: GlobalProperty.isDisplayScore ? 0:1
        }

        Text {
            id: runShortText
            text: qsTr("测试项目")
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
            anchors.top: textIsDisplayScoreText.bottom
            anchors.topMargin: 140 * factor
            anchors.right: textIsDisplayScoreText.right
            visible: GlobalProperty.iDevType == GlobalProperty.devRunShort || GlobalProperty.iDevType == GlobalProperty.devRunMid && GlobalProperty.iTestMode == Enums.TEST_MODE_COMPET? true:false
        }

        CustComboBox{
            id: runShortComboBox
            width: 860 * factor
            height: 140 * factor
            comboBoxDisplayList: GlobalProperty.iTestMode === Enums.TEST_MODE_NORMAL ?
                                 [ "50米短跑测试", "往返跑测试"] : [ "60米跑步测试", "100米跑步测试", "200米跑步测试", "400米跑步测试", "4x100米跑步测试", "100米跨栏测试", "110米跨栏测试"]
            anchors.verticalCenter: runShortText.verticalCenter
            anchors.left: modelSelectComboBox.left
            visible: runShortText.visible
            curComboBoxIndex: getComboxIndex(GlobalProperty.curDevTestItem)
        }

        CustComboBox{
            id: runMidComboBox
            width: 860 * factor
            height: 140 * factor
            comboBoxDisplayList: [ "800米跑步", "1500米跑步"]
            anchors.verticalCenter: runShortText.verticalCenter
            anchors.left: modelSelectComboBox.left
            visible: runShortText.visible && GlobalProperty.iDevType == GlobalProperty.devRunMid
            curComboBoxIndex: getComboxIndex(GlobalProperty.curDevTestItem)
        }

        //跳绳参数设置
        CustInputCmp{
            id: custRopeSkipContentInputCmp
            visible: GlobalProperty.devType === "ROPESKING" ? true : false
            anchors.top: textIsDisplayScoreText.bottom
            anchors.topMargin: 20 * factor
            anchors.left: textIsDisplayScoreText.left
        }


        Text {
            id: runMidText
            text: qsTr("一圈长度(m)")
            color: "white"
            font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
            anchors.top: runShortText.visible ? runShortText.bottom : textIsDisplayScoreText.bottom
            anchors.topMargin: 140 * factor
            anchors.right: textIsDisplayScoreText.right
            visible: GlobalProperty.iDevType == GlobalProperty.devRunMid ? true:false
        }

        TextField{
            id: runMidInputBox
            width: 860 * factor
            height: 140 * factor
            color: "white"
            placeholderText: "配置中长跑测试长度"
            placeholderTextColor: "gray"
            font{ family: "Microsoft YaHei";pixelSize: 60 * factor }
            anchors.verticalCenter: runMidText.verticalCenter
            anchors.left: modelSelectComboBox.left
            validator: RegExpValidator{regExp: /^[0-9/]+$/}
            maximumLength: 4
            text: GlobalProperty.devRunMideLength
            visible: runMidText.visible
            background: Rectangle{
                id: rect
                width: 860 * factor
                height: 140 * factor
                radius: 30 * factor
                color: "#0C3DA4"
                border.color: "#1ADDFC"
            }
        }

        Button{
            id: sureBtn
            width: 200 * factor
            height: 140 * factor
            anchors.verticalCenter: runMidInputBox.verticalCenter
            anchors.left: runMidInputBox.right
            anchors.leftMargin: 20 * factor
            visible: runMidText.visible && GlobalProperty.devRunMideLength != runMidInputBox.text * 1 ? true:false
            background: Rectangle{
                anchors.fill: parent
                color: "#1F71CF"
                border.color: "#1ADDFC"
                radius: 30 * factor
                Text {
                    text: "确定"
                    color: "white"
                    font{ family: "Microsoft YaHei"; pixelSize: 60 * factor }
                    anchors.centerIn: parent
                }
            }
            onClicked: {
                if(runMidInputBox.text * 1)
                {
                    if(InterAction.changedRunMidLength(runMidInputBox.text * 1))
                    {
                        GlobalProperty.devRunMideLength = runMidInputBox.text * 1;
                    }
                }
            }
        }

        Connections{
            target: modelSelectComboBox
            onSignalValueRefresh:{
                //  这里响应切换模式， 1代表异步模式   0代表同步模式
                if( 0 === index ){
                    GlobalProperty.testType = GlobalProperty.testAsyncMode
                    InterAction.changedTestModel( 1 )
                }

                if( 1 === index ){
                    GlobalProperty.testType = GlobalProperty.testSyncMode
                    InterAction.changedTestModel( 0 )
                }

                turnToGroupHomePage()
                closeSystem()
            }
        }

        Connections{
            target: isDisplayScoreComboBox
            onSignalValueRefresh:{
                if( 0 === index ){
                    GlobalProperty.isDisplayScore = true
                }else if( 1 === index ){
                    GlobalProperty.isDisplayScore = false
                }
                InterAction.setIsDisplayScore(GlobalProperty.isDisplayScore)
            }
        }

        Connections{
            target: runShortComboBox
            onSignalValueRefresh:{
                var curTestTimes = InterAction.getDevTestTimes(GlobalProperty.iDevType)
                var curWaitTime = InterAction.getDevWaitStartTime(GlobalProperty.iDevType)
                var curTestItems = getComboxItem(index)
                InterAction.setDevTestConfig(GlobalProperty.iDevType, curTestTimes, curTestItems)
                GlobalProperty.updateDevTestItem()
            }
        }

        Connections {
            target: runMidComboBox
            onSignalValueRefresh: {
                var curTestTimes = InterAction.getDevTestTimes(GlobalProperty.iDevType)
                var curWaitTime = InterAction.getDevWaitStartTime(GlobalProperty.iDevType)
                var curTestItems = index== 0? Enums.ITEM_RUN_800 : Enums.ITEM_RUN_1500
                InterAction.setDevTestConfig(GlobalProperty.iDevType, curTestTimes, curTestItems)
                GlobalProperty.updateDevTestItem()
            }
        }

        Connections{
            target: custRopeSkipContentInputCmp
            onSigOnOkBtnClicked:{
                var jsObj = { "devType": 163, "ropeSkipDelay":  custRopeSkipContentInputCmp.value }
                InterAction.changeDevConfigInfo( JSON.stringify( jsObj ) )
            }
        }
    }

    function getComboxIndex(testItem)
    {
        switch(testItem){
        case Enums.TestRunShort:
            return 0
        case Enums.TestRunBack:
            return 1
        case Enums.TestRunShort60:
            return 0
        case Enums.TestRunShort100:
            return 1
        case Enums.TestRunShort200:
            return 2
        case Enums.TestRunShort400:
            return 3
        case Enums.TestRunShort4_100:
            return 4
        case Enums.TestRunHurdles100:
            return 5
        case Enums.TestRunHurdles110:
            return 6
        default:
            return 0
        }
    }

    function getComboxItem(testIndex)
    {
        if(GlobalProperty.iTestMode == Enums.TEST_MODE_NORMAL)
        {
            return testIndex===0 ? Enums.TestRunShort : Enums.TestRunBack
        }
        else
        {
            switch(testIndex){
            case 0:
                return Enums.TestRunShort60
            case 1:
                return Enums.TestRunShort100
            case 2:
                return Enums.TestRunShort200
            case 3:
                return Enums.TestRunShort400
            case 4:
                return Enums.TestRunShort4_100
            case 5:
                return Enums.TestRunHurdles100
            case 6:
                return Enums.TestRunHurdles110
            }
        }
    }
}
