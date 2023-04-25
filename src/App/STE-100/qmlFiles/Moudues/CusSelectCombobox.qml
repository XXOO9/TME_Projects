import QtQuick 2.12
import QtQuick.Controls 2.12
ComboBox{
    id: root

    implicitWidth: 200
    implicitHeight: width / 4

    signal sigCurrentDevIndexChanged( var curDevValue )

    // name
    ListModel{
        id: listModel
        ListElement{ fieldName: '111'; fieldVal: 'A' }
        ListElement{ fieldName: '222'; fieldVal: 'B' }
    }

    //配置项
    property string bkgRectColor:       'transparent'
    property string bkgRectBorderColor: "red"                          //主体部分边框颜色
    property real   bkgRectBorderWidth: 2 * localFactor                //主体部分边框宽度
    property real   bkgRadius: 12 * factor                             //主题部分的弧度值

    property string displayContentTextColor: "blue"                    //主体部分文字颜色

    property string displayContentRectColor: "transparent"             //主体部分指示器左边部分矩形颜色 !!!
    property string displayContentRectBorderColor: "transparent"       //主体部分指示器左边部分矩形边框颜色  !!!

    property string selectContentHighLightColor: "red"                 //候选部分矩形高亮颜色
    property string selectContentColor:          "green"               //候选部分矩形的颜色
    property string selectContentBorderColor:    "blue"                //候选部分矩形边框的颜色

    property real     maxDisplayContentCnt: 5                             //最大显示的数量
    property real     itemHeight: 50 * localFactor                        //候选单个矩形的高度
    property string   itemTextColor: "black"                              // 候选文字颜色
    property real     localFactor: 1

    property bool mouseEnable: true

    background: Rectangle{
        id: bkgRect
        color: bkgRectColor
        radius: bkgRadius
        border.color: bkgRectBorderColor
        border.width: bkgRectBorderWidth
    }

    contentItem: ItemDelegate{
        id: delegateItem
        implicitWidth: root.width
        padding: 0

        Text {
            text: displayText
            width: parent.width - 100 * factor
            color: displayContentTextColor
            fontSizeMode: Text.Fit
            font{ family: 'Microsoft YaHei'; pixelSize: 50 * localFactor; bold: true }
            horizontalAlignment: Text.AlignLeft
            anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 20 }
        }

        background: Rectangle{
            color: "transparent"
            border.color: "transparent"
        }

        onClicked: {
            if( !mouseEnable ){
                return
            }
            pop.open()
        }
    }

    indicator: Image {
        id: cusIndicator
        x: root.width - width - root.rightPadding
        y: root.topPadding + ( root.availableHeight - height ) / 2
        width: 40 * localFactor
        height: 23 * localFactor
        source: "qrc:/pic/xiala01.png"

        Behavior on rotation {
            NumberAnimation{ duration: 200 }
        }

        states: [
            State{
                when: pop.visible
                PropertyChanges{ target: cusIndicator; rotation: 180 }
            }

        ]
    }


    delegate: ItemDelegate{
        width: root.width
        height: itemHeight

        highlighted: index === root.highlightedIndex

        contentItem: Text {
            text: fieldName
            color: itemTextColor
            width: root.width
            font.pixelSize: 50 * localFactor
            font.bold: true
            font.family: 'Microsoft YaHei'
            horizontalAlignment: Text.AlignLeft
        }

        background: Rectangle{
            width: root.width
            color: highlighted ? selectContentHighLightColor : selectContentColor
            border.color: selectContentBorderColor
        }

        onClicked: {
            console.log( 'ret = ' + fieldVal + ' ' + fieldName )
            if( displayText === fieldName ){
                console.log( "comboBox select not changed!" )
                return
            }
            root.displayText = fieldName
            sigCurrentDevIndexChanged( fieldVal )
        }

    }

    popup: Popup{
        id: pop
        y: root.height
        width: root.width
        padding: 0

        onOpened: selectListView.currentIndex = 0

        ListView{
            id: selectListView
            width: parent.width
            height: root.maxDisplayContentCnt * itemHeight
            model: pop.opened ? root.delegateModel : null
            spacing: 0
            clip: true
            ScrollBar.vertical: ScrollBar { }
        }
    }
}
