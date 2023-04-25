#include "CDevice.h"
#include <QCoreApplication>

CDevice::CDevice()
{

}

CDevice::~CDevice()
{
    devClose();
}

bool CDevice::isValid()
{
    bool bRet = false;
    if(m_pCom != nullptr)
    {
        bRet = m_pCom->IsOpened() && m_bOpened;
    }
    return bRet;
}

/*************************************************
  <函数名称>    devOpen
  <功能描述>    打开设备
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    打开设备的同时，将通讯接口的通讯过程
                移到一个新的线程中，让该线程持续获取
                通讯接口发送来的数据，并立刻保存

  <作    者>   zhy
  <时    间>   2021-11-22
  **************************************************/
bool CDevice::devOpen()
{
    //如果没有打开过并且需要构建独立通讯过程
    if (!m_bOpened)
    {
        if(m_pCom != nullptr)
        {
            m_bOpened = m_pCom->open();

            //如打开成功，开始数据读取线程
            if(m_bOpened)
            {
                //调整缓存大小
                m_Cache.resize(static_cast<int>(m_iCacheSize));

                //connect(&m_communicationThread, &QThread::finished, this, &QObject::deleteLater);
                //connect(&m_communicationThread, &QThread::finished, m_pCom, &QObject::deleteLater);

                //m_pCom->moveToThread(&m_communicationThread);

                //将通讯接口移入一个线程，在该线程中读取数据，并存入队列，保证数据不丢失
                //这里将该线程的优先级 设置为最高级别
                //m_communicationThread.start(QThread::Priority::HighestPriority);

                //开始设备数据更新定时器，定时解析通讯接口的数据，并更新该设备对象的相关信息和数据
                //设置查询时间间隔为50毫秒
                setUpdateDataState(true);
            }
        }
    }

    return m_bOpened;
}

/*************************************************
  <函数名称>    devClose
  <功能描述>    关闭设备
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    关闭设备的同时，退出通讯线程，这里是否
                存在多线程同步问题？需求进一步确认

  <作    者>   zhy
  <时    间>   2021-11-22
  **************************************************/
void CDevice::devClose()
{
    if (m_bOpened)
    {
        m_bOpened = false;

        if(m_pCom != nullptr)
        {
            m_pCom->close();
        }

        setUpdateDataState(false);
    }

    //避免在发送命令当中阻塞
    m_bSendCommandSucess = true;
}


void CDevice::devGetBasicInfo()
{

}

bool CDevice::devReset()
{
    return false;
}

void CDevice::setCommunicate(ICommunicate *pCommunicate)
{
    if(m_pCom != nullptr)
    {
        m_pCom->close();
        disconnect(m_pCom, &ICommunicate::signalErrOccurred, this, &CDevice::onComErrOccurred);
        disconnect(this, &CDevice::signalSendDataToDev, m_pCom, &ICommunicate::signalSendData);
    }

    m_pCom = pCommunicate;

    if(m_pCom != nullptr)
    {
        m_pCom->clearData();
        m_pCom->close();
        connect(m_pCom, &ICommunicate::signalErrOccurred, this, &CDevice::onComErrOccurred);
        connect(this, &CDevice::signalSendDataToDev, m_pCom, &ICommunicate::signalSendData);
    }
}

bool CDevice::sendData(QByteArray &array, bool bWait, int nMsec, unsigned int uiCommandLevel/* = LEVEL_0*/, long long llStartTime/* = 0*/)
{
    if(!isValid())
        return false;

    m_bSendingData = true;

    bool bRet = true;
    //发送信号,某些长度为0的数据若需要等待也让函数进行等待
    if(array.length() > 0)
    {
        emit signalSendDataToDev(array, uiCommandLevel,llStartTime);
    }

    if(bWait)
    {
        m_bSendCommandSucess = false;
        QTime time = QTime::currentTime().addMSecs(nMsec);

        //等待时间超过 命令成功 或者设备关闭 退出当前循环
        while(!(m_bSendCommandSucess || QTime::currentTime() >= time || !isValid()))
        {
            //获取当前时间
            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        }

        bRet = m_bSendCommandSucess;
    }

    m_bSendingData = false;
    return bRet;
}

bool CDevice::sendData(unsigned long length, void *pData, bool bWait, int nMsec, unsigned int uiCommandLevel, long long llStartTime/* = 0*/)
{
    QByteArray array(static_cast<char *>(pData), static_cast<int>(length));
    return sendData(array, bWait, nMsec, uiCommandLevel, llStartTime);
}

void CDevice::clearData()
{
    if(m_pCom != nullptr)
    {
        m_pCom->clearData();
    }

    m_Cache.clear();
    m_iExistCacheSize = 0;
}

void CDevice::onUpdateTimeOut()
{
    //如果是打开了通讯接口
    if(!(m_bOpened && m_pCom != nullptr))
        return;

    //计算剩余缓存空间
    int iCaCheSize = m_Cache.size();
    int iFreeCacheSize = iCaCheSize - m_iExistCacheSize;

    if(iFreeCacheSize > 0)
    {
        //读取一定数据
        unsigned int lReadLength = m_pCom->readData(static_cast<unsigned int>(iFreeCacheSize), m_Cache.data() + m_iExistCacheSize);
        m_iExistCacheSize += lReadLength;

        //如果有数据则进行解析
        if (lReadLength != 0)
        {
            dataParse(m_Cache);

            //有数据则认为设备连接正常，对超时累计清零
            m_iIntervalTotal = 0;
        }
        else
        {
            if(m_bDetermineDisconnect)
            {
                //如果没有数据，进行超时累计
                m_iIntervalTotal += m_iUpdateInteval;
                //如果超过没有数据累计时间间隔，则认为设备断开
                if (m_iIntervalTotal >= m_iNoDataInteval)
                {
                    emit signalDevStatus(false, this);
                }
            }
        }
    }
    else if(iFreeCacheSize == 0)
    {
        emit signalParseBusy();
    }
    else
    {
        //m_Cache初始化出错
    }
}

void CDevice::setUpdateDataState(bool bOpen)
{
    if(bOpen == m_bUpdateDataState)
    {
        return;
    }

    if(bOpen)
    {
        if(m_updateDevInfoTimer.isActive())
        {
            return;
        }

        connect(&m_updateDevInfoTimer, &QTimer::timeout, this, &CDevice::onUpdateTimeOut, Qt::UniqueConnection);
        m_updateDevInfoTimer.start(static_cast<int>(m_iUpdateInteval));
        m_bUpdateDataState = true;
    }
    else
    {
        if(!m_updateDevInfoTimer.isActive())
        {
            return;
        }

        m_updateDevInfoTimer.stop();
        disconnect(&m_updateDevInfoTimer, &QTimer::timeout, this, &CDevice::onUpdateTimeOut);

        m_bUpdateDataState = false;
    }
}
