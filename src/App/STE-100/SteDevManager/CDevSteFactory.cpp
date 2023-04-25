#include "CDevSteFactory.h"
#include "CLogger.h"
#include "CSerialPortCom.h"
//#include "CSerialPortHelper.h"
#include "CDevDataCfg.h"
#include "SteDevManager/SteDevice/CDevSteRunMid.h"

const static unsigned short s_shLenFH = sizeof(CONTENT_FH);
const static unsigned short s_shLenVersion = sizeof(CONTENT_VERSION);
const static unsigned short s_shLenCmd = sizeof(DataFrameInfo::CONTENT_CMD);
const static unsigned short s_shFrameLen = sizeof(DataFrameInfo::CONTENT_FRAME_LENTH);

QByteArray CDevSteFactory::s_arrSetDataReport(CGenerationCommand::getCmdSetDataReport(true));

CDevSteFactory::CDevSteFactory(short shIndex, QString strUnitName)
    :CDevSteBase ()
{
    //设备号，通讯接口名称
    m_shDevNO = shIndex;
    m_strComName = strUnitName;

    //构建通讯接口
    auto pSerialPortCom = new CSerialPortCom();

    pSerialPortCom->setReceiveQueSize(1024 * 3);

    //全部变更为正常发送
    pSerialPortCom->setUseQueueFlag(/*true*/false);

    pSerialPortCom->setComName(strUnitName);

    pSerialPortCom->setOpenMode(ICommunicate::ReadWrite);

    //串口通讯的相关参数设置
    //pSerialPortCom->setPortBaudRate();

    //初始化通讯接口
    setCommunicate(pSerialPortCom);

    //开启心跳检测是因为在平板上的直连设备没有设备插拔断连信息，只能通过数据的心跳来进行判断
    m_bDetermineDisconnect = true;

    //在factor中是判断设备是否有效时间
    m_iNoDataInteval = 2000;
}

CDevSteFactory::~CDevSteFactory()
{
    release();
}

/*************************************************
  <函数名称>    getAllDev()
  <功能描述>    获取全部设备
  <参数说明>

  <返 回 值>    无

  <函数说明>    在返回全部设备时，同时根据逻辑设备进行排序

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
map<int, CDevHardwareBase *> CDevSteFactory::getAllDev()
{
    map<int, CDevHardwareBase *> mapTemp;
    for(auto &element : m_mapDevices)
    {
        if(element.second->isValid())
        {
            mapTemp[element.first] = element.second;
        }
    }

    return mapTemp;
}

/*************************************************
  <函数名称>    getDevSteByID(unsigned int iDevID)
  <功能描述>    根据设备MacID获取具体体测设备
  <参数说明>

  <返 回 值>    无

  <函数说明>

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
CDevHardwareBase *CDevSteFactory::getDevSteByID(unsigned int iDevID)
{
    CDevHardwareBase *pRetDev = nullptr;

    for (auto &it : m_mapDevices)
    {
        if(it.second->getMacID() == iDevID)
        {
            pRetDev = it.second;
            break;
        }
    }

    return pRetDev;
}

bool CDevSteFactory::setNetworkChannelInfo(unsigned int nMaxNum, unsigned int nChannelNo)
{
    if(nChannelNo == 0)
    {
        LOGERROR("set channel No. 0");
        return true;
    }

    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdSetChannelInfo(arr, nMaxNum, nChannelNo);
    return sendData(arr, true, 1000);
}

bool CDevSteFactory::devOpen()
{
    m_nReceiveData = 0;
    m_bSendDataReport = true;

    nStartTimeSendDataReportTime = QDateTime::currentMSecsSinceEpoch();
    return CDevSteBase::devOpen();
}

void CDevSteFactory::devClose()
{
    m_bValidDev = false;

    if(!m_bOpened)
    {
        return;
    }

    CDevSteBase::devClose();

    //关闭所有底层设备
    CDevHardwareBase *pTemp;
    for(auto &element : m_mapDevices)
    {
        pTemp = element.second;
        if(pTemp != nullptr)
        {
            pTemp->devClose();
        }
    }
}

//内部调用函数不能轻易变更
void CDevSteFactory::onSteDevConnectChange(bool bConnect, void *pDev)
{
    //先发送设备断连通知
    emit signalHardwareConnectChange(bConnect, pDev);

    //处理中长跑设备连接，中长跑设备终点设备需要进行监测，将途经点设置到终点设备中
    dealRunMidDevConnectChange(bConnect, pDev);

    if(!bConnect)
    {
        //对无效设备不进行删除，内部含有标志可以进行判断
        //delDev(pDev);

        //直连设备有效设备数为0那么发送断连
        if(!m_bStationSte)
        {
            auto mapTemp = getAllDev();
            if(mapTemp.size() == 0)
                emit signalDevStatus(false, this);
        }
    }

    onRunMidCheckViaPtDev();
}

void CDevSteFactory::onRunMidCheckViaPtDev()
{
    //指向终点设备
    CDevSteRunMid *pTemp = nullptr;

    //记录下所有途经点
    set<CDevSteRunMid *> setDevViaPt, setDevDestination;

    for(auto &element : m_mapDevices)
    {
        pTemp = reinterpret_cast<CDevSteRunMid *>(element.second);

        if(pTemp == nullptr || !pTemp->isValid())
            continue;

        auto &shRunMidDevType = pTemp->getRunMidDevType();
        switch (shRunMidDevType) {
        case CDevSteRunMid::RUN_MID_TYPE_DESTINATION:
            setDevDestination.insert(pTemp);
            break;
        case CDevSteRunMid::RUN_MID_TYPE_VIA_PT:
            setDevViaPt.insert(pTemp);
            break;
        default:
            break;
        }
    }

    if(setDevDestination.size() > 1)
    {
        LOGERROR("Multiple RUN_MID_TYPE_DESTINATION devices exist");
    }

    for(auto &element : setDevDestination)
    {
        element->setViaPtDev(setDevViaPt);
    }
}

/*************************************************
  <函数名称>    isExistDev(unsigned int iDevID)
  <功能描述>    判定当前设备ID是否已经被创建
  <参数说明>    unsigned int iDevID 设备ID

  <返 回 值>    无

  <函数说明>

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
bool CDevSteFactory::isExistDev(unsigned int iDevID)
{
    return getDevSteByID(iDevID) != nullptr;
}

/*************************************************
  <函数名称>    release（）
  <功能描述>    释放工厂资源
  <参数说明>

  <返 回 值>    无

  <函数说明>

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
void CDevSteFactory::release()
{
    //删除全部设备
    devClose();

    set<CDevHardwareBase *> setDev;
    for(auto &element : m_mapDevices)
    {
        setDev.insert(element.second);
    }

    for (auto &element : setDev)
    {
        delDev(element);
    }

    auto *pCom = reinterpret_cast<CSerialPortCom *>(m_pCom);

    //删除之前必须设置一下空的com口
    setCommunicate(nullptr);
    if(pCom != nullptr)
    {
        safeDeleteClassPoint(&pCom);
    }

    m_mapDevices.clear();
}

void CDevSteFactory::replyStationHeartbeat(bool bJudgeStation)
{
    bool bSend = true;

    if(bJudgeStation)
    {
        bSend = isStaionSte();
    }

    if(bSend && isValid())
    {
        //注：目前仅在短跑时才会增加时间戳
        //if(m_shDevType == DEV_TYPE_RUN_SHORT)
        {
            QDate date(QDate::currentDate());
            unsigned short usYear = static_cast<unsigned short>(date.year());
            unsigned char ucMonth = static_cast<unsigned char>(date.month());
            unsigned char ucDay = static_cast<unsigned char>(date.day());
            QTime time(QTime::currentTime());
            unsigned char ucHour = static_cast<unsigned char>(time.hour());
            unsigned char ucMinute = static_cast<unsigned char>(time.minute());
            unsigned char ucSecond = static_cast<unsigned char>(time.second());
            unsigned char ucRemain = 0;

            //判断获取时间是否异常
            if(usYear < 2022)
            {
                LOGERROR("Base station timestamp synchronization abnormality");
            }

            unsigned char *pDestData = reinterpret_cast<unsigned char *>(m_arrCmdHeartbeat.data());
            memcpy(pDestData + 34, &usYear, sizeof(unsigned short));
            memcpy(pDestData + 36, &ucMonth, sizeof(unsigned char));
            memcpy(pDestData + 37, &ucDay, sizeof(unsigned char));
            memcpy(pDestData + 38, &ucHour, sizeof(unsigned char));
            memcpy(pDestData + 39, &ucMinute, sizeof(unsigned char));
            memcpy(pDestData + 40, &ucSecond, sizeof(unsigned char));
            memcpy(pDestData + 41, &ucRemain, sizeof(unsigned char));
        }

        sendData(m_arrCmdHeartbeat);
    }
}

CDevHardwareBase* CDevSteFactory::createSteDevcice(unsigned int iDevID, ICommunicate &comunicate, const short &shDevType)
{
    //中长跑设备的设备id是在内部接收到数据之后设置的
    auto pDevCreate = CDevDataCfg::createSteDevcice(iDevID, comunicate, shDevType);
    if(pDevCreate == nullptr)
    {
        LOGERROR("createSteDevcice false shDevType :" + to_string(shDevType) + "create dev err");
        return nullptr;
    }

    connect(pDevCreate, &CDevice::signalDevStatus, this, &CDevSteFactory::onSteDevConnectChange);
    pDevCreate->setStationFlag(m_bStationSte);
    m_mapDevices[m_nOderOfConnection++] = pDevCreate;

    return pDevCreate;
}

void CDevSteFactory::delDev(void *pDev)
{
    if(pDev == nullptr)
        return;

    CDevHardwareBase *pDevice = reinterpret_cast<CDevHardwareBase *>(pDev);
    disconnect(pDevice, &CDevice::signalDevStatus, this, &CDevSteFactory::onSteDevConnectChange);

    //删除映射关系
    CDevHardwareBase *pDevTemp;
    for(auto it = m_mapDevices.begin(); it != m_mapDevices.end();)
    {
        pDevTemp = it->second;
        if(pDevTemp == pDev)
        {
            pDevTemp->devClose();
            //等待设备可以进行删除
            while (!pDevTemp->isDeleteThis())
            {
                CDevDataCfg::delayMSec(5);
            }

            safeDeleteClassPoint(&it->second);
            it = m_mapDevices.erase(it);
            break;
        }
        else
        {
            it++;
        }
    }
}

void CDevSteFactory::sendAck()
{

}

void CDevSteFactory::timeoutCheckValidDev()
{
    if(m_bValidDev)
        return;

    m_iCheckDevValidTotal += m_iUpdateInteval;
    //如果超过没有数据累计时间间隔，则认为设备不是工作站
    if (m_iCheckDevValidTotal >= m_iNoDataInteval)
    {
        //如果超出一定时间，还是不可用设备则发送通知
        m_iCheckDevValidTotal -= m_iNoDataInteval;

        if(!m_bValidDev)
        {
            m_bConfirmConnect = true;
            emit signalDevError(404, m_strComName);
        }
    }
}

void CDevSteFactory::dealRunMidDevConnectChange(bool bConnect, void *pDev)
{
    if(getDevType() != DEV_TYPE_RUN_MID)
        return;

    CDevSteRunMid *pDevChanges = reinterpret_cast<CDevSteRunMid *>(pDev);
    if(pDevChanges->getRunMidDevType() == CDevSteRunMid::RUN_MID_TYPE_DESTINATION)
    {
        if(bConnect)
        {
            connect(pDevChanges, &CDevSteRunMid::signalViaPtDevCheck, this, &CDevSteFactory::onRunMidCheckViaPtDev);
        }
        else
        {
            disconnect(pDevChanges, &CDevSteRunMid::signalViaPtDevCheck, this, &CDevSteFactory::onRunMidCheckViaPtDev);
        }
    }
}

/*************************************************
  <函数名称>    dataParse(QByteArray &array)
  <功能描述>    数据解析过程
  <参数说明>    QByteArray &array 原始数据

  <返 回 值>

  <函数说明>    对于有独立通讯的设备，才能构建数据解析过程

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
void CDevSteFactory::dataParse(QByteArray &array)
{
    if(m_iExistCacheSize == 0)
        return;

    int iRemainDatalen;
    unsigned char *pData = reinterpret_cast<unsigned char *>(array.data());
    unsigned char *pTemp;

    //数据头 版本信息 命令码
    unsigned short shFrameHead, shVersion, shCmd;

    int i = 0;

    bool bParseHead;
    for(; i < m_iExistCacheSize; i++)
    {
        iRemainDatalen = m_iExistCacheSize - i;
        pTemp = pData + i;

        //数据长度需要大于等于公共数据包长度
        if(iRemainDatalen < DataFrameInfo::INDEX_COMMON_DATA_HEAD_LEN)
        {
            break;
        }

        //判定数据帧头，判断帧头 版本 命令码
        memcpy(&shFrameHead, &pTemp[DataFrameInfo::INDEX_FH], s_shLenFH);
        memcpy(&shVersion, &pTemp[DataFrameInfo::INDEX_VERSION], s_shLenVersion);
        memcpy(&shCmd, &pTemp[DataFrameInfo::INDEX_CMD], s_shLenCmd);

        if(shFrameHead == DataFrameInfo::CONTENT_FH && shVersion == DataFrameInfo::CONTENT_VERSION \
                && shCmd == DataFrameInfo::CONTENT_CMD)
        {
            //帧头解析中会对 m_shFrameLength 、 m_bIgnoreCurFrameData  进行赋值
            bParseHead =  parseFrameHeader(pTemp, iRemainDatalen);

            //数据头没解析成功（数据长度不够）或者数据长度不够
            if(!bParseHead || m_shFrameLength > iRemainDatalen)
            {
                break;
            }

            //if((s_iCurFrameNo != m_nFirstFrameDataNo && !m_bStationSte) || m_bStationSte)
            {
                if(!parseDataPackage(pTemp + DataFrameInfo::INDEX_COMMON_DATA_HEAD_LEN \
                             , m_shFrameLength - DataFrameInfo::INDEX_COMMON_DATA_HEAD_LEN))
                {
                QByteArray byteSourceData(reinterpret_cast<char *>(pData), m_shFrameLength);
                qDebug()<< "parseDataPackage false :" << byteSourceData.toHex();
                LOGERROR("parseDataPackage false :" + byteSourceData.toHex().toStdString());
                }
            }

            //下一次i自加1这里减去
            i += (m_shFrameLength - 1);
        }
    }

    //i 为当前解析的数据长度
    if(i > 0)
    {
        if(i > m_iExistCacheSize)
        {
            m_iExistCacheSize = 0;
        }
        else
        {
            m_iExistCacheSize -= i;
            memcpy(pData, pData + i, static_cast<unsigned int>(m_iExistCacheSize));
        }
    }
}

/*************************************************
  <函数名称>    parseFrameHeader(int iStartPos, QByteArray &array)
  <功能描述>    数据解析帧头过程,帧数据正确解析才进行其中单包数据解析
  <参数说明>    int iStartPos 开始位置
               QByteArray &array 原始数据

  <返 回 值>
               返回值表示当前帧数据长度

  <函数说明>    对于有独立通讯的设备，才能构建数据解析过程

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
bool CDevSteFactory::parseFrameHeader(unsigned char *pData, int iDatalen)
{
    m_bIgnoreCurFrameData = false;
    //保证数据能解析出数据长度
    if(pData == nullptr || iDatalen < INDEX_RESERVED)
    {
        LOGERROR("pData == nullptr || iDatalen < 14 iDatalem = " + to_string(iDatalen));
        return false;
    }
    //获取当前数据帧数据长度
    memcpy(&m_shFrameLength, &pData[INDEX_FRAME_LENTH], s_shFrameLen);

    //获取帧号
    unsigned int nCurFrameNo;
    memcpy(&nCurFrameNo, pData + INDEX_FRAME_NO, 4);

    //本帧数据和上一帧数据重复，忽略帧数据
    //m_bIgnoreCurFrameData = false;
    m_bIgnoreCurFrameData = nCurFrameNo == s_iCurFrameNo && s_iCurFrameNo != 0;

    //qDebug() << "frame no =" << nCurFrameNo;
    if(nCurFrameNo - s_iCurFrameNo != 1)
    {
        //LOGWARN("last frame no =" + to_string(s_iCurFrameNo) + " cur frame no = " + to_string(nCurFrameNo));
    }

    if(m_bIgnoreCurFrameData)
    {
        //LOGWARN("IgnoreCurFrameData");
    }

    s_iCurFrameNo = nCurFrameNo;

//    qDebug() << "frame no :" << s_iCurFrameNo << " bIgnoreCurFrameData :" << m_bIgnoreCurFrameData;

    //若帧类型足以解析数据类型，那么判断类型进行心跳回复
    if(m_shFrameLength >= PACK_DATA_MARK + INDEX_COMMON_DATA_HEAD_LEN)
    {
        //获取命令码
        unsigned short shCmd;
        memcpy(&shCmd, pData + DataFrameInfo::INDEX_COMMON_DATA_HEAD_LEN + PACK_CMD_TYPE, sizeof (CONTENT_CMD_TYPE));

        //获取原始命令
        shCmd = getNormalCmdFromOrigCmd(shCmd);

        //中长跑打印原始数据和帧号
        if(shCmd == DATA_FRAME_TYPE_GET_DATA /*&& getDevType() == DEV_TYPE_RUN_MID*/ && m_bIgnoreCurFrameData)
        {
            //QByteArray arrTemp(reinterpret_cast<char *>(pData), m_shFrameLength);
            //LOGINFO("Frame No：" + to_string(s_iCurFrameNo)  + " data:" + arrTemp.toHex().toStdString());
        }

        //如果是基站数据
        if(shCmd == DATA_FRAME_NETWORK_HEARTBEAT)
        {
            replyStationHeartbeat(false);
        }
        //是组网才进行回复
        else
        {
            replyStationHeartbeat(true);
        }
    }

    return true;
}

/*************************************************
  <函数名称>    parseDataPackage(int iStartPos, QByteArray &array)
  <功能描述>    解析数据包
  <参数说明>    int iStartPos 数据开始位置
              QByteArray &array 原始数据

  <返 回 值>    将不正确的数据都返回false

  <函数说明>    在解析到设备时需要构建纯虚拟的体测具体设备

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
bool CDevSteFactory::parseDataPackage(unsigned char *pData, int iDatalen)
{
    if(pData == nullptr)
        return false;

    unsigned char cHardwareCode;
    unsigned char *pPackStart;
    int iPackLen;
    unsigned int iDevID;
    CDevHardwareBase *pDev;
    short shDevType;

    if( CInterAction::Get_Instance().getIsDeveloper() )
    {
        QByteArray arrTemp(reinterpret_cast<char *>(pData), iDatalen);
        qDebug() << arrTemp.toHex();
        LOGINFO( "parseDataPackage false : + arrTemp = " + QString::fromLocal8Bit( arrTemp.toHex() ).toStdString() );
    }

    bool bOpendDev;

    ///按照概率排列if elese分支使程序更快速
    //正常单包数据长度才进行设备创建，设备类型解析
    if(iDatalen > PACK_DATA_MARK)
    {
        int i = 0;
        for(; i < iDatalen; i++)
        {
            pPackStart = pData + i;

            //硬件编码
            memcpy(&cHardwareCode, pPackStart + PACK_HARDWARE_CODE, 1);
            shDevType = convertHardwareCode2DevType(cHardwareCode);
            //身份ID，4Byte
            memcpy(&iDevID, pPackStart + PACK_HARDWARE_ID, 4);

            if(shDevType == DEV_TYPE_UNKNOW)
            {
                LOGERROR("hardwareCode :" + to_string(cHardwareCode) + " devType : " + to_string(m_shDevType));
                return false;
            }

            //设备未进行确认时需要判断根据标志判断当前设备是否为站点设备，判断依据是 数据为查询信息的回复命令
            if(!m_bConfirmOperation || shDevType == DEV_TYPE_NETWORK)
            {
                iPackLen = parseSinglePackage(s_iCurFrameNo, pPackStart, iDatalen - i);
            }
            else
            {
                //有正常数据就将数据上报设置为false
                m_bSendDataReport = false;

                // 若是跳绳 跳绳设备id和当前工厂id一致 只有跳绳数据比较特殊
                if(m_shDevType == DEV_TYPE_ROPE_SKIP || m_shDevType == DEV_TYPE_RUN_SHORT)
                    iDevID = m_iDevID;

                pDev = getDevSteByID(iDevID);
                //设备不存在
                bOpendDev = false;
                if(pDev == nullptr)
                {
                    bOpendDev = false;
                    pDev = createSteDevcice(iDevID, *m_pCom, shDevType);
                }
                else
                {
                    bOpendDev = pDev->isValid();
                }

                if(pDev != nullptr)
                {
                    if(!bOpendDev)
                    {
                        pDev->devOpen();
                    }
                    //对于具体设备，就把完整的一包数据发给设备，让具体设置自己解析
                    //返回一包数据长度（含数据包和单点数据）

                    iPackLen = pDev->parseSinglePackage(s_iCurFrameNo, pPackStart, iDatalen - i);
                }
                else
                {
                    return iDatalen;
                }
            }

            if(iPackLen > 0)
            {
                i += (iPackLen - 1);
            }
            else
            {
                LOGERROR("parse len err hardwareCode :" + to_string(cHardwareCode) + "devType :" + to_string(m_shDevType) + "parseSinglePackage err");
                return false;
            }
        }

        if(i != iDatalen)
        {
            LOGERROR("parsedata err Please rearrange the logic");
            return false;
        }
    }
    //长度不足以解析出设备号
    else if(iDatalen < PACK_CMD_TYPE)
    {
        //表示限制传入对应类型设备
        bool bLimitToSend = false;
        short shDevType = DEV_TYPE_UNKNOW;

        //跳绳绳子没电，直接发送给跳绳设备处理
        if(iDatalen == 3 && pData[0] == 0xEB && pData[1] == 0x04 && pData[2] ==0x88)
        {
            bLimitToSend = true;
            shDevType = DEV_TYPE_ROPE_SKIP;
        }

        if(m_mapDevices.size() == 0)
            return false;

        for(auto &element : m_mapDevices)
        {
            if(bLimitToSend)
            {
                if(element.second->getDevType() != shDevType)
                {
                    continue;
                }
            }

            element.second->parseSinglePackage(s_iCurFrameNo, pData, iDatalen);
        }

        return false;
    }
    //仅能解析出id
    else if(iDatalen < PACK_DATA_MARK)
    {
        memcpy(&iDevID, pData + PACK_HARDWARE_ID, 4);
        pDev = getDevSteByID(iDevID);
        if(pDev != nullptr)
        {
            pDev->parseSinglePackage(s_iCurFrameNo, pData, iDatalen);
        }

        return false;
    }

    return true;
}

/*************************************************
  <函数名称>    parseSinglePackage(int iStartPos, QByteArray &array)
  <功能描述>    解析单点数据
  <参数说明>    int iStartPos 数据开始位置
              QByteArray &array 原始数据

  <返 回 值>    无

  <函数说明>    只处理与工作站有关的单点数据，在首次连接工作站时，需要调用
  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
int CDevSteFactory::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    //长度不足以解析命令
    if(iDatalen < PACK_DATA_MARK + 1)
    {
        LOGERROR("Not long enough to parse the command");
        return iDatalen;
    }

    if(iFrameNo == 0)
    {

    }

    //获取命令码
    unsigned short shCmd;
    memcpy(&shCmd, pData + PACK_CMD_TYPE, sizeof (CONTENT_CMD_TYPE));

    shCmd = getNormalCmdFromOrigCmd(shCmd);

    //通过组网自动下发的心跳进行信道信息获取
    if(shCmd == DATA_FRAME_NETWORK_HEARTBEAT && !m_bGetChannelInfo)
    {
        m_nConnectDevNum = 0;
        memcpy(&m_nConnectDevNum, pData + PACK_DATA_MARK + 2, 2);
        m_nChannelNo = 0;
        memcpy(&m_nChannelNo, pData + PACK_DATA_MARK + 2 + 2 + 4, 1);
        m_bGetChannelInfo = true;
    }
    else if(shCmd == DATA_FRAME_TYPE_GET_TIME_STAMP)
    {
        for (auto iter = m_mapDevices.begin() ;iter != m_mapDevices.end() ;iter++)
        {
            if(nullptr != iter->second)
            {
                iter->second->parseSinglePackage(iFrameNo,pData,iDatalen);
            }
        }
    }
    //一般情况是设置信道的函数
    else
    {
        if(shCmd == m_shLastCommand)
        {
            m_bSendCommandSucess = true;
        }
    }

    //如果已经确认了设备不进行多次处理
    if(m_bConfirmOperation)
    {
        //只处理心跳,每隔固定时间进行心跳回复
        return iDatalen;
    }
    //进行设备连接处理
    else
    {
        //设备编码
        unsigned char cHardwareCode;
        memcpy(&cHardwareCode, pData + PACK_HARDWARE_CODE, 1);

        //获取id
        memcpy(&m_iDevID, pData + PACK_HARDWARE_ID, sizeof (CONTENT_HARDWARE_ID));

        //确认设备类型，直连设备通过信道号确认，组网设备通过信道号确认
        //硬件编码若为 0那么为组网编码，此时需要通过信道进行设备类型的确定
        if(cHardwareCode == HardwarePropertye::HARDWARE_CODE_NETWORK)
        {
            if(!m_bGetChannelInfo)
            {
                return iDatalen;
            }

            m_shDevType = CDevDataCfg::getDevTypeByChaNo(m_nChannelNo);
        }
        else
        {
            m_shDevType = convertHardwareCode2DevType(cHardwareCode);
        }

        //到这一步设备一定有效
        m_bValidDev = true;

        //设置组网和直连属性
        m_bStationSte = cHardwareCode == HardwarePropertye::HARDWARE_CODE_NETWORK;

        m_bConfirmOperation = true;

        //发送设备连接
        emit signalDevStatus(true, this);

        //跳绳设备立马创建
        if(m_shDevType == DEV_TYPE_ROPE_SKIP)
        {
            auto pDev = createSteDevcice(m_iDevID, *m_pCom, m_shDevType);
            if(pDev != nullptr)
            {
                pDev->devOpen();
            }
            else
            {
                LOGERROR("createSteDevcice false m_shDevType =" + to_string(m_shDevType));
            }

        }

        qDebug() << "hhhhhh" << this;
    }

    return iDatalen;
}

/*************************************************
  <函数名称>    onUpdateTimeOut
  <功能描述>    更新设备数据信息
  <参数说明>

  <返 回 值>

  <函数说明>    一定时间获取硬件通讯接口发送来的数据，对数据进行
               解析并更新设备的相关数据。只有在有独立通讯机制下
               该函数才会被执行

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
void CDevSteFactory::onUpdateTimeOut()
{
    m_bTimeOutRun = true;

    //如果是打开了通讯接口
    if (m_bOpened && m_pCom != nullptr)
    {   
        if(m_bSendDataReport)
        {
            if((QDateTime::currentMSecsSinceEpoch() - nStartTimeSendDataReportTime) > 5000)
            {
                m_bSendDataReport = false;
            }

            m_nSendDataReportinterval += m_iUpdateInteval;
            if(m_nSendDataReportinterval >= 300)
            {
                emit signalSendDataToDev(s_arrSetDataReport, 0, 0);
                m_nSendDataReportinterval -= 500;
            }
        }

        //计算剩余缓存空间
        int iCaCheSize = m_Cache.size();
        int iFreeCacheSize = iCaCheSize - m_iExistCacheSize;

        if(iFreeCacheSize > 0)
        {
            //读取一定数据
            unsigned int lReadLength = m_pCom->readData(static_cast<unsigned int>(iFreeCacheSize), m_Cache.data() + m_iExistCacheSize);
            m_iExistCacheSize += lReadLength;

            //如果有数据则进行解析
            if (lReadLength != 0)
            {
                //有数据则认为设备连接正常，对超时累计清零
                m_iIntervalTotal = 0;

                m_nReceiveData += lReadLength;

                dataParse(m_Cache);
            }

            timeoutCheckValidDev();
        }
        else if(iFreeCacheSize == 0)
        {
            emit signalParseBusy();
        }
        else
        {
            //m_Cache初始化出错
        }
    }

    m_bTimeOutRun = false;
}
