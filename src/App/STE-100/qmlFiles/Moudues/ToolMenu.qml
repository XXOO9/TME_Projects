import QtQuick 2.0
import QtQuick.Controls 2.12
import "../"

Item {

    signal sigClicked( var index )
    width: 700 * factor
    height: width

    ListModel{
        id: menuList
        ListElement{ index: 0; name: "签字版调试" }
        ListElement{ index: 1; name: "扫描线左右" }
        ListElement{ index: 2; name: "计时器" }
    }

    Grid{
        rows: 3
        columns: 3
        anchors.centerIn: parent

        Repeater{
            model: menuList
            delegate: Button{
                width: 200 * factor
                height: width
                text: name
                onClicked: sigClicked( index )
            }
        }
    }
}
