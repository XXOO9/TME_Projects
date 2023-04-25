import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root

    width: 1120 * factorX
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
        width: parent.width
        height: factorY * 1.5
        color: '#2B384F'
        anchors{ top: parent.top; left: parent.left; topMargin: 99 * factorY }
    }

    Text {
        text: '学号: '
        font { family: 'Microsoft YaHei'; pixelSize: 16 * factorX; bold: true }
        color: 'white'
        anchors{ right: stuIdInputText.left; rightMargin: 12 * factorX; verticalCenter: stuIdInputText.verticalCenter }
    }

    CusTextInputFilter{
        id: stuIdInputText
        width: 220 * factorX
        height: 40 * factorY
        candidateModel: FilterModel
        anchors{ top: parent.top; left: parent.left; topMargin: 140 * factorY; leftMargin: 122 * factorX }
        onSigSelected: {
            let contentInfos = selectedContent.split( '+' )
            stuIdInputText.text = contentInfos[ 1 ]
            stuNameInputText.text = contentInfos[ 0 ]
            classNameText.text = CInterAction.getClassNameByUserId( contentInfos[ 1 ] )
        }
    }


    Text {
        text: '年级班级: '
        font { family: 'Microsoft YaHei'; pixelSize: 16 * factorX; bold: true }
        color: 'white'
        anchors{ right: classNameText.left; rightMargin: 12 * factorX; verticalCenter: classNameText.verticalCenter }
    }

    Text {
        id: classNameText
        width: 220 * factorX
        height: 40 * factorY
        text: '----------'
        color: 'white'
        font { family: 'Microsoft YaHei'; pixelSize: 26 * factorX; bold: true }
        anchors{ top: parent.top; left: parent.left; topMargin: 210 * factorY; leftMargin: 125 * factorX }
    }


    Text {
        text: '姓名: '
        font { family: 'Microsoft YaHei'; pixelSize: 16 * factorX; bold: true }
        color: 'white'
        anchors{ right: stuNameInputText.left; rightMargin: 12 * factorX; verticalCenter: stuNameInputText.verticalCenter }
    }

    CusTextInputFilter{
        id: stuNameInputText
        width: 220 * factorX
        height: 40 * factorY
        candidateModel: FilterModel
        anchors{ top: parent.top; left: parent.left; topMargin: 280 * factorY; leftMargin: 122 * factorX }
        onSigSelected: {
            let contentInfos = selectedContent.split( '+' )
            stuIdInputText.text = contentInfos[ 1 ]
            stuNameInputText.text = contentInfos[ 0 ]
            classNameText.text = CInterAction.getClassNameByUserId( contentInfos[ 1 ] )
        }
    }

    CusUserPhotoItem{
        id: userPhoto
        anchors{ top: parent.top; topMargin: 140 * factorY; right: parent.right; rightMargin: 342 * factorX }
    }

    CusCardArea{
        id: cardArea
        anchors{ horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 67 * factorY }
        state: 'noCard'
        onSigClicked: onBtnClicked()
    }

    Connections{
        target: CInterAction
        onSigNewCardDeceted: onCardDecetedChanged( cardNumber )
    }

    function onCardDecetedChanged( decetedCardNumber ){
        cardArea.curCardNumber = decetedCardNumber
        if( decetedCardNumber === '-1' ){
            cardArea.state = 'noCard'
            return
        }

        if( CInterAction.isCardBinded( decetedCardNumber ) ){
            cardArea.state = 'decetedBindedCard'
            return
        }

        cardArea.state = 'decetedUnbindCard'
    }

    function onBtnClicked(){
        if( stuIdInputText.text.length > 0 ){
            console.log( "cur no = " + cardArea.curCardNumber )
            if( cardArea.curCardNumber === '-1' ){
                return
            }

            CInterAction.bindUserWithCardNumber( stuIdInputText.text, cardArea.curCardNumber )
        }
    }


}
