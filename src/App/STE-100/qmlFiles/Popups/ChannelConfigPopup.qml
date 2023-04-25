import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../Moudues"
import "../"

Item {
    id: root

    property int curDevType: 0
    property int curChannel: GlobalProperty.channel
    property int curMaxDevNum: GlobalProperty.maxConnectCount

    function initChannelConfig(devType)
    {
        tipsText.text = qsTr("如果需要同一时段、空间下使用短跑和往返跑，则短跑信道为69，往返跑信道为169!")
        tipsText.visible = false
        var jsonData = InterAction.getChannelCongigJsonStr()
        if(jsonData.length)
        {
            var jsonObject = JSON.parse(jsonData)
            for(var i=0; i< jsonObject.Channels.length; i++)
            {
                if(jsonObject.Channels[i].devType === devType)
                {
                    tipsText.visible = jsonObject.Channels[i].devType === GlobalProperty.devRunShort ? true : false
                    var chanelArr = jsonObject.Channels[i].channel.split('-').filter(function(el){return el!==""})
                    channelChooseComboBox.comboBoxDisplayList = chanelArr
                }
            }
        }
        sureBtn.setActive()
    }

    Component.onCompleted: {
        InterAction.getDevChannelConfig()
        InterAction.getDevMaxConnectCount()
    }

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
            height: 1420 * factor
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
                    text:  "信道配置"
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
                id: testModelText
                text: "设备选择"
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                anchors.top: topTitleRect.bottom
                anchors.topMargin: 140 * factor
                anchors.left: parent.left
                anchors.leftMargin: 195 * factor
            }

            CustComboBox{
                id: modelSelectComboBox
                width: 900 * factor
                height: 140 * factor
                comboBoxDisplayList: [ "身高体重体成分", "肺活量", "立定跳远", "坐位体前屈", "引体向上",
                                       "仰卧起坐", "短跑", "中长跑", "跳绳", "握力", "俯卧撑", "视力", "肺通气量",
                                       "足球", "篮球", "排球", "实心球", "斜身引体向上", "纵跳摸高"]

                anchors.verticalCenter: testModelText.verticalCenter
                anchors.left: testModelText.right
                anchors.leftMargin: 27 * factor
                curComboBoxIndex: GlobalProperty.iDevType - 1
            }

            Text {
                id: channelChooseText
                text: qsTr("信道选择")
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                anchors.top: testModelText.bottom
                anchors.topMargin: 140 * factor
                anchors.right: testModelText.right
            }

            CustComboBox{
                id: channelChooseComboBox
                width: 900 * factor
                height: 140 * factor
                comboBoxDisplayList: [ "33", "205" ]
                anchors.verticalCenter: channelChooseText.verticalCenter
                anchors.left: modelSelectComboBox.left
                curComboBoxIndex: GlobalProperty.channel === channelChooseComboBox.comboBoxDisplayList[0] ? 0:1
            }

            Text {
                id: maxDeviceText
                text: qsTr("最大连接数量")
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                anchors.top: channelChooseText.bottom
                anchors.topMargin: 140 * factor
                anchors.right: channelChooseText.right
            }

            CustComboBox{
                id: maxDevComboBox
                width: 900 * factor
                height: 140 * factor
                comboBoxDisplayList: [ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                                       "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                                       "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"]
                anchors.verticalCenter: maxDeviceText.verticalCenter
                anchors.left: modelSelectComboBox.left
                curComboBoxIndex: GlobalProperty.maxConnectCount>=1 && GlobalProperty.maxConnectCount<=30 ?
                                  GlobalProperty.maxConnectCount -1 : 7;
            }

            Text {
                id: tipsText
                width: parent.width - 100 * factor
                wrapMode: Text.WrapAnywhere
                text: qsTr("如果需要同一时段、空间下使用短跑和往返跑，则短跑信道为69，往返跑信道为169!")
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 62 * factor }
                anchors.top: maxDeviceText.bottom
                anchors.topMargin: 100 * factor
                anchors.horizontalCenter: parent.horizontalCenter
                visible: false
            }

            //提交按钮
            OtmCommitBtn{
                id: sureBtn
                displayText: "确 认"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50 * factor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Connections{
                target: modelSelectComboBox
                onSignalValueRefresh:{
                    curDevType = index + 1;
                    initChannelConfig(curDevType);
                    curChannel = channelChooseComboBox.comboBoxDisplayList[channelChooseComboBox.curComboBoxIndex] * 1;
                    curMaxDevNum = maxDevComboBox.comboBoxDisplayList[maxDevComboBox.curComboBoxIndex] * 1;
                }
            }

            Connections{
                target: channelChooseComboBox
                onSignalValueRefresh:{
                    curChannel = channelChooseComboBox.comboBoxDisplayList[index] * 1;
                }
            }

            Connections{
                target: maxDevComboBox
                onSignalValueRefresh:{
                    curMaxDevNum = maxDevComboBox.comboBoxDisplayList[index] * 1;
                }
            }
        }
    }

    Connections {
        target: sureBtn
        onSigBtnClicked: {
            //提交修改
            console.log("curChannel", curChannel)
            InterAction.setChannelConfig(curChannel, curMaxDevNum)
        }
    }

    Connections {
        target: InterAction
        onSigUpdateHWConfig: {
            if(aws)
            {
                tipsText.text = "设置成功，请等待设备重启"
                sureBtn.setInactive()
            }
            else
            {
                tipsText.text = "设置失败"
            }
            tipsText.visible = true;
        }

        onSigSetDevChannelInfo: {
            //获取配置信道号列表数据
            initChannelConfig(GlobalProperty.iDevType)

            //设置当前显示信道
            GlobalProperty.channel = channel
            curChannel = GlobalProperty.channel
            channelChooseComboBox.curComboBoxIndex = GlobalProperty.channel === channelChooseComboBox.comboBoxDisplayList[0] ? 0:1
        }

        onSigSetDevMaxConnectNum: {
            GlobalProperty.maxConnectCount = num
            curMaxDevNum = GlobalProperty.maxConnectCount
        }
    }

    function open(){
        m_popup.open()
        initChannelConfig(GlobalProperty.iDevType)
        InterAction.curPage = Enums.PAGE_SETTING
    }

    function close(){
        m_popup.close()
        InterAction.curPage = GlobalProperty.pageBeforoPopups
    }
}
