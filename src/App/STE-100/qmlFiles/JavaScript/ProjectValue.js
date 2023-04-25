.pragma library
/***************************提交定义***********************************/
var indexCommitAutoCommit = 0
var indexCommitManualCommit = 1

//查询成绩界面用到的值
var queryScore = {
    maleArray: [ "选择体测项目", "肺活量", "立定跳远", "坐位体前屈",
        "引体向上", "仰卧起坐", "1000米跑步", "50米短跑", "跳绳", "握力", "视力", "肺通气量", "俯卧撑", "往返跑" ],

    femaleArray: [ "选择体测项目", "肺活量", "立定跳远", "坐位体前屈",
        "引体向上", "仰卧起坐", "800米跑步", "50米短跑", "跳绳", "握力", "视力", "肺通气量", "俯卧撑", "往返跑" ],

    timeInterval: [ "近一周", "近一个月", "近三个月" ],

    index_NO_ITEM: 0,
    index_VC: 1,
    index_SBJ: 2,
    index_SFB: 3,
    index_PULL_UP: 4,
    index_SIT_UP: 5,
    index_800_RUN: 6,
    index_1000_RUN: 6,
    index_50_RUN: 7,
    index_ROPE_SKINP: 8,
    index_GRIP: 9,
    index_EYESIGHT: 10,
    index_PVV: 11,
    index_PUSH_UP: 12,
    index_RUN_BACK: 13,

    index_ONE_WEEK: 0,
    index_ONE_MONTH: 1,
    index_THREE_MONTH: 2,


    index_EXAM_SCORE: 1,
    index_EXERCISE_SCORE: 2,
}

var commitConfig = {
    commitModels: [ "手动提交", "自动提交" ],
    signConfig: [ "需要签字", "不需要签字" ],

    index_manualCommit: 0,
    index_autoCommit: 1,

    index_Sign: 0,
    index_NO_Sign: 1,

    countTime: 10,
    signNatureWay: 0,
    commitWay: 0,
}


var rootErrorPopup = {
    textErrorInfo: "null"
}

//将成绩查询界面的测试类型下拉框的下标与TEST_ITEM字符串相对应，增强可读性
function itemComboBoxIndex2ItemName( index, isMale ){
    switch( index ){
    case 1:
        return "FHL"
    case 2:
        return "LDTY"
    case 3:
        return "ZWTQQ"
    case 4:
        return "YTXS"
    case 5:
        return "YWQZ"
    case 6:
        return isMale === true ? "1000RUN" : "800RUN"
    case 7:
        return "RUNSHORT"
    case 8:
        return "ROPESKING"
    case 9:
        return "ROPEGPIP"
    case 10:
        return "VISION"
    case 11:
        return "PVV"
    case 12:
        return "PUSHUP"
    case 13:
        return "RUNBACK"
    default:
        return "FHL"
    }
}
/********************字体*****************/
let fontYaHei = "Microsoft YaHei"

/**********************颜色****************/
let colorTransparent = "transparent"

/****************************图标资源路径**********************/
let imgRestart = "qrc:/pic/reStartBtn.png"
let imgStart   = "qrc:/pic/startBtn.png"
var imgClose   = "qrc:/pic/22-CloseBtn.png"


/**********************设备校准弹窗相关*************************/
var curCorrectDevNumber = 0


let indexPopCorrectDevBody = 0


let urlCmpListViewDelegate = "qrc:/qmlFiles/Moudues/CusTestingInfoComponent.qml"
let urlCmpListViewThreeClosDelegate = "qrc:/qmlFiles/Moudues/CusTestingInfoThreeColumnsComponent.qml"
let urlCorrectDevBody = "qrc:/qmlFiles/CorrectView/CorrectDevBody.qml"
let returnImg = "qrc:/pic/settings/icons8-left-100.png"



let correctItemHeight = 0x01
let correctItemWeight = 0x02

//新版设置界面常量
const settingIndexPad          = 0
const settingIndexDev          = 1
const settingIndexUsing        = 2
const settingIndexDataManage   = 3
const settingBlockHouseChannel = 4

//新版设置界面常量  按页面区分
//主机设置界面    与 CHostSettingConfig::HostElementData  保持同步
const settingPadDevTypeList             = 0
const settingPadRequireSignature        = 1
const settingPadDirectedConnectedModel  = 2
const settingPadWireLessConnectedModel  = 3

//设备设置界面    与 CDevSettingConfig::DevElementData  保持同步
const settingDevChanmel = 0
const settingDevCorrecte = 1
const settingDevOutAssistantDevList = 2

//测试设置界面    与 CUsingSettingConfig::CUsingElementData  保持同步
const settingUsingTimeout = 0
const settingUsingTestCount = 1


