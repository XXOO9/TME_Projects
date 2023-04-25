import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ".."
Item {
    id: root
    width: 1695 * factor
    height: 290 * factor

    property string strBorderColor: "#2995E9"

    //测试模式， true为考试模式，  false为练习模式
    property bool bIsTestGradeType: true

    signal signalTestTypeChanged( int type )

    Image {
        id: imgBorder
        width: 1695 * factor
        height: 244 * factor
        source: "qrc:/pic/19-gradeType2.png"
        anchors.centerIn: parent

        Text {
            text: qsTr( "成绩类型" )
            font.pixelSize: 68 * factor
            font.family: "Microsoft YaHei"
            color: "white"
            anchors.bottom: parent.top
            anchors.bottomMargin: -20 * factor
            anchors.horizontalCenter: parent.horizontalCenter
        }

        //测试成绩按钮
        Rectangle{
            id:rectTestBtn
            width: 85 * factor
            height: width
            radius: width / 2
            color: "transparent"
            border.color: "#D7DBF7"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 467 * factor
            Image {
                visible: bIsTestGradeType ? true : false
                source: "qrc:/pic/19-gradeType1.png"
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    bIsTestGradeType = true
                    signalTestTypeChanged( 2 )
                }
            }
        }
        //文字 "测试"
        Text {
            text: qsTr( "测试" )
            font.pixelSize: 62 * factor
            font.family: "Microsoft YaHei"
            color: "white"
            anchors.verticalCenter: rectTestBtn.verticalCenter
            anchors.left: rectTestBtn.right
            anchors.leftMargin: 29 * factor
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    bIsTestGradeType = true
                    signalTestTypeChanged( 2 )
                }
            }
        }

        //练习成绩按钮
        Rectangle{
            id:rectExerciseBtn
            width: 85 * factor
            height: width
            radius: width / 2
            color: "transparent"
            border.color: "#D7DBF7"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 608 * factor
            Image {
                visible: bIsTestGradeType ? false : true
                source: "qrc:/pic/19-gradeType1.png"
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    bIsTestGradeType = false
                    signalTestTypeChanged( 1 )
                }
            }
        }
        //文字 "练习"
        Text {
            text: qsTr( "练习" )
            font.pixelSize: 62 * factor
            font.family: "Microsoft YaHei"
            color: "white"
            anchors.verticalCenter: rectExerciseBtn.verticalCenter
            anchors.left: rectExerciseBtn.right
            anchors.leftMargin: 29 * factor
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    bIsTestGradeType = false
                    signalTestTypeChanged( 1 )
                }
            }
        }

    }
}
