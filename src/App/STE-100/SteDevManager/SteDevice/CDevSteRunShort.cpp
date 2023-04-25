#include "CDevSteRunShort.h"
#include "SteSubElement/CSubElementRunShort.h"

//设备类型(1字节)
//const short DEVICE_TYPE = 0;
//数据点数(1字节)
//const short DATA_POINTS = 1;
//身份码(4字节)
const short ID_CODE = 2;
//数据类型(2字节)
//const short DATA_TYPE = 6;
//数据状态(2字节)
//const short DATA_STATUS = 8;
//时间戳(8字节)
const short TIME_YEAR = 10;
const short TIME_MONTH = 12;
const short TIME_DAY = 13;
const short TIME_HOUR = 14;
const short TIME_MINUTE = 15;
const short TIME_SECOND = 16;
//const short TIME_OTHER = 17;
//短跑时间(4字节)
const short RUN_TIME = 18;
//跑道ID(2字节)
const short RUN_ID = 22;
//跑道总数(2字节)
const short RUN_TOTAL = 24;
//字节标志(2字节)
const short RUN_FLAG = 26;

CDevSteRunShort::CDevSteRunShort(unsigned int iDevID, ICommunicate &communicate)
    :CDevHardwareBase (iDevID, communicate)
{
    m_shDevType = DEV_TYPE_RUN_SHORT;
    m_bSubElementIDIsDevID = false;

    initSubElement<CSubElementRunShort>(SubElementPropertye::ELEMENT_NUM_Run_Short);
    initDevInfo();
}

CDevSteRunShort::~CDevSteRunShort()
{

}

int CDevSteRunShort::getConnectSubElementNum()
{
    int iNum = 0;

    for (unsigned int i = 0; i < m_vecSubElement.size(); i++)
    {
        if(m_vecSubElement[i]->getConnectFlag())
        {
            iNum++;
        }
    }

    return iNum;
}

void CDevSteRunShort::setUseStartFlag(bool bFlag)
{
    m_bUseStartFlag = bFlag;

    CSubElementRunShort *pElemet = nullptr;
    for (unsigned int i = 0; i < m_vecSubElement.size(); i++)
    {
        pElemet = reinterpret_cast<CSubElementRunShort *>(m_vecSubElement[i]);

        if(nullptr != pElemet)
        {
            pElemet->setUseStartFlag(bFlag);
        }
    }
}

bool CDevSteRunShort::getStartTime()
{
    bool bRes = false;

    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdGetTimeStamp(arr);

    //理论上不需要等待,目前等待是为了保证得到时间
    bRes = sendData(arr,true,200,COMMAND_LEVEL_1);

    qDebug()<<"### Get Start Time is : " << m_fRunShortStartTime;
    LOGINFO("### Get Start Time is : " + to_string(m_fRunShortStartTime));

    //如果时间为0，也定义为失败
    if(m_fRunShortStartTime <= 0.0f)
    {
        bRes = false;
    }

    return bRes;
}

bool CDevSteRunShort::setStartTime(unsigned int uiDelayTime)
{
    //赋值延时时间
    m_fRunShortStartTime -= uiDelayTime;

    //将时间设置为所有设备的启动时间
    bool bRes = true;

    CSubElementRunShort *pElemet = nullptr;
    for (unsigned int i = 0; i < m_vecSubElement.size(); i++)
    {
        pElemet = reinterpret_cast<CSubElementRunShort *>(m_vecSubElement[i]);

        if(nullptr != pElemet && pElemet->getBindFlag())
        {
            pElemet->setStartTime(m_fRunShortStartTime);
        }
    }

    return bRes;
}

bool CDevSteRunShort::setStartTime(unsigned short usElementIndex, unsigned int uiDelayTime)
{
    //赋值延时时间
    m_fRunShortStartTime -= uiDelayTime;

    //将时间设置为所有设备的启动时间
    bool bRes = true;

    if(usElementIndex < m_vecSubElement.size())
    {
        CSubElementRunShort *pElemet = nullptr;
        pElemet = reinterpret_cast<CSubElementRunShort *>(m_vecSubElement[usElementIndex]);
        if(nullptr != pElemet)
        {
            pElemet->setStartTime(m_fRunShortStartTime);
        }
    }

    return bRes;
}

bool CDevSteRunShort::existErrInBindElement()
{
    return /*CDevHardwareBase::existErrInBindElement()*/false;
}

bool CDevSteRunShort::startWork(bool bCast, bool bWait)
{
    //开启所有绑定设备的测试
    bool bRet = false;
    CSubElementBase *pElemet = nullptr;
    for (unsigned int i = 0; i < m_vecSubElement.size(); i++)
    {
        pElemet = m_vecSubElement[i];

        if(nullptr != pElemet && pElemet->getBindFlag())
        {
            pElemet->startTest();

            bRet = true;
        }
    }

    return bRet;
}

bool CDevSteRunShort::stopWork(bool bCast, bool bWait)
{
    //关闭所有绑定设备的测试
    bool bRet = false;
    CSubElementBase *pElemet = nullptr;
    for (unsigned int i = 0; i < m_vecSubElement.size(); i++)
    {
        pElemet = m_vecSubElement[i];

        if(nullptr != pElemet && pElemet->getBindFlag())
        {
            pElemet->stopTest();

            bRet = true;
        }
    }

    return bRet;
}

bool CDevSteRunShort::startWork(unsigned short shElementIndex, bool bWait)
{
    auto elementSize = m_vecSubElement.size();
    if(shElementIndex >= elementSize || elementSize == 0)
    {
        LOGWARN("shelement : " + to_string(shElementIndex) + " element size: " + to_string(elementSize));
        return false;
    }

    CSubElementBase *pElemet = m_vecSubElement[shElementIndex];

    if(nullptr == pElemet)
    {
        LOGERROR("invalid subelement");
        return false;
    }

    pElemet->startTest();

    return true;
}

bool CDevSteRunShort::stopWork(unsigned short shElementIndex, bool bWait)
{
    auto elementSize = m_vecSubElement.size();
    if(shElementIndex >= elementSize || elementSize == 0)
    {
        LOGWARN("shelement : " + to_string(shElementIndex) + " element size: " + to_string(elementSize));
        return false;
    }

    CSubElementBase *pElemet = m_vecSubElement[shElementIndex];

    if(nullptr == pElemet)
    {
        LOGERROR("invalid SubElement");
        return false;
    }

    pElemet->stopTest();

    return true;
}
/*************************************************
  <函数名称>    parseSinglePackage(int iStartPos, QByteArray &array)
  <功能描述>    在独立通讯模式下的单点数据解析（需要沿用父类的通用处理）
  <参数说明>    int iStartPos 数据开始位置
               QByteArray &array 单点数据内容

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2022-02-22
  <满足形式>    地毯式
  **************************************************/
/*
int CDevSteRunShort::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    //0.安全保护
    int nLen = CDevHardwareBase::parseSinglePackage(iFrameNo, pData, iDatalen);
    if(nLen > 0)
        return nLen;

    //检测数据有效性
    if(!checkSignalPackageValid(pData, iDatalen))
        return iDatalen;

    //1.数据帧ID设置
    if(!m_bValidDev)
    {
        //数据帧中的设备id才是当前设备的id，此时才算设备连接上
        emit signalDevStatus(true, this);
        m_bValidDev = true;
    }

    //2.构建具体跑道
    //unsigned char  ucDeviceType;
    //unsigned char  ucDataPoints;
    unsigned int   uiIDCode;
    //unsigned short usDataType;
    //unsigned short usDataStatus;
    unsigned short usYear;
    unsigned char  ucMonth;
    unsigned char  ucDay;
    unsigned char  ucHour;
    unsigned char  ucMinute;
    unsigned char  ucSecond;
    float fRunTime;
    unsigned short usRunID;
    unsigned short usRunTotal;
    unsigned short usRunFlag;

    //2.0 解析设备类型
    //memcpy_s(&ucDeviceType, sizeof(unsigned char), pData + DEVICE_TYPE, sizeof(unsigned char));

    //2.1 解析数据点数
    //memcpy_s(&ucDataPoints, sizeof(unsigned char), pData + DATA_POINTS, sizeof(unsigned char));

    //2.2 解析身份码
    memcpy_s(&uiIDCode, sizeof(unsigned int), pData + ID_CODE, sizeof(unsigned int));

    //2.3 解析数据类型
    //memcpy_s(&usDataType, sizeof(unsigned short), pData + DATA_TYPE, sizeof(unsigned short));

    //2.4 解析数据状态
    //memcpy_s(&usDataStatus, sizeof(unsigned short), pData + DATA_STATUS, sizeof(unsigned short));

    //2.5 解析时间戳
    memcpy_s(&usYear, sizeof(unsigned short), pData + TIME_YEAR, sizeof(unsigned short));
    memcpy_s(&ucMonth, sizeof(unsigned char), pData + TIME_MONTH, sizeof(unsigned char));
    memcpy_s(&ucDay, sizeof(unsigned char), pData + TIME_DAY, sizeof(unsigned char));
    memcpy_s(&ucHour, sizeof(unsigned char), pData + TIME_HOUR, sizeof(unsigned char));
    memcpy_s(&ucMinute, sizeof(unsigned char), pData + TIME_MINUTE, sizeof(unsigned char));
    memcpy_s(&ucSecond, sizeof(unsigned char), pData + TIME_SECOND, sizeof(unsigned char));

    //2.6 解析短跑时间
    memcpy_s(&fRunTime, sizeof(float), pData + RUN_TIME, sizeof(float));

    //2.7 解析跑道ID
    memcpy_s(&usRunID, sizeof(unsigned short), pData + RUN_ID, sizeof(unsigned short));

    //2.8 解析跑道总数
    memcpy_s(&usRunTotal, sizeof(unsigned short), pData + RUN_TOTAL, sizeof(unsigned short));

    //2.9 解析字节标志
    memcpy_s(&usRunFlag, sizeof(unsigned short), pData + RUN_FLAG, sizeof(unsigned short));

    //2.10 根据跑道号总数，激活对应的跑道
    //注：与硬件沟通不会出现中途不可用
    if(usRunTotal != m_usRunwayTotalNum)
    {
        LOGINFO("Valid RunTotal changed from " + to_string(m_usRunwayTotalNum) + " to " + to_string(usRunTotal));
        m_usRunwayTotalNum = usRunTotal;
        for(unsigned int i = 0; i < ELEMENT_NUM_Run_Short; i++)
        {
            if(i < m_usRunwayTotalNum)
            {
                m_vecSubElement[i]->setConnectStatus(1);
            }
            else
            {
                m_vecSubElement[i]->setConnectStatus(0);
            }
        }
    }

    if(usRunID >= m_usRunwayTotalNum)
    {
        LOGERROR("Runway Total is : " + to_string(m_usRunwayTotalNum) + " Invalid Runway ID : " + to_string(usRunID));
        return m_iPackgeLen;
    }

    if(usRunID >= SubElementPropertye::ELEMENT_NUM_Run_Short)
    {
        LOGERROR("Runway Total is : " + to_string(m_usRunwayTotalNum) + " Invalid Runway ID : " + to_string(usRunID));
        return m_iPackgeLen;
    }

    DEV_RUNWAY_INFO structRunway;
    structRunway.uiIDCode = uiIDCode;
    structRunway.usYear = usYear;
    structRunway.ucMonth = ucMonth;
    structRunway.ucDay = ucDay;
    structRunway.ucHour = ucHour;
    structRunway.ucMinute = ucMinute;
    structRunway.ucSecond = ucSecond;
    structRunway.fRunTime = fRunTime;
    if(usRunFlag == 0x0001)
    {
        structRunway.bEndFlag = true;
    }
    else if(usRunFlag == 0x0000)
    {
        structRunway.bEndFlag = false;
    }
    else
    {
        LOGERROR("Error Runway ID is : " + to_string(usRunID) + " Error Run Flag is : " + to_string(usRunFlag));
        return m_iPackgeLen;
    }

    //2.11 传递解析信息
    m_vecSubElement[usRunID]->setElementInfo(static_cast<void*>(&structRunway),0);

    return m_iPackgeLen;
}
*/

/*************************************************
  <函数名称>    parseSinglePackage(int iStartPos, QByteArray &array)
  <功能描述>    在独立通讯模式下的单点数据解析（需要沿用父类的通用处理）
  <参数说明>    int iStartPos 数据开始位置
               QByteArray &array 单点数据内容

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2022-02-22
  <满足形式>    柱状式
  **************************************************/
int CDevSteRunShort::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    //0.安全保护
    int nLen = CDevHardwareBase::parseSinglePackage(iFrameNo, pData, iDatalen);
    if(nLen > 0)
        return nLen;

    //检测数据有效性
    if(!checkSignalPackageValid(pData, iDatalen))
        return iDatalen;

    //1.数据帧ID设置
    if(!m_bValidDev)
    {
        //数据帧中的设备id才是当前设备的id，此时才算设备连接上
        emit signalDevStatus(true, this);
        m_bValidDev = true;
    }

    //2.构建具体跑道
    //unsigned char  ucDeviceType;
    //unsigned char  ucDataPoints;
    unsigned int   uiIDCode;
    //unsigned short usDataType;
    //unsigned short usDataStatus;
    unsigned short usYear;
    unsigned char  ucMonth;
    unsigned char  ucDay;
    unsigned char  ucHour;
    unsigned char  ucMinute;
    unsigned char  ucSecond;
    float fRunTime;
    unsigned short usRunID;
    //unsigned short usRunTotal;
    unsigned short usRunFlag;

    //2.0 解析设备类型
    //memcpy_s(&ucDeviceType, sizeof(unsigned char), pData + DEVICE_TYPE, sizeof(unsigned char));

    //2.1 解析数据点数
    //memcpy_s(&ucDataPoints, sizeof(unsigned char), pData + DATA_POINTS, sizeof(unsigned char));

    //2.2 解析身份码
    memcpy_s(&uiIDCode, sizeof(unsigned int), pData + ID_CODE, sizeof(unsigned int));

    //2.3 解析数据类型
    //memcpy_s(&usDataType, sizeof(unsigned short), pData + DATA_TYPE, sizeof(unsigned short));

    //2.4 解析数据状态
    //memcpy_s(&usDataStatus, sizeof(unsigned short), pData + DATA_STATUS, sizeof(unsigned short));

    //2.5 解析时间戳
    memcpy_s(&usYear, sizeof(unsigned short), pData + TIME_YEAR, sizeof(unsigned short));
    memcpy_s(&ucMonth, sizeof(unsigned char), pData + TIME_MONTH, sizeof(unsigned char));
    memcpy_s(&ucDay, sizeof(unsigned char), pData + TIME_DAY, sizeof(unsigned char));
    memcpy_s(&ucHour, sizeof(unsigned char), pData + TIME_HOUR, sizeof(unsigned char));
    memcpy_s(&ucMinute, sizeof(unsigned char), pData + TIME_MINUTE, sizeof(unsigned char));
    memcpy_s(&ucSecond, sizeof(unsigned char), pData + TIME_SECOND, sizeof(unsigned char));

    //2.6 解析短跑时间
    memcpy_s(&fRunTime, sizeof(float), pData + RUN_TIME, sizeof(float));

    //2.7 解析跑道ID
    memcpy_s(&usRunID, sizeof(unsigned short), pData + RUN_ID, sizeof(unsigned short));

    //2.8 解析跑道总数
    //memcpy_s(&usRunTotal, sizeof(unsigned short), pData + RUN_TOTAL, sizeof(unsigned short));

    //2.9 解析字节标志
    memcpy_s(&usRunFlag, sizeof(unsigned short), pData + RUN_FLAG, sizeof(unsigned short));

    //2.10 判断跑道ID是否合规
    if(usRunID >= SubElementPropertye::ELEMENT_NUM_Run_Short)
    {
        LOGERROR("Runway Total is : " + to_string(m_usRunwayTotalNum) + " Invalid Runway ID : " + to_string(usRunID));
        return m_iPackgeLen;
    }

    DEV_RUNWAY_INFO structRunway;
    structRunway.uiIDCode = uiIDCode;
    structRunway.usYear = usYear;
    structRunway.ucMonth = ucMonth;
    structRunway.ucDay = ucDay;
    structRunway.ucHour = ucHour;
    structRunway.ucMinute = ucMinute;
    structRunway.ucSecond = ucSecond;
    structRunway.fRunTime = fRunTime;
    if(usRunFlag == 0x0001)
    {
        structRunway.bEndFlag = true;

        m_setReceiveEnd.insert(usRunID);
    }
    else if(usRunFlag == 0x0000)
    {
        structRunway.bEndFlag = false;

        m_setReceiveStart.insert(usRunID);
    }
    else
    {
        LOGERROR("Error Runway ID is : " + to_string(usRunID) + " Error Run Flag is : " + to_string(usRunFlag));
        return m_iPackgeLen;
    }

    //2.11 传递解析信息
    m_vecSubElement[usRunID]->setElementInfo(static_cast<void*>(&structRunway),0);

    return m_iPackgeLen;
}

void CDevSteRunShort::onUpdateTimeOut()
{
    //不影响父类逻辑
    CDevHardwareBase::onUpdateTimeOut();

    m_iInsertNum ++ ;

    //每 10s 检测一次 是否存在设备断连
    if(m_iInsertNum > 10 * m_iTargetNum)
    {
        for(unsigned short i = 0; i < SubElementPropertye::ELEMENT_NUM_Run_Short; i++)
        {
            //在使用起点时才判断起点数据
            if(m_bUseStartFlag && m_setReceiveStart.find(i) == m_setReceiveStart.end())
            {
                m_vecSubElement[i]->setConnectStatus(0);
            }
            else if(m_setReceiveEnd.find(i) == m_setReceiveEnd.end())
            {
                m_vecSubElement[i]->setConnectStatus(0);
            }
        }

        m_iInsertNum = 0;
        m_setReceiveStart.clear();
        m_setReceiveEnd.clear();
    }
}

