import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../Moudues"

Item {
    id: root
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

        OtmSystemSetting{
            anchors.centerIn: parent
        }
    }

    function open(){
        m_popup.open()
    }

    function close(){
        m_popup.close()
    }
}
