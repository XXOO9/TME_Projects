import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common
import ".."
import "../Moudues/"
import "../Popups"

Item {
    id: root
    width: 1245 * factor
    height: 1952 * factor

    Rectangle{
        id: bkgRect
        anchors.fill: parent
        color: '#3241CC'
        radius: 30 * factor
    }

    CusSettingElementArea{
        id: blockHouseArea
        showTopLine: false
        titleString: '地堡信息配置:'
        width: parent.width
        height: parent.height

        TextField{
            id: blockHouseChannelText
            width: 660 * factor
            height: 110 * factor
            color: 'white'
            selectByMouse: true
            selectedTextColor: 'white'
            placeholderText: '输入1~189 范围以内的值'
            validator: RegExpValidator{ regExp: /^[1-9]\d{0,1}$|^1[0-9]{2}$|^189$/ }
            font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
            anchors{ left: parent.left; leftMargin: blockHouseArea.leftOffset; top: parent.top; topMargin: 150 * factor }
            background: Rectangle{
                color: Common.colorTransparent
                radius: 20 * factor
                border{ width: 3 * factor; color: '#FFFFFF'; }
            }
        }

        Column{
            Component.onCompleted: {
                clickedBtnCmp.createObject( this, { 'btnTextString': '设置信道' } )
                clickedBtnCmp.createObject( this, { 'btnTextString': '清除信道' } )
            }
            spacing: 80 * factor
            anchors{ top: blockHouseChannelText.bottom; left: blockHouseChannelText.left; topMargin: 20 * factor }
        }
    }

    Component{
        id: clickedBtnCmp
        Rectangle{
            id: btnRect
            property alias btnTextString: btnText.text
            width: 300 * factor
            height: 100 * factor
            radius: 20 * factor
            color: '#1F71CF'
            Text {
                id: btnText
                text: clickedBtnCmp.btnText
                anchors.centerIn: parent
                color: 'white'
                font{ family: Common.fontYaHei; pixelSize: 45 * factor; bold: true }
            }

            MouseArea{
                anchors.fill: parent
                enabled: true
                hoverEnabled: true
                onReleased: btnRect.opacity = 1
                onPressed: btnRect.opacity = 0.7
            }

            function blockHouseBtnClicked(){
                if( index === 0 ){
                    refreshBlockHouseChannel()
                }else{
                    clearBlockHouseChannel()
                }
            }
        }
    }

    //更新地堡信道值
    function refreshBlockHouseChannel(){

    }

    //清除地堡信道值
    function clearBlockHouseChannel(){

    }

    //进入地堡信道配对模式, true 进入配对模式, false 终止配对模式
    function startBlockHousePairModel( readyPair ){

    }
}
