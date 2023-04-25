/*
  需要校准的设备:
  1.身高体重
  2.握力
  3.跳远起跳点
*/

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

    Component.onCompleted: {
        loadSetting()
    }

    Rectangle{
        id: bkgRect
        anchors.fill: parent
        color: '#3241CC'
        radius: 30 * factor
    }

    //未占用的通道号
    ListModel{      // devChannelIndex
        id: unUsedDevChannelNumbersModel
        Component.onCompleted: {
            const max = 20   //最大连接数
            for( let index = 1; index <= max; index++ ){

                if( devNumberConfigModel.isDevOrderExisted( index ) ){
                    continue
                }

                unUsedDevChannelNumbersModel.append( { 'fieldName': String( index ), 'fieldVal': String( index ) } )
            }
        }

        function findTargetChannelNumberIndex( num ){
            const size = unUsedDevChannelNumbersModel.count

            for( let index = 0; index < size; index++ ){
                if( num === unUsedDevChannelNumbersModel.get( index ).fieldVal ){
                    return index
                }
            }

            return -1
        }

        function removeTargetChannelNumber( channelNumber ){
            let tmp = findTargetChannelNumberIndex( channelNumber )

            if( tmp !== -1 ){
                unUsedDevChannelNumbersModel.remove( tmp )
            }
        }

        function replaceChannel( src, tar ){
            let index = findTargetChannelNumberIndex( tar )
            unUsedDevChannelNumbersModel.get( index ).fieldName = src
            unUsedDevChannelNumbersModel.get( index ).fieldVal = src
        }
    }

    ListModel{
        id: devNumberConfigModel
        ListElement{ firstColumn: 'testDev_001'; secondColumn: '0' }
        ListElement{ firstColumn: 'testDev_002'; secondColumn: '0' }
        ListElement{ firstColumn: 'testDev_003'; secondColumn: '0' }

        function constructDevChannelInfo2Array(){
            let infoArray = []
            let tmpInfo = {}
            const size = devNumberConfigModel.count

            for( let i = 0; i < size; i++ ){
                tmpInfo = {}
                tmpInfo[ 'devNumber' ]   = devNumberConfigModel.get( i ).firstColumn
                tmpInfo[ 'devChannel' ]  = devNumberConfigModel.get( i ).secondColumn

                infoArray.push( tmpInfo )
            }

            return infoArray
        }

        function loadDevChannels( srcDevChannelArray ){
            devNumberConfigModel.clear()

            for( let ele of srcDevChannelArray ){
                devNumberConfigModel.append( ele )
            }
        }

        function findDev( targetDevNumber ){
            const size = devNumberConfigModel.count

            for( let i = 0; i < size; i++ ){
                if( devNumberConfigModel.get( i ).firstColumn === targetDevNumber ){
                    return devNumberConfigModel.get( i )
                }
            }

            return null
        }

        //判断目标分机编号是否被占用
        function isDevOrderExisted( tarDevOrder ){
            const size = devNumberConfigModel.count

            tarDevOrder = String( tarDevOrder )

            for( let i = 0; i < size; i++ ){
                if( devNumberConfigModel.get( i ).secondColumn === tarDevOrder ){
                    return true
                }
            }

            return false
        }

        //获取已经占用的通道号
        function getAssisginDevChannels( targetDevNumber = 'null' ){
            const size = devNumberConfigModel.count
            let retChannels = []
            let index = 0
            if( targetDevNumber === 'null' )
            {
                for( ;index < size; index++ ){
                    retChannels.push( devNumberConfigModel.get( index ).secondColumn )
                }
            }
            else
            {
                retChannels.push( findDev( targetDevNumber ).secondColumn )
            }

            return retChannels
        }

        function setTargetDevChannelNumber( targetDevNumber, newChannelNumber ){
            findDev( targetDevNumber ).secondColumn = newChannelNumber
        }
    }

    ListModel{
        id: devCorrectConfigModel
        ListElement{ firstColumn: 'testCorrect_001'; secondColumn: '001' }
        ListElement{ firstColumn: 'testCorrect_002'; secondColumn: '002' }
        ListElement{ firstColumn: 'testCorrect_003'; secondColumn: '003' }

        function loadDevNumbers( devNumberArray ){
            devCorrectConfigModel.clear()

            for( let ele of devNumberArray ){
                devCorrectConfigModel.append( { 'firstColumn': ele, 'secondColumn': '' } )
            }
        }


    }

    ListModel{
        id: outAssistantConfigModel
        ListElement{ assistantDevName: '发令枪'; devEnable: true;  devChecked: true }
        ListElement{ assistantDevName: '打印机'; devEnable: false; devChecked: false }
        ListElement{ assistantDevName: '摄像头'; devEnable: false; devChecked: false }
    }

    Column{
        id: devNumberAndCorrectAreaColumn
        Component.onCompleted: {
            listViewCmp.createObject( devNumberAndCorrectAreaColumn,
                                     { 'devNumberConfigModel': devNumberConfigModel, 'titleString': '分机编号设置',
                                         'listHeaderDataModel': [ '设备号', '分机编号设置' ], 'showTopLine': false } )

            listViewCmp.createObject( devNumberAndCorrectAreaColumn,
                                     { 'devNumberConfigModel': devCorrectConfigModel, 'titleString': '设备校准',
                                         'correctDevBtn': true, 'listHeaderDataModel': [ '设备号', '操作' ] } )

            outAssistantDevConfigCmp.createObject( devNumberAndCorrectAreaColumn )
        }
    }


    Popup{
        id: devCorrectPop
        Component.onCompleted: {
            let tmpDevType = GlobalProperty.devType
            console.log( 'dev type = ' + tmpDevType )
            switch( tmpDevType ){
            case "ZWTQQ":
                correctLoader.source = 'qrc:/qmlFiles/CorrectView/CorrectDevSFB.qml'
                return

            case "BODY":
                correctLoader.source = 'qrc:/qmlFiles/CorrectView/CorrectDevBody.qml'
                return

            case "ROPEGPIP":
                correctLoader.source = 'qrc:/qmlFiles/CorrectView/CorrectDevGrip.qml'
                return

            case "PUSHUP":
                correctLoader.source = 'qrc:/qmlFiles/CorrectView/CorrectDevPushup.qml'
                return

            case "SOLDBALL":
                correctLoader.source = 'qrc:/qmlFiles/CorrectView/CorrectDevSoldBall.qml'
                return
            }
        }

        onOpened: console.log( 'cur correct dev number = ' + Common.curCorrectDevNumber )
        width: root.width
        height: root.height
        anchors.centerIn: root
        closePolicy: Popup.NoAutoClose
        padding: 0
        Loader{
            id: correctLoader
            source: 'qrc:/qmlFiles/CorrectView/CorrectDevSFB.qml'
            anchors.fill: parent
        }
    }


    Component{
        id: outAssistantDevConfigCmp
        CusSettingElementArea{
            titleString: '外接辅助设备'
            width: root.width
            height: 576 * factor

            Column{
                anchors{ top: parent.top; topMargin: 178 * factor; left: parent.left; leftMargin: 123 * factor }
                spacing: 50 * factor

                Repeater{
                    model: outAssistantConfigModel
                    delegate: cusCheckBox
                }
            }
        }
    }


    Component{
        id: cusCheckBox
        CheckBox{
            id: checkBtn
            checkable: devEnable
            checked: devChecked
            indicator: Rectangle{
                visible: checkBtn.checked
                width: 70 * factor
                height: width
                radius: width / 7
                color: '#1473E6'
                border.color: '#FFFFFF'
                Image {
                    width: 46 * factor
                    height: 35 * factor
                    source: 'qrc:/pic/settings/gougou.png'
                    anchors.centerIn: parent
                }
            }

            contentItem: Text {
                text: assistantDevName
                color: '#FFFFFF'
                font.strikeout: !devEnable
                font{ family: 'Microsoft YaHei'; pixelSize: 50 * factor }
                verticalAlignment: Text.AlignVCenter
                leftPadding: checkBtn.indicator.width + checkBtn.leftPadding
                anchors.verticalCenter: checkBtnBkg.verticalCenter
            }

            background: Rectangle{
                id: checkBtnBkg
                width: 70 * factor
                height: width
                radius: width / 7
                color: 'transparent'
                border.color: 'white'
            }
        }
    }



    Component{
        id: listViewCmp
        CusSettingElementArea{
            id: elementArea
            width: root.width
            height: 687 * factor

            property alias devNumberConfigModel: listView.model
            property bool  correctDevBtn: false
            property alias listHeaderDataModel: headerRepeter.model

            Row{
                id: headerRow
                width: 1019 * factor
                height: 113 * factor
                spacing: -1 * factor
                property alias headerDataModel: headerRepeter.model
                anchors{ top: parent.top; topMargin: 152 * factor; horizontalCenter: parent.horizontalCenter }
                Repeater{
                    id: headerRepeter
                    model: listHeaderDataModel
                    delegate: Rectangle{
                        width: 508 * factor
                        height: 113 * factor
                        color: '#1D28A1'
                        border.color: '#4E57C0'
                        Text {
                            text: modelData
                            color: 'white'
                            font{ family: 'Microsoft YaHei'; pixelSize: 50 * factor }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.horizontalAlignment
                            anchors.centerIn: parent
                        }
                    }
                }
            }

            ListView{
                id: listView
                width: 1019 * factor
                height: 330 * factor
                clip: true
                anchors{ top: headerRow.bottom; left: headerRow.left }
                delegate: listDelegate
            }

            Component{
                id: listDelegate
                Row{
                    id: operationRow
                    spacing: -eleRect.border.width
                    Component.onCompleted: {
                        if( elementArea.correctDevBtn ){
                            devCorrectCmp.createObject( operationRow, { 'devNumber': firstColumn } )
                        }else{
                            devChannelComboBoxCmp.createObject( operationRow, { 'model': unUsedDevChannelNumbersModel,
                                                                   'displayText': secondColumn, 'devNumber': firstColumn } )
                        }
                    }

                    Rectangle{
                        id: eleRect
                        width: 510 * factor
                        height: 113 * factor
                        color: '#1D28A1'
                        border.color: '#4E57C0'
                        Text {
                            text: firstColumn
                            color: 'white'
                            font{ family: 'Microsoft YaHei'; pixelSize: 50 * factor }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.horizontalAlignment
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }

    Component{
        id: devChannelComboBoxCmp
        CusSelectCombobox{
            id: devChannelOperationComboBox
            property string devNumber: 'null'
            width: 510 * factor
            height: 113 * factor
            bkgRadius: 0
            displayText: '0'
            displayContentTextColor: 'white'
            bkgRectColor: '#1D28A1'
            bkgRectBorderWidth: 1.5 * factor
            bkgRectBorderColor: '#4E57C0'
            itemHeight: height
            localFactor: factor
            model: unUsedDevChannelNumbersModel

            onSigCurrentDevIndexChanged: {

                let curTargetDevChannelNumber = devNumberConfigModel.getAssisginDevChannels( devNumber )
                //                console.log( 'devNumber = ' + devNumber + ', ' + 'channel = ' + curTargetDevChannelNumber[ 0 ] )
                if( curTargetDevChannelNumber[ 0 ] === '0' )
                {
                    unUsedDevChannelNumbersModel.removeTargetChannelNumber( curDevValue )
                }
                else
                {
                    unUsedDevChannelNumbersModel.replaceChannel( curTargetDevChannelNumber[ 0 ], curDevValue )
                }

                devNumberConfigModel.setTargetDevChannelNumber( devNumber, curDevValue )
            }
        }
    }

    Component{
        id: devCorrectCmp
        Rectangle{
            id: operationRect
            property string devNumber: '-1'

            width: 508 * factor
            height: 110 * factor
            color: '#1D28A1'
            border.color: '#4E57C0'
            Text {
                text: '校准'
                color: '#F39800'
                font{ family: 'Microsoft YaHei'; pixelSize: 50 * factor }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    Common.curCorrectDevNumber = operationRect.devNumber
                    devCorrectPop.open()
                }
            }
        }
    }

    function loadSetting(){
        let totalDevSetting = InterAction.getSystemConfigInfo( Enums.PAGE_DEV )
        devCorrectConfigModel.loadDevNumbers( totalDevSetting.devCorrectConfig )
        devNumberConfigModel.loadDevChannels( totalDevSetting.devChannelConfig )
    }


    function getTotalSettingConfigInfos(){
        let totalConfig = {}
        totalConfig.channelConfig = devNumberConfigModel.constructDevChannelInfo2Array()

        return totalConfig
    }

    function openCorrectPop(){
        devCorrectPop.openCorrectPop(  )
        console.log( "ret = " + GlobalProperty.devType )
    }
}
