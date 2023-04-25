import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "."

ApplicationWindow {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height + 1
    title: qsTr("Hello World")

    flags: Qt.FramelessWindowHint | Qt.Window

    readonly property real factorX: 1
    readonly property real factorY: 1

    Component.onCompleted: {
        loadingPop.open()
    }

    color: 'black'

    CusTopBar{
        id: topBar
        anchors{ top: parent.top; horizontalCenter: parent.horizontalCenter }
    }

    ItemStuInfoArea{
        anchors{ top: topBar.bottom; topMargin: 28 * factorY; left: parent.left; leftMargin: 30 * factorX }
    }

    ItemBindedInfoArea{
        anchors{ top: topBar.bottom; topMargin: 28 * factorY; right: parent.right; rightMargin: 30 * factorX }
    }

    Popup{
        id: loadingPop
        width: 160 * factorX
        height: width
        modal: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: parent
        background: Rectangle{
            color: 'transparent'
        }

        CusLoading{
            id: loading
        }
    }

    Connections{
        target: CInterAction
        onSigCloseLoading: loadingPop.close()
    }
}
