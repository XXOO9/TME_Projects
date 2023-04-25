#ifndef CGENERATIONCOMMAND_H
#define CGENERATIONCOMMAND_H
#include <QByteArray>
#include <QDateTime>
///---------------- 以下作为一个数据包 -------------------
//设备类型
const static ushort INDEX_DEV_TYPE = 24;
const static uchar  CONTENT_DEV_TYPE = 0x00;
//数据点数
const static ushort INDEX_DATA_POINT_NUM = 25;
//设备ID
const static ushort INDEX_DEV_ID = 26;
const static uint   CONTENT_DEV_ID = 0x01020304;
//、单点数据
//数据类型/命令码
const static ushort INDEX_DATA_TYPE = 30;
const static ushort CONTENT_DATA_TYPE = 0x0000;
//数据标记
const static ushort INDEX_DATA_MARK = 32;
//数据内容/命令参数 (查询数据指令没有当前参数)
const static ushort INDEX_DATA_CONTENT = 34;
const static uint   CONTENT_DATA_CONTENT = 0x00000000;

struct cmdInfo{
    uint nFrameNo;
    QByteArray arrCmd;
    ushort shCmdType;
    ushort shIndex;    //命令作用的测试单元（如开始命令需要检测是对哪个测试单元进行的），无关命令不考虑
    qint64 llCmdTime;
    qint64 llLastSendTime; //命令每隔500ms进行发送
    int nInterval; //控制命令的间隔时间，默认500ms根据具体命令调整时间
    uint nParam; //命令参数，某些命令需要核对对应的参数才行

    cmdInfo()
      : llCmdTime(QDateTime::currentMSecsSinceEpoch())
      , nInterval{500}
    {
        llLastSendTime = llCmdTime - nInterval;
    }

    //设置间隔时间需要将上一次的变为命令时间减间隔时间，让第一条信息立马发送
    void setInteval(const int &interval)
    {
        nInterval = interval;
        llLastSendTime = llCmdTime - interval;
    }

    //判断 == 只判断帧号 命令类型 针对的测试单元
    bool operator==(const cmdInfo& cmd) const
    {
        return nFrameNo == cmd.nFrameNo && shIndex == cmd.shIndex && shCmdType == cmd.shCmdType;
    }
};


//nSubDevId通常是多功能参数
class CGenerationCommand
{
public:
    CGenerationCommand();

    static void getCmd(QByteArray &arr, ushort shCmd, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getLedCmd(QByteArray &arr, const QByteArray &ledCmd, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdNull(QByteArray &arr, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdRestory(QByteArray &arr, uchar cHardwareCode = 0x00, uint nDevID = 0);
    //获取信息类型
    static ushort getCmdGetInfo(QByteArray &arr, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdSwork(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0);
    static ushort getCmdEwork(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0);

    //校准命令
    static ushort getCmdCalibrate(QByteArray &arr, uint shDataType, uchar cHardwareCode, uint nDevID, float fCalibrateVale);

    //nSubDevId 在中长跑中值卡号
    static ushort getCmdSworkRunMid(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0);
    static ushort getCmdEworkRunMid(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0);
    static ushort getCmdSworkRopeSkip(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, ushort shSkipTime = 0, ushort shWaitTime = 0);
    static ushort getCmdStime(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdEtime(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdGetData(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdReportData(QByteArray &arr, ushort shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdGetId(QByteArray &arr, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort getCmdRunMidRecordCard(QByteArray &arr, const uint &nNFCCardId, uchar cHardwareCode = 0x00, uint nDevID = 0);

    //足球、篮球的开始命令(与跳绳类似，需要等待时间)
    static ushort getCmdSworkFootball(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nWaitTime = 0);
    //static ushort getCmdEworkFootball(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0);
    static ushort getCmdSworkBasketball(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nWaitTime = 0);
    //static ushort getCmdEworkBasketball(QByteArray &arr, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0);

    //获取分机编号
    static ushort getCmdGetExtensionNo(QByteArray &arr, uint nFrameNo, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0);
    static ushort getCmdSetExtensionNo(QByteArray &arr, uint nExtensionNo, uchar cHardwareCode = 0x00, uint nDevID = 0);

    //获取信道信息
    static ushort getCmdGetChannelInfo(QByteArray &arr);
    static ushort getCmdSetChannelInfo(QByteArray &arr, uint nMaxConnectNum, uint nChannelNo);

    //获取组网中心时间戳
    static ushort getCmdGetTimeStamp(QByteArray &arr);

    //新的通用命令获取函数
    static void getCmdNew(QByteArray &arr, ushort shCmd, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nSubDevId = 0, uint nFrameNo = 0);
    //设置底层数据上报，true为上报，false为停止上报
    static QByteArray getCmdSetDataReport(bool bFlag);

    //------------ 设置433信道相关函数 --------------
    //进入 退出 433配置模式
    static ushort get433EnterCfg(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);
    static ushort get433ExistCfg(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);
    //清除433配对信息
    static ushort get433ClearCfg(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);

    //设置433信道，需要进入配置模式之后设置 范围 1-189
    static ushort get433SetChannel(QByteArray &arr, uint nFrameNo, ushort shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nChannelNo = 0);

    //倒计时信息
    static ushort getCmdSetTimeOut(cmdInfo &info, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, uint nTimeOut = 0);
    static void getCmdGetTimeOut(cmdInfo &info, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0);

    //设置犯规个数参数
    static ushort getSetIllegalTimes(cmdInfo &info, uint shDataType, uchar cHardwareCode = 0x00, uint nDevID = 0, int nTimes = 0);
};

#endif // CGENERATIONCOMMAND_H
