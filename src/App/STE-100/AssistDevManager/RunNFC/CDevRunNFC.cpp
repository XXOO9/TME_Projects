#include "CDevRunNFC.h"
#include "commondefin.h"
#include "CEnumSerialPort.h"
#include "CLogger.h"

CDevRunNFC::CDevRunNFC()
{
    //2s检测一次是否断连
    m_iNoDataInteval = 1000;
}

CDevRunNFC::~CDevRunNFC()
{
    safeDeleteClassPoint(&m_pSerialPort);
}

void CDevRunNFC::setComName(QString comName)
{
    safeNewClassPoint(&m_pSerialPort);
    if(m_pSerialPort != nullptr)
    {
        m_qsComName = comName;

        cfgComInfo(m_pSerialPort, comName);
        setCommunicate(m_pSerialPort);

        clearData();

        devOpen();

        m_int64LastCardTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

        m_listComName.clear();
    }
}

void CDevRunNFC::cfgComInfo(CSerialPortCom *pCom, const QString &comName)
{
    if(pCom == nullptr)
        return;

    pCom->setComName(comName);
    pCom->setOpenMode(CSerialPortCom::ReadWrite);
    pCom->setPortBaudRate(QSerialPort::Baud38400);
}

QByteArray CDevRunNFC::getCmdDevInfo()
{
    //新版本获取命令 115200波特率
    //unsigned char arr[] = {0x02, 0x03, 0x04, 0x05, 0x00, 0x0B, 0x20, 0xE9, 0xC8, 0x27, 0x13, 0x11, 0x01, 0x01, 0x11};
    //老版本获取命令 38400波特率
    unsigned char arr[] = {0x02, 0x03, 0x04, 0x05, 0x00, 0x0B, 0x00, 0x58, 0x44, 0x00, 0xB5};

    int nSize = sizeof (arr);

    QByteArray arrCmd(reinterpret_cast<char *>(arr), nSize);
    return arrCmd;
}

void CDevRunNFC::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(pData == nullptr || nDataLen == 0)
        return;

    if(nDataLen < m_nParseDataLen)
        return;

    CDataParseRunNFC::PARSE_DATA_RUN_NFC *p = static_cast<CDataParseRunNFC::PARSE_DATA_RUN_NFC *>(pData);

    //避免短时间重复识别
    if(p->nFrameType == CDataParseRunNFC::FRAME_CARD_INFO)
    {
        qint64 timeCur = QDateTime::currentDateTime().toMSecsSinceEpoch();
        if(!(p->nCardID == m_nCardID && timeCur - m_int64LastCardTime < 700))
        {
            m_nCardID = p->nCardID;
            emit signalRunNFCRecognizeCard(m_nCardID);
        }
    }
}

bool CDevRunNFC::detectDisconnected()
{
    //判断当前串口是否存在
    CEnumSerialPort &enumSerialPort = CEnumSerialPort::Get_Instance();
    if(!enumSerialPort.lockUseSerialPortCom(20))
        return false;

    const QStringList &comNameList = enumSerialPort.getSerialPortCom();

    bool bRet = false;
    if(m_listComName != comNameList)
    {
        bRet = !comNameList.contains(m_qsComName);
        m_listComName = comNameList;
    }


    enumSerialPort.unlockUseSerialPortCom();
    return bRet;
}
