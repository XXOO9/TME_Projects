import QtQuick 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0


Item {
    id: root
    width: 1920 * factor
    height: 1080 * factor
    z:10

    property string account: ""
    property string passwd: ""
    property string backColor: "#020213"
    property string fontStyle: "Microsoft YaHei"
    property string cancelLogin: "取消"
    property string comfirm: "确认"

    signal sigLogSuccess()

    state: "logining"

    Rectangle{
        id: bkgd
        anchors.fill: root
        color: backColor
    }

    Image {
        id: title
        width: 1115 * factor
        height: 70 * factor
        source: "qrc:/Pic/title.png"
        anchors.right: parent.right
        anchors.rightMargin: 355 * factor
        anchors.top: parent.top
        anchors.topMargin: 130 * factor
    }

    //登录框
    Rectangle{
        id: loginFrame
        width: 687 * factor
        height: 509 * factor
        color: "transparent"

        anchors.right: parent.right
        anchors.rightMargin: 600 * factor
        anchors.top: title.bottom
        anchors.topMargin: 100 * factor

        Image {
            id: loginBkgd
            anchors.fill: parent
            source: "qrc:/Pic/loginImg.png"
        }

        Image {
            id: subTitleUserLogin
            width: 145 * factor
            height: 31 * factor
            source: "qrc:/Pic/userLogin.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 77 * factor
        }

        //账号输入框
        UserIdInput{
            id: userAccountInput
            width: 500 * factor
            height: 60 * factor
            tipsString: ""
//            content: "web01"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 158 * factor

            Image {
                width: 25 * factor
                height: width
                visible: (userAccountInput.textLength || userAccountInput.isFocus) > 0 ? false : true
                source: "qrc:/Pic/head.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25 * factor
            }
        }

        //密码输入框
        UserIdInput{
            id: passwdInput
            width: userAccountInput.width
            height: userAccountInput.height
            tipsString: ""
//            content: "123"
            inputType: Enums.PASSWD
            anchors.left: userAccountInput.left
            anchors.top: userAccountInput.bottom
            anchors.topMargin: 30 * factor

            Image {
                width: 25 * factor
                height: width
                visible: (passwdInput.textLength || passwdInput.isFocus) > 0 ? false : true
                source: "qrc:/Pic/lock.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25 * factor
            }
        }

        //登录按钮
        Rectangle{
            id: loginBtn
            width: userAccountInput.width
            height: userAccountInput.height
            color: "#16796F"
            anchors.horizontalCenter: userAccountInput.horizontalCenter
            anchors.top: passwdInput.bottom
            anchors.topMargin: 50 * factor
            Text {
                text: "登录"
                color: "white"
                font{ family: fontStyle; pixelSize: 26 * factor }
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    onLoginBtnClicked()
                }
            }
        }

    }

    Popup{
        id: m_pop

        property int arrIndex: 0
        property int maxWaitSecs: 5000
        property int singleStep: 1000
        property var waitText: ["登录中.", "登录中..", "登录中..."]

        width: 200 * factor
        height: 150 * factor
        modal: true
        anchors.centerIn: parent
        closePolicy: Popup.NoAutoClose
        padding: 0

        Rectangle{
            anchors.fill: parent
            color: "#040B1B"
            Text {
                id: logStateText
                text: m_pop.waitText[m_pop.arrIndex]
                color: "white"
                font{ family: fontStyle; pixelSize: 30 }
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 30 * factor
            }
            Rectangle{
                id: logOperateBtn
                width: 120 * factor
                height: 40 * factor
                color: "#409EFF"

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20 * factor
                radius: 30 * factor
                Text {
                    id: operateBtnText
                    text: cancelLogin

                    font{family: "Microsoft YaHei"; pixelSize: 20 * factor}
                    anchors.centerIn: parent

                }
                MouseArea{
                    id: logOperateBtnMouseArea
                    anchors.fill: parent
                    onClicked: {
                        onOperateBtnClicked()
                        //                        operateBtnText.text = "SSSSSSSSSSSS"
                    }
                }
            }
        }

        NumberAnimation on arrIndex{
            id: loginAnimation
            running: false
            from: 0
            to: 2
            duration: m_pop.singleStep
            loops: m_pop.maxWaitSecs / m_pop.singleStep

            //如果达到最大等待时间还没有得到结果，则显示超时
            onFinished: {
                root.state = "logTimeout"
            }
        }

        //关闭弹窗的定时器
        Timer{
            id: closeCountTimer
            interval: 2000
            running: false
            repeat: false
            onTriggered: {
                m_pop.close()
                sigLogSuccess()
            }
        }
    }

    states: [
        State {
            name: "logining"
            PropertyChanges {
                target: operateBtnText
                text: cancelLogin
            }
        },
        State {
            name: "loginSuccess"
            PropertyChanges {
                target: logOperateBtn
                visible: false
            }
            PropertyChanges {
                target: logStateText
                text: "登录成功"
            }
        },

        State{
            name: "logFailed"
            PropertyChanges {
                target: logStateText
                text: "密码错误"
            }
            PropertyChanges{
                target: operateBtnText
                text: comfirm
            }
        },
        State{
            name: "logTimeout"
            PropertyChanges{
                target: logStateText
                text: "登录超时"
            }
            PropertyChanges{
                target: operateBtnText
                text: comfirm
            }
        }
    ]

    Component{
        id: accountIcon
        Image {
            source: "qrc:/Pic/head.png"
        }
    }

    Connections{
        target: ClassView
        onSigLoginRetAvailable:{
            loginAnimation.stop()
            if( ret ){
                onLogSuccess()
            }else{
                onLogFailed()
            }
        }
    }

    function onLoginBtnClicked(){
        m_pop.open()
        loginAnimation.start()
        account = userAccountInput.content
        passwd = passwdInput.content
        root.state = "logining"
        ClassView.setAccount( account )
        ClassView.setPasswd( passwd )
        ClassView.tryLogin()
    }

    function onOperateBtnClicked(){
        if( root.state === "logining" ){
            onLoginingClicked()
            return
        }

        if( root.state === "logTimeout" ){
            console.log( " logTimeout clicked" )
            onLogTimeoutClicked()
            return
        }

        if( root.state === "logFailed" ){
            onLogFailedClicked()
            return
        }
    }

    function onLoginingClicked(){
        console.log( " logining clicked" )
        ClassView.cancelLogin()
        m_pop.close()
    }

    function onLogTimeoutClicked(){
        m_pop.close()
    }

    function onLogFailedClicked(){
        m_pop.close()
    }

    function onLogSuccess(){
        root.state = "loginSuccess"
        closeCountTimer.start()
    }

    function onLogFailed(){
        root.state = "logFailed"
    }

}
