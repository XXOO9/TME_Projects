import QtQuick 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0

Item {
    id: root
    implicitWidth: 710 * factor
    implicitHeight: 910 * factor

    property string fontYaHei: "Microsoft YaHei"
    property int pageType: Enums.Page_User_Bind

    Rectangle{
        anchors.fill: parent
        radius: 10 * factor
        color: "#1B2139"
    }

    Text {
        id: textTips
        text: "绑卡列表"
        color: "white"
        font{ pixelSize: 26 * factor; family: fontYaHei; bold: true}
        anchors.top: parent.top
        anchors.topMargin: 40 * factor
        anchors.left: parent.left
        anchors.leftMargin: 40 * factor
    }

    //分割线
    Rectangle{
        id: scaleLine
        width: root.width
        height: 2 * factor
        color: "#2B384F"
        anchors.top: root.top
        anchors.topMargin: 100 * factor
        anchors.left: root.left
    }

    CustTableView{
        id: bindTableView
        tableViewType: pageType
        anchors.top: scaleLine.bottom
        anchors.topMargin: 40 * factor
        anchors.horizontalCenter: parent.horizontalCenter
    }


}
