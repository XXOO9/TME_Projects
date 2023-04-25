import QtQuick 2.12
import QtQuick.Controls 2.12
import ".."
import "../Moudues/"
import "../Popups"
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common

Item {
    id: root

    width: 430 * factor
    height: 500
    property real perRowHeight: 130 * factor

    readonly property string normalForm: 'qrc:/pic/settings/setting_element.png'
    readonly property string hightLightForm: 'qrc:/pic/settings/setting_curElement.png'

    readonly property var disableDevConfigTypeList: [ 'RUNSHORT', 'RUNMID', 'ROPESKING', 'FOOTBALL', 'BASKETBALL', 'VOLLEYBALL', 'PUO', 'VERTICALJUMP' ]

    signal sigHightLightedIndexChanged( var hightLightIndex )

    Component.onCompleted: {
        loadBlockHouseChannelConfigElement()
    }

    ListModel{
        id: settingElementsModel

        ListElement{ name: "主机设置"; val: 0; hightLight: true  } //Common.settingIndexPad
        ListElement{ name: "设备设置"; val: 1; hightLight: false } //Common.settingIndexDev
        ListElement{ name: "测试配置"; val: 2; hightLight: false } //Common.settingIndexUsing
        ListElement{ name: "数据管理"; val: 3; hightLight: false } //Common.settingIndexDataManage

        function setHighLightIndex( val ){

            //如果是不能设置分机编号的设备，直接返回
            if( val === Common.settingIndexDev ){
                if( !checkDevNumberSelectElementAvailable( GlobalProperty.devType ) ){
                    return
                }
            }

            const size = settingElementsModel.count
            for( let i = 0; i < size; i++ ){
                if( settingElementsModel.get( i ).val === val ){
                    settingElementsModel.get( i ).hightLight = true
                    sigHightLightedIndexChanged( i )
                }else{
                    settingElementsModel.get( i ).hightLight = false
                }
            }
        }
    }

    ListView{
        id: listView
        anchors.fill: parent
        model: settingElementsModel
        spacing: 30 * factor
        delegate: Component{
            Item{
                width: 426 * factor
                height: perRowHeight
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    source: hightLight ? hightLightForm : normalForm
                }
                Text {
                    text: name
                    color: 'white'
                    font{ family: 'Microsoft YaHei'; pixelSize: 60 * factor; bold: hightLightForm }
                    anchors.centerIn: parent
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: settingElementsModel.setHighLightIndex( val )
                }
            }
        }
    }

    //public:



    //private
    //一些不能设置分机编号的设备, 如果点击了设备配置，则直接返回
    function checkDevNumberSelectElementAvailable( devVal ){
        for( let ele of disableDevConfigTypeList ){
            if( ele === devVal ){
                return false
            }
        }

        return true
    }

    //判断是否加载地堡信道配置界面
    function loadBlockHouseChannelConfigElement(){
        if( GlobalProperty.devType === "BASKETBALL" || GlobalProperty.devType === "FOOTBALL" ){
            settingElementsModel.append( { 'name': '地堡配置', 'val': 4, 'hightLight': false } )
        }
    }

}
