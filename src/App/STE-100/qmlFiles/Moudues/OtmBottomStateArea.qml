import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"

Item {
    id: root
    width: 1920 * factor
    height: 80 * factor
    property string onlinePath: "qrc:/pic/onLine.png"
    property string offLinePath: "qrc:/pic/offLine.png"

    signal signalClicked()

    state: InterAction.netConnectStatus ? "serverOnline" : "serverOffline"

    Rectangle{
        anchors.fill: parent
        color: "#2F44CF"
    }

    Image {
        id: serverOnlineState
        width: 50 * factor
        height: width
        anchors.left: parent.left
        anchors.leftMargin: 83 * factor
        anchors.verticalCenter: parent.verticalCenter
    }

    //服务器连接状态文字
    Text {
        id: serverOnlineStateText
        color: "white"
        font{ family: "苹方 中等"; pixelSize: 44 * factor }
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: serverOnlineState.right
        anchors.leftMargin: 20 * factor
    }

    //设备连接状态图标
    Image {
        id: devConnectState
        width: 71 * factor
        height: 42 * factor
        source: "qrc:/pic/devConnect.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 810 * factor
        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                root.signalClicked()
            }
        }
    }

    Text {
        id: devConnectedCount
        text: "设备连接 X" + InterAction.multiDevCount
        color: "white"
        font{ family: "苹方 中等"; pixelSize: 44 * factor }
        anchors.left: devConnectState.right
        anchors.leftMargin: 23 * factor
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    //插头图标(无电池时显示)，有电池时，bBatteryExisted为true,则不显示该控件
    Image {
        id: plugInImage
        visible: !GlobalProperty.bBatteryExisted
        width: 40 * factor
        height: 50 * factor
        anchors.right: parent.right
        anchors.rightMargin: 170 * factor
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 18 * factor
        source: "qrc:/pic/plugIn.png"
    }

    //电量显示
    Image {
        id: electricQuantityImage
        visible: GlobalProperty.bBatteryExisted
        width: 100 * factor
        height: 38 * factor
        anchors.right: parent.right
        anchors.rightMargin: 83 * factor
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/pic/electricQuantity.png"

        //电量剩余白色条 (100%电量宽度为75px)
        Rectangle{
            id:electricQuantityRect
            property real singleStep: 0.78 * factor
            width: GlobalProperty.curDevElecQuantity * singleStep
            height: 26 * factor
            color: GlobalProperty.bCharging ? "lightgreen" : "#FFFFFF"
            z:10
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 8 * factor
        }
    }

    //电量值显示
    Text {
        id: electricQuantityText
        visible: !GlobalProperty.bCharging && GlobalProperty.bBatteryExisted
        anchors.right: electricQuantityImage.left
        anchors.rightMargin: 17 * factor
        anchors.verticalCenter: electricQuantityImage.verticalCenter
        font.pixelSize: 48 * factor
        font.family: "PingFang SC"
        color: "white"
        text: GlobalProperty.curDevElecQuantity +  qsTr("%")
    }

    states: [
        State {
            name: "serverOnline"
            PropertyChanges {
                target: serverOnlineState
                source: onlinePath
            }
            PropertyChanges {
                target: serverOnlineStateText
                text: "服务器已连接"
                color: "white"
            }
        },
        State{
            name: "serverOffline"
            PropertyChanges {
                target: serverOnlineState
                source: offLinePath
            }
            PropertyChanges {
                target: serverOnlineStateText
                text: "服务器连接异常"
                color: "#999999"
            }
        }
    ]

    Connections{
        target: InterAction

        //设置电源类型
        onSignalSetPowerType:{
            GlobalProperty.bBatteryExisted = bIsBatteryPowerd
            if( GlobalProperty.bBatteryExisted ){
                electricQuantityText.visible = false
            }else{
                electricQuantityImage.visible = true
            }
        }

        //设置电池充电状态
        onSignalSetDisplayChargeState:{
            if( GlobalProperty.bBatteryExisted ){
                //如果电池不存在，直接退出
                return
            }

            GlobalProperty.bCharging = bCharged
        }

        //电池电量变化
        onSignalBatteryQuantityChanged:{
            if( GlobalProperty.bBatteryExisted ){
                //如果电池不存在，直接退出
                return
            }

            if( batteryQunatity >= 100 ){
                batteryQunatity = 100
            }

            if( batteryQunatity <= 0 ){
                batteryQunatity = 0
            }

            GlobalProperty.curDevElecQuantity = batteryQunatity
        }
    }

    function setServerState( isOnline ){
        if( !isOnline ){
            root.state = "serverOffline"
        }else{
            root.state = "serverOnline"
        }
    }

    Button{
        id: btn1
        text: "1"
        visible: InterAction.isDeveloper
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: devConnectState.left
        onClicked: {
            InterAction.testFunc1()
        }
    }

    Button{
        id: btn2
        text: "多人识别"
        visible: InterAction.isDeveloper
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: btn1.left
        onClicked: {
            InterAction.otmTestUserRecog()
        }
    }

    Button{
        id: btn3
        text: "调试工具"
        visible: InterAction.isDeveloper
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: btn2.right
        onClicked: {
            openDebugTool()
        }
    }

    //显示一对多提交界面的签字版(和一对一的签字版是同一个签字版，只是更改了尺寸和坐标)
    function displayOtmSignPainter(){
//        20220715 屏蔽
//        var wid = ( 1650 * factor ).toFixed(0).toString()
//        var hei = ( 480 * factor ).toFixed(0).toString()
//        //签字版的坐标为( 40, 590 )
//        InterAction.changedSignPainterPos( wid, hei, 40, 590 )
    }
}
