#include <QDateTime>
#include <QDebug>
#include <QIODevice>

#include "CSerialPortCom.h"
#include "CLogger.h"
#include <QCoreApplication>

CSerialPortCom::CSerialPortCom()
{
    initialize();
}

CSerialPortCom::CSerialPortCom(QSerialPortInfo portInfo)
    : ICommunicate()
{
    m_serialPortInfo = portInfo;

    initialize();
}


CSerialPortCom::~CSerialPortCom()
{
    uninitialize();
}

/*************************************************
  <函数名称>    IsValid
  <功能描述>    判定通讯是否有效，心跳包超时超过次数上限即失效
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   cpc
  <时    间>   2021-09-22
  **************************************************/
bool CSerialPortCom::IsValid()
{
    //上层通过连接状态来判断连接是否有效
    if(m_nconnectStatus == NO_CONNECT)
    {
        return false;
    }
    return true;
}

/*************************************************
  <函数名称>    open
  <功能描述>    打开串口
  <参数说明>    无

  <返 回 值>    true 成功
               flase 失败
  <函数说明>    打开串口，创建数据接收信号槽，开启串口状态检测

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
bool CSerialPortCom::open()
{
    //若设备处于开启状态不重复开启
    if(m_pSerialPort != nullptr)
    {
        if(m_pSerialPort->isOpen())
        {
            return true;
        }
    }

    bool bRet = false;

    //是否创建了串口操作对象
    if(m_pSerialPort == nullptr)
        return false;

    //设置串口通讯参数
    //波特率
    bRet = m_pSerialPort->setBaudRate(m_baudRate);
    //数据位
    bRet &= m_pSerialPort->setDataBits(m_dataBits);
    //校验位
    bRet &= m_pSerialPort->setParity(m_parity);
    //停止位
    bRet &= m_pSerialPort->setStopBits(m_stopBits);

    if(!bRet)
    {
        QString errLog = "打开失败串口波特率:" + QString(m_baudRate) + "数据位：" + QString(m_dataBits);
        return false;
    }

    //打开串口
    bRet = m_pSerialPort->open(QIODevice::OpenMode(m_iOpenMode));

    if(bRet)
    {
        //保存串口打开状态
        m_bOpened = m_pSerialPort->isOpen();

        //若循环队列小于规定的长度，那么重新申请
        if(m_pReceiveQueue != nullptr)
        {
            if(m_pReceiveQueue->getSizeInQueue() < m_lReceiveQueDataNum)
            {
                delete m_pReceiveQueue;
                m_pReceiveQueue = nullptr;
            }
        }

        if(m_pReceiveQueue == nullptr)
        {
            m_pReceiveQueue = new CCycleQueue<unsigned char>(m_lReceiveQueDataNum);
        }

        //建立信号槽
        connect(m_pSerialPort, &QSerialPort::readyRead, this, &CSerialPortCom::onReceivedData);
        connect(m_pSerialPort, &QSerialPort::errorOccurred, this, &CSerialPortCom::onSerialPortError);

        //启动计时器,进行串口信息发送机制建立
        m_timerSendCommand.start(m_iTimerInterval);
    }

    return  bRet;
}

/*************************************************
  <函数名称>    close
  <功能描述>    关闭串口
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::close()
{
    if(m_bOpened)
    {
        m_timerSendCommand.stop();

        if(m_pSerialPort != nullptr)
        {
            m_pSerialPort->close();
        }

        disconnect(m_pSerialPort, &QSerialPort::readyRead, this, &CSerialPortCom::onReceivedData);
        disconnect(m_pSerialPort, &QSerialPort::errorOccurred, this, &CSerialPortCom::onSerialPortError);

        m_bOpened = false;

        //等待定时器逻辑运行完成
        QEventLoop loop;//定义一个新的事件循环
        QTimer::singleShot(200, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
        loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
    }
}

/*************************************************
  <函数名称>    reset
  <功能描述>    重置串口
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
bool CSerialPortCom::reset()
{
    bool bRet = false;

    if(m_bOpened && m_pSerialPort != nullptr)
    {
        bRet = m_pSerialPort->reset();
    }

    return bRet;
}

/*************************************************
  <函数名称>    IsOpened
  <功能描述>    判定是否已经打开
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
bool CSerialPortCom::IsOpened()
{
    return m_bOpened;
}

/*************************************************
  <函数名称>    getDataSize
  <功能描述>    获取数据大小的字节数
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
unsigned long CSerialPortCom::getDataSize()
{
    unsigned long lDataSize = 0;
    if(m_pReceiveQueue != nullptr)
    {
        lDataSize = m_pReceiveQueue->getSizeInQueue();
    }

    return lDataSize;
}

/*************************************************
  <函数名称>    clearData
  <功能描述>    清空接收到的数据
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::clearData()
{
    if(m_pReceiveQueue != nullptr)
    {
        m_pReceiveQueue->clearQueue();
    }
}

unsigned int CSerialPortCom::readData(unsigned int length, void *pData)
{
    unsigned int lReadLen = 0;
    if(m_pReceiveQueue != nullptr)
    {
        lReadLen = m_pReceiveQueue->read(length, static_cast<unsigned char *>(pData));
    }

    return lReadLen;
}

/*************************************************
  <函数名称>    sendData
  <功能描述>    向硬件发送数据
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
unsigned int CSerialPortCom::sendData(unsigned int length, void *pData, unsigned int uiCommandLevel/* = 0*/, long long llStartTime/* = 0*/)
{
    QByteArray pa(static_cast<char *>(pData), static_cast<int>(length));
    return sendData(pa,uiCommandLevel,llStartTime);
}

unsigned int CSerialPortCom::sendData(const QByteArray &array,unsigned int uiCommandLevel/* = 0*/, long long llStartTime/* = 0*/)
{
    if(!m_bUseQueue)
    {
        if(m_pSerialPort == nullptr)
        {
            return 0;
        }

        unsigned int lRet = 0;

        if (array.size() == 0)
        {
            return 0;
        }

        if ((m_pSerialPort->isOpen() && m_pSerialPort->isWritable()))
        {
            lRet = static_cast<unsigned int>(m_pSerialPort->write(array));
            m_pSerialPort->flush();
        }

        return lRet;
    }


    unsigned int uiLength = static_cast<unsigned int>(array.size());

    if (uiLength == 0)
    {
        return uiLength;
    }

    //命令通过等级划分
    //跳绳广播命令
    if(uiCommandLevel == COMMAND_LEVEL_2)
    {
        m_queueSkipCommand.push(array);
        if(llStartTime > 0)
        {
            m_llStartTime = llStartTime;
        }
    }
    //设备命令
    else if(uiCommandLevel == COMMAND_LEVEL_1)
    {
        m_queueDeviceCommand.push(array);
    }
    //心跳命令
    else
    {
        m_queueHeartCommand.push(array);
    }

    //若计时器没有启动，则启动计时器
    if(!m_timerSendCommand.isActive())
    {
        m_timerSendCommand.start(m_iTimerInterval);
    }

    return uiLength;
}

unsigned int CSerialPortCom::stortDataToReceQueue(void *pData, unsigned int length)
{
    if(m_pReceiveQueue == nullptr)
        return 0;

    return m_pReceiveQueue->write(length, static_cast<unsigned char *>(pData));
}

void CSerialPortCom::onSendCommandTimeOut()
{
    //0.指针为空直接返回
    if(m_pSerialPort == nullptr || !m_bOpened)
        return;

    QByteArray array;
    //1.最优先处理跳绳广播命令
    if(!m_queueSkipCommand.empty())
    {
        array = m_queueSkipCommand.front();
        m_queueSkipCommand.pop();

        //若心跳数据不为空，则定义为已经处理了一条(此处理待验证)
        if(!m_queueHeartCommand.empty())
        {
            m_queueHeartCommand.pop();
        }

        //获取当前时间，计算出下发倒计时时间差
        unsigned short usTime = 0;
        long long  llTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        if((llTime - m_llStartTime) < m_usWaitTime)
        {
            usTime = m_usWaitTime - static_cast<unsigned short>(llTime - m_llStartTime);
        }

        qDebug()<<"### Broadcast Start and Time is "<<usTime;

        //安全保护
        if(usTime <= 100)
        {
            return ;
        }

        //赋值倒计时时间
        unsigned char *pDestData = reinterpret_cast<unsigned char *>(array.data());
        memcpy(pDestData + 40, &usTime, sizeof(unsigned short));
    }
    //2.优先处理设备命令
    else if(!m_queueDeviceCommand.empty())
    {
        array = m_queueDeviceCommand.front();
        m_queueDeviceCommand.pop();

        //若心跳数据不为空，则定义为已经处理了一条(此处理待验证)
        if(!m_queueHeartCommand.empty())
        {
            m_queueHeartCommand.pop();
        }
    }
    //3.其次处理心跳命令
    else if(!m_queueHeartCommand.empty())
    {
        array = m_queueHeartCommand.front();
        m_queueHeartCommand.pop();
    }

    if(array.size() > 0)
    {
        //4.下发命令
        if ((m_pSerialPort->isOpen() && m_pSerialPort->isWritable()))
        {
            m_pSerialPort->write(array);
            m_pSerialPort->flush();
        }
    }
}

void CSerialPortCom::setDTR(bool bState)
{
    if(m_pSerialPort != nullptr && m_pSerialPort->isOpen())
        m_pSerialPort->setDataTerminalReady(bState);
}

/*************************************************
  <函数名称>    setOpenMode
  <功能描述>    设置串口打开的模式
  <参数说明>    readonly
               writeonly
               readwrite

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::setOpenMode(OpenMode mode)
{
    if(mode == OpenMode::WriteOnly)
    {
        m_iOpenMode = QIODevice::WriteOnly;
    }
    else if(mode == OpenMode::ReadWrite)
    {
        m_iOpenMode = QIODevice::ReadWrite;
    }
    else
    {
        m_iOpenMode = QIODevice::ReadOnly;
    }
}


QString CSerialPortCom::getComName()
{
    QString qsName;
    if (m_pSerialPort != nullptr)
    {
        qsName = m_pSerialPort->portName();
    }

    return qsName;
}

void CSerialPortCom::setComName(QString qsComname)
{
    if (m_pSerialPort != nullptr)
    {
        m_pSerialPort->setPortName(qsComname);
    }

}

/*************************************************
  <函数名称>    setPortBaudRate
  <功能描述>    设置波特率
  <参数说明>    波特率

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::setPortBaudRate(QSerialPort::BaudRate baudRate /*= QSerialPort::BaudRate::Baud115200*/)
{
    m_baudRate = baudRate;
}

/*************************************************
  <函数名称>    setPortDataBits
  <功能描述>    设置数据位
  <参数说明>    dataBits

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::setPortDataBits(QSerialPort::DataBits dataBits)
{
    m_dataBits = dataBits;
}

/*************************************************
  <函数名称>    setPortParity
  <功能描述>    设置校验位
  <参数说明>    parity

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::setPortParity(QSerialPort::Parity parity)
{
    m_parity = parity;
}

/*************************************************
  <函数名称>    setPortStopBits
  <功能描述>    设置停止位
  <参数说明>    stopBits

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::setPortStopBits(QSerialPort::StopBits stopBits)
{
    m_stopBits = stopBits;
}

/*************************************************
  <函数名称>    onReceivedData
  <功能描述>    数据接收信号槽
  <参数说明>    无，用 QByteArray缓存

  <返 回 值>    无

  <函数说明>    无

  <作    者>   cpc
  <时    间>   2021-09-16
  **************************************************/
void CSerialPortCom::onReceivedData()
{
    if(m_pSerialPort == nullptr)
    {
        LOGERROR("m_pSerialPort == nullptr");
        return;
    }

    QByteArray data = m_pSerialPort->readAll();

    //将数据放到接收队列即可
    unsigned int iSrcLen = static_cast<unsigned int>(data.length());

    if(iSrcLen == 0)
        return;

    unsigned int iWriteLen = stortDataToReceQueue(data.data(), iSrcLen);

    if(iSrcLen != iWriteLen)
    {
        //错误打印数据写入出错
        LOGERROR("the cache is full of lost data byte = :" + to_string(iSrcLen - iWriteLen));
    }
}

/*************************************************
  <函数名称>    onSerialPortError
  <功能描述>    错误处理信号槽
  <参数说明>    数据位

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::onSerialPortError(QSerialPort::SerialPortError error)
{
    QString strError;

    if(error != QSerialPort::NoError)
    {
        strError = "Error Occur";

        //串口出错之后不能调用关闭函数, 发送信号通知外部处理即可，暂时屏蔽
        //close();
        m_lLastErr = error;

        emit signalErrOccurred();
        //return;
        //emit reportPortErrorSignal(m_serialPortInfo.description(), strError);
    }

    //TODO:记录出错日志
//    if(error == QSerialPort::TimeoutError)
//    {
//        strError = "Timeout Error";
//    }
//    else if (error == QSerialPort::DeviceNotFoundError)
//    {
//        strError = "Device Not Found Error";
//    }
//    else if (error == QSerialPort::UnknownError)
//    {
//        strError = "Unknown Error";
//    }
//    else
//    {
//        //close();
//    }
}


/*************************************************
  <函数名称>    initialize
  <功能描述>    初始化
  <参数说明>

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::initialize()
{
    m_pSerialPort = new QSerialPort();
    m_pSerialPort->setPortName(m_serialPortInfo.portName());

    //连接定时器的信息
    connect(&m_timerSendCommand, &QTimer::timeout, this, &CSerialPortCom::onSendCommandTimeOut);
}

/*************************************************
  <函数名称>    uninitialize
  <功能描述>    释放资源，重置属性
  <参数说明>    数据位

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-30
  **************************************************/
void CSerialPortCom::uninitialize()
{
    disconnect(&m_timerSendCommand, &QTimer::timeout, this, &CSerialPortCom::onSendCommandTimeOut);

    if(m_pSerialPort != nullptr)
    {
        m_pSerialPort->close();

        delete m_pSerialPort;
        m_pSerialPort = nullptr;
    }

    if (m_pReceiveQueue != nullptr)
    {
        delete m_pReceiveQueue;
        m_pReceiveQueue = nullptr;
    }
}
