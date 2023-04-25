import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ".."

Item {
    id: root
    property int defaultRet: 0000
    property real offsetPix: 0

    width: 1582  * factor
    height: 864 * factor

    Rectangle{
        id: retRect
        width: parent.width
        height: parent.height
        radius: 30 * factor
        color: "#568BEA"
        anchors.centerIn: parent
        Row{
            id: rowRectRetDisplay
            spacing: 35 * factor
            anchors.left: parent.left
            anchors.leftMargin: 170 * factor
            anchors.top: parent.top
            anchors.topMargin: 318 * factor
            Repeater{
                model: 4
                delegate: Rectangle{
                    width: 168 * factor
                    height: 230 * factor
                    radius: 15 * factor
                    color: "#384DD5"
                    Text {
                        text: GlobalProperty.extractNumber( index + 1, Number( GlobalProperty.strCurRet ) )
                        color: "white"
                        font.pixelSize: 169 * factor
                        font.family: "Microsoft YaHei"
                        anchors.centerIn: parent
                        onTextChanged: offsetPix = offsetProvider()
                    }
                }
            }
        }
        //显示 的单位
        Text {
            id: strMeasure
            text: qsTr( "ml" )
            font{ pixelSize: 62 * factor; family: "Microsoft YaHei" }
            color: "white"
            anchors.bottom: rowRectRetDisplay.bottom
            anchors.left: rowRectRetDisplay.right
            anchors.leftMargin: 28 * factor
        }
        //肺活量标度图
        Image {
            id: imgFHLRuler
            width: 250 * factor
            height: 764 * factor
            source: "qrc:/pic/ruler.png"
            anchors.right: parent.right
            anchors.rightMargin: 170 * factor
            anchors.top: parent.top
            anchors.topMargin: 50 * factor
            //游标
            Image {
                id: name
                width: 68 * factor
                height: 46 * factor
                source: "qrc:/pic/cursor.png"
                anchors.right: parent.right
                anchors.rightMargin: 31 * factor
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 60 * factor + offsetPix

            }
        }
    }

    function offsetProvider(){
        if( GlobalProperty.strCurRet === "--" ){
            return 0
        }

        let ret =  Number( GlobalProperty.strCurRet )

        if( ret >= 7000 ){
            return 7000 * 0.09 * factor
        }

        return ret * 0.09 * factor

    }
}
