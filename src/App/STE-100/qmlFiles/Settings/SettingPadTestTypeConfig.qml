import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common
import ".."
import "../Moudues/"
import "../Popups"

CusSettingElementArea{
    id: root

    titleString: '测试项目配置'

    property string curDevTypeVal: '-1'
    property string curDevChannel: '-1'
    property string maxConnectCnt: '-1'

    property var channelAndMaxConfigObj: undefined

    property alias devTypeSelectEnable: testTypeSelect.mouseEnable

    ListModel{

        id: devTypeListModel
    }

    ListModel{
        id: devSignalChannelListModel

        function loadCorrespondingDevChannels( devType ){
            let channelList = InterAction.queryCorrespondingChannelList( devType )
            loadSelectChannelList( channelList )

            if( root.channelAndMaxConfigObj !== undefined ){
                curDevChannel = channelList[ 0 ]
                maxConnectCnt = 5
                root.channelAndMaxConfigObj.setDisplayText( curDevChannel, maxConnectCnt )
            }
        }
    }

    function loadSelectChannelList( devChannelArray ){
        devSignalChannelListModel.clear()
        for( let ele of devChannelArray ){
            devSignalChannelListModel.append( { 'fieldName': ele, 'fieldVal': ele } )
        }
    }

    ListModel{
        id: devMaxConnectedNumberListModel
    }

    function loadMaxConnectCnt( maxCnt ){
        root.maxConnectCnt = maxCnt
        devMaxConnectedNumberListModel.clear()
        while( maxCnt-- ){
            devMaxConnectedNumberListModel.append( { 'fieldName': maxCnt + 1, 'fieldVal': maxCnt + 1 } )
        }
    }


    Row{
        id: devTypeRow
        spacing: 40 * factor
        anchors{ left: root.left; leftMargin: root.leftOffset; top: root.top; topMargin: 152 * factor }
        z: 10
        Text {
            text: '主机测试项目: '
            color: 'white'
            font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: testTypeSelect.verticalCenter
        }

        CusSelectCombobox{
            id: testTypeSelect
            width: 660 * factor
            height: 110 * factor
            z: 12
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
            model: devTypeListModel

            onSigCurrentDevIndexChanged: {
//                console.log( 'select new dev val = ' + curDevValue )
                root.curDevTypeVal = curDevValue
                devSignalChannelListModel.loadCorrespondingDevChannels( curDevValue )
                sigConfigChanged()
            }


        }
    }

    Component{
        id:channelConfigAreaCmp
        Row{
            spacing: 20 * factor
            anchors{ top: devTypeRow.bottom; topMargin: 10 * factor; left: devTypeRow.left }
            Column{
                spacing: 10 * factor
                Text {
                    text: '信道选择'
                    color: 'white'
                    font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                    verticalAlignment: Text.AlignVCenter
                }

                CusSelectCombobox{
                    id: devChannelSelectComboBox
                    width: 300 * factor
                    height: 110 * factor

                    displayText: root.curDevChannel

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
                    model: devSignalChannelListModel

                    onSigCurrentDevIndexChanged: {
                        root.curDevChannel = curDevValue
                        sigConfigChanged()
                    }
                }
            }

            Column{
                spacing: 10 * factor
                Text {
                    text: '最大连接数量'
                    color: 'white'
                    font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                    verticalAlignment: Text.AlignVCenter
                }

                CusSelectCombobox{
                    id: maxConnectedNumberSelected
                    width: 300 * factor
                    height: 110 * factor

                    displayText: root.maxConnectCnt

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
                    model: devMaxConnectedNumberListModel

                    onSigCurrentDevIndexChanged: {
                        root.maxConnectCnt = curDevValue
                        sigConfigChanged()
                    }
                }
            }

            function setDisplayText( channel, maxConnectedCnt ){
                devChannelSelectComboBox.displayText = channel
                maxConnectedNumberSelected.displayText = maxConnectedCnt
            }
        }
    }




    function setDevTypeList( devTypeList ){
        devTypeListModel.clear()
        for( let ele of devTypeList ){
            devTypeListModel.append( ele )
        }
    }

    function setCurDevTypeName( devTypeName ){
        testTypeSelect.displayText = devTypeName
    }

    function getCurDevTypeValue(){
        return root.curDevTypeVal
    }


    function loadChannelConfigArea(){
        root.channelAndMaxConfigObj = channelConfigAreaCmp.createObject
                ( root, { 'anchors.top': devTypeRow.bottom, 'anchors.topMargin': 15 * factor, 'anchors.left': devTypeRow.left  })
    }
}
