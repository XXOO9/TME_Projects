import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../Moudues"
import "../"

Item {
    id: root

    property int curDevType: GlobalProperty.iDevType
    property int curTestTimes: 1
    property int curWaitTime: 0
    property int curTestMode: GlobalProperty.iTestMode
    property string curTestItems: "1-2"
    property var curTestItemList: []
    property bool bStartFlag: true

    function changeConfig()
    {
        tipsText.visible = false
        curTestTimes = InterAction.getDevTestTimes(curDevType)
        curWaitTime = InterAction.getDevWaitStartTime(curDevType)
        curTestItems = InterAction.getDevTestItems(curDevType)
        curTestItemList = curTestItems.split("-")
        bStartFlag = InterAction.getRunShortStartFlag()
        console.log("curTestItemList", curTestItemList, Enums.TestRunShort60.toString())
        getTestItemsCheckStatus()
    }

    function getTestItemsFormCheck()
    {
        tipsText.visible = false
        curTestItems = "";
        if(curDevType === GlobalProperty.devBody)
        {
            if(heightBox.checked)
            {
                curTestItems += Enums.TestHeight.toString()
            }

            if(weightBox.checked)
            {
                curTestItems += "-" + Enums.TestWeight
            }

            if(humanBox.checked)
            {
                curTestItems += "-" + Enums.TestHuman
            }

            if(bmiBox.checked)
            {
                curTestItems += "-" + Enums.TestBMI
            }
        }
        else if(curDevType === GlobalProperty.devRunShort)
        {
            if(curTestMode === Enums.TEST_MODE_NORMAL)
            {
                if(runShortBox.checked)
                {
                    curTestItems = Enums.TestRunShort.toString()
                }
                else
                {
                    curTestItems = Enums.TestRunBack.toString()
                }
            }
            else
            {
                if( runShortCompet60.checked)
                {
                    curTestItems = Enums.TestRunShort60.toString()
                }
                else if( runShortCompet100.checked)
                {
                    curTestItems = Enums.TestRunShort100.toString()
                }
                else if( runShortCompet200.checked)
                {
                    curTestItems = Enums.TestRunShort200.toString()
                }
                else if( runShortCompet400.checked)
                {
                    curTestItems = Enums.TestRunShort400.toString()
                }
                else if( runShortCompet4_100.checked)
                {
                    curTestItems = Enums.TestRunShort4_100.toString()
                }
                else if(runShortCompet100Hurdle.checked)
                {
                    curTestItems = Enums.TestRunHurdles100.toString()
                }
                else
                {
                    curTestItems = Enums.TestRunHurdles110.toString()
                }
            }
        }
        else if(curDevType == GlobalProperty.devPVV)
        {
            if(vcBox.checked)
            {
                curTestItems = Enums.TestPVVVC.toString()
            }

            if(forcedVCBox.checked)
            {
                curTestItems = Enums.TestPVVFVC.toString()
            }

            if(maxPVVBox.checked)
            {
                curTestItems = Enums.TestPVVMPV.toString()
            }
        }
        else if(curDevType === GlobalProperty.devRunMid)
        {
            if(curTestMode === Enums.TEST_MODE_NORMAL)
            {
                curTestItems = Enums.ITEM_RUN_1000.toString() + "-" + Enums.ITEM_RUN_800.toString()
            }
            else
            {
                if(eightHundredBox.checked)
                {
                    curTestItems = Enums.ITEM_RUN_800.toString()
                }
                else
                {
                    curTestItems = Enums.ITEM_RUN_1500.toString()
                }
            }
        }
    }

    function getTestItemsCheckStatus()
    {
        if(curDevType === GlobalProperty.devRunShort)
        {
            if(curTestMode == Enums.TEST_MODE_NORMAL)
            {
                if(curTestItems == Enums.TestRunShort.toString())
                {
                    runShortBox.checked = true
                    runBackBox.checked = false
                }
                else if(curTestItems == Enums.TestRunBack.toString())
                {
                    runBackBox.checked = true
                    runShortBox.checked = false
                }

                runShortCompet60.checked = true
                runShortCompet100.checked = false
                runShortCompet200.checked = false
                runShortCompet400.checked = false
                runShortCompet4_100.checked = false
                runShortCompet100Hurdle.checked = false
                runShortCompet110.checked = false
            }
            else
            {
                if(curTestItems == Enums.TestRunShort60.toString())
                {
                    runShortCompet60.checked = true
                    runShortCompet100.checked = false
                    runShortCompet200.checked = false
                    runShortCompet400.checked = false
                    runShortCompet4_100.checked = false
                    runShortCompet100Hurdle.checked = false
                    runShortCompet110.checked = false
                }
                else if(curTestItems == Enums.TestRunShort100.toString())
                {
                    runShortCompet100.checked = true
                    runShortCompet60.checked = false
                    runShortCompet200.checked = false
                    runShortCompet400.checked = false
                    runShortCompet4_100.checked = false
                    runShortCompet100Hurdle.checked = false
                    runShortCompet110.checked = false
                }
                else if(curTestItems == Enums.TestRunHurdles100.toString())
                {
                    runShortCompet100.checked = false
                    runShortCompet60.checked = false
                    runShortCompet200.checked = false
                    runShortCompet400.checked = false
                    runShortCompet4_100.checked = false
                    runShortCompet100Hurdle.checked = true
                    runShortCompet110.checked = false
                }
                else if(curTestItems == Enums.TestRunHurdles110.toString())
                {
                    runShortCompet100.checked = false
                    runShortCompet60.checked = false
                    runShortCompet200.checked = false
                    runShortCompet400.checked = false
                    runShortCompet4_100.checked = false
                    runShortCompet100Hurdle.checked = false
                    runShortCompet110.checked = true
                }
                else if(curTestItems == Enums.TestRunShort200.toString())
                {
                    runShortCompet100.checked = false
                    runShortCompet60.checked = false
                    runShortCompet200.checked = true
                    runShortCompet400.checked = false
                    runShortCompet4_100.checked = false
                    runShortCompet100Hurdle.checked = false
                    runShortCompet110.checked = false
                }
                else if(curTestItems == Enums.TestRunShort400.toString())
                {
                    runShortCompet100.checked = false
                    runShortCompet60.checked = false
                    runShortCompet200.checked = false
                    runShortCompet400.checked = true
                    runShortCompet4_100.checked = false
                    runShortCompet100Hurdle.checked = false
                    runShortCompet110.checked = false
                }
                else
                {
                    runShortCompet100.checked = false
                    runShortCompet60.checked = false
                    runShortCompet200.checked = false
                    runShortCompet400.checked = false
                    runShortCompet4_100.checked = true
                    runShortCompet100Hurdle.checked = false
                    runShortCompet110.checked = false
                }
                runBackBox.checked = true
                runShortBox.checked = false
            }
        }

        if(curDevType === GlobalProperty.devRunMid)
        {
            if(curTestMode == Enums.TEST_MODE_NORMAL)
            {
                eightHundredBox.checked = true
                thousandBox.checked = true
            }
            else
            {
                if(curTestItems == Enums.ITEM_RUN_800.toString())
                {
                    eightHundredBox.checked = true
                }
                else
                {
                    runMid1500CompetBox.checked = true
                }
            }
        }
    }

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

        Rectangle{
            width: 1580 * factor
            height: 1800 * factor
            anchors.centerIn: parent
            color: "#6A76F5"

            Rectangle{
                id: topTitleRect
                width: parent.width
                height: 200 * factor
                color: "#5049E5"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    text:  "测试配置"
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
                            close()
                        }
                    }
                }
            }

            Column {
                id: devTestCfgColumn
                width: parent.width
                anchors.top: topTitleRect.bottom
                anchors.topMargin: 50 * factor
                spacing: 20 * factor

                Item {
                    id: devTypeItem
                    width: parent.width
                    height: 140 * factor

                    Text {
                        id: testModelText
                        text: "设备类型"
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                        anchors.left: parent.left
                        anchors.leftMargin: 195 * factor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    CustComboBox{
                        id: modelSelectComboBox
                        width: 900 * factor
                        height: 140 * factor
                        comboBoxDisplayList: [ "身高体重", "肺活量", "立定跳远", "坐位体前屈", "引体向上",
                                               "仰卧起坐", "短跑", "中长跑", "跳绳", "握力", "俯卧撑", "视力", "肺通气量"]
                        anchors.verticalCenter: testModelText.verticalCenter
                        anchors.left: testModelText.right
                        anchors.leftMargin: 27 * factor
                        bEnable: false
                        curComboBoxIndex: GlobalProperty.iDevType == GlobalProperty.devUnknow ? GlobalProperty.iDevType  : GlobalProperty.iDevType - 1
                    }
                }

                Item {
                    id: testCountItem
                    width: parent.width
                    height: 140 * factor

                    Text {
                        id: channelChooseText
                        text: qsTr("测试次数")
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 195 * factor
                    }

                    CustComboBox{
                        id: timesChooseComboBox
                        width: 900 * factor
                        height: 140 * factor
                        comboBoxDisplayList: [ "1", "2", "3", "4", "5" ]
                        anchors.verticalCenter: channelChooseText.verticalCenter
                        anchors.left: channelChooseText.right
                        anchors.leftMargin: 27 * factor
                        curComboBoxIndex: curTestTimes-1
                        bEnable: (curDevType !== GlobalProperty.devBody && curDevType !== GlobalProperty.devRunShort &&
                                  curDevType !== GlobalProperty.devRunMid && curDevType !== GlobalProperty.devRopeSkip)? true : false
                    }
                }

                /*
                Item {
                    id: testTimeItem
                    width: parent.width
                    height: 140 * factor

                    Text {
                        id: maxDeviceText
                        text: qsTr("等待时长(ms)")
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 195 * factor
                        visible: false
                    }

                    TextField{
                        id: waitTimeInputBox
                        width: 860 * factor
                        height: 140 * factor
                        color: "white"
                        placeholderText: "测试开始前等待时长(1-1500)"
                        placeholderTextColor: "gray"
                        font{ family: "Microsoft YaHei";pixelSize: 60 * factor }
                        anchors.verticalCenter: maxDeviceText.verticalCenter
                        anchors.left: maxDeviceText.right
                        anchors.leftMargin: 27 * factor
                        validator: RegExpValidator{regExp: /^[0-9/]+$/}
                        maximumLength: 4
                        text: curWaitTime
                        visible: false
                        background: Rectangle{
                            id: rect
                            width: 860 * factor
                            height: 140 * factor
                            radius: 30 * factor
                            color: "#0C3DA4"
                            border.color: "#1ADDFC"
                        }
                    }
                }

                */
                Item {
                    id: typeSetItem
                    width: parent.width
                    height: 140 * factor

                    Text {
                        id: testTypeText
                        text: "设备模式"
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                        anchors.left: parent.left
                        anchors.leftMargin: 195 * factor
                        anchors.verticalCenter: parent.verticalCenter
                        visible: curDevType === GlobalProperty.devRunShort || curDevType === GlobalProperty.devRunMid
                    }

                    CustComboBox{
                        id: testTypeComboBox
                        width: 900 * factor
                        height: 140 * factor
                        popupHeight: 290 * factor
                        comboBoxDisplayList: [ "体测模式", "比赛模式"]
                        anchors.verticalCenter: testTypeText.verticalCenter
                        anchors.left: testTypeText.right
                        anchors.leftMargin: 27 * factor
                        visible: testTypeText.visible
                        curComboBoxIndex: curTestMode == Enums.TEST_MODE_NORMAL ? 0 : 1
                    }
                }

                Item {
                    id: runShortSetItem
                    width: parent.width
                    height: 140 * factor

                    Text {
                        id: runShortSetText
                        text: "起点设置"
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                        anchors.left: parent.left
                        anchors.leftMargin: 195 * factor
                        anchors.verticalCenter: parent.verticalCenter
                        visible: curDevType === GlobalProperty.devRunShort
                    }

                    CustComboBox{
                        id: runShortSetComboBox
                        width: 900 * factor
                        height: 140 * factor
                        popupHeight: 290 * factor
                        comboBoxDisplayList: [ "取消起点", "存在起点"]
                        anchors.verticalCenter: runShortSetText.verticalCenter
                        anchors.left: runShortSetText.right
                        anchors.leftMargin: 27 * factor
                        visible: runShortSetText.visible
                        curComboBoxIndex: bStartFlag ? 1 : 0
                    }
                }
            }

            Text {
                id: tipsText
                width: parent.width - 100
                wrapMode: Text.WrapAnywhere
                text: qsTr("")
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                anchors.top: itemScrollView.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                visible: false
            }


            Text {
                id: itemChooseText
                text: qsTr("测试项目")
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                anchors.top: devTestCfgColumn.bottom//maxDeviceText.bottom
                anchors.topMargin: 100 * factor
                anchors.left: parent.left
                anchors.leftMargin: 195 * factor
            }

            ScrollView{
                id: itemScrollView
                anchors.top: itemChooseText.top
                anchors.left: itemChooseText.right
                anchors.leftMargin: 27 * factor
                width: 900 * factor
                height: 500 * factor
                clip: true

                Column {
                    id: checkBoxColumn
                    anchors.top: parent.top

                    CheckBox{
                        id: heightBox
                        checked: true
                        enabled: false
                        //checked: curTestItemList.includes(Enums.TestHeight.toString()) ? true: false
                        text: qsTr("身高")
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor}
                        visible: curDevType == GlobalProperty.devBody ? true : false
                        /*
                        onCheckStateChanged: {
                            //身高、体重必选一项
                            if(!checked && !weightBox.checked)
                            {
                                checked = true
                            }
                            getTestItemsFormCheck()
                        }
                        */
                    }

                    CheckBox{
                        id: weightBox
                        checked: true
                        enabled: false
                        //checked: curTestItemList.includes(Enums.TestWeight.toString()) ? true: false
                        text: qsTr("体重")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devBody ? true : false
                        /*
                        onCheckStateChanged: {
                            //身高、体重必选一项
                            if(!checked && !heightBox.checked)
                            {
                                checked = true
                            }
                            getTestItemsFormCheck()
                        }
                        */
                    }

                    CheckBox{
                        id: bmiBox
                        checked: curTestItemList.includes(Enums.TestBMI.toString()) ? true: false
                        text: qsTr("BMI")
                        font: heightBox.font
                        visible: false//curDevType == GlobalProperty.devBody ? true : false
                        onCheckStateChanged: {
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: humanBox
                        checked: curTestItemList.includes(Enums.TestHuman.toString()) ? true: false
                        text: qsTr("人体成分分析")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devBody ? true : false
                        onCheckStateChanged: {
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("肺活量")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devVC ? true : false
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("立定跳远")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devSBJ ? true : false
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("坐位体前屈")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devSFB ? true : false
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("引体向上")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devPullUp ? true : false
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("仰卧起坐")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devSitUp ? true : false
                    }


                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("握力")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devGrip ? true : false
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("视力")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devVision ? true : false
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("俯卧撑")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devPushUp ? true : false
                    }

                    CheckBox{
                        id: vcBox
                        checked: curTestItemList.includes(Enums.TestPVVVC.toString()) ? true: false
                        text: qsTr("肺活量")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devPVV ? true : false
                        onClicked: {
                            forcedVCBox.checked = false
                            maxPVVBox.checked = false
                            checked = true
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: forcedVCBox
                        checked: curTestItemList.includes(Enums.TestPVVFVC.toString()) ? true: false
                        text: qsTr("用力肺活量")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devPVV ? true : false
                        onClicked: {
                            vcBox.checked = false
                            maxPVVBox.checked = false
                            checked = true
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: maxPVVBox
                        checked: curTestItemList.includes(Enums.TestPVVMPV.toString()) ? true: false
                        text: qsTr("最大肺通气量")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devPVV ? true : false
                        onClicked: {
                            vcBox.checked = false
                            forcedVCBox.checked = false
                            checked = true
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: thousandBox
                        checked: true
                        enabled: false
                        text: qsTr("1000m")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunMid && curTestMode == Enums.TEST_MODE_NORMAL? true : false
                    }

                    CheckBox{
                        id: eightHundredBox
                        enabled: curTestMode == Enums.TEST_MODE_NORMAL ? false : true
                        text: qsTr("800m")
                        font: heightBox.font
                        checkable: !checked
                        checked: curTestItemList.includes(Enums.ITEM_RUN_800.toString()) ? true: false
                        visible: curDevType == GlobalProperty.devRunMid ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runMid1500CompetBox.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runMid1500CompetBox
                        checkable: !checked
                        text: qsTr("1500m")
                        font: heightBox.font
                        checked: curTestItemList.includes(Enums.ITEM_RUN_1500.toString()) ? true: false
                        visible: curDevType == GlobalProperty.devRunMid && curTestMode == Enums.TEST_MODE_COMPET? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                eightHundredBox.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortBox
                        checked: curTestItemList.includes(Enums.TestRunShort.toString()) ? true: false
                        text: qsTr("50m")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_NORMAL ? true : false
                        checkable: !checked
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runBackBox.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runBackBox
                        checked: curTestItemList.includes(Enums.TestRunBack.toString()) ? true: false
                        text: qsTr("往返跑 8*50m")
                        font: heightBox.font
                        checkable: !checked
                        visible: curDevType == GlobalProperty.devRunShort  && curTestMode == Enums.TEST_MODE_NORMAL ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortBox.checked = false
                            }

                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortCompet60
                        checked: curTestItemList.includes(Enums.TestRunShort60.toString()) ? true: false
                        text: qsTr("60m")
                        checkable: !checked
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_COMPET ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortCompet100.checked = false
                                runShortCompet200.checked = false
                                runShortCompet400.checked = false
                                runShortCompet4_100.checked = false
                                runShortCompet100Hurdle.checked = false
                                runShortCompet110.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortCompet100
                        checked: curTestItemList.includes(Enums.TestRunShort100.toString()) ? true: false
                        text: qsTr("100m")
                        checkable: !checked
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_COMPET ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortCompet60.checked = false
                                runShortCompet200.checked = false
                                runShortCompet400.checked = false
                                runShortCompet4_100.checked = false
                                runShortCompet100Hurdle.checked = false
                                runShortCompet110.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortCompet200
                        checked: curTestItemList.includes(Enums.TestRunShort200.toString()) ? true: false
                        text: qsTr("200m")
                        checkable: !checked
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_COMPET ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortCompet60.checked = false
                                runShortCompet100.checked = false
                                runShortCompet400.checked = false
                                runShortCompet4_100.checked = false
                                runShortCompet100Hurdle.checked = false
                                runShortCompet110.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortCompet400
                        checked: curTestItemList.includes(Enums.TestRunShort400.toString()) ? true: false
                        text: qsTr("400m")
                        checkable: !checked
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_COMPET ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortCompet60.checked = false
                                runShortCompet100.checked = false
                                runShortCompet200.checked = false
                                runShortCompet4_100.checked = false
                                runShortCompet100Hurdle.checked = false
                                runShortCompet110.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortCompet4_100
                        checked: curTestItemList.includes(Enums.TestRunShort4_100.toString()) ? true: false
                        text: qsTr("4x100m")
                        checkable: !checked
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_COMPET ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortCompet60.checked = false
                                runShortCompet100.checked = false
                                runShortCompet200.checked = false
                                runShortCompet400.checked = false
                                runShortCompet100Hurdle.checked = false
                                runShortCompet110.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortCompet100Hurdle
                        checked: curTestItemList.includes(Enums.TestRunHurdles100.toString()) ? true: false
                        text: qsTr("100m跨栏")
                        checkable: !checked
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_COMPET ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortCompet60.checked = false
                                runShortCompet100.checked = false
                                runShortCompet200.checked = false
                                runShortCompet400.checked = false
                                runShortCompet4_100.checked = false
                                runShortCompet110.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        id: runShortCompet110
                        checked: curTestItemList.includes(Enums.TestRunHurdles110.toString()) ? true: false
                        text: qsTr("110m跨栏")
                        checkable: !checked
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRunShort && curTestMode == Enums.TEST_MODE_COMPET ? true : false
                        onCheckStateChanged: {
                            if(checked)
                            {
                                runShortCompet60.checked = false
                                runShortCompet100.checked = false
                                runShortCompet200.checked = false
                                runShortCompet400.checked = false
                                runShortCompet4_100.checked = false
                                runShortCompet100Hurdle.checked = false
                            }
                            getTestItemsFormCheck()
                        }
                    }

                    CheckBox{
                        checked: true
                        enabled: false
                        text: qsTr("跳绳")
                        font: heightBox.font
                        visible: curDevType == GlobalProperty.devRopeSkip ? true : false
                    }
                }
            }


            //提交按钮
            OtmCommitBtn{
                id: sureBtn
                displayText: "确 认"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50 * factor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Connections{
                target: modelSelectComboBox
                onSignalValueRefresh:{
                    curDevType = index + 1;
                    changeConfig()
                    //curTestTimes = timesChooseComboBox.comboBoxDisplayList[0] * 1;
                    //curWaitTime = waitTimeInputBox.text * 1;
                }
            }

            Connections{
                target: timesChooseComboBox
                onSignalValueRefresh:{
                    curTestTimes = timesChooseComboBox.comboBoxDisplayList[index] * 1;
                }
            }

            Connections{
                target: testTypeComboBox
                onSignalValueRefresh:{
                    curTestMode = index + 1
                    getTestItemsCheckStatus()
                    getTestItemsFormCheck()
                }
            }

            Connections {
                target: runShortSetComboBox
                onSignalValueRefresh: {
                    bStartFlag = index
                }
            }
        }
    }

    Connections {
        target: sureBtn
        onSigBtnClicked: {
            if(curDevType === GlobalProperty.devRunShort)
            {
                InterAction.setRunshortStartFlag(bStartFlag)
            }

            //提交修改
            if(InterAction.setDevTestConfig(curDevType, curTestTimes, curWaitTime, curTestItems))
            {
                tipsText.text = "修改成功"
                //界面同步修改
                GlobalProperty.updateDevTestItem()
            }
            else
            {
                tipsText.text = "修改失败"
            }
            tipsText.visible = true
        }
    }

    function open(){
        m_popup.open()
        curDevType = GlobalProperty.iDevType
        changeConfig()
        InterAction.curPage = Enums.PAGE_SETTING
    }

    function close(){
        m_popup.close()
        InterAction.curPage = GlobalProperty.pageBeforoPopups
    }
}
