import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../"
import "../Moudues"
import "../Popups"
import "."

//异常进入系统时，不显示推荐视频
//正常进入系统的时， 要显示推荐视频
Item{
    id: root
    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor
    property string strTips: "50m指导视频"
    property string videoPathPrefix: "file:///"
    property string videoPath: ""
    property bool bShowrecommendVideo: true
    property bool bVideoExist: false
    property int curVideoType: GlobalProperty.iDevType

    signal signalCloseBtnClicked()

    //防止鼠标穿透
    MouseArea {
        anchors.fill: parent
        onClicked: {

        }
    }

    ListModel {
        id: recommondModel
        ListElement { strName: "肺活量指导视频"; imgPath: "qrc:/pic/videoCovers/VC_1.png" }
        ListElement { strName: "立定跳远指导视频"; imgPath: "qrc:/pic/videoCovers/SBJ_1.png" }
        ListElement { strName: "坐位体前屈指导视频"; imgPath: "qrc:/pic/videoCovers/SFB_1.png" }
        ListElement { strName: "引体向上指导视频"; imgPath: "qrc:/pic/videoCovers/PULL_UP_1.png" }
        ListElement { strName: "仰卧起坐指导视频"; imgPath: "qrc:/pic/videoCovers/SIT_UP_1.png" }
        ListElement { strName: "身高体重体成分指导视频"; imgPath: "qrc:/pic/videoCovers/BODY_1.png" }
        ListElement { strName: "握力指导视频"; imgPath: "qrc:/pic/videoCovers/GRIP_1.png" }
        ListElement { strName: "中长跑指导视频"; imgPath: "qrc:/pic/videoCovers/run_1.png" }
        ListElement { strName: "跳绳指导视频"; imgPath: "qrc:/pic/videoCovers/ROPE_SKIN_1.png" }
    }

    Component.onCompleted: {
        console.log("curVideoType", curVideoType , GlobalProperty.iDevType)
        playVideo(curVideoType)
    }

    function checkVideoPath(path)
    {
        bVideoExist = InterAction.checkDevVideoPathExist(path)
        console.log("iDevType", GlobalProperty.iDevType, videoPath, bVideoExist)
    }

    function playVideo(devVideoType)
    {
        //获取播放标题
        getVideoTitle(devVideoType)

        //获取播放路径
        videoPath = InterAction.getDevVideoPath(devVideoType)

        //检查播放视频是否存在
        checkVideoPath(videoPath)

        //设置播放控件路径
        if(videoPath!=="")
        {
            videoPlayer.strVideoPath = videoPathPrefix + videoPath
            videoPlayer.play()
        }
        else
        {
            videoPlayer.stop()
        }
    }

    //页面背景
    Rectangle{
        id: backGround
        anchors.fill: parent
        color: "#333333"
    }

    //视频类型，  例如： 50m指导视频
    Text {
        id: tipsText
        text: strTips
        font{ pixelSize: 78 * factor; family: "Microsoft YaHei" }
        color: "white"
        anchors.top: parent.top
        anchors.topMargin: 332 * factor
        anchors.left: parent.left
        anchors.leftMargin: 125 * factor
    }

    //视频界面
    CustVideoPlayer{
        id: videoPlayer
        playerWidth: 1919 * factor
        playerHeight:  1081 * factor
        strVideoPath: videoPathPrefix + videoPath
        z:5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 466 * factor
        enabled: bVideoExist
    }

//    CustVideoPlayerCV{
//        id: videoPlayer
//        width: 1919 * factor
//        height: 1081 * factor
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.top: parent.top
//        anchors.topMargin: 466 * factor
//        strVideoPath: videoPathPrefix + videoPath
//        visible: bVideoExist
//    }

    Rectangle {
        id: backGroundRect
        anchors.fill: videoPlayer
        color: "black"
        visible: !bVideoExist
        z: 10

        Text {
            text: "暂无此视频"
            anchors.centerIn: parent
            font{family: "Microsoft YaHei"; pixelSize: 84 * factor; bold: true}
            color: "white"
        }
    }

    //推荐视频框
    //文字 -> "推荐视频"
    Text {
        id: strRecommendVideo
        visible: bShowrecommendVideo
        text: "推荐视频"
        font{ pixelSize: 78 * factor; family: "Microsoft YaHei" }
        color: "white"
        anchors.left: parent.left
        anchors.leftMargin: 57 * factor
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1115 * factor
    }

    //推荐视频滑动区域
    Item{
        id: rectRecommendVideo
        visible: bShowrecommendVideo
        width: 1806 * factor
        height: 459 * factor
        z:5
        anchors.left: strRecommendVideo.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 604 * factor

        ListView{
            width: rectRecommendVideo.width
            height: rectRecommendVideo.height
            spacing: 110 * factor
            orientation: ListView.Horizontal
            anchors.horizontalCenter: parent.horizontalCenter
            model: recommondModel
            clip: true
            snapMode: ListView.SnapToItem
            delegate: Rectangle{
                width: 500 * factor
                height: 458 * factor
                radius: 30 * factor
                Image {
                    source: imgPath
                    anchors.fill: parent
                    Image {
                        width: 152 * factor
                        height: width
                        source: "qrc:/pic/9-playBtn.png"
                        anchors.centerIn: parent
                    }
                    Image {
                        width: parent.width
                        height: parent.height / 3
                        source: "qrc:/pic/picBottom.png"
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        text: strName
                        width: parent.width
                        elide: Text.ElideRight
                        font{ pixelSize:  50 * factor; family: "Microsoft YaHei" }
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 25 * factor
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        //切换视频播放
                        var devType = getCurDevType(index)
                        playVideo(devType)
                    }
                }
            }
        }
    }

    Popup{
        id: errorPopup
        width: 800 * factor
        height: 400 * factor
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose
        anchors.centerIn: parent
        background: Rectangle{
            anchors.fill: parent
            radius: 30 * factor
            color: "#768CF7"
        }

        contentItem: Item {
            anchors.fill: parent
            Rectangle{
                color: "skyblue"
                anchors.top: parent.top
                anchors.topMargin: 30 * factor
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    text: "暂无此视频"
                    anchors.centerIn: parent
                    font{family: "Microsoft YaHei"; pixelSize: 84 * factor; bold: true}
                    color: "white"
                }
            }
        }
    }

    //退出按钮
    Rectangle{
        id: imgExitBtn
        width: 168 * factor
        height: width
        radius: width / 2
        color: "transparent"
        border.width: 3 * factor
        border.color: "#5C5C5C"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 140 * factor
        z:5
        Image {
            width: 62 * factor
            height: width
            source: "qrc:/pic/22-CloseBtn.png"
            anchors.centerIn: parent
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                //退出前，先暂停
                videoPlayer.stop()
                signalCloseBtnClicked()
            }
        }
    }

    MouseArea{
        anchors.fill: parent
        propagateComposedEvents: false
    }


    function getVideoTitle( devType ){
        switch( devType ){
        case GlobalProperty.devVC:
            strTips = "肺活量指导视频"
            break
        case GlobalProperty.devSBJ:
            strTips = "立定跳远指导视频"
            break
        case GlobalProperty.devSFB:
            strTips = "坐位体前屈指导视频"
            break
        case GlobalProperty.devPullUp:
            strTips = "引体向上指导视频"
            break
        case GlobalProperty.devSitUp:
            strTips = "仰卧起坐指导视频"
            break
        case GlobalProperty.devBody:
            strTips = "身高体重体成分指导视频"
            break
        case GlobalProperty.devRunMid:
            strTips = "中长跑测试指导视频"
            break
        case GlobalProperty.devRunShort:
            strTips = "短跑测试指导视频"
            break
        case GlobalProperty.devRopeSkip:
            strTips = "跳绳测试指导视频"
            break
        case GlobalProperty.devGrip:
            strTips = "握力测试指导视频"
            break
        case GlobalProperty.devPushUp:
            strTips = "俯卧撑测试指导视频"
            break
        case GlobalProperty.devVision:
            strTips = "视力测试指导视频"
            break
        case GlobalProperty.devPVV:
            strTips = "肺通气量指导视频"
            break
        case GlobalProperty.devFootBall:
            strTips = "足球测试指导视频"
            break
        case GlobalProperty.devBasketBall:
            strTips = "篮球测试指导视频"
            break
        case GlobalProperty.devVolleyBall:
            strTips = "排球测试指导视频"
            break
        case GlobalProperty.devPUO:
            strTips = "斜身引体向上测试指导视频"
            break
        case GlobalProperty.devSoldBall:
            strTips = "实心球测试指导视频"
            break
        case GlobalProperty.devVerticalJump:
            strTips = "纵跳摸高测试指导视频"
            break
        default:
            strTips = "null"
        }
    }

    //获取当前model设备类型
    function getCurDevType(modelIndex)
    {
        switch(modelIndex){
        case 0:
            return GlobalProperty.devVC
        case 1:
            return GlobalProperty.devSBJ
        case 2:
            return GlobalProperty.devSFB
        case 3:
            return GlobalProperty.devPullUp
        case 4:
            return GlobalProperty.devSitUp
        case 5:
            return GlobalProperty.devBody
        case 6:
            return GlobalProperty.devGrip
        case 7:
            return GlobalProperty.devRunMid
        case 8:
            return GlobalProperty.devRopeSkip
        default:
            return ""
        }
    }
}
