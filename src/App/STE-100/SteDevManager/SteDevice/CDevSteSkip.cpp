#include "CDevSteSkip.h"
#include "SteSubElement/CSubElementRopeSkip.h"
#include "CDevDataCfg.h"

#include "SteTestGuide/CTestGuideSkip.h"

//设备类型(1字节)
//const short DEVICE_TYPE = 0;
//数据点数(1字节)
//const short DATA_POINTS = 1;
//身份码(4字节)
const short ID_CODE = 2;
//数据类型(2字节)
//const short DATA_TYPE = 6;
//数据状态(1字节)
const short DATA_STATUS = 8;
//绑定标志(1字节)
const short DATA_BIND = 9;
//归零次数(2字节)
const short START_NUMBER = 10;
//跳绳次数(2字节)
const short ROPE_NUMBER = 12;
//充电状态(1字节)
const short CHARGE_STATUS = 14;
//充电电量(1字节)
const short POWER_NUM = 15;

CDevSteSkip::CDevSteSkip(unsigned int iDevID, ICommunicate &communicate)
    :CDevHardwareBase (iDevID, communicate)
{
    m_shDevType = DEV_TYPE_ROPE_SKIP;
    m_bSubElementIDIsDevID = false;

    initSubElement<CSubElementRopeSkip>(SubElementPropertye::ELEMENT_NUM_Skip);
    initDevInfo();
}

bool CDevSteSkip::checkSkipBindState(const unsigned int &nCardId)
{
    auto iter = m_mapSkipToElement.find(nCardId);
    if(iter!= m_mapSkipToElement.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CDevSteSkip::setElementBindFlag(const unsigned short &shIndex, bool bBind)
{
    bool bRet = CDevHardwareBase::setElementBindFlag(shIndex, bBind);
    if(bRet && bBind)
    {
        m_mapSkipToElement[m_vecSubElement[shIndex]->getElementID()] = shIndex;
    }
    else
    {
        auto it = m_mapSkipToElement.find(m_vecSubElement[shIndex]->getElementID());
        if(it != m_mapSkipToElement.end())
        {
            if(m_setDisconnectIndex.find(it->second) != m_setDisconnectIndex.end())
            {
                m_setDisconnectIndex.erase(it->second);
            }

            m_mapSkipToElement.erase(it);
        }
    }
    return bRet;
}

//通过广播的方式，开启所有跳绳设备
bool CDevSteSkip::startWork(bool bCast, bool bWait/* = true*/)
{
    //0.保证串口存在
    if(nullptr == m_pCom)
    {
        LOGERROR("communication invalid");
        return false;
    }

    //1.保证至少存在一条绳子被绑定
    if(m_mapSkipToElement.size() == 0)
    {
        LOGERROR("jump rope bind number is zero");
        return false;
    }

    //2.跳绳设备只存在一个设备，调用广播开启所有连接上的绳子
    //跳绳设备的广播启动，在此处将命令写死
    m_shLastCommand = DATA_FRAME_TYPE_SWORK;

    //广播启动时，绑定卡号规定为 0
    m_nRopeSkipCardNo = 0;
    /*unsigned char arrContent[] = {0xFF, 0xFF, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00\
                       , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};*/

    unsigned char arrContent[] = {0xFF, 0xFF, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00\
                       , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00\
                       , 0x58, 0xE8, 0x04, 0x21, 0x01, 0x38, 0x00, 0x00};

    //最后四个字节赋值时间(2字节计时时间 s | 2字节准备时间 ms)
    memcpy(arrContent + 38, &m_usSkipTime, sizeof(unsigned short));

    QByteArray arr(reinterpret_cast<char *>(arrContent), 0x2E);
    qDebug()<<arr.toHex();

    //判断跳绳是否为首次启动
    if(CTestGuideSkip::getFirstStartFlag())
    {
        m_bSuccessFlag = false;

        long long llCurentTime = 0;

        CTestGuideSkip::setFirstStartFlag(false);

        llCurentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

        sendData(arr, bWait, 500, COMMAND_LEVEL_2, llCurentTime);

        CSubElementRopeSkip *pElement = nullptr;
        static CLocation s_location;
        for(auto &element : m_mapSkipToElement)
        {
            //开始测试
            pElement = reinterpret_cast<CSubElementRopeSkip *>(m_vecSubElement[element.second]);
            pElement->startSkipTest();

            s_location.m_iSteDevMacID = m_iDevID;
            s_location.m_shElementIndex = element.second;
            emit signalStartSkipRope(s_location,pElement->getStartTime());
        }
    }
    else
    {
        sendData(arr, bWait, 500, COMMAND_LEVEL_2);
    }

    return m_bSuccessFlag;
}

//通过广播的方式，关闭所有跳绳设备
bool CDevSteSkip::stopWork(bool bCast, bool bWait)
{
    //0.保证串口存在
    if(nullptr == m_pCom)
    {
        LOGERROR("communication invalid");
        return false;
    }

    //1.保证至少存在一条绳子被绑定
    if(m_mapSkipToElement.size() == 0)
    {
        LOGERROR("jump rope bind number is zero");
        return false;
    }

    //2.先停止测试，保证手动取消不会保存结果
    CSubElementRopeSkip *pElement = nullptr;
    for(auto &element : m_mapSkipToElement)
    {
        pElement = reinterpret_cast<CSubElementRopeSkip *>(m_vecSubElement[element.second]);
        pElement->stopTest();
    }

    //3.跳绳设备只存在一个设备，调用广播关闭所有连接上的绳子
    //跳绳设备的广播关闭，在此处将命令写死
    m_shLastCommand = DATA_FRAME_TYPE_EWORK;
    unsigned char arrContent[] = {0xFF, 0xFF, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00\
                       , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00};

    QByteArray arr(reinterpret_cast<char *>(arrContent), 0x22);
    qDebug()<<arr.toHex();

    bool bRet = false;
    bRet = sendData(arr, bWait, 500, COMMAND_LEVEL_1);

    return bRet;
}

//通过指定命令，开始对应跳绳设备
bool CDevSteSkip::startWork(unsigned short shElementIndex, bool bWait)
{
    //0.保证串口存在
    if(nullptr == m_pCom)
    {
        LOGERROR("communication invalid");
        return false;
    }

    //1.保证启动序号已经被绑定
    bool bFindFlag = false;
    unsigned int uiSkipCode = 0;
    for(auto element = m_mapSkipToElement.begin(); element != m_mapSkipToElement.end(); element++)
    {
        if(element->second == shElementIndex)
        {
            bFindFlag = true;
            uiSkipCode = element->first;
            break;
        }
    }

    if(!bFindFlag)
    {
        LOGERROR("element index invalid");
        return false;
    }

    //2.开启指定设备
    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdSworkRopeSkip(arr, CDevDataCfg::getFrameTypeByTestItem(TEST_ITEM_ROPE_SKIP), m_cHardwareCode, uiSkipCode, m_usSkipTime, 0);

    //1对1启动，需要验证ID正确性
    m_nRopeSkipCardNo = uiSkipCode;

    bool bRet = false;

    bRet = sendData(arr, bWait, 1000, COMMAND_LEVEL_1);

    qDebug()<<"##### Skip Code：" <<uiSkipCode<<" Send Result："<< bRet;

    //暂时在成功后，才会开始测试
    if(bRet)
    {
        CSubElementRopeSkip *pElement = nullptr;
        pElement = reinterpret_cast<CSubElementRopeSkip *>(m_vecSubElement[shElementIndex]);
        pElement->startSkipTest();

        //开启超时计时器
        static CLocation s_location;
        s_location.m_iSteDevMacID   = m_iDevID;
        s_location.m_shElementIndex = shElementIndex;
        emit signalStartSkipRope(s_location,pElement->getStartTime());
    }

    return bRet;
}

//通过指定命令，停止对应跳绳设备
bool CDevSteSkip::stopWork(unsigned short shElementIndex, bool bWait)
{
    //0.保证串口存在
    if(nullptr == m_pCom)
    {
        LOGERROR("communication invalid");
        return false;
    }

    //1.保证启动序号已经被绑定
    bool bFindFlag = false;
    unsigned int uiSkipCode = 0;
    for(auto element = m_mapSkipToElement.begin(); element != m_mapSkipToElement.end(); element++)
    {
        if(element->second == shElementIndex)
        {
            bFindFlag = true;
            uiSkipCode = element->first;
            break;
        }
    }

    if(!bFindFlag)
    {
        LOGERROR("element index invalid");
        return false;
    }

    //2.先停止测试，保证手动取消不会保存结果
    CSubElementRopeSkip *pElement = nullptr;
    pElement = reinterpret_cast<CSubElementRopeSkip *>(m_vecSubElement[shElementIndex]);
    pElement->stopTest();

    /*
    //跳绳设备暂时不发送取消测试的命令，测试认为取消测试会震动导致用户无法分清
    return true;
    */

    //3.停止指定设备
    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdEwork(arr, CDevDataCfg::getFrameTypeByTestItem(TEST_ITEM_ROPE_SKIP), m_cHardwareCode, uiSkipCode, m_usSkipTime);
    qDebug()<<arr.toHex();

    bool bRet = false;
    bRet = sendData(arr, bWait, 500, COMMAND_LEVEL_1);

    return bRet;
}

void CDevSteSkip::setSuccessFlag(bool bFlag)
{
    m_bSuccessFlag = bFlag;
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
  **************************************************/
int CDevSteSkip::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    //0.安全保护
    int nLen = CDevHardwareBase::parseSinglePackage(iFrameNo, pData, iDatalen);
    if(nLen > 0)
        return nLen;

    //检测数据有效性
    if(!checkSignalPackageValid(pData, iDatalen))
        return iDatalen;

    //1.组装跳绳信息
    //unsigned char  ucDeviceType;
    //unsigned char  ucDataPoints;
    unsigned int   uiIDCode;
    //unsigned short usDataType;
    unsigned char  ucDataStatus;
    unsigned char  ucBindFlag;
    unsigned short usStartNum;
    unsigned short usSkipNum;
    unsigned char  ucChargeStatus;
    unsigned char  ucPowerNum;

    //2.0 解析设备类型
    //memcpy_s(&ucDeviceType, sizeof(unsigned char), pData + DEVICE_TYPE, sizeof(unsigned char));

    //2.1 解析数据点数
    //memcpy_s(&ucDataPoints, sizeof(unsigned char), pData + DATA_POINTS, sizeof(unsigned char));

    //2.2 解析身份码
    memcpy_s(&uiIDCode, sizeof(unsigned int), pData + ID_CODE, sizeof(unsigned int));

    //2.3 解析数据类型
    //memcpy_s(&usDataType, sizeof(unsigned short), pData + DATA_TYPE, sizeof(unsigned short));

    //2.4 解析数据状态(0-无效 1-测试中 2-测试结束)
    memcpy_s(&ucDataStatus, sizeof(unsigned char), pData + DATA_STATUS, sizeof(unsigned char));

    //2.5 解析绑定状态
    memcpy_s(&ucBindFlag, sizeof(unsigned char), pData + DATA_BIND, sizeof(unsigned char));

    //解析开始次数
    memcpy_s(&usStartNum, sizeof(unsigned short), pData + START_NUMBER, sizeof(unsigned short));

    //2.6 解析跳绳次数
    memcpy_s(&usSkipNum, sizeof(unsigned short), pData + ROPE_NUMBER, sizeof(unsigned short));

    //2.7 解析充电状态
    memcpy_s(&ucChargeStatus, sizeof(unsigned char), pData + CHARGE_STATUS, sizeof(unsigned char));

    //2.8 解析电量
    memcpy_s(&ucPowerNum, sizeof(unsigned char), pData + POWER_NUM, sizeof(unsigned char));

    //身份码保护(因为硬件有时会组装无效的 00 数据，导致解析出错，硬件说暂时无法找到什么原因导致，因此增加此保护)
    if(uiIDCode < 1000000)
    {
        return m_iPackgeLen;
    }


    //2.9 判断绑定消息
    if(ucBindFlag == 1)
    {
        emit signalSkipBind(uiIDCode);
    }

    //3.0 仅发送绑定绳子消息
    auto itFind = m_mapSkipToElement.find(uiIDCode);
    if(itFind == m_mapSkipToElement.end())
    {
        return m_iPackgeLen;
    }

    //3.1 存储接收过的 Index
    m_setReceiveIndex.insert(itFind->second);

    //3.2 组装信息
    Dev_Rope_Info structRope;
    unsigned char ucDaStatus;
    CDevDataCfg::getTestState(ucDataStatus,ucDaStatus);

    structRope.uiIDCode = uiIDCode;
    structRope.usStartNum = usStartNum;
    structRope.usSkipNum = usSkipNum;
    structRope.ucChargeStatus = ucChargeStatus;
    structRope.ucPowerNum = ucPowerNum;
    structRope.ucDataStatus = ucDaStatus;

    //3.3 传递解析信息
    m_vecSubElement[itFind->second]->setElementInfo(static_cast<void*>(&structRope),0);

    return m_iPackgeLen;
}

void CDevSteSkip::onUpdateTimeOut()
{
    //不影响父类逻辑
    CDevHardwareBase::onUpdateTimeOut();

    //单独子设备是否断连判断( 1s 判断一次)
    m_iInsertNum ++ ;

    //每 10s 检测一次 是否存在设备断连
    if(m_iInsertNum > 10 * m_iTargetNum)
    {
        m_iInsertNum = 0;

        checkSubElementStatus(true);
    }
    //每 1s 判断一次  断连设备是否重连成功
    else if(m_iInsertNum % m_iTargetNum == 0)
    {
        checkSubElementStatus();
    }
}

//检查子设备状态
void CDevSteSkip::checkSubElementStatus(bool bCheckDisconnect/* = false*/)
{
    for (auto iter = m_mapSkipToElement.begin(); iter != m_mapSkipToElement.end(); iter++)
    {
        //测试中才进行判断
        short shTestState = m_vecSubElement[iter->second]->getTestState();
        if(CTestStateManage::TEST_IN_PROCESS == shTestState)
        {
            //存在数据
            if(m_setReceiveIndex.find(iter->second) != m_setReceiveIndex.end())
            {
                //判断原本是否为设备断连
                if(m_setDisconnectIndex.find(iter->second) != m_setDisconnectIndex.end())
                {
                    m_setDisconnectIndex.erase(iter->second);

                    signalCheckSubElement(iter->first,true);
                }
            }
            //不存在数据
            else
            {
                if(bCheckDisconnect)
                {
                    m_setDisconnectIndex.insert(iter->second);

                    signalCheckSubElement(iter->first,false);
                }
            }
        }
    }

    if(bCheckDisconnect)
    {
        m_setReceiveIndex.clear();
    }
}
