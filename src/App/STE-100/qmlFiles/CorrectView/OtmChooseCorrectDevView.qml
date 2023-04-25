import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    ListModel{
        id: m_devList
        ListElement{ isChecked: false; devNum: "001" }
    }

    ListView{
        id: devListView
        anchors.fill: parent
        clip: true
    }

}
