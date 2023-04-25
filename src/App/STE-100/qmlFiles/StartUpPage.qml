import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import ProjectEnums 1.0
import "./"
import  "./Moudues"
import "."

Item {
    id : selfchek
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    property int selfCheckTimerCount: 0

    //自检完成-
    signal selfCheckFinished( bool success )

    //设置自检标志
    signal setSelfCheckFlag();

    //自检失败弹窗弹窗
    Loader{
        id : tipPopup
        z: 20
        anchors.centerIn: parent
    }

    //背景 “蓝紫色”
    Rectangle{
        id : backGround
        color: GlobalProperty.startPageBgColor
        z:0
        anchors.fill: parent
    }

    //“怡康标志”
    Image {
        id: imgCompanyLogo
        width: 367 * factor
        height: 141 * factor
        anchors.left: parent.left
        anchors.leftMargin: 112 * factor
        anchors.top: parent.top
        anchors.topMargin: 111 * factor
        source: "qrc:/pic/1-LOGO01.png"
    }

    //"正中心深蓝紫色矩形及其里面显示的内容"
    Rectangle{
        id : rectProductLogo
        width: 1400 * factor
        height: 760 * factor
        color: GlobalProperty.startPageTitleBgColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 672 * factor

        //STE-100的Logo
        Image {
            id: imgSte100Logo
            width: 278 * factor
            height: 203 * factor
            anchors.top: parent.top
            anchors.topMargin: 137 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/pic/2-logo02.png"
        }

        //文字 "STE-100" + "体质测试系统"
        Text {
            id: strTipsUp
            text: "STE-100" + "\n" + "体质测试系统"
            color: "#F0F0F0"
            font.pixelSize: 114 * factor
            font.family: "MF YueYuan (Noncommercial)"
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1.2
            bottomPadding: 20 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: imgSte100Logo.bottom
            anchors.topMargin: 54 * factor
        }
    }

    //进度条
    CustPrograssBar{
        id : progressBar
        anchors.top: rectProductLogo.bottom
        anchors.topMargin: 282 * factor
        anchors.horizontalCenter: rectProductLogo.horizontalCenter
    }

    //启动状态
    Text {
        id: strStatrupState
        text: qsTr( "启动中..." )
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        color: "#CBCFEF"
        anchors.left: progressBar.left
        anchors.top: progressBar.bottom
        anchors.topMargin: 52 * factor
    }

    //显示加载进度百分比
    Text {
        id: strLoadedPersent
        text: ( progressBar.loadedPersent * 100 ).toFixed( 0 ) + "%"
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        color: "#CBCFEF"
        anchors.right: progressBar.right
        anchors.top: progressBar.bottom
        anchors.topMargin: 52 * factor
        onTextChanged: {
            if( progressBar.loadedPersent >= 1 ){
                text = '100%'
            }
        }
    }

    //显示版本号
    Text {
        id: strSoftwareVersion
        text: GlobalProperty.strSoftwareVersion
        font.pixelSize: 62 * factor
        font.family: "Microsoft YaHei"
        color: "#CBCFEF"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 135 * factor
    }
    //版本号左边的装饰物
    Image {
        id: imgVersionDecorationLeft
        width: 235 * factor
        height: 40 * factor
        source: "qrc:/pic/3-1versionpic.png"
        anchors.verticalCenter: strSoftwareVersion.verticalCenter
        anchors.right: strSoftwareVersion.left
        anchors.rightMargin: 24 * factor
    }

    //版本号右边的装饰物
    Image {
        id: imgVersionDecorationRight
        width: 235 * factor
        height: 40 * factor
        source: "qrc:/pic/3-2versionpic.png"
        anchors.verticalCenter: strSoftwareVersion.verticalCenter
        anchors.left: strSoftwareVersion.right
        anchors.leftMargin: 24 * factor
    }

    Connections{
        target: progressBar
        //进度条加载完成
        onSigLoadFinished:{
            selfCheckFinished( true )
        }

        onSigLoadError:{
            selfCheckFinished( false )
        }
    }

//    MouseArea{
//        anchors.fill: parent
//        onClicked: {
//            selfCheckFinished( true )
//        }
//    }

    Connections{
        target: InterAction
        //响应自检结果
        onSignalReportSelfCheckRet:{
            setSelfCheckFlag()
            if( selfCheckRet )
            {
                //自检成功 设置设备类型
                GlobalProperty.setTestDeviceType();
                selfCheckFinished( true )
            }
            else
            {
                //自检失败，加载异常弹窗
                showCheckErrorPopup()
            }
        }

        onUpdateProcessVal: {
            progressBar.autoAddLength()
        }
    }

    //显示自检失败弹窗
    function showCheckErrorPopup()
    {
        tipPopup.source = ""
        tipPopup.setSource("qrc:/qmlFiles/Popups/CheckErrorPopup.qml");
    }


}
