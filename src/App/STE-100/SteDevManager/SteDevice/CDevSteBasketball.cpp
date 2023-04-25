#include "CDevSteBasketball.h"
#include "SteSubElement/CSubElementBasketball.h"
#include "CDevDataCfg.h"

#include "SteTestGuide/CTestGuideBasketball.h"

CDevSteBasketball::CDevSteBasketball(unsigned int iDevID, ICommunicate &communicate)
    :CDevHardwareBase(iDevID, communicate)
    , m_shDataLen(sizeof (CElementDataBasketball))
{
    m_shDevType = DEV_TYPE_BASKETBALL;

    initSubElement<CSubElementBasketball>(SubElementPropertye::ELEMENT_NUM_Basketball);
    initDevInfo();
}

CDevSteBasketball::~CDevSteBasketball()
{
}

bool CDevSteBasketball::startWork(bool bCast, bool bWait)
{
    //0.安全保护
    if(nullptr == m_pCom)
    {
        LOGERROR("communication invalid");
        return false;
    }

    //1.保证设备已经绑定
    auto elementSize = m_vecSubElement.size();
    if(elementSize == 0)
    {
        LOGERROR("element size is zero");
        return false;
    }

    //2.获取当前时间
    long long llCurrentTime = 0;

    llCurrentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    if(CTestGuideBasketball::getFirstStartFlag())
    {
        //修改相关标志
        CTestGuideBasketball::setFirstStartFlag(false);

        m_bSuccessFlag = false;

        CTestGuideBasketball::setFirstStartTime(llCurrentTime);

        QByteArray byteArray;
        m_shLastCommand = CGenerationCommand::getCmdSworkBasketball(byteArray,CDevDataCfg::getFrameTypeByTestItem(TEST_ITEM_BASKETBALL),m_cHardwareCode,getMacID(),m_uiVoiceTime);

        //发送命令
        sendData(byteArray, bWait, 400, COMMAND_LEVEL_2, llCurrentTime);

        //首次开始需要播放语音
        if(!bCast)
        {
            emit signalStartAllElement(m_iDevID);
        }
        else
        {
            emit signalStartCast();
        }
    }
    else
    {
        //后续启动需要计算出时间差
        long long llTimeDiff = 0;

        llTimeDiff = QDateTime::currentDateTime().toMSecsSinceEpoch() - CTestGuideBasketball::getFirstStartTime();

        if(llTimeDiff > m_uiVoiceTime)
        {
            m_bSuccessFlag = true;
        }
        else
        {
            QByteArray byteArray;
            m_shLastCommand = CGenerationCommand::getCmdSworkBasketball(byteArray,CDevDataCfg::getFrameTypeByTestItem(TEST_ITEM_BASKETBALL),m_cHardwareCode,getMacID(),static_cast<unsigned short>(m_uiVoiceTime - llTimeDiff));

            qDebug()<<"Send Time is:"<<static_cast<unsigned short>(m_uiVoiceTime - llTimeDiff);

            sendData(byteArray, bWait, 400, COMMAND_LEVEL_2);
        }
    }

    /*
    //2.生成启动命令
    QByteArray byteArray;
    m_shLastCommand = CGenerationCommand::getCmdSworkBasketball(byteArray,CDevDataCfg::getFrameTypeByTestItem(TEST_ITEM_BASKETBALL),m_cHardwareCode,getMacID(),m_uiVoiceTime);

    qDebug()<<"Basketball StartWork: "<<byteArray.toHex();

    //3.第一次启动(需要下发当前时间)
    if(CTestGuideBasketball::getFirstStartFlag())
    {
        //修改相关标志
        CTestGuideBasketball::setFirstStartFlag(false);

        m_bSuccessFlag = false;
        //m_bSuccessFlag = true;

        //获取当前时间
        long long llCurrentTime = 0;

        llCurrentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

        CTestGuideBasketball::setFirstStartTime(llCurrentTime);

        //发送命令
        sendData(byteArray, bWait, 400, COMMAND_LEVEL_3, llCurrentTime);

        //首次开始需要播放语音
        if(!bCast)
        {
            emit signalStartAllElement(m_iDevID);
        }
        else
        {
            emit signalStartCast();
        }
    }
    else
    {
        long long llTimeDiff = 0;

        llTimeDiff = QDateTime::currentDateTime().toMSecsSinceEpoch() - CTestGuideBasketball::getFirstStartTime();

        if(llTimeDiff > m_uiVoiceTime)
        {
            m_bSuccessFlag = true;
        }
        else
        {
            sendData(byteArray, bWait, 400, COMMAND_LEVEL_3);
        }
    }
    */

    return m_bSuccessFlag;
}

void CDevSteBasketball::setSuccessFlag(bool bFlag)
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
int CDevSteBasketball::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    int nLen = CDevHardwareBase::parseSinglePackage(iFrameNo, pData, iDatalen);
    if(nLen > 0)
        return nLen;

    //到这一步能保证数据为数据帧中的 单包数据，只进行当前数据解析即可

    //检测数据有效性
    if(!checkSignalPackageValid(pData, iDatalen))
        return iDatalen;

    //解析数据设置到测试单元中
    CSubElementBase *pElement = getElement(0);
    if(pElement == nullptr)
    {
        LOGERROR("pElement Err invalid");
        return m_iPackgeLen;
    }


    static CElementDataBasketball elementData;

    //获取状态位
    getTestStateFromPackgeData(elementData, pData);

    //获取数据
    memcpy(&elementData.m_fData, pData + PACK_DATA_MARK + 2, 4);

    //将数据设置到对应测试单元
    pElement->setElementInfo(&elementData, m_shDataLen);

    return m_iPackgeLen;
}
