import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../Moudues"
import "../"

Item {
    id: root

    //表头
    property var headName: ["", "设备号", "分机编号", "修改结果"]

    //列宽
    property var widthArray: [0.1, 0.3, 0.3, 0.3]

    //表格列数
    property int tableCount: 3

    //列高
    property real itemHeigt: 117 * factor

    //表格字体大小
    property var headerTextPix: 62

    //表格字体大小
    property var tableTextPix: 50

    Popup{
        id: m_popup
        width: parent.width
        height: parent.height
        modal: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: parent

        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
        }

        Rectangle{
            width: 1580 * factor
            height: 1020 * factor
            anchors.centerIn: parent
            color: "#6A76F5"

            Rectangle{
                id: topTitleRect
                width: parent.width
                height: 200 * factor
                color: "#5049E5"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    text:  "设置分机编号"
                    color: "white"
                    font{ family: "Microsoft YaHei"; pixelSize: 80 * factor; bold: true }
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 60 * factor
                }

                Image {
                    id: closeImg
                    width: 100 * factor
                    height: width
                    source: "qrc:/pic/22-CloseBtn.png"
                    anchors.right: parent.right
                    anchors.rightMargin: 50 * factor
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            close()
                        }
                    }
                }
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: topTitleRect.bottom
                anchors.topMargin: 250 * factor
                text: "没有可用设备!"
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 72 * factor ; bold: true}
                visible: !headRow.visible//!tableViewItem.visible
            }

            Text {
                id: tipText
                width: parent.width - 335 * factor
                wrapMode: Text.WrapAnywhere
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20 * factor
                horizontalAlignment: Text.AlignHCenter
                text: "组网模式下设置分机编号为0,该设备将被软件暂时屏蔽,无法识别为可测试设备!"
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 50 * factor}
                visible: headRow.visible
            }

            Item {
                id: tableViewItem
                width: parent.width - 400 * factor
                height: parent.height - topTitleRect.height - 180 * factor
                anchors.top: topTitleRect.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                //表格 列头
                Item {
                    id: headItem
                    width: parent.width
                    height: itemHeigt
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    Row {
                        id: headRow
                        anchors.fill: parent
                        spacing: -1
                        visible: DevNumModel.rowCount()!==0
                        Repeater {
                            model: 3
                            Item{
                                id: headItemRect
                                width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[index]
                                height: itemHeigt
                                Text {
                                    text: headName[index]
                                    font.pixelSize: 50 * factor
                                    font.family: "Microsoft YaHei"
                                    font.bold: true
                                    color: "#D8D6F0"
                                    anchors.centerIn: parent
                                }
                            }
                        }
                    }
                }

                TableView{
                    id: tableView
                    width: parent.width
                    height: parent.height - headItem.height
                    anchors.top: headItem.bottom
                    anchors.topMargin: -1
                    rowSpacing: 18 * factor
                    columnSpacing: -1
                    model: DevNumModel
                    flickableDirection: Flickable.VerticalFlick
                    clip: true
                    property int curIndex: -1

                    //滚动条
                    ScrollBar.vertical: ScrollBar {
                        visible: true
                    }

                    onRowsChanged: {
                       if(DevNumModel.rowCount()>0)
                       {
                           headRow.visible = true
                       }
                       else
                       {
                           headRow.visible = false
                       }
                   }

                    delegate: Row {
                        id: currentRow
                        width: tableView.width
                        height: itemHeigt

                        property bool btnVisible: false
                        property int curDevNum: 1

                        Item {
                            id: imgItem
                            width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[0]
                            height: parent.height
                            Image {
                                id: setImg
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                height: itemHeigt * 0.8
                                width: itemHeigt  * 0.8
                                visible: forth === "true"
                                source: third==="false"? "qrc:/pic/failed.png" : "qrc:/pic/success.png"
                            }
                        }

                        Item {
                            id: firstColunm
                            width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[1]
                            height: parent.height
                            Item {
                                id: firstRect
                                anchors.fill: parent
                                anchors.centerIn: parent
                                Text {
                                    id: fristText
                                    text: first
                                    fontSizeMode: Text.Fit
                                    width: parent.width
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pixelSize: tableTextPix * factor
                                    font.family: "Microsoft YaHei"
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }

                        CustComboBox{
                            id: curDevComboBox
                            width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[2]
                            height: parent.height
                            comboBoxDisplayList: [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                                                   "11", "12", "13", "14", "15", "16", "17", "18", "19", "20" ]
                            curComboBoxIndex:  second * 1
                            onSignalValueRefresh:{
                                tableView.curIndex = row
                                if(currentRow.curDevNum !== index)
                                {
                                    currentRow.curDevNum = index * 1;
                                    second = index * 1
                                    curComboBoxIndex = index * 1
                                }
                                InterAction.updateDevModelData(second, currentRow.curDevNum)
                                InterAction.updateDevModelData(first, "false")
                                console.log("curDevComboBox text", curDevComboBox.textStr, second)
                            }
                        }

                        Item {
                            id: btnItem
                            width: ( tableView.width + (tableCount - 1) * 1 ) * widthArray[3]
                            height: parent.height

                            Button{
                                id: sureBtn
                                width: 200 * factor
                                height: parent.height
                                anchors.left: parent.left
                                anchors.leftMargin: 40 * factor
                                visible: tableView.curIndex === row || third==="false" ? true:false
                                background: Rectangle{
                                    anchors.fill: parent
                                    color: "#1F71CF"
                                    border.color: "#1ADDFC"
                                    radius: 30 * factor
                                    Text {
                                        text: "确定"
                                        color: "white"
                                        font{ family: "Microsoft YaHei"; pixelSize: 60 * factor }
                                        anchors.centerIn: parent
                                    }
                                }
                                onClicked: {
                                    tableView.curIndex = -1
                                    InterAction.updateDevModelData(first, "true")
                                    InterAction.setCurDevNo(first, currentRow.curDevNum)

                                }
                            }
                        }
                    }
                }
            }
        }
    }


    function open(){
        m_popup.open()
        InterAction.getAllDevToSetNum()
        InterAction.curPage = Enums.PAGE_SETTING
    }

    function close(){
        m_popup.close()
        InterAction.curPage = GlobalProperty.pageBeforoPopups
    }
}
