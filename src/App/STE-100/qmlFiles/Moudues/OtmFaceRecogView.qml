import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "../JavaScript/ProjectValue.js" as Assistant
import "../"
Item {
    id: root
    width: 830 * factor
    height: width

    property real offset: 0.0
    property alias scanfLineStartPos: scanfLineAnimation.from

    signal sigRecogSuccess()

    Component.onCompleted: {
        onViewComplete()
    }

    Rectangle{
        anchors.fill: parent
        radius: width / 2
        color: "#073B9F"
        border.color: "#1ADDFC"


        Image {
            id: imgFaceRecogView
            anchors.fill: parent
            source: ""
            cache: false
            smooth: true
            visible: false
            Image {
                id: imgScanfLine
                clip: true
                smooth: true
                width: imgFaceRecogView.width
                source: "qrc:/pic/scanfLine.png"
//                rotation: 270
//                y: faceRecogScanfLineYPosProvider()
                NumberAnimation on y{
                    id: scanfLineAnimation
                    from: GlobalProperty.scanfLineStartPos * factor
                    to: imgFaceRecogView.x + imgFaceRecogView.width + ( parent.width/2.5 * factor )
                    loops: Animation.Infinite
                    duration: 3000

                    onFromChanged: scanfLineAnimation.restart()
                }
            }
        }

        Image {
            id: imgFaceView
            source: "qrc:/pic/7-faceRecog.png"
            anchors.fill: parent
            z: 10
        }

        Rectangle{
            id: mask
            color: "#6A76F5"
            width: parent.width
            height: parent.height
            radius: width/2
            visible: false
        }

        OpacityMask {
            id: imgFaceRecogViewMask
            anchors.fill: imgFaceRecogView
            source: imgFaceRecogView
            maskSource: mask
            z:1
        }
    }

    //识别结果提示图
    Image {
        id: imgRecogRet
        visible: false
        width: 80 * factor
        height: width
        source: "qrc:/pic/success.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 150 * factor
    }

    Text {
        id: recogRetText
        visible: false
        text: InterAction.curRecogUserName + "识别成功"
        width: 253 * factor
        height: 58 * factor
        color: "white"
        font{ pixelSize: 62 * factor; family: "Microsoft YaHei"; bold: true }
        anchors.horizontalCenter: imgRecogRet.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: imgRecogRet.bottom
        anchors.topMargin: 30 * factor
    }

    Timer{
        id: recogRetTimer
        interval: 2000
        repeat: false
        onTriggered: {
            imgRecogRet.visible = false
            recogRetText.visible = false
        }
    }

    Connections{
        target: InterAction
        onSignalNewQimageAvailable:{
//            imgFaceRecogViewMask.rotation = 90
            imgFaceRecogView.source = ""
            imgFaceRecogView.source = "image://imageProvider"
        }

        //接收 提前告知界面的识别结果
        onSignalAdvanceReportToViewRecogRet: onShowAdvanceRecogRet( ret )
    }

    function faceRecogScanfLineYPosProvider(){
        let ret = 0.0

        if( GlobalProperty.bDirectMode ){
            ret = imgFaceRecogView.y + ( parent.height * 0.5 * factor ) - GlobalProperty.scanfHorizontalPos * factor
        }else{
            if( GlobalProperty.testType === GlobalProperty.testSyncMode ){
                ret = imgFaceRecogView.y + ( parent.height * 0.5 * factor ) - GlobalProperty.scanfHorizontalPos * factor
            }else{
                ret = imgFaceRecogView.y + ( parent.height * 0.5 * factor ) - GlobalProperty.scanfHorizontalPos * factor
            }
        }

        return ret
    }

    function onViewComplete(){
        if( GlobalProperty.bDirectMode ){
            GlobalProperty.scanfHorizontalPos = 475
            GlobalProperty.scanfLineStartPos = -400
        }else{
            GlobalProperty.scanfHorizontalPos = 1120
            GlobalProperty.scanfLineStartPos = -180
        }
    }

    function displayScanfStartPos(){
        scanfLineAnimation.restart()
        console.log( "start pos = " + scanfLineAnimation.from )
    }

    function onShowAdvanceRecogRet( ret ){
        recogRetTimer.stop()
        imgRecogRet.visible = true
        recogRetText.visible = true
        if( 0 === ret ){
            imgRecogRet.source = ""
            imgRecogRet.source = "qrc:/pic/success.png"
            recogRetText.text = InterAction.curRecogUserName + "识别成功"
            sigRecogSuccess()
        }
        else if(ret === 10)
        {
            imgRecogRet.source = ""
            imgRecogRet.source = "qrc:/pic/failed.png"
            recogRetText.text = "识别失败"
        }
        //用户已绑定
        else if( 2 === ret ){
            imgRecogRet.source = ""
            recogRetText.text = ""
            Assistant.rootErrorPopup.textErrorInfo = "用户已绑定"
            openRootPopup("userConfirmFailed")
        }
        else if( 4 === ret ){
            imgRecogRet.source = ""
            recogRetText.text = ""
            Assistant.rootErrorPopup.textErrorInfo = "绑定已达上限"
            openRootPopup("userConfirmFailed")
        }
        else{
            imgRecogRet.source = ""
            recogRetText.text = ""
            Assistant.rootErrorPopup.textErrorInfo = "未找到该人员"
            openRootPopup("userConfirmFailed")
        }
        recogRetTimer.start()
    }
}
