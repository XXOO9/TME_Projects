import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root

    readonly property real perRowWeight: 157 * factorX
    readonly property real perRowHeight: 47 * factorY

    readonly property string borderColor: '#22848A'
    readonly property string cellColor: '#2E3C51'
    readonly property string headerCellColor: '#171D33'

    width: 710 * factorX
    height: 910 * factorY

    Rectangle{
        anchors.fill: parent
        radius: 10 * factorX
        color: '#1B2139'
    }

    Text {
        text: '刷卡学生基本信息'
        color: '#FCFCFC'
        font{ family: 'Microsoft YaHei'; pixelSize: 26 * factorX; bold: true }
        anchors{ top: parent.top; left: parent.left; topMargin: 42 * factorY; leftMargin: 40 * factorX }
    }

    Rectangle{
        id: line
        width: parent.width
        height: factorY * 1.5
        color: '#2B384F'
        anchors{ top: parent.top; left: parent.left; topMargin: 99 * factorY }
    }

    //
    ListModel{
        id: bindedUserInfosModel

        ListElement{ className: '年级班级'; userName: '姓名'; userId: '学号'; cardNumber: '卡号' }
    }

    ListView{

        id: bindUserInfos
        width: 630 * factorX
        height: 732 * factorY
        model: bindedUserInfosModel
        anchors{ top: line.bottom; topMargin: 40 * factorY; horizontalCenter: parent.horizontalCenter }
        delegate: listDelegate
    }

    Component{
        id: listDelegate

        Row{
            spacing: -1 * factorX
            Rectangle{
                width: perRowWeight
                height: perRowHeight
                color: cellColor
                border.color: borderColor

                Text {
                    text: cardNumber
                    color: '#FFFFFF'
                    font{ family: 'Microsoft YaHei'; pixelSize: 16 * factorX }
                    anchors.centerIn: parent
                }
            }

            Rectangle{
                width: perRowWeight
                height: perRowHeight
                color: cellColor
                border.color: borderColor

                Text {
                    text: className
                    color: '#FFFFFF'
                    font{ family: 'Microsoft YaHei'; pixelSize: 16 * factorX }
                    anchors.centerIn: parent
                }
            }

            Rectangle{
                width: perRowWeight
                height: perRowHeight
                color: cellColor
                border.color: borderColor

                Text {
                    text: userName
                    color: '#FFFFFF'
                    font{ family: 'Microsoft YaHei'; pixelSize: 16 * factorX }
                    anchors.centerIn: parent
                }
            }

            Rectangle{
                width: perRowWeight
                height: perRowHeight
                color: cellColor
                border.color: borderColor

                Text {
                    text: userId
                    color: '#FFFFFF'
                    font{ family: 'Microsoft YaHei'; pixelSize: 16 * factorX }
                    anchors.centerIn: parent
                }
            }
        }
    }

    Connections{
        target: CInterAction
        onSigAppendBindedUserInfo:{
            bindedUserInfosModel.append( infos )
        }
    }
}
