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
import "./Moudues/"
import "./Popups/"
import "."

Item {
    id : root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    signal signalTurnToPageVideoPlayer( var videoName )

    //系统设置弹窗
    AuthenticationPopup {
        id: adminAuthtiCationPopup
        anchors.centerIn: parent
    }

    Text {
        id: topTips
        text: "STE-100 体质测试系统"
        font.family: "造字工房悦圆（非商用）常规体"
        font.pixelSize: 124 * factor
        color: "white"
        anchors.top: parent.top
        anchors.topMargin: 217 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ScrollView {
        width: 1582 * factor
        height: 2320 * factor
        anchors.top: topTips.bottom
        anchors.topMargin: 84 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true

        //中心区域
        Grid {
            id : centerGird
            rowSpacing: 36 * factor
            columnSpacing: 56 * factor
            columns: 2
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            clip: true

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
                        playVideo( GlobalProperty.devSitUp, "仰卧起坐指导视频" )
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
                        playVideo( GlobalProperty.devSBJ, "立定跳远指导视频" )
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
                        playVideo( GlobalProperty.devSFB, "坐位体前屈指导视频" )
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
                        playVideo( GlobalProperty.devRunShort, "50米跑指导视频" )
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
                        playVideo( GlobalProperty.devRunMid, "800米跑指导视频" )
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
                        playVideo( GlobalProperty.devPullUp, "引体向上指导视频" )
                    }
                }
            }

            CustVideoPlayItem {
                id: thousandCPItem
                itemWidth: 763
                itemHeight: 543
                itemColor: "#4AACC5"
                title: qsTr("1000米跑")
                imgPath: "qrc:/pic/HomePage/1000meter.png"
                imgWidth: 404
                imgHeight: 549
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        playVideo( GlobalProperty.devRunMid, "1000米跑指导视频" )
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
                        playVideo( GlobalProperty.devVC, "肺活量指导视频" )
                    }
                }
            }

            CustVideoPlayItem {
                id: pushUPItem
                itemWidth: 763
                itemHeight: 543
                itemColor: "#EE7F61"
                title: qsTr("俯卧撑")
                imgPath: "qrc:/pic/HomePage/pushUp.png"
                imgWidth: 638
                imgHeight: 288
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        playVideo( GlobalProperty.devPushUp, "俯卧撑指导视频" )
                    }
                }
            }

            CustVideoPlayItem {
                id: gripItem
                itemWidth: 763
                itemHeight: 543
                itemColor: "#5BA46C"
                title: qsTr("握力")
                imgPath: "qrc:/pic/HomePage/grip.png"
                imgWidth: 219
                imgHeight: 475
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        playVideo( GlobalProperty.devGrip, "握力计指导视频" )
                    }
                }
            }
            CustVideoPlayItem {
                id: visionItem
                itemWidth: 763
                itemHeight: 543
                itemColor: "#3788E8"
                title: qsTr("视力")
                imgPath: "qrc:/pic/HomePage/vision.png"
                imgWidth: 340
                imgHeight: 579
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        playVideo( GlobalProperty.devVision, "视力测试指导视频" )
                    }
                }
            }
        }

    }


    Loader{
        id: videoPlayerLoader
        visible: false
        z: 10
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
        var path = InterAction.getDevVideoPath(videoType)
        console.log( "qml video path = " + path )
        videoPlayerLoader.visible = true
        videoPlayerLoader.source = ""
        videoPlayerLoader.setSource( "qrc:/qmlFiles/Popups/CustVideoDisplayPage.qml", { "curVideoType": videoType,
                                        "bShowrecommendVideo": false} )
    }
}
