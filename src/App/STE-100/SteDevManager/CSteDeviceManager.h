///////////////////////////////////////////////////////////
//  CSteDeviceManager.h
//  Implementation of the Class CSteDeviceManager
//  Created on:      08-1月-2022 11:19:50
//  Original author: 陈磊
///////////////////////////////////////////////////////////
#if !defined(EA_FCEA602E_7160_4339_97AE_62D0FAD58CC5_INCLUDED_)
#define EA_FCEA602E_7160_4339_97AE_62D0FAD58CC5_INCLUDED_

#include "CDevSteFactory.h"
#include "CThreadResourceCtrlBase.h"
#include "CSerialPortCom.h"

#include "../Common/CSingletonPattern.h"

#include <string>
#include <QThread>
#include <QTimer>
#include <set>
using namespace std;

/*************************************************
  <类    名>    CSteDeviceManager
  <功能描述 >    该管理类主要去实现从通讯接口的连接与否去管理
                体测设备的连接状态，即管理体测设备工厂对象。
                工厂对象为有独立数据通讯和数据解析过程的设备，
                在工厂类中根据数据包构建具体体测设备。
  <作    者>    zhy
  <时    间>    2022-02-17
  <补    充>   当前体测设备在存在电源时根据电源连接情况进行，当一直没有电源时逻辑正常运转当
               存在一次电源断连之后，需要电源连接上之后才正常进行逻辑
  </补    充>
**************************************************/

class CSteDeviceManager : public QObject, public CSingletonPtr<CSteDeviceManager>
{
    Q_OBJECT

    //声明为单例模式
    DECLARE_SINGLETON_CLASS(CSteDeviceManager)

public:
    CSteDeviceManager();
    virtual ~CSteDeviceManager();

public:
    //获取连接的全部体测设备
    map<int, CDevHardwareBase *> getAllDev();

    //根据设备设备ID获取设备
    CDevHardwareBase *getDevSteByID(unsigned int iDevID);

    //设置信道信息，最大连接数和信道号
    bool setChannelInfo(unsigned int nMaxConnect, unsigned int nChannelNo);

    //获取当前信道号
    unsigned int getCurDevChannel();

    //获取当前最大连接数量
    unsigned int getCurDevMaxConnectNum();

    //判定是否为工作站模式（一对多）
    bool isStaionSte();
    short getType();

    void startActive();
    void stopActive();

    //获取设备状态(未测试，测试中，测试完成)
    inline short getTestStatus(){return m_objTestStateManager.getTestState();}

    inline CTestStateManage &getTestStateMgr(){return m_objTestStateManager;}

    //添加电源连接状态是因为电源断电之后异常操作容易造成串口不正常行为
    void setPowerConnectState(bool bConnect);

    //获取设备的分机编号
    QVariantMap getDevNumbers();
private:
    //枚举体测设备工厂，判断当前通讯单元是否为合法的体测设备
    void enumDevSteFactory();

    //创建体测站点
    void createDevSteFactory(const QString &strComName);

    //删除所有非直连设备
    void delAllStation();

    //
    void timeOutDelInvalidDev();

    CDevSteFactory* getFactoryByName(const QString &qsName);

    //延时设置设备
    void delaySetDev();
signals:
    //第一次连接体测设备或者无线切换到有线会触发
    void signalFactoryConnectChange();

    //具体体测设备连接状态变更
    void signalSteDevConnectChange(bool bConnect, void *pDev);
protected slots:

    //体测设备通讯接口检测定时处理
    void onMonitorTimeOut();

    //CDevSteFactory连接变更响应
    void onFactoryConnectChange(bool bStatus, void *pDev);

    //设备发送错误
    void onSteDevError(unsigned int iDevError, QString strComName);

    //实际过程中发现存在函数重入之后逻辑混乱造成的软件闪退问题，是因为其中的关闭函数存在延时
    void deleteMappingByName(const QString &strComName, bool bDeleteMapping);

    void setFactory(CDevSteFactory *pDev);

    void onTestStateChange();

    //接收设备连接状态主要是为了管理整体的测试状态
    void onSteDevConnectChange(bool bConnect, void *pDev);
private:
    //查找体测工作站和设备监测定时器
    QTimer m_timerMonitor;

    //一个平板只对应于一个体测设备工厂,指针指向 CDevSteFactory中的设备
    QString m_qsCurFactory{""};

    //临时创建的体测设备工厂，因为会有个多通讯接口，需要从其中找到合法的接口
    map<QString,CDevSteFactory *> m_mapTempFactory;

    //整合底层的设备状态，仅存在 未测试 测试中 测试完成 3种状态
    CTestStateManage m_objTestStateManager;

    bool m_bSetChannelInfo{false};
    long long m_llSetChannelTimeStamp;

    //第一次连接的设备若是组网设备，同时当前设备中存在多个串口，那么最多等待800ms再进行连接
    bool m_bFirstConnect{true};

    //延时设置设备
    bool m_bDelaySetting{false};
    QString m_qsDelayComName;
    long long m_llDelayDev;

    //存储即将delete的工厂设备指针,关闭设备之后立即删除指针，此时设备内部定时器可能在运行
    //删除之后导致软件闪退，在定时器中判断是否可以删除，再进行指针删除
    set<CDevSteFactory *> m_setDeletePointer;

    //判断第一次的设备是否都已经进行了连接
    bool m_bFirstEnumCom{true};
    set<QString> m_setFirstEnumCom;

    //表明电源连接状态
    bool m_bPowerConnectState{true};
};
#endif // !defined(EA_FCEA602E_7160_4339_97AE_62D0FAD58CC5_INCLUDED_)
