import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Moudues/"
import "../"
import "."

CusSettingElementArea{
    id: root

    titleString: '测试过渡页: '

    Row{
        spacing: 80 * factor
        anchors{ left: root.left; leftMargin: root.leftOffset; top: root.top; topMargin: 152 * factor }
        Text {
            text: '开启测试过渡页: '
            color: 'white'
            font{ family: 'Microsoft YaHei'; pixelSize: 50 * factor; bold: true }
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: userInfoPageSwitch.verticalCenter
        }

        CustSwitch{
            id: userInfoPageSwitch
            width: 200 * factor
            height: 100 * factor
            switchWidth: width
            switchHeight: height
            checkedFalseText: ''
            checkedTrueText: ''
            state: 'noText'
            onIsCheckedChanged: sigConfigChanged()
        }
    }

    function setSwitchChecked( isRequireUserInfoPage ){
        userInfoPageSwitch.setSwitchState( isRequireUserInfoPage )
        GlobalProperty.userInfoPageExited = isRequireUserInfoPage
    }

    function getSwitchChecked(){
        return userInfoPageSwitch.isChecked
    }
}
