///////////////////////////////////////////////////////////
//  CAssistDevManage.h
//  Implementation of the Class CAssistDevManage
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_9B48A990_1F7E_4f87_8AC8_DA51D271B7C7_INCLUDED_)
#define EA_9B48A990_1F7E_4f87_8AC8_DA51D271B7C7_INCLUDED_
#include "commondefin.h"
#include "PowerUart/CDevPowerUart.h"
#include "RunNFC/CDevRunNFC.h"
#include "CConfirmDevRun.h"
#include "CDataParseRunNFC.h"
#include "CThreadResourceCtrlBase.h"
#include "CIdentificDevNFC.h"
#include "CDevCamera.h"
#include "CCamerInfoForTest.h"
#include "CDevStartingGun.h"
#include "CInterAction.h"
#include "FoulControl/CDevFoulControl.h"

#include <string>
#include <QVector>

using namespace std;
/**
 * 按照当前软件模式体测设备同时只存在一个，若有多个体测设备，那么只进行实例化一个虚拟设备对象
 */

//软件模式变更，通过一个串口可以实例化多个虚拟体测设备，

class CAssistDevManage : public CThreadResourceCtrlBase
{
    Q_OBJECT

public:
    CAssistDevManage();
    ~CAssistDevManage() override;

    //执行完成之后若当前电源设备为nullptr则表示设备未连接
    void checkingDevPowerConnect();

    //若设备连接之后是长跑设备，那么需要调用函数检测跑步刷卡设备
    void checkingDevRunDevNfcConnect();

    //开始活动(初次运行需要调用函数进行)
    void startActive();

    //停止活动(软件结束需要进行调用)
    void stopActive();

    //找到跑步NFC刷卡设备
    void enumSerialPortToFindRunNfcDev();

    //找到发令枪设备
    void enumSerialPortToFindStartingGunDev();

    //配置摄像机
    void configCamer();

    //获取电源设备(当前设备是固定的电源设备对象，返回指针会一直有效，若不操作指针对象
    //可以判断设备有效无效
    static CDevPowerUart* getDevPowerUart();

    //获取跑步刷卡设备
    CDevRunNFC *getDevRunNfc();

    CIdentificDevNFC *getIdentifyDev();

    // 获取身份识别摄像头
    CDevCamera& getIdentifyCamera();

    //获取防作弊摄像头,再具体的设备测试指导流程中使用
    static CDevCamera &getAntiCheatingCamera();

    //
    static CDevCamera *getCamer(const long &camerType);

    //查找跑步NFC设备
    void findRunNfcDev();

    //查找发令枪设备
    void findStartingGunDev();
signals:
    //电源设备状态改变
    void signalDevPowerConnectChange();

    //跑步刷卡设备状态改变
    void signalDevRunNfcConnectChange();

    //发令枪设备状态变更
    void signalDevStartingGunConnectChange(bool bConnect);

    //更新灯模式
    void signalLightMode(unsigned short shMode);

    //创建跑步NFC识别设备
    void signalCreatRunNFCDev(QString comName);

    //创建发令枪设备
    void signalCreatStartingGun(QString comName);

    //发令枪发令开始
    void signalStartingGun();

    void signalIdentifyFindCard(const string &strCardID);

    void signalDevIdentifyConnectChange(bool bConnect);

    //犯规设备触发犯规
    void signalFoulType(unsigned short shFoulType);
public slots:
    //创建跑步NFC设备
    void onCreateDevRunNFC(QString comName);

    //创建发令枪设备，需要在NFC设备创建成功之后再创建
    void onCreateDevStartingGun(QString comName);

    //电源管理设备连接状态变更
    void onDevPowerConnecChange(void *pDev, bool bConnect);

    //跑步NFC设备连接状态变更
    void onDevRunNFCConnectChange(void *pDev, bool bConnect);

    //发令枪设备连接状态变更
    void onDevStartingGunCOnnectChange(void *pDev, bool bConnect);

    //犯规控制设备连接状态变更
    void onDevFoulControlConnectChange(void *pDev, bool bConnect);

    void onTimeOut();

    //设备类型变更
    void onSteDevTypeChange(short shDevType, bool bDirectConnect);
protected:

    //在线程中进行设备的查找
    virtual void run() override;

    virtual void dealThreadLogic() override;
private:
    //设备确认类，需要在线程中创建出来，保证每次在同一线程中
    CConfirmDevRun               *m_pConfirmDev{nullptr};
    CDataParseRunNFC             m_objParseRunNFC;
    CSerialPortCom               *m_pSerialPortCom{nullptr};

    //电源管理设备对象(内部自己管理连接重连,使用指针是为了更好的进行线程创建)
    static CDevPowerUart         *s_pDevPower;
    QThread                      *m_pThreadPower{nullptr};

    //跑步NFc刷卡对象（不是每个设备都有，需要外部在外部控制识别重连）
    CDevRunNFC                   *m_pDevRunNFC{nullptr};

    //发令枪设备
    CDevStartingGun              *m_pDevStartingGun{nullptr};

    //犯规控制设备，在测试过程中主动控制是否存在有犯规（部分设备控制）
    CDevFoulControl              *m_pDevFoulControl{nullptr};

    //身份识别nfc对象(内部自己管理连接重连)
    CIdentificDevNFC             *m_pDevIdentifyNfc{nullptr};
    QThread                      *m_pThreadIdentifyNFC{nullptr};
    QSemaphore                   m_semaphoreFindNfc;

    /**
     * @brief 是否重新轮询标志位
     */
    bool                          m_bRestart{false};

    //查找设备定时器
    QTimer                        m_timer;

    //定义查询设备变量 电源设备 跑步NFC刷卡设备
    std::atomic<bool>             m_bFindDevRunNfc{false};

    //
    std::atomic<bool>             m_bFindDevStartingGun{false};

    //自检状态
    std::atomic<bool>             m_bSelfCheckState{false};

    //记录当前设备类型
    std::atomic<short>            m_shCurDevType{DEV_TYPE_UNKNOW};
    //记录连接方式是否是直连
    std::atomic<bool>             m_bDirectConnect{false};

    //查找跑步刷卡设备时间间隔 ms
    int                           m_nFindRunNFCTotal{0};
    int                           m_nFindRunNFCInterval{1000};

    //查找发令枪设备时间间隔
    int                           m_nFindStartingGunTotal{0};
    int                           m_nFindStartingGunInterval{1000};

    //检测相机时间间隔 ms
    int                           m_nCheckCamerTotal{0};
    int                           m_nCheckCamerInterval{1500};

    //定时器时间间隔
    int                           m_nTimerInterval{200};

    //整个实验中用到的摄像头
    static CDevCamera             s_camerDefault; //默认摄像头 可用于身份识别和防作弊
    static CDevCamera             s_camerExternal; //仅用于防作弊判断
    CCamerInfoForTest             m_camerinfoCfg;
    QSemaphore                    m_semaphoreCheckCamer;
    std::atomic<long>             m_lCamerErrCode{camerType::camer_Photograph | camerType::camer_Recognition};

    //拍照摄像机是否初始化使用（需要先打开，再暂停），在进行防作弊拍照时摄像头开启耗时较长，而恢复很快
    //这里正确配置摄像头之后要进行开启然后暂停操作，变量为 true表示已经进行过配置
    bool                          m_bCamerExternalInit{false};


    CInterAction &m_ui{CInterAction::Get_Instance()};
};
#endif // !defined(EA_9B48A990_1F7E_4f87_8AC8_DA51D271B7C7__INCLUDED_)
