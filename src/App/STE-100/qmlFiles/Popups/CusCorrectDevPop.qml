/*
@FileName: CusCorrectDevPop.qml
@Create Date:  2022-07-28  12:01:33
@Author: xutianci
@Describe: 体测选择需要校准设备的界面，一对一和一对多通用
@Attentions:
*/
import QtQuick 2.0
import QtQuick.Controls 2.12
import "../JavaScript/ProjectValue.js" as Common
import "../"


Item {
    id: name
    width: 1582 * factor
    height: 1400 * factor

    ListModel{
        id: connectedDevList
    }

    Popup{
        id: m_pop
        width: parent.width
        height: parent.height
        modal: true
        padding: 0
        clip: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: parent

        background: Rectangle{
            anchors.fill: parent
            radius: 40 * factor
            color: "#409EFF"
        }

        Image {
            width: 60 * factor
            height: width
            fillMode: Image.PreserveAspectFit
            source: Common.imgClose
            anchors{ top: parent.top; topMargin: 10 * factor;  right: parent.right; rightMargin: 10 * factor }
            z:5
            MouseArea{
                anchors.fill: parent
                onClicked: closePop()
            }
        }

        Loader{
            id: contentLoader
            anchors.fill: parent
        }

        onOpened: onPopOpened()
    }

    Component{
        id: devSelectView
        Rectangle{
            width: 800 * factor
            height: 1000 * factor
            radius: 40 * factor
            color: "#409EFF"

            Text {
                color: "white"
                text: "设备列表:"
                width: parent.width
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor }
                anchors.top: parent.top
                anchors.topMargin: 50 * factor
                anchors.left: parent.left
                anchors.leftMargin: 50 * factor
            }

            ListView{
                id: m_list
                width: parent.width * 0.5
                height: parent.height * 0.85
                anchors.top: parent.top
                anchors.topMargin: 200 * factor
                anchors.horizontalCenter: parent.horizontalCenter
                model: connectedDevList
                clip: true
                snapMode: ListView.SnapOneItem
                delegate: Rectangle{
                    width: parent.width
                    height: 100 * factor
                    color: "#2B41D0"
                    border.color: "white"
                    Text {
                        id: devNumberText
                        text: devNumber
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 40 * factor }
                        anchors{ left: parent.left; leftMargin: 50 * factor; verticalCenter: parent.verticalCenter }
                    }

                    Text {
                        id: devOrderText
                        text: devOrder
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 40 * factor }
                        anchors{ right: parent.right; rightMargin: 50 * factor; verticalCenter: parent.verticalCenter }
                    }
                    MouseArea{
                        anchors.fill: parent
                        onDoubleClicked: selectSpecificDev( index, devNumberText.text )
                    }
                }
            }

            Text {
                color: "#BC6534"
                text: "双击选择需要校准的设备"
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font{ family: "Microsoft YaHei"; pixelSize: 80 * factor }
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }


    }

    Connections{
        target: InterAction
        onSigEnableCorrectViewBtn: {
            contentLoader.item.setEnableBtn( true )
        }
    }

    function openCorrectDevPop(){
        m_pop.open()
    }

    function closePop(){
        m_pop.close()
    }

    function onPopOpened(){
        contentLoader.sourceComponent = devSelectView
        loadDevModel()
    }

    function selectSpecificDev( tmpIndex, targetDevNumber ){
        if( 0 === tmpIndex ){
            return
        }

        let url
        let index = GlobalProperty.iDevType
        switch( index ){
        case GlobalProperty.devBody:
            url = "qrc:/qmlFiles/CorrectView/CorrectDevBody.qml"
            break
        case GlobalProperty.devGrip:
            url = "qrc:/qmlFiles/CorrectView/CorrectDevGrip.qml"
            break
        case GlobalProperty.devSFB:
            url = "qrc:/qmlFiles/CorrectView/CorrectDevSFB.qml"
            break
        default:
            url = ""
            break
        }
        let tmp = parseInt( targetDevNumber, 10 )
        console.log( tmp, "type = " + typeof( tmp ) )
        Common.curCorrectDevNumber = tmp
        contentLoader.setSource( url )
        m_pop.open()
    }

    function loadDevModel(){
        let devList = InterAction.getConnectedDevNumbers()
        connectedDevList.clear()
        connectedDevList.append( { "devNumber": "设备号", "devOrder": "设备编号" } )
        connectedDevList.append( { "devNumber": "test1", "devOrder": "test1" } )
        for( let ele of devList ){
            connectedDevList.append( { "devNumber": ele.devNumber.toString(), "devOrder": ele.devOrder.toString() } )
        }
    }
}
