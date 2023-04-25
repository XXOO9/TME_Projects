import QtQuick 2.0
import QtQuick.Controls 2.5
import CustVideoPlayer 1.0
import QtQuick.Layouts 1.12

//定位布局 - 不用写太多锚定位
Item {
    width: 1919 * factor
    height: 1081 * factor

    //视频路径
    property string strVideoPath: ""

    //是否处于播放状态
    property bool bPlaying: false

    //进度条是否显示
    property bool bPrograssBarVis: false

    FontLoader{
        id: fontLoader
        source: "qrc:/font/PingFang Regular.ttf"
    }

    OpencvVideoPlayer {
        id: videoPlayer
        videoControl: opencvViewPort
    }

    MouseArea {
        id: playMouseArea
        anchors.fill: parent
        onClicked: {
            if(bPlaying)
            {
                bPlaying = false
                imgPauseBtn.visible = true
                videoPlayer.playState = OpencvVideoPlayer.PausedState
            }
            else
            {
                bPlaying = true
                imgPauseBtn.visible = false
                videoPlayer.playState = OpencvVideoPlayer.PlayingState
            }
        }
    }

    Item {
        id: video_box
        anchors.fill: parent

        OpencvVideoControl{
            id: opencvViewPort
            anchors.fill: parent
        }

        Image {
            id: imgPauseBtn
            width: 197 * factor
            height: 197 * factor
            anchors.centerIn: parent
            source: "qrc:/pic/9-playBtn.png"
            visible: false
        }

        //显示控制按钮的区域
        MouseArea {
            id:showArea
            width:parent.width
            height: 253 * factor
            anchors.bottom: parent.bottom
            hoverEnabled: true
            propagateComposedEvents: true

            onEntered: {
                bPrograssBarVis = true
            }

            onExited: {
                bPrograssBarVis = false
            }
        }

        Item {
            id: progressBarItem
            anchors.left: parent.left
            anchors.leftMargin: 310 * factor + 25 * factor
            anchors.bottom: parent.bottom
            width: 1256 * factor
            height: 190 * factor

            Rectangle {
                width: parent.width
                height: 15 * factor
                color: "lightGray"
                radius: 15
                visible: bPrograssBarVis
                anchors.centerIn: parent

                Rectangle {
                    id: rect
                    anchors{
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }
                    radius: 15
                    color: "#757575"
                    width:  videoPlayer.frameNumber > 0 ?
                                parent.width*videoPlayer.frameNumber/videoPlayer.frameCount : 0
                }

                Image {
                    id: handle
                    width: 50 * factor
                    height: 50 * factor
                    anchors.horizontalCenter: rect.right
                    anchors.verticalCenter: rect.verticalCenter
                    source: "qrc:/pic/Progress_Bar.png"
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var _fNum = videoPlayer.frameCount * mouse.x/width|0
                    videoPlayer.setVideoframeNumber(_fNum)
                }
            }
        }


        //在进度条左侧显示当前播放时间
        Text{
            id:current_time
            anchors.top: parent.top
            anchors.topMargin: 946 * factor
            anchors.left: parent.left
            anchors.leftMargin: 144 * factor
            font{family: fontLoader.name;pixelSize: 62 * factor }
            text: videoPlayer.curTime
            color:"white"
            visible: bPrograssBarVis
        }

        //在进度条右侧显示视频总共时间
        Text{
            id: video_time
            anchors.top: parent.top
            anchors.topMargin: 946 * factor
            anchors.right: parent.right
            anchors.rightMargin: 129 * factor
            font{family: fontLoader.name;pixelSize: 62 * factor }
            text: videoPlayer.totalTime
            color:"white"
            visible: bPrograssBarVis
        }
    }


    //开始播放
    function play(){
        bPlaying = true
        videoPlayer.videoURL = strVideoPath
        imgPauseBtn.visible = false
        videoPlayer.playState = OpencvVideoPlayer.PlayingState
    }

    //暂停播放
    function pause(){
        bPlaying = false
        imgPauseBtn.visible = true
        videoPlayer.playState = OpencvVideoPlayer.PausedState
    }

    //结束播放
    function stop(){
        bPlaying = false
        videoPlayer.playState = OpencvVideoPlayer.StoppedState
    }
}
