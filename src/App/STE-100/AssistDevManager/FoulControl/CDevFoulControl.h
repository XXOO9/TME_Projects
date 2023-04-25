#ifndef CDevFoulControl_H
#define CDevFoulControl_H
#include "CSerialPortCom.h"
#include "CDeviceBase.h"
#include "CDataParseFoulControl.h"
#include "CConfirmDevFoulControl.h"

#include <atomic>
#include <set>
using namespace std;

namespace FOUL_DEV_CMD{

//当前设备命令表示的含义,犯规设备 心跳 违例 犯规（硬件定义）
enum{
    FoulDevHR = 0, FoulDevIllegal = 2, FoulDevFoul = 1
};
}

//犯规控制设备
class CDevFoulControl : public CDeviceBase
{
    Q_OBJECT
public:
    CDevFoulControl();
    virtual ~CDevFoulControl() override;

    //通过发信号方式 进行设备工作设置，避免内部通讯串口操作在一个线程中
    void setWorkState(bool bState);

    //打开设备
    bool devOpen() override;
protected:
    //发送心跳命令
    void sendHeartBeatCmd();

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

    //使用 FoulTypeExternal
    void sigSendFoulType(unsigned short shFoulType);
protected slots:

    void onSetWorkState(bool state);
protected:
    //本地取数据容器，避免频繁的进行数据申请释放
    unsigned char *m_pDataContainer{nullptr};
    unsigned short m_shDataCOntainerSize;

    //上一次接收心跳时间
    qint64 m_int64LastRecvTime{0};

    //用于查找设备让设备自动运行
    CSerialPortCom *m_pSerialPortCom{nullptr};

    CDataParseFoulControl *m_pDataParse{nullptr};
    CConfirmDevFoulControl *m_pConfirmDev{nullptr};

    //定时向底层设备发送心跳
    int m_nSendHeartbeatInteval{300};
    int m_nSendHeartbeatTotal{0};

    //记录无用设备，在设备连接上之后删除，在设备未连接时给每个设备3次重连机会
    map<QString, int> m_mapMarkingDev;

    //记录当前犯规串口名称
    QString m_qsComName{""};
};

#endif // CDevFoulControl_H
