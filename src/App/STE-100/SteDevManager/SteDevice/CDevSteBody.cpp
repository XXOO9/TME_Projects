#include "CDevSteBody.h"
#include "SteSubElement/CSubElementBody.h"
#include "CDevDataCfg.h"

// 身高体重，体型仪
CDevSteBody::CDevSteBody(unsigned int iDevID, ICommunicate &communicate)
    : CDevHardwareBase (iDevID, communicate)
    , m_shDataLenHeight(sizeof (CElementHeight))
    , m_shDataLenWeight(sizeof (CElementWeight))
    , m_shDataLenHBC(sizeof (CElementHBC))
{
    m_shDevType = DEV_TYPE_BODY;

    initSubElement<CSubElementBody>(SubElementPropertye::ELEMENT_NUM_Body);
    initDevInfo();

    m_setDataFrameTypeHW.insert(DATA_FRAME_TYPE_HEIGHT);
    m_setDataFrameTypeHW.insert(DATA_FRAME_TYPE_WEIGHT);
}

CDevSteBody::~CDevSteBody()
{

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
int CDevSteBody::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    //若是 组网测试 人体阻抗数据,那么清空对应的采集空间
    if(m_shLastCommand == DATA_FRAME_TYPE_SWORK && m_bStationSte && m_vecSubElement[0]->getCurTestItem() == TEST_ITEM_HBC)
    {
        m_setSubHBC.clear();
    }

    int nLen = CDevHardwareBase::parseSinglePackage(iFrameNo, pData, iDatalen);
    if(nLen > 0)
        return nLen;

    //到这一步能保证数据为数据帧中的 单包数据，只进行当前数据解析即可
    CSubElementBase *pElement = getElement(0);
    if(pElement == nullptr)
    {
        LOGERROR("pElement Err invalid");
        return iDatalen;
    }

    unsigned short shCmd;
    memcpy(&shCmd, pData + PACK_CMD_TYPE, 2);
    shCmd = getCmdFromOrigCmd(shCmd);
    int nTheoryLen = getDataTypeLen(shCmd);

    unsigned short shCurDataFrameType = CDevDataCfg::getFrameTypeByTestItem( m_vecSubElement[0]->getCurTestItem());

    ///判断数据类型是否一致
    bool bCorrectDataType = false;

    //身高体重统一进行测试，人体阻抗进行其他其他判断
    if(m_setDataFrameTypeHW.find(shCurDataFrameType) != m_setDataFrameTypeHW.end())
    {
        bCorrectDataType = m_setDataFrameTypeHW.find(shCmd) != m_setDataFrameTypeHW.end();
    }
    //判断人体阻抗,组网的人体阻抗和直连的阻抗不同，主要是命令不同
    else
    {
        bCorrectDataType = shCmd == shCurDataFrameType;
    }

    if(!bCorrectDataType && nTheoryLen == 0)
    {
        return iDatalen;
    }

    ///判断数据长度是否合适
    if(iDatalen < nTheoryLen)
    {
        QByteArray arr(reinterpret_cast<char *>(pData), iDatalen);
        arr = arr.toHex();
        LOGERROR("sigle pack len Err nTheoryLen = " + to_string(nTheoryLen) + " iDatalen =" + to_string(iDatalen) + string(arr.data(), static_cast<unsigned int>(arr.size())));
        return iDatalen;
    }

    ///根据具体命令进行解析
    switch (shCmd) {
    case DATA_FRAME_TYPE_HEIGHT:
        parseHeight(pElement, pData);
        break;
    case DATA_FRAME_TYPE_WEIGHT:
        parseWeight(pElement, pData);
        break;
    case DATA_FRAME_TYPE_BODY_IMPEDANCE:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE1:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE2:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE3:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE4:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE5:
        parseHBC(pElement, pData, shCmd);
        break;
    }

    return nTheoryLen;
}

bool CDevSteBody::existErr(const unsigned short &shElement)
{
    if(shElement >= m_vecSubElement.size())
        return true;

    return false;
}

bool CDevSteBody::calibrateWeight(const float &fValue)
{
    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdCalibrate(arr, DATA_FRAME_TYPE_WEIGHT, m_cHardwareCode, m_iDevID, fValue);
    return sendData(arr, true, 2000, COMMAND_LEVEL_1);
}

bool CDevSteBody::calibrateHeight(const float &fValue)
{
    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdCalibrate(arr, DATA_FRAME_TYPE_HEIGHT, m_cHardwareCode, m_iDevID, fValue);
    qDebug()<< arr.toHex();
    return sendData(arr, true, 2000, COMMAND_LEVEL_1);
}

bool CDevSteBody::startWork(unsigned short shElementIndex, bool bWait)
{
    if(m_pCom == nullptr)
    {
        LOGERROR("communication invalid");
        return false;
    }

    auto elementSize = m_vecSubElement.size();
    if(shElementIndex >= elementSize || elementSize == 0)
    {
        LOGWARN("shelement :" + to_string(shElementIndex) + "element size:" + to_string(elementSize));
        return false;
    }

    CSubElementBase *pElemet = m_vecSubElement[shElementIndex];

    auto &nSubID = pElemet->getElementID();
    if(nSubID == INVALID_ELEMENT_ID)
    {
        LOGERROR("invalid element ID = " + to_string(nSubID));
        return false;
    }

    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdSwork(arr, pElemet->getFrameType(), m_cHardwareCode, m_iDevID, nSubID);
    LOGINFO("startWork" + arr.toHex().toStdString());
    qDebug() << "startWork" << arr.toHex();

    //经过测试发现身高体重仪发送命令函数需要等待的时间更长一点
    int nMsec = 300;
    if(m_shDevType == DEV_TYPE_BODY)
        nMsec = 1000;

    bool bRet = false;

    if(pElemet->checkIfCanStart(pElemet->getCurTestItem()))
    {
        bool bRet = sendData(arr, bWait, nMsec, COMMAND_LEVEL_1);
        if(bRet)
        {
            pElemet->setElementStartFlag(pElemet->getCurTestItem(), true);
            static CLocation s_location;
            s_location.m_iSteDevMacID = m_iDevID;
            s_location.m_shElementIndex = shElementIndex;
            emit signalStartElement(s_location);
        }
    }
    else
    {
        bRet= true;
    }

    return bRet;
}

void CDevSteBody::parseHeight(CSubElementBase *pElement, unsigned char *pData)
{
    static CElementHeight elementData;

    elementData.m_shTestItem = TEST_ITEM_HEIGHT;
    //获取状态位
    getTestStateFromPackgeData(elementData, pData);

    if(m_shLastCommand == DATA_FRAME_TYPE_SWORK && elementData.m_cTestState == DEV_TEST_STATE_TESTING
            && pElement->getCurTestItem() == TEST_ITEM_HEIGHT && !m_bSendCommandSucess)
    {
        m_bSendCommandSucess = true;
        m_shLastCommand = DATA_FRAME_TYPE_NULL;
    }

    //获取数据
    memcpy(&elementData.m_fHeight, pData + PACK_DATA_MARK + 2, 4);

    //原始数据转为标准数据
    getNormalData(elementData.m_fHeight, pElement->getCurTestItem());

    //将数据设置到对应测试单元（长度不为0即可）
    pElement->setElementInfo(&elementData, m_shDataLenHeight);
}

void CDevSteBody::parseWeight(CSubElementBase *pElement, unsigned char *pData)
{
    static CElementWeight elementData;

    elementData.m_shTestItem = TEST_ITEM_WEIGHT;
    //获取状态位
    getTestStateFromPackgeData(elementData, pData);

    if(m_shLastCommand == DATA_FRAME_TYPE_SWORK && elementData.m_cTestState == DEV_TEST_STATE_TESTING
            && pElement->getCurTestItem() == TEST_ITEM_WEIGHT && !m_bSendCommandSucess)
    {
        m_bSendCommandSucess = true;
        m_shLastCommand = DATA_FRAME_TYPE_NULL;
    }

    //获取数据
    memcpy(&elementData.m_fWeight, pData + PACK_DATA_MARK + 2, 4);

    //将数据设置到对应测试单元（长度不为0即可）
    pElement->setElementInfo(&elementData, m_shDataLenWeight);
}

void CDevSteBody::parseHBC(CSubElementBase *pElement, unsigned char *pData, const unsigned short &shCmd)
{   
    //原始人体阻抗 30个float
    static CElementHBC elementData;
    //获取状态位
    getTestStateFromPackgeData(elementData, pData);
    elementData.m_shTestItem = TEST_ITEM_HBC;
    bool bSendToElement = true;

    if(m_shLastCommand == DATA_FRAME_TYPE_SWORK && elementData.m_cTestState == DEV_TEST_STATE_TESTING
            && pElement->getCurTestItem() == TEST_ITEM_HBC && !m_bSendCommandSucess)
    {
        m_bSendCommandSucess = true;
        m_shLastCommand = DATA_FRAME_TYPE_NULL;
    }

    //直连设备解析
    if(shCmd == DATA_FRAME_TYPE_BODY_IMPEDANCE && !m_bStationSte)
    {
        memcpy(&elementData.m_arrHBC, pData + PACK_DATA_MARK + 2, 120);
    }
    //若是人体阻抗分包需要等到所有数据完整再发送 ,分包数据一个大小为 6个float
    else
    {
        bool bInsert = true;
        QByteArray arr;

        int nStartIndex = 0;
        switch(shCmd){
        case DATA_FRAME_TYPE_BODY_IMPEDANCE1:
            nStartIndex = 0;
            break;
        case DATA_FRAME_TYPE_BODY_IMPEDANCE2:
            nStartIndex = 6;
            break;
        case DATA_FRAME_TYPE_BODY_IMPEDANCE3:
            nStartIndex = 6 * 2;
            break;
        case DATA_FRAME_TYPE_BODY_IMPEDANCE4:
            nStartIndex = 6 * 3;
            break;
        case DATA_FRAME_TYPE_BODY_IMPEDANCE5:
            nStartIndex = 6 * 4;
            break;
        default:
            bInsert = false;
            break;
        }

//        if(bInsert || m_setSubHBC.size() == 0)
//        {
//            if(m_setSubHBC.size() == 0)
//            {
//                qDebug() << "000000";
//            }
//            else
//            {
//                qDebug() << shCmd;
//            }

//            CGenerationCommand::getCmdNew(arr, DATA_FRAME_TYPE_GET_DATA, shCmd, m_cHardwareCode, getMacID(), getMacID());
//            sendData(arr);
//        }

        //测试完成标志为true才进行后续动作
        bInsert &= CDevDataCfg::isHardwareComplete(elementData.m_cTestState);

        if(!bInsert)
            return;

        memcpy(&elementData.m_arrHBC[nStartIndex], pData + PACK_DATA_MARK + 2, 24);

        m_setSubHBC.insert(shCmd);
        if(m_setSubHBC.size() != 5)
        {
            bSendToElement = false;
        }
    }

    if(bSendToElement)
    {
        pElement->setElementInfo(&elementData, m_shDataLenHBC);
    }
}

short CDevSteBody::getDataTypeLen(const unsigned short &shDataType)
{
    short shLen = 0;
    switch (shDataType) {
    case DATA_FRAME_TYPE_HEIGHT:
        shLen = Packet_Len_Height;
        break;
    case DATA_FRAME_TYPE_WEIGHT:
        shLen = Packet_Len_Weight;
        break;
    case DATA_FRAME_TYPE_BODY_IMPEDANCE:
        shLen = Packet_Len_Impedance;
        break;
    case DATA_FRAME_TYPE_BODY_IMPEDANCE1:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE2:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE3:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE4:
    case DATA_FRAME_TYPE_BODY_IMPEDANCE5:
        shLen = Packet_Len_Sub_Impedance;
        break;
    default:
        break;
    }

    return shLen;
}

void CDevSteBody::onUpdateTimeOut()
{
    CDevHardwareBase::onUpdateTimeOut();

    //获取数据(目前组网设备会自动上传测试的数据，在测试身高时需要同时获取体重数据，反之亦然)
    if(m_bGetDataFlag && m_bStationSte)
    {
        m_iGetDataTotal += m_iUpdateInteval;
        if(m_iGetDataTotal >= m_iGetDataInterval)
        {
            //获取当前测试项的数据
            unsigned short shDataFrameType = CDevDataCfg::getFrameTypeByTestItem(m_vecSubElement[0]->getCurTestItem());

            //身高和体重需要一直进行查询
            if(shDataFrameType != DATA_FRAME_TYPE_BODY_IMPEDANCE)
            {
//                CGenerationCommand::getCmdNew(m_arrGetData, DATA_FRAME_TYPE_HEIGHT, DATA_FRAME_TYPE_HEIGHT, m_cHardwareCode, m_iDevID, m_iDevID);
//                sendData(m_arrGetData, false, 300, COMMAND_LEVEL_1);
//                CGenerationCommand::getCmdNew(m_arrGetData, DATA_FRAME_TYPE_WEIGHT, DATA_FRAME_TYPE_WEIGHT, m_cHardwareCode, m_iDevID, m_iDevID);
//                sendData(m_arrGetData, false, 300, COMMAND_LEVEL_1);
            }
            //人体阻抗是分包进行发送，根据完成的人体阻抗进行命令选择(目前组网不测人体)
            else
            {
                size_t szHBCPregress = m_setSubHBC.size();
                switch (szHBCPregress) {
                case 0:
                    shDataFrameType = DATA_FRAME_TYPE_BODY_IMPEDANCE1;
                    break;
                case 1:
                    shDataFrameType = DATA_FRAME_TYPE_BODY_IMPEDANCE2;
                    break;
                case 2:
                    shDataFrameType = DATA_FRAME_TYPE_BODY_IMPEDANCE3;
                    break;
                case 3:
                    shDataFrameType = DATA_FRAME_TYPE_BODY_IMPEDANCE4;
                    break;
                case 4:
                    shDataFrameType = DATA_FRAME_TYPE_BODY_IMPEDANCE5;
                    break;
                default:
                    m_iGetDataTotal -= m_iGetDataInterval;
                    return;
                }

                qDebug() << "QQQQQQQQQQQQQQQQQ" << szHBCPregress;
                CGenerationCommand::getCmdNew(m_arrGetData, shDataFrameType, shDataFrameType, m_cHardwareCode, m_iDevID, m_iDevID);
                sendData(m_arrGetData, false, 300, COMMAND_LEVEL_1);
            }

            m_iGetDataTotal -= m_iGetDataInterval;
        }
    }
}
