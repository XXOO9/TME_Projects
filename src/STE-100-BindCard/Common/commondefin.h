#ifndef COMMONDEFIN_H
#define COMMONDEFIN_H

///、------------ 定义软件测试项目编号 ---------------------
const short TEST_ITEM_UNKNOW            = 0x00;
const short TEST_ITEM_HEIGHT            = 0x01;
const short TEST_ITEM_WEIGHT            = 0x02;
//人体成分分析
const short TEST_ITEM_BCI               = 0x03;
//肺活量
const short TEST_ITEM_VC                = 0x04;
//立定跳远standing broad jump
const short TEST_ITEM_SBJ               = 0x05;
//坐位体前屈 Sitting forward bending
const short TEST_ITEM_SFB               = 0x06;
//引体向上 Pull-ups
const short TEST_ITEM_PULL_UP           = 0x07;
//仰卧起坐 Sit-up
const short TEST_ITEM_SIT_UP            = 0x08;
//BMI
const short TEST_ITEM_BMI               = 0x09;

//1000m
const short TEST_ITEM_RUN_1000          = 0xA0;
//800m
const short TEST_ITEM_RUN_800           = 0xA1;
//50m
const short TEST_ITEM_RUN_50            = 0xA2;
//跳绳
const short TEST_ITEM_ROPE_SKIP         = 0xA3;
//往返跑 8*50
const short TEST_ITEM_RUN_BACK_8_50     = 0xA4;

///、------------ 定义测试设备编号 ---------------------
//如身高体重仪可以测试 TEST_ITEM_HEIGHT TEST_ITEM_WEIGHT TEST_ITEM_BCI TEST_ITEM_BMI这几个项目

const short DEV_TYPE_UNKNOW             = 0x00;
//身高体重人体成分仪器，定义为体型仪
const short DEV_TYPE_BODY               = 0x01;
//肺活量
const short DEV_TYPE_VC                 = 0x02;
//立定跳远仪 standing broad jump
const short DEV_TYPE_SBJ                = 0x03;
//坐位体前屈 Sitting forward bending
const short DEV_TYPE_SFB                = 0x04;
//引体向上 Pull-ups
const short DEV_TYPE_PULL_UP            = 0x05;
//仰卧起坐 Sit-ups
const short DEV_TYPE_SIT_UP             = 0x06;
//跑步
const short DEV_TYPE_RUN_SHORT          = 0x07;
const short DEV_TYPE_RUN_MID            = 0x08;
//跳绳
const short DEV_TYPE_ROPE_SKIP          = 0x09;
//握力
const short DEV_TYPE_ROPE_GRIP          = 0x0A;
//俯卧撑
const short DEV_TYPR_PUSH_UP            = 0x0B;

///、------------ 定义体测设备数据类型码 ----------------------
//空指令
const unsigned short DATA_FRAME_TYPE_NULL = 0x0000;
//复位
const unsigned short DATA_FRAME_TYPE_RESTORY = 0x0001;
//获取信息
const unsigned short DATA_FRAME_TYPE_GET_INFO = 0x0002;
//开始工作 开始什么设备的测量
const unsigned short DATA_FRAME_TYPE_SWORK = 0x0007;
//停止工作
const unsigned short DATA_FRAME_TYPE_EWORK = 0x0008;
//开始计时
const unsigned short DATA_FRAME_TYPE_STIME = 0x0009;
//停止计时
const unsigned short DATA_FRAME_TYPE_ETIME = 0x000A;
//获取数据
const unsigned short DATA_FRAME_TYPE_GET_DATA = 0x000C;
//主动上报数据 注：由体测设备主动发起。
const unsigned short DATA_FRAME_TYPE_REPORT_DATA = 0x000D;
//查询硬件ID
const unsigned short DATA_FRAME_TYPE_GET_ID = 0x001A;

//针对短跑、跳绳，遇到该类型需要转换为 空指令
const unsigned short DATA_FRAME_TYPE_CONVERT_DATA = 0x8000;

const unsigned short DATA_FRAME_TYPE_ERROR_DATA = 0xFFFF;

///、针对部分类型数据有特殊处理
// 短跑数据
const unsigned short DATA_FRAME_TYPE_RUN_SHORT_DATA = 0x3900;
// 短跑心跳
const unsigned short DATA_FRAME_TYPE_RUN_SHORT_HEART = 0x3901;
// 短跑基站数据长度
const unsigned short DATA_FRAME_TYPE_RUN_PACKET_LEN = 0x0118;
// 短跑跑道数据长度
const unsigned short DATA_FRAME_TYPE_RUN_WAY_LENGTH = 0x0100;
// 短跑心跳长度(跳绳心跳长度)
const unsigned short DATA_FRAME_TYPE_RUN_HEART_LENGTH = 0x0032;

///、具体设备类型
//体温
const unsigned short DATA_FRAME_TYPE_TEMPERATURE = 0x1001;
//心率
const unsigned short DATA_FRAME_TYPE_BPM = 0x1002;
//血氧含量
const unsigned short DATA_FRAME_TYPE_SPOC = 0x1003;
//收缩压
const unsigned short DATA_FRAME_TYPE_SBP = 0x1004;
//舒张压
const unsigned short DATA_FRAME_TYPE_DBP = 0x1005;
//呼吸
const unsigned short DATA_FRAME_TYPE_RESPIRATION = 0x1006;
//PH
const unsigned short DATA_FRAME_TYPE_PH = 0x1007;

//身高
const unsigned short DATA_FRAME_TYPE_HEIGHT = 0x1100;
//体重
const unsigned short DATA_FRAME_TYPE_WEIGHT = 0x1101;
//肺活量
const unsigned short DATA_FRAME_TYPE_VC = 0x1102;
//握力
const unsigned short DATA_FRAME_TYPE_GRIP = 0x1103;
//坐位体前屈
const unsigned short DATA_FRAME_TYPE_SFB = 0x1104;
//跳绳
//const unsigned short DATA_FRAME_TYPE_SKIPPING_ROPE = 0x1105;
const unsigned short BASE_STATION_FRAME_TYPE_SKIPPING_ROPE = 0x3801;
const unsigned short DATA_FRAME_TYPE_SKIPPING_ROPE = 0x3800;

//引体向上
const unsigned short DATA_FRAME_TYPE_PULL_UP = 0x1106;
//一分钟仰卧起坐
const unsigned short DATA_FRAME_TYPE_SIT_UP_1 = 0x1107;
//台阶试验
const unsigned short DATA_FRAME_TYPE_STEP_TEST = 0x1108;
//立定跳远
const unsigned short DATA_FRAME_TYPE_SBJ = 0x1109;
//短跑
const unsigned short DATA_FRAME_TYPE_RUN_SHORT = 0x110A;
//中长跑
const unsigned short DATA_FRAME_TYPE_RUN_MID = 0x110B;
//读卡器数据指令
const unsigned short DATA_FRAME_TYPE_READCARD = 0x000E;
//人体阻抗
const unsigned short DATA_FRAME_TYPE_BODY_IMPEDANCE = 0x110C;
//跳绳基站上传的数据包长度
const unsigned short DATA_PACKAGE_ROPE_SKIPPING_LENGTH = 0x0158;
//跳绳数据帧长度16 * 20
const unsigned short DATA_FRAME_ROPE_SKIPPING_LENGTH = 320;
//控制身高体重仪器灯光
const unsigned short DATA_FRAME_BODY_LED_BAND_EFFECT = 0x2800;


///定义测量状态(设备返回的测量数据状态)
const unsigned char DEV_TEST_STATE_INVALID = 0x00; //无效
const unsigned char DEV_TEST_STATE_TESTING = 0x01; //正在测量
//EOT(End Of Test)
const unsigned char DEV_TEST_STATE_EOT_VALID = 0x02; //测试结束 有效
const unsigned char DEV_TEST_STATE_EOT_INVALID = 0x03; //测试结束，无效

///定义测量犯规类型
const unsigned char FOUL_TYPE_ = 0x00;
#endif // COMMONDEFIN_H
