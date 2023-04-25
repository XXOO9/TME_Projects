/*
     @ FileName:     OtherSettingsConfigPop.qml
     @ Describe:
     1.该控件为其他设置弹窗控件， 打开时弹窗时， 首先获取设备类型，根据不同的设备类型加载出不同的设置项
     2.某些设置项为所有设备的通用设置项目， 比如切换直连和组网的连接模式，通用的设置项全部加载
     @ Author:       xutianci
     @ Attentions:
     @ Create Date:  2022-10-08  14:19
*/
import QtQuick 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../JavaScript/ProjectValue.js" as Common
import "../Moudues/"
import "../"


Popup {
    id: root
    width: 1582 * factor
    height: 1400 * factor


    property real curTestItem: -1        //当前设备类型
    property bool directModel: true     // 设备是直连还是组网连接

    modal: true

    background: Rectangle{
        anchors.fill: parent
        radius: 30 * factor
        color: "#768CF7"
    }

    onOpened: onPopopened()



    // elementName
    ListModel{
        id: settingsElementsModel

        /********************设置项的字段*********************/
        readonly property real changeDevConnectModel: 0      //  设置立定跳远起始位置
        readonly property real standingJumpStartPos: 1       //  设置立定跳远起始位置
        readonly property real newSystemSettings: 2          //  设置界面
    }

    StackView{
        id: settingsStackArea
        anchors.fill: parent
        clip: true
        initialItem: settingsInitView
    }

    Component{
        id: settingsInitView
        Item {
            width: root.width
            height: root.height
            ListView{
                id: menuView
                width: root.width / 3
                height: root.height
                anchors.centerIn: parent
                model: settingsElementsModel

                delegate: Component{
                    Rectangle{
                        readonly property real perRowHeight: 200 * factor
                        readonly property string menuElementColor: "#384DD5"
                        readonly property string menuElementBorderColor: "green"
                        width: menuView.width
                        height: perRowHeight
                        color: menuElementColor
                        border.color: menuElementBorderColor
                        radius: 30 * factor

                        Text {
                            text: eleSettingName
                            color: "white"
                            font{ pixelSize: 60 * factor }
                            anchors.centerIn: parent
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: enterSpecificSettingView( eleSettingFlag )
                        }
                    }
                }
            }
        }

    }

    function onPopopened(){

        //加载所有设备共有的设置选项
        loadCommonSettingElements()

        curTestItem = parseInt( GlobalProperty.curDevTestItem )

        switch( curTestItem ){

        case Enums.ITEM_SBJ:
            loadStandingJumpConfig()
            break

        default:
            break
        }
    }

    //根据不同的设置索引打开加载响应的界面
    function enterSpecificSettingView( settingFlag ){
        switch( settingFlag ){
        case settingsElementsModel.standingJumpStartPos:
            loadStandingJumpStartPosView()
            break

        case settingsElementsModel.changeDevConnectModel:
            loadChangeDevConnectModelView()
            break

        case settingsElementsModel.newSystemSettings:
            loadTmpNewSystemView()
            break

        default:
            break
        }
    }

    //加载所有设备共有的设置项
    function loadCommonSettingElements(){
        settingsElementsModel.append( { "eleSettingName": "切换设备连接模式", "eleSettingFlag": settingsElementsModel.changeDevConnectModel } )
        settingsElementsModel.append( { "eleSettingName": "详细设置", "eleSettingFlag": settingsElementsModel.newSystemSettings } )
    }


    //加载立定跳远设置菜单
    function loadStandingJumpConfig(){
        settingsElementsModel.append( { "eleSettingName": "设置起跳点", "eleSettingFlag": settingsElementsModel.standingJumpStartPos } )
    }

    //加载立定跳远设置起跳点界面
    function loadStandingJumpStartPosView(){
        settingsStackArea.push( standingJumpStartPosSettingView, StackView.PopTransition )
    }

    //加载切换设备连接模式的界面
    function loadChangeDevConnectModelView(){
        settingsStackArea.push( changeConnectModelView )
    }


    //临时放这里，加载新版设置
    function loadTmpNewSystemView(){
        mainLoader.source = 'qrc:/qmlFiles/Settings/SettingHome.qml'
    }


    //切换设备连接模式界面
    Component{
        id: changeConnectModelView
        Item {
            id: changeConnectModelViewItem
            width: settingsStackArea.width
            height: settingsStackArea.height

            //界面加载完成后，读取数据库中的设备连接信息并加载到界面上
            Component.onCompleted: {
                let devConnectInfo = InterAction.getLocalDevConnectInfo()

                connecteModel.changeDevConnectModel( devConnectInfo.connectModel )
                connecteModelAutoChange.changeDevConnectModelAutoChanged( devConnectInfo.autoChange )
            }

            Image{
                id: returnBtn
                width: 150 * factor
                height: width
                source: Common.returnImg
                anchors{ top: parent.top; topMargin: 50 * factor; left: parent.left; leftMargin: 50 * factor }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        settingsStackArea.pop( StackView.PopTransition )
                    }
                }
            }

            ButtonGroup{
                buttons: rowConnectModelBtns.children
            }

            Text {
                visible: rowConnectModelBtns.visible
                text: "选择设备连接模式:"
                color: "white"
                font{ pixelSize: 70 * factor; bold: true }
                anchors{ bottom: rowConnectModelBtns.top; bottomMargin: 70 * factor; left: rowConnectModelBtns.left }
            }

            ListModel{
                id: connecteModel
                ListElement{ connectType: "直连模式"; setState: false }
                ListElement{ connectType: "组网模式"; setState: false }

                function changeDevConnectModel( directConnect ){
                    if( directConnect ){
                        connecteModel.get( 0 ).setState = true
                        connecteModel.get( 1 ).setState = false
                    }else{
                        connecteModel.get( 0 ).setState = false
                        connecteModel.get( 1 ).setState = true
                    }
                }

                function isDirectConnectModel(){
                    let ok = connecteModel.get( 0 ).setState
                    return ok
                }
            }

            ListModel{
                id: connecteModelAutoChange
                ListElement{ changeType: "手动切换"; setState: false }
                ListElement{ changeType: "自动切换"; setState: false }

                function changeDevConnectModelAutoChanged( autoChanged ){
                    if( autoChanged ){
                        connecteModelAutoChange.get( 0 ).setState = false
                        connecteModelAutoChange.get( 1 ).setState = true
                    }else{
                        connecteModelAutoChange.get( 0 ).setState = true
                        connecteModelAutoChange.get( 1 ).setState = false
                    }
                }

                function isAutoChangeConnect(){
                    let ok = connecteModelAutoChange.get( 1 ).setState
                    return ok
                }
            }

            //选择设备连接模式
            Row{
                id: rowConnectModelBtns
                spacing: 400 * factor
                anchors{ top: rowConnectModelAutoChangeBtns .bottom; topMargin: 80 * factor; horizontalCenter: rowConnectModelAutoChangeBtns.horizontalCenter }
                Repeater{
                    model: connecteModel
                    delegate: RadioButton{
                        id: radioBtn
                        width:  250 * factor
                        height: width
                        checked: setState
                        indicator: Rectangle{
                            id: indicator
                            width: parent.width * 0.4
                            height: width
                            radius: width / 2
                            color: "transparent"
                            border.color: "lightgreen"

                            Rectangle{
                                visible: radioBtn.checked
                                width: parent.width * 0.8
                                height: width
                                radius: width / 2
                                color: "lightgreen"
                                anchors.centerIn: parent
                            }
                        }

                        contentItem: Text{
                            text: connectType
                            color: "white"
                            font{ pixelSize: 80 * factor }
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: indicator.width + radioBtn.spacing
                        }

                        onCheckedChanged: {
                            setState = checked
                            console.log( "change " + connectType + " to " + setState )
                        }
                    }
                }
            }

            ButtonGroup{
                buttons: rowConnectModelAutoChangeBtns.children
            }

            Text {
                text: "选择设备是否自动切换连接模式:"
                color: "white"
                font{ pixelSize: 60 * factor; bold: true }
                anchors{ bottom: rowConnectModelAutoChangeBtns.top; bottomMargin: 70 * factor; left: rowConnectModelAutoChangeBtns.left }
            }

            //选择设备自动切换连接模式
            Row{
                id: rowConnectModelAutoChangeBtns
                spacing: 400 * factor
                anchors{ top: parent.top; topMargin: 200 * factor; horizontalCenter: parent.horizontalCenter }
                Repeater{
                    model: connecteModelAutoChange
                    delegate: RadioButton{
                        id: radioBtn_autoChange
                        width:  250 * factor
                        height: width
                        checked: setState
                        indicator: Rectangle{
                            id: indicator_autoChange
                            width: parent.width * 0.4
                            height: width
                            radius: width / 2
                            color: "transparent"
                            border.color: "lightgreen"

                            Rectangle{
                                visible: radioBtn_autoChange.checked
                                width: parent.width * 0.8
                                height: width
                                radius: width / 2
                                color: "lightgreen"
                                anchors.centerIn: parent
                            }
                        }

                        contentItem: Text{
                            text: changeType
                            color: "white"
                            font{ pixelSize: 80 * factor }
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: indicator_autoChange.width + radioBtn_autoChange.spacing
                        }

                        onCheckedChanged: {
                            setState = checked
                            rowConnectModelBtns.visible = !connecteModelAutoChange.isAutoChangeConnect()
                        }
                    }
                }
            }

            Row{
                spacing: 50 * factor
                anchors{ horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 20 * factor }
                Repeater{
                    model: [ "确定", "取消" ]
                    delegate: Rectangle{
                        width:  250 * factor
                        height: 150 * factor
                        radius: 30  * factor
                        color: "#1F7ED3"
                        Text {
                            text: modelData
                            font{ pixelSize: 80 * factor }
                            anchors.centerIn: parent
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {

                                // 点击确定按钮, 则向后台设置设备连接信息
                                if( index === 0 ){
                                    InterAction.setDevConnectedInfo( connecteModel.isDirectConnectModel(),
                                                                    connecteModelAutoChange.isAutoChangeConnect() )
                                    close()
                                }else{
                                    settingsStackArea.pop( StackView.PopTransition )
                                }

                            }
                        }
                    }
                }
            }
        }
    }

    //立定跳远起跳位置设置界面
    Component{
        id: standingJumpStartPosSettingView
        Item{
            width: settingsStackArea.width
            height: settingsStackArea.height

            readonly property real minPos: 0
            readonly property real maxPos: 80

            ListModel{
                id: datas
                ListElement{ fieldName: 0 }
                ListElement{ fieldName: 80 }
            }

            Text {
                id: title1
                text: "选择起跳长度(cm):"
                font{ pixelSize: 40 * factor }
                anchors{ top: parent.top; topMargin: 50 * factor; left: parent.left; leftMargin: 50 * factor}
            }

            CusSelectCombobox{
                id: startPosCombobox
                width: 400 * factor
                height: 100 * factor
                selectContentBorderColor: "green"
                selectContentColor: "gray"
                selectContentHighLightColor: "#1F7ED3"
                model: datas
                anchors{ top: title1.bottom; topMargin: 50 * factor; left: title1.left }
            }

            Row{
                spacing: 50 * factor
                anchors{ horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 20 * factor }
                Repeater{
                    model: [ "确定", "取消" ]
                    delegate: Rectangle{
                        width:  250 * factor
                        height: 150 * factor
                        radius: 30  * factor
                        color: "#1F7ED3"
                        Text {
                            text: modelData
                            font{ pixelSize: 80 * factor }
                            anchors.centerIn: parent
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {

                                // 点击确定按钮, 则向后台设置起跳位置
                                if( index === 0 ){
                                    let targetStartPos = startPosCombobox.currentText
                                    let ok = InterAction.setStandingJumpStartPos( targetStartPos )
                                }

                                settingsStackArea.pop( StackView.ReplaceTransition )
                            }
                        }
                    }
                }
            }
        }
    }

    function clearSettingMenu(){
        settingsElementsModel.clear()
    }
}
