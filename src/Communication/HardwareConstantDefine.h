/**************************************************
*!
* file     HardwareConstantDefine.h 是底层硬件参数常量、结构定义的头文件
*          同时也包括和NIM-100扩展板之间串口通信需要的参数和结构体。
* brief    该文件存贮NIM-100 底层硬件和上层中共同使用的常量和结构
*          该文件遵循 《生物信号采集与处理系__软硬件通讯标准》
*          和《术中主机与扩展板通信协议》
*          该文件是底层硬件与上层软件交互的基础，请勿轻易修改
*
*          变量名的缩写规则：
*      	   常数，结构名，类名和函数名中尽量采样全名的方式，少采样缩写
*      	   变量则尽量采用缩写，便于程序阅读和快速构建
*     	   常见的缩写单词：
*     	   Cha: 	Channel
*     	   Len: 	Length
*          Num: 	Number
*          Freq: 	Frequency
*
* refer    Hardware_Constant_Define.h
* author   fmj
* version  0.0.1
* date     2018-08-02
*!
**************************************************/


#ifndef HARDWARECONSTANTDEFINE_H
#define HARDWARECONSTANTDEFINE_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <vector>
#include <string.h>
#include <QString>
#include <QVector>
#include <QObject>


/*************************************************
  <文件 名 >     HardwareConstantDefine.h
  <功能描述>     该文件定义了整个软件系统后台所用到的常量，
                注意不要随意修改。

  <作    者>     cpc
  <时    间>     2021/8/18
**************************************************/

//---------------------------------------------------------------－－－－－－－－－－－－－－－－－－－－－－－－－－－－
//---------------------------------------【一、公共定义】-----------------------－－－－－－－－－－－－－－－－－－－
//---------------------------------------------------------------－－－－－－－－－－－－－－－－－－－－－－－－－－－－

typedef  unsigned char   U8;
typedef  unsigned short  U16;
typedef  unsigned int    U32;
typedef  float           FLOAT;

//硬件模式常量
const   U32      APP_MODE                 = 1;         //APP模式
const   U32      IAP_MODE                 = 0;         //IAP模式

//串口连接状态
const   U32      NO_CONNECT               = 0;         //当前无连接
const   U32      NORMAL_CONNECT           = 1;         //当前为正常连接

//自检结果常量
const   U16      CHA_NORMAL               = 0;         //采用通道正常
const   U16      CHA_COMM_ABNORMAL        = 1;         //通道单片机通信异常
const   U16      CHA_BIG_ABNORMAL         = 2;         //通道放大器异常

const   U16      STIM_NORMAL              = 0;         //刺激器正常
const   U16      STIM_ABNORMAL            = 1;         //刺激器异常

const   U16      DATA_SAMPLE_NORMAL       = 0;         //数据采样功能正常
const   U16      DATA_SAMPLE_ABNORMAL     = 1;         //数据采样功能异常

const   U16      POWER_NORMAL             = 0;         //电源正常
const   U16      POWER_LOW                = 1;         //电源电压偏低
const   U16      POWER_HIGH               = 2;         //电源电压偏高

const   U16      CPU_CARD_NORMAL          = 0;         //CUP读卡器正常
const   U16      CUP_CARD_ABNORMAL        = 1;         //CPU读卡器异常

const   U16      ACCELE_SENSOS_NORMAL     = 0;         //加速度传感器正常
const   U16      ACCELE_SENSOS_ABNORMAL   = 1;         //加速度传感器异常

const   U16      TEMP_SENSOS_NORMAL       = 0;         //温度传感器正常
const   U16      TEMP_SENSOS_ABNORMAL     = 1;         //温度传感器异常

const   U16      EEPROM_NORMAL            = 0;         //EEPROM正常
const   U16      EEPROM_ABNORMAL          = 1;         //EEPROM异常


const int        HEART_TYPE               = 1;         //心跳包数据类型
const int        COMMAND_TYPE             = 2;         //命令帧数据类型

//静音状态相关常量
const   U32      MUTE_STATE               = 1;         //静音状态
const   U32      NOT_MUTE_STATE           = 0;         //非静音状态

//通道开关量常数
const	U16	 CHA_ON                       = 1;	       //通道开关量，表示开，为保证2字节，因此使用16进制定义
const	U16	 CHA_OFF                      = 0;         //通道开关量，表示关


//启停电极阻抗测量的常数定义
const   U16      START_ELEC_CHECK         = 1;         //启动电极阻抗测量
const   U16      STOP_ELEC_CHECK          = 0;         //停止电极阻抗测量

//刺激器开关量常数定义
const   U16      STI_ON                   = 1;         //刺激器开关量，表示开
const   U16      STI_OFF                  = 0;         //刺激器开关量，表示关

//刺激器电极类型常数定义
const   U16      STI_SINGLE               = 0;         //单电极
const   U16      STI_DOUBLE               = 1;         //双电极

//增减偏移
const	U16	 DEC		                  = 1;         //减1
const	U16	 INC		                  = 0;	       //加1


//电极阻抗测量类型常数
const   U16      CHANNEL_CHECK            = 0;         //通道检测
const   U16      STI_CHECK                = 1;         //刺激器检测
const   U16      GND_CHECK                = 2;         //接地检测

//校验类型常量
const  U16       NO_VERIFY                = 0;         //无校验
const  U16       ODD_EVEN_VERIFY          = 3;         //奇偶校验
const  U16       SUM_VERIFY               = 2;         //校验和
const  U16       CRC32_VERIFY             = 1;         //CRC校验

//采集器复位外设号
const  U32       RESET_NUM_INVALID        = 0;         //无效
const  U32       RESET_NUM_CHANNEL        = 1;         //通道单片机
const  U32       RESET_NUM_AD             = 2;         //AD芯片
const  U32       RESET_NUM_PGA308         = 4;         //PGA308放大器

//时钟信息结构体
struct REAL_TIME_CALENDAR
{
    U16 year;	        //年
    U8  month;          //月
    U8  date;	        //日
    U8  hour;           //时
    U8  minute;         //分
    U8  second;         //秒
    U8  maintain;       //保留
    //重载操作符“=”
    void operator = (const REAL_TIME_CALENDAR &tmp)
    {
        year     = tmp.year;
        month    = tmp.month;
        date     = tmp.date;
        hour     = tmp.hour;
        minute   = tmp.minute;
        second   = tmp.second;
        maintain = tmp.maintain;
    }
    //重载操作符“==”
    bool operator == (const REAL_TIME_CALENDAR &tmp)
    {
        if(year == tmp.year && month == tmp.month && date == tmp.date &&
                minute == tmp.minute && second == tmp.second)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};   //共8字节


//设备管理人基本信息结构体
const	U8	    MANAGER_INFO_ID_LEN          = 16;
const	U8	    MANAGER_INFO_NAME_LEN        = 24;
const	U8	    MANAGER_INFO_SEX_LEN         = 8;
const	U8	    MANAGER_INFO_BIRTHDAY        = 16;
const	U8	    MANAGER_INFO_TELEPHONE       = 16;
const	U8	    MANAGER_INFO_NATION_LEN      = 8;
const	U8	    MANAGER_INFO_MARRY_LEN       = 8;
const	U8	    MANAGER_INFO_ADDRESS_LEN     = 64;
const	U8	    MANAGER_INFO_WORK_LEN        = 64;
const	U8	    MANAGER_INFO_INS_CODE_LEN    = 32;
const	U8	    MANAGER_INFO_ZIP_CODE_LEN    = 16;
const	U8	    MANAGER_INFO_COUNTRY         = 64;
const	U8	    MANAGER_INFO_HEIGHT          = 8;
const	U8	    MANAGER_INFO_WEIGHT          = 8;
const	U8	    MANAGER_INFO_FILL_DATE_LEN   = 32;

struct Device_Manager_Info {
    U8		u8ID[MANAGER_INFO_ID_LEN];                   // 设备管理者ID，由医院管理，EDF需要
    U8		u8Name[MANAGER_INFO_NAME_LEN];               // 设备管理者姓名，EDF需要
    U8		u8Sex[MANAGER_INFO_SEX_LEN];                 // 性别（'M'Male or 'F' Female），EDF需要
    U8		u8Birthday[MANAGER_INFO_BIRTHDAY];           // 出生，格式：dd-MMM-yyyy,EDF需要
    U8		u8Telephone[MANAGER_INFO_TELEPHONE];         // 联系电话,EDF需要
    U8		u8Nation[MANAGER_INFO_NATION_LEN];           // 民族，比如汉
    U8		u8Marry[MANAGER_INFO_MARRY_LEN];             // 婚否，true  or false
    U8		u8Address[MANAGER_INFO_ADDRESS_LEN];         // 家庭住址
    U8		u8Work[MANAGER_INFO_WORK_LEN];               // 工作单位
    U8		u8Insurance_Code[MANAGER_INFO_INS_CODE_LEN]; // 社会保险号
    U8		u8Zip_Code[MANAGER_INFO_ZIP_CODE_LEN];       // 家庭住址的邮政编码
    U8		u8Country[MANAGER_INFO_COUNTRY];             // 国家
    U8		u8Height[MANAGER_INFO_HEIGHT];               // 身高，单位：cm
    U8		u8Weight[MANAGER_INFO_WEIGHT];               // 体重，单位：kg
    U8		u8Maintain[MANAGER_INFO_FILL_DATE_LEN];      // 保留值，共32字节，必须填零
};                                                       // 设备管理者基本信息，共256字节

//所有通道的基本信息结构体
const	U8	ALL_CHANNEL_FILL_DATE_LEN = 8;

struct All_Channel_Basic_Info {
    U16		u16Machine_No;				                 // 机器号
    U16		u16Sample_Channel_Number;	                 // 当前采样通道数，能够工作通道总数
    U16		u16Physical_Channel_Number;	                 // 物理通道数
    U16		u16Hide_Channel_Number;		                 // 隐藏通道数，隐藏通道用于环境测量
    U8		u8Maintain[ALL_CHANNEL_FILL_DATE_LEN];       // 数据配齐
};                                                       // 所有通道基本信息 共16字节

//通道采样率结构体
const	U8	MAX_SR_NUMBER	= 25;		                // 最大采样率档位
const   U8  SAMPLE_RATE_INFO_REPLENISH   =  22;         // 数据补齐

struct	Sample_Rate_Info {
    U16		u16Sample_Rate_Number;		                // 采样率档位，比如：20，
    float	fDef_Sample_Rate;                           // 当前采样率，单位：Hz
    float	fSample_Rate[MAX_SR_NUMBER];                // 采样率，单位：Hz
    U8		u8Maintain[SAMPLE_RATE_INFO_REPLENISH];		// 数据配齐
};// 采样率信息 共128字节
typedef Sample_Rate_Info  SAMPLE_RATE_INFO,*PSAMPLE_RATE_INFO;


//1个通道的基本信息结构体
const   U8      TRANSDUCER_TYPE_LEN  = 16;
const   U8      SIGNAL_LABEL_LEN     = 16;
const   U8      PHYSICAL_METRIC_LEN  = 16;
const   U8      CHANNEL_COMMAND_LEN  = 4;

struct	One_Channel_Basic_Info {
    U16		u16Machine_No;				                // 机器号
    U16		u16Sample_Channel_No;		                // 采样通道号，从0开始编码
    U16		u16Physical_Channel_No;		                // 对应的物理通道号，从0开始编码
    U16		u16Hide_Channel_No;			                // 隐藏通道号，从0开始编码
    U16		u16Channel_Type;			                // 当前通道类型，通用，隐藏，ECG
    U16		u16Data_Bits_Number;		                // 数据位数，16位，32位，64位
    U8		u8Transducer_Type[TRANSDUCER_TYPE_LEN];	    // 传感器类型，“TP-101”,“None”…   备注:详细请参见《生物信号采集与处理系__软硬件通讯标准》
    U8		u8Signal_Label[SIGNAL_LABEL_LEN];	        // 信号标签，“Force”,“Preesure”…
    U8		u8Physical_Metric[PHYSICAL_METRIC_LEN];	    // 物理度量，比如uV 或 degreeC
    U8		u8Channel_Command[CHANNEL_COMMAND_LEN];	    // 通道可以支持的命令
}; // 通道信息 共64字节
typedef One_Channel_Basic_Info  ONE_CHANNEL_BASIC_INFO,*PONE_CHANNEL_BASIC_INFO;


//通道增益范围结构体
const	U8	MAX_RANGE_NUMBER	  = 25;	               // 最大量程档位
const   U8  RANGE_INFO_REPLENISH  = 46;                // 数据补齐
struct	Range_Info {
    U16		u16Range_Number;	                       // 量程档位，比如：20
    float	fDef_MinRange;      	   	               // 默认量程，采用高值表达，比如：1mV
    float	fDef_MaxRange;			                   // 默认量程，采用高值表达，比如：1mV
    float	fMin_Range[MAX_RANGE_NUMBER];              // 量程低值
    float	fMax_Range[MAX_RANGE_NUMBER];              // 量程高值
    U8	    u8Maintain[RANGE_INFO_REPLENISH];		   // 数据配齐
};						     // 增益范围信息共256字节
typedef Range_Info RANGE_INFO,*PRANGE_INFO;

//通道滤波信息结构结构体
const	U8	 MAX_LP_NUMBER = 25;	              // 最大低通滤波档位
const	U8	 MAX_HP_NUMBER = 25;                  // 最大高通滤波档位
const   U16  CLOSE_50NOTCH = 0;                   // 关闭50Hz滤波开关
const   U16  OPEN_50NOTCH  = 1;                   // 打开50Hz滤波开关
const   U8   MAINTAIN_REPLENISH = 42;             // 数据补齐

struct Filter_Info {
    float		fDef_LP_Filter;		              // 默认低通滤波值，比如：100Hz
    float		fDef_HP_Filter;		              // 默认高通滤波值，比如：1Hz
    U16		    u16LP_Filter_Number;              // 低通滤波档位数，比如：20
    U16		    u16HP_Filter_Number;	          // 高通滤波档位数，比如：20
    float		fLP_Filter[MAX_LP_NUMBER];        // 每一档低通滤波值，单位：Hz
    float		fHP_Filter[MAX_HP_NUMBER];        // 每一档高通滤波值，单位：Hz
    U16		    u16Def_50Hz_Notch;	              // 默认50Hz滤波开关，非零打开50Hz
    U8		    u8Maintain[MAINTAIN_REPLENISH];	  // 数据配齐
};						     // 滤波信息共256字节
typedef Filter_Info  FILETER_INFO, *PFILETER_INFO;


//刺激波表支持的最大刺激点数
const   U16      BLL_OUTPUT_STIMULUS_WAVETABLE_MAXPOINTS   = 1000;

//刺激模式
enum BLL_OUTPUT_STIMULUS_MODE
{
    BLL_OUTPUT_STIMULUS_MODE_TOF = 0,       //TOF刺激
    BLL_OUTPUT_STIMULUS_MODE_PTC = 1,       //PTC刺激
    BLL_OUTPUT_STIMULUS_MODE_STS = 2,       //STS刺激
    BLL_OUTPUT_STIMULUS_MODE_DBS = 3,       //DBS刺激
};

//刺激类型
enum BLL_OUTPUT_STIMULUS_TYPE
{
    BLL_OUTPUT_STIMULUS_TYPE_VOLTAGE = 0,  //恒压刺激
    BLL_OUTPUT_STIMULUS_TYPE_CURRENT = 1,  //恒流刺激
};

//刺激波表单元结构体
struct Bll_Output_Stimulus_Wavetablecell
{
    float value;                  //刺激值
    float time;                   //刺激时间
};

//
struct STIM_PARAMER_OLD{
    U16   tofStimulateCur;          //TOF刺激电流 ， 0~70mA
    U16   tofStimulateFre;          //TOF刺激频率, 1、2HZ
    U16   tofStimulateWav;          //TOF刺激波宽， 0.1、0.2、0.3ms,下发时乘上100
    U16   tofContinuousTime;        //TOF连续刺激时间,  10~300s
    U16   ptcStimulateCur;          //PTC刺激电流 ， 0~70mA
    U16   ptcStimulateFre;          //PTC刺激频率, 50、100HZ
    U16   ptcStimulateWav;          //PTC刺激波宽， 0.1、0.2、0.3ms,下发时乘上100
    U16   ptcStimulateTime;         //PTC强直刺激时间，1~10s
    U16   ptcStimulateTimeInterval; //PTC刺激间隔时间, 1~10s
    U16   ptcContinuousTime;        //PTC连续刺激时间,  5~60min
    U16   stsStimulateCur;          //STS刺激电流 ， 0~70mA
    U16   stsStimulateWav;          //STS刺激波宽， 0.1、0.2、0.3ms,下发时乘上100
    U16   stsContinuousTime;        //STS连续刺激时间,  1、10s
    U16   dbsStimulateCur;          //DBS刺激电流 ， 0~70mA
    U16   dbsStimulateFre;          //DBS刺激频率, 50、100HZ
    U16   dbsStimulateWav;          //DBS刺激波宽， 0.1、0.2、0.3ms,下发时乘上100
    U16   dbsStimulateUnite;        //DBS刺激联数， 0（二联），1（三联）
    U16   dbsContinuousTime;        //DBS连续刺激时间,  10~300s
};  //共36字节

//刺激参数结构体
struct STIM_PARAMER{
    U16   stimulateCur;             //刺激电流 ， 均为0~70mA
    U16   stimulateFre;             //刺激频率,   (TOF)1、2HZ  | (PTC)50、100HZ | (DBS)50、100HZ
    U16   stimulateWav;             //刺激波宽，  均为0.1、0.2、0.3ms,下发时乘上100
    U16   ptcStimulateTime;         //PTC强直刺激时间，1~10s
    U16   ptcStimulateTimeInterval; //PTC刺激间隔时间, 1~10s
    U16   dbsStimulateUnite;        //DBS刺激联数， 0（二联），1（三联）
    //初始化刺激参数
    STIM_PARAMER()
    {
        stimulateCur             = 50;
        stimulateFre             = 2;
        stimulateWav             = 2;
        ptcStimulateTime         = 5;
        ptcStimulateTimeInterval = 3;
        dbsStimulateUnite        = 1;
    }
};  //共12字节
typedef	struct  STIM_PARAMER 	HW_STIM_PARA, *PHW_STIM_PARA;

//设置刺激模式
struct SET_STIMULUS_MODE
{
    U32 mode;            //刺激模式
    STIM_PARAMER para;   //刺激参数
};  //共16字节

//一屏事件帧结构（TOF刺激）
struct OneScreenFrameTOFData{
    short m_shCurPlusIndex;     //当前脉冲编号
    short m_shSetCurrent;       //设置电流
    short m_shRealCurrent;      //实际电流
    short m_shAmplitude_x;       //x轴刺激幅度
    short m_shAmplitude_y;       //y轴刺激幅度
    short m_shAmplitude_z;       //z轴刺激幅度

};

//一屏事件帧结构（PTC刺激）
struct OneScreenFramePTCData{



};

//一屏事件帧结构（STS刺激）
struct OneScreenFrameSTSData{



};

//一屏事件帧结构（DBS刺激）
struct OneScreenFrameDBSData{



};




//---------------------------------------------------------------－－－－－－－－－－－－－－－－－－－－－－－－－－－－
//---------------------------------------【NMT-100协议2.0】-----------------------－－－－－－－－－－－－－－－－－－－
//---------------------------------------------------------------－－－－－－－－－－－－－－－－－－－－－－－－－－－－

/*
 * 命令帧头部结构，NMT-100上层软件向下层硬件发送的命令帧头部结构，
 * 头部结构后面可跟带可变大小的参数
*/
const  short    FRAME_HEAD_LEN=   24;    //帧头长度

struct Command_Frame_Head {
    U16      u16Start_Mark;             //起始标志，2字节，默认0xFFFF
    U16      u16Version;                //版本号，2字节,默认为0
    U32      u32Frame_Num;              //帧编号，4字节，默认值为0，每发送一帧值递增1
    U16      u16Device_Num;             //机器号，2字节，默认为0
    U16      u16Command_Code;           //命令码，2字节，默认填0
    U16      u16Frame_Len;              //帧长度，2字节，默认填24(帧头固定大小为24字节)
    U8       u8Reserved[4];             //保留字段，4字节，默认置为0
    U16      u16Check_Type;             //校验类型，2字节，默认置为0
    U32      u32Check_Code;             //校验码，4字节，默认置为0

    //初始化命令帧头部字段
    Command_Frame_Head()
    {
        u16Start_Mark   = 0xFFFF;
        u16Version      = 0;
        u32Frame_Num    = 0;
        u16Device_Num   = 0;
        u16Command_Code = 0;
        u16Frame_Len    = FRAME_HEAD_LEN;
        u16Check_Type   = 0;
        u32Check_Code   = 0;
    }
}; //命令帧头部大小为24字节


struct RTC_DATE
{
    U16 year;       //年
    U8 month;       //月
    U8 day;         //日
};
struct RTC_TIME
{
    U8 hour;      //时
    U8 minute;    //分
    U8 second;    //秒
    U8 week;      //周
};

struct RTC_INFO
{
    RTC_DATE date;   //日期
    RTC_TIME time;   //时间
};

//1、设备系统管理命令(命令或上0x8000表示是下层回复上层的对应命令)
const	U16     NOP                       = 0x8000;    //空指令 无参数(作为心跳包检验连接是否有效)
const	U16     RESET                     = 0x0001;    //系统复位 参数1：复位方式
const	U16     RESET_UPLOAD              = 0x8001;    //回复系统复位 参数1：复位方式
const	U16	    GET_INFO	              = 0x0002;    //获取硬件基本信息       参数1：请求获取信息类型
const	U16	    GET_INFO_UPLOAD           = 0x8002;    //获取硬件基本信息上传    参数1：信息类型
const	U16	    SET_INFO	              = 0x0003;    //设置基本信息    参数1：信息类型  1个特参 备注:  特殊参数:硬件使用信息(32 字节)
//用户单位基本信息(256字节)  struct Work_Unit_Info (详见 结构4) \设备管理人基本信息(256字节)  Device_Manager_Info (详见 结构5)
const   U16     SET_INFO_UPLOAD           = 0X8003;    //设置基本信息上传 参数1：信息类型  1个特参

const	U16	    MATCH_HW                  = 0x000A;    //配对硬件
const	U16	    MATCH_HW_UPLOAD           = 0x800A;    //配对硬件回传
const   U16     CHOOSE_RUN_MODE           = 0x000B;    //选择运行模式  参数1：运行模式
const   U16     CHOOSE_RUN_MODE_UPLOAD    = 0x800B;    //选择运行模式上传
const   U16     ENTER_PROGRAM_UPDATE      = 0x0010;    //命令设备进入程序升级状态
const   U16     ENTER_PROGRAM_UPDATE_UPLOAD= 0x8010;   //命令设备进入程序升级状态
const   U16     GET_HW_CHECK_RESULT        = 0x0011;   //获取硬件自检结果
const   U16     GET_HW_CHECK_RESULT_UPLOAD = 0x8011;   //获取硬件自检结果上传
const   U16     GET_LEAD_IMPENDANCE_CHECK_RESULT        = 0x0013;  //获取电极阻抗测量结果
const   U16     GET_LEAD_IMPENDANCE_CHECK_RESULT_UPLOAD = 0x8013;  //获取电极阻抗测量结果上传
const	U16	    ENABLE_EVENT              = 0x0014;    // 允许上传事件	         无参数
const	U16	    DISABLE_EVENT             = 0x0015;    // 禁止上传事件	         无参数
const	U16	    REQUEST_SETTING_HWRTC        = 0x0016;    // 下层主动请求时钟同步
const	U16	    REQUEST_SETTING_HWRTC_UPLOAD = 0x8016;    // 请求时钟同步上传
const	U16	    SET_HW_RTC                = 0x0017;    // 时钟同步          1个特参    特殊参数:时钟信息（8字节）  响应是否执行成功
const	U16	    SET_HW_RTC_UPLOAD         = 0x8017;
const	U16	    GIVE_HW_ID                = 0x0018;    // 报告自身硬件ID号   2个参数    机器号(4字节)  特殊参数:硬件ID   备注:设备在机器号为0时反复主动上报自身硬件ID号
const	U16	    SET_HW_NO	              = 0x0019;    // 设置机器号        2个参数    机器号(4字节)  特殊参数:硬件ID   备注:设备在硬件ID匹配的情况下才识别执行此命令
const	U16	    GET_HW_ID                 = 0x001A;    // 获取硬件ID号      2个参数    机器号(4字节)  特殊参数:硬件ID
const	U16	    SET_SAMPLE_MODE           = 0x001B;    // 设置采样模式       2个参数    模式 (4字节)
const	U16	    SET_FRAME_LEN             = 0x001C;    // 设置数据帧长度	   1个参数     帧长 (4字节）
const   U16     SET_SAMPLE_FRAME_NUMBER   = 0x001D;    // 设置每次探测到刺激后上报数据的帧数
const   U16     FAST_GET_LEAD_IMPENDANCE_CHECK_RESULT          = 0x001F;    // 快速阻抗测量
const   U16     FAST_GET_LEAD_IMPENDANCE_CHECK_RESULT_UPLOAD   = 0x801F;
const   U16     NOTICE_MUTE_STATE         = 0x0022;    // 通知静音状态
const   U16     NOTICE_MUTE_STATE_UPLOAD  = 0x8022;
const   U16     RESET_PERIPHERAL          = 0x0023;    //复位指定外设，此命令为解决采集盒静电而设
const   U16     RESET_PERIPHERAL_UPLOAD   = 0x8023;
const   U16     SKIN_ANNEX_INFO           = 0x0024;    //跳过附件卡认证命令
const   U16     SKIN_ANNEX_INFO_UPLOAD    = 0x8024;
const   U16     SWITCH_ONOFF_WIFI         = 0x0025;    //打开或关闭wifi功能
const   U16     GET_ACCELERATION_STATUS            = 0x0027;    //加速度传感器连接状态
const   U16     GET_ACCELERATION_STATUS_UPLOAD     = 0x8027;    //加速度传感器连接状态回复  1个参数（4字节） ： 0（正常），！0（异常）

//设备刺激管理命令（暂未使用）
const	U16     SET_STI_MODE               = 0x0051;    //设置刺激器模式              2个参数(8字节)    刺激通道号、模式  参数1，参数2，参数3均为4字节
const	U16     SET_STI_DELAY1             = 0x0053;    //设置延时1                  2个参数(8字节)    刺激通道号、延时1
const	U16     SET_STI_DELAY2             = 0x0054;    //设置延时2                  2个参数(8字节)    刺激通道号、延时2
const	U16     SET_STI_DELAY2_UPLOAD      = 0x8054;
const	U16     SET_STI_WIDTH1             = 0x0055;    //设置波宽1                  2个参数(8字节)    刺激通道号、波宽
const	U16     SET_STI_WIDTH1_UPLOAD      = 0x8055;
const   U16     SET_STI_WIDTH2             = 0x0056;    //设置波宽2                  2个参数(8字节)    刺激通道号、波宽
const   U16     SET_STI_INTERVAL           = 0x0057;    //设置波间隔       　　       2个参数(8字节)    刺激通道号、波间隔
const	U16     SET_STI_FREQUENCY          = 0x0058;    //设置频率                   2个参数(8字节)    刺激通道号、频率
const	U16     SET_STI_AMPLITUDE1         = 0x0059;    //设置强度1                  2个参数(8字节)    刺激通道号、幅度1
const	U16     SET_STI_AMPLITUDE1_UPLOAD  = 0x8059;
const   U16     SET_STIM_PARAMER           = 0x0060;    //设置刺激参数                2个参数
const	U16     SET_STIM_PARAMER_UPLOAD    = 0x8060;    //设置刺激参数回传             2个参数
const	U16     SET_STI_AMPLITUDE2         = 0x005A;    //设置强度2                  2个参数(8字节)    刺激通道号、幅度2
const	U16     SET_STI_CLUSTER_NUM        = 0x005B;    //设置串刺激个数              2个参数(8字节)    刺激通道号、串个数
const	U16     SET_STI_WAVE_TABLE         = 0x005C;    //设置刺激波表    　　　       2个参数(8+8*n字节)刺激通道号、波表
const   U16     SET_STI_FAKE_DELAY         = 0x005D;    //设置假象延迟    　　　       2个参数(8字节)    刺激通道号、延时
const   U16     OPEN_CHANNEL_STI           = 0x005E;    //刺激器开关
const   U16     OPEN_CHANNEL_STI_UPLOAD    = 0x805E;
const   U16     SET_LEAD_TYPE              = 0x005F;    //设置电极类型

//计算机事件管理命令
const	U16     GET_HW_EVT_DIG             = 0x0070;    //查询指定通道号的数字输入状态   2个参数(8) 数字输入通道号、状态值
const	U16     SET_HW_EVT_DIG             = 0x0071;    //设置指定通道号的数字输出状态   2个参数(8) 数字输出通道号、状态值
const	U16     GET_HW_EVT_USE             = 0x0072;    //查询设备使用操作相关事件状态   3个参数(8) 事件源、事件类型、状态值
const	U16     GET_HW_EVT_STI             = 0x0073;    //查询设备刺激事件状态   　　   3个参数(12) 事件类型、状态值、真实刺激强度
const	U16     GET_HW_EVT_CNT_DROPS       = 0x0074;    //查询设备记滴事件状态         2个参数(8) 事件类型、状态值

//设备事件上报命令（数据帧）
const	U16     REPORT_HW_EVT_DIG          = 0x0080;    //上报指定通道号的数字输入状态   3个参数(12) 数字输入通道号、事件类型、状态值
const	U16     REPORT_HW_EVT_USE          = 0x0081;    //上报设备使用操作相关事件状态   3个参数(12) 事件源、事件类型、状态值
const	U16     REPORT_HW_EVT_STI_STATE    = 0x0082;    //上报设备刺激事件状态  　　    3个参数(12) 事件类型、状态值、真实刺激强度
const	U16     REPORT_HW_EVT_STI_CHANGE   = 0x0083;    //上报设备刺激强度   　　　　   3个参数(12) 事件类型、设置强度、真实强度
const	U16     REPORT_HW_EVT_CNT_DROPS    = 0x0084;    //上报设备记滴事件状态   　　   2个参数(8) 事件类型、状态值
const	U16     REPORT_CALIBRATION_SUCCESS        = 0x0085;    //校准完成上报   　　          2个参数(8) 最适电流、校准幅度值
const	U16     REPORT_CALIBRATION_SUCCESS_UPLOAD = 0x8085;    //校准完成回复   　　          2个参数(8) 最适电流、校准幅度值
const   U16     SEND_CH_DATA               = 0x0102;    //数据帧

//新增命令
const   U16     GET_TEMPERATURE            = 0x2400;    //获取温度传感器外设温度值
const   U16     GET_TEMPERATURE_UPLOAD     = 0xA400;    //温度值上传                  1个参数（4字节） ： 温度值
const   U16     SWITCH_CALIBRATION         = 0x2401;    //校准开关                    1个参数（4字节） ： 0（关），！0（开）
const   U16     SWITCH_CALIBRATION_UPLOAD  = 0xA401;    //回复校准


//1.1 系统复位方式
const   U32   RESETTYPE_POWER_DOWN         = 0;    //掉电复位
const   U32   RESETTYPE_PIN                = 1;    //引脚复位
const   U32   RESETTYPE_SOFTWARE           = 2;    //软件复位
const   U32   RESETTYPE_WATCHDOG           = 3;    //独立看门狗复位
const   U32   RESETTYPE_OTHER              = 4;    //其他复位

//1.2 请求获取信息类型：

const   U32   PARATYPE_HW_INVALID_INFO     = 0;    //无效信息
const   U32   PARATYPE_HW_BASIC_INFO       = 1;    //获取硬件基本信息
const   U32   PARATYPE_HW_STATE_INFO       = 2;    //获取硬件状态信息
const   U32   PARATYPE_HW_USE_INFO         = 3;    //获取硬件使用信息
const   U32   PARATYPE_WORK_UNIT_INFO      = 4;    //获取用户单位基本信息
const   U32   PARATYPE_DEVICE_MANAGER_INFO = 5;    //获取设备使用者基本信息
const   U32   PARATYPE_USER_CONFIG_INFO    = 6;    //获取用户配置信息

//1.3 运行模式常量定义(对硬件)

const   U16      PAUSE                    = 0;         //暂停(冻结)模式，不刺激，不采集
const   U16      MONITOR                  = 1;         //监视(连续触发)模式
const   U16      DETECTION                = 2;         //探测(刺激触发)模式
const   U16      MONITOR_NO_STIM          = 3;         //探测不刺激模式
const   U16      STOP                     = 4;         //停止模式

//1.4 硬件自检结构体

const   U16      MAX_CHANNEL_NUMBER                 = 8;         //最大通道数
const   U16      BLL_OUTPUT_STIMULUS_CHANNEL_NUMBER = 2;         //输出刺激数
const   U16      MAX_POWERSOURCE_NUMBER             = 4;         //最大电源数

struct HW_SELFCHECK_RESULT
{
    U8 sampleChMcuComm[MAX_CHANNEL_NUMBER];       //采样通道检测结果，默认为8通道，0表示无异常，1表示通道单片机通讯异常，2表示通道放大器通讯异常。
    U8 stimulusChAmpComm[BLL_OUTPUT_STIMULUS_CHANNEL_NUMBER];     //刺激通道检测结果，0表示无异常，1表示刺激电路调零异常
    U8 dataSample;               //数据采样功能检测结果，0表示无异常，1表示无法采样数据
    U8 powerSource[MAX_POWERSOURCE_NUMBER];           //电源检测结果，0无异常，1电压偏低，2电压偏高(第一个为模拟电源+5V,第二个为模拟电源-5V,第3个为刺激电源-24V,第4个为刺激电源-72V)
    U8 accelerationSensorCheck ; //加速度传感器检测结果，0表示无异常，1表示有异常
    U8 temperatureCheck;         //温度传感器检测结果，0表示无异常，1表示有异常
    U8 maintain[15];             //保留字段
};  //共32字节

//1.5 硬件基本信息结构体   注： 硬件自动生成，出厂后不变
const	U8  	DEVICE_TYPE_LEN             =  32;	 // 机器类型字符串长度
const	U8  	SERIES_NO_LEN               =  16;	 // 序列号字符串长度
const	U8  	MANUFACTURE_LEN             =  96;	 // 制造商字符串长度
const	U8  	CONFIG_TYPE_LEN             =  16;	 // 机器类型配置字符串长度
const	U8  	HW_BASIC_INFO_FILL_DATA_LEN =  16;   // 数据配齐需要填入的字节数

typedef struct HW_Basic_Info
{
  U8 dataStyleVersion[4]; /* 字符，数据格式版本 */
  U8 hardwareDataManage[32]; /* 硬件型号，上层软件根据该型号解释信息 */
  U32 channel; /* 通道数，硬件支持通道数 */
  U32 versionMain; /* 固件主版本号，硬件内置软件（固件）的主版本号 */
  U32 versionMinor; /* 固件次版本号，硬件内置软件（固件）的次版本号 */
  RTC_DATE productDay; /* 生产日期 */
  U32 batch; /* 批次，生产批次 */
  U32 expandLen; /* 扩展信息长度，硬件扩展信息长度，跟在基本信息后 */
  U32 productNum; /* 批次总数量，次批次生产的产品总数量 */
  U32 productSN; /* 序列号，次产品的序列号 */
  U8 company[96]; /* 公司名称 */
  U8 maintain[28]; /* 保留，备今后扩展使用 */
} HW_BASIC_INFO; /* 共192字节 */                    // 硬件基本信息共192字节
typedef	struct  HW_Basic_Info *PHW_BASIC_INFO;

//1.6 硬件状态信息结构体   注： 硬件自动生成，可变
const	U8   	STATE_INFO_FILL_DATA_LEN  =  32;     // 数据配齐需要填入的字节数

struct HWDate {
    U16		u16Year;		                         // 年
    U8	  	u8month;                                 // 月,1-12
    U8	  	u8Date;                                  // 日,1-31
    U8	  	u8Hour;                                  // 时
    U8	  	u8Minute;                                // 分
    U8	  	u8Second;                                // 秒
    U8	  	u8Maintain;                              // 保留，为了8字节配齐
}; // 日期数据结构

struct HW_State_Info {
    HWDate  stRealTime;                              // 实时时钟，反馈当前硬件的实时时钟
    U32	 	u32RemainBat;                            // 剩余电量，电池剩余电量百分比（0-100）
    U32	 	u32SizeUnit;                             // 总空间单位，硬件总存储空间单位0--位，
    U32	 	u32Size;                                 // 总空间大小，硬件总存储空间大小
    U32	 	u32RemainSizeUnit;                       // 剩余空间单位，硬件剩余空间大小单
    U32	 	u32RemainSize;                           // 剩余空间大小，硬件剩余空间大小
    float	fSysTemp;                                // 内置温度
    U8	  	u8Maintain[STATE_INFO_FILL_DATA_LEN];    // 保留，备今后扩展使用 有32个
};                                                   // 硬件状态信息结构体，共64字节
typedef  HW_State_Info  HW_STATE_Info,*PHW_STATE_INFO;

//1.7 硬件使用信息结构体    注： 硬件自动生成，可变
const	U8    USE_INFO_FILL_DATA_LEN   = 10;      	// 数据配齐需要填入的字节数

struct HW_Use_Info {
    U16		u16Device_No;			                // 设备号
    U16		u16Used_Number;                         // 使用次数，只有启动采样才算使用
    U32		u32Used_Sum_Duration;                   // 累计使用时间，单位：s，可以到136年
    U32		u32Max_Duration;                        // 最长使用时间，单位：s，可以到136年
    U16		u16First_Used_Year;                     // 第一次使用的年
    U16		u16First_Used_Month;                    // 第一次使用的月
    U16		u16First_Used_Day;                      // 第一次使用的日
    U16		u16Last_Used_Year;                      // 上次使用的年
    U16		u16Last_Used_Month;                     // 上次使用的月
    U16		u16Last_Used_Day;                       // 上次使用的日
    U8	    u8Maintain[USE_INFO_FILL_DATA_LEN];     // 保留值，共10字节，必须填为0
};                                                  // 硬件使用信息结构体 共32字节
typedef  HW_Use_Info  HW_USE_INFO,*PHW_USE_INFO;

//1.8 用户单位基本信息结构体  注：有效信息来自上层软件
const	U8     WORK_UNIT_ID_LEN        = 32;           // 工作单位编号字符串长度
const	U8     WORK_UNIT_NAME_LEN      = 64;           // 工作单位名称字符串长度
const	U8     WORK_UNIT_ADDRESS_LEN   = 64;           // 地址字符串长度
const	U8     WORK_UNIT_TELEPHONe_LEN = 32;           // 电话字符串长度
const	U8     WORK_UNIT_WEB_LEN       = 32;           // 网址字符串长度
const	U8     WORK_UNIT_EMAIL_LEN     = 32;           // Email字符串长度

struct	Work_Unit_Info {
    U8      u8Work_Unit_ID[WORK_UNIT_ID_LEN];		       // 工作单位编号，EDF需要
    U8      u8Work_Unit_Name[WORK_UNIT_NAME_LEN];		   // 工作单位名称
    U8      u8Work_Unit_Address[WORK_UNIT_ADDRESS_LEN];	   // 地址，医院名可能重复，地址不会
    U8      u8Work_Unit_Telephone[WORK_UNIT_TELEPHONe_LEN];// 电话，可能是总机，考虑有分机
    U8      u8Work_Unit_Web_Address[WORK_UNIT_WEB_LEN];    // 网址，打印报告时使用
    U8      u8Work_Unit_Email[WORK_UNIT_EMAIL_LEN];		   // Email，打印报告时使用
};// 用户单位基本信息，共256字节

//1.9 设备使用者基本信息   注：有效信息来自上层软件
struct	Device_User_Info {
    U8      u8Work_Unit_ID[WORK_UNIT_ID_LEN];		       // 工作单位编号，EDF需要
    U8      u8Work_Unit_Name[WORK_UNIT_NAME_LEN];		   // 工作单位名称
    U8      u8Work_Unit_Address[WORK_UNIT_ADDRESS_LEN];	   // 地址，医院名可能重复，地址不会
    U8      u8Work_Unit_Telephone[WORK_UNIT_TELEPHONe_LEN];// 电话，可能是总机，考虑有分机
    U8      u8Work_Unit_Web_Address[WORK_UNIT_WEB_LEN];    // 网址，打印报告时使用
    U8      u8Work_Unit_Email[WORK_UNIT_EMAIL_LEN];		   // Email，打印报告时使用
};

//1.10 用户配置信息    注：有效信息来自上层软件
struct	User_Config_Info {

};


//1.11 获取电极阻抗测量结果
//阻抗测量电极类型
enum  HAL_LDIM_LEAD_TYPE
{
    HAL_LDIM_LEAD_TYPE_CHA = 0,   //通道电极类型
    HAL_LDIM_LEAD_TYPE_STI = 1,   //刺激器电极类型
    HAL_LDIM_LEAD_TYPE_GND = 2,   //接地电极类型
};

//阻抗测量结果 --- 单通道
struct HAL_LDIM_RESULT_CELL
{
    U16   channelType;           //通道类型
    U16   channelId;             //通道号
    U32   impendancePositive;    //正极阻抗值
    U32   impendanceNegitive;    //负极阻抗值
};

//阻抗测量结果---所有通道
struct HAL_LDIM_RESULT
{
    U32 channelNumber;           //通道数
    HAL_LDIM_RESULT_CELL result[MAX_CHANNEL_NUMBER + 3];  //各个通道阻抗测量结果
};


//2、数据帧结构体
struct DataFrame
{
    short shMode;            //刺激模式     每种模式的分别是：00（TOF），01（TOF）02（DBS）03（STS）
    short shCurPlusindex;    //当前脉冲编号
    short fSetCurrent;       //设置电流
    short fRealCurrent;      //实际电流
    short amplitude_x;       //x轴刺激幅度
    short amplitude_y;       //y轴刺激幅度
    short amplitude_z;       //z轴刺激幅度
    RTC_INFO rtcInfo;        //时钟信息
};//22byte字节


//3、错误指示帧
const	U16     ERROR_INDICATOR_VERSION    = 0x9000;	 //版本号字段错误，跟2字节参数，被响应的命令码
const	U16     ERROR_INDICATOR_DEVICEID   = 0x9001;     //机器号字段错误，跟2字节参数，被响应的命令码
const	U16     ERROR_INDICATOR_FRAMELEN   = 0x9002;	 //帧长字段错误，跟2字节参数，被响应的命令码
const	U16     ERROR_INDICATOR_VERIFYTYPE = 0x9003;	 //校验类型字段错误，跟2字节参数，被响应的命令码
const	U16     ERROR_INDICATOR_VERIFY     = 0x9004;	 //校验错误，跟2字节参数，被响应的命令码
const	U16     ERROR_INDICATOR_INVALIDCMD = 0x9005;	 //无效命令，跟2字节参数，被响应的命令码
const	U16     ERROR_INDICATOR_PARAERR    = 0x9006;	 //参数错误  备注:跟2字节参数，被响应的命令码后面续跟2字节参数错误类型PARAMETER_CHECK_ERR (详见 结构12)
const	U16     ERROR_INDICATOR_CANTEXE    = 0x9007;	 //命令无法执行，跟2字节参数，被响应的命令码

//3.1 参数错误类型常量定义
enum PARAMETER_CHECK_ERR{
    PARAMETER_CHECK_ERR_CHANNELNUM = 1,                    // 通道号不正确
    PARAMETER_CHECK_ERR_SAMPLERATE = 2,                    // 采样率不正确
    PARAMETER_CHECK_ERR_FRAMELEN = 3,                      // 帧长不正确
    PARAMETER_CHECK_ERR_MORETHAN_THROUGHPUT = 4,           // 超过吞吐率限制
    PARAMETER_CHECK_ERR_ALLCHSWITCHOFF = 5,                // 所有通道均关闭
    PARAMETER_CHECK_ERR_CALENDAR = 6,                      // 日期时钟信息不合法
    PARAMETER_CHECK_ERR_UUID = 7,                          // 硬件ID号不一致
    PARAMETER_CHECK_ERR_MACHINENO = 8,                     // 机器号超出最大允许范围
    PARAMETER_CHECK_ERR_LEADNUM = 9,                       // 电极编号不正确
    PARAMETER_CHECK_ERR_INVALIDRESULT = 10,                // 结果不可用（阻抗测量）
    PARAMETER_CHECK_ERR_STIMMODE = 11,                     // 刺激模式不正确
    PARAMETER_CHECK_ERR_STIMTYPE = 12,                     // 刺激类型不正确
    PARAMETER_CHECK_ERR_STIMDELAY = 13,                    // 刺激延迟不正确
    PARAMETER_CHECK_ERR_STIMFREQ = 14,                     // 刺激频率不正确
    PARAMETER_CHECK_ERR_STIMAMPL = 15,                     // 刺激强度不正确
    PARAMETER_CHECK_ERR_STIMSERIALNUM = 16,                // 刺激串脉冲数不正确
    PARAMETER_CHECK_ERR_CONFLICT_STI_IMP = 17,             // 刺激和阻抗测量有冲突
    PARAMETER_CHECK_ERR_GEARNUMBER = 18,                   // 设置档位数不正确
    PARAMETER_CHECK_ERR_RANGE = 19,                        // 量程不正确
    PARAMETER_CHECK_ERR_RANGESETTING = 20,                 // 前一量程设置还未执行完毕
    PARAMETER_CHECK_ERR_INCREMENTAL = 21,                  // 直流偏移增量不正确
    PARAMETER_CHECK_ERR_DCINCSETTING= 22,                  // 前一直流偏移增量设置还未执行完毕
    PARAMETER_CHECK_ERR_STIMULUALEADTYPE = 23,             // 刺激电极类型不正确
    PARAMETER_CHECK_ERR_FRAMENUM = 24,                     // 探测模式突发传输帧数不正确
};


#endif // HARDWARECONSTANTDEFINE_H
