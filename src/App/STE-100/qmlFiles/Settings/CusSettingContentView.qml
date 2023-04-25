import QtQuick 2.12
import QtQuick.Controls 2.12
import ".."
import "../Moudues/"
import "../Popups"
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common

Item {
    id: root
    width: 1245 * factor
    height: 2052 * factor

    property bool configChanged: false
    Component.onCompleted: {
        InterAction.sigGetSpecificSettingViewInfo( Common.settingIndexPad )
    }

    Binding{
        target: root
        property: 'configChanged'
        value: settingLoader.item.isChanged
    }

    Loader{
        id: settingLoader
        source: 'qrc:/qmlFiles/Settings/SettingPadConfig.qml'
        anchors.fill: parent
    }

    function turnPage( index ){

        let taregetUrl = ''

        switch( index ){
        case Common.settingIndexPad:
            taregetUrl = 'qrc:/qmlFiles/Settings/SettingPadConfig.qml'
            break

        case Common.settingIndexDev:
            taregetUrl = 'qrc:/qmlFiles/Settings/SettingDevConfig.qml'
            break

        case Common.settingIndexUsing:
            taregetUrl = 'qrc:/qmlFiles/Settings/SettingUsingConfig.qml'
            break

        case Common.settingIndexDataManage:
            taregetUrl = 'qrc:/qmlFiles/Settings/SettingDataManageConfig.qml'
            break

        case Common.settingBlockHouseChannel:
            taregetUrl = 'qrc:/qmlFiles/Settings/SettingBlockHouseConfig.qml'
            break

        default:
            taregetUrl = 'qrc:/qmlFiles/Settings/SettingPadConfig.qml'
            break
        }

        settingLoader.source = taregetUrl
    }

    function getCurrentPageTotalConfigInfos(){
        return settingLoader.item.getTotalSettingConfigInfos()
    }


}
