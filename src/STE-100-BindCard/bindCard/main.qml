import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import com.Company.TME 1.0
import "."


ApplicationWindow {
    flags: Qt.FramelessWindowHint | Qt.Window

    property real factor: 1
    property real screenX: Screen.width
    property real screenY: Screen.height
    visible: true
    width: screenX
    height: screenY + 1

    Component.onCompleted: {
        turnToPageUserBind()
    }

//    Button{
//        id: btn1
//        z:5
//        onClicked: {
//            ClassView.testStuView_cardBind()
//        }
//    }

//    Button{
//        z:5
//        anchors.top: btn1.bottom
//        anchors.topMargin: 5 * factor
//        anchors.horizontalCenter: btn1.horizontalCenter
//        onClicked: {
//            ClassView.testStuView_cardNotBind()
//        }
//    }

    Rectangle{
        id:backGround
        anchors.fill: parent
        color: "black"
    }

    TopTitle{
        id:schoolTitle
        schoolNameText: "成都泰盟小学"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        onSigTitleClicked:{
            topTitleClickedHandler( index )
        }
    }

    StackLayout{
        Component.onCompleted: {
            userBindPage.createObject( this )
            classBindPage.createObject( this )
            loginPage.createObject( this )
        }

        id: mainStackLayout
        width: parent.width
        height: parent.height - schoolTitle.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: schoolTitle.bottom
        onCurrentIndexChanged: {

        }
    }

    Component{
        id: userBindPage
        PageUserBind{

        }

    }

    Component{
        id: classBindPage
        PageClassBind{
        }
    }

    Component{
        id: loginPage
        PageLogoIn{
            id: login
            onSigLogSuccess:{
                ClassView.setLoginState( true )
                turnToPageClassBind()
            }
        }

    }

    function topTitleClickedHandler( index ){

        if( index === Enums.Page_Class_Bind && (!ClassView.loginState()) ){
            turnToPageLogin()
        }else if( index === Enums.Page_User_Bind ){
            turnToPageUserBind()
        }

        else if( index === Enums.Page_Class_Bind ){
            turnToPageClassBind()
        }


    }

    function turnToPageUserBind(){
        mainStackLayout.currentIndex = Enums.Page_User_Bind
        StudentView.setCurrentPage()
    }

    function turnToPageClassBind(){
        mainStackLayout.currentIndex = Enums.Page_Class_Bind
        ClassView.setCurrentPage()
    }

    function turnToPageLogin(){
        mainStackLayout.currentIndex = Enums.Page_Login
    }


}
