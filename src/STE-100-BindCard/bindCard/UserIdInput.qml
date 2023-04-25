import QtQuick 2.12
import QtQuick.Controls 2.12
import com.Company.TME 1.0

Item {
    id: root
    width: 220 * factor
    height: 40 * factor

    property string tipsString: "请输入学号"
    property int inputType: Enums.STUDENTID
    property int parentPage: Enums.Page_User_Bind
    property string fontColor: "#23BDC2"
    property string borderColor: "#23BDC2"
    property var srcCmp: Component
    property alias textLength: contentInput.length
    property alias isFocus: contentInput.activeFocus
    property alias content: contentInput.text

    TextField{
        id: contentInput
        width: root.width
        height: root.height
        color: fontColor
        font{ pixelSize: 25 * factor; family: "Microsoft YaHei"}
        echoMode: inputType === Enums.PASSWD ? TextInput.Password : TextInput.Normal
        placeholderText: tipsString
        placeholderTextColor: "gray"
        //        validator:IntValidator{ bottom: 0; top: 99999999 }

        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
            border.color: borderColor
            radius: 5 * factor
        }

        onDisplayTextChanged: {
            whileDisplayTextChanged()
        }

        Keys.enabled: true
        Keys.onPressed: {
            if( event.key === Qt.Key_Return && contentInput.length > 0){
                enterkeyPressed()
            }
        }
    }

    function setCurDisplayText( str ){
        contentInput.text = str
    }

    function enterkeyPressed(){
        //手动查询学生
        if( inputType === Enums.STUDENTID ){
            console.log( "学号输入框按回车了" )
            StudentView.stuId = contentInput.displayText
        }

        if( inputType === Enums.STUDENTNAME ){
            console.log( "姓名输入框按回车了" )
        }
    }

    function whileDisplayTextChanged(){
        //输入长度达到最大值时，自动查询

        //在此区分是学生绑定界面还是教师绑定界面
        if( parentPage === Enums.Page_User_Bind ){
            if( contentInput.length >= 3 && root.inputType === Enums.STUDENTID ){
                StudentView.stuId = contentInput.displayText
            }
        }

        //在此区分是学生绑定界面还是教师绑定界面
        if( parentPage === Enums.Page_Class_Bind ){
            if( contentInput.length >= 3 && root.inputType === Enums.STUDENTID ){
                ClassView.setStuId( contentInput.displayText )
            }
        }
    }
}
