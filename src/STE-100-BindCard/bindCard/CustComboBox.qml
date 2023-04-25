import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    width: 220 * factor
    height: 40 * factor
    property var dataModel: [ "1", "2", "3", "4", "5" ]
    property alias curText: curDisplayText.text

    ComboBox{
        id: m_comboBox
        implicitWidth: 220 * factor
        implicitHeight: 40 * factor
        model: dataModel

        background: Rectangle{
            width: m_comboBox.width
            height: m_comboBox.height
            radius: 5 * factor
            color: "transparent"
            border.color: "#23BDC2"
        }

        contentItem: Text {
            id: curDisplayText
            text: m_comboBox.displayText
            color: "#23BDC2"
            font{ pixelSize: 30 * factor; family: "Microsoft YaHei"}
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }


        delegate: ItemDelegate{
            width: m_comboBox.width
            contentItem: Text {
                anchors.fill: parent
                padding: 0
                text: modelData
                elide: Text.ElideRight
                color: "white"
                font{ pixelSize: 25 * factor; family: "Microsoft YaHei"}
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle{
                anchors.fill: parent
                //            color: highlighted ? "#2C77B4" : "transparent"
                color: highlighted ? "#23BDC2" : "transparent"
            }


            highlighted: m_comboBox.highlightedIndex === index
        }

        popup: Popup{
            id: comboBoxPopup
            width: m_comboBox.width
//            implicitHeight: contentItem.implicitHeight
            implicitHeight: 400 * factor
            padding: 1
            clip: true
            y: m_comboBox.height + 5 * factor
            background: Rectangle{
                width: parent.width - 1
                height: parent.height
                radius: 5 * factor
                color: "#1B2139"
                border.color: "#23BDC2"
            }

            contentItem: ListView{
                implicitWidth: comboBoxPopup.width
    //            height: comboBoxPopup.height
                implicitHeight: contentHeight
                model: m_comboBox.popup.visible ? m_comboBox.delegateModel : ""

                //内容不能超出边界
                boundsBehavior: Flickable.StopAtBounds
                ScrollIndicator.vertical: ScrollIndicator { }
            }
        }

    }

    function setCurClassName( val ){
        curDisplayText.text = val
    }


}
