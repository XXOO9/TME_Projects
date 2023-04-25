#include "CDevSteRunMid.h"
#include "SteSubElement/CSubElementRunMid.h"
#include "CDevDataCfg.h"

////设备类型(1字节)
//const short DEVICE_TYPE = 0;
////数据点数(1字节)
//const short DATA_POINTS = 1;
////身份码(4字节)
//const short ID_CODE = 2;
////数据类型(2字节)
//const short DATA_TYPE = 6;
////数据状态(2字节)
//const short DATA_STATUS = 8;
////时间戳(8字节)
//const short TIME_YEAR = 10;
//const short TIME_MONTH = 12;
//const short TIME_DAY = 13;
//const short TIME_HOUR = 14;
//const short TIME_MINUTE = 15;
//const short TIME_SECOND = 16;
////const short TIME_OTHER = 17;
////短跑时间(4字节)
const short RUN_TIME = 18;
////卡号ID(4字节)
const short RUN_CARD = 22;
////字节标志(2字节),表明当前设备所代表的中长跑数据类型
const short RUN_FLAG = 26;

QByteArray CDevSteRunMid::s_arrCmdGetParam;

CDevSteRunMid::CDevSteRunMid(unsigned int iDevID, ICommunicate &communicate)
    : CDevHardwareBase (iDevID, communicate)
    , m_shDataLen(sizeof (DEV_RUNWAY_INFO))
{
    m_shDevType = DEV_TYPE_RUN_MID;
    m_bSubElementIDIsDevID = false;

    initSubElement<CSubElementRunMid>(SubElementPropertye::ELEMENT_NUM_Run_Mid);

    m_iNoDataInteval = 8000;

    initDevInfo();
}

CDevSteRunMid::~CDevSteRunMid()
{

}

bool CDevSteRunMid::devOpen()
{
    //只有在拥有独立通讯机制的情况下进行打开设备
    if(m_bIndependent)
    {
        CDevice::devOpen();
    }
    //非独立设备也需要打开对应的定时器
    else
    {
        m_bOpened = true;
        setUpdateDataState(true);
    }

    return true;
}

bool CDevSteRunMid::checkRunCardBindState(const unsigned int &nCardId)
{
    return m_mapCardAndElementMapping.find(nCardId) != m_mapCardAndElementMapping.end();
}

bool CDevSteRunMid::setElementBindFlag(const unsigned short &shIndex, bool bBind)
{
    if(shIndex >= m_vecSubElement.size())
        return true;

    bool bRet = CDevHardwareBase::setElementBindFlag(shIndex, bBind);

    //在外部进行绑卡信息时就绑定了对应绑定测试单元信息，避免不同地方进行设置造成的信息设置差异
    //这里只进行卡号的解绑操作
    if(!bBind)
    {
        for(auto it = m_mapCardAndElementMapping.begin(); it != m_mapCardAndElementMapping.end();)
        {
            if(it->second == shIndex)
            {
                updateStopCardContent(it->first, true);
                it = m_mapCardAndElementMapping.erase(it);
                continue;
            }

            it++;
        }

        checkViaPtDev();
    }
    else
    {
        auto &nCardID = getElement(shIndex)->getElementID();
        updateStopCardContent(nCardID, false);
    }

    //解绑途经点的所有信息
    if(m_shRunMidDevType == RUN_MID_TYPE_DESTINATION)
    {
        for(auto &element : m_setDevViaPt)
        {
            element->setElementBindFlag(shIndex, bBind);
        }
    }

    return bRet;
}

bool CDevSteRunMid::startWork(bool bCast, bool bWait/* = true*/)
{
    m_setRebindCardID.clear();

    if(m_pCom == nullptr)
    {
        LOGERROR("communication invalid");
        return false;
    }

    auto elementSize = m_vecSubElement.size();
    if(m_mapCardAndElementMapping.size() == 0)
    {
        LOGWARN("element size:" + to_string(elementSize) + " m_mapCardAndElementMapping size:" + to_string(m_mapCardAndElementMapping.size()));
        return false;
    }

    //删除停止队列中的卡号
    for(auto &element : m_mapCardAndElementMapping)
    {
        updateStopCardContent(element.first, false);
    }

    //中长跑开启设备仅存在一个设备，调用广播开始全部测试,途经点不需要发送指定的命令，只需要判断是否开启成功即可
    QByteArray arr;
    unsigned int nCardNo = 0;
    m_shLastCommand = CGenerationCommand::getCmdSworkRunMid
            (arr, CDevDataCfg::getFrameTypeByTestItem(TEST_ITEM_RUN_800), m_cHardwareCode, /*m_iDevID*/0, nCardNo);

    qDebug() << "startWork" << arr.toHex();

    simulationViaSendCmd(m_shLastCommand);
    bool bRet = sendData(arr, bWait, 1000, COMMAND_LEVEL_1);
    bRet &= getViaSimulationSendFlag();

    if(bRet)
    {
        long long llTimeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

        setStartSucessElementInfo(llTimeStamp);
        emit signalStartAllElement(getMacID());
    }

    return bRet;
}

bool CDevSteRunMid::stopWork(bool bCast, bool bWait)
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

    //删除停止队列中的卡号
    for(auto &element : m_mapCardAndElementMapping)
    {
        updateStopCardContent(element.first, true);
    }

    //初始所有测试类型都一致，当 测试单元id为0时代表开启所有设备进行测试
    //同时设备id也填0
    QByteArray arr;
    unsigned int nDevID = bCast ? 0 : m_iDevID;
    unsigned int nCardNo = 0;
    m_shLastCommand = CGenerationCommand::getCmdEworkRunMid(arr, m_vecSubElement[0]->getFrameType(), m_cHardwareCode, nDevID, nCardNo);
    qDebug() << "stopWork" << arr.toHex();

    return sendData(arr, bWait, 500, COMMAND_LEVEL_1);
}

bool CDevSteRunMid::startWork(unsigned short shElementIndex, bool bWait)
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

    m_setRebindCardID.erase(nSubID);

    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdSworkRunMid(arr, pElemet->getFrameType(), m_cHardwareCode, /*m_iDevID*/0, nSubID);

    qDebug() << "startWork" << arr.toHex();

    //在接收命令时会变更
    m_nRunMidCardNo = 1;
    for(auto &element : m_mapCardAndElementMapping)
    {
        if(element.second == shElementIndex)
        {
            m_nRunMidCardNo = element.first;
        }
    }

    if(m_nRunMidCardNo == 1)
    {
        LOGERROR("shElementIndex not exist card id ");
        m_nRunMidCardNo = 0;
    }

    //删除停止队列中的卡号
    updateStopCardContent(m_nRunMidCardNo, false);

    //模拟发送命令，途经点只需要接收命令成功失败即可，需要全部设备都开启成功
    simulationViaSendCmd(m_shLastCommand);

    bool bRet = sendData(arr, bWait, 1000, COMMAND_LEVEL_1);
    bRet &= getViaSimulationSendFlag();

    if(bRet)
    {
        //设置测试单元命令开启时间
        reinterpret_cast<CSubElementRunMid *>(pElemet)->updateStartCmdSendTime();

        //中长跑需要通知是否真正开启时间，信号需要直连
        static CLocation location;
        location.m_iSteDevMacID = m_iDevID;
        location.m_shElementIndex = shElementIndex;
        emit signalStartElement(location);
    }

    return bRet;
}

bool CDevSteRunMid::stopWork(unsigned short shElementIndex, bool bWait)
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

    //将卡号添加到停止队列当中
    updateStopCardContent(nSubID, true);
    return true;
}

bool CDevSteRunMid::bindCard(const unsigned int &nCardId, short shAttemptsNum, const unsigned short &shElementIndex)
{
    if(shElementIndex >= m_vecSubElement.size())
        return false;

    QByteArray arr;
    //生成广播类型的绑卡命令
    m_shLastCommand = CGenerationCommand::getCmdRunMidRecordCard(arr, nCardId, m_cHardwareCode, /*m_iDevID*/0);

    qDebug() << "bindCard" << arr.toHex();

    //发送开始工作需要等待底层命令返回
    setRunMidCardNo(nCardId);

    //若当前设备是终点设备那么需要开启所有途经点设备，途经点只需要模拟发送命令就行底层会统一返回
    bool bRet = true;
    simulationViaSendBindCard(nCardId);

    for(short i = 0; i < shAttemptsNum; i++)
    {
        bRet = sendData(arr, true, 1000, COMMAND_LEVEL_1);

        if(!bRet)
            continue;

        //若是终点设备，需要判断途经点
        bRet &= getViaSimulationSendFlag();

        //若绑定成功，内部同时设置了途经点信息
        if(bRet)
        {
            setCardAndElementMapping(nCardId, shElementIndex);
            break;
        }
    }

    return bRet;
}

bool CDevSteRunMid::setViaPtDev(set<CDevSteRunMid *> &setDev)
{
    if(m_setDevViaPt == setDev)
        return true;

    //当前存在录入信息或者正在测试中返回false
    if(m_mapCardAndElementMapping.size() > 0 || m_objStateManage.getTestState() == CTestStateManage::TEST_IN_PROCESS)
    {
        return false;
    }

    m_setDevViaPt = setDev;
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
  **************************************************/
int CDevSteRunMid::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    if(s_arrCmdGetParam.isEmpty())
    {
        CGenerationCommand::getCmdNew(s_arrCmdGetParam, DATA_FRAME_DEV_PARAM_GET, DATA_FRAME_TYPE_RUN_MID, m_cHardwareCode, m_iDevID, 0);
    }

    int nLen = CDevHardwareBase::parseSinglePackage(iFrameNo, pData, iDatalen);

    //针对当前设备的某些命令,处理中长跑断连之后的重连
    if(nLen > 0)
    {
        unsigned short shCmd;
        memcpy(&shCmd, pData + PACK_CMD_TYPE, 2);

        shCmd = getNormalCmdFromOrigCmd(shCmd);
        if(shCmd == DATA_FRAME_DEV_PARAM_GET && m_bSendGetParamCmd)
        {
            QByteArray arr(reinterpret_cast<char *>(pData), iDatalen);
            qDebug() << arr.toHex();;
            unsigned int nParam = 0;
            memcpy(&nParam, pData + iDatalen - 4, 4);

            //参数为1表示设备连接上之后还存在绑定的id，证明设备未断电只是信号不好断连
            if(nParam)
            {
                for(auto &element : m_vecSubElement)
                {
                    element->restoreLastNormalState();
                }
            }

            m_bSendGetParamCmd = false;
        }
    }

    if(nLen > 0)
        return nLen;

    //到这一步能保证数据为数据帧中的 单包数据，只进行当前数据解析即可

    //检测数据有效性
    if(!checkSignalPackageValid(pData, iDatalen))
        return iDatalen;

    //解析数据设置到测试单元中
    static DEV_RUNWAY_INFO runwayInfo;

    //目前仅需要获取其中的卡号id和时间即可，其他暂时不用解析

    //获取卡号id
    memcpy(&runwayInfo.uiIDCode, pData + RUN_CARD, sizeof (unsigned int));

    //获取中长跑设备编号
    if(m_shRunMidDevNo == RUN_MID_TYPE_INVALID)
    {
        memcpy(&m_shRunMidDevNo, pData + RUN_FLAG, 2);

        switch (m_shRunMidDevNo) {

        case RUN_MID_TYPE_DESTINATION:
            m_shRunMidDevType = m_shRunMidDevNo;
            break;
        default:
            if(m_shRunMidDevNo >= RUN_MID_TYPE_COUNT)
            {
                m_shRunMidDevType = RUN_MID_TYPE_COUNT;
            }
            else
            {
                m_shRunMidDevType = RUN_MID_TYPE_VIA_PT;
            }
            break;
        }

        LOGWARN("^^^^^ m_shRunMidDevNo" + to_string(m_shRunMidDevNo));
    }

    //解析编号之后才进行设备连接通知
    if(!m_bValidDev)
    {
        //数据帧中的设备id才是当前设备的id，此时才算设备连接上
        emit signalDevStatus(true, this);
        m_bValidDev = true;
    }

    //若对应卡号不在已经绑定的列表中，那么忽略当前信息
    //一般来说出现此问题是软件卡号的绑定和底层软件卡号绑定出现问题
    auto itFind = m_mapCardAndElementMapping.find(runwayInfo.uiIDCode);
    if(itFind == m_mapCardAndElementMapping.end())
    {
        if(runwayInfo.uiIDCode)
        {
            LOGWARN("card id not bind" + to_string(runwayInfo.uiIDCode));
        }

        static set<unsigned short> setTemp;
        setTemp.clear();

        //中长跑的默认数据卡号为0，若设备断连过一次
        if(m_bCheckReopen)
        {
            for(auto &element : m_vecSubElement)
            {
                if(element->getTestState() == CTestStateManage::TEST_ERR)
                {
                    //在测试中不进行处理
                    if(element->getStartFlag())
                    {
                        continue;
                    }

                    setTemp.insert(element->getIndex());
                    //人员绑定同时不在测试状态
                    if(element->getBindFlag())
                    {
                        //需要重新
                        for(auto &cardElement : m_mapCardAndElementMapping)
                        {
                            if(cardElement.second == element->getIndex())
                            {
                                m_setStopCardID.erase(cardElement.first);
                                m_setRebindCardID.insert(cardElement.first);
                                setTemp.erase(element->getIndex());
                                break;
                            }
                        }
                    }
                    else
                    {
                        //将状态设置为正常
                        setTemp.erase(element->getIndex());
                        element->setTestState(CTestStateManage::TEST_NOT_START);
                    }
                }
            }

            for(auto &element : setTemp)
            {
                LOGERROR("&&&&&& element index =" + to_string(element));
                setTestState(element, CTestStateManage::TEST_NOT_START);
            }

            m_bCheckReopen = false;
        }

        return m_iPackgeLen;
    }

    CSubElementBase *pElement = getElement(itFind->second);
    if(pElement == nullptr)
    {
        LOGERROR("pElement Err invalid");
        return m_iPackgeLen;
    }

    //获取通过时间
    memcpy(&runwayInfo.fRunTime, pData + RUN_TIME, sizeof (float));

    //将时间转为标准值
    getNormalData(runwayInfo.fRunTime, TEST_ITEM_RUN_1000);

    LOGWARN("card id =" + to_string(runwayInfo.uiIDCode) + " time :" + to_string(runwayInfo.fRunTime) + " type :" + to_string(m_shRunMidDevNo));

    //将数据设置到对应测试单元
    m_vecSubElement[itFind->second]->setElementInfo(&runwayInfo, m_shDataLen);

    return m_iPackgeLen;
}

void CDevSteRunMid::onElementTestStateChange()
{
    CDevHardwareBase::onElementTestStateChange();

    //当前设备时终点设备需要设置对应的设备状态一致
    if(getRunMidDevType() == RUN_MID_TYPE_DESTINATION)
    {
        auto vecElement = getAllBindElements();
        CSubElementBase *pElement;

        for(auto &elementDev : m_setDevViaPt)
        {
            //将当前设备状态设置到所有途经点中
            for(auto &element : vecElement)
            {
                pElement = elementDev->getElement(element->getIndex());
                if(pElement != nullptr)
                {
                    pElement->setTestState(element->getTestState());
                }
            }
        }
    }

    checkViaPtDev();
}

void CDevSteRunMid::checkViaPtDev()
{
    if(m_shRunMidDevType == RUN_MID_TYPE_DESTINATION)
    {
        if(m_mapCardAndElementMapping.size() == 0 && getTestState() == CTestStateManage::TEST_IN_PROCESS)
        {
            emit signalViaPtDevCheck();
        }
    }
}

void CDevSteRunMid::setCardAndElementMapping(const unsigned int &nCardNo, const unsigned short &shElementIndex)
{
    for(auto it = m_mapCardAndElementMapping.begin(); it != m_mapCardAndElementMapping.end();)
    {
        if(it->second == shElementIndex)
        {
            it = m_mapCardAndElementMapping.erase(it);
            continue;
        }

        it++;
    }

    //绑定卡号与对应测试单元信息
    m_mapCardAndElementMapping[nCardNo] = shElementIndex;
    m_vecSubElement[shElementIndex]->setElementID(nCardNo);

    //当前设备为终点设备需要设置途经点信息
    if(getRunMidDevType() != RUN_MID_TYPE_DESTINATION)
        return;

    for(auto &element : m_setDevViaPt)
    {
        element->setCardAndElementMapping(nCardNo, shElementIndex);
    }
}

void CDevSteRunMid::setStartSucessElementInfo(const long long &llTimeStamp)
{
    CSubElementRunMid *pElement;
    for(auto &element : m_mapCardAndElementMapping)
    {
        pElement = reinterpret_cast<CSubElementRunMid *>(m_vecSubElement[element.second]);
        pElement->setStartCmdSendTime(llTimeStamp);
    }

    //若当前设备是终点设备，设置对应的途经点
    if(getRunMidDevType() != RUN_MID_TYPE_DESTINATION)
        return;

    for(auto &element : m_setDevViaPt)
    {
        element->setStartSucessElementInfo(llTimeStamp);
    }
}

set<unsigned int> CDevSteRunMid::getStopCardID()
{
    set<unsigned int> setTemp;
    m_mutexStopCardID.lock();
    setTemp = m_setStopCardID;
    m_mutexStopCardID.unlock();

    return setTemp;
}

void CDevSteRunMid::setStopCardID(const set<unsigned int> &setTemp)
{
    m_mutexStopCardID.lock();
    m_setStopCardID = setTemp;
    m_mutexStopCardID.unlock();
}

void CDevSteRunMid::onUpdateTimeOut()
{
    CDevHardwareBase::onUpdateTimeOut();

    if(!m_bValidDev)
    {
        m_bCheckReopen = true;
    }

    //每次发送一个停止，完成之后就删除队列中的卡号
    auto setStopIDTemp = getStopCardID();
    if(setStopIDTemp.size() > 0)
    {
        auto nCardId = *setStopIDTemp.begin();
        m_nStopCmdParam = nCardId;

        if(stopCardID(nCardId, true))
        {
            updateStopCardContent(nCardId, false);
            LOGINFO("run mid del CardId = " + to_string(nCardId));
        }
    }

    if(m_setRebindCardID.size() > 0)
    {
        auto nCardId = *m_setRebindCardID.begin();
        m_nStopCmdParam = nCardId;

        auto itFind = m_mapCardAndElementMapping.find(nCardId);
        if(itFind != m_mapCardAndElementMapping.end())
        {
            auto elementIndex = itFind->second;
            //绑卡会重新操作m_mapCardAndElementMapping
            if(bindCard(nCardId, 1, elementIndex))
            {
                auto pElement = getElement(elementIndex);
                if(pElement != nullptr && !pElement->getStartFlag())
                {
                    pElement->setTestState(CTestStateManage::TEST_NOT_START);
                }
                m_setRebindCardID.erase(nCardId);
            }
        }
        else
        {
            LOGERROR("card id rebind not exist id = " + to_string(nCardId))
        }
    }

    timeOutSendGetParamCmd();
}

void CDevSteRunMid::simulationViaSendCmd(const unsigned short &shCmd)
{
    if(getRunMidDevType() == RUN_MID_TYPE_DESTINATION)
    {
        //广播发送只需要知道所有的途经点是否发送成功就行，模拟发送
        for(auto &elementDev : m_setDevViaPt)
        {
            elementDev->setLastCmd(shCmd);
        }
    }
}

void CDevSteRunMid::simulationViaSendBindCard(const unsigned int &nCardNo)
{
    if(getRunMidDevType() != RUN_MID_TYPE_DESTINATION)
        return;

    //广播发送只需要知道所有的途经点是否发送成功就行，模拟发送
    for(auto &elementDev : m_setDevViaPt)
    {
        elementDev->setLastCmd(DATA_FRAME_TYPE_READCARD);
        elementDev->setRunMidCardNo(nCardNo);
    }
}

bool CDevSteRunMid::getViaSimulationSendFlag()
{
    bool bRet = true;
    if(getRunMidDevType() == RUN_MID_TYPE_DESTINATION)
    {
        //广播发送只需要知道所有的途经点是否发送成功就行，模拟发送
        for(auto &elementDev : m_setDevViaPt)
        {
            bRet &= elementDev->getSendCmdFlag();

            if(!bRet)
                break;
        }
    }
    return bRet;
}

bool CDevSteRunMid::stopCardID(const unsigned int &nCardNo, bool bWait)
{
    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdEworkRunMid(arr, DATA_FRAME_TYPE_RUN_MID, m_cHardwareCode, m_iDevID, nCardNo);
    qDebug() << "stopWork" << arr.toHex();

    //模拟发送命令，途经点只需要接收命令成功失败即可，需要全部设备都关闭成功
    simulationViaSendCmd(m_shLastCommand);

    //所有设备关闭成功才关闭当前设备
    bool bRet = sendData(arr, bWait, 300, COMMAND_LEVEL_1);
    if(bRet && bWait)
    {
        bRet = getViaSimulationSendFlag();
    }

    return bRet;
}

void CDevSteRunMid::updateStopCardContent(const unsigned int &nCardNo, bool bAdd)
{
    if(getRunMidDevType() != RUN_MID_TYPE_DESTINATION)
        return;

    auto setTemp = getStopCardID();

    if(bAdd){
        setTemp.insert(nCardNo);
    }else {
        setTemp.erase(nCardNo);
    }

    setStopCardID(setTemp);
}

bool CDevSteRunMid::timeOutCheckConnect()
{
    bool bRet = CDevHardwareBase::timeOutCheckConnect();
    if(!bRet)
    {
        m_bSendGetParamCmd = true;
    }

    return bRet;
}

void CDevSteRunMid::timeOutSendGetParamCmd()
{
    if(m_bSendGetParamCmd)
    {
        m_iSendGetParamCmdIntevalTotal += m_iUpdateInteval;
        if(m_iSendGetParamCmdIntevalTotal >= 1000)
        {
            sendData(s_arrCmdGetParam, false, COMMAND_LEVEL_1);
            m_iSendGetParamCmdIntevalTotal -= 1000;
        }
    }
}
