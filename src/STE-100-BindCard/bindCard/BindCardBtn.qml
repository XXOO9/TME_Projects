import QtQuick 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0

Item {
    id: root
    implicitWidth: 200 * factor
    implicitHeight: 50 * factor
    state: "disBind"

    signal clicked()

    Rectangle{
        id: bindBtn
        anchors.fill: parent
        radius: 25 * factor
        Text {
            id: display
            text: "绑定"
            font{ pixelSize: 18 * factor; family: "Microsoft YaHei" }
            anchors.centerIn: parent
        }

        MouseArea{
            id:btnMouseAarea
            anchors.fill: parent
            onClicked: {
                root.clicked()
            }
        }
    }

    states: [
        State {
            name: "readyBind"
            PropertyChanges {
                target: bindBtn
                color: "#009944"
            }
            PropertyChanges{
                target: display
                color:"white"
            }

            PropertyChanges{
                target: btnMouseAarea
                enabled:true
            }
        },

        State {
            name: "disBind"
            PropertyChanges {
                target: bindBtn
                color: "#009944"
            }
            PropertyChanges{
                target: btnMouseAarea
                enabled:true
            }
            PropertyChanges {
                target: display
                color: "#FFFFFF"
                text: "解绑"
            }
        },

        State {
            name: "noCard"
            PropertyChanges {
                target: bindBtn
                color: "#666666"
            }
            PropertyChanges{
                target: btnMouseAarea
                enabled: false
            }
            PropertyChanges {
                target: display
                color: "#929292"
                text: "绑定"
            }
        }
    ]

    function setBindBtnState( btnState ){
        switch( btnState ){
        case Enums.BindBtn_Disable:
            root.state = "noCard"
            return
        case Enums.BindBtn_Ready_Bind:
            root.state = "bind"
            return
        case Enums.BindBtn_Ready_DisBind:
            root.state = "disBind"
            return
        default:
            root.state = "noCard"
            return
        }
    }
}
