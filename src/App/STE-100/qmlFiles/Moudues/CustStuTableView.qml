import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import ".."

Item {
    id: tableViewItem

    property double factor: Screen.height === 1080.0 ? ( Screen.height - 100 )/ 2880.0 : Screen.height / 2880.0

    width: 1526 * factor
    height: 468 * factor

    //数据model
    property var dataModel: commonDataModel

    //表头
    property var headName: ["测试时间", "结果("+ GlobalProperty.getUnitByDevType() + ")", "成绩", "", "", "", ""]

    //列宽
    property var widthArray: [0.3, 0.35, 0.35, 0, 0, 0, 0]

    //单元格类型
    property var cellTypeArray: [GlobalProperty.normalCell, GlobalProperty.normalCell,
                                 GlobalProperty.normalCell, GlobalProperty.normalCell,
                                 GlobalProperty.normalCell, GlobalProperty.normalCell, GlobalProperty.normalCell]
    //列高
    property real itemHeigt: 117 * factor

    //表格字体大小
    property var headerTextPix: 62

    //表格字体大小
    property var tableTextPix: 50

    //表格类型
    property int tableType: 0

    //表格列数
    property int tableCount: 3

    //操作按钮点击信号
    signal signBtnClicked(var index)

    //取消绑定点击信号
    signal sigUnbindBtnClicked(var index)

    //拖拽取消
    signal sigDragEnd()


    function refreshContent()
    {
        tableView.contentY = 0
    }

    z:10

    Component.onCompleted: {
        refreshContent();
        initTableConfig();
    }

    function initTableConfig()
    {
        let tableCfg = {}
        switch(tableType)
        {
        case GlobalProperty.directTestTable:
            tableCfg = GlobalProperty.getDirectTestTableCfg(GlobalProperty.iDevType)
            break
        case GlobalProperty.directTouristTable:
            tableCfg = GlobalProperty.getDirectTouristTableCfg(GlobalProperty.iDevType)
            break
        case GlobalProperty.directScoreTable:
            tableCfg = GlobalProperty.getDirectScoreTableCfg(GlobalProperty.iQueryTestItem)
            break
        case GlobalProperty.syncTestTable:
            tableCfg = GlobalProperty.getSyncTestTableCfg(GlobalProperty.iDevType)
            break
        case GlobalProperty.syncStudentTable:
            tableCfg = GlobalProperty.getSyncStudentTableCfg(GlobalProperty.iDevType)
            break
        case GlobalProperty.asyncStudentTable:
            tableCfg = GlobalProperty.getAsyncStudentTableCfg(GlobalProperty.iDevType)
            break
        case GlobalProperty.asyncTestTable:
            tableCfg = GlobalProperty.getAsyncTestTableCfg(GlobalProperty.iDevType)
            break
        case GlobalProperty.groupScoreTable:
            tableCfg = GlobalProperty.getGroupScoreTableCfg(GlobalProperty.iDevType)
            break
        default:
            tableCfg = GlobalProperty.getDirectTestTableCfg(GlobalProperty.iDevType)
            break
        }

        tableCount = tableCfg.tableCount
        widthArray = tableCfg.widthArray
        headName = tableCfg.headName
        cellTypeArray = tableCfg.cellTypeArray
    }

    //根据设备状态加载图片
    function getDevStateImg(curState)
    {
        if( curState === "设备断连")
        {
            return "qrc:/pic/failed.png"
        }
        else if(curState === "超时结束")
        {
            return "qrc:/pic/error.png"
        }
        else
        {
            return "qrc:/pic/testFinish.png"
        }
    }

    //根据表格类型，设备状态加载文本颜色
    function getTextColor(curState)
    {
        var color = "white"
        var strFoulKeyWords = "FOUL:"
        var strInvalidResultKeyWords = "INVALIDRESULT:"

        if(curState === "测试中")
        {
            color = "#F39800"
        }
        else if(curState === "测试完成")
        {
            color = "#8FC31F"
        }
        else if(curState === "测试出错" || curState === "设备断连" || curState === "超时结束"
                || 0 === curState.indexOf(strFoulKeyWords) || 0 === curState.indexOf(strInvalidResultKeyWords) )
        {
            color = "#FF2E3E"
        }
        return color
    }

    function getStateText(curState)
    {
        var strFoulKeyWords = "FOUL:"
        var strInvalidResultKeyWords = "INVALIDRESULT:"

        if(0 === curState.indexOf(strFoulKeyWords) )
        {
            return curState.substring(strFoulKeyWords.length, curState.length);
        }
        else if(0 === curState.indexOf(strInvalidResultKeyWords) )
        {
            return curState.substring(strInvalidResultKeyWords.length, curState.length);
        }
        else
        {
            return curState
        }
    }

    Connections{
        target: InterAction
        onSignalGradeTypeChange:{
            let tableCfg = GlobalProperty.getDirectScoreTableCfg(testItem)
            tableCount = tableCfg.tableCount
            widthArray = tableCfg.widthArray
            headName = tableCfg.headName
            cellTypeArray = tableCfg.cellTypeArray
        }
    }


    Rectangle{
        id: rectTableView
        anchors.fill: parent
        //radius: 30 * factor
        color: "#483ACE"
        clip: true

        //表格 列头
        Item {
            id: headItem
            width: parent.width
            height: itemHeigt
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            Row {
                anchors.fill: parent
                spacing: -1
                Repeater {
                    model: tableCount
                    Rectangle{
                        id: headItemRect
                        width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[index]
                        height: itemHeigt
                        color: "#3A34B6"
                        border.color: "#D7D5F0"
                        Text {
                            text: tableViewItem.headName[index]
                            font.pixelSize: headerTextPix * factor
                            font.family: "Microsoft YaHei"
                            font.bold: true
                            color: "#D8D6F0"
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }

        //常规测试仪器
        TableView{
            id: tableView
            width: tableViewItem.width
            height: parent.height - headItem.height
            anchors.top: headItem.bottom
            anchors.topMargin: -1
            //spacing: -1
            //rowSpacing: -1
            //columnSpacing: -1
            model: dataModel
            flickableDirection: Flickable.VerticalFlick
            clip: true
            maximumFlickVelocity: 2 * tableView.height
            /*
            //ListView显示最新项
            onCountChanged: {
                if(tableType !== GlobalProperty.directTouristTable)
                {
                    if((dataModel.rowCount() > tableView.height / itemHeigt )  && tableType !== GlobalProperty.directScoreTable)
                    {
                        currentIndex = count -1
                    }
                    else if(dataModel.rowCount() === 1)
                    {
                        //确保第一行位于首页
                        currentIndex = 0
                    }
                }
                else
                {
                    currentIndex = 0
                }
            }
            */

            onRowsChanged: {
                if(tableType !== GlobalProperty.directTouristTable)
                {
                    if((dataModel.rowCount() > tableView.height / itemHeigt )  && tableType !== GlobalProperty.directScoreTable)
                    {
                        contentY += itemHeigt
                    }
                    else if(dataModel.rowCount() === 1)
                    {
                        //确保第一行位于首页
                        contentY = 0
                    }
                }
                else
                {
                    contentY = 0
                }
            }

            onModelChanged: {
                forceLayout()
            }

            onDragEnded: {
                sigDragEnd()
            }

            //内容不能超出边界
            boundsBehavior: Flickable.StopAtBounds

            //滚动条
            ScrollBar.vertical: ScrollBar {

            }

            function checkDevStutas(text1, text2, text3)
            {
                if(text1 === "设备断连" || text2 === "设备断连" || text3 === "设备断连" ||
                   text1 === "超时结束" || text2 === "超时结束" || text3 === "超时结束"  )
                {
                    return true
                }
                return false
            }

            delegate: Row {
                id: delegateRow
                property int curOpacity: 1
                width: parent.width
                height: itemHeigt
                spacing: -1
                Connections {
                    target: tableViewItem
                    //避免鼠标事件与drag事件重叠后丢失
                    onSigDragEnd: {
                        unBindBtnText.color = "#F39800"
                        btnText.color = "#F39800"
                    }
                }

                Component.onCompleted: {
                    initTableConfig()
                }

                //测试出错闪烁整行闪烁
                SequentialAnimation {
                    id: seqanimation
                    running: tableView.checkDevStutas(forth, fifth, sixth) ? true : false
                    loops: Animation.Infinite
                    NumberAnimation {
                        target: delegateRow  //字体对象的id
                        property: "curOpacity"  // 变量为透明度
                        duration: 500
                        to: 0.2
                        easing.type: Easing.InOutQuad
                    }

                    NumberAnimation {
                        target: delegateRow
                        property: "curOpacity"
                        duration: 500
                        to: 1.0
                        easing.type: Easing.InOutQuad
                    }
                    onRunningChanged:
                    {
                        if(!running)
                        {
                           delegateRow.curOpacity = 1
                        }
                    }
                }

                Rectangle {
                    id: firstColunm
                    width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[0]
                    height: parent.height
                    color: "#483ACE"
                    border.color: "#D7D5F0"
                    clip: true
                    Item {
                        id: firstRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        Text {
                            id: fristText
                            text: first
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: "white"
                            opacity: curOpacity
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }

                Rectangle {
                    id: secondColunm
                    width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[1]
                    height: parent.height
                    color: "#483ACE"
                    border.color: "#D7D5F0"
                    clip: true
                    Item {
                        id: secondRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        Text {
                            id: secondText
                            text: second
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: "white"
                            opacity: curOpacity
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }

                Rectangle {
                    id: thirdColunm
                    width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[2]
                    height: parent.height
                    color: "#483ACE"
                    border.color: "#D7D5F0"
                    visible: tableCount > 2 ?  true : false
                    clip: true
                    Item {
                        id: thirdRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        Text {
                            id: thirdText
                            text: third
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color:  "white"
                            opacity: curOpacity
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }

                Rectangle {
                    id: forthColunm
                    width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[3]
                    height: parent.height
                    visible: tableCount > 3 ?  true : false
                    color: "#483ACE"
                    border.color: "#D7D5F0"
                    clip: true
                    Item {
                        id: forthRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[3] === GlobalProperty.normalCell || cellTypeArray[3] === GlobalProperty.testCell ? true : false
                        Text {
                            id: forthText
                            text: getStateText(forth)
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: cellTypeArray[3] === GlobalProperty.testCell ? getTextColor(forth) : "white"
                            horizontalAlignment: Text.AlignHCenter
                            opacity: curOpacity
                        }
                    }

                    Image {
                        id: devStatus
                        visible: !forthRect.visible
                        width: 100 * factor
                        height: 100 * factor
                        anchors.centerIn: parent
                        source: getDevStateImg(forth)
                    }
                }

                Rectangle {
                    id: fifthColunm
                    width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[4]
                    height: parent.height
                    color: "#483ACE"
                    border.color: "#D7D5F0"
                    visible: tableCount > 4 ?  true : false
                    clip: true
                    Item {
                        id: fifthRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: !unBindItem.visible && !cancelBtn.visible
                        Text {
                            id: fifthText
                            text: getStateText(fifth)
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: cellTypeArray[4] === GlobalProperty.testCell ? getTextColor(fifth) : "white"
                            horizontalAlignment: Text.AlignHCenter
                            opacity: curOpacity
                        }
                    }

                    //操作按钮
                    Item {
                        id: unBindItem
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[4] === GlobalProperty.deleteCell ? true : false
                        Text {
                            id: unBindBtnText
                            text: qsTr("取消绑定")
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: "#F39800"
                            horizontalAlignment: Text.AlignHCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                unBindBtnText.color = "#B67200"
                            }
                            onReleased: {
                                unBindBtnText.color = "#F39800"
                            }
                            onClicked: {
                                //取消绑定
                                sigUnbindBtnClicked(third)
                            }
                        }
                    }

                    //操作按钮
                    Item {
                        id: cancelBtn
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[4] === GlobalProperty.btnCell ? true : false
                        Text {
                            id: cancelBtnText
                            text: qsTr("取消成绩")
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: "#F39800"
                            horizontalAlignment: Text.AlignHCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                cancelBtn.color = "#B67200"
                            }
                            onReleased: {
                                cancelBtn.color = "#F39800"
                            }
                            onClicked: {
                                //取消成绩
                                signBtnClicked(first)
                            }
                        }
                    }
                }

                Rectangle {
                    id: sixthColunm
                    width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[5]
                    height: parent.height
                    color: "#483ACE"
                    border.color: "#D7D5F0"
                    visible: tableCount > 5 ?  true : false
                    clip: true
                    //普通文字
                    Item {
                        id: exiRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[5] === GlobalProperty.normalCell || cellTypeArray[5] === GlobalProperty.testCell ? true : false
                        Text {
                            id: sixthText
                            text: getStateText(sixth)
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: cellTypeArray[5] === GlobalProperty.testCell ? getTextColor(sixth) : "white"
                            horizontalAlignment: Text.AlignHCenter
                            opacity: curOpacity
                        }
                    }
                    //设备状态
                    Image {
                        id: devStatusSix
                        visible: cellTypeArray[5] === GlobalProperty.deviceCell ? true : false
                        width: 100 * factor
                        height: 100 * factor
                        anchors.centerIn: parent
                        source: getDevStateImg(sixth)
                    }

                    //操作按钮
                    Item {
                        id: sixthRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[5] === GlobalProperty.btnCell ? true : false
                        Text {
                            id: btnText
                            text: qsTr("取消成绩")
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: "#F39800"
                            horizontalAlignment: Text.AlignHCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                btnText.color = "#B67200"
                            }
                            onReleased: {
                                btnText.color = "#F39800"
                            }
                            onClicked: {
                                //取消成绩
                                signBtnClicked(first)
                            }
                        }
                    }

                    //异步操作按钮
                    Item {
                        id: sixAsyncBtn
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[5] === GlobalProperty.deleteCell ? true : false
                        Image {
                            width: 64 * factor
                            height: 64 * factor
                            anchors.centerIn: parent
                            source: sixth == "true" ? "qrc:/pic/delete.png" : "qrc:/pic/disable.png"
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: sixth == "true" ? true : false
                            onClicked: {
                                //取消绑定
                                sigUnbindBtnClicked(third)
                            }
                        }
                    }
                }

                Rectangle {
                    id: sevenColunm
                    width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[6]
                    height: parent.height
                    color: "#483ACE"
                    border.color: "#D7D5F0"
                    visible: tableCount > 6 ?  true : false
                    clip: true
                    Item {
                        id: sevenRect
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[6] === GlobalProperty.normalCell ? true : false
                        Text {
                            id: seventhText
                            text: seventh
                            fontSizeMode: Text.Fit
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: tableTextPix * factor
                            font.family: "Microsoft YaHei"
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            opacity: curOpacity
                        }
                    }

                    //异步操作按钮
                    Item {
                        id: sevenRectItem
                        anchors.fill: parent
                        anchors.centerIn: parent
                        visible: cellTypeArray[6] === GlobalProperty.deleteCell ? true : false
                        Image {
                            width: 64 * factor
                            height: 64 * factor
                            anchors.centerIn: parent
                            source: seventh == "true" ? "qrc:/pic/delete.png" : "qrc:/pic/disable.png"
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: seventh == "true" ? true : false
                            onClicked: {
                                //取消绑定
                                sigUnbindBtnClicked(third)
                            }
                        }
                    }
                }
            }
        }
    }

    function appendData( jsObj ){
        dataModel.append( jsObj )
    }
}
