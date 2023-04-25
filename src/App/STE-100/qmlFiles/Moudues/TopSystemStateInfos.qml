import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../"
import "."
Item {
    id : root
    width: 1920 * factor
    height: 100 * factor

    signal signalClicked()

    property string strdebug: "this is debug info"

    Connections {
        target: InterAction
        //响应设备连接信号
        onUpdateNetConnectStatus: {
            if(status)
            {
                strNetState.color = "#FFFFFF"
            }
            else
            {
                strNetState.color = "#F39800"
            }
        }
    }

    Button{
        id: debugBtn1
        width: parent.height
        height: width
        visible: InterAction.isDeveloper
        anchors.left: parent.left

        z:10
        onClicked: {
            InterAction.testFunc3()
        }
    }

    Button{
        id: debugBtn2
        width: 200 * factor
        height: 100 * factor
        visible: InterAction.isDeveloper
        text: "打开"
        anchors.left: debugBtn1.right
        anchors.leftMargin: 10 * factor
        anchors.verticalCenter: debugBtn1.verticalCenter
        z:10
        onClicked: {
            InterAction.startSignature( "1" )
        }
    }

    Button{
        id: debugBtn3
        width: 200 * factor
        height: 100 * factor
        visible: InterAction.isDeveloper
        text: "tool"
        anchors.left: debugBtn2.right
        anchors.leftMargin: 10 * factor
        anchors.verticalCenter: debugBtn1.verticalCenter
        z: 10
        onClicked: openRootPopup( "adjustSign" )
    }

    Button{
        id: debugBtn4
        width: 200 * factor
        height: 100 * factor
        visible: InterAction.isDeveloper
        text: "change"
        anchors.left: debugBtn3.right
        anchors.leftMargin: 10 * factor
        anchors.verticalCenter: debugBtn1.verticalCenter
        z: 10
        onClicked: changeSignPainterOtoSize()
    }

    Rectangle{
        id : rectTipsInfo
        width: parent.width
        height: parent.height

        //背景色
        color: "#4756EB"
        anchors.fill: parent
    }

    //连接图标
    Image {
        id: imgConnectIcon
        width: 84 * factor
        height: 51 * factor
        anchors.left: parent.left
        anchors.leftMargin: 810 * factor
        anchors.verticalCenter: parent.verticalCenter
        source: GlobalProperty.bDevConnect ? "qrc:/pic/deviceConnect.png" : "qrc:/pic/connectError.png"
        z: 1
        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                root.signalClicked()
            }
        }
    }
    // 文字"设备连接"
    Text {
        id: strDeviceConnect
        text: qsTr("设备连接")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 50 * factor
        font.family: "Microsoft YaHei"
        color: GlobalProperty.bDevConnect ? "#FFFFFF" : "#F39800"
        anchors.left: imgConnectIcon.right
        anchors.leftMargin: 23 * factor
        anchors.verticalCenter: parent.verticalCenter
    }
    //wifi图标
    Image {
        id: imgNetworkStateIcon
        width: 56 * factor
        height: 56 * factor
        source: InterAction.netConnectStatus ?  "qrc:/pic/netNormal.png" : "qrc:/pic/netError.png"
        anchors.left: parent.left
        anchors.leftMargin: 177 * factor
        anchors.verticalCenter: parent.verticalCenter
    }

    //文字 "网络状态"
    Text {
        id: strNetState
        text: InterAction.netConnectStatus ? "服务器已连接" : "服务器连接异常"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 50 * factor
        font.family: "Microsoft YaHei"
        color: InterAction.netConnectStatus ? "#FFFFFF" : "#F39800"
        anchors.left: imgNetworkStateIcon.right
        anchors.leftMargin: 21 * factor
        anchors.verticalCenter: imgNetworkStateIcon.verticalCenter
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
        visible: !plugInImage.visible
        width: 100 * factor
        height: 38 * factor
        anchors.right: parent.right
        anchors.rightMargin: 175 * factor
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 24 * factor
        source: "qrc:/pic/electricQuantity.png"

        //电量剩余白色条 (100%电量宽度为75px)
        Rectangle{
            id: electricQuantityRect
            property real singleStep: 0.78 * factor
            visible: electricQuantityImage.visible
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

    function getCurrentTime(){
        return Qt.formatDateTime( new Date(), "hh:mm:ss" )
    }

    property int rret: 0
    MouseArea{
        anchors.fill: parent
        onClicked: {
            InterAction.leftVision = 1.2
            InterAction.rightVision = 1.6
        }
    }

    Connections{
        target: InterAction

        //设置电源类型
        onSignalSetPowerType:{
            InterAction.qmlLogout( "1----topSystemStateInfo------------- ret = " + GlobalProperty.bBatteryExisted )
            GlobalProperty.bBatteryExisted = bIsBatteryPowerd
            InterAction.qmlLogout( "2----topSystemStateInfo------------- ret = " + GlobalProperty.bBatteryExisted )
            if( !GlobalProperty.bBatteryExisted ){
                electricQuantityText.visible = false
            }else{
                electricQuantityImage.visible = true
            }
        }

        //设置电池充电状态
        //2022/7/21 因为充电状态电量值不准，故只有两种情况，连接电源-显示图标 未连接电源-显示数值
        onSignalSetDisplayChargeState:{
            /*
            if( !GlobalProperty.bBatteryExisted ){
                //如果电池不存在，直接退出
                return
            }
            GlobalProperty.bCharging = bCharged
            */
            GlobalProperty.bBatteryExisted = bCharged
            if( GlobalProperty.bBatteryExisted )
            {
                plugInImage.visible = true
                electricQuantityImage.visible = false
                electricQuantityText.visible = false
                InterAction.qmlLogout( "显示插头" )
            }
            else
            {
                plugInImage.visible = false
                electricQuantityImage.visible = true
                electricQuantityText.visible = true
                InterAction.qmlLogout( "显示电池" )
            }
        }

        //电池电量变化
        onSignalBatteryQuantityChanged:{
            if( !GlobalProperty.bBatteryExisted ){
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


}
