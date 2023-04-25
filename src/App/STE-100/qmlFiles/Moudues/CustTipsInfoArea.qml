import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    width: 500 * factor
    height: 800 * factor

    ListModel{
        id: m_listModel
    }

    ListView{
        id: m_listView
        anchors.fill: parent
        model: m_listModel

        delegate: Rectangle{
            width: parent.width
            height: 150 * factor
            color: "transparent"
            border.color: "white"
            Text {
                text: tips
                anchors.centerIn: parent
                color: "white"
                font{ family: "Microsoft YaHei"; pixelSize: 54 * factor }
            }
        }

        MouseArea{
            anchors.fill: parent
            propagateComposedEvents: true
        }
    }

    Connections{
        target: InterAction
        onSigAppendViewDisplayErrorString:{
            append( errorString )
        }

        onSigRemoveSpecificErorrString:{
            removeSpecificErorrString( index)
        }

        onSigClearViewErorrStrings:{
            clearAllErorrString()
        }
    }

    function append( tipsString ){
        var jsonData = tipsString
        var jsonObj = { "tips": jsonData }
        m_listModel.append( jsonObj )
    }

    function removeSpecificErorrString( index ){
        //保护措施
        if( (m_listModel.count - 1) < index ){
            return
        }
        m_listModel.remove( index )
    }

    function clearAllErorrString(){
        m_listModel.clear()
    }
}
