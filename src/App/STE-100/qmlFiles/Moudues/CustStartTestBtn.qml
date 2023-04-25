import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ".."

Item {
    id : root

    width: 1582 * factor
    height: 760 * factor

    property bool bClicked: true

    signal signalTouristTestModel()
    signal signalStudentTestModel()

    readonly property string footballTesterImg: ""
    readonly property string basketballTesterImg: ""
    readonly property string volleyballTesterImg: ""
    readonly property string puoTesterImg: ""
    readonly property string soldballTesterImg: ""
    readonly property string verticalJumpTesterImg: ""


    Component.onCompleted: {
        console.log("devType", GlobalProperty.devType)

        loadCorrespondingStartBtnView()
    }


    //倒计时圈圈不在这个 qml 里面
    Rectangle{
        id: baseBtn
        visible: false
        color: "#4AA4C9"
        radius: 30 * factor
        anchors.fill: parent

        //文字 "开始测试"
        Text {
            id: strStartTestText
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }

        //文字 "start test"
        Text {
            id: strStartTestEnglishText
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestText.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestText.left
        }

        //动漫人物造型
        Image {
            id: baseTesterImg
            width: 420 * factor
            height: 975 * factor
            source: "qrc:/pic/FHLandBody.png"
            anchors.right: parent.right
            anchors.rightMargin: 532 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -100 * factor
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }

    /***********************1.开始测试按钮( 身高体重人体成分 )*******************************/
    Rectangle{
        id : rectStartTestBtnBODY
        visible: GlobalProperty.checkVisible( "BODY" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#4AA4C9"
        anchors.fill: parent
        Text {
            id: strStartTestBODYINFO
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishBODYINFO
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestBODYINFO.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestBODYINFO.left
        }
        Image {
            width: 400 * factor
            height: 975 * factor
            source: "qrc:/pic/FHLandBody.png"
            anchors.right: parent.right
            anchors.rightMargin: 532 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -100 * factor
        }

        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }
                else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }

    /***********************1.开始测试按钮( 肺活量 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnFHL
        visible: GlobalProperty.checkVisible( "FHL" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#4AA4C9"
        anchors.fill: parent
        Text {
            id: strStartTestFHL
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishFHL
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestFHL.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestFHL.left
        }
        Image {
            width: 400 * factor
            height: 975 * factor
            source: "qrc:/pic/FHLandBody.png"
            anchors.right: parent.right
            anchors.rightMargin: 532 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -100 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }
    /***********************1.开始测试按钮( 肺活量 )***********end***********/



    /***********************2.开始测试按钮( 立定跳远 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnLDTY
        visible: GlobalProperty.checkVisible( "LDTY" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#DE6C41"
        anchors.fill: parent
        Text {
            id: strStartTestLDTY
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishLDTY
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestLDTY.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestLDTY.left
        }
        Image {
            width: 1072 * factor
            height: 803 * factor
            source: "qrc:/pic/ty.png"
            anchors.right: parent.right
            anchors.rightMargin: 166 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -30 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }
    /***********************2.开始测试按钮( 立定跳远 )*********end***********************/

    /***********************3.开始测试按钮( 坐位体前屈 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnZWTQQ
        visible: GlobalProperty.checkVisible( "ZWTQQ" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#8288FD"
        anchors.fill: parent
        Text {
            id: strStartTestZWTQQ
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishZWTQQ
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestZWTQQ.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestZWTQQ.left
        }
        Image {
            width: 966 * factor
            height: 756 * factor
            source: "qrc:/pic/ZWTQQ.png"
            anchors.right: parent.right
            anchors.rightMargin: 307 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -30 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }
    /***********************3.开始测试按钮( 坐位体前屈 )*********end***********************/

    /***********************4.开始测试按钮( 引体向上 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnYTXS
        visible: GlobalProperty.checkVisible( "YTXS" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#3F8BE5"
        anchors.fill: parent
        Text {
            id: strStartTestYTXS
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishYTXS
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestYTXS.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestYTXS.left
        }
        Image {
            width: 633 * factor
            height: 1023 * factor
            source: "qrc:/pic/YTXS.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: -110 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }
    /***********************4.开始测试按钮( 引体向上 )*********end***********************/

    /***********************5.开始测试按钮( 仰卧起坐 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnYWQZ
        visible: GlobalProperty.checkVisible( "YWQZ" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#1EA08E"
        anchors.fill: parent
        Text {
            id: strStartTestYWQZ
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishYWQZ
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestYWQZ.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestYWQZ.left
        }
        Image {
            width: 1239 * factor
            height: 626 * factor
            source: "qrc:/pic/YWQZ.png"
            anchors.right: parent.right
            anchors.rightMargin: 52 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 81 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }
    /***********************5.开始测试按钮( 仰卧起坐 )*********end***********************/


    /************************6.握力计开始测试按钮************start***************************/
    Rectangle{
        id : rectStartTestBtnROPEGPIP
        visible: GlobalProperty.checkVisible( "ROPEGPIP" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#5BA46C"
        anchors.fill: parent
        Text {
            id: strStartTestROPEGPIP
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishROPEGPIP
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestROPEGPIP.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestROPEGPIP.left
        }
        Image {
            width: 511 * factor
            height: 756 * factor
            source: "qrc:/pic/grip.png"
            anchors.right: parent.right
            anchors.rightMargin: 52 * factor
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 81 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }

    /***********************7.开始测试按钮( 俯卧撑测试 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnPushup
        visible: GlobalProperty.checkVisible( "PUSHUP" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#DE7B6F"
        anchors.fill: parent
        Text {
            id: strStartTestPushup
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishPushup
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestPushup.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestPushup.left
        }
        Image {
            width: 1407 * factor
            height: 769 * factor
            source: "qrc:/pic/pushUp.png"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -30 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }

    /***********************8.开始测试按钮( 视力测试 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnVision
        visible: GlobalProperty.checkVisible( "VISION" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#3788E8"
        anchors.fill: parent
        Text {
            id: strStartTestVision
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishVision
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestVision.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestVision.left
        }
        Image {
            width: 657 * factor
            height: 996 * factor
            source: "qrc:/pic/vision.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: -110 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }

    /***********************8.开始测试按钮( 肺通气量 )*********strat***********************/
    Rectangle{
        id : rectStartTestBtnPVV
        visible: GlobalProperty.checkVisible( "PVV" )
        width: 1582 * factor
        height: 760 * factor
        radius: 30 * factor
        color: "#3F8BE5"
        anchors.fill: parent
        Text {
            id: strStartTestPVV
            text: qsTr( "开始测试" )
            font.pixelSize: 84 * factor
            font.family: "MF DianHei (Noncommercial)"
            color: "white"
            anchors.top: parent.top
            anchors.topMargin: 90 * factor
            anchors.left: parent.left
            anchors.leftMargin: 90 * factor
        }
        Text {
            id: strStartTestEnglishPVV
            text: qsTr( "Start Test" )
            font.pixelSize: 56 * factor
            font.family: "MF YueYuan (Noncommercial)"
            font.bold: true
            color: "white"
            opacity: 0.5
            anchors.top: strStartTestPVV.bottom
            anchors.topMargin: 30 * factor
            anchors.left: strStartTestPVV.left
        }
        Image {
            width: 633 * factor
            height: 1023 * factor
            source: "qrc:/pic/YTXS.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: -110 * factor
        }
        MouseArea{
            enabled: bClicked
            anchors.fill: parent
            onClicked: {
                //判断是否为游客模式
                if( GlobalProperty.bIsTouristModel ){
                    //开始游客测试模式
                    signalTouristTestModel()
                }else{
                    //开始学生测试模式
                    signalStudentTestModel()
                }
            }
        }
    }

    function loadCorrespondingStartBtnView(){
        baseBtn.visible = true
        console.log( "loadCorrespondingStartBtnView() devType=  " + GlobalProperty.devType )
        switch( GlobalProperty.devType ){
        case "FOOTBALL":
            loadFootballStartBtn()
            break

        case "BASKETBALL":
            loadBasketBallStartBtn()
            break

        case "VOLLEYBALL":
            loadVolleyBallStartBtn()
            break

        case "PUO":
            loadPUOStartBtn()
            break

        case "SOLDBALL":
            loadSoldBallStartBtn()
            break

        case "VERTICALJUMP":
            loadVerticalJumpStartBtn()
            break
        default:
            baseBtn.visible = false
            break
        }
    }

    function loadFootballStartBtn(){
        baseTesterImg.source = "qrc:/pic/HomePage/footBall.png"
    }

    function loadBasketBallStartBtn(){
        baseTesterImg.source = "qrc:/pic/HomePage/basketball.png"
    }

    function loadVolleyBallStartBtn(){
        baseTesterImg.source = "qrc:/pic/HomePage/volleyball.png"
    }

    function loadPUOStartBtn(){
        baseTesterImg.source = "qrc:/pic/HomePage/PUO.png"
    }

    function loadSoldBallStartBtn(){
        baseTesterImg.source = "qrc:/pic/HomePage/soldBall.png"
    }

    function loadVerticalJumpStartBtn(){
        baseTesterImg.source = "qrc:/pic/HomePage/verticalJump.png"
    }
}
