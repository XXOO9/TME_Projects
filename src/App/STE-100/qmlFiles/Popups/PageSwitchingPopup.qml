/*************************************************
  <文件名称>    PageSwitchingPopup
  <功能描述>    界面跳转切换弹窗

  <作    者>    tcy
  <时    间>    2021-10-20
**************************************************/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "."

Item {
    id : root
    width: 1200 * factor
    height: 340 * factor

    property alias popupVisible: pop.visible
    property int switchCount: 10

    signal popClose();

    Popup{
        id: pop
        modal: true
        visible: true
        width: root.width
        height: root.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnPressOutside
        onClosed: {
            popClose();
            InterAction.qmlLogout("弹窗关闭")
            InterAction.stopSpeech()
        }

        onOpened: {
            InterAction.textToSpeech( "长时间未操作，即将自动退出，点击弹窗即可取消" )
        }


        background: Rectangle{
            id: backGrd
            width: root.width
            height: root.height
            radius: 30 * factor
            color: "#3241CC"

            Row {
                anchors.centerIn: parent
                Text {
                    text: qsTr("因长期未操作，")
                    height: backGrd.height - 140 * factor
                    color: "white"
                    font.pixelSize: 62 * factor
                    font.family: "PingFang SC"
                    font.bold: true
                    verticalAlignment: Text.AlignBottom
                }

                Text {
                    id: numText
                    text: switchCount
                    height: backGrd.height - 140 * factor
                    color: "#F39800"
                    font.pixelSize: 120 * factor
                    font.family: "Rosenrot"
                    verticalAlignment: Text.AlignBottom
                }

                Text {
                    text: qsTr("s后将返回到首页！")
                    height: backGrd.height - 140 * factor
                    font.bold: true
                    color: "white"
                    font.pixelSize: 62 * factor
                    font.family: "PingFang SC"
                    verticalAlignment: Text.AlignBottom
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                hide()
            }
        }
    }

    function hide(){
        popClose();
        popupVisible = false;
    }
}
