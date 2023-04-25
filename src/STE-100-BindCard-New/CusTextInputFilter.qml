import QtQuick 2.0
import QtQuick.Controls 2.12
Item {
    id: root

    implicitWidth: 236
    implicitHeight: 500

    //对外信号
    signal sigSelected( var selectedContent )

    //只读属性
    readonly property int maxCandidateCount: 5
    readonly property real perRowHeight: 30

    //输入属性
    property string bkgColor: "black"
    property string bkgBorderColor: "#1690A2"
    property alias tips: textArea.placeholderText
    property alias tipsColor: textArea.placeholderTextColor
    property alias text: textArea.text

    //候选属性
    property var candidateObj
    property alias candidateModel: candidateView.model
    property string fontFamily: "Microsoft YaHei"

    TextField{
        id: textArea

        color: "white"
        selectedTextColor: "skyblue"
        implicitWidth: parent.width
        implicitHeight: root.height
        selectByMouse: true
        placeholderTextColor: "#85898B"
        font{ family: fontFamily; pixelSize: 16 * factorX }
        anchors{ top: parent.top; horizontalCenter: parent.horizontalCenter }

        background: Rectangle{
            anchors.fill: parent
            color: bkgColor
            border.color: bkgBorderColor
        }

        onTextEdited: onTextEditContentChanged()
        Timer{
            id: timer
            interval: 1500
            running: false
            repeat: false

            onTriggered: startSearch()
        }
    }

    Rectangle{
        id: listViewBkg
        width: root.width
        visible: false
        height: candidateView.count >= maxCandidateCount ? maxCandidateCount * perRowHeight : candidateView.count * perRowHeight
        anchors{ top: textArea.bottom; horizontalCenter: parent.horizontalCenter }

        border.color: "#097486"
        border.width: 1 * factorX
        color: bkgColor

        ListView{
            id: candidateView
            clip: true
            width: parent.width - parent.border.width * 2
            height: parent.height - parent.border.width * 2
            anchors.centerIn: parent
            highlightMoveDuration: 50
            delegate: candidateContent
        }
    }

    Component{
        id: candidateContent
        Rectangle{
            width: candidateView.width
            height: perRowHeight
            color: bkgColor
            Text {
                text: userName
                color: "white"
                font{ family: fontFamily; pixelSize: 16 }
                anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 5 * factorX }
            }

            Text {
                text: userUniqueNum
                color: "white"
                font{ family: fontFamily; pixelSize: 16 }
                anchors{ verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: 5 * factorX }
            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.color = "#097486"
                onExited: parent.color = bkgColor
                onClicked: {
                    console.log( "ret = " + userName + " index = " + index )
                    listViewBkg.visible = false
                    sigSelected( userName + '+' + userUniqueNum )
                }
            }
        }
    }

    function onTextEditContentChanged(){
        if( textArea.text.length >= 1 ){
            timer.restart()
        }else{
            timer.stop()
            listViewBkg.visible = false
        }
    }

    function startSearch( manualOpeartion = false ){
        listViewBkg.visible = true
        console.log( "filte to user info, " + textArea.text )
        FilterModel.setFilterWildcard( textArea.text + "*" )

        if( FilterModel.rowCount() <= 0 ){
            console.log( '7777777777777777777777777777' )
        }
    }
}


