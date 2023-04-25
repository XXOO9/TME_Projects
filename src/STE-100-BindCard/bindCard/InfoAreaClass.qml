import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import com.Company.TME 1.0

Item {
    id: root
    implicitWidth: 1120 * factor
    implicitHeight: 910 * factor

    state: "noCard"

    property string fontYaHei: "Microsoft YaHei"
    property alias titleText: tipsText.text
    property string curGender: "男"
    property string userIdPhoto: ""
    property int tableIndex: 0

    Component.onCompleted: {
        fillClassModel()
    }

    ListModel{
        id: classInfoList
    }

    ListModel{
        id: stuInfoList
    }


    Rectangle{
        id: stuInfoArea
        anchors.fill: parent
        radius: 10 * factor
        color: "#171D33"
        //分割线
        Rectangle{
            width: stuInfoArea.width
            height: 2 * factor
            color: "#2B384F"
            anchors.top: stuInfoArea.top
            anchors.topMargin: 100 * factor
            anchors.left: stuInfoArea.left
        }
        Text {
            id: tipsText
            text: "刷卡学生基本信息"
            color: "white"
            font{ pixelSize: 26 * factor; family: fontYaHei; bold: true}
            anchors.top: stuInfoArea.top
            anchors.topMargin: 42 * factor
            anchors.left: stuInfoArea.left
            anchors.leftMargin: 40 * factor
        }
    }

    //班级选择
    CustComboBox{
        id:gradeSelectComboBox
        dataModel: classInfoList
        //        dataModel: ["first", "second", "third", "forth", "fifth" ]
        anchors.top: parent.top
        anchors.topMargin: 140 * factor
        anchors.right: parent.right
        anchors.rightMargin: 778 * factor
    }

    Text {
        id: tipsTextClassName
        text: "年级班级:"
        color: "white"
        font{ pixelSize: 16 * factor; family: fontYaHei }
        anchors.verticalCenter: gradeSelectComboBox.verticalCenter
        anchors.right: gradeSelectComboBox.left
        anchors.rightMargin: 12 * factor
    }

    //学号输入框
    UserIdInput{
        id: userId
        parentPage: Enums.Page_Class_Bind
        anchors.left: gradeSelectComboBox.left
        anchors.top: gradeSelectComboBox.bottom
        anchors.topMargin: 30 * factor
    }

    Text {
        id: tipsUserId
        text: "学      号:"
        color: "white"
        font{ pixelSize: 16 * factor; family: fontYaHei }
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: tipsTextClassName.horizontalCenter
        anchors.verticalCenter: userId.verticalCenter
    }



    Text {
        id: tipsUserName
        text: "姓       名:"
        color: "white"
        font{ pixelSize: 16 * factor; family: fontYaHei }
        anchors.verticalCenter: userName.verticalCenter
        anchors.right: userName.left
        anchors.rightMargin: 12 * factor
    }

    //姓名输入框
    UserIdInput{
        id: userName
        tipsString: "请输入姓名"
        inputType: Enums.STUDENTNAME
        anchors.top: userId.bottom
        anchors.topMargin: 30 * factor
        anchors.right: parent.right
        anchors.rightMargin: 778 * factor
    }

    Text {
        id: tipsGender
        color: "white"
        text: "性       别:       " + curGender
        font{ pixelSize: 16 * factor; family: fontYaHei }
        anchors.left: tipsUserName.left
        anchors.top: parent.top
        anchors.topMargin: 363 * factor
    }

    //学生头像
    Rectangle{
        id: userPhoto
        implicitWidth: 202 * factor
        implicitHeight: width
        radius:  width / 2
        color: "gray"
        anchors.top: parent.top
        anchors.topMargin: 140 * factor
        anchors.right: parent.right
        anchors.rightMargin: 342 * factor
        Image {
            id: userImg
            visible: false
            anchors.fill: parent
            source: userIdPhoto
        }

        Rectangle{
            id: maskRect
            width: parent.width
            height: parent.height
            anchors.fill: parent
            color: "red"
            radius: width / 2
            visible: false
        }

        OpacityMask{
            anchors.fill: parent
            maskSource: maskRect
            source: userImg
        }
    }



    //刷卡区
    CardArea{
        id: cardArea
        anchors.left: parent.left
        anchors.leftMargin: 346 * factor
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 147 * factor
    }

    Text {
        id: textCardState
        text: "当前刷卡状态"
        color: "#AFB1B8"
        font{ pixelSize: 20 * factor; family: fontYaHei }
        anchors.left: parent.left
        anchors.leftMargin: 40 * factor
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 451 * factor
    }

    //绑定按钮
    BindCardBtn{
        id: bindBtn
        anchors.horizontalCenter: cardArea.horizontalCenter
        anchors.top: cardArea.bottom
        anchors.topMargin: 30 * factor
    }

    Connections{
        target: ClassView
        //查询到人员信息后，将信息填充到界面上
        onSigNewDisplayStuInfoAvailable:{
            whileNewDisplayTextAvailable( jsonString )
        }

        //检测到NFC卡的状态发生改变
        onSigDecetedCardStatedChanged:{
            whileDecetedCardStateChanged( cardState )
        }
    }

    Connections{
        target: bindBtn
        onClicked:{
            onBindBtnClicked()
        }
    }

    states: [
        //没有检测到NFC卡
        State {
            name: "noCard"
            PropertyChanges {
                target: cardArea
                state: "noCard"
            }
            PropertyChanges {
                target: bindBtn
                state: "noCard"
            }
        },
        //检测到NFC卡，但是没有绑定人员信息
        State {
            name: "readyBind"
            PropertyChanges {
                target: cardArea
                state: "existCard"
            }
            PropertyChanges {
                target: bindBtn
                state: "readyBind"
            }
        },
        //检测到NFC卡，已经绑定人员信息
        State {
            name: "disBind"
            PropertyChanges {
                target: cardArea
                state: "existCard"
            }
            PropertyChanges {
                target: bindBtn
                state: "disBind"
            }
        }
    ]

    //填充班级信息
    function fillClassModel(){
        for( var i = 0; i < ClassDataModel.rowCount(); i++ ){
            var className = ClassDataModel.data( i )
            //            console.log( "class name = " + className )
            classInfoList.append( {"name": className} )
        }
    }

    function fillStuModel(){
        for( var i = 0; i < StuDataModel.size(); i++ ){
            var stuName = StuDataModel.data( i )
            //            console.log( "stuName  = " + stuName )
            stuInfoList.append( { "name": stuName } )
        }
    }

    function whileNewDisplayTextAvailable( jsonString ){
        var tmp = JSON.parse( jsonString )
        gradeSelectComboBox.setCurClassName( tmp.className )
        userName.setCurDisplayText( tmp.name )
        ClassView.setStuCardNum( tmp.cardNo )
        userIdPhoto = tmp.avatarUrl
        curGender = tmp.gender
    }

    function whileDecetedCardStateChanged( cardState ){
        if( cardState === Enums.Exist_Card_Bind ){
            root.state = "disBind"
            return
        }

        if( cardState === Enums.Exist_Card_UnBind ){
            root.state = "readyBind"
            return
        }
        if( cardState === Enums.NO_Card ){
            root.state = "noCard"
            return
        }
    }

    function onBindBtnClicked(){

        //如果NFC卡是已绑定状态，点击则解绑
        if( root.state === "disBind" ){
            cardDisBind()
            return
        }

        //如果NFC卡是未绑定状态,则进行绑定
        if( root.state === "readyBind"){
            cardBind()
        }

        if( userId.textLength <= 0 || userName.textLength <= 0 || gradeSelectComboBox.curText.length <= 0 ){
            return
        }
    }

    //解除NFC绑定
    function cardDisBind(){
        root.state = "readyBind"
        ClassView.disBindUserCard( ClassView.stuId() )
    }

    //绑定新的卡号
    function cardBind(){
        root.state = "disBind"
        ClassView.updateUserCardNumber( ClassView.stuId(), StudentView.cardNum() )
        var className = gradeSelectComboBox.curText
        var id = userId.content
        var name = userName.content
        ClassView.appendNewRow( (++tableIndex).toString() + "+" + className + "+" + name + "+" + id )
    }
}



