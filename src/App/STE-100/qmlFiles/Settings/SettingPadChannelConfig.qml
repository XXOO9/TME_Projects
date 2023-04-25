import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ProjectEnums 1.0
import "../../qmlFiles/JavaScript/ProjectValue.js" as Common
import ".."
import "../Moudues/"
import "../Popups"

Item{
    id: testTypeConfig


    ListModel{

        id: devTypeListModel

        ListElement{ fieldName: '111' }
        ListElement{ fieldName: '222' }
    }

    Rectangle{
        width: parent.width
        height: 3 * factor
        color: '#2B85D9'
        anchors{ top: parent.top; left: parent.left }
    }

    Column{
        spacing: 80 * factor

        Text {
            text: '信道配置: '
            color: 'white'
            font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
        }

        Row{
            spacing: 40 * factor
            Text {
                text: '主机信道选择: '
                color: 'white'
                font{ family: Common.fontYaHei; pixelSize: 50 * factor; bold: true }
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: testTypeSelect.verticalCenter
            }

            CusSelectCombobox{
                id: testTypeSelect
                width: 660 * factor
                height: 110 * factor
                bkgRectColor: 'transparent'
                displayContentTextColor: 'white'
                bkgRectBorderColor: 'white'
                bkgRectBorderWidth: 3 * factor
                selectContentColor: '#4756EB'
                selectContentHighLightColor: '#1ADDFC'
                selectContentBorderColor: 'white'
                localFactor: factor
                itemHeight: 110 * factor
                itemTextColor: 'white'
                model: devTypeListModel
            }
        }
    }
}
