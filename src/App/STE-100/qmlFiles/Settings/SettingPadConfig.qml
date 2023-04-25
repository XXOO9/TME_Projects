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
    property bool devConfigChanged: false

    Component.onCompleted: {
        loadSettings()
        isChanged = false
        devConfigChanged = false
    }

    Rectangle{
        id: bkgRect
        anchors.fill: parent
        color: '#3241CC'
        radius: 30 * factor
    }

    Column{
        anchors.fill: parent

        //测试项目配置
        SettingPadTestTypeConfig{
            id: devTypeListConfig
            width: root.width
            height: 500 * factor
            showTopLine: false
            onSigConfigChanged: {
                isChanged = true
                devConfigChanged = true
            }
        }

        SettingPadConnectModelConfig{
            id: directConnectedConfig
            width: root.width
            height: 600 * factor
            onSigConfigChanged: isChanged = true
        }

        SettingPadCommitConfig{
            id: signatureConfig
            width: root.width
            height: 624 * factor
            onSigConfigChanged: function(){
                isChanged = true

                //如果选中的是异步模式，用户又选择了自动提交, 则强制切换为手动提交
                if( !directConnectedConfig.getSyncTestState() && signatureConfig.getAutoCommitCheckedState() ){
                    signatureConfig.setAutoCommitChecked( false )
                    signatureConfig.openWaringPop()
                }
            }
        }

        SettingPadUserInfoDisplayConfig{
            id: userInfoConfig
            width: root.width
            height: 300 * factor
            onSigConfigChanged: isChanged = true
        }
    }

    function loadSettings(){
        let totalPadSetting = InterAction.getSystemConfigInfo( Enums.PAGE_HOST )

        devTypeListConfig.setDevTypeList( totalPadSetting.devTypeList )
        devTypeListConfig.setCurDevTypeName( totalPadSetting.curDevTypeName )
        devTypeListConfig.loadSelectChannelList( totalPadSetting.signalChannelList )
        devTypeListConfig.loadMaxConnectCnt( 30 )
        devTypeListConfig.curDevChannel = totalPadSetting.curDevChannel
        devTypeListConfig.curDevTypeVal = totalPadSetting.curDevTypeValue
        devTypeListConfig.devTypeSelectEnable = totalPadSetting.loadChannelConfig

        if( totalPadSetting.loadChannelConfig ){
            devTypeListConfig.loadChannelConfigArea()
        }

        signatureConfig.setSigNatureSwitchChecked( totalPadSetting.requireSignature )
        signatureConfig.setDelBtnSwitchChecked( totalPadSetting.delScoreBtnEnable )
        signatureConfig.setAutoCommitTime( totalPadSetting.autoCommitTime )

        directConnectedConfig.setConnectedModel( totalPadSetting.manulChangeMode, totalPadSetting.wireLessConnectedModel, totalPadSetting.syncTestModel )

        userInfoConfig.setSwitchChecked( totalPadSetting.requiredUserInfoPage )

    }


    //  读取设置时，判断每一项数值是否合法, 如果有不合法的值，不予更改，并弹窗提醒
    function getTotalSettingConfigInfos(){
        let totalConfigInfo = {}

        let curDevTypeValue        = devTypeListConfig.curDevTypeVal
        let curDevChannel          = devTypeListConfig.curDevChannel
        let maxConnectedCnt        = devTypeListConfig.maxConnectCnt

        let requireSignature       = !signatureConfig.signSwitchState
        let delBtnEnable           = signatureConfig.delBtnswitchState
        let commitTime             = signatureConfig.countTime

        let directConnectedState   = directConnectedConfig.getChangeMode()
        let wireLessConnectedState = directConnectedConfig.getWireLessConnectedState()
        let syncTestState          = directConnectedConfig.getSyncTestState()

        let requireUserInfoPage    = userInfoConfig.getSwitchChecked()

        totalConfigInfo = {
            'curDevTypeValue': curDevTypeValue,
            'delScoreBtnEnable': delBtnEnable,
            'autoCommitTime': commitTime,
            'requireSignature': requireSignature,
            'manulChangeMode': directConnectedState,
            'wireLessConnectedModel': wireLessConnectedState,
            'syncTestModel':syncTestState,
            'requiredUserInfoPage': requireUserInfoPage,
            'curDevChannel': curDevChannel,
            'maxConnectCnt': maxConnectedCnt
        }

        //保存签字版设置
        saveSignatureConfig( requireSignature , delBtnEnable, commitTime, requireUserInfoPage )

        //保存同步异步模式设置
        saveSyncTestModeConfig( syncTestState )

        if( devConfigChanged ){
            InterAction.qmlLogout( "开始切换设备" )
            openRootPopup( "loadingPopWithoutTimer" )
        }

        return totalConfigInfo
    }


    //private:
    function isAllSettingsCorrect(){

        //检查
    }


    //保存签字版设置
    function saveSignatureConfig( requeriedSignature , delBtnEnable, commitTime, userInfoRequired ){
        //设置qml中缓存的签字版状态
        if( requeriedSignature ){
            GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_Signature
        }else{
            GlobalProperty.currentCommitPopup_type = GlobalProperty.commitPopup_value
        }

        GlobalProperty.userInfoPageExited = userInfoRequired

        let delScoreBtnEnable = delBtnEnable
        let commitModel = !requeriedSignature //存在签字板:手动提交 不存在：自动提交
        let autoUploadCountTime = commitTime  //自动提交的倒计时

        //InterAction.saveCommitPopupParamToDataBase(delScoreBtnEnable, commitModel, requeriedSignature, autoUploadCountTime )
        GlobalProperty.isDelBtnEnable = delScoreBtnEnable
    }

    //保存同步异步设置
    function saveSyncTestModeConfig(syncMode)
    {
        if(syncMode)
        {
             GlobalProperty.testType = GlobalProperty.testSyncMode
        }
        else
        {
            GlobalProperty.testType = GlobalProperty.testAsyncMode
        }

        setBRefreshHomePage(true)
    }
}
