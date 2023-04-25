import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectEnums 1.0
import "../"
import "../Moudues"

Item {
    id: root
    width: 1760 * factor
    height: 2530 * factor

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

        OtmKeyBord{
            id: keyBordView
            anchors.centerIn: parent
        }
    }

    Connections{
        target: keyBordView
        onSigConfirmBtnClicked:{
            onKeyBordInputFinished( content )
        }
    }

    function open(){
        //InterAction.curPage = Enums.PAGE_SETTING
        InterAction.procAddUserID(true, "");
        m_popup.open()
    }

    function close(){
        //InterAction.curPage = GlobalProperty.pageBeforoPopups
        InterAction.procAddUserID(false, "");
        keyBordView.clear()
        m_popup.close()

    }

    function onKeyBordInputFinished( content ){
        if( content === "123456*" ){
            //管理员后台密码
            closeKeyBord()
            openSystem()
            return
        }
        else
        {
            InterAction.procAddUserID(true, content)
            close()
        }
    }
}
