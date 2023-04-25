#include "CDevFoulControl.h"
#include "CLogger.h"
#include "commondefin.h"
#include "CEnumSerialPort.h"

using namespace FOUL_DEV_CMD;

#include <Windows.h>
CDevFoulControl::CDevFoulControl()
    : m_shDataCOntainerSize(1024)
{
    m_pDataContainer = new unsigned char[m_shDataCOntainerSize];

    connect(this, &CDevFoulControl::signalWorkState, this, &CDevFoulControl::onSetWorkState);

    m_iNoDataInteval = 2000;
}

CDevFoulControl::~CDevFoulControl()
{

    if(m_pDataContainer != nullptr)
        delete [] m_pDataContainer;

    m_pDataContainer = nullptr;
}

void CDevFoulControl::setWorkState(bool bState)
{
    emit signalWorkState(bState);
}

void CDevFoulControl::sendHeartBeatCmd()
{
}

void CDevFoulControl::onSetWorkState(bool bState)
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

void CDevFoulControl::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(nDataLen < 2 || pData == nullptr)
        return;

    //更新设备断连时间
    m_iIntervalTotal = 0;

    static unsigned short shCmd;
    memcpy(&shCmd, pData, 2);
    if(shCmd != FoulDevHR)
    {
        qDebug() << "CDevFoulControl cmd:" << shCmd;
        LOGINFO("CDevFoulControl cmd:" + to_string(shCmd));
    }

    //判断是心跳需要返回
    switch (shCmd){
    //心跳
    case FoulDevHR:
        break;
    //存在犯规
    case FoulDevIllegal:
    case FoulDevFoul:
        sigSendFoulType(shCmd);
        break;
    }
}

void CDevFoulControl::onUpdateTimeOut()
{
    CDeviceBase::onUpdateTimeOut();

    //添加充电逻辑，充电需要持续发送命令
    if(isValid())
    {
        //定时给底层设备发送心跳
        m_nSendHeartbeatTotal += m_iUpdateInteval;
        if(m_nSendHeartbeatTotal > m_nSendHeartbeatInteval)
        {
            m_nSendHeartbeatTotal -= m_nSendHeartbeatInteval;
            sendHeartBeatCmd();
        }
    }
}

void CDevFoulControl::reconnect()
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

    //若存在对应的串口名称
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
                //若串口在设备中同时设备无效，那么删除在无效设备中的标记，避免信息不一致造成设备不能重连
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


bool CDevFoulControl::devOpen()
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
        connect(m_pTimerUpdateInfo, &QTimer::timeout, this, &CDevFoulControl::onUpdateTimeOut);
        m_pTimerUpdateInfo->start(static_cast<int>(m_iUpdateInteval));

        m_iIntervalTotal = 0;
    }

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

void CDevFoulControl::setSerialPortProperty(CSerialPortCom *pCom)
{
    if(pCom == nullptr)
        return;

    pCom->setOpenMode(CSerialPortCom::ReadWrite);
    pCom->setPortBaudRate(QSerialPort::Baud115200);
}

void CDevFoulControl::delMapAccordToComList(const QStringList &comList)
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

void CDevFoulControl::recordInvalidComName(const QString &qsComName)
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

void CDevFoulControl::findTargetDev(const QString &qsComName)
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

