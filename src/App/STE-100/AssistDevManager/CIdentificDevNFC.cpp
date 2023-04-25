#include "CIdentificDevNFC.h"
#include "Logger/CLogger.h"
#include "commondefin.h"
#include <string>
#include <QTime>
#include <QDebug>
using namespace std;

typedef unsigned char  byte;
static int dt;

CIdentificDevNFC::CIdentificDevNFC()
{
    //根据选项存在 115200 57600 38400 19200 9600
    connect(this, &CIdentificDevNFC::signalOpen, this, &CIdentificDevNFC::onOpen);
    connect(this, &CIdentificDevNFC::signalClose, this, &CIdentificDevNFC::onClose);
}

CIdentificDevNFC::~CIdentificDevNFC()
{
    disconnect(this, &CIdentificDevNFC::signalOpen, this, &CIdentificDevNFC::onOpen);
    disconnect(this, &CIdentificDevNFC::signalClose, this, &CIdentificDevNFC::onClose);

    close();

    safeDeleteClassPoint(&m_pTimer);
}

void CIdentificDevNFC::setInterfaceMode(short shMode)
{
    if(!m_bDevOpenStatus)
    {
        m_shInterfaceMode = shMode;
    }
}

void CIdentificDevNFC::onOpen(bool bAutoList)
{   
    startTimerCustom();

    if(!m_bDevOpenStatus)
    {
        int nMuchhid = 0, nNowId = 0;
        int nRet = FAIL;

        if(m_shInterfaceMode == Interface_HID)
        {
            nMuchhid = listhid(0x0483, 0x5750);
            if(nMuchhid > 0)
            {
                nNowId = 1;
                nRet = openhid(0x0483, 0x5750, 1);
            }
        }
        else if(m_shInterfaceMode == Interface_PCSC)
        {
            nRet = openpcsc(0, nullptr);
        }
        else
        {
            //暂不实现
        }

        if(nRet == OK)
        {
            unsigned char addr,pBuf[2048];
            string strRes;
            char cTemp;
            nRet = GetReaderUID(&addr, pBuf);

            //获取设备UID
            if(nRet==OK)
            {
                for(int i=0; i < addr; i++)
                {
                    sprintf(&cTemp, "%02x", pBuf[i]);
                    strRes+=cTemp;
                }
                m_bDevOpenStatus = true;
            }

            //设备断连判断时间
            timeoutCom(1000);
        }
        else
        {
            if(m_bFirstReconnectFlag)
            {
                LOGERROR("open identific dev false");
            }
        }

        //暂不管多个设备
        if(nMuchhid > 1)
        {

        }
    }

    //开启自动寻卡 条件为 设备开启 自动寻卡未开启 传入标志为开启自动寻卡
    if(m_bDevOpenStatus && bAutoList)
    {
        startAutoListCard();
    }

    //成功打开通知外部
    if(m_bDevOpenStatus)
    {
        emit signalConnectChange(true);
        m_bReadCardState = true;
    }
}

void CIdentificDevNFC::onClose()
{
    if(m_pTimer != nullptr && m_pTimer->isActive())
    {
        m_pTimer->stop();
    }

    //关闭NFC设备
    if(m_shInterfaceMode == Interface_HID)
    {
        closehid();
    }
    else if(m_shInterfaceMode == Interface_PCSC)
    {
        closepcsc();
    }
    else
    {
        //暂不实现
    }

    m_bDevOpenStatus = false;
}

void CIdentificDevNFC::onTimeOut()
{
    //自动寻卡
    if(m_bReadCardState)
    {
        READ_CARDID = 1;
        m_nReadCardTotal += m_nUpdateTimeOut;
        if(m_nReadCardTotal >= m_nReadCardInterval)
        {
            readCard();
            m_nReadCardTotal -= m_nReadCardInterval;
        }
    }
    //进行重连逻辑
    else
    {
        READ_CARDID = 0;
        m_nReconnectTotal += m_nUpdateTimeOut;
        if(m_nReconnectTotal >= m_nReconnectInterval)
        {
            onOpen(true);
            m_bFirstReconnectFlag = false;
            m_nReconnectTotal -= m_nReconnectInterval;
        }
    }
}

bool CIdentificDevNFC::startAutoListCard()
{
    unsigned char ISO14443A(1),ISO15693(0),ISO14443B(0),CID(0),CIDInfor(0),ISO7816(0);
    
    bool bAutoFindCard = ISO14443A||ISO14443B|ISO15693||CID||CIDInfor||ISO7816;

    if(bAutoFindCard)
    {
        m_bReadCardState = true;
    }
    
    return bAutoFindCard;
}

bool CIdentificDevNFC::buzzing()
{
    int nRet = SetBuzzer(0x05,0x01);

    return nRet == OK;
}


static int TLret = 1;
static byte tempbuff[2048];
static byte snlength = 0;
static byte isNewReader;
static unsigned char adder = 0;
static unsigned int relen;
static unsigned int ISO14443A,ISO14443B,ISO15693,CID,CIDInfor,ISO7816;

void CIdentificDevNFC::open(bool bAutoList)
{
    emit signalOpen(bAutoList);
}

void CIdentificDevNFC::close()
{
    emit signalClose();
}

void CIdentificDevNFC::readCard()
{
    if(INTANSFER != 0)
    {
        INTANSFER_();
    }
    
    if(READ_CARDID == 1)
    {
        READ_CARDID_();
    }

    //说明刷卡设备断连需要启用定时器
    if(READ_CARDID == 0)
    {
        m_bReadCardState = false;

        if(m_bDevOpenStatus)
        {
            m_bFirstReconnectFlag = true;
            signalConnectChange(false);
            m_bDevOpenStatus = false;
        }
    }
    
    if(READ_QUEUE==1)
    {
        READ_QUEUE_();
    }
}

void CIdentificDevNFC::INTANSFER_()
{
    //-------------------------------------------------------------------------------------------------
    if(INTANSFER == 1)
    {
        QTime current_Time = QTime::currentTime();
        dt = current_Time.msecsTo(current_Time);
        
        //IN TRANS,PLEASE WAIT......!

        //参照源码中的打开文件，未找到暂时屏蔽
        return;
    }
    //-------------------------------------------------------------------------------------------------
    if(INTANSFER == 3)
    {
    }
}

void CIdentificDevNFC::READ_CARDID_()
{
    //
    ISO14443A = 1;
    ISO15693  = 0;
    CID = 0;
    CIDInfor  = 0;
    ISO7816  = 0;
    ISO14443B  = 1;
    //
    adder++;
    string temp = "\r\n";
    string temp1;
    char cTemp, cTemp1[2];
    sprintf(&cTemp, "%02x", adder);
    temp += cTemp;

    bool bFindCard = false;
    string strCardID("");

    //附属类型
    int RATS = 0, APDU = 0;

    //get softVS
    TLret = GetSoftwareVS(&tempbuff[0], &tempbuff[1]);
    if(TLret==OK)
    {
        isNewReader = 0;
        temp+= "----------------------------------------------------\r\nSuccessed!";
        temp1 = "\r\nSoftware VS:";
        temp+=temp1;
        for(int i=0;i < tempbuff[0];i++)
        {
            sprintf(&cTemp, "%02x", tempbuff[1+i]);
            temp+=temp1;
        }
    }
    if( (TLret==READTIMEOUT)||(TLret==WRITECOMMFAIL) )
    {
        temp = "\r\n^----------------------------------------------------\r\nTimeOut Stop!";
        //自动寻卡结束
        READ_CARDID = 0;
        return;
    }
    //
    if(ISO14443A)
    {
        TLret = PiccActivateA(0x0A,0x52,&tempbuff[0],&tempbuff[2],&tempbuff[3],&tempbuff[4]) ;
        if(TLret==OK)
        {
            temp = "\r\n^----------------------------------------------------\r\nSuccessed!";
            temp1 = "\r\nATQ:";
            temp+=temp1;
            sprintf(&cTemp, "%02x", tempbuff[0]);
            temp+=cTemp;
            sprintf(&cTemp, "%02x", tempbuff[1]);
            temp+=cTemp;
            temp+="\r\n";
            //
            sprintf(&cTemp, "%02x", tempbuff[2]);
            temp+="SAK:";
            temp+=cTemp;
            temp+="\r\n";
            //
            temp+="UID:";

            unsigned long long nCardID = 0;
            memcpy(&nCardID, &tempbuff[4], tempbuff[3]);

//            for(int i=0;i < tempbuff[3];i++)
//            {
//                sprintf(cTemp1, "%02x", tempbuff[4+i]);
//                temp  = temp + cTemp1;
//                strCardID += cTemp1;
//            }
            strCardID = to_string(nCardID);
            bFindCard = true;

            //
            if(RATS)
            {
                TLret = PiccRequestATS(0,&tempbuff[0],&tempbuff[1]);
                if(TLret==OK)
                {
                    temp+="\r\nATS:";
                    for(int i=0;i < tempbuff[0];i++)
                    {
                        sprintf(&cTemp, "%02x", tempbuff[1+i]);
                        temp+=temp1;
                    }
                    if(APDU)
                    {
                        TLret = PiccTPCL(5, (unsigned char *)"\x00\x84\x00\x00\x08",&relen,&tempbuff[0]);
                        if(TLret==OK)
                        {
                            temp+="\r\nAPDU:";
                            for(unsigned int i=0;i < relen;i++)
                            {
                                sprintf(&cTemp, "%02x", tempbuff[1]);
                                temp = temp + " " + cTemp;
                            }
                        }
                        else
                        {
                            temp+="\r\nAPDU Fail:";
                        }
                    }
                }
                else
                {
                    temp+="\r\nATS Fail:";
                }
            }
        }
        else if( (TLret==READTIMEOUT)||(TLret==WRITECOMMFAIL) )
        {
            temp = "\r\n^----------------------------------------------------\r\nTimeOut Stop!";
            READ_CARDID = 0;
            return;
        }
        else
        {
            temp = "\r\n^----------------------------------------------------\r\nISO14443A NO Card!";
            temp1 = "\r\nError Code:";
            temp+=temp1;
            sprintf(&cTemp, "%02x", TLret);
            temp+=cTemp;

            strCardID.clear();
        }
    }
    
    //
    if(ISO14443B)
    {
        TLret = PiccActivateB(0x0A,1,0,0,&tempbuff[0],&tempbuff[1]);
        if(TLret==OK)
        {
            temp = "\r\n^----------------------------------------------------\r\nSuccessed!";
            temp1 = "\r\nUID:";
            temp+=temp1;
            for(int i=0;i < tempbuff[0];i++)
            {
                sprintf(cTemp1, "%02x", tempbuff[1+i]);
                temp = temp + cTemp1;

                strCardID += cTemp1;
            }		

            bFindCard = true;
        }
        else if(TLret==READTIMEOUT)
        {
            temp = "\r\n^----------------------------------------------------\r\nTimeOut Stop!";
        }
        else
        {
            temp = "\r\n^----------------------------------------------------\r\nChinese CID NO Card!";
            temp1 = "\r\nError Code:";
            temp+=temp1;
            sprintf(&cTemp, "%02x", TLret);
            temp = temp + " " + cTemp;
        }
    }

    if(strCardID.empty())
    {
        m_strLastCardID.clear();
    }
    else
    {
        if(strCardID != m_strLastCardID)
        {
            buzzing();
            m_strLastCardID = strCardID;
            QString strTemp = QString::fromStdString(m_strLastCardID);

            //查询数据请求
            qDebug() << QString::fromStdString(m_strLastCardID);
//            LOGINFO("========++++++++++++++new card no = " + m_strLastCardID);
            emit signalFindCard(m_strLastCardID);
        }
    }
}

void CIdentificDevNFC::READ_QUEUE_()
{
    string temp, temp1;
    char cTemp;
    TLret = QueueUartData(&snlength,&tempbuff[0]);
    if((TLret==OK)&&(snlength>0))
    {
        //
        temp = "\r\n^----------------------------------------------------\r\nSuccessed!";
        temp1 = "\r\nUart Data:\r\n";
        temp+=temp1;
        for(int j=0;j<snlength;j++)
        {
            sprintf(&cTemp, "%02x", tempbuff[j]);
            temp+=cTemp;
        }	

        QString str;
        str.fromStdString(temp);
        qDebug() << str << endl;
    }
    else
    {
        
    }
}

void CIdentificDevNFC::startTimerCustom()
{
    if(m_pTimer == nullptr)
    {
        m_pTimer = new QTimer();
        connect(m_pTimer, &QTimer::timeout, this, &CIdentificDevNFC::onTimeOut);
    }

    if(!m_pTimer->isActive())
    {
        m_pTimer->start(m_nUpdateTimeOut);
    }
}
