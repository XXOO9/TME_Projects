#include <QDebug>
#include <QString>
#include <QCoreApplication>
#include <QEventLoop>
#include "CAssistDevManage.h"
#include "ProductRegistrar.h"
#include "CLogger.h"
#include "CAssistDevManage.h"
#include "PowerUart/PowerUartApplication.h"
#include "CEnumSerialPort.h"
#include "CDatabaseManage.h"

CDevCamera CAssistDevManage::s_camerDefault;
CDevCamera CAssistDevManage::s_camerExternal;
CDevPowerUart *CAssistDevManage::s_pDevPower{nullptr};

CAssistDevManage::CAssistDevManage()
{
    qRegisterMetaType<string>("string");
    //电源管理系统内部结构需要进行初始化才能进行使用
    PowerUartApplicationInit();

    connect(&m_timer, &QTimer::timeout, this, &CAssistDevManage::onTimeOut);
    connect(this, &CAssistDevManage::signalCreatRunNFCDev, this, &CAssistDevManage::onCreateDevRunNFC, Qt::BlockingQueuedConnection);
    connect(this, &CAssistDevManage::signalCreatStartingGun, this, &CAssistDevManage::onCreateDevStartingGun, Qt::BlockingQueuedConnection);
}

CAssistDevManage::~CAssistDevManage()
{
    safeDeleteClassPoint(&s_pDevPower);
    safeDeleteClassPoint(&m_pDevRunNFC);
    safeDeleteClassPoint(&m_pDevIdentifyNfc);

    //关闭定时器
    m_timer.stop();

    stopActive();
}

void CAssistDevManage::startActive()
{
    //电源管理,变更为在线程中运行
    safeNewClassPoint(&s_pDevPower);
    if(s_pDevPower != nullptr)
    {
        m_pThreadPower = new QThread();
        connect(m_pThreadPower, &QThread::finished, m_pThreadPower, &QObject::deleteLater);
        connect(m_pThreadPower, &QThread::finished, s_pDevPower, &QObject::deleteLater);

        s_pDevPower->moveToThread(m_pThreadPower);
        m_pThreadPower->start();

        connect(s_pDevPower, &CDevPowerUart::signalConnectChange, this, &CAssistDevManage::onDevPowerConnecChange, Qt::QueuedConnection);
        s_pDevPower->setWorkState(true);
    }

    //身份识别nfc,在线程中进行识别
    m_pDevIdentifyNfc = new CIdentificDevNFC();
    m_pThreadIdentifyNFC = new QThread();
    connect(m_pThreadIdentifyNFC, &QThread::finished, m_pThreadIdentifyNFC, &QObject::deleteLater);
    connect(m_pThreadIdentifyNFC, &QThread::finished, m_pDevIdentifyNfc, &QObject::deleteLater);

    connect(m_pDevIdentifyNfc, &CIdentificDevNFC::signalConnectChange, this, &CAssistDevManage::signalDevIdentifyConnectChange);

    m_pDevIdentifyNfc->moveToThread(m_pThreadIdentifyNFC);
    m_pThreadIdentifyNFC->start();
    m_pDevIdentifyNfc->open();

    //开启线程查找其他设备
    startThread();

    m_timer.start(m_nTimerInterval);

    safeNewClassPoint(&m_pDevFoulControl);
    connect(m_pDevFoulControl, &CDevFoulControl::signalConnectChange, this, &CAssistDevManage::onDevFoulControlConnectChange);
}

void CAssistDevManage::stopActive()
{
    //电源设备
    if(s_pDevPower != nullptr)
    {
        disconnect(s_pDevPower, &CDevPowerUart::signalConnectChange, this, &CAssistDevManage::onDevPowerConnecChange);
        s_pDevPower->setWorkState(false);

        if(m_pThreadPower != nullptr)
        {
            m_pThreadPower->quit();
        }

        //safeDeleteClassPoint(&s_pDevPower);
    }

    //身份识别
    if(m_pDevIdentifyNfc != nullptr && m_pThreadIdentifyNFC != nullptr)
    {
        m_pDevIdentifyNfc->close();
        m_pThreadIdentifyNFC->quit();
    }

    //跑步刷卡设备
    m_bFindDevRunNfc = false;
    if(m_pDevRunNFC != nullptr)
    {
        disconnect(m_pDevRunNFC, &CDevRunNFC::signalConnectChange, this, &CAssistDevManage::onDevRunNFCConnectChange);
        m_pDevRunNFC->devClose();
        delete m_pDevRunNFC;
        m_pDevRunNFC = nullptr;
    }

    //发令抢设备
    m_bFindDevStartingGun = false;
    if(m_pDevStartingGun != nullptr)
    {
        disconnect(m_pDevStartingGun, &CDevStartingGun::signalConnectChange, this, &CAssistDevManage::onDevStartingGunCOnnectChange);
        m_pDevStartingGun->devClose();
        delete m_pDevStartingGun;
        m_pDevStartingGun = nullptr;
    }

    //犯规控制设备
    if(m_pDevFoulControl != nullptr)
    {
        safeNewClassPoint(&m_pDevFoulControl);
    }


    //关闭所有摄像机
    s_camerDefault.closeCamera();
    s_camerExternal.closeCamera();

    m_timer.stop();

    //等待线程运行完成
    stopThread();
}

void CAssistDevManage::enumSerialPortToFindRunNfcDev()
{
    if(!m_semaphoreFindNfc.tryAcquire())
    {
        return;
    }

    if(!m_bFindDevRunNfc)
        return;

    if(m_pDevRunNFC != nullptr)
    {
        if(m_pDevRunNFC->isValid())
        {
            m_bFindDevRunNfc = false;
            return;
        }
    }

    CEnumSerialPort &enumSerialPort = CEnumSerialPort::Get_Instance();
    //串口正在被其他类使用，下一次再使用
    if(!enumSerialPort.lockUseSerialPortCom())
       return;

    const QStringList &comNameList = enumSerialPort.getAvailableSerialPortCom();

    if(m_pConfirmDev == nullptr)
        return;

    //设置设备信息(电源设备自动上传不需要确定返回命令)
    m_pConfirmDev->setDataParse(&m_objParseRunNFC);
    m_pConfirmDev->setCommunicate(m_pSerialPortCom);

    //握手命令使用 查询设备命令
    QByteArray arrCmd(CDevRunNFC::getCmdDevInfo());

    bool bFindDev = false;
    for(auto &comName : comNameList)
    {
        bFindDev = false;

        m_objParseRunNFC.setBoundRate(QSerialPort::Baud38400);
        CDevRunNFC::cfgComInfo(m_pSerialPortCom, comName);

        m_pConfirmDev->clearData();

        //根据调试发现 120ms作为等待时间比较合适
        if(m_pConfirmDev->devOpen())
        {
            bFindDev = m_pConfirmDev->confirmDev(arrCmd, 200);
        }
        m_pConfirmDev->devClose();

        if(bFindDev)
        {
            m_bFindDevRunNfc = false;
            emit signalCreatRunNFCDev(comName);
            break;
        }
    }

    enumSerialPort.unlockUseSerialPortCom();
}

void CAssistDevManage::enumSerialPortToFindStartingGunDev()
{
    if(!m_bFindDevStartingGun)
        return;

    if(m_pDevStartingGun != nullptr)
    {
        if(m_pDevStartingGun->isValid())
        {
            m_bFindDevStartingGun = false;
            return;
        }
    }

    CEnumSerialPort &enumSerialPort = CEnumSerialPort::Get_Instance();
    //串口正在被其他类使用，下一次再使用
    if(!enumSerialPort.lockUseSerialPortCom())
       return;

    //获取未被占用的串口名称
    const QStringList &comNameList = enumSerialPort.getAvailableSerialPortCom();

    //若当前存在的串口存在发令枪设备的串口号
    QString qsStartingGunComName = CDatabaseManage::GetInstance().getTestRunConfig().getStartingGunComName();

    if(comNameList.contains(qsStartingGunComName))
    {
        m_bFindDevRunNfc = false;
        emit signalCreatStartingGun(qsStartingGunComName);
    }

    enumSerialPort.unlockUseSerialPortCom();
}

void CAssistDevManage::configCamer()
{
    if(!m_semaphoreCheckCamer.tryAcquire())
        return;

    if(m_shCurDevType == DEV_TYPE_UNKNOW)
    {
        m_lCamerErrCode = camerType::camer_Photograph | camerType::camer_Recognition;
        return;
    }

//    //根据当前设备类型和设备连接状态进行设备连接检查
    m_camerinfoCfg.checkCamerInfo(m_shCurDevType, m_bDirectConnect);
    const long &lErrCode = m_camerinfoCfg.getErrCode();

//    //相机出现某些错误,需要在界面上进行变更
    vector<long> vecErrCode = m_camerinfoCfg.getErrCamer(lErrCode);

//    //当前存在错误
    bool bExistErrRecog = std::find(vecErrCode.begin(), vecErrCode.end(), camerType::camer_Recognition) != vecErrCode.end(); //识别
    bool bExistErrPhoto = std::find(vecErrCode.begin(), vecErrCode.end(), camerType::camer_Photograph) != vecErrCode.end();  //过程相机

//    //记录当前相机有没有问题，若状态变更，那么发送状态到界面(默认初始存在问题)
    static bool s_bExistErrRecog = false, s_bExistErrPhoto = false;

    //在界面上显示摄像头断连
    if(s_bExistErrRecog != bExistErrRecog)
    {
        m_ui.setViewErorrString(MERR_CAMERA_RECOGNITION, bExistErrRecog);
        s_bExistErrRecog = bExistErrRecog;
    }

    if(bExistErrPhoto != s_bExistErrPhoto)
    {
        m_ui.setViewErorrString(MERR_CAMERA_ANTICHEATING, bExistErrPhoto);
        s_bExistErrPhoto = bExistErrPhoto;
    }

    //记录正常的相机到容器中
    static vector<CDevCamera *> vecNormalCamer;
    vecNormalCamer.clear();

    if(!bExistErrRecog)
    {
        vecNormalCamer.push_back(getCamer(camerType::camer_Recognition));
    }

    if(!bExistErrPhoto)
    {
        vecNormalCamer.push_back(getCamer(camerType::camer_Photograph));
    }

    CDevCamera *pCamera;
    //对摄像机进行配置
    if(lErrCode != m_lCamerErrCode)
    {
        LOGWARN("camera change lErrCode" + to_string(lErrCode));

        for(auto &camerType : vecErrCode)
        {
            pCamera = getCamer(camerType);
            if(pCamera == nullptr)
            {
                continue;
            }

            //摄像头出问题之后重置，同时若是外部摄像头，需要将外部摄像头初始化标志设置为false
            pCamera->resetCamer();
            if(camerType == camerType::camer_Photograph)
            {
                m_bCamerExternalInit = false;
            }
        }

        //配置摄像机
        m_camerinfoCfg.cfgCamer(&s_camerDefault, &s_camerExternal);

        m_lCamerErrCode = lErrCode;

        //若两个相机配置不一致，需要先开启外部过程照片摄像头，避免后面开启摄像头耗时过长
        if(!m_bCamerExternalInit && s_camerExternal.getCamerIndex() >= 0
                && s_camerDefault.getCamerIndex() != s_camerExternal.getCamerIndex())
        {
            s_camerExternal.openCamer();
            s_camerExternal.pause();
            m_bCamerExternalInit = true;
        }
    }

    for(auto &element : vecNormalCamer)
    {
        if(!element->getLastSetState())
        {
            continue;
        }

        if(element->getCamerIndex() < 0)
        {
            continue;
        }

        //相机工作正常，不做任何操作
        if(element->isOpenExternal() && element->isOpenInterior())
        {
            continue;
        }

        element->openCamer();
    }
}

CDevPowerUart* CAssistDevManage::getDevPowerUart()
{
    CDevPowerUart *p = nullptr;
    if(s_pDevPower != nullptr && s_pDevPower->isValid())
    {
        p = s_pDevPower;
    }

    return p;
}

CDevRunNFC *CAssistDevManage::getDevRunNfc()
{
    return m_pDevRunNFC;
}

CIdentificDevNFC *CAssistDevManage::getIdentifyDev()
{
    return m_pDevIdentifyNfc;
}

CDevCamera& CAssistDevManage::getIdentifyCamera()
{
    return s_camerDefault;
}

CDevCamera &CAssistDevManage::getAntiCheatingCamera()
{
    if(s_camerDefault.getCamerIndex() == s_camerExternal.getCamerIndex())
    {
        return s_camerDefault;
    }

    return s_camerExternal;
}

CDevCamera *CAssistDevManage::getCamer(const long &camerType)
{
    CDevCamera *pCamer = nullptr;
    switch (camerType) {
    case camerType::camer_Photograph:
        pCamer = &s_camerExternal;
        break;
    case camerType::camer_Recognition:
        pCamer = &s_camerDefault;
        break;
    default:
        break;
    }

    return pCamer;
}

void CAssistDevManage::findRunNfcDev()
{
    if(m_pDevRunNFC == nullptr || !m_pDevRunNFC->isValid())
    {
        //界面设置跑步设备
        m_ui.setViewErorrString(MERR_RUN_MID_NFC, true);
        m_bFindDevRunNfc = true;
    }
}

void CAssistDevManage::findStartingGunDev()
{
    if(m_pDevStartingGun == nullptr || !m_pDevStartingGun->isValid())
    {
        //界面设置跑步设备
        m_ui.setViewErorrString(MINFO_STARTING_GUN, true);
        m_bFindDevStartingGun = true;
    }
}

void CAssistDevManage::onCreateDevRunNFC(QString comName)
{
    LOGINFO("create run NFC dev");
    m_bFindDevRunNfc = false;

    //若设备处于断连状态，将设备删除
    if(m_pDevRunNFC != nullptr)
    {
        if(m_pDevRunNFC->isValid())
        {
            return;
        }
        else
        {
            m_pDevRunNFC->devReset();
        }
    }

    if(m_pDevRunNFC == nullptr)
    {
        m_pDevRunNFC = new CDevRunNFC;
        connect(m_pDevRunNFC, &CDevRunNFC::signalConnectChange, this, &CAssistDevManage::onDevRunNFCConnectChange);
    }

    if(m_pDevRunNFC != nullptr)
    {
        //刷卡设备和确认设备类使用同一套解析协议
        m_pDevRunNFC->setDataParse(&m_objParseRunNFC);
        m_pDevRunNFC->setComName(comName);
    }
}

void CAssistDevManage::onCreateDevStartingGun(QString comName)
{
    LOGINFO("create starting gun dev");
    m_bFindDevStartingGun = false;

    //若设备处于断连状态，将设备删除
    if(m_pDevStartingGun != nullptr)
    {
        if(m_pDevStartingGun->isValid())
        {
            return;
        }
        else
        {
            m_pDevStartingGun->devReset();
        }
    }

    if(m_pDevStartingGun == nullptr)
    {
        m_pDevStartingGun = new CDevStartingGun;
        connect(m_pDevStartingGun, &CDevStartingGun::signalConnectChange, this, &CAssistDevManage::onDevStartingGunCOnnectChange);
    }

    if(m_pDevStartingGun != nullptr)
    {
        m_pDevStartingGun->setComName(comName);
    }
}

void CAssistDevManage::onDevPowerConnecChange(void *pDev, bool bConnect)
{
    if(pDev == nullptr)
        return;

    //电源设备连接
    string strDevName("Power Dev");
    if(bConnect)
    {
        strDevName += "  connect";
    }
    else
    {
        strDevName += "  disconnect";
    }

    emit signalDevPowerConnectChange();

    LOGINFO(strDevName);
}

void CAssistDevManage::onDevRunNFCConnectChange(void *pDev, bool bConnect)
{
    if(pDev == nullptr)
        return;

    if(pDev != m_pDevRunNFC || m_pDevRunNFC == nullptr)
    {
        LOGERROR("run nfc dev err");
        return;
    }

    //跑步刷卡设备连接
    string strDevName("Run nfc Dev");
    if(bConnect)
    {
        strDevName += "  connect";

        //发令枪设备在识别设备连接之后
        if(CDevDataCfg::isRunTimeDev(m_shCurDevType))
        {
            findStartingGunDev();
        }
    }
    else
    {
        m_pDevRunNFC->devClose();
        strDevName += "  disconnect";

        //若是中长跑设备那么重连
        if(m_shCurDevType == DEV_TYPE_RUN_MID)
        {
            findRunNfcDev();
        }
    }

    emit signalDevRunNfcConnectChange();

    LOGINFO(strDevName);
}

void CAssistDevManage::onDevStartingGunCOnnectChange(void *pDev, bool bConnect)
{
    if(pDev == nullptr || pDev != m_pDevStartingGun)
        return;

    if(bConnect)
    {
        m_ui.setViewErorrString(MINFO_STARTING_GUN, false);

        //连接开始发令信号
        connect(m_pDevStartingGun, &CDevStartingGun::signalStartingGun, this, &CAssistDevManage::signalStartingGun, Qt::DirectConnection);
    }
    else
    {
        if(m_pDevStartingGun != nullptr)
        {
            m_pDevStartingGun->devClose();
        }

        disconnect(m_pDevStartingGun, &CDevStartingGun::signalStartingGun, this, &CAssistDevManage::signalStartingGun);
        //disconnect(m_pDevStartingGun, &CDevStartingGun::signalConnectChange, this, &CAssistDevManage::onDevStartingGunCOnnectChange);

        //若是中长跑等设备那么重连发令枪
        if(CDevDataCfg::isSpecialDev(m_shCurDevType))
        {
            findStartingGunDev();
        }
    }

    emit signalDevStartingGunConnectChange(bConnect);
}

void CAssistDevManage::onDevFoulControlConnectChange(void *pDev, bool bConnect)
{
    if(pDev == nullptr || pDev != m_pDevFoulControl)
    {
        return;
    }

    if(bConnect)
    {
        m_ui.setViewErorrString(MINFO_FOUL_CONTROL, false);
        //连接犯规信号
        connect(m_pDevFoulControl, &CDevFoulControl::sigSendFoulType, this, &CAssistDevManage::signalFoulType);
    }
    else
    {
        if(CDevDataCfg::isNeedViolationInfoDev(m_shCurDevType))
        {
            m_ui.setViewErorrString(MINFO_FOUL_CONTROL, true);
        }

        if(m_pDevStartingGun != nullptr)
        {
            m_pDevStartingGun->devClose();
        }

        disconnect(m_pDevFoulControl, &CDevFoulControl::sigSendFoulType, this, &CAssistDevManage::signalFoulType);
    }
}

void CAssistDevManage::onTimeOut()
{
    bool bRunThread = false;
    if(m_bFindDevRunNfc)
    {
        m_nFindRunNFCTotal += m_nTimerInterval;

        //满足条件添加线程运行资源
        if(m_nFindRunNFCTotal >= m_nFindRunNFCInterval)
        {
            if(!m_bProcessState)
            {
                bRunThread = true;
                m_semaphoreFindNfc.release();
                m_nFindRunNFCTotal -= m_nFindRunNFCInterval;
            }
        }
    }

    //发令枪设备需要在
    if(m_bFindDevStartingGun)
    {
        m_nFindStartingGunTotal += m_nTimerInterval;

        //满足条件添加线程运行资源
        if(m_nFindStartingGunTotal >= m_nFindStartingGunInterval)
        {
            if(!m_bProcessState)
            {
                bRunThread = true;
                m_nFindStartingGunTotal -= m_nFindStartingGunInterval;
            }
        }
    }

    //摄像机需要一直进行检测
    m_nCheckCamerTotal += m_nTimerInterval;
    if(m_nCheckCamerTotal >= m_nCheckCamerInterval)
    {
        if(!m_bProcessState)
        {
            bRunThread = true;
            m_semaphoreCheckCamer.release();
            m_nCheckCamerTotal -= m_nCheckCamerInterval;
        }
    }

    if(bRunThread)
    {
        m_semaphoreThread.release();
    }
}

void CAssistDevManage::onSteDevTypeChange(short shDevType, bool bDirectConnect)
{

    m_lCamerErrCode = camerType::camer_Photograph | camerType::camer_Recognition;

    m_shCurDevType = shDevType;
    m_bDirectConnect = bDirectConnect;

    //发令枪设备需要在中长跑识别设备连接之后再进行连接
    if(CDevDataCfg::isRunTimeDev(m_shCurDevType))
    {
        if(m_shCurDevType != DEV_TYPE_RUN_MID)
        {
            findStartingGunDev();
        }
    }
    else
    {
        //界面设置跑步设备
        m_ui.setViewErorrString(MINFO_STARTING_GUN, false);

        if(m_pDevStartingGun != nullptr)
        {
            m_pDevStartingGun->devClose();
        }
    }

    //中长跑刷卡设备
    if(m_shCurDevType == DEV_TYPE_RUN_MID)
    {
        findRunNfcDev();
    }
    else
    {
        //界面设置跑步设备
        m_ui.setViewErorrString(MERR_RUN_MID_NFC, false);

        if(m_pDevRunNFC != nullptr)
        {
            m_pDevRunNFC->devClose();
        }
    }

    if(CDevDataCfg::isNeedViolationInfoDev(m_shCurDevType))
    {
        //界面设置犯规设备
        m_ui.setViewErorrString(MINFO_FOUL_CONTROL, true);

        m_pDevFoulControl->setWorkState(true);
    }
    else
    {
        m_ui.setViewErorrString(MINFO_FOUL_CONTROL, false);
    }
}

void CAssistDevManage::run()
{
    safeNewClassPoint(&m_pConfirmDev);
    safeNewClassPoint(&m_pSerialPortCom);

    CThreadResourceCtrlBase::run();

    //线程结束删除枚举的设备变量
    safeDeleteClassPoint(&m_pConfirmDev);
    safeDeleteClassPoint(&m_pSerialPortCom);
}

void CAssistDevManage::dealThreadLogic()
{
    //查找中长跑设备的刷卡设备
    enumSerialPortToFindRunNfcDev();

    //查找发令枪设备
    enumSerialPortToFindStartingGunDev();

    configCamer();
}
