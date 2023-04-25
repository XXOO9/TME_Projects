import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../JavaScript/ProjectValue.js" as Common
import ".."
import "../Moudues/"
import "../Popups"
Item{
    id: root

    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    property real curSettingViewIndex: 0
    property bool bRefreshHomePage: false

    //该页面为loader加载 需要屏蔽鼠标事件
    MouseArea {
        anchors.fill: parent
        onClicked: {

        }
    }

//    readonly property var disableDevConfigTypeList: [ 'RUNSHORT', 'RUNMID', 'ROPESKING', 'FOOTBALL', 'BASKETBALL', 'VOLLEYBALL', 'PUO', 'VERTICALJUMP', 'SOLDBALL' ]

    Rectangle{
        anchors.fill: parent
        color: '#4756EB'
    }

    Image {
        id: imgLogo
        width: 110 * factor
        height: width
        source: "qrc:/pic/4-LOGO.png"
        fillMode: Image.PreserveAspectCrop
        anchors{ top: parent.top; left: parent.left; leftMargin: 128 * factor; topMargin: 40 * factor }
    }

    Text {
        id: timeText
        text: '-----'
        font{ family: "Digital-7 Mono"; pixelSize: 100 * factor }
        color: 'white'
        anchors{ top: imgLogo.top; left: imgLogo.right; leftMargin: 1248 * factor  }

        Timer{
            interval: 1000
            repeat: true
            running: true
            onTriggered: timeText.text = Qt.formatDateTime( new Date(), "hh:mm:ss" )
        }
    }


    //返回按钮
    CustReturnBtn{
        id:rectReturnBtn
        anchors{ top: imgLogo.bottom; topMargin: 44 * factor; left: parent.left; leftMargin: 113 * factor }
        onSignalBtnClicked: {
            closeSystemSetting()
            //切换异步模式时 需要刷新
            if(bRefreshHomePage)
            {
                turnToHomePage()
                turnToGroupHomePage()
            }
        }
    }

    Text{
        id: viewTitle
        text: '系统设置'
        color: 'white'
        font{ family: "PingFang SC"; pixelSize: 124 * factor }
        anchors{ horizontalCenter: parent.horizontalCenter; verticalCenter: imgLogo.verticalCenter }
    }

    CusSettingElement{
        id: settingElements

        anchors{ left: parent.left; leftMargin: 113 * factor; top: parent.top; topMargin: 450 * factor; }
        onSigHightLightedIndexChanged: {
            startAnimation()
            turnTargetConfigView( hightLightIndex )
        }
    }

    CusSettingContentView{
        id: settingView
        anchors{ top: settingElements.top; right: parent.right; rightMargin: 112 * factor }

        ParallelAnimation{
            id: animationMgr
            running: true
            PropertyAnimation{ id: opacityAnimatiopn; target: settingView; property: 'opacity'; from: 0; to: 1; duration: 200 }
            PropertyAnimation{ id: rightMarginAnimatiopn; target: settingView; property: 'anchors.rightMargin'; from: -10; to: 112 * factor; duration: 200 }
        }

    }

    Row{
        spacing: 80 * factor
        anchors{ horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 135 * factor }
        Repeater{
            model: [ '保存设置' ]
            delegate: cusClickedBtn
        }
    }

    CusSettingWaringPop{
        id: waringPop
        anchors.centerIn: parent
        modal: true

        onSigOk: {
            saveConfigInfos()
            waringPop.close()
            turnTargetConfigView( waringPop.nextPageIndex )
        }

        onSigCancel: {
            waringPop.close()
            settingView.configChanged = false
            turnTargetConfigView( waringPop.nextPageIndex )
        }
    }

    CusAutoCloseTipsPop{
        id: autoCloseTipsPop
        anchors.centerIn: parent
        modal: true
    }

    Component{
        id: cusClickedBtn
        Rectangle{
            id: clickedBtn
            width: 460 * factor
            height: 168 * factor
            radius: 84 * factor
            color: "#009E96"
            Text {
                text: modelData
                color: 'white'
                //color: GlobalProperty.iDevType !== GlobalProperty.devUnknow ? 'white' : "gray"
                font{ family: 'Microsoft YaHei'; pixelSize: 70 * factor; bold: true }
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea{
                id: btnMouseArea
                anchors.fill: parent
                //enabled: GlobalProperty.iDevType !== GlobalProperty.devUnknow
                onClicked: saveConfigInfos()
            }

            states: [
                State{
                    when: btnMouseArea.pressed
                    PropertyChanges{ target: clickedBtn; opacity: 0.8 }
                }
            ]
        }
    }

    function startAnimation(){
        animationMgr.start()
    }

    function turnTargetConfigView( index ){
//        //如果点击的是设备配置，检查是否为可以设置分机编号的设备
//        if( Common.settingIndexDev === index  ){
//            if( !checkDevNumberSelectElementAvailable( GlobalProperty.devType ) ){
//                return
//            }
//        }

        //先检查是否有改动
        if( settingView.configChanged ){
            waringPop.nextPageIndex = index
            waringPop.open()
            return
        }

        startAnimation()
        settingView.turnPage( index )
        curSettingViewIndex = index
    }

    //保存当前的设置
    function saveConfigInfos(){
        settingView.configChanged = false
        let curTotalConfigInfos = settingView.getCurrentPageTotalConfigInfos()
        InterAction.setSystemConfigInfo( curSettingViewIndex, curTotalConfigInfos )
        GlobalProperty.updateDevTestItem()
        autoCloseTipsPop.openAutoClosePop()
    }

    //设置是否需要刷新页面
    function setBRefreshHomePage(refresh)
    {
        bRefreshHomePage = refresh
    }

    Connections{
        target: InterAction
        onSigUpdateHWConfig:{
            if(!aws)
            {
                closeRootPopup()
                autoCloseTipsPop.tipString = '设置失败'
                autoCloseTipsPop.openAutoClosePop()
                closeRootPopup()
            }
            else
            {
                closeRootPopup()
                autoCloseTipsPop.openAutoClosePop()
                closeRootPopup()
            }
        }
    }
}
