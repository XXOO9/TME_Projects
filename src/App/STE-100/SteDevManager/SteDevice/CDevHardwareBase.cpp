#include "CDevHardwareBase.h"
#include "AssistDevManager/PowerUart/CDevPowerUart.h"
#include "CDevDataCfg.h"

//主要用于提示，不进行其他任何操作
#include "CInterAction.h"
static bool bUseNewCmd = true;

CDevHardwareBase::CDevHardwareBase(unsigned int iDevID, ICommunicate &communicate)
{   
    setCommunicate(&communicate);
    setMacID(iDevID);

    //从该构造函数构建的体测设备为非独立设备
    m_bIndependent = false;

    //设备需要检测断连
    m_bDetermineDisconnect = true;

    connect(&m_objStateManage, &CTestStateManage::signalTestStateChange, this, &CDevHardwareBase::onTestStateChange);

    //断连时间在.h中设置设备是组网还是直连的时候设置
}

CDevHardwareBase::~CDevHardwareBase()
{
    uninitSubElement();
}

void CDevHardwareBase::setCommunicate(ICommunicate *pCommunicate)
{
    if(m_pCom != nullptr)
    {
        m_pCom->close();
        disconnect(m_pCom, &ICommunicate::signalErrOccurred, this, &CDevHardwareBase::onComErrOccurred);
        disconnect(this, &CDevice::signalSendDataToDev, m_pCom, &ICommunicate::signalSendData);
    }

    m_pCom = pCommunicate;

    if(m_pCom != nullptr)
    {
        connect(m_pCom, &ICommunicate::signalErrOccurred, this, &CDevHardwareBase::onComErrOccurred);
        connect(this, &CDevice::signalSendDataToDev, m_pCom, &ICommunicate::signalSendData);
    }
}

bool CDevHardwareBase::startWork(bool bCast, bool bWait/* = true*/)
{
    if(m_pCom == nullptr)
    {
        LOGERROR("communication invalid");
        return false;
    }

    auto elementSize = m_vecSubElement.size();
    if(elementSize == 0)
    {
        LOGWARN("element size:" + to_string(elementSize));
        return false;
    }

    //初始所有测试类型都一致，当 测试单元id为0时代表开启所有设备进行测试
    //同时设备id也填0
    unsigned int nDevId = 0;

    cmdInfo info;
    info.setInteval(300);

    if(m_shDevType == DEV_TYPE_BODY || m_shDevType == DEV_TYPE_SOLIDBALL)
    {
        info.setInteval(600);
    }

    if(bCast)
    {
        nDevId = 0;
        info.shIndex = ELEMENT_NUM_CUST;
    }
    else
    {
        nDevId = m_iDevID;
        info.shIndex = ELEMENT_NUM_CUR_DEV;
    }

    info.shCmdType = m_shLastCommand = CGenerationCommand::getCmdSwork(info.arrCmd, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, nDevId, 0);
    LOGINFO("startWork cast" + info.arrCmd.toHex().toStdString());
    qDebug() << "startWork cast" << info.arrCmd.toHex();

    bool bRet = true;

    if(!bUseNewCmd)
    {
        if(bRet)
        {
            if(!bCast)
            {
                emit signalStartAllElement(m_iDevID);
            }
            else
            {
                emit signalStartCast();
            }
        }
    }
    else
    {
        updateCmd(info);
    }

    return bRet;
}

bool CDevHardwareBase::stopWork(bool bCast, bool bWait)
{
    if(m_pCom == nullptr)
    {
        LOGERROR("communication invalid");
        return false;
    }

    auto elementSize = m_vecSubElement.size();
    if(elementSize == 0)
    {
        LOGWARN("element size:" + to_string(elementSize));
        return false;
    }

    //初始所有测试类型都一致，当 测试单元id为0时代表开启所有设备进行测试
    //同时设备id也填0
    QByteArray arr;
    unsigned int nDevID = bCast ? 0 : m_iDevID;

    m_shLastCommand = CGenerationCommand::getCmdEwork(arr, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, nDevID, 0);
    qDebug() << "stopWork" << arr.toHex();

    unsigned int nDevId = 0;
    cmdInfo info;
    info.setInteval(300);

    if(m_shDevType == DEV_TYPE_BODY || m_shDevType == DEV_TYPE_SOLIDBALL)
    {
        info.setInteval(600);
    }

    if(bCast)
    {
        nDevId = 0;
        info.shIndex = ELEMENT_NUM_CUST;
    }
    else
    {
        nDevId = m_iDevID;
        info.shIndex = ELEMENT_NUM_CUR_DEV;
    }

    info.shCmdType = m_shLastCommand = CGenerationCommand::getCmdEwork(info.arrCmd, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, nDevId, 0);
    LOGINFO("startWork cast" + info.arrCmd.toHex().toStdString());
    qDebug() << "startWork cast" << info.arrCmd.toHex();

    bool bRet = true;

    if(!bUseNewCmd)
    {
        if(bRet)
        {
            if(!bCast)
            {
                emit signalStartAllElement(m_iDevID);
            }
            else
            {
                emit signalStartCast();
            }
        }
    }
    else
    {
        updateCmd(info);
    }


    return sendData(arr, bWait, 300, COMMAND_LEVEL_1);
}

bool CDevHardwareBase::startWork(unsigned short shElementIndex, bool bWait)
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

    cmdInfo info;
    info.setInteval(300);
    info.shIndex = shElementIndex;
    info.shCmdType = m_shLastCommand = CGenerationCommand::getCmdSwork(info.arrCmd, pElemet->getFrameType(), m_cHardwareCode, m_iDevID, nSubID);
    LOGINFO("startWork" + info.arrCmd.toHex().toStdString());
    qDebug() << "startWork" << info.arrCmd.toHex();

    if(m_shDevType == DEV_TYPE_BODY || m_shDevType == DEV_TYPE_SOLIDBALL)
    {
        info.setInteval(600);
    }

    bool bRet = true;

    if(!bUseNewCmd)
    {
        bRet = sendData(info.arrCmd, bWait, info.nInterval, COMMAND_LEVEL_1);
        if(bRet)
        {
            static CLocation s_location;
            s_location.m_iSteDevMacID = m_iDevID;
            s_location.m_shElementIndex = shElementIndex;
            emit signalStartElement(s_location);
        }
    }
    else
    {
        updateCmd(info);
    }

    return bRet;
}

bool CDevHardwareBase::stopWork(unsigned short shElementIndex, bool bWait)
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
        LOGERROR("invalid element ID");
        return false;
    }

    cmdInfo info;
    info.setInteval(300);
    info.shIndex = shElementIndex;
    m_shLastCommand = info.shCmdType = CGenerationCommand::getCmdEwork(info.arrCmd, pElemet->getFrameType(), m_cHardwareCode, m_iDevID, nSubID);

    qDebug() << "stopWork" << info.arrCmd.toHex();
    LOGINFO("stopWork" + info.arrCmd.toHex().toStdString());

    if(m_shDevType == DEV_TYPE_SOLIDBALL)
    {
        info.setInteval(600);
    }

    bool bRet = true;

    if(!bUseNewCmd)
    {
        bRet = sendData(info.arrCmd, bWait, info.nInterval, COMMAND_LEVEL_1);
    }
    else
    {
        updateCmd(info);
    }

    pElemet->stopTest();
    return bRet;
}

bool CDevHardwareBase::setElementBindFlag(const unsigned short &shIndex, bool bBind)
{
    bool bRet = false;
    if(shIndex < m_vecSubElement.size())
    {
        m_vecSubElement[shIndex]->setBindFlag(bBind);
        bRet = true;
    }

    return bRet;
}

vector<CSubElementBase *> CDevHardwareBase::getAllBindElements()
{
    vector<CSubElementBase *> vecTemp;
    for(auto &element : m_vecSubElement)
    {
        if(element->getBindFlag())
        {
            vecTemp.push_back(element);
        }
    }

    return vecTemp;
}

bool CDevHardwareBase::existErrInBindElement()
{
    bool bRet = false;

    for(auto &element : m_vecSubElement)
    {
        if(element->getBindFlag())
        {
            element->checkErr();

            if ((DEVICE_ERR_TYPE_UNKNOWN & element->getErrType()) ||
                (DEVICE_ERR_TYPE_MOUDLE  & element->getErrType()) || element->getErrCode() != DEVICE_NORMAL)
            {
                bRet = true;
                break;
            }
        }
    }

    return bRet;
}

bool CDevHardwareBase::existErr(const unsigned short &shElement)
{
    if(shElement >= m_vecSubElement.size())
        return false;

    bool bRet = false;

    if (m_vecSubElement[shElement]->getErrCode() != DEVICE_NORMAL)
    {
        bRet = true;
    }

    return bRet;
}

CSubElementBase *CDevHardwareBase::getFirstAvailableElement()
{
    CSubElementBase* pSubElement = nullptr;

    for (unsigned int i = 0; i < m_vecSubElement.size(); i++)
    {
        if(m_vecSubElement[i]->getAvailableFlag())
        {
            pSubElement = m_vecSubElement[i];
            break;
        }
    }

    return pSubElement;
}


//
/*************************************************
  <函数名称>    parseSinglePackage(int iStartPos, QByteArray &array)
  <功能描述>    当前类进行统一的逻辑处理，保证到达确定设备之后都是对应设备的专业处理
                当前类不进行原始数据结构的变更
  <参数说明>

  <返 回 值>    返回0 底层设备处理对应的数据帧信息，若是其他命令返回对应命令具体长度

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
int CDevHardwareBase::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    //初始化测试项目数据包长(根据设备类型获取数据帧对应的包长，设备仅存在一个数据类型的可以使用对应的变量)
    if(!m_bPackgeLenInit)
    {
        updateTestItemToChangePackgeLen(CDevDataCfg::getTestItemByDevType(m_shDevType));
        m_bPackgeLenInit = true;
    }

    if(pData == nullptr && iFrameNo == 0)
    {
        return iDatalen;
    }

    //进行心跳间隔计时清0
    m_iIntervalTotal = 0;

    //长度不符合规定要求
    if(iDatalen < PACK_DATA_MARK)
    {
        QByteArray arr(reinterpret_cast<char *>(pData), iDatalen);
        arr = arr.toHex();
        LOGERROR("sigle pack len Err :" + string(arr.data(), static_cast<unsigned int>(arr.size())));
        return iDatalen;
    }

    unsigned short shCmd;
    memcpy(&shCmd, pData + PACK_CMD_TYPE, 2);
    shCmd = getNormalCmdFromOrigCmd(shCmd);

    auto listCmd = getCmd();

    if(shCmd != DATA_FRAME_TYPE_NULL)
    {
        for(auto &Info : listCmd)
        {
            //            if(Info.nFrameNo != iFrameNo)
            //            {
            //                continue;
            //            }

            //按照设计这里可以只判断帧号(目前组网帧号错乱，只能通过命令进行设置)
            if(shCmd == Info.shCmdType)
            {
                //某些设备的开始命令需要判断对应的测试单元号，如中长跑，其他设备不进行判断，同一时间开启的start只会有一个
                if(shCmd == DATA_FRAME_TYPE_SWORK || shCmd == DATA_FRAME_TYPE_EWORK)
                {
                    switch (m_shDevType) {
                    case DEV_TYPE_RUN_MID:
                        break;
                    case DEV_TYPE_ROPE_SKIP:
                        break;
                    default:
                        deleteCmd(Info);
                        break;
                    }
                }
                //犯规次数需要对比对应的犯规次数才能删除命令
                else if(shCmd == DATA_FRAME_TYPE_SET_ILLEGAL_TIMES)
                {
                    unsigned int nTempTimes = 0;
                    memcpy(&nTempTimes, pData + iDatalen - 4, 4);

                    LOGINFO("set illegal times = " + to_string(Info.nParam) + " ret times = " + to_string(nTempTimes))
                    if(nTempTimes == Info.nParam)
                    {
                        deleteCmd(Info);
                    }
                }
                else if(shCmd == DATA_FRAME_DEV_PARAM_SET)
                {
                    unsigned int nParam = 0;
                    memcpy(&nParam, pData + iDatalen - 4, 4);

                    LOGINFO("set param = " + to_string(Info.nParam) + " ret param = " + to_string(nParam))
                    if(nParam == Info.nParam)
                    {
                        deleteCmd(Info);
                    }
                }
                else
                {
                    deleteCmd(Info);
                }

                //处理当前命令返回值
                switch (shCmd) {
                case DATA_FRAME_TYPE_SWORK:
                {
                    switch (Info.shIndex) {
                    case ELEMENT_NUM_CUST:
                        emit signalStartCast();
                        break;
                    case ELEMENT_NUM_CUR_DEV:
                        emit signalStartAllElement(m_iDevID);
                        break;
                    default:
                    {
                        CLocation location(m_iDevID, Info.shIndex);
                        emit signalStartElement(location);
                        break;
                    }
                    }

                    break;
                }
                //分机编号，获取当前设备设备编号,四字节(只有普通类型设备会发送查询分机号，在发送时就进行了判断)
                case DATA_FRAME_TYPE_SET_EXTENSION_NO:
                {
                    cmdInfo info;
                    m_iNumIntervalTotal -= 500;
                    info.nFrameNo = CDevDataCfg::s_nFrameNo++;
                    info.shCmdType = CGenerationCommand::getCmdGetExtensionNo(info.arrCmd, info.nFrameNo, m_cHardwareCode, m_iDevID, m_vecSubElement[0]->getElementID());
                    updateCmd(info);
                    break;
                }
                case DATA_FRAME_TYPE_GET_EXTENSION_NO:
                {
                    if(!m_bInitExtensionNo)
                    {
                        if(iDatalen - PACK_DATA_MARK >= 4)
                        {
                            memcpy(&m_nExtensionNo, pData + PACK_DATA_MARK + 2, 4);
                            qDebug() << "&&&&&&&&&&&&&&&&&&&&&&&&" << m_nExtensionNo;
                            m_bInitExtensionNo = true;

                            //第一次设置分机编号需要将连个值设置为相同
                            if(m_nExtensionNo == m_nExtensionNoTarget && m_nExtensionNo == -1)
                            {
                                m_nExtensionNoTarget = m_nExtensionNo;
                            }
                        }
                        else
                        {
                            QByteArray arr(reinterpret_cast<char *>(pData), iDatalen);
                            LOGERROR("Get extension No false Single Package len = " + to_string(iDatalen) + "content = " + arr.toHex().toStdString());
                        }
                    }
                    break;
                }
                case DATA_FRAME_TYPE_433_EXIST_CFG:
                case DATA_FRAME_TYPE_433_ENTER_CFG:
                case DATA_FRAME_TYPE_433_CLEAR_CFG:
                case DATA_FRAME_TYPE_433_SET_CHANNEL:
                {
                    sigSetDevCfgRet(shCmd, getMacID());
                    //通知外部
                    break;
                }
                default:
                {

                }
                }

            }
        }
    }

    //判断命令是否是最后一次发送指令用于判断指令是否发送成功
    if(shCmd != DATA_FRAME_TYPE_NULL && shCmd == m_shLastCommand)
    {
        if(m_shLastCommand == DATA_FRAME_TYPE_READCARD)
        {
            unsigned int nTempCardNo = 0;
            memcpy(&nTempCardNo, pData + iDatalen - 4, 4);

            m_bSendCommandSucess = nTempCardNo == m_nRunMidCardNo;

            //除了在这里取消，在中长跑设备中的删除生效之后也进行发送
            bool bTip = false;
            if(nTempCardNo == 0xFFFFFFFF)
                bTip = true;

            CInterAction::Get_Instance().setViewErorrString(MINFO_LOAD_FACE_FEATURE, bTip);
            m_nRunMidCardNo = 0;
        }
        //中长跑开始命令需要判断卡号是否和当前一致
        else if(m_shDevType == DEV_TYPE_RUN_MID && shCmd == DATA_FRAME_TYPE_SWORK && m_nRunMidCardNo > 0)
        {
            unsigned int nTempCardNo = 0;
            memcpy(&nTempCardNo, pData + iDatalen - 4, 4);

            m_bSendCommandSucess = nTempCardNo == m_nRunMidCardNo;
            m_nRunMidCardNo = 0;
        }
        //跳绳一对一启动时，需要判断绳子ID是否和当前一致
        else if(m_shDevType == DEV_TYPE_ROPE_SKIP && shCmd == DATA_FRAME_TYPE_SWORK && m_nRopeSkipCardNo > 0)
        {
            unsigned int nTempCardNo = 0;
            memcpy(&nTempCardNo, pData + 2, 4);

            m_bSendCommandSucess = nTempCardNo == m_nRopeSkipCardNo;
            m_nRopeSkipCardNo = 0;
        }
        //获取组网中心时间处理
        else if(shCmd == DATA_FRAME_TYPE_GET_TIME_STAMP)
        {
            //得到时间
            unsigned int unRunTime = 0;

            memcpy(&unRunTime, pData + iDatalen - 4, 4);

            m_bSendCommandSucess = true;

            //短跑则直接设置起始时间
            if(m_shDevType == DEV_TYPE_RUN_SHORT)
            {
                m_fRunShortStartTime = unRunTime;
            }
        }
        //停止命令(中长跑需要特殊处理)
        else if(shCmd == DATA_FRAME_TYPE_EWORK)
        {
            m_bSendCommandSucess = true;

            //需要核对卡号,中间四字节为多功能参数( 现在的停止参数是4字节文档中是8字节)
            if(m_shDevType == DEV_TYPE_RUN_MID)
            {
                uint unCard = 0;
                memcpy(&unCard, pData + iDatalen - 4, 4);

                m_bSendCommandSucess = unCard == 0 || unCard == m_nStopCmdParam;
            }
        }
        else
        {
            m_bSendCommandSucess = true;
        }

        m_shLastCommand = DATA_FRAME_TYPE_NULL;
    }

    //只向底层设备发送数据帧，其他都返回0
    if(shCmd == DATA_FRAME_TYPE_GET_DATA)
    {
        return 0;
    }
    else
    {
        return CDevDataCfg::getSinglePackgeLenByDataType(shCmd);
    }
}

bool CDevHardwareBase::checkSignalPackageValid(unsigned char *pData, const int &iDatalen)
{
    bool bRet = iDatalen >= m_iPackgeLen;
    if(!bRet)
    {
        QByteArray arr(reinterpret_cast<char *>(pData), iDatalen);
        arr = arr.toHex();
        LOGERROR("sigle pack len Err :" + string(arr.data(), static_cast<unsigned int>(arr.size())));
    }

    return bRet;
}

void CDevHardwareBase::dataParse(QByteArray &array)
{
    if(array.size() == 0)
        return;
}

void CDevHardwareBase::uninitSubElement()
{
    for(auto &element : m_vecSubElement)
    {
        if(element != nullptr)
        {
            disconnect(&element->getTestStateManage(), &CTestStateManage::signalTestStateChange,\
                    this, &CDevHardwareBase::onElementTestStateChange);
            delete element;
        }
    }

    m_vecSubElement.clear();
}

short CDevHardwareBase::getElementState2devState(const short &shState)
{
    short shTestState = CTestStateManage::TEST_NOT_START;

    //测试中
    set<short> setStateTesting;
    setStateTesting.insert(CTestStateManage::TEST_IN_PROCESS);

    //测试完成
    set<short> setStateComplete;
    setStateComplete.insert(CTestStateManage::TEST_COMPLETED);
    setStateComplete.insert(CTestStateManage::TEST_TIMEOUT);

    if(setStateComplete.find(shState) != setStateComplete.end())
    {
        shTestState = CTestStateManage::TEST_COMPLETED;
    }
    else if(setStateTesting.find(shState) != setStateTesting.end())
    {
        shTestState = CTestStateManage::TEST_IN_PROCESS;
    }
    else
    {
        shTestState = CTestStateManage::TEST_NOT_START;
    }

    return shTestState;
}

void CDevHardwareBase::timeOutGetExtensionNo()
{
    //直连和组网的非特殊设备都要进行设置
    if(!m_bInitExtensionNo && !CDevDataCfg::isSpecialDev(m_shDevType))
    {
        m_iNumIntervalTotal += m_iUpdateInteval;
        if(m_iNumIntervalTotal > 500)
        {
            cmdInfo info;
            m_iNumIntervalTotal -= 500;
            info.nFrameNo = CDevDataCfg::s_nFrameNo++;
            info.shCmdType = CGenerationCommand::getCmdGetExtensionNo(info.arrCmd, info.nFrameNo, m_cHardwareCode, m_iDevID, m_vecSubElement[0]->getElementID());
            updateCmd(info);
        }
    }

    //若要设置的目标编号和当前编号不一致那么往下发送设置分机编号变量
    if(m_bInitExtensionNo && m_nExtensionNo != m_nExtensionNoTarget && m_nExtensionNoTarget >= 0)
    {
         //设置一次
         cmdInfo info;
         info.nFrameNo = CDevDataCfg::s_nFrameNo++;
         info.shCmdType = CGenerationCommand::getCmdSetExtensionNo(info.arrCmd, static_cast<uint>(m_nExtensionNoTarget), m_cHardwareCode, m_iDevID);
         updateCmd(info);

         //设置编号相同
         m_nExtensionNo = m_nExtensionNoTarget;
    }
}

bool CDevHardwareBase::timeOutCheckConnect()
{
    m_iIntervalTotal += m_iUpdateInteval;

    //设备长时间没有数据
    if (m_bDetermineDisconnect && m_iIntervalTotal >= m_iNoDataInteval)
    {
        m_iIntervalTotal -= m_iNoDataInteval;

        //跳绳设备不进行判断
        if(m_shDevType == DEV_TYPE_ROPE_SKIP)
            return true;

        //设置所有测试单元状态为错误状态
        for(auto &element : m_vecSubElement)
        {
            //设备断开停止所有测试单元的防作弊
            element->stopAntiCheating(false);

            setTestState(element->getIndex(), CTestStateManage::TEST_ERR);
        }

        //发送设备断连通知
        m_bValidDev = false;

        signalDevStatus(false, this);
        return false;
    }

    return true;
}

void CDevHardwareBase::updateTestItemToChangePackgeLen(const short &shTestItemLen)
{
    m_iPackgeLen = CDevDataCfg::getItemSignalPackgeLen(shTestItemLen);
    if(m_iPackgeLen < 0)
    {
        LOGERROR("m_iPackgeLen = -1 && shTestItem = " + to_string(shTestItemLen));
    }
}

void CDevHardwareBase::updateDevHardwareCode()
{

}

void CDevHardwareBase::initDevInfo()
{
    m_cHardwareCode = convertDevType2HardwareCode(m_shDevType);
    if(!chekHardwareCode(m_cHardwareCode))
    {
        LOGERROR("m_cHardwareCode invalid when m_shDevType =" + to_string(m_shDevType));
    }


    if(m_bSubElementIDIsDevID)
    {
        //测试单元和设备id一致
        for(auto &element : m_vecSubElement)
        {
            element->setElementID(m_iDevID);
        }
    }
}

void CDevHardwareBase::updateCmd(const cmdInfo &cmdInf)
{
    m_mutexCmd.lock();

    bool bAdd = true;
    //这里根据命令判断是否有重复命令,存在功能相反的命令需要删除
    if(cmdInf.shCmdType == DATA_FRAME_TYPE_SWORK || cmdInf.shCmdType == DATA_FRAME_TYPE_EWORK)
    {
        bool bStart = cmdInf.shCmdType == DATA_FRAME_TYPE_SWORK;
        //命令针对全部开始或者全部结束
        bool bJudgeElementIndex = cmdInf.shIndex == ELEMENT_NUM_CUST || cmdInf.shIndex == ELEMENT_NUM_CUST;

        for(auto it = m_listCmd.begin(); it != m_listCmd.end(); it++)
        {
            //存在相同的命令,忽略
            if(((it->shCmdType == DATA_FRAME_TYPE_SWORK && bStart) || (it->shCmdType == DATA_FRAME_TYPE_EWORK && !bStart))
                    && cmdInf.shIndex == it->shIndex)
            {
                if(bJudgeElementIndex)
                {
                    if(cmdInf.shIndex == it->shIndex)
                    {

                    }
                }
                else
                {

                }

                bAdd = false;
                break;
            }

            //存在相反的命令，删除即可
            if(((it->shCmdType == DATA_FRAME_TYPE_433_ENTER_CFG && !bStart) || (it->shCmdType == DATA_FRAME_TYPE_433_EXIST_CFG && bStart))
                    && cmdInf.shIndex == it->shIndex)
            {
                m_listCmd.erase(it);
                break;
            }
        }
    }
    //添加判断之后命令就不存在重复情况和相异的情况
    else if(cmdInf.shCmdType == DATA_FRAME_TYPE_433_ENTER_CFG || cmdInf.shCmdType == DATA_FRAME_TYPE_433_EXIST_CFG)
    {
        bool bEnter = cmdInf.shCmdType == DATA_FRAME_TYPE_433_ENTER_CFG;

        for(auto it = m_listCmd.begin(); it != m_listCmd.end(); it++)
        {
            //存在相同的命令
            if((it->shCmdType == DATA_FRAME_TYPE_433_ENTER_CFG && bEnter)
                    || (it->shCmdType == DATA_FRAME_TYPE_433_EXIST_CFG && !bEnter))
            {
                bAdd = false;
                break;
            }

            //存在相反的命令，删除即可
            if((it->shCmdType == DATA_FRAME_TYPE_433_ENTER_CFG && !bEnter)
                    || (it->shCmdType == DATA_FRAME_TYPE_433_EXIST_CFG && bEnter))
            {
                m_listCmd.erase(it);
                break;
            }
        }
    }
    //以前有这个命令那么对这个命令进行替换
    else
    {
        for(auto &cmd : m_listCmd)
        {
            if(cmd.shCmdType == cmdInf.shCmdType)
            {
                cmd = cmdInf;
                bAdd = false;
                break;
            }
        }
    }

    if(bAdd)
    {
        m_listCmd.push_back(cmdInf);
    }


    m_mutexCmd.unlock();
}

void CDevHardwareBase::deleteCmd(const cmdInfo &cmdInfo)
{
    m_mutexCmd.lock();

    bool bErase = false;
    auto it = m_listCmd.begin();

    for(; it != m_listCmd.end(); it++)
    {
        if(it->shCmdType == cmdInfo.shCmdType)
        {
            if(cmdInfo.shCmdType == DATA_FRAME_TYPE_EWORK)
            {
                bErase = true;
            }
            else
            {
                bErase = true;
            }

            if(bErase)
            {
                break;
            }
        }
    }

    if(bErase)
    {
        m_listCmd.erase(it);
    }

    m_mutexCmd.unlock();
}

list<cmdInfo> CDevHardwareBase::getCmd()
{
    m_mutexCmd.lock();
    auto listRet = m_listCmd;
    m_mutexCmd.unlock();

    return listRet;
}

void CDevHardwareBase::updateCmdTime(const cmdInfo &cmd)
{
    m_mutexCmd.lock();
    for(auto &element : m_listCmd)
    {
        if(cmd == element)
        {
            element.llLastSendTime = cmd.llLastSendTime;
        }
    }
    m_mutexCmd.unlock();
}

void CDevHardwareBase::onElementTestStateChange()
{
    //设备状态对外状态 若出现一个设备错误，那么就是 只有4种,测试未进行、测试中、测试完成、设备错误
    auto vecBindElement = getAllBindElements();

    //没有设备绑定，设置为未测状态
    if(vecBindElement.size() == 0)
    {
        m_objStateManage.setTestState(CTestStateManage::TEST_NOT_START);
        return;
    }

    //存在绑定设备，有两种，一种是内部设备开始，一种是未开始
    bool bBindElementStart = false;
    set<short> setTestState;
    for(auto &element : vecBindElement)
    {
        setTestState.insert(getElementState2devState(element->getTestState()));

        bBindElementStart |= element->getStartFlag();
    }

    //仅存在完成标志的变量表示测试完成
    auto setTemp = setTestState;
    setTemp.erase(CTestStateManage::TEST_COMPLETED);
    setTemp.erase(CTestStateManage::TEST_TIMEOUT);
    if(setTemp.size() == 0)
    {
        m_objStateManage.setTestState(CTestStateManage::TEST_COMPLETED);
        return;
    }

    short shTestState = CTestStateManage::TEST_NOT_START;
    //若存在错误，有两种，一种是未测，将设备设置为未测，一种是在测试中，将设备设置为错误
    if(setTestState.find(CTestStateManage::TEST_ERR) != setTestState.end())
    {
        shTestState = bBindElementStart ? CTestStateManage::TEST_ERR : CTestStateManage::TEST_NOT_START;
    }
    else if(setTestState.find(CTestStateManage::TEST_IN_PROCESS) != setTestState.end())
    {
        shTestState = CTestStateManage::TEST_IN_PROCESS;
    }
    //未测
    else
    {
    }
    m_objStateManage.setTestState(shTestState);
}

void CDevHardwareBase::setLightBelt(short shTestState, int nTestCode)
{
    unsigned short shMode = convertTestState2LightState(shTestState);

    QByteArray arrLedParam;
    CDevPowerUart::getLightBeltParam(arrLedParam, shMode, nTestCode);

    cmdInfo info;
    info.nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.shCmdType = CGenerationCommand::getLedCmd(info.arrCmd, arrLedParam, m_cHardwareCode, getMacID());
    info.setInteval(2500);

    qDebug() << "LightBeltState:"<< shMode;

    updateCmd(info);
}

void CDevHardwareBase::setStationFlag(bool bStation)
{
    m_bStationSte = bStation;

    //断连时间根据连接属性进行判断，直连2s 组网8s
    m_iNoDataInteval = m_bStationSte ? 8000 : 2000;

    //直连身高体重4s
    if(m_bStationSte && m_shDevType == DEV_TYPE_BODY)
    {
        m_iNoDataInteval = 4000;
    }

    CSubElementBase::s_bNotifyTimeOut = bStation;

    for(auto &element : m_vecSubElement)
    {
        element->setAntiCheatingAvailable(!bStation);
    }
}

bool CDevHardwareBase::setExTensionNo(const int &nExtensionNo)
{
    m_nExtensionNoTarget = nExtensionNo;
    return true;
}

unsigned short CDevHardwareBase::convertTestState2LightState(const short shTestState)
{
    unsigned short shTemp = LED_BELT_MODE_DEFUALT;
    switch (shTestState) {
    case CTestStateManage::TEST_IN_PROCESS:           // 测试中
        shTemp = LED_BELT_MODE_BODY_TESTING;
        break;
    case CTestStateManage::TEST_COMPLETED:    // 测试完成
        shTemp = LED_BELT_MODE_BODY_TEST_RESULT_LOCK;
        break;
    case CTestStateManage::TEST_ERR:           // 测试出错 ==》 体测犯规或系统错误
        shTemp = LED_BELT_MODE_BODY_TEST_FOUL;
        break;
    default:
        break;
    }
    return shTemp;
}

void CDevHardwareBase::enterCfg433()
{
    if(!CDevDataCfg::is433DevType(m_shDevType))
    {
        return;
    }

    if(m_vecSubElement.size() == 0)
    {
        return;
    }

    cmdInfo info;
    uint nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.shCmdType = CGenerationCommand::get433EnterCfg(info.arrCmd, nFrameNo, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, getMacID());

    qDebug() << "433进入 " << info.arrCmd.toHex();
    updateCmd(info);
}

void CDevHardwareBase::existCfg433()
{
    if(!CDevDataCfg::is433DevType(m_shDevType))
    {
        return;
    }

    if(m_vecSubElement.size() == 0)
    {
        return;
    }

    cmdInfo info;
    uint nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.shCmdType = CGenerationCommand::get433ExistCfg(info.arrCmd, nFrameNo, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, getMacID());

    qDebug() << "433退出 " << info.arrCmd.toHex();
    updateCmd(info);
}

void CDevHardwareBase::clearCfg433()
{
    if(!CDevDataCfg::is433DevType(m_shDevType))
    {
        return;
    }

    if(m_vecSubElement.size() == 0)
    {
        return;
    }

    cmdInfo info;
    uint nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.shCmdType = CGenerationCommand::get433ClearCfg(info.arrCmd, nFrameNo, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, getMacID());

    qDebug() << "433清空 " << info.arrCmd.toHex();
    updateCmd(info);
}

void CDevHardwareBase::setChannel433(uint nChannel)
{
    if(!CDevDataCfg::is433DevType(m_shDevType))
    {
        return;
    }

    if(m_vecSubElement.size() == 0)
    {
        return;
    }

    cmdInfo info;
    info.setInteval(500);
    uint nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.shCmdType = CGenerationCommand::get433SetChannel(info.arrCmd, nFrameNo, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, getMacID(), nChannel);

    qDebug() << "433设置 " << info.arrCmd.toHex();
    updateCmd(info);
}

void CDevHardwareBase::setIllegalTimes(const int &nTimes)
{
    if(nTimes == m_nIllTime)
    {
        return;
    }
    m_nIllTime = nTimes;

    cmdInfo info;
    info.setInteval(1500);
    info.nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.shCmdType = CGenerationCommand::getSetIllegalTimes(info, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, getMacID(), nTimes);

    info.nParam = static_cast<uint>(nTimes);

    qDebug() << "犯规次数设置 " << nTimes;
    updateCmd(info);
}

void CDevHardwareBase::onUpdateTimeOut()
{
    if(!isValid())
        return;

    //如果没有获取设备编号，那么发送信息获取设备编号
    timeOutGetExtensionNo();

    //判断设备断连
    timeOutCheckConnect();

    //发送命令队列中的数据
    if(isValid())
    {
        qint64 llCur = QDateTime::currentMSecsSinceEpoch();
        auto listCmd = getCmd();
        for(auto &cmd : listCmd)
        {
            if(llCur - cmd.llLastSendTime >= cmd.nInterval)
            {
                cmd.llLastSendTime = llCur;
                sendData(cmd.arrCmd, false, 300, COMMAND_LEVEL_1);
                updateCmdTime(cmd);
            }
        }
    }
}

bool CDevHardwareBase::chekHardwareCode(unsigned char &cHardwareCode)
{
    if(cHardwareCode >= HardwarePropertye::HARDWARE_CODE_SIZE)
    {
        return false;
    }

    return true;
}

void CDevHardwareBase::getTestStateFromPackgeData(CElementDataBase &elementData, unsigned char *pData)
{
    memcpy(&elementData.m_cTestState, pData + PACK_DATA_MARK, 1);
    //将状态位转为测试状态
    CDevDataCfg::getTestState(elementData.m_cTestState, elementData.m_cTestState);

    memcpy(&elementData.m_tTestState, pData + PACK_DATA_MARK, 2);
}

void CDevHardwareBase::getNormalData(float &fSrcData, const short &shTestItem)
{
    switch (shTestItem){
    //mm->cm
    case TEST_ITEM_HEIGHT:
        fSrcData /= 10;
        if(fSrcData < 0.0f)
            fSrcData = 0.0f;
        break;
    //mm->cm 坐位体前屈
    case TEST_ITEM_SFB:
        fSrcData /= 10;
        break;
    //ms->s
    case TEST_ITEM_RUN_800:
    case TEST_ITEM_RUN_1000:
    case TEST_ITEM_RUN_50:
    case TEST_ITEM_RUN_BACK_8_50:
    case TEST_ITEM_RUN_60:
    case TEST_ITEM_RUN_100:
    case TEST_ITEM_RUN_200:
    case TEST_ITEM_RUN_400:
    case TEST_ITEM_RUN_4_100:
    case TEST_ITEM_RUN_HURDLES_100:
    case TEST_ITEM_RUN_HURDLES_110:
    case TEST_ITEM_RUN_BACK_4_10:
    case TEST_ITEM_RUN_2000:
        fSrcData /= 1000;
        break;
    default:
        break;
    }
}

void CDevHardwareBase::onTestStateChange(short shTestState)
{
    //设备状态变更，根据状态设置设备灯光
    if(m_shDevType == DEV_TYPE_BODY)
    {
        setLightBelt(shTestState);
    }

    m_bGetDataFlag = false;

    if(shTestState == CTestStateManage::TEST_IN_PROCESS)
    {
        m_bGetDataFlag = true;
    }

    emit signalTestStateChange(this, shTestState);
}

