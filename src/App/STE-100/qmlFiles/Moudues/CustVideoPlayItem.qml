/*************************************************
  <文件名称>    CustVideoPlayItem
  <功能描述>    自定义视频播放子组件

  <作    者>    tcy
  <时    间>    2021-10-11
**************************************************/
import QtQuick 2.0

Item {
    property int itemWidth: 763
    property int itemHeight: 563
    property string itemColor: "#1EA08E"
    property string title: "仰卧起坐"
    property string imgPath: ""
    property int imgWidth: itemWidth * 0.45
    property int imgHeight: itemHeight * 0.45
    property bool bPlay: true
    signal signalVideoBtnClicked(  var btnName )
    width: itemWidth * factor
    height: itemHeight * factor

    //标题
    Text {
        id: titleText
        text: title
        font.pixelSize: 62 * factor
        font.family: "MF DianHei (Noncommercial)"
        color: "white"
        anchors.top: parent.top
        anchors.topMargin: 50 * factor
        anchors.left: parent.left
        anchors.leftMargin: 60 * factor
    }

    Image {
        id: sportImg
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: parent.width/7
        anchors.verticalCenter: parent.verticalCenter
        fillMode: Image.PreserveAspectFit
        width: imgWidth * factor
        height: imgHeight * factor
        source: imgPath
    }

    Image {
        id: palyImage
        width: 320 * factor
        height: 320 * factor
        anchors.centerIn: parent
        z: 1
        source: "qrc:/pic/HomePage/play.png"
        visible: bPlay
    }

    Rectangle {
        width: itemWidth * factor
        height: (itemHeight - 20) * factor
        radius: 30 * factor
        color: itemColor
        anchors.centerIn: parent
        z: -1
    }
}
