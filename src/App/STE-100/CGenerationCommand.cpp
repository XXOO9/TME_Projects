#include "CGenerationCommand.h"
#include "commondefin.h"
#include "SteDevManager/SteDevice/CSteCommonDefin.h"
#include <QDebug>
using namespace DataFrameInfo;

CGenerationCommand::CGenerationCommand()
{

}

//这个命令只针对具体的设备进行生成命令
void CGenerationCommand::getCmd(QByteArray &arr, ushort shCmd, uint nDataType, uchar cHardwareCode, uint nDevID)
{
    arr.clear();
    ushort shFrameLenth = INDEX_DATA_CONTENT + sizeof(CONTENT_DATA_CONTENT);

    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    //设备类型 (************)
    memcpy(pDestData + INDEX_DEV_TYPE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pDestData + INDEX_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pDestData + INDEX_DEV_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pDestData + INDEX_DATA_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pDestData + INDEX_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //数据内容/命令参数 (查询数据指令没有当前参数)  (************)
    memcpy(pDestData + INDEX_DATA_CONTENT, &nDataType, sizeof(CONTENT_DATA_CONTENT));
}

void CGenerationCommand::getCmdNew(QByteArray &arr, ushort shCmd, uint shDataType, uchar cHardwareCode, uint nDevID, uint nSubDevId, uint nFrameNo)
{
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4） + 子设备id + 数据类型(4)}
    arr.clear();
    ushort shFrameLenth = 46;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &nFrameNo, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //子设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nSubDevId, sizeof(CONTENT_HARDWARE_ID));
    //数据类型
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);
}

QByteArray CGenerationCommand::getCmdSetDataReport(bool bFlag)
{
    QByteArray arr;
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 8字节 {设备id（填0即可） + 0或者1 1为开启}
    ushort shFrameLenth = 42;
    arr.resize(shFrameLenth);

    ushort shCmd = DATA_FRAME_TYPE_SET_DATA_REPORT;
    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    uchar cHardwareCode = 0;
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    uint nDevID = 0;
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);
    //上报标志
    uint nReportFlag = bFlag ? 1 : 0;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nReportFlag, 4);

    return arr;
}

ushort CGenerationCommand::get433EnterCfg(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_433_ENTER_CFG;
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4） + 卡号id}
    ushort shFrameLenth = 46;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &nFrameNo, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);
    //多功能参数
    uint nReportFlag = 0;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nReportFlag, 4);
    //数据类型码
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return shCmd;
}

ushort CGenerationCommand::get433ExistCfg(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_433_EXIST_CFG;
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4） + 卡号id}
    ushort shFrameLenth = 46;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &nFrameNo, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);
    //多功能参数
    uint nReportFlag = 0;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nReportFlag, 4);
    //数据类型码
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return shCmd;
}

ushort CGenerationCommand::get433ClearCfg(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_433_CLEAR_CFG;
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4） + 卡号id}
    ushort shFrameLenth = 46;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &nFrameNo, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);
    //多功能参数
    uint nReportFlag = 0;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nReportFlag, 4);
    //数据类型码
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return shCmd;
}

ushort CGenerationCommand::get433SetChannel(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode, uint nDevID, uint nChannelNo)
{
    ushort shCmd = DATA_FRAME_TYPE_433_SET_CHANNEL;
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4） + 卡号id}
    ushort shFrameLenth = 46;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &nFrameNo, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);
    //多功能参数
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nChannelNo, 4);
    //数据类型码
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return shCmd;
}

ushort CGenerationCommand::getCmdSetTimeOut(cmdInfo &info, uint shDataType, uchar cHardwareCode, uint nDevID, uint nTimeOut)
{
    info.shCmdType = DATA_FRAME_TYPE_SET_TIME_OUT;
    //后面12字节参数分别为 设备id（nDevID） 多功能参数：倒计时时间（nTimeOut） 数据类型码（shDataType）
    getCmdNew(info.arrCmd, info.shCmdType, shDataType, cHardwareCode, nDevID, nTimeOut);

    return info.shCmdType;
}

void CGenerationCommand::getCmdGetTimeOut(cmdInfo &info, uint shDataType, uchar cHardwareCode, uint nDevID)
{
    info.shCmdType = DATA_FRAME_TYPE_GET_TIME_OUT;
    //后面12字节参数分别为 设备id（nDevID） 多功能参数（0） 数据类型码（shDataType）
    getCmdNew(info.arrCmd, info.shCmdType, shDataType, cHardwareCode, nDevID, 0);
}

ushort CGenerationCommand::getSetIllegalTimes(cmdInfo &info, uint shDataType, uchar cHardwareCode, uint nDevID, int nTimes)
{
    ushort shCmd = DATA_FRAME_TYPE_SET_ILLEGAL_TIMES;
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4） + 卡号id}
    ushort shFrameLenth = 46;
    QByteArray &arr = info.arrCmd;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &info.nFrameNo, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);
    //多功能参数
    uint unTimes = static_cast<uint>(nTimes);
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &unTimes, 4);
    //数据类型码
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return shCmd;
}

ushort CGenerationCommand::getLedCmd(QByteArray &arr, const QByteArray &ledCmd, uchar cHardwareCode, uint nDevID)
{
    arr.clear();
    ushort shFrameLenth = INDEX_DATA_CONTENT + static_cast<ushort>(ledCmd.size()) + 4;
    arr.resize(shFrameLenth);
    memset(arr.data(), 0, shFrameLenth);

    ushort shCmd = DATA_FRAME_BODY_LED_BAND_EFFECT;

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    //设备类型 (************)
    memcpy(pDestData + INDEX_DEV_TYPE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pDestData + INDEX_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pDestData + INDEX_DEV_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pDestData + INDEX_DATA_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pDestData + INDEX_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));


    //设备id
    memcpy(pDestData + INDEX_DATA_CONTENT, &nDevID, sizeof(CONTENT_DEV_ID));

    //数据内容/命令参数 (查询数据指令没有当前参数)  (************) (灯光控制命令为10字节命令)
    memcpy(pDestData + INDEX_DATA_CONTENT + 4, (uchar *)ledCmd.data(), ledCmd.size());

    return shCmd;
}

ushort CGenerationCommand::getCmdNull(QByteArray &arr, uchar cHardwareCode/* = 0x00*/, uint nDevID /*= 0*/)
{
    ushort shCmd = DATA_FRAME_TYPE_NULL;
    getCmd(arr, shCmd, 0,cHardwareCode, nDevID);
    return shCmd;
}

ushort CGenerationCommand::getCmdRestory(QByteArray &arr, uchar cHardwareCode /*= 0x00*/, uint nDevID /*= 0*/)
{
    ushort shCmd = DATA_FRAME_TYPE_RESTORY;
    getCmd(arr, DATA_FRAME_TYPE_RESTORY, 0, cHardwareCode, nDevID);
    return shCmd;
}

ushort CGenerationCommand::getCmdGetInfo(QByteArray &arr, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_GET_INFO;
    getCmd(arr, shCmd, 1, cHardwareCode, nDevID);

    return shCmd;
}

ushort CGenerationCommand::getCmdSwork(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, uint nSubDevId)
{
    ushort shCmd = DATA_FRAME_TYPE_SWORK;

    getCmdNew(arr, shCmd, shDataType, cHardwareCode, nDevID, nSubDevId);
//    getCmd(arr, shCmd, shDataType, cHardwareCode, nDevID);

//    QByteArray arrTemp(arr);
//    short shLenth = static_cast<short>(arrTemp.size() + 4);
//    arr.resize(shLenth);

//    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
//    memcpy(pDestData, arrTemp.data(), arrTemp.size());

//    //变更长度
//    memcpy(pDestData + INDEX_FRAME_LENTH, &shLenth, sizeof(CONTENT_FRAME_LENTH));

//    //在最后四字节前加上子设备id

//    //数据内容/命令参数 (查询数据指令没有当前参数)  (************)
//    memcpy(pDestData + INDEX_DATA_CONTENT, &nSubDevId, sizeof(CONTENT_DATA_CONTENT));
//    memcpy(pDestData + INDEX_DATA_CONTENT + 4, &shDataType, sizeof(CONTENT_DATA_CONTENT));


    return shCmd;
}

ushort CGenerationCommand::getCmdEwork(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, uint nSubDevId)
{
    ushort shCmd = DATA_FRAME_TYPE_EWORK;
    getCmdNew(arr, shCmd, shDataType, cHardwareCode, nDevID, nSubDevId);
//    getCmd(arr, shCmd, shDataType, cHardwareCode, nDevID);


//    QByteArray arrTemp(arr);
//    short shLenth = static_cast<short>(arrTemp.size() + 4);
//    arr.resize(shLenth);

//    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
//    memcpy(pDestData, arrTemp.data(), arrTemp.size());

//    //变更长度
//    memcpy(pDestData + INDEX_FRAME_LENTH, &shLenth, sizeof(CONTENT_FRAME_LENTH));

//    //在最后四字节前加上子设备id

//    //数据内容/命令参数 (查询数据指令没有当前参数)  (************)
//    memcpy(pDestData + INDEX_DATA_CONTENT, &nSubDevId, sizeof(CONTENT_DATA_CONTENT));
//    memcpy(pDestData + INDEX_DATA_CONTENT + 4, &shDataType, sizeof(CONTENT_DATA_CONTENT));

    return shCmd;
}

ushort CGenerationCommand::getCmdCalibrate(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, float fCalibrateVale)
{
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4）+ 数据类型(4) + 校准值（float 4）}
    arr.clear();
    ushort shFrameLenth = 46;
    arr.resize(shFrameLenth);

    ushort shCmd = DATA_FRAME_TYPE_CALIBRATION;

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //数据类型
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nDataType, 4);
    //校准值
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &fCalibrateVale, 4);

    return shCmd;
}

ushort CGenerationCommand::getCmdSworkRunMid(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, uint nSubDevId)
{
    shDataType = DATA_FRAME_TYPE_SWORK;

    arr.clear();
    ushort shFrameLenth = 46;

    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &DATA_FRAME_TYPE_SWORK, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //卡号id
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nSubDevId, sizeof(CONTENT_HARDWARE_ID));
    //数据类型
    uint nDataType = DATA_FRAME_TYPE_RUN_MID;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return DATA_FRAME_TYPE_SWORK;
}

ushort CGenerationCommand::getCmdEworkRunMid(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, uint nSubDevId)
{
    shDataType = DATA_FRAME_TYPE_EWORK;
    arr.clear();
    ushort shFrameLenth = 46;

    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &DATA_FRAME_TYPE_EWORK, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //卡号id
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nSubDevId, sizeof(CONTENT_HARDWARE_ID));
    //数据类型
    uint nDataType = DATA_FRAME_TYPE_RUN_MID;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return DATA_FRAME_TYPE_EWORK;
}

ushort CGenerationCommand::getCmdSworkRopeSkip(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, ushort shSkipTime, ushort shWaitTime)
{
    ushort shCmd = DATA_FRAME_TYPE_SWORK;

    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 12字节 {设备id（4） + 子设备id + 数据类型(4)}
    arr.clear();
    ushort shFrameLenth = 46;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //跳绳时间
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &shSkipTime, sizeof(ushort));
    //等待时间
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 2, &shWaitTime, sizeof(ushort));
    //数据类型
    uint nDataType = shDataType;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return shCmd;
}

ushort CGenerationCommand::getCmdStime(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_STIME;
    getCmd(arr, shCmd, shDataType, cHardwareCode, nDevID);
    return shCmd;
}

ushort CGenerationCommand::getCmdEtime(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_ETIME;
    getCmd(arr, shCmd, shDataType, cHardwareCode, nDevID);
    return  shCmd;
}

ushort CGenerationCommand::getCmdGetData(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_GET_DATA;
    //获取数据结尾没有数据内容
    arr.clear();
    ushort shFrameLenth = INDEX_DATA_MARK + sizeof(INDEX_DATA_MARK);
    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    //设备类型 (************)
    memcpy(pDestData + INDEX_DEV_TYPE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pDestData + INDEX_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pDestData + INDEX_DEV_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pDestData + INDEX_DATA_TYPE, &shDataType, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pDestData + INDEX_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    return shCmd;
}

ushort CGenerationCommand::getCmdReportData(QByteArray &arr, ushort shDataType, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_REPORT_DATA;
    getCmd(arr, shDataType, 0, cHardwareCode, nDevID);
    return shCmd;
}

ushort CGenerationCommand::getCmdGetId(QByteArray &arr, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_GET_ID;
    getCmd(arr, shCmd, 0, cHardwareCode, nDevID);
    return shCmd;
}

ushort CGenerationCommand::getCmdRunMidRecordCard(QByteArray &arr, const uint &nNFCCardId, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_READCARD;
    //命令组成为 数据头（24）INDEX_COMMON_DATA_HEAD_LEN + 公共数据长度（10） Packet_Commond_Head + 8字节 {设备id（4） + 卡号id}
    ushort shFrameLenth = 42;
    arr.resize(shFrameLenth);

    ///---------------- 公共数据头(24) ---------------------------------
    //起始标志 帧头
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 数据包位置(单包数据公共数据头) 10-------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;

    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memset(pPacketStart + PACK_HARDWARE_ID, 0, sizeof(CONTENT_DEV_ID));
    //memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));

    ///---------------- 命令内容（）-------------------------------------
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //卡号
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nNFCCardId, sizeof(CONTENT_HARDWARE_ID));
    return shCmd;
}

ushort CGenerationCommand::getCmdSworkFootball(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, uint nWaitTime)
{
    shDataType = DATA_FRAME_TYPE_SWORK;

    arr.clear();
    ushort shFrameLenth = 46;

    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &DATA_FRAME_TYPE_SWORK, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //等待时间
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nWaitTime, sizeof(CONTENT_HARDWARE_ID));
    //数据类型
    uint nDataType = DATA_FRAME_TYPE_FOOTBALL;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return DATA_FRAME_TYPE_SWORK;
}

ushort CGenerationCommand::getCmdSworkBasketball(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, uint nWaitTime)
{
    shDataType = DATA_FRAME_TYPE_SWORK;

    arr.clear();
    ushort shFrameLenth = 46;

    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &DATA_FRAME_TYPE_SWORK, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //等待时间
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nWaitTime, sizeof(CONTENT_HARDWARE_ID));
    //数据类型
    uint nDataType = DATA_FRAME_TYPE_BASKETBALL;
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4 + 4, &nDataType, 4);

    return DATA_FRAME_TYPE_SWORK;
}

ushort CGenerationCommand::getCmdGetExtensionNo(QByteArray &arr, uint nFrameNo, uchar cHardwareCode, uint nDevID , uint nSubDevId)
{
    nSubDevId = nDevID;
    ushort shCmd = DATA_FRAME_TYPE_GET_EXTENSION_NO;

    arr.clear();
    ushort shFrameLenth = 42;
    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &nFrameNo, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nDevID, sizeof(CONTENT_HARDWARE_ID));

    return shCmd;
}

ushort CGenerationCommand::getCmdSetExtensionNo(QByteArray &arr, uint nExtensionNo, uchar cHardwareCode, uint nDevID)
{
    ushort shCmd = DATA_FRAME_TYPE_SET_EXTENSION_NO;

    arr.clear();
    ushort shFrameLenth = 42;
    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cHardwareCode, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //设备id
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, sizeof(CONTENT_HARDWARE_ID));
    //分机号
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nExtensionNo, sizeof(CONTENT_HARDWARE_ID));

    return shCmd;
}

ushort CGenerationCommand::getCmdGetChannelInfo(QByteArray &arr)
{
    ushort shCmd = DATA_FRAME_TYPE_GET_CHANNEL_INFO;

    arr.clear();
    ushort shFrameLenth = 0x26;
    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    uchar cDevType = HardwarePropertye::HARDWARE_CODE_NETWORK;
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cDevType, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    uint nDevID = 0;
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //四字节整型0
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);

    return shCmd;
}

ushort CGenerationCommand::getCmdSetChannelInfo(QByteArray &arr, uint nMaxConnectNum, uint nChannelNo)
{
    ushort shCmd = DATA_FRAME_TYPE_SET_CHANNEL;

    arr.clear();
    ushort shFrameLenth = 42;
    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    uchar cDevType = HardwarePropertye::HARDWARE_CODE_NETWORK;
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cDevType, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    uint nDevID = 0;
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //最大连接数
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nMaxConnectNum, sizeof(CONTENT_HARDWARE_ID));
    //信道
    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nChannelNo, sizeof(CONTENT_HARDWARE_ID));

    return shCmd;
}

ushort CGenerationCommand::getCmdGetTimeStamp(QByteArray &arr)
{
    ushort shCmd = DATA_FRAME_TYPE_GET_TIME_STAMP;

    arr.clear();
    ushort shFrameLenth = 0x2A;
    arr.resize(shFrameLenth);

    //起始标志 帧头 0
    uchar *pDestData = reinterpret_cast<uchar *>(arr.data());
    memcpy(pDestData + INDEX_FH, &CONTENT_FH, sizeof(CONTENT_FH));
    //版本号 2
    memcpy(pDestData + INDEX_VERSION, &CONTENT_VERSION, sizeof(CONTENT_VERSION));
    //帧号 4
    memcpy(pDestData + INDEX_FRAME_NO, &CONTENT_FRAME_NO, sizeof(CONTENT_FRAME_NO));
    //机器号 8
    memcpy(pDestData + INDEX_MACHINE_NO, &CONTENT_MACHINE_NO, sizeof(CONTENT_MACHINE_NO));
    //命令码 10
    memcpy(pDestData + INDEX_CMD, &CONTENT_CMD, sizeof(CONTENT_CMD));
    //帧长度 12 (************)
    memcpy(pDestData + INDEX_FRAME_LENTH, &shFrameLenth, sizeof(CONTENT_FRAME_LENTH));
    //保留位 reserved
    memcpy(pDestData + INDEX_RESERVED, &CONTENT_RESERVED, sizeof(CONTENT_RESERVED));
    //校验类型
    memcpy(pDestData + INDEX_CHECK_TYPE, &CONTENT_CHECK_TYPE, sizeof(CONTENT_CHECK_TYPE));
    //校验码
    memcpy(pDestData + INDEX_CHECK_CODE, &CONTENT_CHECK_CODE, sizeof(CONTENT_CHECK_CODE));

    ///---------------- 以下作为一个数据包 -------------------
    uchar *pPacketStart = pDestData + INDEX_COMMON_DATA_HEAD_LEN;
    //设备类型 (************)
    uchar cDevType = HardwarePropertye::HARDWARE_CODE_NETWORK;
    memcpy(pPacketStart + PACK_HARDWARE_CODE, &cDevType, sizeof(CONTENT_DEV_TYPE));
    //数据点数  (************)
    memcpy(pPacketStart + PACK_DATA_POINT_NUM, &CONTENT_DATA_POINT_NUM, sizeof(CONTENT_DATA_POINT_NUM));
    //设备ID (************)
    uint nDevID = 0;
    memcpy(pPacketStart + PACK_HARDWARE_ID, &nDevID, sizeof(CONTENT_DEV_ID));
    //、单点数据
    //数据类型/命令码  (************)
    memcpy(pPacketStart + PACK_CMD_TYPE, &shCmd, sizeof(CONTENT_DATA_TYPE));
    //数据标记
    memcpy(pPacketStart + PACK_DATA_MARK, &CONTENT_DATA_MARK, sizeof(CONTENT_DATA_MARK));
    //四字节整型0
    memcpy(pPacketStart + PACK_DATA_MARK + 2, &nDevID, 4);

    memcpy(pPacketStart + PACK_DATA_MARK + 2 + 4, &nDevID, 4);

    return shCmd;
}
