import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import QtGraphicalEffects 1.0
import ProjectEnums 1.0
import "../Popups"
import "../Moudues"
import "../"
import "."
import "../ConfigObject/"

Item {
    id: homePage

    width: GlobalProperty.screenZoomX * factor
    height: GlobalProperty.screenZoomY * factor

    property int dlgType: 0 //0-normal 1-中长跑
    property int countSecond: 0 //中长跑倒计时
    property int nfcRecogSecond: 0 //跑步nfc刷卡倒计时

    ListModel {
        id: dynamicObjects
    }

    //一对一：怡康标题栏
    DeviceTitle {
        id: rectDeviceName
        anchors.top: parent.top
        anchors.topMargin: 180 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Component.onCompleted: {
        onViewComplete()
    }

    //常规测试仪器首页
    Item {
        id: normalItem
        width: 1582 * factor
        height: 2403 * factor
        anchors.top: parent.top
        anchors.topMargin: 410 * factor
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: rectCenterRect
            width: 1582 * factor
            height: 1688 * factor
            radius: 30 * factor
            color: "#6A76F5"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            //文字 "人脸识别"
            Text {
                id: strFaceRecog1
                text: qsTr("人脸识别")
                font.pixelSize: 84 * factor
                font.family: "MF DianHei (Noncommercial)"
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: rectCenterRect.bottom
                anchors.bottomMargin: 75 * factor
            }

            //中心区域紫色矩形框
            OtmFaceRecogView {
                id: faceRecog
                width: 1351 * factor
                height: width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 109 * factor
            }

            //直接点进去的开关
            MouseArea {
                anchors.fill: parent

                onDoubleClicked: {
                    if( InterAction.isDeveloper ){
                        GlobalProperty.bIsTouristModel = false
                        mainRoot.turnToStudentInfoPage()
                    }
                }

                onClicked: userAddPopup.open()
            }
        }

        //录入身份弹窗
        OtmPopupUserInfoInput{
            id: userAddPopup
            anchors.centerIn: parent
        }

        //指导视频按钮
        Rectangle {
            id: rectGuideVideoBtn
            width: 749 * factor
            height: 518 * factor
            radius: 30 * factor
            anchors.left: rectCenterRect.left
            anchors.top: rectCenterRect.bottom
            anchors.topMargin: 85 * factor

            Image {
                id: imgGuidePicture
                clip: true
                smooth: true
                fillMode: Image.Stretch
                source: "qrc:/pic/GuideVideo.png"
                anchors.fill: parent
                Image {
                    id: imgPlayBtn
                    width: 254 * factor
                    height: 254 * factor
                    source: "qrc:/pic/14-playBtn.png"
                    anchors.centerIn: imgGuidePicture
                    fillMode: Image.PreserveAspectCrop
                    opacity: 0.9
                }
            }
            Text {
                id: strGuideVideo
                text: GlobalProperty.sportName + qsTr("指导视频")
                font.pixelSize: 54 * factor
                font.family: "Microsoft YaHei"
                color: "white"
                anchors.horizontalCenter: rectGuideVideoBtn.horizontalCenter
                anchors.bottom: rectGuideVideoBtn.bottom
                anchors.bottomMargin: 30 * factor
                z: 5
            }

            Image {
                id: vedioBottom
                width: parent.width - 1 * factor
                height: 223 * factor
                clip: true
                smooth: true
                fillMode: Image.Stretch
                source: "qrc:/pic/picBottom.png"
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }

            //点击指导视频按钮
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mainRoot.showVideoPlayer()
                }
            }
        }

        //游客按钮
        Rectangle {
            id: rectTouristBtn
            width: 749 * factor
            height: 518 * factor
            color: "#DE7663"
            radius: 30 * factor
            anchors.right: rectCenterRect.right
            anchors.top: rectCenterRect.bottom
            anchors.topMargin: 85 * factor
            Image {
                id: imgTouristIcon
                width: 171 * factor
                height: 171 * factor
                source: "qrc:/pic/8-tourist.png"
                anchors.top: parent.top
                anchors.topMargin: 111 * factor
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: strTouristModel
                text: qsTr("游客模式")
                font.pixelSize: 84 * factor
                font.family: "MF DianHei (Noncommercial)"
                color: "white"
                anchors.horizontalCenter: imgTouristIcon.horizontalCenter
                anchors.bottom: rectTouristBtn.bottom
                anchors.bottomMargin: 66 * factor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //点击游客模式
                    GlobalProperty.bIsTouristModel = true
                    InterAction.startElapsedTimer()
                    mainRoot.turnToTouristPage()
                }
            }
        }
    }

    Component {
        id: debugMenu
        Rectangle {
            id: debugMenuRect
            width: 700 * factor
            height: width
            color: "gray"

            Loader {
                id: menuLoader
                z: 5
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent
                sourceComponent: developerDebugBtn
            }

            Component {
                id: developerDebugBtn
                Button {
                    text: "调试工具"
                    onClicked: debugMenuRect.createSelectDebugMenu()
                }
            }

            Component {
                id: scanfAdjust
                Row {
                    Button {
                        text: "-"
                        onClicked: GlobalProperty.scanfHorizontalPos
                                   = (GlobalProperty.scanfHorizontalPos - 10 * factor).toFixed(
                                       0)
                    }

                    Text {
                        text: GlobalProperty.scanfHorizontalPos
                        color: "white"
                        font.pixelSize: 50 * factor
                    }

                    Button {
                        text: "+"
                        onClicked: GlobalProperty.scanfHorizontalPos
                                   = (GlobalProperty.scanfHorizontalPos + 10 * factor).toFixed(
                                       0)
                    }
                }
            }

            Component {
                id: scanfStartPosAdjust
                Row {
                    Button {
                        text: "-"
                        onClicked: {
                            faceRecog.scanfLineStartPos = (faceRecog.scanfLineStartPos - 10
                                                           * factor).toFixed(0)
                            faceRecog.displayScanfStartPos()
                        }
                    }

                    Text {
                        text: faceRecog.scanfLineStartPos
                        color: "white"
                        font.pixelSize: 50 * factor
                    }

                    Button {
                        text: "+"
                        onClicked: {
                            faceRecog.scanfLineStartPos = (faceRecog.scanfLineStartPos + 10
                                                           * factor).toFixed(0)
                            faceRecog.displayScanfStartPos()
                        }
                    }
                }
            }

            Component {
                id: debugMenuGrid
                Grid {
                    ListModel {
                        Component.onCompleted: {
                            debugCmpList.append({
                                                    "order": 0,
                                                    "name": "扫描线"
                                                })
                            debugCmpList.append({
                                                    "order": 1,
                                                    "name": "扫描起点"
                                                })
                        }

                        id: debugCmpList
                    }
                    rows: 3
                    columns: 3
                    spacing: 10 * factor
                    Repeater {
                        model: debugCmpList
                        delegate: Button {
                            width: 200 * factor
                            height: width
                            text: name
                            background: Rectangle {
                                color: "lightgreen"
                                MouseArea {
                                    anchors.fill: parent
                                    onPressed: opacity = 0.5
                                    onReleased: opacity = 0
                                    onClicked: onBtnClicked(order)
                                }
                            }
                        }
                    }
                }
            }

            function onBtnClicked(order) {
                switch (order) {
                case 0:
                    menuLoader.sourceComponent = scanfAdjust
                    return
                case 1:
                    menuLoader.sourceComponent = scanfStartPosAdjust
                }
            }

            function createSelectDebugMenu() {
                menuLoader.sourceComponent = debugMenuGrid
            }

            MouseArea {
                id: debugMenuMouseArea
                property int lastPosX: 0
                property int lastPosY: 0
                property bool isPressed: false

                anchors.fill: parent
                onReleased: isPressed = false
                onPressed: onMousePressed(mouse)
                onPositionChanged: moveDebugTool(mouse)

                function onMousePressed(mouse) {
                    isPressed = true
                    debugMenuMouseArea.lastPosX = mouse.x
                    debugMenuMouseArea.lastPosY = mouse.y
                }

                function moveDebugTool(mouse) {
                    if (!debugMenuMouseArea.isPressed) {
                        return
                    }
                    var offsetX = mouse.x - debugMenuMouseArea.lastPosX
                    var offsetY = mouse.y - debugMenuMouseArea.lastPosY
                    debugMenuRect.x += offsetX
                    debugMenuRect.y += offsetY
                }
            }
        }
    }

    Connections {
        target: InterAction

        //人脸识别成功
        onSignalIdentifyUserSuccess: {
            console.log("recv signal signalIdentifyUserSuccess inetrAction")
            InterAction.startElapsedTimer()
            GlobalProperty.bIsTouristModel = false
            GlobalProperty.reFreshCurStuInfo()

            //2023.01.12， 每一次识别完一个人，就置为false， 表示该用户目前还未进入过测试页面
            GlobalProperty.curUserEnteredTestView = false

            mainRoot.recogSuccessTurnToStudentInfoPage()


        }
    }

    function createDebugTool() {
        var obj = debugMenu.createObject(homePage, {
                                             "x": 0,
                                             "y": 0
                                         })
        dynamicObjects.append({
                                  "obj": obj
                              })
    }

    function onViewComplete() {

        InterAction.setCurrentPage( Enums.PAGE_DIRECT_HOME )

        //2022/07/07 修改: 加载同步模式的时候，先读取提交配置(异步模式强制改变了提交模式，影响了GlobbalProperty。commit的值)
        InterAction.readCommitPopupConfigFromDataBase()
        //获取匹配的视频封面图片路径
        imgGuidePicture.source = ""
        imgGuidePicture.source = GlobalProperty.getSuitableVideoCover()


        //2022.07.15 修改， 解决一对一签字版变成了一对多签字版的bug
        SignPainterConfig.initSignPainter(true)

        //2022/07/25   开发者模式下，创建调试按钮
        //        if( InterAction.isDeveloper ){
        //            createDebugTool()
        //        }

        //2022.07.18 修改, 界面跳转计时
        InterAction.stopElapsedTimer("跳转到一对一首页")
    }
}
