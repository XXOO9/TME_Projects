import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import ".."
Item {
    id: root
    width: 620 * factor
    height: 140 * factor
    property var popupHeight: 1350 * factor

    property var comboBoxDisplayList: [ "1", "2", "3", "4" ]

    property string fontType: "Microsoft YaHei"
    property var  curComboBoxIndex: 0
    property bool bEnable: true

    signal signalValueRefresh( var index )

    onCurComboBoxIndexChanged: {
        m_comboBox.currentIndex = curComboBoxIndex
        //textItem.text = m_comboBox.currentText
    }

    ComboBox{
        id: m_comboBox
        width: parent.width
        height: parent.height
        model: comboBoxDisplayList
        clip: true
        anchors.centerIn: parent
        currentIndex: curComboBoxIndex
        enabled: bEnable

        //选中框
        contentItem: Text {
            id: textItem
            text: m_comboBox.currentText
            font{ pixelSize: 62 * factor; family: fontType }
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        //选中框背景
        background: Rectangle{
            id: selectBackground
            width: parent.width
            color: "#1F71CF"
            radius: 70 * factor
            border.width: 1
            border.color: "#1ADDFC"
        }

        //候选框中展示的数据
        delegate: ItemDelegate{
            id: itemDelegate
            width: m_comboBox.width
            contentItem: Text {
                text: modelData
                color: highlighted ? "#50E8FF" : "white"
                font{ pixelSize: 62 * factor; family: fontType }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            background: Rectangle{
                height: m_comboBox.implicitBackgroundHeight
                width: m_comboBox.width
                color: "transparent"

                Image {
                    id: lineImage
                    width: parent.width
                    height: 2
                    anchors.top: parent.top
                    anchors.topMargin: -3
                    source: "qrc:/pic/line.png"
                    visible: index!=0
                }
            }

            highlighted: m_comboBox.highlightedIndex == index
        }

        indicator: Image {
            id: downIcon
            width: 40 * factor
            height: 23 * factor
            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.rightMargin: 56 * factor
            anchors.verticalCenter: parent.verticalCenter
            source: !m_comboBox.down ? "qrc:/pic/downIndactor.png" : "qrc:/pic/comboBoxIndactior.png"
            visible: bEnable
        }

        popup: Popup{
            id: popup
            y: parent.height + 20 * factor
            width: parent.width
            clip: true
            implicitHeight: contentItem.implicitHeight
            padding: 0
            //height: popupHeight

            contentItem: ListView{
                clip: true
                implicitHeight: contentHeight + 30 * factor
                model: m_comboBox.popup.visible ? m_comboBox.delegateModel : null
                ScrollBar.vertical: ScrollBar { }
            }

            background: Rectangle{
                id: popupBackground
                color: "#1E30C0"
                radius: 20 * factor
                border.width: 3 * factor
                border.color: "#1ADDFC"
            }
        }

        onActivated: {
            signalValueRefresh( m_comboBox.currentIndex )
        }
    }
}
