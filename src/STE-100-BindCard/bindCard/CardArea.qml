import QtQuick 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0

Item {
    id:root
    implicitWidth: 436 * factor
    implicitHeight: 274 * factor
    state: "noCard"


    Rectangle{
        id:rectCardView
        anchors.fill: parent
        radius: 10 * factor
        Image {
            id:imgEasyHealthTips
            anchors.centerIn: parent
        }
    }

    states: [
        State {
            name: "noCard"
            PropertyChanges {
                target: rectCardView
                color:"#666666"
            }

            PropertyChanges {
                target: imgEasyHealthTips
                source: "qrc:/Pic/YKtitle1.png"
            }
        },
        State{
            name: "existCard"
            PropertyChanges{
                target: rectCardView
                color:"#2C77B4"
            }
            PropertyChanges {
                target: imgEasyHealthTips
                source: "qrc:/Pic/YKtitle2.png"
            }
        }
    ]

    function detectCard( detectRet ){

        if( Enums.Exist_Card ){
            root.state = "existCard"
        }

        if( Enums.NO_Card ){
            root.state = "noCard"
        }
    }
}
