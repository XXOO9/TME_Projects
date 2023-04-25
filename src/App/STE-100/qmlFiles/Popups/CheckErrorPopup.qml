import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../"

Item {
    id : root
    width: 1582 * factor
    height: 1116 * factor

    property alias popupVisible: pop.visible

    signal signalDetailBtnClicked()
    signal signalEnterBtnClicked()

    Popup{
        id: pop
        modal: true
        visible: true
        width: root.width
        height: root.height
        anchors.centerIn: parent
        closePolicy: Popup.NoAutoClose

        background: Rectangle{
            id: backGrd
            width: root.width
            height: root.height
            radius: 30 * factor
            color: "#747DF2"
            clip: true
            //顶部文字补充条-消除标题圆角效果
            Rectangle{
                id: rectTopTextSupRect
                width: parent.width
                height: 20 * factor
                color: "#4D3DDD"
                anchors.bottom: rectTopText.bottom
                anchors.horizontalCenter:backGrd.horizontalCenter
            }
            //顶部文字 和 按钮
            Rectangle{
                id: rectTopText
                width: parent.width
                height: 197 * factor
                radius: 30 * factor
                color: "#4D3DDD"
                anchors.top: backGrd.top
                z:1
                anchors.horizontalCenter:backGrd.horizontalCenter

                Rectangle {
                    id: pointRect
                    width: 23 * factor
                    height: 23 * factor
                    radius: width / 2
                    color: "#FFB784"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: 5 * factor
                    anchors.left: parent.left
                    anchors.leftMargin: 56 * factor
                }

                Text {
                    id: titleText
                    text: qsTr( "系统检测异常！" )
                    font.pixelSize: 78 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 108 * factor
                }
            }

            //错误提示区域
            Row {
                id: tipArea
                width: 1280 * factor
                height: 565 * factor
                anchors.top: rectTopText.bottom
                anchors.topMargin: 48 * factor
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 300 * factor

                Item {
                    id: devErrorItem
                    width: 490 * factor
                    height: 565 * factor
                    visible: true //自检设备连接异常
                    Image {
                        id: devErrorImg
                        width: 405 * factor
                        height: 414 * factor
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        source: "qrc:/pic/DevError.png"
                    }

                    Text {
                        id: devErrorText
                        text: qsTr("体测设备连接异常")
                        font.pixelSize: 62 * factor
                        font.family: "Microsoft YaHei"
                        color: "white"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: devErrorImg.bottom
                        anchors.topMargin: 74 * factor
                    }
                }

                Item {
                    id: netErrorItem
                    width: 490 * factor
                    height: 565 * factor
                    visible: true //自检服务器连接异常
                    Image {
                        id: netErrorImg
                        width: 377 * factor
                        height: 443 * factor
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        source: "qrc:/pic/24-2NetWorkError.png"
                    }

                    Text {
                        id: netErrorText
                        text: qsTr("服务器连接异常")
                        font.pixelSize: 62 * factor
                        font.family: "Microsoft YaHei"
                        color: "white"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: netErrorImg.bottom
                        anchors.topMargin: 61 * factor
                    }
                }
            }

            //退出系统
            Rectangle{
                id:rectCleanBtn
                width: 422 * factor
                height: 141 * factor
                radius: 70 * factor
                color: "#2B41D0"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 87 * factor
                anchors.left: parent.left
                anchors.leftMargin: 339 * factor
                Text {
                    text: qsTr( "退出系统" )
                    font.pixelSize: 62 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        //退出软件
                        InterAction.quit()
                    }
                }
            }
            //确认按钮
            Rectangle{
                id:rectConfirmBtn
                width: 422 * factor
                height: 141 * factor
                radius: 70 * factor
                color: "#2B41D0"
                anchors.bottom: rectCleanBtn.bottom
                anchors.right: parent.right
                anchors.rightMargin: 339 * factor
                Text {
                    text: qsTr( "直接进入" )
                    font.pixelSize: 62 * factor
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        //点击直接进入
                        selfchek.selfCheckFinished( false );
                        hide();
                    }
                }
            }
        }
    }

    function show(){
        popupVisible = true
    }

    function hide(){
        popupVisible = false
    }
}
