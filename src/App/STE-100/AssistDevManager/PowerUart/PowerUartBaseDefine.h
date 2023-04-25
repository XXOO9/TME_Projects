#ifndef PowerUartBaseDefine_h
#define PowerUartBaseDefine_h

/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:PowerUartBaseDefine.h
* 文件标识:
* 摘要: 该文件主要包含通信协议的函数定义声明
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2018.05.04
*/

//定义数据的传输大小范围

#define DATA_LEN_MAX                           (unsigned short)(16)//数据占用最大半字数,只包含应用数据的数据部分

//定义传输帧的默认参数
#define POWER_UART_PKG_HEAD_TAG	               (unsigned char)0xFC//数据包帧头,传输帧带双帧头

#define CMD_POWER_DEVICE_HEART                 (unsigned short)0x8001//设备心跳命令

#define CMD_SET_POWER_DEVICE_PAD_ChargeE        (unsigned short)0x8002//设置设备平板充电状态命令
#define CMD_UPLOAD_POWER_DEVICE_PAD_ChargeE     (unsigned short)0x9002//上传设备平板充电状态命令
	
#define CMD_GET_POWER_DEVICE_SUPPLY            (unsigned short)0x8003//获取设备本地电量(未启用)
#define CMD_UPLOAD_POWER_DEVICE_SUPPLY         (unsigned short)0x9003//上传设备本地电量(未启用)
	
#define CMD_SET_POWER_DEVICE_LEDBAND_MODE      (unsigned short)0x8004//设置电源设备呼吸灯模式
#define CMD_UPLOAD_POWER_DEVICE_LEDBAND_MODE   (unsigned short)0x9004//上传电源设备呼吸灯模式
//add cmd

#define CMD_UPLOAD_POWER_DEVICE_ERROR          (unsigned short)0x90FF//上传设备错误命令


//Uart连接状态定义
typedef enum EnumPowerDeviceUartConnectType
{
	POWER_UART_CONNCET_NORMAL = 0x00, //连接正常
	POWER_UART_CONNCET_NOTSURE,       //连接不确定
	POWER_UART_CONNCET_UNUSUAL,       //连接异常
}PowerDeviceUartConnectType;

//平板充电状态定义
typedef enum EnumPadChargeeType
{
    PAD_ChargeE_OFF = 0x0000, //平板充电关
    PAD_ChargeE_ON = 0x0001,  //平板充电开
}PadChargeeType;

//平板充电状态定义
typedef enum EnumLedbandModeType
{
	LEDBAND_MODE_BREATHE = 0x0000, //灯带呼吸模式-用于待测状态
	LEDBAND_MODE_LOOP,             //灯带循环模式-用于测量中
}LedbandModeType;

//LED灯带
enum EnumLedBeltModeType
{
	LED_BELT_MODE_SHUTDOWN = 1,           // 关机
	LED_BELT_MODE_SYSTEM_ERROR,           // 系统错误
	LED_BELT_MODE_BODY_TEST_FOUL,         // 体测犯规
	LED_BELT_MODE_BODY_TESTING,           // 体测进行中
	LED_BELT_MODE_BODY_TEST_RESULT_LOCK,  // 体测结果锁定
	LED_BELT_MODE_DEFUALT,                // 默认
};

//设置灯效模式
enum EnumLEDBeltEffectType
{
	LEDBAND_MODE_TURNOFF             = 0x00,
	LEDBAND_MODE_TURNON              = 0x01,
	LEDBAND_MODE_BREATHE_NEW         = 0x02,
	LEDBAND_MODE_FLASH               = 0x03,
	LEDBAND_MODE_WATERFALL_ONECOLOR  = 0x04,
	LEDBAND_MODE_WATERFALL_COLORFULL = 0x05,
};

//设置灯颜色
enum EnumLEDBeltColorType
{
    LED_BELT_COLOR_GOLD = 0,         // 金色
    LED_BELT_COLOR_SILVER,           // 银灰色
    LED_BELT_COLOR_ALICEBLUE,        // 爱丽丝蓝
    LED_BELT_COLOR_COPPERSTAINS,     // 铜色
    LED_BELT_COLOR_LIME,             // 闪光绿
    LED_BELT_COLOR_BLUE,             // 纯蓝
    LED_BELT_COLOR_PURPLE,           // 紫色
};

#pragma pack(push)
#pragma pack(1)

//应用帧结构体定义
typedef struct
{
	unsigned short m_CmdType;
	unsigned short m_Data[DATA_LEN_MAX];
}PowerUartApplicationDataStruct;

//链路帧头结构体定义
typedef struct
{
	unsigned char m_Head1;
	unsigned char m_Head2;
	unsigned short m_CheckSum;//包含长度的校验和
	unsigned short m_Len;//表示的是U16长度,不计算自身
}PowerUartDataLinkFrameHeadStruct;

//完整数据包结构体定义
typedef struct
{
	PowerUartDataLinkFrameHeadStruct m_DataLink;
	PowerUartApplicationDataStruct m_AppData;
}PowerUartFrameStruct;

#pragma pack(pop)

extern unsigned short PowerUartCheckSum16(unsigned short initValue, unsigned short *pBuf, unsigned short destLen);

#endif
