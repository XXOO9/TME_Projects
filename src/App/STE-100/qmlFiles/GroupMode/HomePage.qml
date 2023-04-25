import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../Moudues"
import "../Popups"
import "../"
import "../ConfigObject/"

Item {
    id: root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    Component.onCompleted: {
        onPageComplete()
    }

    //录入身份弹窗
    OtmPopupUserInfoInput{
        id: userAddPopup
        anchors.centerIn: parent
    }

    //系统设置弹窗
    OtmPopupSystemSetting{
        id: systemSettingView
        anchors.centerIn: parent
    }

    //信道配置弹窗
    ChannelConfigPopup {
        id: channelConfigView
        anchors.centerIn: parent
    }

    Loader{
        id: otmLoader
        width: root.width
        height: root.height - 250 * factor
        //source: "qrc:/qmlFiles/GroupMode/DevVC/syncModeView.qml"
        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 170 * factor
    }

    //加载组连模式系统首页
    function turnToGroupHomePage()
    {
        SignPainterConfig.changeOtmSignPainter()
        mainRoot.turnToGroupHomePage()
        var folderName = GlobalProperty.getDevQMLFileName()
        var qmlFileName = GlobalProperty.getTestTypeFileName()
        var targetPage  = "qrc:/qmlFiles/GroupMode/"+ folderName + "/" + qmlFileName + "View.qml"
        console.log("targetPage", targetPage)
        otmLoader.source = ""
        otmLoader.setSource(targetPage)
        InterAction.stopElapsedTimer( "跳转到一对多首页" )
    }

    function turnToPageTestingPage(){
        InterAction.startElapsedTimer()
        mainRoot.turnToGroupSyncTestPage();
        var qmlFileName = GlobalProperty.getDevQMLFileName();
        var targetPage  = "qrc:/qmlFiles/GroupMode/"+ qmlFileName + "/syncMdodelTestingView.qml"
        otmLoader.source = ""
        otmLoader.setSource(targetPage)
    }

    function openKeyBord(){
        userAddPopup.open()
    }

    function closeKeyBord(){
        userAddPopup.close()
    }

    function openSystem(){
        systemSettingView.open()
    }

    function closeSystem(){
        systemSettingView.close()
    }

    function onPageComplete(){
        turnToGroupHomePage()
        console.log("dev", GlobalProperty.devType)

        //2022.07.15 修改， 解决一对多签字版变成了一对多签字版的bug
        SignPainterConfig.initSignPainter( false )

        //2022.07.18加入界面跳转时间
        InterAction.stopElapsedTimer( "跳转到一对多首页" )
    }
}
