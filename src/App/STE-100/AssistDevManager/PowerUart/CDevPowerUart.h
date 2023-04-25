#ifndef CDEVPOWERUART_H
#define CDEVPOWERUART_H
#include "PowerUartApplication.h"
#include "CSerialPortCom.h"
#include "CDeviceBase.h"
#include "CConfirmDevPowerUart.h"
#include "CDataParsePowerUart.h"

#include <atomic>
#include <set>
using namespace std;

//电源设备是软件中的固定设备，应该保证设备的存在，需要在当前设备中进行设备的重连
class CDevPowerUart : public CDeviceBase
{
    Q_OBJECT
public:
    CDevPowerUart();
    virtual ~CDevPowerUart() override;

    //通过发信号方式 进行设备工作设置，避免内部通讯串口操作在一个线程中
    void setWorkState(bool bState);

    //设置充电状态
    void setChargeState(bool bCharge);

    //设置LED灯带(关机、系统错误、体测犯规、体测进行中、体测结果锁定、默认)
    void setLightBelt(unsigned short shMode, int nTestCode = 100);

    //由于后续变更体测设备也存在灯光变化，将灯光控制命令通过静态函数进行统一生成
    //shMode 是由 LED_BELT_MODE_SHUTDOWN 同类命令组成
    static void getLightBeltParam(QByteArray &arr, unsigned short shMode, int nTestCode = 100);

    //获取平板剩余电量
    static short getRemindPower();

    //获取硬件电池剩余电量
    short getRemindPowerForPowerUart();

    //打开设备
    bool devOpen() override;
protected:
    //发送心跳命令
    void sendHeartBeatCmd();
    void sendChargeCmd(unsigned short shChargeType);
    void sendLightModeCmd(unsigned short shMode);
    void sendLightBeltCmd(unsigned short shMode, int nTestCode);
    //发送供电状态命令获取 剩余电量和连接充电状态
    void sendPowerSupplyState();
    void sendCmdToDev(unsigned short shCmd, unsigned short *shCmdType = nullptr, unsigned short shCmdTypeLen = 0);

    /**
     * 成功解析一帧函数触发消息，和解析类处理同一个线程，当前类需要设置到解析类中进行触发
     * 详细的解析看解析类中的东西
     */
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen) override;

    //更新数据
    void onUpdateTimeOut() override;

    //设备重连
    virtual void reconnect() override;

    //设置串口属性
    void setSerialPortProperty(CSerialPortCom *pCom);

    //删除记录的串口中存在的无效设备
    void delMapAccordToComList(const QStringList &comList);

    //记录一次无效串口名称
    void recordInvalidComName(const QString &qsComName);

    //找到目标设备
    void findTargetDev(const QString &qsComName);
signals:
    void signalWorkState(bool bState);

    //电量变更信号，在cmainframe中和信号进行关联,平板电量
    void signalBatteryChange(short shRemaind);

    //底层供电设备电量变更
    void signalPowerUintBatteryChange(short shPowerUintRemaind);

    //供电设备连接电源状态变更
    void signalPowerUintConnec220VChange(bool bConnect);
protected slots:
    //查询主机电量（平板）
    void queryingHostPower();

    static int  GetColorIndexByTestCode(int nTestCode);

    void onSetWorkState(bool state);
protected:
    //充电相关定时变量
    bool m_bCharge{false};
    unsigned int m_iChargeInteval{ 1500 };
    unsigned int m_iChargeIntevalTotal{ 0 };

    //电量检测，1分钟检测一次
    unsigned int m_iQueryBatteryInteval{ 3 * 1000 };
    unsigned int m_iQueryBatteryIntevalTotal{ 0 };

    //本地取数据容器，避免频繁的进行数据申请释放
    unsigned char *m_pDataContainer{nullptr};
    unsigned short m_shDataCOntainerSize;

    //记录平板剩余电量
    short m_shRemaindPower{-1};

    short m_shRemaindPowerForPowerUart{-1};

    bool m_bUpdatePowerInfo{true};

    //是否连接220v电源
    bool m_bPowerUintConnect220VPower{false};

    short m_shSendPadQueryBatteryIndex{0};

    //上一次接收心跳时间
    qint64 m_int64LastRecvTime{0};

    //用于查找设备让设备自动运行
    CSerialPortCom *m_pSerialPortCom{nullptr};

    CDataParsePowerUart *m_pDataParse{nullptr};
    CConfirmDevPowerUart *m_pConfirmDev{nullptr};

    //定时向底层设备发送心跳
    int m_nSendHeartbeatInteval{300};
    int m_nSendHeartbeatTotal{0};

    //记录无用设备，在设备连接上之后删除，在设备未连接时给每个设备3次重连机会
    map<QString, int> m_mapMarkingDev;

    //记录电源串口名称
    QString m_qsComName{""};
};

#endif // CDEVPOWERUART_H
