#include "CDeviceBase.h"
#include "commondefin.h"

#include <QCoreApplication>

CDeviceBase::CDeviceBase()
    :CDevice ()
{
    m_Cache.resize(static_cast<int>(m_iCacheSize));
    connect(this, &CDeviceBase::signalReconnect, this, &CDeviceBase::reconnect, Qt::ConnectionType::QueuedConnection);
}

CDeviceBase::~CDeviceBase()
{
}

void CDeviceBase::setDataParse(CDataParseBase *pDataParse)
{
    m_pDataParse = pDataParse;

    if(m_pDataParse != nullptr)
    {
        m_pDataParse->clearDataCache();
        //若解析成功通知当前类
        m_pDataParse->setNotifyFunc(&CDeviceBase::onDataParase, *this);
    }
}

void CDeviceBase::clearData()
{
    if(m_pDataParse != nullptr)
    {
        m_pDataParse->clearDataCache();
    }

    if(m_pCom != nullptr)
    {
        m_pCom->clearData();
    }
}

bool CDeviceBase::isValid()
{
    bool bRet = false;
    if(m_pCom != nullptr)
    {
        bRet = m_pCom->IsOpened() && m_pDataParse != nullptr;
    }

    return bRet;
}

void CDeviceBase::devClose()
{
    if (m_bOpened)
    {
        if(m_pCom != nullptr)
        {
            m_pCom->close();
        }

        m_bOpened = false;
    }

    if(m_pTimerUpdateInfo != nullptr && !m_bSelfReconnectFlag)
    {
        m_pTimerUpdateInfo->stop();
        disconnect(m_pTimerUpdateInfo, &QTimer::timeout, this, &CDeviceBase::onUpdateTimeOut);
        safeDeleteClassPoint(&m_pTimerUpdateInfo);
    }

    //避免在发送命令当中阻塞
    m_bSendCommandSucess = true;
}

/*************************************************
  <函数名称>    devOpen
  <功能描述>    打开设备
  <参数说明>    无

  <返 回 值>    无

  <函数说明>


  <作    者>   zhy
  <时    间>   2021-11-22
  <修    改>   cl 需要注意使用时和设备所处的线程处于同一线程
  **************************************************/
bool CDeviceBase::devOpen()
{
    if(isValid())
        return true;

    //关闭中会关闭定时器 删除定时器
    devClose();

    if(m_pCom == nullptr)
        return false;

    m_bOpened = m_pCom->open();
    if(m_bOpened)
    {
        emit signalConnectChange(this, true);
    }
    else
    {
        return false;
    }

    //调整缓存大小
    m_Cache.resize(static_cast<int>(m_iCacheSize));

    //新的定时器
    if(m_pTimerUpdateInfo == nullptr)
    {
        safeNewClassPoint(&m_pTimerUpdateInfo);
        connect(m_pTimerUpdateInfo, &QTimer::timeout, this, &CDeviceBase::onUpdateTimeOut, Qt::UniqueConnection);
        m_pTimerUpdateInfo->start(static_cast<int>(m_iUpdateInteval));
    }

    m_iIntervalTotal = 0;
    return true;
}

bool CDeviceBase::devReset()
{
    devClose();
    m_pCom = nullptr;
    m_pDataParse = nullptr;

    return true;
}

void CDeviceBase::setSelfReconnectFlag(bool bFlag)
{
    m_bSelfReconnectFlag = bFlag;
}

void CDeviceBase::onUpdateTimeOut()
{
    if(isValid())
    {
        unsigned long lRealReadLen = m_pCom->readData(static_cast<unsigned int>(m_Cache.size()), m_Cache.data());

        if(lRealReadLen > 0)
        {
            m_pDataParse->dataParse(lRealReadLen, m_Cache.data());
            m_iIntervalTotal = 0;
        }
        else
        {
            m_iIntervalTotal += m_iUpdateInteval;
            //设备正常开启 并且 断连时间超时
            if(detectDisconnected())
            {
                m_iIntervalTotal -= m_iNoDataInteval;
                //设备内部重连逻辑
                if(m_bSelfReconnectFlag)
                    m_iReconnectIntevalTotal = 0;

                devReset();
                emit signalConnectChange(this, false);
            }
        }
    }
    else
    {
        if(m_bSelfReconnectFlag)
        {
            m_iReconnectIntevalTotal += m_iUpdateInteval;
            if(m_iReconnectIntevalTotal >= m_iReconnectInteval)
            {
                m_iReconnectIntevalTotal -= m_iReconnectInteval;
                emit signalReconnect();
            }
        }
    }
}

void CDeviceBase::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(pData == nullptr || nDataLen == 0)
        return;
}

void CDeviceBase::onDataParase(void *pData, unsigned int nDataLen)
{
    onDataParseSucess(pData, nDataLen);
}

void CDeviceBase::reconnect()
{

}

void CDeviceBase::onComErrOccurred()
{
    if(m_pCom != nullptr)
    {
        //unsigned long lErrCode = m_pCom->getLastErr();
        //signalConnectChange(this, false);
    }
}

//检测是否断连，通常的处理为超出了断连时间认为断连，但是某些设备认为仅
//串口断连才断连
bool CDeviceBase::detectDisconnected()
{
    return m_iIntervalTotal > m_iNoDataInteval;
}
