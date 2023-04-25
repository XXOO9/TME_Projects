#if !defined(EA_ABC78D71_62DB_49e0_967C_F2736542792B_INCLUDED_)
#define EA_ABC78D71_62DB_49e0_967C_F2736542792B_INCLUDED_

#include <map>
#include "CInterAction.h"
#include "SteDevManager/SteDevice/CDevHardwareBase.h"

using namespace std;
/*************************************************
  <类    名>    CSteStation
  <功能描述>     体测设备工厂类，主要对接收串口发送上来的数据
                进行解析，然后构建出对应的虚拟体测设备，需要对数
                据进行固定时间间隔的读取解析

  <作    者>    zhy
  <时    间>    2022-02-18
**************************************************/
class CDevSteFactory : public CDevSteBase
{
    Q_OBJECT

public:
    //根据通讯单元名称和序号构建工厂对象
    /**
     * 构建一个设备对象
     * @param intDevNO 逻辑设备号
     * @param strUnitName 通讯单元名称，比如COM1、COM3且应该唯一
     * @param helper 通讯单元助手
     */
    CDevSteFactory(short shIndex, QString strUnitName);
    virtual ~CDevSteFactory() override;

    //获取全部设备，反回的first值为逻辑设备号
    map<int, CDevHardwareBase *> getAllDev();

    //根据设备设备ID获取设备
    CDevHardwareBase *getDevSteByID(unsigned int iDevID);

    //获取组网最大连接数
    inline const unsigned int &getNetworkMaxConnectDevNum(){return m_nConnectDevNum;}

    //获取组网信道号
    inline const unsigned int &getNetworkChannelNo() const {return m_nChannelNo;}

    //设置信道及连接数量
    bool setNetworkChannelInfo(unsigned int nMaxNum, unsigned int nChannelNo);

    virtual bool devOpen() override;
    virtual void devClose() override;

    //是否接收到数据
    bool getDataReceivedState(){return m_nReceiveData > 30;}

    //当前设备是否完成连接确认
    bool isConfirmConnectComplete(){return m_bConfirmConnect || m_bConfirmOperation;}
signals:
    void signalHardwareConnectChange(bool bConnect, void *pDev);
    
protected slots:
    void onSteDevConnectChange(bool bConnect, void *pDev);

    //中长跑检测途经点,设备变更和特定的设备信号变更会触发
    void onRunMidCheckViaPtDev();
protected:
    //判定新解析处理的设备ID是否已经在设备集合中
    bool isExistDev(unsigned int iDevID);

    //是否工厂资源
    void release();

    //回复基站心跳,参数为是否判断基站标志
    void replyStationHeartbeat(bool bJudgeStation = true);
    
    CDevHardwareBase *createSteDevcice(unsigned int iDevID, ICommunicate &comunicate, const short &shDevType);

    void delDev(void *pDev);

    void sendAck();
private:
    //该工作站连接的全部设备，第一个参数仅为序号和逻辑设备号无关
    map<int, CDevHardwareBase*> m_mapDevices;

    //检测是否是合法设备
    void timeoutCheckValidDev();

    //处理中长跑设备连接
    void dealRunMidDevConnectChange(bool bConnect, void *pDev);

    // CDevSteBase interface
protected:
    // IDataParse interface
    virtual void dataParse(QByteArray &array) override final;

    //解析数据帧头
    bool parseFrameHeader(unsigned char *pData, int iDatalen);

    bool parseDataPackage(unsigned char *pData, int iDatalen);
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

    //进行重载是因为在组网设备进行连接时需要定时恢复
    virtual void onUpdateTimeOut() override;
private:
    //是否忽略帧数据，一般是帧号重复才会
    bool m_bIgnoreCurFrameData{false};

    //获取信道信息
    bool m_bGetChannelInfo{false};
    //从命令中读取连接设备数
    unsigned int m_nConnectDevNum{0};

    //从命令中读取当前信道号
    unsigned int m_nChannelNo{0};

    //设备连接顺序，针对具体设备，从0开始递增
    int m_nOderOfConnection{0};

    //设备在确认期间收到的总字节数，若在确认期间收到的总字节数少于30,当前设备是否收到数据，若收到数据设备类型还是未知，那么设备就不是所需要的数据
    unsigned int m_nReceiveData{0};

    short m_shDevNO;

    //判断设备有没有进行确认具体设备类型
    bool m_bConfirmOperation{false};

    //设备有没有进行连接确认（外部需要使用），超时未确认或者确认了具体的设备
    bool m_bConfirmConnect{false};

    //是否发送确认帧，确认帧再收到反馈或者设备打开5s之后不再进行发送
    bool m_bSendDataReport{true};
    static QByteArray s_arrSetDataReport;
    qint64 nStartTimeSendDataReportTime;
    int m_nSendDataReportinterval{0};

};
#endif // !defined(EA_ABC78D71_62DB_49e0_967C_F2736542792B_INCLUDED_)
