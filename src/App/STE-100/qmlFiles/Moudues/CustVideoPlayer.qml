import QtQuick 2.9
import QtMultimedia 5.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: root
    property real playerWidth: 1919 * factor

    property real playerHeight: 1080 * factor

    //视频路径
    property string strVideoPath: ""

    //是否处于播放状态
    property bool bPlaying: false

    //进度条是否显示
    property bool bPrograssBarVis: false

    width: playerWidth
    height: playerHeight

    Component.onCompleted: {
        //play()
    }

    Rectangle{
        id: playerView
        width: root.width
        height: root.height
        color: "black"

        FontLoader{
            id: fontLoader
            source: "qrc:/font/PingFang Regular.ttf"
        }

        MediaPlayer {
            id: mediaplayer
            source: strVideoPath
            onPlaybackStateChanged: {
                if( mediaplayer.playbackState === MediaPlayer.PlayingState ){
                    console.log( "ret1 = ", MediaPlayer.PlayingState )//1
                }
                if( mediaplayer.playbackState === MediaPlayer.PausedState ){
                    console.log( "ret2 = ", MediaPlayer.PausedState )
                }

                if( mediaplayer.playbackState === MediaPlayer.StoppedState ){
                    console.log( "ret3 = ", MediaPlayer.StoppedState )//0
                }
            }
            onError: {
                console.log("MediaPlayer", error, errorString)
            }
        }

        VideoOutput {
            anchors.fill: parent
            source: mediaplayer
        }

        //播放按钮图标
        Image {
            id: imgPauseBtn
            visible: true
            width: 197 * factor
            height: width
            clip: true
            source: "qrc:/pic/9-playBtn.png"
            anchors.centerIn: parent
        }

        //播放区域
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if( bPlaying === false ){
                    console.log( "开始播放" )
                    mediaplayer.play()
                    bPlaying = true
                }else{
                    console.log( "暂停播放" )
                    mediaplayer.pause()
                    bPlaying = false
                }
                imgPauseBtn.visible = !bPlaying
            }
        }

        //显示控制按钮的区域
        MouseArea {
            id:showArea
            width:parent.width
            height:253 * factor
            anchors.bottom: parent.bottom
            hoverEnabled: true

            onEntered: {
                bPrograssBarVis = true
            }

            onExited: {
                bPrograssBarVis = false
            }
        }

        //进度条
        //进度条显示(在左右侧显示时间戳)
        //样式：白色100% 白色40%
        Slider{
            id:progress_bar
            anchors.left: parent.left
            anchors.leftMargin: 310 * factor + 25 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 90 * factor
            width:1306 * factor - 50 * factor
            height:8 * factor
            maximumValue: mediaplayer.duration
            minimumValue: 0
            value: mediaplayer.position
            stepSize: 1000
            visible: bPrograssBarVis
            style: SliderStyle {
                groove: Rectangle {
                    implicitHeight:8 * factor
                    color: "gray"
                    Rectangle {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: mediaplayer.duration>0?parent.width*mediaplayer.position/mediaplayer.duration:0
                        color: "white"

                        Image {
                            anchors.left: parent.left
                            anchors.leftMargin: -25 * factor + parent.width
                            anchors.top: parent.top
                            anchors.topMargin: -20 * factor
                            height:50 * factor
                            width: 50 * factor
                            source: "qrc:/pic/Progress_Bar.png"
                            fillMode: Image.Stretch
                            visible: bPrograssBarVis
                            z:1
                        }
                    }
                }

                handle: Rectangle {
                    anchors.centerIn: parent
                    visible: false
                    color: "white"
                    border.color: "white"
                    border.width: 2
                    implicitWidth: 25
                    implicitHeight: 25
                    radius:15
                    Rectangle{
                        width: parent.width-8
                        height: width
                        radius: width/2
                        color: "white"
                        anchors.centerIn: parent
                    }
                }
            }

            MouseArea{
                property double pos
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    bPrograssBarVis = true
                }

                onClicked: {
                    if (mediaplayer.seekable)
                        pos = mediaplayer.duration * mouse.x/parent.width
                    mediaplayer.seek(pos)
                    progress_bar.value=pos;
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
            text:parent.change_time_style(mediaplayer.position)
            color:"white"

            visible: bPrograssBarVis
        }

        //在进度条右侧显示视频总共时间
        Text{
            id:video_time
            anchors.top: parent.top
            anchors.topMargin: 946 * factor
            anchors.right: parent.right
            anchors.rightMargin: 129 * factor
            font{family: fontLoader.name;pixelSize: 62 * factor }
            text:parent.change_time_style(mediaplayer.duration)
            color:"white"

            visible: bPrograssBarVis
        }

        //时间格式化
        function change_time_style(time){
            var sec= Math.floor(time/1000);
            var hours=Math.floor(sec/3600);
            var minutes=Math.floor((sec-hours*3600)/60);
            var seconds=sec-hours*3600-minutes*60;
            var hh,mm,ss;
            if(hours.toString().length<2)
                hh="0"+hours.toString();
            else
                hh=hours.toString();
            if(minutes.toString().length<2)
                mm="0"+minutes.toString();
            else
                mm=minutes.toString();
            if(seconds.toString().length<2)
                ss="0"+seconds.toString();
            else
                ss=seconds.toString();
            //默认只显示分钟
            if(hh === "00")
                return mm+":"+ss
            else
                return hh+":"+mm+":"+ss
        }
    }

    //开始播放
    function play(){
        bPlaying = true
        mediaplayer.source = strVideoPath
        //mediaplayer.play()
        imgPauseBtn.visible = false
        playTimer.restart()
    }

    //停止播放
    function pause(){
        bPlaying = false
        mediaplayer.pause()
        imgPauseBtn.visible = true
    }

    function stop(){
        bPlaying = false
        mediaplayer.stop()
        imgPauseBtn.visible = true
    }

    Timer {
        id: playTimer
        interval: 100
        repeat: false
        running: false
        onTriggered: {
            mediaplayer.play()
        }
    }
}
