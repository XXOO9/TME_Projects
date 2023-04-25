#include "CDevStartingGun.h"
#include "CEnumSerialPort.h"
#include "commondefin.h"

CDevStartingGun::CDevStartingGun()
{

}

void CDevStartingGun::setComName(QString comName)
{
    safeNewClassPoint(&m_pSerialPort);
    if(m_pSerialPort != nullptr)
    {
        setDataParse(&m_objDataParse);

        m_qsComName = comName;

        m_pSerialPort->setComName(comName);
        m_pSerialPort->setOpenMode(CSerialPortCom::ReadWrite);
        m_pSerialPort->setPortBaudRate(QSerialPort::Baud9600);

        setCommunicate(m_pSerialPort);

        clearData();

        devOpen();
    }
}

void CDevStartingGun::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    //发令枪解析成功暂时不传输特殊结构体
    if(pData == nullptr || nDataLen == 0)
    {
    }

    //发令枪开始指令
    emit signalStartingGun();
}

bool CDevStartingGun::detectDisconnected()
{
    //判断当前串口是否存在
    CEnumSerialPort &enumSerialPort = CEnumSerialPort::Get_Instance();
    if(!enumSerialPort.lockUseSerialPortCom(20))
        return false;

    const QStringList &comNameList = enumSerialPort.getSerialPortCom();

    bool bRet = !comNameList.contains(m_qsComName);
    enumSerialPort.unlockUseSerialPortCom();

    return bRet;
}
