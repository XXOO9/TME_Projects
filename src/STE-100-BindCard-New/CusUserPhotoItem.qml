import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

Item {
    id: root

    implicitWidth: 202 * factorX
    implicitHeight: width

    property alias photoUrl: userImg.source

    Rectangle{
        id: userPhoto
        radius:  width / 2
        color: "gray"
        anchors.fill: parent
        Image {
            id: userImg
            visible: false
            anchors.fill: parent
            source: ''
        }

        Rectangle{
            id: maskRect
            width: parent.width
            height: parent.height
            anchors.fill: parent
            color: "red"
            radius: width / 2
            visible: false
        }

        OpacityMask{
            anchors.fill: parent
            maskSource: maskRect
            source: userImg
        }
    }
}
