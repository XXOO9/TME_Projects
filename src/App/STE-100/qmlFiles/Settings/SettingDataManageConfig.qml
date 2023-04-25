import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common
import ".."
import "../Moudues/"
import "../Popups"

Item{
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
        anchors.fill: parent
        titleString: '数据导入导出'
        showTopLine: false


        Column{
            spacing: 100 * factor
            anchors{ top: parent.top; topMargin: 183 * factor; horizontalCenter: parent.horizontalCenter }
            Repeater{
                model: [ '数据导入', '数据导出' ]
                delegate: cusClickedBtn
            }
        }
    }

    Component{
        id: cusClickedBtn
        Rectangle{
            id: btnRect
            width: 509 * factor
            height: 110 * factor
            radius: 20 * factor
            color: 'transparent'
            border{ width: 3 * factor; color: '#FFFFFF' }

            Row{
                id: row
                spacing: 28 * factor
                anchors.centerIn: parent
                Image {
                    width: 52 * factor
                    height: width
                    source: row.sourceProvider( index )
                }

                Text {
                    text: modelData
                    color: 'lightgray'//'#FFFFFF'
                    font.strikeout: true
                    font{ family: 'Microsoft YaHei'; pixelSize: 50 * factor; }
                    anchors.verticalCenter: parent.verticalCenter
                }

                states: [
                    State{
                        when: rectMouse.pressed
                        PropertyChanges{ target: btnRect; color: '#1ADDFC' }
                    }

                ]

                function sourceProvider( index ){
                    if( index === 0 ){
                        return 'qrc:/pic/settings/import.png'
                    }else{
                        return 'qrc:/pic/settings/outport.png'
                    }
                }
            }

            MouseArea{
                id: rectMouse
                anchors.fill: parent
                enabled: false
                onClicked: {
                    if( index === 0 ){
                        importData()
                    }else{
                        outportData()
                    }
                }
            }
        }
    }

    function importData(){
        console.log( 'import' )
    }

    function outportData(){
        console.log( 'outport' )
    }
}
