#ifndef CSTECOMMONDEFIN_H
#define CSTECOMMONDEFIN_H

namespace STE_DEV {

}

//测试单元属性
namespace SubElementPropertye {
//描述测试单元个数
enum ELEMENT_NUM{
        ELEMENT_NUM_Body      = 1,
        ELEMENT_NUM_Eyesight  = 1,
        ELEMENT_NUM_Grip      = 1,
        ELEMENT_NUM_Pullup    = 1,
        ELEMENT_NUM_Pushup    = 1,
        ELEMENT_NUM_Run_Mid   = 50,
        ELEMENT_NUM_Run_Short = 8,
        ELEMENT_NUM_SBJ       = 1,
        ELEMENT_NUM_SFB       = 1,
        ELEMENT_NUM_Situp     = 1,
        ELEMENT_NUM_Skip      = 30,
        ELEMENT_NUM_VC        = 1,
        ELEMENT_NUM_PVV       = 1,
        ELEMENT_NUM_Basketball= 1,
        ELEMENT_NUM_Football  = 1,
        ELEMENT_NUM_Volleyball= 1,
        ELEMENT_NUM_Solidball = 1,
        ELEMENT_NUM_PUO       = 1,
        ELEMENT_NUM_VJump     = 1,

        ELEMENT_NUM_CUR_DEV   = 10000, //代表当前设备中的所有测试单元
        ELEMENT_NUM_CUST      = 10001  //代表所有类型设备
    };
}

//硬件属性
namespace HardwarePropertye
{
    ///硬件测试状态
    #define DEV_TEST_STATE_INVALID  0x00 //无效
    #define DEV_TEST_STATE_TESTING  0x01 //正在测量
    #define DEV_TEST_STATE_EOT_VALID  0x02 //测试结束 有效
    #define DEV_TEST_STATE_EOT_INVALID  0x03 //测试结束，无效

    #define DEV_TEST_STATE_TIME_OUT_NOT_TEST 0xFF // 软件层面的开始测试之后超时未结束

    //对应底层硬件对应的类型编码
    enum HARDWARE_CODE{
        HARDWARE_CODE_NETWORK    = 0,  //代表组网
        HARDWARE_CODE_SBJ        = 1,  //立定跳远
        HARDWARE_CODE_SFB        = 2,  //坐位体前屈
        HARDWARE_CODE_VC         = 3,  //肺活量
        HARDWARE_CODE_BODY       = 4,  //身高体重人体成分
        HARDWARE_CODE_SIT_UP     = 5,  //仰卧起坐
        HARDWARE_CODE_PULL_UP    = 6,  //引体向上
        HARDWARE_CODE_PULL_UP_C  = 70, //引体向上
        HARDWARE_CODE_RUN_SHORT  = 7,  //短跑
        HARDWARE_CODE_RUN_MID    = 8,  //中长跑
        HARDWARE_CODE_ROPE_SKING = 9,  //跳绳
        HARDWARE_CODE_EYESIGHT   = 10, //视力
        HARDWARE_CODE_GRIP       = 11, //握力
        HARDWARE_CODE_PUSHUP     = 12, //俯卧撑
        HARDWARE_CODE_PVV        = 13, //肺通气量  pulmonary ventilation volume
        HARDWARE_CODE_FOOTBALL   = 14, //足球
        HARDWARE_CODE_BASKETBALL = 15, //篮球
        HARDWARE_CODE_VOLLEYBALL = 16, //排球
        HARDWARE_CODE_SOLIDBALL  = 17, //实心球
        HARDWARE_CODE_PUO        = 18, //斜身引体向上
        HARDWARE_CODE_VJUMP      = 19, //纵跳摸高
        HARDWARE_CODE_SIZE
    };


}

namespace DataFrameInfo {

/*
 *  PACKET_LEN 只包含数据帧长度，即下发到具体设备中的一次数据
 *              例：短跑只代表一条跑道的完整数据总字节数
 *              只针对数据帧使用,如身高体重中的数据针对某一个类型的长度
 */
    enum PACKET_LEN{
        Packet_Commond_Head  = 1 + 1 + 4 + 2 + 2,       // 通用包头：设备类型(1) + 数据点数(1) + 设备id(4) + 数据类型(2) + 状态(2)
        Packet_Len_Height    = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_Weight    = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_Impedance       = Packet_Commond_Head + 30*4,  // 通用包头 + 30个float
        Packet_Len_Sub_Impedance   = Packet_Commond_Head + 6*4,   // 人体阻抗分包为原始包分为5个，一包含6个float
        Packet_Len_VC              = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_SBJ             = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_SFB             = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_PULL_UP         = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_SIT_UP          = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_GRIP            = Packet_Commond_Head + 4,     // 通用包头 + 数据(4)
        Packet_Len_EYEIGHT         = Packet_Commond_Head + 8,     // 通用包头 + 数据(2 * 4) 分为左眼右眼数据长度一致
        Packet_Len_PUSHUP          = Packet_Commond_Head + 4,     // 通用包头 + 数据(4),
        Packet_Len_Run_Mid         = 28,
        Packet_Len_Run_Short       = 28,
        Packet_Len_Jump_Rope       = 16,
        Packet_Len_PVV             = Packet_Commond_Head + 10*4,  // 通用包头 + 数据(4)
        Packet_Len_Basketball      = Packet_Commond_Head + 4,  // 通用包头 + 数据(4)
        Packet_Len_Footbal         = Packet_Commond_Head + 4,  // 通用包头 + 数据(4)
        Packet_Len_Solidball       = Packet_Commond_Head + 4,  // 通用包头 + 数据(4)
        Packet_Len_PUO             = Packet_Commond_Head + 4,  // 通用包头 + 数据(4)
        Packet_Len_Vertival_Jump   = Packet_Commond_Head + 4,  // 通用包头 + 数据(4)
        Packet_Len_Volleyball      = Packet_Commond_Head + 4   // 通用包头 + 数据(4)
    };

//起始标志 帧头 frameHead
const static unsigned short INDEX_FH = 0;
const static unsigned short CONTENT_FH = 0xFFFF;
//版本号
const static unsigned short INDEX_VERSION = 2;
const static unsigned short CONTENT_VERSION = 0x0400;
//帧号
const static unsigned short INDEX_FRAME_NO = 4;
const static unsigned int   CONTENT_FRAME_NO = 0x00000000;
//机器号
const static unsigned short INDEX_MACHINE_NO = 8;
const static unsigned short CONTENT_MACHINE_NO = 0x0000;
//命令码
const static unsigned short INDEX_CMD = 10;
const static unsigned short CONTENT_CMD = 0x0101;
//帧长度
const static unsigned short INDEX_FRAME_LENTH = 12;
const static unsigned short CONTENT_FRAME_LENTH = 0x0000;
//保留位 reserved
const static unsigned short INDEX_RESERVED = 14;
const static unsigned int   CONTENT_RESERVED = 0x00000000;
//校验类型
const static unsigned short INDEX_CHECK_TYPE = 18;
const static unsigned short CONTENT_CHECK_TYPE = 0x0000;
//校验码
const static unsigned short INDEX_CHECK_CODE = 20;
const static unsigned int   CONTENT_CHECK_CODE = 0x00000000;
//数据包起始位置
const static unsigned short INDEX_COMMON_DATA_HEAD_LEN = 24;

///数据包结构，每个数据包都可以计算长度，同时数据包的前面一部分内容是一致的

//硬件编码,通过硬件编码可以获取设备类型
const static unsigned short PACK_HARDWARE_CODE = 0;
const static unsigned char  CONTENT_HARDWARE_CODE = 0x00;
//数据点数
const static unsigned short PACK_DATA_POINT_NUM = 1;
const static unsigned char  CONTENT_DATA_POINT_NUM = 0x01;
//设备ID
const static unsigned short PACK_HARDWARE_ID = 2;
const static unsigned int   CONTENT_HARDWARE_ID = 0x01020304;
//数据类型/命令码
const static unsigned short PACK_CMD_TYPE = 6;
const static unsigned short CONTENT_CMD_TYPE = 0x0000;
//数据标记(同时也是数据包最短长度)
const static unsigned short PACK_DATA_MARK = 8;
const static unsigned short CONTENT_DATA_MARK = 0x0000;


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
//查询分机编号
const unsigned short DATA_FRAME_TYPE_GET_EXTENSION_NO = 0x280B;
//设置分机编号
const unsigned short DATA_FRAME_TYPE_SET_EXTENSION_NO = 0x280C;
//信道设置
const unsigned short DATA_FRAME_TYPE_SET_CHANNEL = 0x280D;
//获取信道信息
const unsigned short DATA_FRAME_TYPE_GET_CHANNEL_INFO = 0x280E;
//校准
const unsigned short DATA_FRAME_TYPE_CALIBRATION = 0x2803;

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
//视力
const unsigned short DATA_FRAME_TYPE_EYESIGHT = 0x1112;
//俯卧撑
const unsigned short DATA_FRAME_TYPE_PUSHUP = 0x1114;
//肺通气量
const unsigned short DATA_FRAME_TYPE_PVV = 0x1115;
//足球
const unsigned short DATA_FRAME_TYPE_FOOTBALL = 0x1116;
//篮球
const unsigned short DATA_FRAME_TYPE_BASKETBALL = 0x1117;
//排球
const unsigned short DATA_FRAME_TYPE_VOLLEYBALL = 0x1118;
//斜身引体向上
const unsigned short DATA_FRAME_TYPE_PUO = 0x1119;
//实心球
const unsigned short DATA_FRAME_SOLIDBALL = 0x111A;
//纵跳摸高
const unsigned short DATA_FRAME_VJUMP = 0x111B;
//组网心跳命令
const unsigned short DATA_FRAME_NETWORK_HEARTBEAT = 0x2801;

//读卡器数据指令
const unsigned short DATA_FRAME_TYPE_READCARD = 0x2802;
//人体阻抗 原始为 30个float
const unsigned short DATA_FRAME_TYPE_BODY_IMPEDANCE = 0x110C;
//人体阻抗分包1  6个float
const unsigned short DATA_FRAME_TYPE_BODY_IMPEDANCE1 = 0x110D;
//人体阻抗分包2
const unsigned short DATA_FRAME_TYPE_BODY_IMPEDANCE2 = 0x110E;
//人体阻抗分包3
const unsigned short DATA_FRAME_TYPE_BODY_IMPEDANCE3 = 0x110F;
//人体阻抗分包4
const unsigned short DATA_FRAME_TYPE_BODY_IMPEDANCE4 = 0x1110;
//人体阻抗分包5
const unsigned short DATA_FRAME_TYPE_BODY_IMPEDANCE5 = 0x1111;

//跳绳基站上传的数据包长度
const unsigned short DATA_PACKAGE_ROPE_SKIPPING_LENGTH = 0x0158;
//跳绳数据帧长度16 * 20
const unsigned short DATA_FRAME_ROPE_SKIPPING_LENGTH = 320;
//控制身高体重仪器灯光
const unsigned short DATA_FRAME_BODY_LED_BAND_EFFECT = 0x2800;
//体测参数命令
const unsigned short DATA_FRAME_DEV_PARAM_SET = 0x280F;
const unsigned short DATA_FRAME_DEV_PARAM_GET = 0x2810;

const unsigned short DATA_FRAME_TYPE_GET_TIME_STAMP = 0x2811;

//设置数据上报
const unsigned short DATA_FRAME_TYPE_SET_DATA_REPORT = 0x2812;

//进入433m频段配置
const unsigned short DATA_FRAME_TYPE_433_ENTER_CFG = 0x2813;
//退出433m频段配置
const unsigned short DATA_FRAME_TYPE_433_EXIST_CFG = 0x2814;
//清空433m频段配置
const unsigned short DATA_FRAME_TYPE_433_CLEAR_CFG = 0x2815;
//设置433m频段 信道
const unsigned short DATA_FRAME_TYPE_433_SET_CHANNEL = 0x2816;

//设置倒计时参数 俯卧和产能 引体向上 斜身引体等
const unsigned short DATA_FRAME_TYPE_SET_TIME_OUT = 0x2817;
const unsigned short DATA_FRAME_TYPE_GET_TIME_OUT = 0x2818;

//设置犯规个数参数
const unsigned short DATA_FRAME_TYPE_SET_ILLEGAL_TIMES = 0x2819;
}

//描述测试单元所处位置,直接使用指针不太安全同时位置描述不够准确
class CLocation
{
public:
    CLocation(){}
    CLocation(const CLocation &loction):m_shElementIndex(loction.m_shElementIndex),m_iSteDevMacID(loction.m_iSteDevMacID){}
    CLocation(unsigned int iSteDevMacID, unsigned short shElementIndex):m_shElementIndex(shElementIndex),m_iSteDevMacID(iSteDevMacID){}

    //测试单元索引
    unsigned short m_shElementIndex;
    //测试单元对应的设备ID
    unsigned int m_iSteDevMacID;

    bool operator==(const CLocation &pt) const
    {
        if(m_iSteDevMacID == pt.m_iSteDevMacID && m_shElementIndex == pt.m_shElementIndex)
        {
            return true;
        }

        return false;
    }

    bool operator>(const CLocation &pt) const
    {
        bool bRes = false;
        if(m_iSteDevMacID > pt.m_iSteDevMacID)
        {
            bRes = true;
        }

        if(m_iSteDevMacID == pt.m_iSteDevMacID)
        {
            if(m_shElementIndex > pt.m_shElementIndex)
            {
                bRes = true;
            }
        }

        return bRes;
    }

    bool operator>=(const CLocation &pt) const
    {
        return (*this > pt || *this == pt);
    }

    bool operator<(const CLocation &pt) const
    {
        bool bRes = false;
        if(m_iSteDevMacID < pt.m_iSteDevMacID)
        {
            bRes = true;
        }

        if(m_iSteDevMacID == pt.m_iSteDevMacID)
        {
            if(this->m_shElementIndex < pt.m_shElementIndex)
            {
                bRes = true;
            }
        }
        return bRes;
    }

    CLocation& operator=(const CLocation&pt)
    {
        m_iSteDevMacID = pt.m_iSteDevMacID;
        m_shElementIndex = pt.m_shElementIndex;
        return *this;
    }
};

#endif // CSTECOMMONDEFIN_H
