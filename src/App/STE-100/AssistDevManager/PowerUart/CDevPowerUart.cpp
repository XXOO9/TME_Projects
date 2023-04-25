#include "CDevPowerUart.h"
#include "PowerUartFifo8.h"
#include "PowerUartApplication.h"
#include "CLogger.h"
#include "commondefin.h"
#include "CEnumSerialPort.h"

#include <Windows.h>
CDevPowerUart::CDevPowerUart()
    : m_shDataCOntainerSize(1024)
{
    m_pDataContainer = new unsigned char[m_shDataCOntainerSize];

    connect(this, &CDevPowerUart::signalWorkState, this, &CDevPowerUart::onSetWorkState);

    m_iNoDataInteval = 2000;
}

CDevPowerUart::~CDevPowerUart()
{

    if(m_pDataContainer != nullptr)
        delete [] m_pDataContainer;

    m_pDataContainer = nullptr;
}

void CDevPowerUart::setWorkState(bool bState)
{
    emit signalWorkState(bState);
}

void CDevPowerUart::sendHeartBeatCmd()
{
    sendCmdToDev(CMD_POWER_DEVICE_HEART);
}

void CDevPowerUart::sendChargeCmd(unsigned short shChargeType)
{
    sendCmdToDev(CMD_SET_POWER_DEVICE_PAD_ChargeE, &shChargeType, 1 );
}

void CDevPowerUart::sendLightModeCmd(unsigned short shMode)
{
    sendCmdToDev(CMD_SET_POWER_DEVICE_LEDBAND_MODE, &shMode, 1 );
}

void CDevPowerUart::sendLightBeltCmd(unsigned short shMode, int nTestCode)
{
    LOGINFO("-------------------------------");
    LOGINFO("---Start Send LightBelt Cmd");
    unsigned short shCmd = CMD_SET_POWER_DEVICE_LEDBAND_MODE;

    QByteArray arrLedParam;
    getLightBeltParam(arrLedParam, shMode, nTestCode);

    unsigned short shData[10];
    memcpy(shData, arrLedParam.data(), 10);
    PowerUartWriteApplicationDataSegment(shCmd, shData, 5);

    unsigned short shReadDataLen = MyQueue_Fifo8_getArray(&g_PowerUartSendFifo8, m_pDataContainer, m_shDataCOntainerSize, m_shDataCOntainerSize);
    if (shReadDataLen > 0)
    {
        QByteArray arr(reinterpret_cast<char*>(m_pDataContainer), shReadDataLen);
        //qDebug() << arr.toHex();
        sendData(arr);
    }
    LOGINFO("---End Send LightBelt Cmd");
}

void CDevPowerUart::sendPowerSupplyState()
{
    sendCmdToDev(CMD_GET_POWER_DEVICE_SUPPLY);
}

int CDevPowerUart::GetColorIndexByTestCode(int nTestCode)
{
    /* 暂定（软件可根据情况自行调整）各种颜色代表的分数分别为（假设满分 100 分）
     * 0: 金色（100 分）
     * 1/2: 银灰色/爱丽丝蓝（90 分~99 分）
     * 3: 铜色（80 分~89 分）
     * 4: 闪光绿（60 分~79 分）
     * 5: 纯蓝（30 分~59 分）
     * 6: 紫色（0 分~29 分）
     */
    int nColorIndex = LED_BELT_COLOR_GOLD;

    nTestCode = nTestCode < 0 ? 0 : (nTestCode > 100 ? 100 : nTestCode);
    if (nTestCode == 100)
    {
        nColorIndex = LED_BELT_COLOR_GOLD;
    }
    else if (nTestCode >= 90 && nTestCode <= 99)
    {
        nColorIndex = LED_BELT_COLOR_SILVER;
    }
    else if (nTestCode >= 80 && nTestCode <= 89)
    {
        nColorIndex = LED_BELT_COLOR_COPPERSTAINS;
    }
    else if (nTestCode >= 60 && nTestCode <= 79)
    {
        nColorIndex = LED_BELT_COLOR_LIME;
    }
    else if (nTestCode >= 30 && nTestCode <= 59)
    {
        nColorIndex = LED_BELT_COLOR_BLUE;
    }
    else
    {
        nColorIndex = LED_BELT_COLOR_PURPLE;
    }

    return nColorIndex;
}

void CDevPowerUart::onSetWorkState(bool bState)
{
    if(bState)
    {
        safeNewClassPoint(&m_pSerialPortCom);
        safeNewClassPoint(&m_pDataParse);
        safeNewClassPoint(&m_pConfirmDev);

        //信号触发保证串口和当前设备处于同一线程
        setSerialPortProperty(m_pSerialPortCom);

        //这里设备会开启失败，但是会触发设备中的重连机制，在重连机制当中进行
        //设备的重连,主要是开启数据更新定时器触发重连
        setSelfReconnectFlag(true);
        devOpen();
    }
    else
    {
        devClose();

        setCommunicate(nullptr);
        safeDeleteClassPoint(&m_pSerialPortCom);
        safeDeleteClassPoint(&m_pDataParse);
        safeDeleteClassPoint(&m_pConfirmDev);
    }
}

void CDevPowerUart::sendCmdToDev(unsigned short shCmd, unsigned short *shCmdType, unsigned short shCmdTypeLen)
{
    PowerUartWriteApplicationDataSegment(shCmd, shCmdType, shCmdTypeLen);

    unsigned short shReadDataLen = MyQueue_Fifo8_getArray(&g_PowerUartSendFifo8, m_pDataContainer, m_shDataCOntainerSize, m_shDataCOntainerSize);
    if(shReadDataLen > 0)
    {
        QByteArray arr(reinterpret_cast<char *>(m_pDataContainer), shReadDataLen);
        sendData(arr);
    }
}

void CDevPowerUart::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(nDataLen < 2 || pData == nullptr)
        return;

    //命令个数等于 数据长度 / 2
    nDataLen = nDataLen >> 1;
    unsigned short *pCmd = static_cast<unsigned short *>(pData);

    //判断是心跳需要返回
    switch (pCmd[0]){
    //本来进行回复心跳，在固定的数据更新结构中进行心跳回复
    case CMD_POWER_DEVICE_HEART:
        break;
    case CMD_UPLOAD_POWER_DEVICE_SUPPLY:
    {
        if(nDataLen == 3)
        {
            //参数分为两个 第一个是电源电量百分比 第二个是否插了220v电源 0是220 1是电源
            bool bConnect220VPower = pCmd[2] == 0;
            unsigned short shRemindPower = pCmd[1];

            if(bConnect220VPower != m_bPowerUintConnect220VPower || m_bUpdatePowerInfo)
            {
                m_bPowerUintConnect220VPower = bConnect220VPower;
                emit signalPowerUintConnec220VChange(m_bPowerUintConnect220VPower);
            }

            //连接电源之后存在独立电源才发送剩余电量
            //if(!m_bPowerUintConnect220VPower || m_bUpdatePowerInfo)
            {
                m_shRemaindPowerForPowerUart = static_cast<short>(shRemindPower);
                emit signalPowerUintBatteryChange(m_shRemaindPowerForPowerUart);
            }

            m_bUpdatePowerInfo = false;
        }
    }
        break;
    default:
        break;
    }
}

void CDevPowerUart::onUpdateTimeOut()
{
    CDeviceBase::onUpdateTimeOut();

    //添加充电逻辑，充电需要持续发送命令
    if(isValid())
    {
        if(m_bCharge)
        {
            m_iChargeIntevalTotal += m_iUpdateInteval;

            if(m_iChargeIntevalTotal >= m_iChargeInteval)
            {
                sendChargeCmd(PAD_ChargeE_ON);
                m_iChargeIntevalTotal -= m_iChargeInteval;
            }
        }

        //查询电量
        m_iQueryBatteryIntevalTotal += m_iUpdateInteval;
        if(m_iQueryBatteryIntevalTotal >= m_iQueryBatteryInteval)
        {
            queryingHostPower();
            m_iQueryBatteryIntevalTotal -= m_iQueryBatteryInteval;
        }

        //定时给底层设备发送心跳
        m_nSendHeartbeatTotal += m_iUpdateInteval;
        if(m_nSendHeartbeatTotal > m_nSendHeartbeatInteval)
        {
            m_nSendHeartbeatTotal -= m_nSendHeartbeatInteval;
            sendHeartBeatCmd();
        }
    }
}

void CDevPowerUart::reconnect()
{
    if(isValid())
    {
        return;
    }

    //当前设备重连是从串口中找到对应的串口设备
    //设置交互类，需要在触发活动当中保证new出来
    if(!m_pConfirmDev->isValidCommunicate())
    {
        m_pConfirmDev->setCommunicate(m_pSerialPortCom);
    }

    if(!m_pConfirmDev->isValidDataParse())
    {
        m_pConfirmDev->setDataParse(m_pDataParse);
    }

    CEnumSerialPort &enumSerialPort = CEnumSerialPort::Get_Instance();

    //电源优先级较高,在重连时如果存在电源设备那么保证电源设备先进行重连
    int nMes = m_qsComName != "" ? 300 : 20;

    if(!enumSerialPort.lockUseSerialPortCom(nMes))
        return;

    const QStringList &comNameList = enumSerialPort.getSerialPortCom();

    QSerialPortInfo portInfo;

    //握手命令为空
    QByteArray arrCmd;
    bool bFindDev;

    //若当前无效串口中存在的名称不在枚举列表中，代表无效列表中的设备断连需要删除
    delMapAccordToComList(comNameList);

    map<QString,int>::iterator itFindCom;
    for(auto &comName : comNameList)
    {
        //若一个串口连续几次都没有数据，那么标记串口
        itFindCom = m_mapMarkingDev.find(comName);
        if(itFindCom != m_mapMarkingDev.end())
        {
            if(itFindCom->second >= 3)
            {
                //若串口在设备中同时设备无效，那么删除在无效设备中的标记，避免信息不一致造成电源设备不能重连
                if(m_qsComName != "" && m_qsComName == comName){
                    m_mapMarkingDev.erase(itFindCom);
                }else{
                    continue;
                }
            }
        }

        portInfo = QSerialPortInfo(comName);
        if(portInfo.isBusy())
            continue;

        m_pSerialPortCom->setComName(comName);
        m_pConfirmDev->clearData();
        m_pConfirmDev->devOpen();

        //经过调试确认电源设备的确认时间在 800ms内（在台式电脑上）
        bFindDev = m_pConfirmDev->confirmDev(arrCmd);
        m_pConfirmDev->devClose();

        //发现设备之后再开启设备即可
        if(bFindDev)
        {
            findTargetDev(comName);
            break;
        }
        //存在过数据同时不是电源设备那么记录设备为无效设备
        else if(!bFindDev && m_pDataParse->haveBeenData())
        {
            //记录本次无效的通讯口名称
            recordInvalidComName(comName);
        }
    }

    //串口使用完成解绑即可
    enumSerialPort.unlockUseSerialPortCom();
}


bool CDevPowerUart::devOpen()
{
    if(isValid())
        return true;

    m_iIntervalTotal = 0;

    //调整缓存大小
    m_Cache.resize(static_cast<int>(m_iCacheSize));

    //新的定时器
    if(m_pTimerUpdateInfo == nullptr)
    {
        safeNewClassPoint(&m_pTimerUpdateInfo);
        connect(m_pTimerUpdateInfo, &QTimer::timeout, this, &CDevPowerUart::onUpdateTimeOut);
        m_pTimerUpdateInfo->start(static_cast<int>(m_iUpdateInteval));

        m_iIntervalTotal = 0;
    }

    //重置电源电量信息标志
    m_bUpdatePowerInfo = true;

    //若不存在重连标志，需要所有 信息都存在
    if(m_pCom == nullptr)
        return false;

    m_bOpened = m_pCom->open();
    if(m_bOpened)
    {
        emit signalConnectChange(this, true);
    }

    return m_bOpened;
}

void CDevPowerUart::setSerialPortProperty(CSerialPortCom *pCom)
{
    if(pCom == nullptr)
        return;

    pCom->setOpenMode(CSerialPortCom::ReadWrite);
    pCom->setPortBaudRate(QSerialPort::Baud9600);
}

void CDevPowerUart::delMapAccordToComList(const QStringList &comList)
{
    int nFindExistIndex;
    for(auto it = m_mapMarkingDev.begin(); it != m_mapMarkingDev.end();)
    {
        nFindExistIndex = comList.contains(it->first);
        if(nFindExistIndex < 0)
        {
            m_mapMarkingDev.erase(it);
            it = m_mapMarkingDev.begin();
        }
        else
        {
            it++;
        }
    }
}

void CDevPowerUart::recordInvalidComName(const QString &qsComName)
{
    auto itFind = m_mapMarkingDev.find(qsComName);
    if(itFind == m_mapMarkingDev.end())
    {
        m_mapMarkingDev[qsComName] = 1;
    }
    else
    {
        itFind->second++;
    }
}

void CDevPowerUart::findTargetDev(const QString &qsComName)
{
    m_qsComName = qsComName;
    //找到设备无用的设备标记
    m_mapMarkingDev.clear();

    //清空确认设备
    m_pConfirmDev->setDataParse(nullptr);
    m_pConfirmDev->setCommunicate(nullptr);

    setCommunicate(m_pSerialPortCom);
    setDataParse(m_pDataParse);
    clearData();
    devOpen();
}

void CDevPowerUart::setChargeState(bool bCharge)
{
    //充电需要连续发送充电指令
    m_bCharge = bCharge;

    if(m_bCharge)
    {
        m_iChargeIntevalTotal = 0;

        //充电逻辑在更新 信息响应当中触发
        //LOGINFO("Start charging");
        sendChargeCmd(PAD_ChargeE_ON);
    }
    else
    {
        //发送停止充电命令
        //LOGINFO("End charging");
        sendChargeCmd(PAD_ChargeE_OFF);
    }
}

void CDevPowerUart::setLightBelt(unsigned short shMode, int nTestCode)
{
    sendLightBeltCmd(shMode, nTestCode);
}

void CDevPowerUart::getLightBeltParam(QByteArray &arr, unsigned short shMode, int nTestCode)
{
    arr.resize(10);

    memset(arr.data(), 0, static_cast<unsigned int>(arr.length()));
    //灯效模式
    unsigned char cLEDBeltEffectType = LEDBAND_MODE_BREATHE_NEW;
    unsigned char *pDestData = reinterpret_cast<unsigned char *>(arr.data());
    unsigned short shParams;

    switch (shMode) {
    case LED_BELT_MODE_SHUTDOWN:  //关机
    {
        cLEDBeltEffectType = LEDBAND_MODE_TURNOFF;
    }
    break;
    case LED_BELT_MODE_SYSTEM_ERROR:           // 系统错误
    {   //系统错误：红色指示：持续快闪(亮0.25s 灭0.25s)，直到错误解决
        cLEDBeltEffectType = LEDBAND_MODE_FLASH;
        pDestData[1] = 255;        //红色
        pDestData[2] = 0;
        pDestData[3] = 0;

        shParams = 250;
        memcpy(&pDestData[6], &shParams, sizeof(shParams));
        memcpy(&pDestData[8], &shParams, sizeof(shParams));
    }
    break;
    case LED_BELT_MODE_BODY_TEST_FOUL:         // 体测犯规
    {  //体测犯规：橙色指示：持续快闪(亮0.25s 灭0.25s)直到犯规标记清除或本次测试结束
        cLEDBeltEffectType = LEDBAND_MODE_FLASH;
        pDestData[1] = 255;        //橙色RGB(255, 165, 0)
        pDestData[2] = 165;
        pDestData[3] = 0;

        shParams = 250;
        memcpy(&pDestData[6], &shParams, sizeof(shParams));
        memcpy(&pDestData[8], &shParams, sizeof(shParams));
    }
    break;
    case LED_BELT_MODE_BODY_TESTING:           // 体测进行中
    {   //体测进行中：彩虹流水指示：持续流转
        cLEDBeltEffectType = LEDBAND_MODE_WATERFALL_COLORFULL;
        pDestData[5] = 1;
    }
    break;
    case LED_BELT_MODE_BODY_TEST_RESULT_LOCK:  // 体测结果锁定
    {   /* 体测结果锁定：单色指示：持续慢闪(亮1s灭1s)直到用户退出或超时（比如30s）。
         * 颜色由平板软件根据评分(由高到低)：金色、银灰色/爱丽丝蓝色、铜色、闪光绿、纯蓝、紫色
         * 金色（255, 215, 0） 银灰色（192, 192, 192），爱丽丝蓝色（240, 248, 255）铜色（184, 115, 51)
         * 闪光绿（0, 255, 0）、纯蓝（0, 0, 255）、紫色（128, 0, 128）
         */
        unsigned char chColor[7][3] = { {255, 215, 0},
                               {192, 192, 192},
                               {240, 248, 255},
                               {184, 115, 51},
                               {0, 255, 0},
                               {0, 0, 255},
                               {128, 0, 128}};

        int nIndex = GetColorIndexByTestCode(nTestCode);
        cLEDBeltEffectType = LEDBAND_MODE_FLASH;
        pDestData[1] = chColor[nIndex][0];
        pDestData[2] = chColor[nIndex][1];
        pDestData[3] = chColor[nIndex][2];

        shParams = 1000;
        memcpy(&pDestData[6], &shParams, sizeof(shParams));
        memcpy(&pDestData[8], &shParams, sizeof(shParams));
    }
    break;
    case LED_BELT_MODE_DEFUALT:                // 默认
    default:
    {   //默认：绿色指示：持续周期性呼吸节律渐变（5s周期，这里的周期不用pc向下设置）
        cLEDBeltEffectType = LEDBAND_MODE_BREATHE_NEW;
        pDestData[2] = 255;
    }
    break;
    }

    pDestData[0] = cLEDBeltEffectType;
}

short CDevPowerUart::getRemindPower()
{
    SYSTEM_POWER_STATUS systemPowerSatus;
    BOOL bRet =  GetSystemPowerStatus(&systemPowerSatus);

    short shRemindPower = 50;
    if(bRet)
    {
        shRemindPower = static_cast<short>(systemPowerSatus.BatteryLifePercent);
    }

    if(shRemindPower < 0 || shRemindPower > 100)
    {
        shRemindPower = 50;
    }

    return shRemindPower;
}

//返回值0~100
short CDevPowerUart::getRemindPowerForPowerUart()
{
    if(m_shRemaindPowerForPowerUart < 0 || m_shRemaindPowerForPowerUart > 100)
    {
        return 10;
    }

    return m_shRemaindPower;
}

void CDevPowerUart::queryingHostPower()
{
    m_shSendPadQueryBatteryIndex++;

    //查询平板电量不需要那么频繁，一分钟左右查询一次
    //这个函数更新时间间隔是 m_iQueryBatteryInteval 60s时间就是 次数20次
    if(m_shSendPadQueryBatteryIndex >= 20)
    {
        SYSTEM_POWER_STATUS systemPowerSatus;
        BOOL bRet =  GetSystemPowerStatus(&systemPowerSatus);
        if(bRet)
        {
            short shRemaindPower = static_cast<short>(systemPowerSatus.BatteryLifePercent);

            string strPower = to_string(shRemaindPower);
            //LOGINFO("remaind power " + strPower);

            if(m_shRemaindPower != shRemaindPower && shRemaindPower > 0)
            {
                m_shRemaindPower = shRemaindPower;
            }

            //电量低于 40 发送充电指令
            if(shRemaindPower < 40)
            {
                setChargeState(true);
            }

            //电量高于 70 停止平板充电
            if(shRemaindPower >= 70)
            {
                setChargeState(false);
            }
        }
        m_shSendPadQueryBatteryIndex = 0;
    }

    //查询供电状态，需要比较频繁的查找,现在定义的3s查找一次
    sendPowerSupplyState();
}
