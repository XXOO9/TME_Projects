import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "../"
import "../Moudues"

Item {
    id: root
    property var contentArray: ["first", "second"]
    property int curIndex: 0
    property bool checkAble: true

    signal sigBtnClicked( var btnIndex )

    Component.onCompleted: {
        setCurrentIndex( 0 )
    }

    Row{
        spacing: 250 * factor
        Repeater{
            id: radioBtnRepeater
            model: contentArray.length
            RadioButton{
                id: m_radioBtn
                padding: 0
                checkable: checkAble
                indicator: Rectangle{
                    implicitWidth: 100 * factor
                    implicitHeight: width
                    radius: width / 2
                    border.color: "#3989EE"

                    Rectangle{
                        width: parent.width * 0.7
                        height: width
                        radius: width / 2
                        color: "#3989EE"
                        visible: m_radioBtn.checked
                        anchors.centerIn: parent
                    }
                }

                contentItem: Text {
                    text: contentArray[index]
                    color: "white"
                    font{ family: "Microsoft YaHei"; pixelSize: 50 * factor }
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: m_radioBtn.indicator.width + m_radioBtn.spacing
                }

                onClicked: {
                    console.log( "clicked index = " + index )
                    curIndex = index
                    sigBtnClicked( index )
                }
            }
        }
    }

    function setCurrentIndex( tmpIndex ){
        curIndex = tmpIndex
        radioBtnRepeater.itemAt( tmpIndex ).checked = true
    }
}
