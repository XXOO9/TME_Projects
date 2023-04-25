import QtQuick 2.12
import QtQuick.Controls 2.12
import "../"
import "."
import "../Popups"

CusSettingElementArea{
    id: root
    titleString: '测试模式: '

    signal sigEnableASyncTestModel( var enable )

    ButtonGroup{
        buttons: testModelRow.children
    }

    ButtonGroup{
        buttons: connectedCloumn.children
    }

    ButtonGroup{
        buttons: testModeCloumn.children
    }

    ListModel{
        id: testConfigDataModel
        ListElement{ modelName: '手动切换'; btnChecked: true }
        ListElement{ modelName: '自动切换'; btnChecked: false }

        function setDirectedModel( isManulChangeMode ){
            testConfigDataModel.get( 0 ).btnChecked = isManulChangeMode
            testConfigDataModel.get( 1 ).btnChecked = !isManulChangeMode
            setDirectConnectedModelVisible( isManulChangeMode )
        }

        function isManulChangeMode(){
            return testConfigDataModel.get( 0 ).btnChecked
        }
    }

    ListModel{
        id: conncteConfigDataModel
        ListElement{ modelName: '一对一'; btnChecked: true }
        ListElement{ modelName: '一对多'; btnChecked: false }

        function setWireLessModel( isWireLessModel ){
            conncteConfigDataModel.get( 0 ).btnChecked = !isWireLessModel
            conncteConfigDataModel.get( 1 ).btnChecked = isWireLessModel
            setTestModelVisible( isWireLessModel )
        }

        function isWireLessTestMode(){
            return conncteConfigDataModel.get( 1 ).btnChecked
        }
    }

    ListModel{
        id: testModelConfigDataModel
        ListElement{ modelName: '同步模式'; btnChecked: true }
        ListElement{ modelName: '异步模式'; btnChecked: false }

        function setSyncModel( isSyncModel ){
            testModelConfigDataModel.get( 0 ).btnChecked = isSyncModel
            testModelConfigDataModel.get( 1 ).btnChecked = !isSyncModel
        }
    }

    Row{
        id: testModelRow
        spacing: 185 * factor

        anchors{ left: root.left; leftMargin: root.leftOffset; top: root.top; topMargin: 152 * factor }
        Repeater{
            model: testConfigDataModel
            delegate: cusCheckBox
        }

    }

    Rectangle{
        id: connectedModel
        width: 380 * factor
        height: 288 * factor
        radius: 20 * factor
        color: '#4756EB'
        anchors{ top: parent.top; topMargin: 275 * factor; left: parent.left; leftMargin: 89 * factor }

        Column{
            id: connectedCloumn
            anchors{ left: parent.left; leftMargin: 20 * factor; top: parent.top; topMargin: 50 * factor }
            spacing: 30 * factor
            padding: 0
            Repeater{
                model: conncteConfigDataModel
                delegate: cusCheckBox
            }
        }
    }

    Rectangle{
        id: testModeModel
        width: 380 * factor
        height: 288 * factor
        radius: 20 * factor
        color: '#4756EB'
        anchors{ top: parent.top; topMargin: 275 * factor; right: parent.right; rightMargin: 289 * factor }

        Column{
            id: testModeCloumn
            anchors{ left: parent.left; leftMargin: 20 * factor; top: parent.top; topMargin: 50 * factor }
            spacing: 30 * factor
            padding: 0
            Repeater{
                model: testModelConfigDataModel
                delegate: cusCheckBox
            }
        }
    }



    Component{
        id: cusCheckBox
        CheckBox{
            id: checkBtn
            checked: btnChecked
            onCheckedChanged: {
                btnChecked = checked
                setDirectConnectedModelVisible( testConfigDataModel.isManulChangeMode() )
                setTestModelVisible( conncteConfigDataModel.isWireLessTestMode() )
                sigConfigChanged()
                displayCurConnecteInfo()

                //2023.01.11新增，如果选择了异步模式，那么只能为手动提交成绩,
//                if( modelName === '异步模式' ){
//                    if( !root.insidePropertyChanged ){
//                        root.insidePropertyChanged = true
//                        sigEnableASyncTestModel( true )
//                    }

//                }else{
//                    if( !root.insidePropertyChanged ){
//                        root.insidePropertyChanged = true
//                        sigEnableASyncTestModel( false )
//                    }
//                }
            }

            indicator: Rectangle{
                width: 75 * factor
                height: width
                radius: width / 2
                color: checkBtn.checked ? '#1F71CF' : 'transparent'
                border.color: '#1ADDFC'
                Rectangle{
                    width: 0.27 * parent.width
                    height: width
                    radius: width / 2
                    color: checkBtn.checked ? 'white' : 'transparent'
                    anchors.centerIn: parent
                }
            }

            contentItem: Text {
                text: modelName
                color: '#FFFFFF'
                font{ family: 'Microsoft YaHei'; pixelSize: 45 * factor }
                verticalAlignment: Text.AlignVCenter
                leftPadding: checkBtn.indicator.width + checkBtn.leftPadding
                anchors.verticalCenter: checkBtnBkg.verticalCenter
            }

            background: Rectangle{
                id: checkBtnBkg
                width: 75 * factor
                height: width
                radius: width / 2
                color: 'transparent'
                border.color: 'white'
            }
        }
    }

//  root  public funcs:
    function setConnectedModel( isManulChangeMode, isWireLessModel , isSyncTestMode){
        testConfigDataModel.setDirectedModel( isManulChangeMode )
        conncteConfigDataModel.setWireLessModel( isWireLessModel )
        testModelConfigDataModel.setSyncModel( isSyncTestMode )
        setDirectConnectedModelVisible( isManulChangeMode )
    }

    function getChangeMode(){
        return testConfigDataModel.get( 0 ).btnChecked
    }

    function getWireLessConnectedState(){
        return conncteConfigDataModel.get( 1 ).btnChecked
    }

    //获取当前是 同步还是异步测试模式 true->同步模式，  false->异步模式
    function getSyncTestState(){
        return testModelConfigDataModel.get( 0 ).btnChecked
    }

//  root  private funcs:

    //控制 手动切换还是自动切换区域的显隐
    function setDirectConnectedModelVisible( isManulChange ){
        connectedModel.visible = isManulChange
    }

    //控制 是直连还是组网连接
    function setTestModelVisible( isWireLessTest){
        testModeModel.visible = isWireLessTest
    }

    function displayCurConnecteInfo(){
        console.log( 'change mode = ' + getChangeMode() + ' wireless mode = ' + getWireLessConnectedState() )
    }
}
