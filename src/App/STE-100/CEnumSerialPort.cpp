#include "CEnumSerialPort.h"

CEnumSerialPort::CEnumSerialPort()
{

}

bool CEnumSerialPort::lockUseSerialPortCom(int nMsec)
{
    return m_mutexUse.tryLock(nMsec);
}

const QStringList &CEnumSerialPort::getSerialPortCom()
{
    m_listComName = m_serialPortHelper.enumComUnits();
    return m_listComName;
}

const QStringList &CEnumSerialPort::getAvailableSerialPortCom()
{
    getSerialPortCom();

    m_listAvailableComName.clear();
    QSerialPortInfo info;
    for(auto &comName : m_listComName)
    {
        info = m_serialPortHelper.getOneSerialPortInfo(comName);

        //如果当前通讯接口正在被读写
        if(info.isBusy())
        {
            continue;
        }

        m_listAvailableComName.append(comName);
    }

    return m_listAvailableComName;
}

void CEnumSerialPort::unlockUseSerialPortCom()
{
    m_mutexUse.unlock();
}
