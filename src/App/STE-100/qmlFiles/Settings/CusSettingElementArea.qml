import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"

Item {
    id: root

    width: 1245 * factor
    height: 312 * factor

    readonly property font titleFont: Qt.font( { 'family': 'Microsoft YaHei', 'pixelSize': 50 * factor, 'bold': true } )
    property bool     showTopLine: true

    property alias titleString: titleText.text
    property real  leftOffset: 113 * factor

    signal sigConfigChanged()

    Component.onCompleted: {
        if( showTopLine ){
            topLineCmp.createObject( root, { 'anchors.top': root.top, 'anchors.left': root.left } )
        }
    }

    Text {
        id: titleText
        text: 'title'
        color: 'white'
        font: titleFont
        anchors{ left: parent.left; leftMargin: leftOffset; top: parent.top; topMargin: 53 * factor }
    }

    Component{
        id: topLineCmp
        Rectangle{
            id: topLine
            width: parent.width
            height: 3 * factor
            color: '#2B85D9'
            anchors{ top: parent.top; left: parent.left }
        }
    }
}
