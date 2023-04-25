import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common
import ".."
import "../Moudues/"
import "../Popups"

Item{
    id: root
    width: 1245 * factor
    height: 1952 * factor

    property bool isChanged: false

    readonly property string testScoreCmpName: 'testScoreCmp'
    readonly property string timeoutCmpName: 'timeoutCmp'
    readonly property string testCntCmpName: 'testCntCmp'
    readonly property string testItemCmpName: 'testItemCmp'
    readonly property string satrtFlagCmpName: 'satrtFlagCmp'
    readonly property string satrtPosCmpName: 'satrtPosCmp'
    readonly property string ropeDelayCmpName: 'ropeDelayCmp'
    readonly property string circleLenCmpName: 'circleLenCmp'

    property QtObject testScoreSelectObj: null
    property QtObject timeoutSelectObj: null
    property QtObject testCntSelectObj: null
    property QtObject testItemSelectObj: null
    property QtObject startFlagSelectObj: null
    property QtObject startPosSelectObj: null
    property QtObject ropeDelaySelectObj: null
    property QtObject circleLengthSelectObj: null

    property string tmpCurScoreTypeSelect: 'null'
    property string tmpCurTimeoutSelect: 'null'
    property string tmpCurTestCntSelect: 'null'
    property string tmpCurTestItemSelect: 'null'
    property string tmpCurStartFlagSelect: 'null'
    property string tmpCurStartPosSelect: 'null'
    property string tmpCurRopeDelaySelect: 'null'
    property string tmpCurCircleLenSelect: 'null'

    Rectangle{
        id: bkgRect
        anchors.fill: parent
        color: '#3241CC'
        radius: 30 * factor
    }

    ListModel{
        id: socreDisplayListModel
    }

    ListModel{
        id: timeoutListModel
    }

    ListModel{
        id: perUserTestCountModel
    }

    ListModel{
        id: testItemSelectListModel
    }

    ListModel{
        id: startFlagSelectListModel
    }

    ListModel{
        id: startPosSelectListModel
    }

    Column{
        id: testConfigCloumn
        objectName: 'usingColumn'
        Component.onCompleted: {
            //配置通用测试设置
            testScoreSelectObj = usingConfigCmp.createObject( testConfigCloumn, { 'titleString': '测试成绩配置', 'selectModel': socreDisplayListModel,
                                                               'tips': '测试成绩显示选择: ', 'objectName': testScoreCmpName, 'showTopLine': false  } )

            //组连模式不设置页面跳转超时等待时长
            let timeoutSelectObjVis = false
            if(GlobalProperty.bDirectMode)
            {
                timeoutSelectObjVis = true
            }

            timeoutSelectObj = usingConfigCmp.createObject( testConfigCloumn, { 'titleString': '无操作机制设置', 'selectModel': timeoutListModel,
                                            'tips': '无操作返回时长选择: ', 'objectName': timeoutCmpName, 'visible': timeoutSelectObjVis} )

            testCntSelectObj = usingConfigCmp.createObject( testConfigCloumn, { 'titleString': '测试次数', 'selectModel': perUserTestCountModel,
                                            'tips': '测试次数选择: ', 'objectName': testCntCmpName } )

            testItemSelectObj = usingConfigCmp.createObject( testConfigCloumn, { 'titleString': '测试项目选择', 'selectModel': testItemSelectListModel,
                                            'tips': '选择测试项: ', 'objectName': testItemCmpName } )

            //配置设备相关
            let startFlagObjVis = false
            let satrPosObjVis = false
            let ropeDelayObjVis = false
            let circleLenObjVis = false
            if(GlobalProperty.iDevType == GlobalProperty.devRunShort)
            {
                startFlagObjVis = true;
            }
            else if(GlobalProperty.iDevType == GlobalProperty.devSBJ)
            {
                satrPosObjVis = true;
            }
            else if(GlobalProperty.iDevType == GlobalProperty.devRopeSkip)
            {
                ropeDelayObjVis = true;
            }
            else if(GlobalProperty.iDevType == GlobalProperty.devRunMid)
            {
                circleLenObjVis = true;
            }

            //短跑-起点设置
            startFlagSelectObj = usingConfigCmp.createObject( testConfigCloumn, { 'titleString': '起点配置', 'selectModel': startFlagSelectListModel,
                                            'tips': '起点设置: ', 'objectName': satrtFlagCmpName , 'visible': startFlagObjVis} )

            //立定跳远-起跳位置
            startPosSelectObj = usingConfigCmp.createObject( testConfigCloumn, { 'titleString': '起跳长度配置', 'selectModel': startPosSelectListModel,
                                            'tips': '起点长度设置: ', 'objectName': satrtPosCmpName, 'visible': satrPosObjVis} )

            //跳绳-超时等待时间
            ropeDelaySelectObj = usingConfigInput.createObject( testConfigCloumn, { 'titleString': '超时等待配置', 'placeholder': "超时等待(5-30)s",
                                                                  'validatorExp' : /([5-9]|[1-2][0-9]|30)$\d*/, 'maxLen': 2,
                                                                   'tips': '超时等待(s): ', 'objectName': ropeDelayCmpName, 'visible': ropeDelayObjVis} )

            //中长跑-一圈长度配置
            circleLengthSelectObj = usingConfigInput.createObject( testConfigCloumn, { 'titleString': '一圈长度配置', 'placeholder': '中长跑一圈长度',
                                                                  'validatorExp' : /^([1-9][0-9]{0,2}|1000)$/, 'maxLen': 4,
                                                                   'tips': '一圈长度(m): ', 'objectName': circleLenCmpName, 'visible': circleLenObjVis} )

            loadSetting()
        }
    }

    function getTotalSettingConfigInfos(){

        let usingConfig = {
            'timeoutConfig': tmpCurTimeoutSelect,
            'testCntConfig': tmpCurTestCntSelect,
            'scoreConfig' : tmpCurScoreTypeSelect,
            'curTestItemName': tmpCurTestItemSelect,
            'satrtFlagConfig': tmpCurStartFlagSelect,
            'satrtPosConfig': tmpCurStartPosSelect,
            'ropeDelayConfig': tmpCurRopeDelaySelect,
            'circleLengthConfig': tmpCurCircleLenSelect
        }

        //配置界面成绩显示
        setSorceDisplay()

        return usingConfig
    }

    Component{
        id: usingConfigCmp
        CusSettingElementArea{
            id: eleSettingArea
            titleString: '无操作机制设置'
            width: root.width
            height: 342 * factor

            objectName: 'obj'

            property alias selectModel: testTypeSelect.model
            property alias tips: tipsText.text

            Row{
                visible: eleSettingArea.visible
                spacing: 40 * factor
                anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 113 * factor }
                Text {
                    id: tipsText
                    text: '无操作返回时长选择: '
                    color: 'white'
                    font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: testTypeSelect.verticalCenter
                }

                CusSelectCombobox{
                    id: testTypeSelect
                    width: 509 * factor
                    height: 110 * factor
                    bkgRectColor: 'transparent'
                    displayContentTextColor: 'white'
                    bkgRectBorderColor: 'white'
                    bkgRectBorderWidth: 3 * factor
                    selectContentColor: '#4756EB'
                    selectContentHighLightColor: '#1ADDFC'
                    selectContentBorderColor: 'white'
                    localFactor: factor
                    itemHeight: 110 * factor
                    itemTextColor: 'white'
                    model: timeoutListModel
                    onSigCurrentDevIndexChanged: {
                        console.log( "object: " + eleSettingArea.objectName + ' -> ' + curDevValue )
                        if( testScoreCmpName === eleSettingArea.objectName ){
                            tmpCurScoreTypeSelect = curDevValue
                        }
                        if( timeoutCmpName === eleSettingArea.objectName ){
                            tmpCurTimeoutSelect = curDevValue
                        }
                        if( testCntCmpName === eleSettingArea.objectName ){
                            tmpCurTestCntSelect = curDevValue
                        }
                        if( testItemCmpName === eleSettingArea.objectName ){
                            tmpCurTestItemSelect = curDevValue
                        }
                        if( satrtFlagCmpName === eleSettingArea.objectName ){
                            tmpCurStartFlagSelect = curDevValue
                        }
                        if( satrtPosCmpName === eleSettingArea.objectName ){
                            tmpCurStartPosSelect = curDevValue
                        }
                    }
                }
            }

            function setCurComboBoxDisplay( displayVal , realVal){
                testTypeSelect.displayText = displayVal
                if( eleSettingArea.objectName === testScoreCmpName ){
                    tmpCurScoreTypeSelect = realVal
                }
                if( eleSettingArea.objectName === timeoutCmpName ){
                    tmpCurTimeoutSelect = realVal
                    testTypeSelect.displayText += '秒'
                }
                if( eleSettingArea.objectName === testCntCmpName ){
                    tmpCurTestCntSelect = realVal
                    testTypeSelect.displayText += '次'
                }
                if( eleSettingArea.objectName === testItemCmpName ){
                    tmpCurTestItemSelect = realVal
                }
                if( eleSettingArea.objectName === satrtFlagCmpName ){
                    tmpCurStartFlagSelect = realVal
                }
                if( eleSettingArea.objectName === satrtPosCmpName ){
                    tmpCurStartPosSelect = realVal
                    testTypeSelect.displayText += '米'
                }
            }

            function getCurComboBoxDisplay(){
                return testTypeSelect.displayText
            }
        }
    }

    Component {
        id: usingConfigInput
        CusSettingElementArea{
            id: eleSettingArea1
            titleString: '无操作机制设置'
            width: root.width
            height: 342 * factor

            objectName: 'obj'

            property alias placeholder: inputBox.placeholderText
            property alias tips: tipsText.text
            property alias validatorExp: reg.regExp
            property alias maxLen: inputBox.maximumLength

            Row{
                visible: eleSettingArea1.visible
                spacing: 40 * factor
                anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 113 * factor }
                Text {
                    id: tipsText
                    text: '无操作返回时长选择: '
                    color: 'white'
                    font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: inputBox.verticalCenter
                }

                TextField{
                    id: inputBox
                    width: 509 * factor
                    height: 110 * factor
                    color: "white"
                    placeholderText: "配置中长跑测试长度"
                    placeholderTextColor: "gray"
                    font{ family: "Microsoft YaHei";pixelSize: 40 * factor }
                    validator: RegExpValidator { id: reg }
                    maximumLength: 4
                    background: Rectangle{
                        id: rect
                        anchors.fill: parent
                        radius: 12 * factor
                        color: "transparent"
                        border.color: "white"
                        border.width: 2 * factor
                    }
                    onTextChanged: {
                        if( eleSettingArea1.objectName === ropeDelayCmpName ){
                            tmpCurRopeDelaySelect = text
                        }
                        if( eleSettingArea1.objectName === circleLenCmpName ){
                            tmpCurCircleLenSelect = text
                        }
                    }
                }
            }

            function setCurInputBoxDisplay(displayVal)
            {
                inputBox.text = displayVal
                if( eleSettingArea1.objectName === ropeDelayCmpName ){
                    tmpCurRopeDelaySelect = displayVal
                }
                if( eleSettingArea1.objectName === circleLenCmpName ){
                    tmpCurCircleLenSelect = displayVal
                }
            }

            function getCurInputBoxDisplay(){
                return inputBox.text
            }
        }
    }

    function loadSetting(){
        let totalUsingSetting = InterAction.getSystemConfigInfo( Enums.PAGE_USING )

        let scoreSelectModel = totalUsingSetting.scoreSelectConfig
        let timeoutSelectModel = totalUsingSetting.timeoutSelectConfig
        let testCntSelectModel = totalUsingSetting.testCntSelectConfig
        let testItemSelectedModel = totalUsingSetting.devCorrespondingTestItem
        let startFlagSelectModel = totalUsingSetting.startFlagSelectConfig
        let startPosSelectModel = totalUsingSetting.sbjStartPosSelectConfig

        let timeoutVal = totalUsingSetting.timeoutConfig
        let testCntVal = totalUsingSetting.testCntConfig
        let curTestItemName = totalUsingSetting.curTestItemName
        let curTestItemVal = totalUsingSetting.curTestItem
        let scoreDisplay = totalUsingSetting.scoreConfig
        let startFlag = totalUsingSetting.satrtFlagConfig
        let startPos = totalUsingSetting.satrtPosConfig
        let ropeDelay = totalUsingSetting.ropeDelayConfig
        let circleLen = totalUsingSetting.circleLengthConfig

        testScoreSelectObj.setCurComboBoxDisplay( scoreDisplay, scoreDisplay )
        timeoutSelectObj.setCurComboBoxDisplay( timeoutVal, timeoutVal)
        testCntSelectObj.setCurComboBoxDisplay( testCntVal, testCntVal )
        testItemSelectObj.setCurComboBoxDisplay( curTestItemName, curTestItemVal )
        startFlagSelectObj.setCurComboBoxDisplay( startFlag, startFlag )
        startPosSelectObj.setCurComboBoxDisplay( startPos, startPos )
        ropeDelaySelectObj.setCurInputBoxDisplay( ropeDelay, ropeDelay )
        circleLengthSelectObj.setCurInputBoxDisplay( circleLen, circleLen )

        timeoutListModel.clear()
        perUserTestCountModel.clear()

        for( let ele1 of timeoutSelectModel ){
            timeoutListModel.append( { 'fieldName': ele1 + '秒', 'fieldVal': ele1 } )
        }

        for( let ele2 of testCntSelectModel ){
            perUserTestCountModel.append( { 'fieldName': ele2 + '次', 'fieldVal': ele2 } )
        }

        for( let ele3 of testItemSelectedModel ){
            testItemSelectListModel.append( { 'fieldName': ele3.itemName, 'fieldVal': String(ele3.itemValue) } )
        }

        for( let ele4 of scoreSelectModel ){
            socreDisplayListModel.append( { 'fieldName': ele4.itemName, 'fieldVal': String(ele4.itemValue) } )
        }

        for( let ele5 of startFlagSelectModel ){
            startFlagSelectListModel.append( { 'fieldName': ele5 , 'fieldVal': ele5 } )
        }

        for( let ele6 of startPosSelectModel ){
            startPosSelectListModel.append( { 'fieldName': ele6 , 'fieldVal': ele6 } )
        }

    }

    function setSorceDisplay()
    {
        GlobalProperty.isDisplayScore = (tmpCurScoreTypeSelect === "true")
    }
}
