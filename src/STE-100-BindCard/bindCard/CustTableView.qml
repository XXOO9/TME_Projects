import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0
import "."

Item {
    id: root
    implicitWidth: 630 * factor
    implicitHeight: 732 * factor

    property real unitHeight: 48 * factor
    property string fontYaHei: "Microsoft YaHei"
    property int tableViewType: Enums.User_Bind_Table
    property var dataMode: tableViewType === Enums.User_Bind_Table ? UserDataModel : ClassBindDataModel


    Component.onCompleted: {
        UserDataModel.setHorizontalHeader( "序号#年级班级#姓名#学号" )
        ClassBindDataModel.setHorizontalHeader( "序号#年级班级#姓名#学号" )
        createCmp( UserDataModel.horizontalHeaderCount(), horizontalHeaders, unitRect )
    }

    Rectangle{
        anchors.fill: parent
        color: "transparent"
        border.width: 1 * factor
        border.color: "#22848A"
    }

    Row{
        id:horizontalHeaders
        width: root.width
        height: unitHeight
        anchors.left: root.left
        anchors.top: root.top
    }

    Component{
        id:unitRect
        Rectangle{
            property string displayText: "default"
            implicitWidth: root.width / UserDataModel.horizontalHeaderCount()
            implicitHeight: unitHeight
            color: "transparent"
            border.width: 1 * factor
            border.color: "#22848A"
            Text {
                text: displayText
                color: "#F0F0F0"
                anchors.centerIn: parent
                font{ pixelSize: 20 * factor; family: fontYaHei; bold: true }
            }
        }
    }

    TableView{
        id: tableView
        width: root.width
        height: root.height - horizontalHeaders.height
        model: dataMode
        rowHeightProvider: function(row){ return unitHeight }
        columnWidthProvider: function( column ){ return (root.width / UserDataModel.horizontalHeaderCount()) }
        clip: true
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { }
        anchors.left: horizontalHeaders.left
        anchors.top: horizontalHeaders.bottom
        delegate: Rectangle{
            implicitWidth: tableView.columnWidthProvider( column )
            implicitHeight: tableView.rowHeightProvider( row )
            color: "transparent"
            border.width: 1 * factor
            border.color: "#22848A"

            Text {
                color: "white"
                text: dataMode.data( dataMode.index( row, column ), TableModel.DisplayRole )
                font{ pixelSize: 16 * factor; family: fontYaHei }
                anchors.centerIn: parent
            }
        }
    }

    function createCmp( num, parent, cmp ){
        for( var i = 0; i < num; i++ ){
            var content = dataMode.horizontalHeader( i )
            cmp.createObject( parent, { displayText: content } )
        }
    }
}
