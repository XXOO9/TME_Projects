import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"

Item {
    id: root

    implicitWidth: 1526 * factor
    implicitHeight:  468 * factor

    readonly property real rowHeight: 117 * factor
    readonly property string fontYaHei: "Microsoft YaHei"

    signal sigLoadFinished()

    Component.onCompleted: {
        humanCompositionScoreDataModel.clear()
    }

    Rectangle{
        id: bkgRect
        anchors.fill: parent
        color: "#483ACE"
    }

    ListModel{
        id: humanCompositionScoreDataModel
    }

    ListView{
        id: humanCompositionScoreList
        model: humanCompositionScoreDataModel
        anchors.fill: parent
        delegate: huanmenCompositionElement
        header: listHeader
        spacing: -1.5 * factor
        ScrollBar.vertical: ScrollBar{}
    }


    Component{
        id: huanmenCompositionElement
        Row{
            spacing: -1.5 * recordTimeRect.border.width * factor
            //时间
            Rectangle{
                id: recordTimeRect
                width: root.width / 3
                height: rowHeight
                color: "transparent"
                border.color: "white"
                Text {
                    text: recordTime
                    font{ family: fontYaHei; pixelSize: 55 * factor }
                    color: "white"
                    anchors.centerIn: parent
                }
            }

            //身高
            Rectangle{
                id: humanHeightRect
                width: root.width / 3
                height: rowHeight
                color: "transparent"
                border.color: "white"
                Text {
                    text: humanHeight
                    font{ family: fontYaHei; pixelSize: 55 * factor }
                    color: "white"
                    anchors.centerIn: parent
                }
            }

            //体重
            Rectangle{
                id: humanWeightRect
                width: root.width / 3
                height: rowHeight
                color: "transparent"
                border.color: "white"
                Text {
                    text: humanWeight
                    font{ family: fontYaHei; pixelSize: 55 * factor }
                    color: "white"
                    anchors.centerIn: parent
                }
            }
        }
    }

    Component{
        id: listHeader
        Row{
            spacing: -1.5 * recordTimeRect.border.width * factor
            //时间
            Rectangle{
                id: recordTimeRect
                width: root.width / 3
                height: rowHeight
                color: "#3A34B6"
                border.color: "white"
                Text {
                    text: "测试时间"
                    font{ family: fontYaHei; pixelSize: 62 * factor }
                    color: "white"
                    anchors.centerIn: parent
                }
            }

            //身高
            Rectangle{
                id: humanHeightRect
                width: root.width / 3
                height: rowHeight
                color: "#3A34B6"
                border.color: "white"
                Text {
                    text: "身高(cm)"
                    font{ family: fontYaHei; pixelSize: 62 * factor }
                    color: "white"
                    anchors.centerIn: parent
                }
            }

            //体重
            Rectangle{
                id: humanWeightRect
                width: root.width / 3
                height: rowHeight
                color: "#3A34B6"
                border.color: "white"
                Text {
                    text: "体重(kg)"
                    font{ family: fontYaHei; pixelSize: 62 * factor }
                    color: "white"
                    anchors.centerIn: parent
                }
            }
        }
    }

    Connections{
        target: InterAction
        onSigHumanCompositionAvailable:{
            for( let ele of humanCompositionList ){
                humanCompositionScoreDataModel.append( {  recordTime: ele.recordTime,
                                                          humanHeight: ele.height,
                                                          humanWeight: ele.weight
                                                      } )

                sigLoadFinished()
            }
        }
    }

    function cleanHumanData(){
        humanCompositionScoreDataModel.clear()
    }
}
