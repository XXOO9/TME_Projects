import QtQuick 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0
import "."

Item {
    id:root
    implicitWidth: 1920 * factor
    implicitHeight: 110 * factor

    property string schoolNameText: "null"
    property string currentTime: "default"
    property string currentDateTime   : "1993-01-10"
    property string week       : "星期八"
    property string fontYaHei: "Microsoft YaHei"


    signal sigTitleClicked( var index )

    MouseArea{
        anchors.fill: parent
        onDoubleClicked: {
            Qt.quit()
        }
    }


    Rectangle{
        anchors.fill: parent
        color: "#243452"
    }

    Image {
        id: logo
        width: 57 * factor
        height: 64  * factor
        source: "qrc:/Pic/Logo.png"
        anchors.left: parent.left
        anchors.leftMargin: 30 * factor
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: schoolName
        text: schoolNameText
        color: "white"
        font{ pixelSize: 38 * factor; family: "MF DianHei (Noncommercial)"}
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: logo.right
        anchors.leftMargin: 13 * factor
    }

    TopTitleBtn{
        id:userBtn
        imgPath: ["qrc:/Pic/user1.png", "qrc:/Pic/user2.png"]
        displayText: "个人匹配"
        isSelected: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 679 * factor
    }

    TopTitleBtn{
        id:classBtn
        imgPath:["qrc:/Pic/class1.png", "qrc:/Pic/class2.png"]
        displayText: "班级匹配"
        isSelected: false
        anchors.verticalCenter: userBtn.verticalCenter
        anchors.left: userBtn.right
    }

    Text {
        id: dateTime
        color: "white"
        text: currentTime
        font{ pixelSize: 30 * factor; family: fontYaHei; bold: true }
        anchors.top: parent.top
        anchors.topMargin: 56 * factor
        anchors.right: parent.right
        anchors.rightMargin: 30 * factor
    }

    Text {
        color: "white"
        text: currentDateTime + ' ' + week
        font{ pixelSize: 16 * factor; family: fontYaHei }
        anchors.top: parent.top
        anchors.topMargin: 27 * factor
        anchors.right: parent.right
        anchors.rightMargin: 30 * factor
    }

    Timer{
        id: updateDateTimer
        interval: 1000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            var curTime = Qt.formatDateTime( new Date(), "dddd+yyyy-MM-dd+hh:mm:ss")
            var tmp = curTime.split( "+" )
            currentTime = tmp[2]
            currentDateTime = tmp[1]
            week = tmp[0]
        }
    }

    Connections{
        target: userBtn
        onSigClicked:{
            resetBtnState()
            userBtn.isSelected = true
            sigTitleClicked( Enums.Page_User_Bind )
        }
    }

    Connections{
        target: classBtn
        onSigClicked:{
            resetBtnState()
            classBtn.isSelected = true
            sigTitleClicked( Enums.Page_Class_Bind )
        }
    }

    function resetBtnState(){
        userBtn.isSelected = false
        classBtn.isSelected = false
    }
}
