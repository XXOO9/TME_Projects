import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "../"
import "../Moudues"

Item {
    id: root
    property int indicatorWid: 100 * factor
    property string indicatorReleaseColor: "#1F71CF"
    property string indicatorPressColor: "gray"
    property alias curValue: m_spinBox.value


    SpinBox {
          id: m_spinBox
          width: 400 * factor
          height: 100 * factor
          padding: 0
          from: 3
          to: 10

          contentItem: Rectangle{
              id: rectDisplayValue
              width: 200 * factor
              height: parent.height
              color: "#3241CC"
              border.color: "white"
              radius: 12 * factor
              Text {
                  id: valText
                  text: m_spinBox.value
                  anchors.centerIn: parent
                  color: "white"
                  font{ family: "Microsoft YaHei"; pixelSize: 50 * factor; bold: true }
              }
          }

          up.indicator: Rectangle{
              width: indicatorWid
              height: m_spinBox.height
              color: !enabled || m_spinBox.up.pressed ? indicatorPressColor : indicatorReleaseColor
              border.color: enabled ? "#1ADDFC" : indicatorReleaseColor
              anchors.verticalCenter: rectDisplayValue.verticalCenter
              anchors.left: rectDisplayValue.right
              anchors.leftMargin: 25 * factor

              radius: 12 * factor

              Text {
                  text: "+"
                  color: "white"
                  font{ family: "Microsoft YaHei"; pixelSize: 100 * factor; bold: true }
                  anchors.centerIn: parent
                  verticalAlignment: Text.AlignVCenter
                  horizontalAlignment: Text.AlignHCenter
              }
          }

          down.indicator: Rectangle{
              width: indicatorWid
              height: m_spinBox.height
              color: !enabled || m_spinBox.down.pressed ? indicatorPressColor : indicatorReleaseColor
              border.color: enabled ? "#1ADDFC" : indicatorReleaseColor
              anchors.verticalCenter: rectDisplayValue.verticalCenter
              anchors.right: rectDisplayValue.left
              anchors.rightMargin: 25 * factor
              radius: 12 * factor

              Text {
                  text: "-"
                  color: "white"
                  font{ family: "Microsoft YaHei"; pixelSize: 100 * factor; bold: true }
                  anchors.centerIn: parent
                  verticalAlignment: Text.AlignVCenter
                  horizontalAlignment: Text.AlignHCenter
              }
          }


          background: Rectangle {
              anchors.fill: parent
              color: "#4756EB"
          }
      }

    function setValue( val ){
        m_spinBox.value = val;
    }
}
