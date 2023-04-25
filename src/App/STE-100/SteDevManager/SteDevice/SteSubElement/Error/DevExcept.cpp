#include "DevExcept.h"
#include "commondefin.h"
#include "CLogger.h"
#include <QString>
#include <QDebug>

//错误等级划分
const uint32_t DEVICE_NO_ERR = 0;
const uint32_t DEVICE_ERR_NORMAL = 1;
const uint32_t DEVICE_ERR_VIOLATION = 2;
const uint32_t DEVICE_ERR_FOUL = 3;


//异物遮挡 FOREIGN BODY SHIELDING 缩写为 FBS

const uint32_t DEVICE_EXPCEPT_UNKNOWN = 1;  // 设备未知异常

// 肺活量设备异常
//
//
//


// 立定跳远设备异常
//
//
//
//踩线
const  uint32_t SBJ_STEP_ON_THE_LINE  = 2;
//异物遮挡
const  uint32_t SBJ_FBS = 3;
//长时间检测异物遮挡
const  uint32_t SBJ_FBS_LONG_TIME = 4;
//踩线(测试失败，提示切换起跳线重测)
const  uint32_t SBJ_STEP_ON_THE_LINE_SWITCH = 5;

// 身高体重体成分异常
//
//
// 成分异常
//
// 采样超时
const  uint32_t BODY_SAMPLE_TIMEOUT = 2;
// 增益最大值电压欠量程
const  uint32_t BODY_GAIN_MAXIMUM_VOLTAGE_UNDER_RANGE_ERROR  = 3;
// 增益最小值电压超量程
const  uint32_t BODY_GAIN_MINIMUM_VOLTAGE_OUT_OF_RANGE_ERROR = 4;
// 测量松动
const  uint32_t BODY_MEASURING_LOOSENESS = 5;
// 测量短路
const  uint32_t BODY_MEASURE_SHORT_CIRCUIT = 6;
// 左手电极未联通
const  uint32_t BODY_LEFT_HAND_NOT_CONNECTED  = 7;
// 右手电极未联通
const  uint32_t BODY_RIGHT_HAND_NOT_CONNECTED = 8;
// 左脚电极未联通
const  uint32_t BODY_LEFT_FOOT_NOT_CONNECTED  = 9;
// 右脚电极未联通
const  uint32_t BODY_RIGHT_FOOT_NOT_CONNECTED = 10;

//
// 身高测量异常
// 无人测试
const  uint32_t BODY_HEIGHT_UNMANED = 11;
//
// 体重测量异常
// 显示屏熄灭
const  uint32_t BODY_WEIGHT_DISPLAY_SLAKE = 12;
// 测量异常结束
const  uint32_t BODY_WEIGHT_EXCEPT_END = 13;


//  仰卧起坐设备异常
//
//
//  异物遮挡
const  uint32_t SITUP_FBS = 2;


// 引体向上设备异常
//
//
// 异物遮挡
const  uint32_t PULLUP_FBS = 2;
const  uint32_t PULLUP_INVALID_HAND = 3;


// 中长跑设备异常
//
//
//组网异常
const  uint32_t RUNMID_NETWORK_ABNORMALITY = 2;


// 坐位体前屈设备异常
//
//
// 屈腿
const  uint32_t SFB_BENT_LEG      = 2;
const  uint32_t SFB_NO_TWO_HANDS_PUSH = 3;
const  uint32_t SFB_MOTOR_FAILURE = 4;

//足球异常 foot ball
const  uint32_t FB_OMIT_ROD_1      = 2;  //遗漏第1杆
const  uint32_t FB_OMIT_ROD_2      = 3;  //遗漏第2杆
const  uint32_t FB_OMIT_ROD_3      = 4;  //遗漏第3杆
const  uint32_t FB_OMIT_ROD_4      = 5;  //遗漏第4杆
const  uint32_t FB_OMIT_SP         = 6;  //遗漏起点
const  uint32_t FB_FBS_SP          = 7;  //起点异物遮挡
const  uint32_t FB_FBS_ROD_1       = 8;  //绕杆1异物遮挡
const  uint32_t FB_FBS_ROD_2       = 9;  //绕杆2异物遮挡
const  uint32_t FB_FBS_ROD_3       = 10; //绕杆3异物遮挡
const  uint32_t FB_FBS_ROD_4       = 11; //绕杆4异物遮挡
const  uint32_t FB_FBS_ROD_5       = 12; //绕杆5异物遮挡
const  uint32_t FB_FBS_EP          = 13; //终点异物遮挡
const  uint32_t FB_ROD_FALL        = 14; //倒杆

//篮球 basket ball
const  uint32_t BB_OMIT_SP         = 2; //遗漏起点
const  uint32_t BB_OMIT_RP         = 3; //遗漏折返区
const  uint32_t BB_OMIT_GP         = 4; //未检测到进球
const  uint32_t BB_FBS_SP          = 5; //起点异物遮挡
const  uint32_t BB_FBS_RP          = 6; //折返区异物遮挡
const  uint32_t BB_FBS_GP          = 7; //进球区异物遮挡

//排球 volley ball
const  uint32_t VB_FBS_HP          = 2; //击球区异物遮挡 hitting point
const  uint32_t VB_FBS_RA          = 3; //禁入区异物遮挡 restricted areas

//斜身引体向上
const  uint32_t PUO_FBS            = 2; //异物遮挡
const  uint32_t PUO_ABNORMAL_PEDAL = 3; //踏板异常
const  uint32_t PUO_INVALID_HEAD   = 4; //头部未过杆
const  uint32_t PUO_INVALID_WAIST  = 5; //塌腰或挺腹
const  uint32_t PUO_INVALID_HAND   = 6; //手臂动作不标准

//实心球 SOLIDBALL
const  uint32_t SB_FBS_MA          = 2; //测量区域异物遮挡 measurement area
const  uint32_t SB_FBS_TA          = 3; //投掷区异物遮挡 throwing area
const  uint32_t SB_FOSOL           = 4; //踩线犯规 Foul of stepping on line
const  uint32_t SB_THROW_OUTSIDE   = 5; //投掷区外投球 Throw from outside the throwing zone
const  uint32_t SB_RUN_UP_FOUL     = 6; //助跑犯规 throwing area

//纵跳摸高
const  uint32_t VJUMP_FBS          = 2; //异物遮挡

// 后面可以通过配置文件加载异常
IDevExcept* createExpectObj(uint32_t iType)
{
    IDevExcept* pDevExcept =  nullptr;
    switch (iType)
    {
        // 肺活量设备异常
    case DEV_TYPE_VC:
        pDevExcept = new CDevExcept(

        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN, DEVICE_ERR_TYPE_UNKNOWN}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,    "设备异常,请检查设备"}
        });
        break;

        // 立定跳远设备异常
    case DEV_TYPE_SBJ:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000002u,                SBJ_STEP_ON_THE_LINE},
            {0x00000004u,                SBJ_FBS_LONG_TIME},
            {0x00000008u,                SBJ_FBS},
            {0x00000010u,                SBJ_STEP_ON_THE_LINE_SWITCH},
            {0x00000001u,                DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,     DEVICE_ERR_TYPE_UNKNOWN},
            {SBJ_STEP_ON_THE_LINE,       DEVICE_ERR_TYPE_FOUL},
            {SBJ_FBS_LONG_TIME, DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {SBJ_STEP_ON_THE_LINE_SWITCH, DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {SBJ_FBS, DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,			  "设备异常,请检查设备"},
            {SBJ_STEP_ON_THE_LINE,				  "踩线犯规"},
            {SBJ_FBS_LONG_TIME,"异物遮挡"},
            {SBJ_FBS,		  "异物遮挡"}
        });
        break;

        // 身高体重体成分异常
    case DEV_TYPE_BODY:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            // 成分
            {0x00000802u, BODY_LEFT_HAND_NOT_CONNECTED},
            {0x00000202u, BODY_RIGHT_HAND_NOT_CONNECTED},
            {0x00000102u, BODY_LEFT_FOOT_NOT_CONNECTED},
            {0x00000402u, BODY_RIGHT_FOOT_NOT_CONNECTED},
            {0x00003002u, BODY_GAIN_MINIMUM_VOLTAGE_OUT_OF_RANGE_ERROR},
            {0x00005002u, BODY_MEASURE_SHORT_CIRCUIT},
            {0x00001002u, BODY_SAMPLE_TIMEOUT},
            {0x00002002u, BODY_GAIN_MAXIMUM_VOLTAGE_UNDER_RANGE_ERROR},
            {0x00004002u, BODY_MEASURING_LOOSENESS},
            // 身高 无人测试(2023.1.6屏蔽)
            //{0x00000002u, BODY_HEIGHT_UNMANED},
            // 体重
            //{0x00000004u, BODY_WEIGHT_DISPLAY_SLAKE},
            {0x00000008u, BODY_WEIGHT_EXCEPT_END},
            // 通用
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,         DEVICE_ERR_TYPE_UNKNOWN},
            {BODY_SAMPLE_TIMEOUT,            DEVICE_ERR_TYPE_MOUDLE},
            {BODY_GAIN_MAXIMUM_VOLTAGE_UNDER_RANGE_ERROR,  DEVICE_ERR_TYPE_MOUDLE},
            {BODY_GAIN_MINIMUM_VOLTAGE_OUT_OF_RANGE_ERROR, DEVICE_ERR_TYPE_MOUDLE},
            {BODY_MEASURING_LOOSENESS,       DEVICE_ERR_TYPE_MOUDLE},
            {BODY_MEASURE_SHORT_CIRCUIT,     DEVICE_ERR_TYPE_MOUDLE},
            {BODY_LEFT_HAND_NOT_CONNECTED,   DEVICE_ERR_TYPE_TESTER},
            {BODY_RIGHT_HAND_NOT_CONNECTED,  DEVICE_ERR_TYPE_TESTER},
            {BODY_LEFT_FOOT_NOT_CONNECTED,   DEVICE_ERR_TYPE_TESTER},
            {BODY_RIGHT_FOOT_NOT_CONNECTED,  DEVICE_ERR_TYPE_TESTER},
            // 身高 无人测试(2023.1.6屏蔽)
            //{BODY_HEIGHT_UNMANED,            DEVICE_ERR_TYPE_TEST_RESULT},
            //{BODY_WEIGHT_DISPLAY_SLAKE,      DEVICE_ERR_TYPE_TESTER_PREPARE},
            {BODY_WEIGHT_EXCEPT_END,         DEVICE_ERR_TYPE_TEST_RESULT}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN, "设备异常,请检查设备"},
            {BODY_SAMPLE_TIMEOUT,    "采样超时"},
            {BODY_GAIN_MAXIMUM_VOLTAGE_UNDER_RANGE_ERROR,  "增益最大值电压欠量程"},
            {BODY_GAIN_MINIMUM_VOLTAGE_OUT_OF_RANGE_ERROR, "增益最小值电压超量程"},
            {BODY_MEASURING_LOOSENESS,       "测量松动"},
            {BODY_MEASURE_SHORT_CIRCUIT,     "测量短路"},
            {BODY_LEFT_HAND_NOT_CONNECTED,   "左手电极未联通"},
            {BODY_RIGHT_HAND_NOT_CONNECTED,  "右手电极未联通"},
            {BODY_LEFT_FOOT_NOT_CONNECTED,   "左脚电极未联通"},
            {BODY_RIGHT_FOOT_NOT_CONNECTED,  "右脚电极未联通"},
            //{BODY_HEIGHT_UNMANED,            "无人测试"},
            //{BODY_WEIGHT_DISPLAY_SLAKE,      "体重秤不可用,请等待"},
            {BODY_WEIGHT_EXCEPT_END,         "测量异常结束"}
        });
        break;

        // 仰卧起坐设备异常
    case DEV_TYPE_SIT_UP:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000002u, SITUP_FBS},
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       DEVICE_ERR_TYPE_UNKNOWN},
            {SITUP_FBS, DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"},
            {SITUP_FBS, "异物遮挡"}
        });
        break;

        // 引体向上设备异常
    case DEV_TYPE_PULL_UP:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN},
            {0x00000002u, PULLUP_FBS},
            {0x00000004u, PULLUP_INVALID_HAND}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,        DEVICE_ERR_TYPE_UNKNOWN},
            {PULLUP_FBS,                    DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {PULLUP_INVALID_HAND,           DEVICE_ERR_TYPE_FOUL}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"},
            {PULLUP_FBS,                   "异物遮挡"},
            {PULLUP_INVALID_HAND,          "手臂动作不标准"}
        });
        break;

        // 中长跑设备异常
    case DEV_TYPE_RUN_MID:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000002u, RUNMID_NETWORK_ABNORMALITY},
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,        DEVICE_ERR_TYPE_UNKNOWN},
            {RUNMID_NETWORK_ABNORMALITY,    DEVICE_ERR_TYPE_MOUDLE}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"},
            {RUNMID_NETWORK_ABNORMALITY,   "组网异常"}
        });
        break;

        // 坐位体前屈
    case DEV_TYPE_SFB:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000008u, SFB_MOTOR_FAILURE},
            {0x00000004u, SFB_NO_TWO_HANDS_PUSH},
            {0x00000002u, SFB_BENT_LEG},
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,        DEVICE_ERR_TYPE_UNKNOWN},
            {SFB_BENT_LEG,					DEVICE_ERR_TYPE_FOUL},
            {SFB_NO_TWO_HANDS_PUSH,	        DEVICE_ERR_TYPE_FOUL},
            {SFB_MOTOR_FAILURE,			    DEVICE_ERR_TYPE_MOUDLE},
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"},
            {SFB_BENT_LEG,                 "屈腿犯规"},
            {SFB_NO_TWO_HANDS_PUSH,        "非双手推动"},
            {SFB_MOTOR_FAILURE,            "电机故障"},
        });
        break;

        // 短跑
    case DEV_TYPE_RUN_SHORT:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,        DEVICE_ERR_TYPE_UNKNOWN}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"}
        });
        break;

        // 跳绳
    case DEV_TYPE_ROPE_SKIP:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,        DEVICE_ERR_TYPE_UNKNOWN}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"}
        });
        break;
    case DEV_TYPE_PUSH_UP:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,        DEVICE_ERR_TYPE_UNKNOWN}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"}
        });
        break;
    case DEV_TYPE_FOOTBALL:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}, //                 1
            {0x00000002u, FB_OMIT_ROD_1},          //                10
            {0x00000004u, FB_OMIT_ROD_2},          //               100
            {0x00000008u, FB_OMIT_ROD_3},          //              1000
            {0x00000010u, FB_OMIT_ROD_4},          //             10000
            {0x00000020u, FB_OMIT_SP},             //            100000
            {0x00000100u, FB_FBS_SP},              //        1 00000000
            {0x00000200u, FB_FBS_ROD_1},           //       10 00000000
            {0x00000400u, FB_FBS_ROD_2},           //      100 00000000
            {0x00000800u, FB_FBS_ROD_3},           //     1000 00000000
            {0x00001000u, FB_FBS_ROD_4},           //    10000 00000000
            {0x00002000u, FB_FBS_ROD_5},           //   100000 00000000
            {0x00004000u, FB_FBS_EP},              //  1000000 00000000
            {0x00008000u, FB_ROD_FALL},            // 10000000 00000000
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  DEVICE_ERR_TYPE_UNKNOWN},
            {FB_OMIT_ROD_1,           DEVICE_ERR_TYPE_FOUL},
            {FB_OMIT_ROD_2,           DEVICE_ERR_TYPE_FOUL},
            {FB_OMIT_ROD_3,           DEVICE_ERR_TYPE_FOUL},
            {FB_OMIT_ROD_4,           DEVICE_ERR_TYPE_FOUL},
            {FB_OMIT_SP,              DEVICE_ERR_TYPE_FOUL},
            {FB_FBS_SP,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {FB_FBS_ROD_1,            DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {FB_FBS_ROD_2,            DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {FB_FBS_ROD_3,            DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {FB_FBS_ROD_4,            DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {FB_FBS_ROD_5,            DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {FB_FBS_EP,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {FB_ROD_FALL,             DEVICE_ERR_TYPE_STOP},
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  "设备异常,请检查设备"},
            {FB_OMIT_ROD_1,           "遗漏第1杆"},
            {FB_OMIT_ROD_2,           "遗漏第2杆"},
            {FB_OMIT_ROD_3,           "遗漏第3杆"},
            {FB_OMIT_ROD_4,           "遗漏第4杆"},
            {FB_OMIT_SP,              "遗漏起点"},
            {FB_FBS_SP,               "起点异物遮挡"},
            {FB_FBS_ROD_1,            "1号绕杆异物遮挡"},
            {FB_FBS_ROD_2,            "2号绕杆异物遮挡"},
            {FB_FBS_ROD_3,            "3号绕杆异物遮挡"},
            {FB_FBS_ROD_4,            "4号绕杆异物遮挡"},
            {FB_FBS_ROD_5,            "5号绕杆异物遮挡"},
            {FB_FBS_EP,               "终点异物遮挡"},
            {FB_ROD_FALL,             "倒杆"},
        });
        break;
    case DEV_TYPE_BASKETBALL:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}, //             1
            {0x00000002u, BB_OMIT_SP},             //            10
            {0x00000004u, BB_OMIT_RP},             //           100
            {0x00000008u, BB_OMIT_GP},             //          1000
            {0x00000200u, BB_FBS_SP},              //   10 00000000
            {0x00000400u, BB_FBS_RP},              //  100 00000000
            {0x00000800u, BB_FBS_GP},              // 1000 00000000
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  DEVICE_ERR_TYPE_UNKNOWN},
            {BB_OMIT_SP,              DEVICE_ERR_TYPE_FOUL},
            {BB_OMIT_RP,              DEVICE_ERR_TYPE_FOUL},
            {BB_OMIT_GP,              DEVICE_ERR_TYPE_FOUL},
            {BB_FBS_SP,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {BB_FBS_RP,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {BB_FBS_GP,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  "设备异常,请检查设备"},
            {BB_OMIT_SP,              "遗漏起点"},
            {BB_OMIT_RP,              "遗漏折返区"},
            {BB_OMIT_GP,              "未检测到进球"},
            {BB_FBS_SP,               "起点异物遮挡"},
            {BB_FBS_RP,               "折返区异物遮挡"},
            {BB_FBS_GP,               "进球区异物遮挡"},
        });
        break;
    case DEV_TYPE_VOLLEYBALL:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN},
            {0x00000002u, VB_FBS_HP},
            {0x00000004u, VB_FBS_RA}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  DEVICE_ERR_TYPE_UNKNOWN},
            {VB_FBS_HP,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {VB_FBS_RA,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  "设备异常,请检查设备"},
            {VB_FBS_HP,               "击球区异物遮挡"},
            {VB_FBS_RA,               "禁入区异物遮挡"},
        });
        break;
    case DEV_TYPE_PUO:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN},
            {0x00000002u, PUO_FBS},
            {0x00000004u, PUO_ABNORMAL_PEDAL},
            {0x00000008u, PUO_INVALID_HEAD},
            {0x00000010u, PUO_INVALID_WAIST},
            {0x00000020u, PUO_INVALID_HAND}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  DEVICE_ERR_TYPE_UNKNOWN},
            {PUO_FBS,                 DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {PUO_ABNORMAL_PEDAL,      DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {PUO_INVALID_HEAD,        DEVICE_ERR_TYPE_FOUL},
            {PUO_INVALID_WAIST,       DEVICE_ERR_TYPE_FOUL},
            {PUO_INVALID_HAND,        DEVICE_ERR_TYPE_FOUL}

        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  "设备异常,请检查设备"},
            {PUO_FBS,                 "异物遮挡"},
            {PUO_ABNORMAL_PEDAL,      "踏板异常"},
            {PUO_INVALID_HEAD,        "头部未过杆"},
            {PUO_INVALID_WAIST,       "塌腰或挺腹"},
            {PUO_INVALID_HAND,        "手臂动作不标准"}
        });
        break;
    case DEV_TYPE_SOLIDBALL:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN},   // 00000001
            {0x00000002u, SB_FBS_MA},                // 00000010
            {0x00000004u, SB_FBS_TA},                // 00000100
            {0x00000008u, SB_FOSOL},                 // 00001000
            {0x00000010u, SB_THROW_OUTSIDE},         // 00010000
            {0x00000020u, SB_RUN_UP_FOUL},           // 00100000
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  DEVICE_ERR_TYPE_UNKNOWN},
            {SB_FBS_MA,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {SB_FBS_TA,               DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT},
            {SB_FOSOL,                DEVICE_ERR_TYPE_FOUL},
            {SB_THROW_OUTSIDE,        DEVICE_ERR_TYPE_FOUL},
            {SB_RUN_UP_FOUL,          DEVICE_ERR_TYPE_FOUL},
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,  "设备异常,请检查设备"},
            {SB_FBS_MA,               "测量区域异物遮挡"},
            {SB_FBS_TA,               "投掷区异物遮挡"},
            {SB_FOSOL,                "踩线犯规"},
            {SB_THROW_OUTSIDE,        "投掷区外投球"},
            {SB_RUN_UP_FOUL,          "助跑犯规"},
        });
        break;
    case DEV_TYPE_VJUMP:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN},
            {0x00000002u, VJUMP_FBS}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN, DEVICE_ERR_TYPE_UNKNOWN},
            {VJUMP_FBS,              DEVICE_ERR_TYPE_TESTER_PREPARE | DEVICE_ERR_TYPE_WAIT}
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN, "设备异常,请检查设备"},
            {DEVICE_EXPCEPT_UNKNOWN, "异物遮挡"}
        });
        break;
    //新添加进来的设备若没有设置特定的异常，默认仅有设备异常
    default:
        pDevExcept = new CDevExcept(
        // 硬件异常检测表
        {
            {0x00000001u, DEVICE_EXPCEPT_UNKNOWN}
        },
        // 异常类型表
        {
            {DEVICE_EXPCEPT_UNKNOWN,        DEVICE_ERR_TYPE_UNKNOWN},
        },
        // 硬件异常信息表
        {
            {DEVICE_EXPCEPT_UNKNOWN,       "设备异常,请检查设备"},
        });

        char log[128] = { 0 };
        sprintf_s(log, 128, "Failed to allcate Expcept Object, Err Dev Type: %u", iType);
        LOGINFO(string(log));
        break;
    }

    if (nullptr == pDevExcept)
    {
        char log[128] = { 0 };
        sprintf_s(log, 128, "Failed to allcate Expcept Object %u", iType);
        LOGINFO(string(log));
    }

    return pDevExcept;
}

CDevExcept::CDevExcept(const std::map<uint32_t, uint32_t>& exceptCodeTable,
                       const std::map<uint32_t, uint32_t>& exceptTypeTable,
                       const std::map<uint32_t, string>& exceptInfo):
     m_errCodeTable(exceptCodeTable)
    ,m_errTypeTable(exceptTypeTable)
    ,m_strErrInfo(exceptInfo)
{
}

CDevExcept::~CDevExcept()
{
}

string CDevExcept::getErrDesc(const uint32_t iErrCode) const
{
    if (m_strErrInfo.end() == m_strErrInfo.find(iErrCode))
    {
        char log[256] = { 0 };
        sprintf_s(log, 256, "Failed to getErrDesc： %u", iErrCode);
        LOGINFO(string(log));
        return string("");
    }
    return string(m_strErrInfo.at(iErrCode).c_str(), m_strErrInfo.at(iErrCode).size());
}

uint32_t CDevExcept::checkErr(const uint32_t value)
{ 
    setLastErrCode(getErrCode());
    setLastErrType(getErrType());

    unsigned int uiLevel = DEVICE_NO_ERR;
    uint32_t uiCurrent = DEVICE_NORMAL;

    //查找所有错误
    for (auto it = m_errCodeTable.cbegin(); it != m_errCodeTable.cend(); it++)
    {
        if ((it->first & value) == it->first)
        {
            //进行优先级判断(设备未知异常(3)，设备强制中止(2)， 其他(1))
            if(DEVICE_ERR_TYPE_UNKNOWN == m_errTypeTable.at(it->second))
            {
                if(uiLevel < DEVICE_ERR_FOUL)
                {
                    uiLevel   = DEVICE_ERR_FOUL;
                    uiCurrent = it->second;
                }
            }
            else if(DEVICE_ERR_TYPE_STOP == m_errTypeTable.at(it->second))
            {
                if(uiLevel < DEVICE_ERR_VIOLATION)
                {
                    uiLevel   = DEVICE_ERR_VIOLATION;
                    uiCurrent = it->second;
                }
            }
            else
            {
                if(uiLevel < DEVICE_ERR_NORMAL)
                {
                    uiLevel   = DEVICE_ERR_NORMAL;
                    uiCurrent = it->second;
                }
            }

            LOGINFO("Error Code: " + to_string(it->second) + "  " + getErrDesc(it->second));
        }
    }

    //优先发送高等级错误
    if(uiLevel > DEVICE_NO_ERR)
    {
        setErrCode(uiCurrent);
        setErrType(m_errTypeTable.at(uiCurrent));
        return uiCurrent;
    }

    setErrCode(DEVICE_NORMAL);
    setErrType(TYPE_NORMAL);
    return DEVICE_NORMAL;
}


