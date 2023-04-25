///////////////////////////////////////////////////////////
//  CSteDeviceManager.cpp
//  Implementation of the Class CSteDeviceManager
//  Created on:      08-1月-2022 11:19:50
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSteDeviceManager.h"
#include "commondefin.h"
#include "CGenerationCommand.h"
#include "CLogger.h"
#include "CEnumSerialPort.h"
#include "CDevDataCfg.h"
#include "SteDevice/CDevSteRunMid.h"

CSteDeviceManager::CSteDeviceManager()
{
    connect(&m_timerMonitor, &QTimer::timeout, this, &CSteDeviceManager::onMonitorTimeOut);
}

CSteDeviceManager::~CSteDeviceManager()
{
    for (auto it = m_mapTempFactory.begin(); it != m_mapTempFactory.end(); it++)
    {
        if(it->second != nullptr)
        {
            delete it->second;
            it->second = nullptr;
        }
    }
}

/*************************************************
  <函数名称>    getAllDev()
  <功能描述>    获取全部设备
  <参数说明>

  <返 回 值>    无

  <函数说明>    在返回全部设备时，同时根据逻辑设备进行排序

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
map<int, CDevHardwareBase *> CSteDeviceManager::getAllDev()
{
    //删除new出的设备对象
    map<int, CDevHardwareBase *> map;

    auto *pFactory = getFactoryByName(m_qsCurFactory);
    if(pFactory != nullptr)
    {
        map = pFactory->getAllDev();
    }

    return map;
}

/*************************************************
  <函数名称>    getDevSteByID(unsigned int iDevID)
  <功能描述>    根据ID获取设备
  <参数说明>

  <返 回 值>    无

  <函数说明>

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
CDevHardwareBase *CSteDeviceManager::getDevSteByID(unsigned int iDevID)
{
    CDevHardwareBase *pDev = nullptr;

    auto pFactory = getFactoryByName(m_qsCurFactory);
    if(pFactory != nullptr)
    {
        pDev = pFactory->getDevSteByID(iDevID);
    }

    return pDev;
}

bool CSteDeviceManager::setChannelInfo(unsigned int nMaxConnect, unsigned int nChannelNo)
{
    //保存在当前列表中已经创建的需要进行删除的设备
    set<QString> setTemp;

    auto pFactory = getFactoryByName(m_qsCurFactory);
    if(pFactory != nullptr && pFactory->isStaionSte())
    {
        if(pFactory->setNetworkChannelInfo(nMaxConnect, nChannelNo))
        {
            setTemp.insert(pFactory->getComName());
        }
    }
    else
    {
        for(auto &element : m_mapTempFactory)
        {
            if(element.second == nullptr || !element.second->isValid() || !element.second->isStaionSte())
                continue;

            if(element.second->setNetworkChannelInfo(nMaxConnect, nChannelNo))
            {
                setTemp.insert(element.second->getComName());
            }
        }
    }

    if(setTemp.size() > 0)
    {
        for(auto &element : setTemp)
        {
            deleteMappingByName(element, true);
        }

        m_bSetChannelInfo = true;
        m_llSetChannelTimeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    }

    return setTemp.size() > 0;
}

unsigned int CSteDeviceManager::getCurDevChannel()
{
    auto pFactory = getFactoryByName(m_qsCurFactory);

    unsigned int channel = 0;

    if(pFactory != nullptr && isStaionSte())
    {
        channel = pFactory->getNetworkChannelNo();
    }
    return channel;
}

unsigned int CSteDeviceManager::getCurDevMaxConnectNum()
{
    auto pFactory = getFactoryByName(m_qsCurFactory);
    unsigned int num = 8;

    if(pFactory != nullptr && isStaionSte())
    {
        num = pFactory->getNetworkMaxConnectDevNum();
    }

    return num;
}

/*************************************************
  <函数名称>    isStaionSte()
  <功能描述>    判定设备模式
  <参数说明>

  <返 回 值>    无

  <函数说明>

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
bool CSteDeviceManager::isStaionSte()
{
    bool bStation = true;

    auto pFactory = getFactoryByName(m_qsCurFactory);
    if(pFactory != nullptr)
    {
        bStation = pFactory->isStaionSte();
    }

    return bStation;
}

short CSteDeviceManager::getType()
{
    short shDevType = DEV_TYPE_UNKNOW;
    auto pFactory = getFactoryByName(m_qsCurFactory);
    if(pFactory != nullptr)
    {
        shDevType = pFactory->getDevType();
    }

    return shDevType;
}

void CSteDeviceManager::startActive()
{
    if(!m_timerMonitor.isActive())
    {
        m_timerMonitor.start(1000);
    }
}

void CSteDeviceManager::stopActive()
{
    //停止查找设备定时器
    m_timerMonitor.stop();

    //断开所有设备连接
    for(auto &element : m_mapTempFactory)
    {
        if(element.second != nullptr)
        {
            element.second->devClose();
            delete element.second;
        }
    }

    m_mapTempFactory.clear();
}

void CSteDeviceManager::setPowerConnectState(bool bConnect)
{
    qDebug() << QThread::currentThreadId();
    m_bPowerConnectState = bConnect;

    //断连所有连接的设备置为无效即可,断开所有连接上的设备,将状态恢复为第一次连接
    if(!m_bPowerConnectState)
    {
        //延时设置设备
        m_bDelaySetting = false;
        m_qsDelayComName.clear();

        m_bSetChannelInfo = false;

        setFactory(nullptr);
        set<QString> setTemp;

        //使用中间变量是因为在遍历过程中 m_mapTempFactory可能被变更
        auto mapTemp = m_mapTempFactory;
        for(auto &element : mapTemp)
        {
            if(element.second != nullptr)
            {
                setTemp.insert(element.first);
            }
        }

        for(auto elemenet : setTemp)
        {
            deleteMappingByName(elemenet, true);
        }

        m_mapTempFactory.clear();
        m_bFirstConnect = true;
    }
}


QVariantMap CSteDeviceManager::getDevNumbers()
{
    QVariantMap retMap; //  <设备号, 设备信道号>
    CDevHardwareBase *tmpDevPtr = nullptr;

    auto devMap = getAllDev();

    //同一个板子上的信道号都是一样的
    for( auto &ele : devMap ){
        tmpDevPtr = ele.second;
        retMap[ QString::number( tmpDevPtr->getMacID() ) ] = QString::number( tmpDevPtr->getExtensionNo() );
    }

    return retMap;
}

/*************************************************
  <函数名称>    enumDevSteFactory
  <功能描述>    枚举体测设备工厂，判定是否是合法体测
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    在枚举过程中会对具体体测设备进行创建等操作

  <作    者>   zhy
  <时    间>   2022-02-17
  **************************************************/
void CSteDeviceManager::enumDevSteFactory()
{
    CEnumSerialPort &enumSerialPort = CEnumSerialPort::Get_Instance();

    //串口正在被其他类使用，下一次再使用
    if(!enumSerialPort.lockUseSerialPortCom(30))
       return;

    const QStringList &comNameList = enumSerialPort.getSerialPortCom();

    //分别表示现在 电脑存在的串口 和 软件中保存的串口信息
    set<QString> setExistCom, setTempCom;
    for(auto &element : comNameList)
    {
        setExistCom.insert(element);
    }

    for(auto &element : m_mapTempFactory)
    {
        setTempCom.insert(element.first);
    }

    //串口信息不一致才进行后续判断
    if(setExistCom != setTempCom)
    {
        bool bContinue = true;
        //当前设备未在存在的串口列表中,发送断连通知
        auto pFactory = getFactoryByName(m_qsCurFactory);
        if(setExistCom.find(m_qsCurFactory) == setExistCom.end())
        {
            if(pFactory != nullptr)
                onFactoryConnectChange(false, pFactory);
        }

        //删除保存的串口名称中不存在的串口
        for(auto &element : setTempCom)
        {
            if(setExistCom.find(element) == setExistCom.end())
            {
                deleteMappingByName(element, true);
            }
        }

        //需要持续检测串口，组网情况下和不存在连接设备才检测
        if(((pFactory != nullptr && isStaionSte()) || m_qsCurFactory.isEmpty()) && m_bPowerConnectState)
        {
            //设置信道之后，需要等待 3.5s等待硬件重启，要不会存在问题，也不能进行睡眠等待，会丢掉数据
            if(m_bSetChannelInfo)
            {
                if(QDateTime::currentDateTime().toMSecsSinceEpoch() >= m_llSetChannelTimeStamp + 3500)
                {
                    m_bSetChannelInfo = false;
                }
                else
                {
                    //时间未到不进行处理
                    bContinue = false;
                }
            }

            if(bContinue)
            {

                if(!(m_mapTempFactory.size() == 0 && m_setDeletePointer.size() != 0))
                {
                    long l1 = clock();
                    const QStringList &comNameList = enumSerialPort.getAvailableSerialPortCom();
                    for(auto &comName : comNameList)
                    {
                        //如果当前通讯接口为空闲，则进行设备工厂的试创建，对于非法的体测设备，会在设备错误处理函数中被释放
                        createDevSteFactory(comName);
                    }
                    long l2 = clock();
                    if(l2 - l1 > 500)
                    {
                        LOGERROR(/*"### comName :" + comName.toStdString() +*/ " createDevSteFactory time: " + to_string(l2 - l1));
                    }
                }
            }
        }
    }

    enumSerialPort.unlockUseSerialPortCom();
}

/*************************************************
  <函数名称>    createDevSteFactory(QString &strComName)
  <功能描述>    根据通讯接口名称去试创建设备工厂
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    这里是设备工厂的尝试创建，创建后会向设备发送一个设备类型查询命令，如果收到正确回复，则认为合法

  <作    者>   zhy
  <时    间>   2022-02-17
  **************************************************/
void CSteDeviceManager::createDevSteFactory(const QString &strComName)
{
    //如果已经在临时工厂集合中，则不进行创建
    auto it = m_mapTempFactory.find(strComName);
    if(it != m_mapTempFactory.end())
        return;

    //创建临时体测设备工厂
    qDebug() << "nnnnnnnn" << strComName;
    CDevSteFactory *pTemp = new CDevSteFactory(0, strComName);

    if(pTemp->devOpen())
    {
        //缓存起全部临时工厂
        m_mapTempFactory[strComName] = pTemp;

        //绑定设备出错处理
        connect(pTemp, &CDevSteFactory::signalDevError, this, &CSteDeviceManager::onSteDevError, Qt::QueuedConnection);

        //绑定设备连接上的处理
        connect(pTemp, &CDevSteFactory::signalDevStatus, this, &CSteDeviceManager::onFactoryConnectChange, Qt::QueuedConnection);
    }
    else
    {
        safeDeleteClassPoint(&pTemp);
    }
}

void CSteDeviceManager::delAllStation()
{
    for(auto it = m_mapTempFactory.begin(); it != m_mapTempFactory.end();)
    {
        if(it->second != nullptr && it->second->isStaionSte())
        {
            deleteMappingByName(it->second->getComName(), true);
            it = m_mapTempFactory.begin();
        }
        else
        {
            it++;
        }
    }
}

void CSteDeviceManager::timeOutDelInvalidDev()
{
    bool bErase;
    CDevSteFactory *pFactoryTemp;
    for(auto it = m_setDeletePointer.begin(); it != m_setDeletePointer.end();)
    {
        pFactoryTemp = *it;
        bErase = pFactoryTemp == nullptr;

        if(!bErase)
        {
            if((*it)->isDeleteThis())
            {
                safeDeleteClassPoint(&pFactoryTemp);
                bErase = true;
            }
        }

        if(bErase)
        {
            it = m_setDeletePointer.erase(it);
        }
        else
        {
            it++;
        }
    }
}

CDevSteFactory* CSteDeviceManager::getFactoryByName(const QString &qsName)
{
    CDevSteFactory* pRet = nullptr;
    auto itFindFactory = m_mapTempFactory.find(qsName);
    if(itFindFactory != m_mapTempFactory.end())
    {
        pRet = itFindFactory->second;
    }

    return pRet;
}

void CSteDeviceManager::delaySetDev()
{
    //延时设置设备，第一个设置的设备为组网设备，同时存在其他设备需要判断
    //判断条件为超过某个时间没有直连的设备，或者所有设备已经确认完成
    if(m_bDelaySetting && m_qsDelayComName.length() > 0)
    {
        bool bAllDevVeryfyCompleted = true;
        for(auto &elemet : m_mapTempFactory)
        {
            if(elemet.second == nullptr)
                continue;

            if(!elemet.second->isConfirmConnectComplete())
            {
                bAllDevVeryfyCompleted = false;
                break;
            }
        }

        //身高体重设备启动较慢，超时时间定长点，添加了变量bAllDevVeryfyCompleted不会影响其他设备的识别速度
        if(QDateTime::currentDateTime().toMSecsSinceEpoch() - m_llDelayDev < 5000 && !bAllDevVeryfyCompleted)
            return;

        m_bDelaySetting = false;

        //设备存在优先选择直连设备
        CDevSteFactory* pTempFactory = getFactoryByName(m_qsCurFactory);

        //设备存在，表示直连设备连接，删除当前组网设备
        if(pTempFactory != nullptr)
            return;

        pTempFactory = getFactoryByName(m_qsDelayComName);
        if(pTempFactory != nullptr)
        {
            setFactory(pTempFactory);
            m_qsDelayComName.clear();
        }
    }
}

/*************************************************
  <函数名称>    onMonitorTimeOut()
  <功能描述>    通讯接口的监控定时处理
  <参数说明>    无

  <返 回 值>    无

  <函数说明>
  <作    者>   zhy
  <时    间>   2022-02-17
  **************************************************/
void CSteDeviceManager::onMonitorTimeOut()
{
    //延时设置设备，
    delaySetDev();

    //枚举体测设备连接情况
    enumDevSteFactory();

    //判断设备是否可以删除，若可以则删除
    timeOutDelInvalidDev();
}

/*************************************************
  <函数名称>    onFactoryConnectChange(bool bStatus, QString strComName)
  <功能描述>    响应合法体测设备连接状态变更的处理函数
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    如果连接上了正确的设备，则赋值合法的设备工厂对象

  <作    者>   zhy
  <时    间>   2022-02-17
  **************************************************/
void CSteDeviceManager::onFactoryConnectChange(bool bStatus, void *pDev)
{
    CDevSteFactory *pSteDevFactory= reinterpret_cast<CDevSteFactory *>(pDev);
    if(pSteDevFactory == nullptr)
        return;

    //设备连接
    if(bStatus)
    {
        //若当前设备是直连设备，后续连接的所有设备都关闭
        CDevSteFactory *pFindFactory = getFactoryByName(m_qsCurFactory);
        if(pFindFactory != nullptr && pFindFactory->isValid() && !pFindFactory->isStaionSte())
        {
            delAllStation();
            return;
        }

        //若新连接的设备时直连设备，关闭所有组网设备
        if(!pSteDevFactory->isStaionSte())
        {
            delAllStation();
        }

        //第一次连接的设备为组网同时 串口 > 1, 那么延时设置设备
        if(m_bFirstConnect && pSteDevFactory->isStaionSte() && m_mapTempFactory.size() > 1)
        {
            m_bDelaySetting = true;
            m_qsDelayComName = pSteDevFactory->getComName();
            m_llDelayDev = QDateTime::currentDateTime().toMSecsSinceEpoch();
        }
        else
        {
            setFactory(pSteDevFactory);
        }

        m_bFirstConnect = false;
    }
    else
    {
        //若断连的设备为当前工厂，进行处理
        if(m_qsCurFactory == pSteDevFactory->getComName())
        {
            //体测设备断连会触发
            deleteMappingByName(pSteDevFactory->getComName(), true);

            //从已连接的设备中选择一个有效设备进行绑定,优先选择其中的直连设备
            vector<CDevSteFactory *> vecValidDirect; //有效直连设备
            vector<CDevSteFactory *> vecValidStation;//有效组网设备

            for(auto &element : m_mapTempFactory)
            {
                auto &pDevFactory = element.second;
                if(pDevFactory == nullptr)
                    continue;

                if(!pDevFactory->isValid())
                    continue;

                if(pDevFactory->isStaionSte())
                {
                    vecValidStation.push_back(pDevFactory);
                }
                else
                {
                    vecValidDirect.push_back(pDevFactory);
                }
            }

            pSteDevFactory = nullptr;

            if(vecValidDirect.size() > 0 && vecValidStation.size() > 0)
            {
                pSteDevFactory = vecValidDirect.size() > 0 ? vecValidDirect[0] : vecValidStation[0];
            }

            setFactory(pSteDevFactory);
        }
        //其他设备断连，删除对应的消息
        else
        {
            deleteMappingByName(pSteDevFactory->getComName(), true);
        }
    }
}

/*************************************************
  <函数名称>    onSteDevError(unsigned int iDevError, QString strComName)
  <功能描述>    响应合法体测设备错误连接的处理函数
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    如果连接上了错误的设备，则释放该对象，但并不清空m_mapTempFactory，因为可以作下一次
              设备连接的排他功能

  <作    者>   zhy
  <时    间>   2022-02-17
  **************************************************/
void CSteDeviceManager::onSteDevError(unsigned int iDevError, QString strComName)
{
    //如果是非法设备,释放临时工厂资源
    if(iDevError == 404)
    {
        auto pFactory = getFactoryByName(strComName);
        if(pFactory == nullptr)
            return;

        //设备进行了尝试连接，但是超时之后设备无效,删除对应指针，但是不删除map中的关系
        bool bDeleteInfo = pFactory->getDataReceivedState();

        deleteMappingByName(strComName, !bDeleteInfo);
    }
}

void CSteDeviceManager::deleteMappingByName(const QString &strComName, bool bDeleteMapping)
{
    auto itFind = m_mapTempFactory.find(strComName);

    if(itFind == m_mapTempFactory.end())
        return;

    auto pFactory = itFind->second;
    itFind->second = nullptr;

    if(pFactory != nullptr)
    {
        //当前设备是正在使用的设备
        if(strComName == m_qsCurFactory)
        {
            //将内部设备统一发送断连
            auto mapDev = pFactory->getAllDev();
            m_qsCurFactory.clear();

            for(auto &element : mapDev)
            {
                element.second->sendConnectState(false);
            }
        }

        //存在延时造成函数重入，需要在之前就将对应的设备设置为nullptr
        pFactory->devClose();

        disconnect(pFactory, &CDevSteFactory::signalHardwareConnectChange, this, &CSteDeviceManager::signalSteDevConnectChange);
        disconnect(pFactory, &CDevSteFactory::signalDevError, this, &CSteDeviceManager::onSteDevError);
        disconnect(pFactory, &CDevSteFactory::signalDevStatus, this, &CSteDeviceManager::onFactoryConnectChange);

        //删除new出的设备对象(不能立即删除，会导致软件崩溃)
        m_setDeletePointer.insert(pFactory);
    }

    itFind = m_mapTempFactory.find(strComName);
    if(bDeleteMapping && itFind != m_mapTempFactory.end())
    {
        m_mapTempFactory.erase(itFind);
    }
}

void CSteDeviceManager::setFactory(CDevSteFactory *pDev)
{
    auto pFactory = getFactoryByName(m_qsCurFactory);
    if(pFactory != nullptr)
    {
        //先将设备连接的所有设备全部断连
        auto mapDev = pFactory->getAllDev();
        for(auto &element : mapDev)
        {
            if(element.second == nullptr)
                continue;

            element.second->sendConnectState(false);
        }

        //同时取消设备变更消息
        disconnect(pFactory, &CDevSteFactory::signalHardwareConnectChange, this, &CSteDeviceManager::signalSteDevConnectChange);
        disconnect(pFactory, &CDevSteFactory::signalHardwareConnectChange, this, &CSteDeviceManager::onSteDevConnectChange);
    }

    if(pDev != nullptr)
    {
        m_qsCurFactory = pDev->getComName();

        //具体设备连接断连影响
        connect(pDev, &CDevSteFactory::signalHardwareConnectChange, this, &CSteDeviceManager::signalSteDevConnectChange, Qt::DirectConnection);
        connect(pDev, &CDevSteFactory::signalHardwareConnectChange, this, &CSteDeviceManager::onSteDevConnectChange, Qt::DirectConnection);

        //若连接的设备是直连设备，将其他所有有效设备关闭,主要是直连的时候关闭组网,需要删除在映射关系中保存的东西
        if(!pDev->isStaionSte())
        {
            delAllStation();
        }
    }
    else
    {
        m_qsCurFactory.clear();
    }

    emit signalFactoryConnectChange();
}

void CSteDeviceManager::onTestStateChange()
{
    auto *pFactory = getFactoryByName(m_qsCurFactory);
    if(!(pFactory != nullptr))
        return;
    //设备状态对外只有3种,测试未进行、测试中、测试完成,底层设备已经整合
    //设备中存在的状态为 未进行、测试中、测试完成三种

    auto mapDev = pFactory->getAllDev();

    //中长跑只针对终点设备进行判断
    if(pFactory->getDevType() == DEV_TYPE_RUN_MID)
    {
        map<int, CDevHardwareBase *> mapTemp;
        CDevSteRunMid *pTemp;
        for(auto &element : mapDev)
        {
            pTemp = dynamic_cast<CDevSteRunMid *>(element.second);
            if(pTemp == nullptr)
                continue;

            if(pTemp->getRunMidDevType() == CDevSteRunMid::RUN_MID_TYPE_DESTINATION)
            {
                mapTemp[element.first] = element.second;
                break;
            }
        }

        mapDev = mapTemp;
    }

    set<short> setTestState;
    bool bDevErr = true;
    for(auto &element : mapDev)
    {
        bDevErr &= element.second->getTestState() == CTestStateManage::TEST_ERR;
        setTestState.insert(element.second->getTestState());
    }

    //测试中 其中一个是测试中
    if(setTestState.find(CTestStateManage::TEST_IN_PROCESS) != setTestState.end())
    {
        m_objTestStateManager.setTestState(CTestStateManage::TEST_IN_PROCESS);
    }
    else
    {        
        //全部设备断连
        if(bDevErr)
        {
            m_objTestStateManager.setTestState(CTestStateManage::TEST_ERR);
            return;
        }

        if(setTestState.find(CTestStateManage::TEST_COMPLETED) != setTestState.end())
        {
            m_objTestStateManager.setTestState(CTestStateManage::TEST_COMPLETED);
        }
        else
        {
            m_objTestStateManager.setTestState(CTestStateManage::TEST_NOT_START);
        }
    }
}

void CSteDeviceManager::onSteDevConnectChange(bool bConnect, void *pDev)
{
    if(pDev == nullptr)
    {
        LOGERROR("pSteDev invalid");
        return;
    }

    CDevHardwareBase *pSteDev = reinterpret_cast<CDevHardwareBase *>(pDev);
    if(bConnect)
    {
        connect(pSteDev, &CDevHardwareBase::signalTestStateChange, this, &CSteDeviceManager::onTestStateChange);
    }
    else
    {
        disconnect(pSteDev, &CDevHardwareBase::signalTestStateChange, this, &CSteDeviceManager::onTestStateChange);
    }
}

