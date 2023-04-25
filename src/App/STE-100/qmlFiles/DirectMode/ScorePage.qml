import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import ProjectEnums 1.0
import "../JavaScript/ProjectValue.js" as Assistant
import "."
import "../Moudues/"
import ".."

Item {
    id : root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    property var testItemIndex: -1
    property int testTimeIntervalIndex: -1
    property var queryItemArray: []

    Component.onCompleted: {
        //初始化查询项目
        initQueryItem()

        //默认加载
        queryWhenComingIn()

        InterAction.stopElapsedTimer( "跳转到查成绩界面" )
    }

    //返回按钮
    CustReturnBtn{
        id: btnReturnBtn
        anchors.left: parent.left
        anchors.leftMargin: 113 * factor
        anchors.verticalCenter: strTopCenterText.verticalCenter
        onSignalBtnClicked: {
            InterAction.startElapsedTimer()
            mainRoot.turnToStudentInfoPage()
        }
    }

    //顶部中心文字"成绩查询"
    Text {
        id: strTopCenterText
        text: qsTr( "成绩查询" )
        font.pixelSize: 124 * factor
        font.family: "PingFang SC"
        color: "white"
        font.bold: true
        anchors.top: parent.top
        anchors.topMargin: 240 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //姓名旁边的图标
    Image {
        id: imgNameIcon
        width: 90 * factor
        height: 90 * factor
        source: "qrc:/pic/10-1name.png"
        anchors.right: parent.right
        anchors.rightMargin: 323 * factor
        anchors.verticalCenter: btnReturnBtn.verticalCenter
    }
    //当前测试学生的名字
    Text {
        id : strCurTestStuName
        width: 260 * factor
        elide: Text.ElideMiddle
        text: GlobalProperty.strCurStudentName
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        font.bold: true
        color: "white"
        anchors.verticalCenter: imgNameIcon.verticalCenter
        anchors.left: imgNameIcon.right
        anchors.leftMargin: 32 * factor
    }
    //文字 "体测项目"
    Text {
        id: strTestType
        text: qsTr( "体测项目:" )
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        color: "white"
        anchors.left: parent.left
        anchors.leftMargin: 114 * factor
        anchors.top: parent.top
        anchors.topMargin: 521 * factor
    }

    //体测项目下拉框
    CustComboBox{
        id: testItemComboBox
        width: 620 * factor
        height: 140 * factor
        comboBoxDisplayList: queryItemArray//GlobalProperty.bCurStudentSex === true ? maleArray : femaleArray
        curComboBoxIndex: 0
        anchors.verticalCenter: strTestType.verticalCenter
        anchors.left: strTestType.right
        anchors.leftMargin: 26 * factor
        onSignalValueRefresh: {
            testItemIndex = getTestItemByComboBoxIndex( index )

            GlobalProperty.iQueryTestItem = testItemIndex

            //检查是否有效( 第一个选项为无效项目, 选择了无效的项目, 就清除表格 )
            if( index < 0 ){
                InterAction.cleanDataModel()
                return
            }

            //如果选择的事身高体重, 则切换显示的表格
            if( testItemIndex === Enums.TestHeight ){
                setHumanCompositionScoreViewVisible( true )
            }else{
                setHumanCompositionScoreViewVisible( false )
            }

            queryHistoryScore()
            InterAction.responseGradeTypeChange(testItemIndex)
        }
    }
    //文字 "时间"
    Text {
        id: strTestTime
        text: qsTr( "时间:" )
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        color: "white"
        anchors.left: parent.left
        anchors.leftMargin: 1158 * factor
        anchors.verticalCenter: strTestType.verticalCenter
    }

    //时间选择下拉框
    CustComboBox{
        id: dataSelectComboBox
        comboBoxDisplayList: Assistant.queryScore.timeInterval
        width: 478 * factor
        height: 140 * factor
        curComboBoxIndex: 0
        anchors.verticalCenter: strTestTime.verticalCenter
        anchors.left: strTestTime.right
        anchors.leftMargin: 30 * factor
        onSignalValueRefresh: {
            if( index < 0 ){
                InterAction.cleanDataModel()
                return
            }
            testTimeIntervalIndex = index + 1
            queryHistoryScore()
            stuGradeTableView.refreshContent()
        }
    }

    //成绩类型选择
    CustSelectGradeType{
        id: gradeSelect
        anchors.top: parent.top
        anchors.topMargin: 780 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        onSignalTestTypeChanged: {
            queryHistoryScore()
        }
    }

    //成绩显示矩形
    Rectangle{
        id: rectGrade
        width: 1694 * factor
        height: 1524 * factor
        clip: true
        color: "#3241CC"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 210 * factor
        anchors.horizontalCenter: parent.horizontalCenter

        //身高体重成绩显示区域
        ScoreHumanComposition{
            id: humanCompositionScoreView
            visible: false
            width: parent.width
            height: 1522 * factor
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            onSigLoadFinished: closeRootPopup()
        }

        CustStuTableView{
            id:stuGradeTableView
            visible: false
            width: parent.width
            height: 1522 * factor
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            tableType: GlobalProperty.directScoreTable
        }
    }

    function queryHistoryScore(){
        //数据表恢复置顶
        stuGradeTableView.refreshContent()

        //清空身高体重表格
        humanCompositionScoreView.cleanHumanData()

        if( testItemIndex === 0 ){
            return
        }

        openRootPopup( "loadingPopup" )

        //如果选择查询的时间尺度，未进行选择，则默认查询近三个月
        if( testTimeIntervalIndex === 0 ){
            testTimeIntervalIndex = 3
        }

        var scoreType = gradeSelect.bIsTestGradeType ? Enums.TEST_FORMAL : Enums.TEST_INFORMAL
        InterAction.queryHistoryScore( testItemIndex, testTimeIntervalIndex, scoreType )
    }

    function queryWhenComingIn(){
        //查询当前设备
        setCurComboBoxIndex( GlobalProperty.iDevType )
        InterAction.responseGradeTypeChange(testItemIndex)

        //检查是否是身高体重
        if( testItemIndex === Enums.TestHeight  || testItemIndex === Enums.TestWeight )
        {
            setHumanCompositionScoreViewVisible( true )
        }
        else
        {
            setHumanCompositionScoreViewVisible( false )
        }

        //时间默认三个月
        dataSelectComboBox.curComboBoxIndex = Assistant.queryScore.index_THREE_MONTH
        testTimeIntervalIndex = 3

        //测试成绩
        var testType = Enums.TEST_FORMAL
        InterAction.queryHistoryScore( testItemIndex, testTimeIntervalIndex, testType )
    }

    //设置普通项目, 身高体重成绩的显示状态
    function setHumanCompositionScoreViewVisible( isDisplay ){
        if( isDisplay ){
            humanCompositionScoreView.visible = true
            stuGradeTableView.visible = false
//            gradeSelect.visible = false
        }else{
            humanCompositionScoreView.visible = false
            stuGradeTableView.visible = true
//            gradeSelect.visible = true
        }
    }

    ListModel{
        id: testItemListModel
    }

    //初始化成绩查询项
    function initQueryItem()
    {
        //获取查询配置
        let config = InterAction.getSystemConfigInfo( Enums.PAGE_USING , Enums.CONFIG_QUERY_ITEM)
        let allQueryItem = config.queryTestItemConfig

        //按测试项排序
        allQueryItem.sort(sortItem)

        //解析所有可查询测试项
        for( let ele of allQueryItem ){
            queryItemArray.push(ele.itemName)
            testItemListModel.append( { 'testItemName': ele.itemName, 'testItemVal': ele.itemValue } )
        }

        //设置查询下拉框
        testItemComboBox.comboBoxDisplayList = queryItemArray
    }

    //根据index获取查询项目
    function getTestItemByComboBoxIndex( index ){
        if(index > testItemListModel.count)
            return 0;
        return testItemListModel.get(index).testItemVal
    }

    //根据查询项目确认index
    function findElement(testItem) {
        for(var i = 0; i < testItemListModel.count; i++) {
            var element = testItemListModel.get(i);
            if(String(testItem) === String(element.testItemVal))
            {
                return i;
            }
        }
        return 0;
    }

    //根据测试项进行排序
    function sortItem(a,b){
        return a.itemValue-b.itemValue
    }

    //初始化查询
    function setCurComboBoxIndex( curDevType ){
        switch( curDevType ){
        case GlobalProperty.devVC:
            testItemIndex = Enums.ITEM_VC
            break
        case GlobalProperty.devSBJ:
            testItemIndex = Enums.ITEM_SBJ
            break
        case GlobalProperty.devSFB:
            testItemIndex = Enums.ITEM_SFB
            break
        case GlobalProperty.devPullUp:
            testItemIndex = Enums.ITEM_PULL_UP
            break
        case GlobalProperty.devSitUp:
            testItemIndex = Enums.ITEM_SIT_UP
            break
        case GlobalProperty.devRunMid:
            testItemIndex = GlobalProperty.bCurStudentSex === true ? Enums.ITEM_RUN_1000 : Enums.ITEM_RUN_800
            break
        case GlobalProperty.devRunShort:
            testItemIndex = Enums.ITEM_RUN_50
            break
        case GlobalProperty.devRopeSkip:
            testItemIndex = Enums.ITEM_ROPE_SKIP
            break
        case GlobalProperty.devGrip:
            testItemIndex = Enums.ITEM_GRIP
            break
        case GlobalProperty.devPushUp:
            testItemIndex = Enums.ITEM_PUSHUP
            break
        case GlobalProperty.devBody:
            testItemIndex = Enums.ITEM_WEIGHT
            break
        case GlobalProperty.devFootBall:
            testItemIndex = Enums.ITEM_FOOTBALL
            break
        case GlobalProperty.devBasketBall:
            testItemIndex = Enums.ITEM_BASKETBALL
            break
        case GlobalProperty.devVolleyBall:
            testItemIndex = Enums.ITEM_VOLLEYBALL
            break
        case GlobalProperty.devPUO:
            testItemIndex = Enums.ITEM_PUO
            break
        case GlobalProperty.devSoldBall:
            testItemIndex = Enums.ITEM_SOLIDBALL
            break
        case GlobalProperty.devVerticalJump:
            testItemIndex = Enums.ITEM_VJUMP
            break
        default:
            console.log( "unknown test item..." )
        }
        GlobalProperty.iQueryTestItem = testItemIndex
        testItemComboBox.curComboBoxIndex = findElement(testItemIndex)
    }
}
