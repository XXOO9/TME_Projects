import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root

    width: 436 * factorX
    height: 354 * factorY

    readonly property string cardAreaDecetedCardColor: '#2C77B4'
    readonly property string cardAreaNoCardColor: '#666666'

    readonly property string btnDecetedCardColor:'#009944'           //检测到卡
    readonly property string btnNoCardColor: '#666666'               //未检测到卡

    readonly property string cardAreaImgDecetedCardUrl: 'qrc:/pics/YKtitle2.png'
    readonly property string cardAreaImgNoCardUrl: 'qrc:/pics/YKtitle1.png'

    property string curCardNumber: '-1'

    signal sigClicked()



    Rectangle{
        id: cardStateRect
        width: parent.width
        height: 274 * factorY
        color: cardAreaNoCardColor
        radius: 10 * factorX
        anchors{ top: parent.top; horizontalCenter: parent.horizontalCenter }

        Image {
            id: companyImg
            width: 210 * factorX
            height: 60 * factorY
            source: cardAreaImgNoCardUrl
            anchors.centerIn: parent
        }

        MouseArea{
            anchors.fill: parent
            onDoubleClicked: console.log( "current card NO: " + curCardNumber )
        }
    }

    Rectangle{
        id: bindBtn
        width: 200 * factorX
        height: 50 * factorY
        radius: 25 * factorX
        color: btnNoCardColor
        anchors{ bottom: parent.bottom; horizontalCenter: parent.horizontalCenter }

        Text {
            id: btnText
            text: '绑定'
            color: '#FFFFFF'
            font{ family: 'Microsoft YaHei'; pixelSize: 18 * factorX; bold: true }
            anchors.centerIn: parent
        }

        MouseArea{
            anchors.fill: parent
            onPressed: bindBtn.color = Qt.lighter( bindBtn.color, 0.6 )
            onReleased: bindBtn.color = Qt.lighter( bindBtn.color, 1 )
            onClicked: {
                if( root.state === 'noCard' ){
                    return
                }

                sigClicked()
            }
        }
    }

    states: [
        State{
            name: 'noCard'
            PropertyChanges{ target: cardStateRect; color: cardAreaNoCardColor }
            PropertyChanges{ target: companyImg; source: cardAreaImgNoCardUrl }
            PropertyChanges{ target: btnText; text: '绑定' }
            PropertyChanges{ target: bindBtn; color: btnNoCardColor }
        },

        State{
            name: 'decetedUnbindCard'
            PropertyChanges{ target: cardStateRect; color: cardAreaDecetedCardColor }
            PropertyChanges{ target: companyImg; source: cardAreaImgDecetedCardUrl }
            PropertyChanges{ target: btnText; text: '绑定' }
            PropertyChanges{ target: bindBtn; color: btnDecetedCardColor }
        },

        State{
            name: 'decetedBindedCard'
            PropertyChanges{ target: cardStateRect; color: cardAreaDecetedCardColor }
            PropertyChanges{ target: companyImg; source: cardAreaImgDecetedCardUrl }
            PropertyChanges{ target: btnText; text: '解绑' }
            PropertyChanges{ target: bindBtn; color: btnDecetedCardColor }
        }
    ]


}
