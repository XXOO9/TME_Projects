pragma Singleton
import QtQuick 2.0
import ProjectEnums 1.0
import "./JavaScript/ProjectValue.js" as Common


QtObject {

    property int pix: 1
    property int screenZoomX: 1920 * pix
    property int screenZoomY: 2880 * pix

    property int scanfLineStartPos: 0
    property int scanfHorizontalPos: 0

    /****************当前学生信息*******************/
    property string strCurStudentName: "null"
    property string strCurStudentAge: "null"
    property string strCurStudentGrade: "null"
    property string strCurStudentClass: "null"
    property string strCurStudentNumber: "null"
    property bool   bCurStudentSex: true

    /****************全局***************************/
    property string signInfos: ""

    //登录模式
    property bool bIsTouristModel: false

    //lastPage
    property int pageBeforoPopups: Enums.PAGE_UNKNOWN

    /****************启动页面****************/
    property string startPageBgColor: "#4756EB"
    property string startPageTitleBgColor: "#3A34B6"
    property string prograssBarColor: "#6A76EF"
    property string prograssBarLoadedColor: "#21BCA8"
    property string strSoftwareVersion: "版本号: V2.0.0.46"

    /************* 设备信息******************/
    //显示在首页的字符串
    property string deviceName: "default device"
    //设备名
    property string sportName: "仰卧起坐"
    //设备类型 例如: FHL  (后期改为commmon.h 中定义的变量名)
    property string devType: "STE"
    //设备类型 int
    property int iDevType: devUnknow
    //设备编号
    property string devId: "0000"
    //测试类型的单位， 例如肺活量为 -> ml, 初始化为空
    property string strMeasure: ""
    //设备连接模式
    property bool bDirectMode: true
    //设备使用模式
    property int testType: testAsyncMode
    //设备是否连接
    property bool bDevConnect: false
    //设备测试单位
    property string testUnit: "次"

    //设备测试模式
    property int iTestMode: Enums.TEST_MODE_NORMAL

    //设备信道配置
    property int channel: 29

    //设备最大连接数
    property int maxConnectCount: 8

    /****** 相关常量定义（需与后端保持一致） *********/
    readonly property int testSyncMode: 0
    readonly property int testAsyncMode: 1
    readonly property int testCountMode: 2 //记录测试模式数量

    //设备
    readonly property int devUnknow: 0 //未知设备
    readonly property int devBody: 1 //身高体重人体成分仪器，定义为体型仪
    readonly property int devVC: 2 //肺活量
    readonly property int devSBJ: 3 //立定跳远仪 standing broad jump
    readonly property int devSFB: 4 //坐位体前屈 Sitting forward bending
    readonly property int devPullUp: 5 //引体向上 Pull-ups
    readonly property int devSitUp: 6 //仰卧起坐 Sit-ups
    readonly property int devRunShort: 7 //短跑
    readonly property int devRunMid: 8 //中长跑
    readonly property int devRopeSkip: 9 //跳绳
    readonly property int devGrip: 10   //握力
    readonly property int devPushUp: 11 //俯卧撑
    readonly property int devVision: 12 //视力
    readonly property int devPVV: 13 //肺通气量 pulmonary ventilation volume
    readonly property int devFootBall: 14 // 足球
    readonly property int devBasketBall: 15 // 篮球
    readonly property int devVolleyBall: 16 // 排球
    readonly property int devPUO: 17 // 斜身引体向上
    readonly property int devSoldBall: 18 // 实心球
    readonly property int devVerticalJump: 19 // 纵跳摸高

    readonly property int devRunBack: 20 //往返跑

    //表格类型
    readonly property int directTestTable: 0 //直连测试表
    readonly property int directTouristTable: 1 //直连游客测试表
    readonly property int directScoreTable: 2 //直连成绩查询表
    readonly property int syncStudentTable: 3 //组连同步学生识别表
    readonly property int syncTestTable: 4 //组连同步测试成绩表
    readonly property int asyncStudentTable: 5 //组连异步完成测试学生表
    readonly property int asyncTestTable: 6 //组连异步测试中学生表
    readonly property int groupScoreTable: 7 //组连异步提交成绩表

    //单元格类型
    readonly property int normalCell: 0 //普通显示单元格
    readonly property int deviceCell: 1 //设备状态单元格
    readonly property int testCell: 2 //测试状态单元格
    readonly property int btnCell: 3 //操作按钮单元格
    readonly property int deleteCell: 4 //删除按钮单元格

    //短跑设备类型
    readonly property int runShort: 0 //50m跑
    readonly property int runBack: 1 //往返跑

    //测试状态
    readonly property int testNotStart: 0 //测试未开始
    readonly property int testPrepare: 1 //准备测试，中间可以设置等待时间
    readonly property int testChecked: 2 //自检完成(肺活量等直接开始，坐位体前屈还要等待复位标志完成) 提示开始测试
    readonly property int testInProcess: 3 //测试中
    readonly property int testCompleted: 4 //测试完成
    readonly property int testError: 5 //测试出错，一般是设备出现问题

    /***************测试页面*********************/

    //测试状态
    property string strPrepareState: "准备测试"
    property string strTestingState: "测试中"
    property string strEndTestState: "测试结束"

    //是否显示成绩
    property bool isDisplayScore: true

    //当前设备测试项目
    property string curDevTestItem: Enums.TestRunShort

    //中长跑测试长度
    property int devRunMideLength: 800

    //跳绳接收硬件数据延时, 以秒为数量级
    property int devRopeSkipRecvDelay: 20

    //签字保存路径
    property string paintSavePath: ""

    //当前体测项目的测试次数
    property int iCurTestCount: 0

    //    //当前测试结果框( 双框 )显示的结果，默认值为 "--"
    //    property string strDoubleRectRet: "--"

    //    //当前测试结果框( 单框 例如:肺活量 )显示的结果，默认值为 "0000"
    property int strsingleRectRet: 0000

    //当前测试结果
    property string strCurRet: "-1"

    //当前测试分数
    property string iCurTestGrade: "0.0"

    //成绩查询测试类型 int
    property int iQueryTestItem: Enums.ITEM_UNKNOW

    /***************指导视频的封面路径***********************/
    //人体成分指导视频封面
    property string bciPic: ""

    //肺活量指导视频封面
    property string vcPic: "qrc:/pic/videoCovers/VC_1.png"
    property string vcPic2: "qrc:/pic/videoCovers/VC_2.png"

    //引体向上指导视频封面
    property string pullUpPic: "qrc:/pic/videoCovers/PULL_UP_1.png"
    property string pullUpPic2: "qrc:/pic/videoCovers/PULL_UP_2.png"

    //立定跳远指导视频封面
    property string sbjPic: "qrc:/pic/videoCovers/SBJ_1.png"
    property string sbjPic2: "qrc:/pic/videoCovers/SBJ_2.png"

    //仰卧起坐分指导视频封面
    property string sitUpPic: "qrc:/pic/videoCovers/SIT_UP_1.png"
    property string sitUpPic2: "qrc:/pic/videoCovers/SIT_UP_2.png"

    //坐位体前屈指导视频封面
    property string sfbPic: "qrc:/pic/videoCovers/SFB_1.png"
    property string sfbPic2: "qrc:/pic/videoCovers/SFB_2.png"

    //仰卧起坐
    property string sitUp: "qrc:/pic/videoCovers/SIT_UP_1.png"
    property string sitUp2: "qrc:/pic/videoCovers/SIT_UP_2.png"

    //身高体重
    property string body1: "qrc:/pic/videoCovers/BODY_1.png"
    property string body2: "qrc:/pic/videoCovers/BODY_2.png"

    //界面无操作跳转倒计时
    property int pageSwitchCountTime: 30

    //提示界面跳转倒计时
    property int tipPopupCountTime: 10

    /************************犯规提示语句**************************************/
    property string strFoulVC: "重复吹气犯规"

    property bool bBatteryExisted: false
    property bool bCharging: false
    property int curDevElecQuantity: 90

    /**********************计时项目的开始计时时候的系统时间******************/
    property string startDateString: ""


    //2023.01.12 是否显示用户信息页面
    property bool userInfoPageExited: true

    //2023.01.12 当前用户是否已经进入过测试页面
    property bool curUserEnteredTestView: false

    /**********************************提交成绩弹窗类型**********************************/
    //当前设置的提交弹窗类型===============>   仅用于控制是否自动弹出弹窗
    property int currentCommitPopup_type: 0

    //带有签字版的提交弹窗
    readonly property int commitPopup_Signature: 0

    //不带有签字版的提交弹窗
    readonly property int commitPopup_value: 1

    //自动提交模式下, 自动提交的倒计时
    property int autoCommitCountTime: 3

    property bool isCommitAbnormal: false

    //是否可以取消成绩
    property int isDelBtnEnable: 1

    /********************启动软件时， 设置设备类型********************************/
    function setTestDeviceType() {
        iDevType = InterAction.getIDevType()
        console.log("global dev", iDevType)
        bDevConnect = true
        switch (iDevType) {
        case devVC:
            deviceName = "肺活量测试"
            sportName = "肺活量"
            devType = "FHL"
            testUnit = "ml"
            break
        case devSBJ:
            deviceName = "立定跳远测试"
            sportName = "立定跳远"
            devType = "LDTY"
            testUnit = "cm"
            break
        case devSFB:
            deviceName = "坐位体前屈测试"
            sportName = "坐位体前屈"
            devType = "ZWTQQ"
            testUnit = "cm"
            break
        case devPullUp:
            deviceName = "引体向上测试"
            sportName = "引体向上"
            devType = "YTXS"
            testUnit = "次"
            break
        case devSitUp:
            deviceName = "仰卧起坐测试"
            sportName = "仰卧起坐"
            devType = "YWQZ"
            testUnit = "次"
            break
        case devBody:
            deviceName = "身高体重体成分"
            sportName = "身高体重体成分测试"
            devType = "BODY"
            testUnit = "kg"

            break
        case devRunMid:
            deviceName = "中长跑测试"
            sportName = "中长跑跑步测试"
            devType = "RUNMID"
            testUnit = "s"
            break
        case devRunShort:
            deviceName = "50米短跑"
            sportName = "50米短跑测试"
            devType = "RUNSHORT"
            testUnit = "s"
            break
        case devRopeSkip:
            deviceName = "跳绳测试"
            sportName = "跳绳测试"
            devType = "ROPESKING"
            testUnit = "次"
            break
        case devGrip:
            deviceName = "握力测试"
            sportName = "握力测试"
            devType = "ROPEGPIP"
            testUnit = "kg"
            break
        case devPushUp:
            deviceName = "俯卧撑测试"
            sportName = "俯卧撑测试"
            devType = "PUSHUP"
            testUnit = "次"
            break
        case devVision:
            deviceName = "视力测试"
            sportName = "视力测试"
            devType = "VISION"
            testUnit = "视力"
            break
        case devPVV:
            deviceName = "肺活量测试"
            sportName = "肺活量"
            devType = "PVV"
            testUnit = "ml/s"
            break
        case devRunBack:
            deviceName = "往返跑"
            sportName = "往返跑测试"
            devType = "RUNBACK"
            testUnit = "s"
            break


        case devFootBall:
            deviceName = "足球"
            sportName = "往返跑测试"
            devType = "FOOTBALL"
            testUnit = "s"
            break

        case devBasketBall:
            deviceName = "篮球"
            sportName = "篮球测试"
            devType = "BASKETBALL"
            testUnit = "s"
            break

        case devVolleyBall:
            deviceName = "排球"
            sportName = "排球测试"
            devType = "VOLLEYBALL"
            testUnit = "次"
            break

        case devPUO:
            deviceName = "斜身引体向上"
            sportName = "斜身引体向上测试"
            devType = "PUO"
            testUnit = "次"
            break

        case devSoldBall:
            deviceName = "实心球"
            sportName = "实心球测试"
            devType = "SOLDBALL"
            testUnit = "m"
            break

        case devVerticalJump:
            deviceName = "纵跳摸高"
            sportName = "纵跳摸高测试"
            devType = "VERTICALJUMP"
            testUnit = "cm"
            break

        default:
            console.log("default")
            devType = "unKnown"
            bDevConnect = false
            testUnit = ""
            break
        }

        //身高体重测试项目，页面超时时间为2倍
        if(  devType === "BODY" ){
            pageSwitchCountTime = InterAction.getPageTimeout() * 2
        }else{
            pageSwitchCountTime = InterAction.getPageTimeout()
        }
        let info = { 'Common': iDevType }

        setTestConfig()
    }

    /***************************** 获取测试配置 *************************************/
    function setTestConfig() {
        isDisplayScore = InterAction.getIsDisplayScore()
        devRunMideLength = InterAction.getRunMideLength()
        devRopeSkipRecvDelay = InterAction.getRopeSkipRecvDelay()
        updateDevTestItem()
    }

    /*************************** 测试项目切换 *********************************/
    function updateDevTestItem() {
        curDevTestItem = InterAction.getDevTestItems(iDevType)

        if (iDevType == Enums.DEV_PVV) {
            if (curDevTestItem === Enums.TestPVVVC.toString()) {
                deviceName = "肺活量"
                sportName = "肺活量测试"
            } else if (curDevTestItem === Enums.TestPVVFVC.toString()) {
                deviceName = "用力肺活量"
                sportName = "用力肺活量测试"
            } else {
                deviceName = "最大肺通气量"
                sportName = "最大肺通气量测试"
            }
        }
        else if (iDevType == Enums.DEV_RUN_SHORT) {
            if (curDevTestItem === Enums.TestRunShort.toString()) {
                deviceName = "50米短跑"
                sportName = "50米短跑测试"
                iTestMode = Enums.TEST_MODE_NORMAL
            }
            else if(curDevTestItem === Enums.TestRunBack.toString())
            {
                deviceName = "往返跑"
                sportName = "往返跑测试"
                iTestMode = Enums.TEST_MODE_NORMAL
            }
            else if(curDevTestItem === Enums.TestRunShort60.toString())
            {
                deviceName = "60米短跑"
                sportName = "60米短跑测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.TestRunShort100.toString())
            {
                deviceName = "100米短跑"
                sportName = "100米短跑测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.TestRunHurdles100.toString())
            {
                deviceName = "100米跨栏"
                sportName = "100米跨栏测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.TestRunHurdles110.toString())
            {
                deviceName = "110米跨栏"
                sportName = "110米跨栏测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.TestRunShort200.toString())
            {
                deviceName = "200米跑步"
                sportName = "200米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.TestRunShort400.toString())
            {
                deviceName = "400米跑步"
                sportName = "400米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.TestRunShort4_100.toString())
            {
                deviceName = "4x100米跑步"
                sportName = "4x100米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.ITEM_RUN_BACK_4_10.toString())
            {
                deviceName = "4x10米跑步"
                sportName = "4x10米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
        }
        else if (iDevType == Enums.DEV_RUN_MID)
        {
            if(curDevTestItem === Enums.ITEM_RUN_800.toString())
            {
                deviceName = "800米跑步"
                sportName = "800米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.ITEM_RUN_1500.toString())
            {
                deviceName = "1500米跑步"
                sportName = "1500米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.ITEM_RUN_1000.toString())
            {
                deviceName = "1000米跑步"
                sportName = "1000米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else if(curDevTestItem === Enums.ITEM_RUN_2000.toString())
            {
                deviceName = "2000米跑步"
                sportName = "2000米跑步测试"
                iTestMode = Enums.TEST_MODE_COMPET
            }
            else
            {
                deviceName = "中长跑测试"
                sportName = "中长跑跑步测试"
                iTestMode = Enums.TEST_MODE_NORMAL
            }
        }
    }

    /******************核对设备显示的对应界面( 针对双矩形 )******************************/
    function checkVisible(_deviceType) {
        if (_deviceType === devType) {
            return true
        } else {
            return false
        }
    }

    //核对是否为双矩形类设备
    function checkDoubleRect() {
        if (devType === "LDTY" || devType === "ZWTQQ" || devType
                === "YTXS" || devType === "YWQZ" || devType === "BODY" || devType
                === "ROPEGPIP" || devType === "PUSHUP" || devType === "VISION") {
            return true
        } else {
            return false
        }
    }

    // 是否为肺活量设备(单矩形)
    function checkFHL() {
        if (devType === "FHL") {
            return true
        } else {
            return false
        }
    }

    // 是否为视力设备(单矩形)
    function checkVision() {
        if (devType === "VISION") {
            return true
        } else {
            return false
        }
    }

    //是否为跳远
    function checkTY() {
        if (devType === "LDTY") {
            return true
        } else {
            return false
        }
    }

    /***********************点击开始测试按钮**********************************/
    function clickStartTestBtn() {
        InterAction.clickStartTestBtn()
    }

    /**************************对学生信息的操作***************************/
    function getCurStuNmae() {
        strCurStudentName = InterAction.getCurStuName()
    }

    function getCurStuAge() {
        strCurStudentAge = InterAction.getCurStuAge()
    }

    function getCurStuGrade() {
        strCurStudentGrade = InterAction.getCurStuGrade()
    }

    function getCurStuClass() {
        strCurStudentClass = InterAction.getCurStuClass()
    }

    function getCurStuSchoolNO() {
        strCurStudentNumber = InterAction.getCurStuSchoolNO()
    }

    function getCurStuSex() {
        bCurStudentSex = InterAction.getCurStuSex()
    }

    /***********************刷新当前学生的所有信息******************************/
    function reFreshCurStuInfo() {
        getCurStuGrade()
        getCurStuClass()
        getCurStuNmae()
        getCurStuAge()
        getCurStuSex()
        getCurStuSchoolNO()
    }
    function getCurSoftWareVersion() {
        strSoftwareVersion = InterAction.getSoftWareVersion()
    }

    /*********************取四位数的单独某一位**************************/
    function extractNumber( index, num ) {
        if (!num) {
            num = 0
        }

        var ret = 0

        if (num > 9999 || num < 0) {
            return -1
        }

        switch (index) {
        case 4:
            ret = parseInt(num) % 10
            break
        case 3:
            ret = parseInt(num / 10) % 10
            break
        case 2:
            ret = parseInt(num / 100) % 10
            break
        case 1:
            ret = parseInt(num / 1000)
            break
        default:
            ret = -1
            break
        }
        return ret
    }

    /***************************获取当前测试类型的单位*********************************/
    function getMeaurement(devType) {

        switch (devType) {
        case "":
            return "(单位)"
        case "FHL":
            return "(ml)"
        case "LDTY":
            return "(cm)"
        case "ZWTQQ":
            return "(cm)"
        case "YTXS":
            return "(次)"
        case "YWQZ":
            return "(次)"
        case "RUNSHORT":
        case "RUNBACK":
            return "(s)"
        case "1000RUN":
        case "800RUN":
            return "(s)"
        case "ROPESKING":
        case "PUSHUP":
            return "(次)"
        case "ROPEGPIP":
            return "(kg)"
        case "PVV":
            return "(ml/s)"
        default:
            return ""
        }
    }

    function getMeaurementByDevName(name) {

        switch (name) {
        case "ROPEGPIP":
            return "kg"
        }
    }
    /******************************根据设备类型不同，按照不同的方式的解析结果数据*************************/
    function parseRet(retparseType) {
        //结果转为int
        if (parseType === "int") {
            ret = parseInt(ret)
            return ret
        } //结果转为float,    JavaScript 没有转Double? else if (parseType === "float") {
        ret = parseFloat(ret)
        return ret
    }

    /**************************根据设备类型返回响应的指导视频封面路径**********************/
    function getSuitableVideoCover() {
        var videoCoverPath = ""
        switch (devType) {
        case "FHL":
            videoCoverPath = vcPic
            break
        case "YTXS":
            videoCoverPath = pullUpPic
            break
        case "LDTY":
            videoCoverPath = sbjPic
            break
        case "ZWTQQ":
            videoCoverPath = sfbPic
            break
        case "YWQZ":
            videoCoverPath = sitUp
            break
        case "BODY":
            videoCoverPath = body1
            break
        default:
            videoCoverPath = "qrc:/pic/GuideVideo.png"
        }
        return videoCoverPath
    }

    function getSuitableVideoCover2() {
        var videoCoverPath = ""
        switch (devType) {
        case "FHL":
            videoCoverPath = vcPic2
            break
        case "YTXS":
            videoCoverPath = pullUpPic2
            break
        case "LDTY":
            videoCoverPath = sbjPic2
            break
        case "ZWTQQ":
            videoCoverPath = sfbPic2
            break
        case "YWQZ":
            videoCoverPath = sitUp2
            break
        case "BODY":
            videoCoverPath = body2
            break
        default:
            videoCoverPath = "qrc:/pic/GuideVideo.png"
        }
        return videoCoverPath
    }

    /**************************根据设备类型返回相应的测试类型(例如: 肺活量为: 2 )**********************/
    function translateDevTypeNameToShort() {
        var index = -1
        switch (devType) {
        case "FHL":
            index = 2
            break
        case "YTXS":
            index = 5
            break
        case "LDTY":
            index = 3
            break
        case "ZWTQQ":
            index = 4
            break
        case "BODY":
            index = 1
            break
        case "YWQZ":
            index = 6
            break
        case "RUNSHORT":
            index = 7
            break
        case "ROPESKING":
            index = 8
            break
        default:
            index = -1
        }
        return index
    }

    //秒转换为时分秒  用于计时
    function getTimerText(time) {
        var timeStr
        var hour
        var min
        var sec

        if (time > -1) {
            hour = Math.floor(time / 3600)
            min = Math.floor((time / 60) % 60)
            sec = time % 60
            if (hour < 10) {
                timeStr = "0" + hour + ":"
            }
            if (min < 10) {
                timeStr += "0"
            }
            timeStr += min + ":"
            if (sec < 10) {
                timeStr += "0"
            }
            timeStr += sec.toFixed(0)
        }

        return timeStr
    }
    /*********************根据设备类型返回设备名称( 用于获取设备中文名称, 在播放视频界面 )*****************/
    function getDevChineseName() {
        var devType = InterAction.getIDevType()
        var chineseName
        switch (devTypeName) {
        case devVC:
            chineseName = "肺活量测试"
            break
        case devSBJ:
            chineseName = "立定跳远"
            break
        case devSFB:
            chineseName = "坐位体前屈"
            break
        case devPullUp:
            chineseName = "引体向上"
            break
        case devSitUp:
            chineseName = "仰卧起坐"
            break
        case devBody:
            chineseName = "身高体重人体成份"
            break
        case devRunMid:
            chineseName = "中长跑"
            break
        default:
            console.log("default, function -> GlobalProperty, getDevChineseName")
            break
        }
        return chineseName
    }

    /******************判断是否为多人测试， 目前( 2021/10/29 )主要针对 跳绳，50米跑******************/
    function checkMultiStudentsTest() {
        var devType = InterAction.getIDevType()
        if (devType === devRunShort || devType === devRopeSkip) {
            return true
        } else {
            return false
        }
    }

    /**************************根据设备类型，来判断是否需要显示倒计时，仰卧起坐，引体向上需要显示**********************/
    function checkDisplayCountTime() {
        if (devType === "YWQZ" || devType === "YTXS" || devType === "PUSHUP") {
            return true
        } else {
            return false
        }
    }

    /*******************临时用这获取单位******************************/
    function getUnitByDevType() {
        var unit = "次"
        switch (iDevType) {
        case devBody:
            unit = "kg"
            break
        case devVC:
            unit = "ml"
            break
        case devSBJ:
        case devSFB:
        case devVerticalJump:
            unit = "cm"
            break
        case devPullUp:
        case devSitUp:
        case devPushUp:
        case devRopeSkip:
        case devVolleyBall:
        case devPUO:
            unit = "次"
            break
        case devGrip:
            unit = "kg"
            break
        case devVision:
            unit = "分"
            break
        case devPVV:
            unit = "ml/s"
            break
        case devRunBack:
        case devRunShort:
        case devRunMid:
        case devFootBall:
        case devBasketBall:
            unit = "s"
            break
        case devSoldBall:
            unit = "m"
            break
        default:
            break
        }
        return unit
    }

    function getUnitByTestItem(testItem)
    {
        var unit = "次"
        switch (testItem) {
        case Enums.ITEM_WEIGHT:
            unit = "kg"
            break
        case Enums.ITEM_HEIGHT:
            unit = "kg"
            break
        case Enums.ITEM_VC:
            unit = "ml"
            break
        case Enums.ITEM_SBJ:
        case Enums.ITEM_SFB:
            unit = "cm"
            break
        case Enums.ITEM_PULL_UP:
        case Enums.ITEM_SIT_UP:
        case Enums.ITEM_PUSHUP:
        case Enums.ITEM_ROPE_SKIP:
        case Enums.ITEM_VOLLEYBALL:
        case Enums.ITEM_PUO:
        case Enums.ITEM_SOLIDBALL:
        case Enums.ITEM_VJUMP:
            unit = "次"
            break
        case Enums.ITEM_GRIP:
            unit = "kg"
            break
        case Enums.ITEM_EYESIGHT:
            unit = "分"
            break
        case Enums.ITEM_PVV:
            unit = "ml/s"
            break


        case Enums.ITEM_RUN_50:
        case Enums.ITEM_RUN_BACK_8_50:
        case Enums.ITEM_RUN_60:
        case Enums.ITEM_RUN_100:
        case Enums.ITEM_RUN_200:
        case Enums.ITEM_RUN_HURDLES_100:
        case Enums.ITEM_RUN_HURDLES_110:
        case Enums.ITEM_RUN_BACK_4_10:
        case Enums.ITEM_FOOTBALL:
        case Enums.ITEM_BASKETBALL:
            unit = "s"
            break

        case Enums.ITEM_RUN_400:
        case Enums.ITEM_RUN_4_100:
        case Enums.ITEM_RUN_1000:
        case Enums.ITEM_RUN_800:
        case Enums.ITEM_RUN_1500:
        case Enums.ITEM_RUN_2000:
            unit = '分·秒'
            break
        default:
            break
        }
        return unit
    }

    /****************是否为单项多人测试, 比如跳绳， 多项多人测试有短跑(往返跑),中长跑(1000,800)***********************/
    function checkSingleTestItemMulti() {
        if (devType === "ROPESKING") {
            return true
        } else {
            return false
        }
    }

    /***************从系统时间获取时间差**************************/
    function getTimeDifference(startTimeendTime) {
        //        console.log( "start..." )
        var startDate = new Date(startTime)
        var endDate = new Date(endTime)

        var startMin = startDate.getMinutes()
        var startSec = startDate.getSeconds() + (startMin * 60)

        var endMin = endDate.getMinutes()
        var endSec = endDate.getSeconds() + (endMin * 60)

        var difference = endSec - startSec
        //        console.log( "startDate = " + startTime )
        //        console.log( "endDate = " + endDate )
        //        console.log( "ret = ", difference )
        return difference
    }

    /***************获取对应设备文件名**************************/
    /**********返回名需与qml中添加设备名保持一致！！！**************/
    function getDevQMLFileName() {
        switch( iDevType ){
        case devBody:
            return "DevBody"

        case devVC:
            return "DevVC"

        case devSBJ:
            return "DevSBJ"

        case devSFB:
            return "DevSFB"

        case devPullUp:
            return "DevPullUp"

        case devSitUp:
            return "DevSitUp"

        case devRunShort:
            return "DevRunShort"

        case devRunMid:
            return "DevRunMid"

        case devRopeSkip:
            return "DevRopeSkip"

        case devGrip:
            return "DevGrip"

        case devPushUp:
            return "DevPushUp"

        case devVision:
            return "DevVision"

        case devPVV:
            return "DevPVV"

        case devFootBall:
            return "DevFootball"

        case devBasketBall:
            return "DevBasketball"

        case devVolleyBall:
            return "DevVolleyball"

        case devPUO:
            return "DevPUO"

        case devSoldBall:
            return "DevSoldball"

        case devVerticalJump:
            return "DevVerticalJump"
        default:
            return ""
        }
    }

    function getDevTypeFileName() {
        if (bDirectMode) {
            return "DirectMode"
        } else {
            return "GroupMode"
        }
    }

    function getTestTypeFileName() {
        if (testType === testSyncMode) {
            return "syncMode"
        } else {
            return "asyncMode"
        }
    }

    /******************************所有设备表格配置**********************************/
    //新增设备需在此处加入对应表格配置数据
    //直连成绩表-普通设备
    function getDirectTestTableCfg(devType)
    {
        let tableCfg = {}

        //直连中测试表仅展示
        tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, normalCell, normalCell]

        //根据设备类型加载当前表格配置
        switch(devType) {
        case devBody:
            tableCfg["tableCount"] = 2
            tableCfg["widthArray"] = [0.5, 0.5, 0, 0, 0, 0]
            tableCfg["headName"] = ["项目", "结果", "", "", "", "", ""]
            break
        case devVC:
        case devSBJ:
        case devSFB:
        case devPullUp:
        case devSitUp:
        case devGrip:
        case devPushUp:
        case devPUO:
        case devSoldBall:
            tableCfg["tableCount"] = isDisplayScore ? 3 : 2
            tableCfg["widthArray"] = isDisplayScore ? [0.3, 0.35, 0.35, 0, 0, 0] : [0.5, 0.5, 0, 0, 0, 0]
            tableCfg["headName"] = ["序次", "结果("+ getUnitByDevType() + ")", "成绩(分)", "", "", "", ""]
            break
        case devVision:
            tableCfg["tableCount"] = 3
            tableCfg["widthArray"] = [0.3, 0.35, 0.35, 0, 0, 0]
            tableCfg["headName"] = ["序次", "左眼视力", "右眼视力", "", "", "", ""]
            break
        case devFootBall:
        case devBasketBall:
        case devVolleyBall:
        case devVerticalJump:
            tableCfg["tableCount"] = isDisplayScore ? 4 : 3
            tableCfg["widthArray"] = isDisplayScore ? [0.25, 0.25, 0.3, 0.2, 0, 0, 0] : [0.3, 0.35, 0.35, 0, 0, 0]
            tableCfg["headName"] = ["序次", "结果("+ getUnitByDevType() + ")", "违规判定(次)", "成绩(分)", "", "", ""]
            break
        default:
            tableCfg["tableCount"] = isDisplayScore ? 3 : 2
            tableCfg["widthArray"] = isDisplayScore ? [0.2, 0.2, 0.3, 0.3, 0, 0, 0] : [0.3, 0.35, 0.35, 0, 0, 0]
            tableCfg["headName"] = ["序次", "结果", "成绩", "", "", "", ""]
            break
        }
        JSON.stringify(tableCfg)
        return tableCfg
    }

    //直连游客测试表
    function getDirectTouristTableCfg(devType)
    {
        let tableCfg = {}

        //直连中测试表仅展示
        tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, normalCell, normalCell]

        //根据设备类型加载当前表格配置
        switch(devType) {
        case devBody:
            tableCfg["tableCount"] = 2
            tableCfg["widthArray"] = [0.5, 0.5, 0, 0, 0, 0]
            tableCfg["headName"] = ["项目", "结果", "", "", "", "", ""]
            break
        case devVC:
        case devSBJ:
        case devSFB:
        case devPullUp:
        case devSitUp:
        case devGrip:
        case devPushUp:
        case devPUO:
        case devSoldBall:
        case devVerticalJump:
            tableCfg["tableCount"] = 2
            tableCfg["widthArray"] = [0.5, 0.5, 0, 0, 0, 0]
            tableCfg["headName"] = ["序次", "结果("+ getUnitByDevType() + ")", "", "", "", "", ""]
            break
        case devVision:
            tableCfg["tableCount"] = 3
            tableCfg["widthArray"] = [0.3, 0.35, 0.35, 0, 0, 0]
            tableCfg["headName"] = ["序次", "左眼视力", "右眼视力", "", "", "", ""]
            break
        case devFootBall:
        case devBasketBall:
        case devVolleyBall:
            tableCfg["tableCount"] = 3
            tableCfg["widthArray"] = [0.3, 0.35, 0.35, 0, 0, 0]
            tableCfg["headName"] = ["序次", "结果(" + getUnitByDevType() +")", "违规判定(次)", "", "", "", ""]
            break
        case devPVV:
            tableCfg["tableCount"] = 4
            tableCfg["widthArray"] = [0.25, 0.25, 0.2, 0.3, 0, 0, 0]
            tableCfg["headName"] = ["测试指标", "结果", "参考值", "结果解读", "", "", ""]
            break
        default:
            tableCfg["tableCount"] = 2
            tableCfg["widthArray"] = [0.5, 0.5, 0, 0, 0, 0, 0]
            tableCfg["headName"] = ["序次", "结果("+ getUnitByDevType() + ")", "", "", "", "", ""]
            break
        }
        JSON.stringify(tableCfg)
        return tableCfg
    }

    //直连成绩查询表 根据体测项目查询表格配置
    function getDirectScoreTableCfg(testItem)
    {
        let tableCfg = {}

        //直连中成绩表仅展示
        tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, normalCell, normalCell]
        //根据设备类型加载当前表格配置
        switch(testItem) {
        case Enums.ITEM_HEIGHT:
            tableCfg["tableCount"] = 3
            tableCfg["widthArray"] = [0.3, 0.35, 0.35, 0, 0, 0]
            tableCfg["headName"] = ["测试时间", "身高(cm)", "体重(kg)", "", "", "", ""]
            break
        case Enums.ITEM_EYESIGHT:
            tableCfg["tableCount"] = isDisplayScore ? 3 : 2
            tableCfg["widthArray"] = isDisplayScore ? [0.3, 0.35, 0.35, 0, 0, 0] : [0.5, 0.5, 0, 0, 0, 0]
            tableCfg["headName"] = ["测试时间", "结果", "成绩(分)", "", "", "", ""]
            break
        case Enums.ITEM_FOOTBALL:
        case Enums.ITEM_BASKETBALL:
        case Enums.ITEM_VOLLEYBALL:
            tableCfg["tableCount"] = isDisplayScore ? 4 : 3
            tableCfg["widthArray"] = isDisplayScore ? [0.2, 0.2, 0.3, 0.3, 0, 0, 0] : [0.3, 0.35, 0.35, 0, 0, 0]
            tableCfg["headName"] = ["测试时间", "结果("+ getUnitByTestItem(testItem) + ")", "违规判定(次)", "成绩(分)", "", "", ""]
            break
        default:
            tableCfg["tableCount"] = isDisplayScore ? 3 : 2
            tableCfg["widthArray"] = isDisplayScore ? [0.3, 0.35, 0.35, 0, 0, 0] : [0.5, 0.5, 0, 0, 0, 0]
            tableCfg["headName"] = ["测试时间", "结果("+ getUnitByTestItem(testItem) + ")", "成绩(分)", "", "", "", ""]
            break
        }
        JSON.stringify(tableCfg)
        return tableCfg
    }

    //组连同步学生信息表
    function getSyncStudentTableCfg(devType)
    {
        let tableCfg = {}

        //根据设备类型加载当前表格配置
        switch(devType) {
        case devBody:
        case devVC:
        case devSBJ:
        case devSFB:
        case devPullUp:
        case devSitUp:
        case devRunMid:
        case devRopeSkip:
        case devGrip:
        case devPushUp:
        case devVision:
        case devPUO:
        case devSoldBall:
        case devVerticalJump:
            tableCfg["tableCount"] = 5
            tableCfg["widthArray"] = [0.15, 0.2, 0.25, 0.2, 0.2, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "设备状态", "操作", "", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, deviceCell, deleteCell, normalCell]
            break
        case devRunShort:
            tableCfg["tableCount"] = 5
            tableCfg["widthArray"] = [0.15, 0.2, 0.25, 0.2, 0.2, 0]
            tableCfg["headName"] = ["跑道号", "姓名", "准考证号", "设备状态", "操作", "", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, deviceCell, deleteCell, normalCell]
            break
        default:
            tableCfg["tableCount"] = 5
            tableCfg["widthArray"] = [0.15, 0.2, 0.25, 0.2, 0.2, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "设备状态", "操作", "", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, deviceCell, deleteCell, normalCell]
            break
        }
        JSON.stringify(tableCfg)
        return tableCfg
    }

    //组连同步学生测试表
    function getSyncTestTableCfg(devType)
    {
        let tableCfg = {}
        //根据设备类型加载当前表格配置
        switch(devType) {
        case devBody:
            tableCfg["tableCount"] = 6
            tableCfg["widthArray"] = [0.15, 0.175, 0.2, 0.1625, 0.1625, 0.15]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "身高", "体重", "状态", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell]
            break
        case devVC:
        case devSBJ:
        case devSFB:
        case devGrip:
            tableCfg["tableCount"] = 6
            tableCfg["widthArray"] = [0.15, 0.175, 0.225, 0.125, 0.175, 0.15]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "次数", "结果("+ getUnitByDevType() + ")", "状态", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell]
            break
        case devPullUp:
        case devSitUp:
        case devPushUp:
        case devPUO:
        case devSoldBall:
        case devVerticalJump:
            tableCfg["tableCount"] = 5
            tableCfg["widthArray"] = [0.15, 0.175, 0.25, 0.25, 0.175, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "结果("+ getUnitByDevType() + ")", "状态", "", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, testCell, normalCell]
            break
        case devRunShort:
            if(curDevTestItem === Enums.TestRunBack.toString() || curDevTestItem === Enums.ITEM_RUN_BACK_4_10.toString() ||
                    curDevTestItem === Enums.ITEM_RUN_BACK_8_50.toString())
            {
                tableCfg["tableCount"] = 6
                tableCfg["widthArray"] = [0.15, 0.175, 0.2, 0.1, 0.2, 0.175]
                tableCfg["headName"] = ["跑道号", "姓名", "准考证号", "圈数", "结果", "状态", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell]
            }
            else
            {
                tableCfg["tableCount"] = 5
                tableCfg["widthArray"] = [0.15, 0.175, 0.3, 0.2, 0.175, 0]
                tableCfg["headName"] = ["跑道号", "姓名", "准考证号", "结果", "状态", "", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, testCell, normalCell]
            }
            break
        case devRunMid:
            tableCfg["tableCount"] = 6
            tableCfg["widthArray"] = [0.15, 0.175, 0.2, 0.1, 0.2, 0.175]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "圈数", "结果", "状态", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell]
            break
        case devRopeSkip:
            tableCfg["tableCount"] = 6
            tableCfg["widthArray"] = [0.15, 0.175, 0.225, 0.15, 0.15, 0.15]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "结果(次)", "状态", "电量(%)", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, testCell, normalCell]
            break
        case devVision:
            tableCfg["tableCount"] = 6
            tableCfg["widthArray"] = [0.15, 0.175, 0.2, 0.1625, 0.1625, 0.15]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "左眼", "右眼", "状态", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell]
            break
        default:
            tableCfg["tableCount"] = 6
            tableCfg["widthArray"] = [0.15, 0.175, 0.225, 0.125, 0.175, 0.15]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "次数", "结果", "状态", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell]
            break
        }
        JSON.stringify(tableCfg)
        return tableCfg
    }

    function getAsyncStudentTableCfg(devType)
    {
        let tableCfg = {}
        tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell,normalCell]
        //根据设备类型加载当前表格配置
        switch(devType) {
        case devBody:
            tableCfg["tableCount"] = isDisplayScore? 6 : 5
            tableCfg["widthArray"] = isDisplayScore? [0.15, 0.175, 0.225, 0.15, 0.15, 0.15] : [0.2, 0.15, 0.3, 0.2, 0.15, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "身高", "体重", "成绩", ""]

            break
        case devVC:
        case devSBJ:
        case devSFB:
        case devGrip:
            tableCfg["tableCount"] = isDisplayScore? 6 : 5
            tableCfg["widthArray"] = isDisplayScore? [0.15, 0.175, 0.2, 0.125, 0.175, 0.175] : [0.15, 0.15, 0.25, 0.25, 0.2, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "次数", "结果("+ getUnitByDevType() + ")", "成绩(分)", ""]
            break
        case devPullUp:
        case devSitUp:
        case devRopeSkip:
        case devPushUp:
        case devPUO:
        case devSoldBall:
        case devVerticalJump:
            tableCfg["tableCount"] = isDisplayScore? 5 : 4
            tableCfg["widthArray"] = isDisplayScore? [0.15, 0.175, 0.25, 0.25, 0.175, 0] : [0.2, 0.2, 0.3, 0.3, 0, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "结果("+ getUnitByDevType() + ")", "成绩(分)", "", ""]
            break
        case devRunShort:
            if(curDevTestItem === Enums.TestRunBack.toString() || curDevTestItem === Enums.ITEM_RUN_BACK_4_10.toString() ||
                    curDevTestItem === Enums.ITEM_RUN_BACK_8_50.toString())
            {
                tableCfg["tableCount"] =  isDisplayScore? 6 : 5
                tableCfg["widthArray"] = isDisplayScore ? [0.15, 0.15, 0.2, 0.1, 0.2, 0.2] : [0.2, 0.15, 0.25, 0.15, 0.25, 0]
                tableCfg["headName"] = ["跑道号", "姓名", "准考证号", "圈数", "结果", "成绩(分)", ""]
            }
            else
            {
                tableCfg["tableCount"] =  isDisplayScore? 5 : 4
                tableCfg["widthArray"] = isDisplayScore ? [0.15, 0.15, 0.3, 0.2, 0.2, 0] : [0.2, 0.2, 0.35, 0.25, 0, 0]
                tableCfg["headName"] = ["跑道号", "姓名", "准考证号", "结果", "成绩(分)", "", ""]
            }
            break
        case devRunMid:
            tableCfg["tableCount"] =  isDisplayScore? 6 : 5
            tableCfg["widthArray"] = isDisplayScore ? [0.15, 0.175, 0.2, 0.125, 0.2, 0.15] : [0.2, 0.15, 0.25, 0.15, 0.25, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "圈数", "结果", "成绩(分)", ""]
            break
        case devVision:
            tableCfg["tableCount"] =  isDisplayScore? 6 : 5
            tableCfg["widthArray"] = isDisplayScore ? [0.15, 0.175, 0.2, 0.125, 0.2, 0.15] : [0.2, 0.15, 0.25, 0.15, 0.25, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "左眼", "右眼", "成绩(分)", ""]
            break
        default:
            tableCfg["tableCount"] = isDisplayScore? 6 : 5
            tableCfg["widthArray"] = isDisplayScore? [0.15, 0.175, 0.2, 0.125, 0.175, 0.175] : [0.15, 0.15, 0.25, 0.25, 0.2, 0]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "次数", "结果("+ getUnitByDevType() + ")", "成绩(分)", ""]
            break
        }
        JSON.stringify(tableCfg)
        return tableCfg
    }

    function getAsyncTestTableCfg(devType)
    {
        let tableCfg = {}
        //根据设备类型加载当前表格配置
        switch(devType) {
        case devBody:
            tableCfg["tableCount"] = 7
            tableCfg["widthArray"] = [0.125, 0.15, 0.175, 0.15, 0.15, 0.15, 0.1]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "身高", "体重", "状态", "操作"]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            break
        case devVC:
        case devSBJ:
        case devSFB:
        case devGrip:
            tableCfg["tableCount"] = 7
            tableCfg["widthArray"] = [0.125, 0.135, 0.2, 0.1, 0.175, 0.15, 0.115]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "次数", "结果("+ getUnitByDevType() + ")", "状态", "操作"]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            break
        case devPullUp:
        case devSitUp:
        case devPushUp:
        case devPUO:
        case devSoldBall:
        case devVerticalJump:
            tableCfg["tableCount"] = 6
            tableCfg["widthArray"] = [0.125, 0.175, 0.20, 0.20, 0.15, 0.15]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "结果("+ getUnitByDevType() + ")", "状态", "操作", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            break
        case devRunShort:
            if(curDevTestItem === Enums.TestRunBack.toString() || curDevTestItem === Enums.ITEM_RUN_BACK_4_10.toString()||
                    curDevTestItem === Enums.ITEM_RUN_BACK_8_50.toString())
            {
                tableCfg["tableCount"] = 7
                tableCfg["widthArray"] = [0.125, 0.15, 0.2, 0.1, 0.175, 0.125, 0.125]
                tableCfg["headName"] = ["跑道号", "姓名", "准考证号", "圈数", "结果", "状态", "操作"]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            }
            else
            {
                tableCfg["tableCount"] = 6
                tableCfg["widthArray"] = [0.15, 0.15, 0.2, 0.2, 0.15, 0.15]
                tableCfg["headName"] = ["跑道号", "姓名", "准考证号", "结果", "状态", "操作", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            }
            break
        case devRunMid:
            tableCfg["tableCount"] = 7
            tableCfg["widthArray"] = [0.125, 0.15, 0.2, 0.1, 0.175, 0.125, 0.125]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "圈数", "结果", "状态", "操作"]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            break
        case devRopeSkip:
            tableCfg["tableCount"] = 7
            tableCfg["widthArray"] = [0.125, 0.15, 0.2, 0.15, 0.125, 0.15, 0.1]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号" ,"结果(次)", "状态", "电量(%)", "操作"]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, testCell, normalCell, deleteCell]
            break
        case devVision:
            tableCfg["tableCount"] = 7
            tableCfg["widthArray"] = [0.125, 0.15, 0.2, 0.15, 0.15, 0.125, 0.1]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号" ,"左眼视力", "右眼视力", "状态", "操作"]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            break
        default:
            tableCfg["tableCount"] = 7
            tableCfg["widthArray"] = [0.125, 0.135, 0.2, 0.1, 0.175, 0.15, 0.115]
            tableCfg["headName"] = ["设备号", "姓名", "准考证号", "次数", "结果("+ getUnitByDevType() + ")", "状态", "操作"]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, testCell, deleteCell]
            break
        }
        JSON.stringify(tableCfg)
        return tableCfg
    }

    function getGroupScoreTableCfg(devType)
    {
        let tableCfg = {}

        switch(devType) {
        case devBody:
            tableCfg["tableCount"] = isDelBtnEnable ? 6 : 5
            tableCfg["widthArray"] = isDelBtnEnable ? [0.1125, 0.15, 0.2, 0.1625, 0.1625, 0.2125] : [0.15, 0.15, 0.25, 0.25, 0.2, 0]
            tableCfg["headName"] = ["序号", "姓名", "准考证号", "身高", "体重", "操作", ""]
            tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, btnCell]
            break
        case devVC:
        case devSBJ:
        case devSFB:
        case devPullUp:
        case devSitUp:
        case devRunMid:
        case devRunShort:
        case devRopeSkip:
        case devGrip:
        case devPushUp:
        case devPVV:
        case devPUO:
        case devSoldBall:
        case devVerticalJump:
            if(isDisplayScore)
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 6 : 5
                tableCfg["widthArray"] = isDelBtnEnable ? [0.125, 0.15, 0.2, 0.175, 0.15, 0.2] : [0.15, 0.15, 0.25, 0.25, 0.2, 0]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "结果("+ getUnitByDevType() + ")", "成绩", "操作", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell, normalCell, normalCell, btnCell]
            }
            else
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 5 : 4
                tableCfg["widthArray"] = isDelBtnEnable ? [0.15, 0.15, 0.25, 0.25, 0.2, 0, 0] : [0.2, 0.2, 0.3, 0.3, 0, 0, 0]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "结果("+ getUnitByDevType() + ")", "操作", "", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell,
                                             normalCell, btnCell, normalCell]
            }
            break
        case devRunMid:
        case devRunShort:
            if(isDisplayScore)
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 6 : 5
                tableCfg["widthArray"] = isDelBtnEnable ? [0.1125, 0.15, 0.2, 0.2125, 0.15, 0.2] : [0.15, 0.15, 0.25, 0.25, 0.2, 0]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "结果", "成绩", "操作", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell,
                                             normalCell, btnCell, btnCell]
            }
            else
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 5 : 4
                tableCfg["widthArray"] = isDelBtnEnable ? [0.15, 0.15, 0.25, 0.25, 0.2, 0] : [0.2, 0.2, 0.3, 0.3, 0, 0]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "结果", "操作", "", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell,
                                             normalCell, btnCell, normalCell]
            }
            break
        case devVision:
            if(isDisplayScore)
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 7 : 6
                tableCfg["widthArray"] = isDelBtnEnable ? [0.1, 0.15, 0.2, 0.125, 0.125, 0.15, 0.15] : [0.15, 0.15, 0.2, 0.2, 0.15, 0.15]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "左眼视力", "右眼视力", "成绩", "操作"]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell,
                                             normalCell, normalCell, normalCell, btnCell]
            }
            else
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 6 : 5
                tableCfg["widthArray"] = isDelBtnEnable ? [0.1125, 0.15, 0.2, 0.2125, 0.15, 0.2] : [0.15, 0.15, 0.25, 0.25, 0.2, 0]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "左眼视力", "右眼视力",  "操作", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell,
                                             normalCell, normalCell, btnCell]
            }
            break
        default:
            if(isDisplayScore)
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 6 : 5
                tableCfg["widthArray"] = isDelBtnEnable ? [0.125, 0.15, 0.2, 0.175, 0.15, 0.2] : [0.15, 0.15, 0.25, 0.25, 0.2, 0]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "结果("+ getUnitByDevType() + ")", "成绩", "操作", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell,
                                             normalCell, normalCell, btnCell]
            }
            else
            {
                tableCfg["tableCount"] = isDelBtnEnable ? 5 : 4
                tableCfg["widthArray"] = isDelBtnEnable ? [0.15, 0.15, 0.25, 0.25, 0.2, 0, 0] : [0.2, 0.2, 0.3, 0.3, 0, 0, 0]
                tableCfg["headName"] = ["序号", "姓名", "准考证号", "结果("+ getUnitByDevType() + ")", "操作", "", ""]
                tableCfg["cellTypeArray"] = [normalCell, normalCell, normalCell,
                                             normalCell, btnCell, normalCell]
            }
            break
        }

        JSON.stringify(tableCfg)
        return tableCfg
    }
}
