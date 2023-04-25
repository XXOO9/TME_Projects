import QtQuick 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../"

Item {
    id: root

    width: 1583 * factor
    height: 824 * factor
    state: "ready"

    property string displayContent: GlobalProperty.strCurRet

    property real shortCountTime: 10

    property QtObject rightAreaObj

    //状态变更时，如果是准备 状态，则将测试结果清零
    onStateChanged: {
        if( root.state === "ready" && GlobalProperty.devType === "VOLLEYBALL" ){
            GlobalProperty.strCurRet = '0'
        }
    }

    //测试阶段区域
    Item {
        id: testPhaseArea

        readonly property real rectRadius: 30 * factor

        readonly property string rectNormalColor:  "#6AB4F8"
        readonly property string rectWarningColor: "#B6A8C9"
        readonly property string rectFoulgColor:   "#B694D0"

        property string testPhaseTextContent: "准备测试"

        width: 594 * factor
        height: parent.height
        anchors{ left: parent.left; top: parent.top }

        Image {
            id: imgSound
            width: 104 * factor
            height: 87 * factor
            source: "qrc:/pic/15-sound.png"
            anchors.top: parent.top
            anchors.topMargin: 56 * factor
            anchors.left: parent.left
            anchors.leftMargin: 56 * factor
        }

//        Button{
//            visible: InterAction.isDeveloper
//            z: 5
//            onClicked: root.state = "ready"
//        }



        //测试阶段要展示出来的内容
        Rectangle{
            id: testPhaseRect
            anchors.fill: parent
            radius: testPhaseArea.rectRadius
            color: testPhaseArea.rectNormalColor
            Text {
                id:testPhaseText
                width: parent.width
                color: "white"
                text: testPhaseArea.testPhaseTextContent
                font{ family: "PingFang SC"; pixelSize: 100 * factor; bold: true }
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
            }

            //违例或者犯规的时候展示展示出来的图
            Image {
                id: specialPhase
                width: 233 * factor
                height: 300 * factor
                readonly property string foulImg: "qrc:/pic/TestPhase/foulImg.png"
                readonly property string warningImg: "qrc:/pic/TestPhase/warning.png"
                visible: false
                anchors.centerIn: testPhaseRect
                onVisibleChanged: testPhaseText.visible = !specialPhase.visible
            }
        }

        //三个小圆点
        Row{
            id: rowRectArea
            readonly property string yellowColor: "#F39800"
            readonly property string greenColor:  "#009944"
            readonly property string redColor:    "#BB0102"

            property string curColor: yellowColor

            spacing: 40 * factor
            anchors{ bottom: parent.bottom; bottomMargin: 90 * factor; horizontalCenter: parent.horizontalCenter }
            Repeater{
                model: 3
                delegate: Rectangle{
                    width: 40 * factor
                    height: width
                    radius: width / 2
                    color: "white"
                    Rectangle{
                        width: parent.width * 0.7
                        height: width
                        radius: width / 2
                        anchors.centerIn: parent
                        color: rowRectArea.curColor
                    }
                }
            }
        }
    }

    //对于短时间计时的项目，比如斜身引体向上，可用这个定时器计时
    Timer{
        id: shortTimer

        interval: 1000
        repeat: true
        onTriggered: {
            if( shortCountTime > 0 ){
                shortCountTime--
            }

            rightAreaObj.setDisplayReleaseCountTime( shortCountTime )
        }
    }

    //对于长时间计时的项目, 该定时器只是周期性去的取值,获取时间差值
    Timer{
        id: intervalTimer
        interval: 100
        repeat: true
        onTriggered: {
            shortCountTime -= InterAction.calculateTimeDifference( Enums.CALCULATETIMEDIFFERENCE )
            console.log( 'display time cnt-- = ' + shortCountTime )
        }
    }

    Connections {
        target: InterAction
        onSendTestStateChangeSignal:{
            let testState = InterAction.testState
            if(testState === "请准备")
            {
                state = "ready"

                //准备阶段将测试结果值清零
                GlobalProperty.strCurRet = '0'
            }
            else if(testState === "测试中")
            {
                if( GlobalProperty.devType === "VOLLEYBALL" ){
                    return
                }
                state = "testing"
            }
            else if(testState === "测试完成")
            {
                state = "finished"
            }
            else
            {
                if(testState.charAt("FOUL:"))
                    state = "foul"
            }
        }
    }

    states:[
        State{
            name: "ready"
            PropertyChanges{ target: testPhaseRect; color: testPhaseArea.rectNormalColor }
            PropertyChanges{ target: rowRectArea; curColor: rowRectArea.yellowColor }
            PropertyChanges{ target: specialPhase; visible: false }
            PropertyChanges{ target: testPhaseArea; testPhaseTextContent: "准备测试" }
        },

        State{
            name: "testing"
            PropertyChanges{ target: testPhaseRect; color: testPhaseArea.rectNormalColor }
            PropertyChanges{ target: rowRectArea; curColor: rowRectArea.greenColor }
            PropertyChanges{ target: specialPhase; visible: false }
            PropertyChanges{ target: testPhaseArea; testPhaseTextContent: "测试中" }
        },

        State{
            name: "finished"
            PropertyChanges{ target: testPhaseRect; color: testPhaseArea.rectNormalColor }
            PropertyChanges{ target: rowRectArea; curColor: rowRectArea.redColor }
            PropertyChanges{ target: specialPhase; visible: false }
            PropertyChanges{ target: testPhaseArea; testPhaseTextContent: "测试完成" }
        },

        //犯规
        State{
            name: "foul"
            PropertyChanges{ target: testPhaseRect; color: testPhaseArea.rectFoulgColor }
            PropertyChanges{ target: rowRectArea; curColor: rowRectArea.redColor }
            PropertyChanges{ target: specialPhase; visible: true; source: specialPhase.foulImg }
        },

        //违例
        State{
            name: "warning"
            PropertyChanges{ target: testPhaseRect; color: testPhaseArea.rectWarningColor }
            PropertyChanges{ target: rowRectArea; curColor: rowRectArea.yellowColor }
            PropertyChanges{ target: specialPhase; visible: true; source: specialPhase.warningImg }
        }

    ]

    //计时和计数区域
    Item {
        id: countTimeArea
        width: 960 * factor
        height: parent.height
        anchors{ top: parent.top; right: parent.right }

        function loadCorrespondingItem(){
            switch( GlobalProperty.devType ){
            case "FOOTBALL":
                loadCountTimeAreaOnly()
                break

            case "BASKETBALL":
                loadCountTimeAreaOnly()
                break

            case "VOLLEYBALL":
                loadCountTimeAndNumber()
                break

            case "PUO":
                loadCountTimeAndNumber( "请尽快完成动作", "斜身引体完成次数" )
                break

            case "SOLDBALL":
                loadCountTimeAreaOnly( "当前测试结果" , true )
                break

            case "VERTICALJUMP":
                loadCountTimeAreaOnly( "当前测试结果" , true )
                break

            default:
                loadCountTimeAreaOnly()
                break
            }
        }

        function loadCountTimeAreaOnly( titleString = "测试已进行时间" , unitVisible = false){
            rightAreaObj = countTimeOnlyCmp.createObject( countTimeArea, { "titleString": titleString , "unitVis": unitVisible} )
        }

        function loadCountTimeAndNumber( upString = "当前测试剩余时间", downString = "当前对墙垫球次数" ){
            rightAreaObj = countTimeAndcountNumberCmp.createObject( countTimeArea, { "upTitleString": upString, "downTitleString": downString } )
        }

        Component.onCompleted: {
            loadCorrespondingItem()
        }

        // 只计时的框
        Component{
            id: countTimeOnlyCmp
            Item {
                id: countTimeOnlyItem
                width: countTimeArea.width
                height: countTimeArea.height

                property string titleString: "测试已进行的时间"
                property alias unitVis: resultUnit.visible

                Rectangle{
                    anchors.fill: parent
                    radius: 30 * factor
                    color: "#6AB4F8"
                }

                Text {
                    text: countTimeOnlyItem.titleString
                    color: "#FFF100"
                    font{ family: "Microsoft YaHei"; pixelSize: 62 * factor; bold: true }
                    anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 90 * factor }
                }

                //实际计时的时间
                Text {
                    id: countTime
                    width: parent.width - resultUnit.width - 20 * factor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    text: displayContent
                    color: "#FFF100"
                    font{ family: "Rosenrot"; pixelSize: 250 * factor; wordSpacing: 150 * factor }
                    anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 316 * factor }
                }

                //当前测试的单位
                Text {
                    id: resultUnit
                    text: GlobalProperty.getUnitByDevType()
                    color: "#FFF100"
                    font.family: "Microsoft YaHei"
                    font.pixelSize: 62 * factor
                    font.wordSpacing: 100 * factor
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 90 * factor
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: false
                }
            }
        }

        //计数和计时的框
        Component{
            id: countTimeAndcountNumberCmp
            Item{
                id: countTimeAndcountNumberItem
                width: countTimeArea.width
                height: countTimeArea.height

                property real countDownTime: 40
                property real countNumber: isNaN( GlobalProperty.strCurRet * 1 ) ? '0' : GlobalProperty.strCurRet * 1

                property string upTitleString: "测试已进行的时间"
                property string downTitleString: "当前对墙垫球次数"

//                Button{
//                    visible: InterAction.isDeveloper
//                    z: 5
//                    onClicked: GlobalProperty.strCurRet = '1'
//                }

                onCountNumberChanged: {
                    if( GlobalProperty.devType === "VOLLEYBALL" && countNumber >= 1){
                        if( !shortTimer.running ){
                            root.state = "testing"
                        }
                    }
                }


                Rectangle{
                    anchors.fill: parent
                    radius: 30 * factor
                    color: "#6AB4F8"
                }

                Rectangle{
                    id: countTimeRect
                    width: 906 * factor
                    height: 371 * factor
                    color: "#4370CA"
                    radius: 23 * factor
                    anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 25 * factor }
                    Text {

                        text: countTimeAndcountNumberItem.upTitleString
                        color: "#FFF100"
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor; bold: true }
                        anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 64 * factor }
                    }


                    Text {
                        id: releaseCountTimeText
                        text: countTimeAndcountNumberItem.countDownTime + " s"
                        color: "#FFF100"
                        font{ family: "Rosenrot"; pixelSize: 160 * factor; bold: true }
                        anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 176 * factor }
                    }
                }

                Rectangle{
                    id: countNumberRect
                    width: 906 * factor
                    height: 371 * factor
                    color: "#4370CA"
                    radius: 23 * factor
                    anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 427 * factor }

                    Text {
                        id: upTitleText
                        text: countTimeAndcountNumberItem.downTitleString
                        color: "white"
                        font{ family: "Microsoft YaHei"; pixelSize: 62 * factor; bold: true }
                        anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 64 * factor }
                    }

                    Text {
                        text: countTimeAndcountNumberItem.countNumber + " 次"
                        color: "#4CD3F0"
                        font{ family: "Rosenrot"; pixelSize: 160 * factor; bold: true }
                        anchors{ horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 176 * factor }
                    }

                }

                function setDisplayReleaseCountTime( releaseTime ){
                    releaseCountTimeText.text = releaseTime + " s"
                }
            }
        }
    }

    function startTimer( running = true ){
        //短时间计时
        if( GlobalProperty.devType === "PUO"  || GlobalProperty.devType === "VOLLEYBALL" ){
            //开始计时
            if( running )
            {
                if(GlobalProperty.devType === "PUO" )
                {
                    shortCountTime = 10
                }
                else
                {
                    shortCountTime = 40
                }

                shortTimer.start()
            }
            //停止计时
            else{
                shortCountTime = 0
                rightAreaObj.setDisplayReleaseCountTime( shortCountTime )
                shortTimer.stop()
            }

            return
        }

        //长时间计时
        if( running ){
            InterAction.calculateTimeDifference( Enums.STARTORRESTARTCOUNTTIMER )
            intervalTimer.start()
        }else{
            intervalTimer.stop()
        }
    }
}
