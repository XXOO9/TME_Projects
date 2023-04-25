import QtQuick 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0

Item {
    id:root
    width: 1920 * factor
    height: 970 * factor

    property string userBindTitle: "学生刷卡基本信息"

    InfoAreaClass{
        id: classInfoArea
        titleText: userBindTitle
        anchors.top: parent.top
        anchors.topMargin: 28 * factor
        anchors.left: parent.left
        anchors.leftMargin: 30 * factor
    }


    BindedArea{
        id:bindedArea
        pageType: Enums.Page_Class_Bind
        anchors.right: parent.right
        anchors.rightMargin: 30 * factor
        anchors.top: classInfoArea.top
    }
}
