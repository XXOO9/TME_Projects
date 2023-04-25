import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../../Moudues"
import "../../Popups"
import "../../"
import "."

Item {
    id: root
    width: 1920 * factor
    height: 2630 * factor

    Component.onCompleted: {
        //2022/07/07 修改: 加载同步模式的时候，先读取提交配置(异步模式强制改变了提交模式，影响了GlobbalProperty。commit的值)
        InterAction.readCommitPopupConfigFromDataBase()
    }

    //蓝色背景
    Rectangle{
        anchors.fill: parent
        color: "#4756EB"
    }

    OtmHelpBtn{
        id: helpBtn
        z:5
        anchors.left: parent.left
        anchors.leftMargin: -30 * factor
        anchors.top: parent.top
        anchors.topMargin: 520 * factor
    }

    OtmAddUserBtn{
        id: userAddBtn
        z:5
        anchors.verticalCenter: helpBtn.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: -30 * factor
    }

    //人脸识别区域
    OtmFaceRecogView{
        id: faceRecog
        width: 1002 * factor
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 109 * factor
    }

    //显示已经绑定好了的人数
    Text {
        id: bindUserCountText
        text: "已绑定: " + InterAction.multiUserCount + "人"
        font{ family: "Microsoft YaHei"; pixelSize: 70 * factor; bold: true }
        color: "white"
        anchors.left: recogedUserTableRect.left
        anchors.bottom: recogedUserTableRect.top
        anchors.bottomMargin: 50 * factor
    }

    OtmTableBackground{
        id: recogedUserTableRect
        width: 1760 * factor
        height: 990 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:faceRecog.bottom
        anchors.topMargin: 170 * factor
        rectColor: "#9585F7"
        titleBackgroundColor: "qrc:/pic/16.png"
        CustStuTableView{
            id: syncStudentTable
            tableType: GlobalProperty.syncStudentTable
            width: 1700 * factor
            height: 820 * factor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * factor
            tableCount: 5
            widthArray: [0.15, 0.2, 0.25, 0.2, 0.2, 0]
            headName: ["设备号", "姓名", "准考证号", "设备状态", "操作", ""]
            cellTypeArray: [GlobalProperty.normalCell, GlobalProperty.normalCell, GlobalProperty.normalCell,
                             GlobalProperty.deviceCell, GlobalProperty.deleteCell, GlobalProperty.normalCell]
        }
    }

    //开始测试按钮
    OtmCommitBtn{
        id: startTestBtn
        state: "inactive"
        displayText: "开始测试"
        anchors.top: recogedUserTableRect.bottom
        anchors.topMargin: 100 * factor
        anchors.horizontalCenter: parent.horizontalCenter
        onSigBtnClicked: {
            //跳转页面
            turnToPageTestingPage()
            //点击事件
            InterAction.procBtnClicked(Enums.OTM_START_TEST_BTN)
        }
    }

    BindDeviceTipPopup {
        id: bindGuidePopup
        anchors.centerIn: parent
    }

    UnbindConfirmPopup {
        id: unBindCPoup
        anchors.centerIn: parent
    }

    Connections{
        target: syncStudentTable
        onSigUnbindBtnClicked: {
            unBindCPoup.open(index)
        }
    }

    Connections {
        target: unBindCPoup
        onSigOKBtnClicked: {
            InterAction.unBindStudent(index)
            if(commonDataModel.rowCount()<= 0)
            {
                startTestBtn.setInactive()
            }
        }
    }

    //响应交互类的信号
    Connections {
        target: InterAction
        //人员识别成功 开启nfc刷卡倒计时
        onUpdateNFCRecogTime: {
            if(time>0)
            {
                bindGuidePopup.nfcRecogSecond = time / 1000
                bindGuidePopup.show()
            }
        }

        //结束刷卡倒计时
        onSigStopNFCRecogTimer: {
            bindGuidePopup.hide()
        }

        //后端控制开始测试按钮是否可点
        onSigSetBtnEnable: {
            if(btnType === Enums.OTM_START_TEST_BTN)
            {
                startTestBtn.state = bEnable ? "active":"inactive"
            }
        }
    }

}
