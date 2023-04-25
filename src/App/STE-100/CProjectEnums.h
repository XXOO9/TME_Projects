#ifndef CPROJECTENUMS_H
#define CPROJECTENUMS_H

#include "../common/commondefin.h"
#include <QObject>

class CProjectEnums : public QObject
{
    Q_OBJECT
public:
    explicit CProjectEnums(QObject *parent = nullptr);

    enum DevType{
        DEV_UNKNOWN = 0,
        DEV_BODY,
        DEV_VC,
        DEV_SBJ,
        DEV_SFB,
        DEV_PULL_UP,
        DEV_SIT_UP,
        DEV_RUN_SHORT,
        DEV_RUN_MID,
        DEV_ROPE_SKIP,
        DEV_GRIP,
        DEV_PUSH_UP,
        DEV_EYESIGHT,
        DEV_PVV,
    };
    Q_ENUM( DevType )

    enum Test_Item{
        ITEM_UNKNOW          = TEST_ITEM_UNKNOW,
        ITEM_HEIGHT          = TEST_ITEM_HEIGHT,
        ITEM_WEIGHT          = TEST_ITEM_WEIGHT,
        ITEM_BCI             = TEST_ITEM_HBC,
        ITEM_VC              = TEST_ITEM_VC,
        ITEM_SBJ             = TEST_ITEM_SBJ,
        ITEM_SFB             = TEST_ITEM_SFB,
        ITEM_PULL_UP         = TEST_ITEM_PULL_UP,
        ITEM_SIT_UP          = TEST_ITEM_SIT_UP,
        ITEM_BMI             = TEST_ITEM_BMI,
        ITEM_GRIP            = TEST_ITEM_GRIP,
        ITEM_PUSHUP          = TEST_ITEM_PUSHUP,
        ITEM_EYESIGHT        = TEST_ITEM_EYESIGHT,
        ITEM_PVV             = TEST_ITEM_PVV,
        ITEM_FOOTBALL        = TEST_ITEM_FOOTBALL,
        ITEM_BASKETBALL      = TEST_ITEM_BASKETBALL,
        ITEM_VOLLEYBALL      = TEST_ITEM_VOLLEYBALL,
        ITEM_PUO             = TEST_ITEM_PUO,
        ITEM_SOLIDBALL       = TEST_ITEM_SOLIDBALL,
        ITEM_VJUMP           = TEST_ITEM_VJUMP,
        ITEM_RUN_1000        = TEST_ITEM_RUN_1000,
        ITEM_RUN_800         = TEST_ITEM_RUN_800,
        ITEM_RUN_50          = TEST_ITEM_RUN_50,
        ITEM_ROPE_SKIP       = TEST_ITEM_ROPE_SKIP,
        ITEM_RUN_BACK_8_50   = TEST_ITEM_RUN_BACK_8_50,
        ITEM_RUN_60          = TEST_ITEM_RUN_60,
        ITEM_RUN_100         = TEST_ITEM_RUN_100,
        ITEM_RUN_200         = TEST_ITEM_RUN_200,
        ITEM_RUN_400         = TEST_ITEM_RUN_400,
        ITEM_RUN_4_100       = TEST_ITEM_RUN_4_100,
        ITEM_RUN_HURDLES_100 = TEST_ITEM_RUN_HURDLES_100,
        ITEM_RUN_HURDLES_110 = TEST_ITEM_RUN_HURDLES_110,
        ITEM_RUN_1500        = TEST_ITEM_RUN_1500,
        ITEM_RUN_BACK_4_10   = TEST_ITEM_RUN_BACK_4_10,
        ITEM_RUN_2000        = TEST_ITEM_RUN_2000,

    };
    Q_ENUM( Test_Item )

    enum queryComBoBoxIndex{
        Query_UNKNOWN = 0,
        Query_VC,
        Query_SBJ,
        Query_SFB,
        Query_PULLUP,
        Query_SITUP,
        Query_RUN_1000,
        Query_RUN_800,
        Query_RUN_50,
        Query_ROPESKIP,
    };
    Q_ENUM( queryComBoBoxIndex )

    //测试类型
    enum TestNature{
        TEST_UNKONW = 0,
        TEST_INFORMAL,
        TEST_FORMAL,
        TEST_FREE
    };


    Q_ENUM( TestNature )

    enum BTN_TYPE {
        UNKNOWN_BTN = -1,                //
        OTO_START_STU_TEST_BTN = 0,     //开始学生测试
        OTO_START_TUR_TEST_BTN,         //开始游客测试
        OTO_QUERY_SCORCE_BTN,           //成绩查询按钮
        OTO_INFO_BACK_HOME_BTN,         //信息页面返回按钮
        OTO_TEST_BACK_INFO_BTN,         //测试中返回信息按钮
        OTO_TEST_BACK_HOME_BTN,         //测试中返回首页按钮
        OTO_COMMIT_BTN,                 //提交按钮
        OTO_CLEAR_SIGN_BTN,             //清除签字按钮
        OTO_VERIFY_SIGN_BTN,            //确认签字按钮
        OTM_START_TEST_BTN,             //同步开始测试按钮
        OTM_CANCEL_TEST_BTN,            //同步取消测试按钮
        OTM_RESTART_TEST_BTN,           //同步重测按钮
        OTM_SYNC_COMMIT_BTN,            //同步提交按钮
        OTM_CLEAR_SIGN_BTN,             //清除签字按钮
        OTM_VERIFY_SIGN_BTN,            //确认签字按钮
        OTM_ASYNC_COMMIT_BTN,           //异步提交按钮
        OTM_ASYNC_CANCEL_BTN            //异步退出测试按钮
    };
    Q_ENUM( BTN_TYPE )

    //当前界面枚举, 不要改变顺序！！！！要添加新内容只能在最后追加
    enum ViewPage{
        PAGE_UNKNOWN = 0,       //未知界面
        PAGE_SYSTEM_SELFCHECK,  //系统自检页面
        PAGE_SYSTEM_ABNORMAL,   //异常进入系统的界面
        PAGE_DIRECT_HOME,       //直连设备软件首页
        PAGE_GROUP_HOME,        //组连设备软件首页
        PAGE_TOURIST_HOME,      //游客主界面
        PAGE_TOURIST_TEST,      //游客测试界面
        PAGE_USER_INFO,         //用户信息界面
        PAGE_USER_TEST,         //用户测试界面
        PAGE_USER_SCOREQUERY,   //用户查询成绩界面
        PAGE_GROUP_TEST,        //组连设备测试页面
        PAGE_VIDEO,             //视频播放界面
        PAGE_SETTING,           //设置弹窗
        PAGE_POPUP              //所有影响人脸视频的弹窗
    };
    Q_ENUM( ViewPage )

    enum ViewRootDisplay{
        View_ServerOffLine = 0,
        View_Loading,
    };
    Q_ENUM( ViewRootDisplay )

    //肺通气量图表类型
    enum PVVChartViewType{
        VT_Chart = 0,       //流量-时间图
        FV_Chart,           //流速-流量图
        RT_Chart            //呼吸波形图
    };
    Q_ENUM( PVVChartViewType )

    //容积环阶段
    enum FVStageType{
        Expiration = 0,     //呼气阶段
        Inspiration = 1     //吸气阶段
    };
    Q_ENUM( FVStageType )

    //测试项转qml界面
    enum DevTestItem{
        TestHeight        = 1,     //身高
        TestWeight        = 2,      //体重
        TestHuman         = 3,      //人体成分
        TestBMI           = 9,        //BMI
        TestRunShort      = 162, //短跑
        TestRunBack       = 164,  //往返跑
        TestRunShort60    = TEST_ITEM_RUN_60,
        TestRunShort100   = TEST_ITEM_RUN_100,
        TestRunShort200   = TEST_ITEM_RUN_200,
        TestRunShort400   = TEST_ITEM_RUN_400,
        TestRunShort4_100 = TEST_ITEM_RUN_4_100,
        TestRunHurdles100 = TEST_ITEM_RUN_HURDLES_100,
        TestRunHurdles110 = TEST_ITEM_RUN_HURDLES_110,
        TestPVVVC = 353,    //肺通气量肺活量
        TestPVVFVC = 354,   //用力肺活量
        TestPVVMPV = 355    //最大肺通气量
    };
    Q_ENUM( DevTestItem )

    //立定跳远起始区域设置
    enum StandingJumpStartPos{
        Start_40_cm = 0,
        Start_80_cm,
    };
    Q_ENUM( StandingJumpStartPos )

    //自定义高精度定时器枚举
    enum HightCountTimer{
        STARTORRESTARTCOUNTTIMER = 0,
        STOPCOUNTTIMER,
        CALCULATETIMEDIFFERENCE
    };
    Q_ENUM( HightCountTimer )

    //设置界面
    enum SystemSettingPage{
        PAGE_HOST = 0,
        PAGE_DEV,
        PAGE_USING,
        PAGE_DATAMANAGE
    };
    Q_ENUM( SystemSettingPage )

    //测试类型
    enum TestMode{
        TEST_MODE_UNKONW = 0,
        TEST_MODE_NORMAL,
        TEST_MODE_COMPET
    };
    Q_ENUM( TestMode )

    //系统设置配置类型
    enum ConfigItem{
        CONFIG_UNKONW = 0,
        CONFIG_QUERY_ITEM = 5
    };
    Q_ENUM( ConfigItem )


public:

signals:

public slots:
};

#endif // CPROJECTENUMS_H
