#include "CConfirmDevBase.h"
#include "commondefin.h"

#include <QDateTime>

CConfirmDevBase::CConfirmDevBase()
{

}

CConfirmDevBase::~CConfirmDevBase()
{

}

void CConfirmDevBase::setReturnConfirmCmd(size_t szConfirmCmd)
{
    m_szReturnConfirmCmd = szConfirmCmd;
}

bool CConfirmDevBase::confirmDev(QByteArray &cmd, int nWaitMsec)
{
    return sendData(cmd, true, nWaitMsec);
}

//和父类的区别在于当前设备生成之后只在一个线程中进行运行，定时器不需要申请多次
bool CConfirmDevBase::devOpen()
{
    if(!m_bOpened)
    {
        if(m_pTimerUpdateInfo == nullptr)
        {
            m_pTimerUpdateInfo = new QTimer();
            connect(m_pTimerUpdateInfo, &QTimer::timeout, this, &CConfirmDevBase::onUpdateTimeOut);
        }

        if(m_pCom != nullptr)
        {
            m_bOpened = m_pCom->open();
            if(m_bOpened)
            {
                m_pTimerUpdateInfo->start(static_cast<int>(m_iUpdateInteval));
            }

        }
    }

    return m_bOpened;
}

void CConfirmDevBase::devClose()
{
    if (m_bOpened)
    {
        m_bOpened = false;
    }

    if(m_pCom != nullptr)
    {
        m_pCom->close();
    }

    if(m_pTimerUpdateInfo != nullptr)
    {
        m_pTimerUpdateInfo->stop();
    }
}

//需要根据当前设备需求将数据序列化为通用数据
void CConfirmDevBase::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(pData == nullptr || nDataLen == 0)
        return;

    //这里规定传过来的为某个具体的命令
    size_t szCmd = 0;
    memcpy(&szCmd, pData, nDataLen);

    m_bSendCommandSucess = szCmd == m_szReturnConfirmCmd;
}

