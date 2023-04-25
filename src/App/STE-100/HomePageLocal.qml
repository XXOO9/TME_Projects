/*************************************************
  <文件名称>    HomePageLocal
  <功能描述>    自检失败后的系统主界面

  <作    者>    tcy
  <时    间>    2021-10-11
**************************************************/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "."
Item {
    id : root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    signal signalTurnToPageVideoPlayer( var videoName )

    //背景"蓝紫色"
    Rectangle{
        id : backGround
        color: "#384DD5"
        z:0
        anchors.fill: parent
    }

    //顶部显示时间
    CustCurrentTimeDisplay{
        id: curTime
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //顶部信息栏
    TopSystemStateInfos{
        id : rectTipsInfo
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

//    //怡康标题栏
//    DeviceTitle{
//        id : rectDeviceName
//        anchors.top: parent.top
//        anchors.topMargin: 187 * factor
//        anchors.horizontalCenter: parent.horizontalCenter
//    }

    Text {
        id: topTips
        text: "STE-100 中小学体质测试系统"
        font.family: "造字工房悦圆（非商用）常规体"
        font.pixelSize: 124 * factor
        color: "white"
        anchors.top: parent.top
        anchors.topMargin: 217 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //中心区域紫色矩形框
    Grid {
        id : centerGird
        width: 1582 * factor
        height: 2340 * factor
        rowSpacing: 36 * factor
        columnSpacing: 56 * factor
        columns: 2
        anchors.top: topTips.bottom
        anchors.topMargin: 84 * factor
        anchors.horizontalCenter: parent.horizontalCenter

        CustVideoPlayItem {
            id: ywqzItem
            itemWidth: 763
            itemHeight: 543
            itemColor: "#1EA08E"
            title: qsTr("仰卧起坐")
            imgPath: "qrc:/pic/HomePage/sit-up.png"
            imgWidth: 495
            imgHeight: 356

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "6", "仰卧起坐指导视频" )
                }
            }
        }

        CustVideoPlayItem {
            id: ldtyItem
            itemWidth: 763
            itemHeight: 543
            itemColor: "#E9825B"
            title: qsTr("立定跳远")
            imgPath: "qrc:/pic/HomePage/jump.png"
            imgWidth: 544
            imgHeight: 378
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "3", "立定跳远指导视频" )
                }
            }
        }

        CustVideoPlayItem {
            id: zltqqItem
            itemWidth: 763
            itemHeight: 543
            itemColor: "#907FFF"
            title: qsTr("坐位体前屈")
            imgPath: "qrc:/pic/HomePage/sitAndReach.png"
            imgWidth: 439
            imgHeight: 318
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "4", "坐位体前屈指导视频" )
                }
            }
        }

        CustVideoPlayItem {
            id: dpItem
            itemWidth: 763
            itemHeight: 543
            itemColor: "#E471AE"
            title: qsTr("50米跑")
            imgPath: "qrc:/pic/HomePage/50meter.png"
            imgWidth: 423
            imgHeight: 532
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "TEST_ITEM_RUN_50", "50米跑指导视频" )
                }
            }
        }

        CustVideoPlayItem {
            id: cpItem
            itemWidth: 763
            itemHeight: 543
            itemColor: "#EC8186"
            title: qsTr("800米跑")
            imgPath: "qrc:/pic/HomePage/800meter.png"
            imgWidth: 354
            imgHeight: 546
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "TEST_ITEM_RUN_800", "800米跑指导视频" )
                }
            }
        }

        CustVideoPlayItem {
            id: ytxsItem
            itemWidth: 763
            itemHeight: 543
            itemColor: "#3F8BE5"
            title: qsTr("引体向上")
            imgPath: "qrc:/pic/HomePage/pull-up.png"
            imgWidth: 633 / 2
            imgHeight: 1023 / 2
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "5", "引体向上指导视频" )
                }
            }
        }

        CustVideoPlayItem {
            id: thousandCPItem
            itemColor: "#4AACC5"
            title: qsTr("1000米跑")
            imgPath: "qrc:/pic/HomePage/1000meter.png"
            itemWidth: 763
            itemHeight: 543
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "TEST_ITEM_RUN_1000", "1000米跑指导视频" )
                }
            }
        }

        CustVideoPlayItem {
            id: fhlItem
            itemWidth: 763
            itemHeight: 543
            itemColor: "#9E6CD7"
            title: qsTr("肺活量")
            imgPath: "qrc:/pic/HomePage/VC.png"
            imgWidth: 266
            imgHeight: 502
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    playVideo( "2", "肺活量指导视频" )
                }
            }
        }
    }

    Loader{
        id: videoPlayerLoader
        visible: false
        z:50
        anchors.fill: root
    }

    Connections{
        target: videoPlayerLoader.item
        onSignalCloseBtnClicked:{
            videoPlayerLoader.source = ""
            videoPlayerLoader.visible = false
        }
    }

    function playVideo( videoType, tipsWords ){
        var path = InterAction.getVideoAbsolutePath( videoType );
        console.log( "qml video path = " + path )
        videoPlayerLoader.visible = true
        videoPlayerLoader.setSource( "qrc:/CustVideoDisplayPage.qml", { "videoPath": path,
                                        "bShowrecommendVideo": false,
                                        "strTips": tipsWords } )
    }
}
