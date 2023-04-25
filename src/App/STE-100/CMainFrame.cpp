///////////////////////////////////////////////////////////
//  CMainFrame.cpp
//  Implementation of the Class CMainFrame
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CMainFrame.h"
#include "CTextToSpeech.h"
#include "CDatabaseManage.h"

#include "../../Common/HttpClient/CHttpConfigCommonDefine.h"
#include "../../Common/common_error.h"
#include "CServerCommunication.h"
#include "CFilePath.h"

#include "SteDevManager/SteDevice/CDevSteBody.h"
#include "SteDevManager/SteDevice/CDevSteSFB.h"
#include "SteDevManager/SteDevice/CDevSteGrip.h"
#include "SteDevManager/SteDevice/CDevSteRunShort.h"
#include "SteDevManager/SteDevice/CDevStePushup.h"
#include "SteDevManager/SteDevice/CDevSteSBJ.h"

#include "CCustVTChartView.h"
#include "CCustFVChartView.h"
#include "CCustRTChartView.h"
#include "CProjectEnums.h"

#include "../Common/CSignPainterCommandDefin.h"
#include <vector>

#include <QMap>

using namespace soft;

using namespace BindValue;
CMainFrame::CMainFrame(QObject *parent)
    : QObject(parent)
{
    //删除软件生成的无用文件夹
    deleteUnnecessaryFolder();

    //初始化画板连接
    initSignPaint();

    initQmlEngine();

    //初始化连接
    initConnection();

    //初始化同步线程
    initDataSync();

    //初始化服务器信号
    initNetWorkConnection();

    CServerCommunication::Get_Instance().startActive();

    m_objAssistDevMgr.startActive();

    m_ui.setQmlEngine( &m_objQmlEngine );

    m_ui.setMainFrame(this);

    int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();

    switch (nSoftVersion) {
    case VERSION_CD_PSB:
        LOGINFO( "当前为公安局版本" );
        break;
    case VERSION_QLNJXX:
        LOGINFO( "当前为邛崃南街版本" );
        break;
    default:
        LOGINFO( "当前为通用版本" );
        break;
    }
}

CMainFrame::~CMainFrame()
{
    close();
}

QVariantMap CMainFrame::getDevDetailInfos()
{
    QVariantMap devDetailInfosMap;
    CDevHardwareBase *pTmpDev = nullptr;

    //存放分机编号,以供设置分机编号使用
    QVariantMap devOrderMap;

    //存放设备号, 以供校准使用
    QStringList devNumberList;

    auto devMap = m_objSteDevMgr.getAllDev();

    for( auto &ele : devMap ){
        pTmpDev = ele.second;

        //获取设备号
        devNumberList << QString::number( pTmpDev->getMacID() );

        //获取分机编号
        devOrderMap.insert( QString::number( pTmpDev->getMacID() ), pTmpDev->getExtensionNo() );
    }

#if 0
    devOrderMap.insert( "testDev001", "testOrder1" );
#endif

    devDetailInfosMap[ "devOrderMap" ] = devOrderMap;
    devDetailInfosMap[ "devNumberList" ] = devNumberList;

    return devDetailInfosMap;
}

void CMainFrame::close()
{
    killProcess("SignPainter.exe");
    CDevDataCfg::delayMSec(1000);

    //停止身份识别线程
    m_objIdentifyUser.stop();

    //停止所有正在进行的测试
    m_objTestGuideManager.cancelTest(false, false, false);
    m_objTestGuideManager.clearAll();

    //停止所有体测设备及其线程
    m_objSteDevMgr.stopActive();

    //停止自动上传及其线程
    CServerCommunication::Get_Instance().stopActive();

    //停止辅助设备及其线程
    m_objAssistDevMgr.stopActive();

    //释放同步数据线程
    if(nullptr != m_pDataSync)
    {
        if(m_pSyncThread->isRunning())
        {
            m_pSyncThread->quit();
            m_pSyncThread->wait();
        }
    }

    killProcess("STE-100.exe");
}

void CMainFrame::onSystemModeChange(short shTestMode)
{
    if(shTestMode >= TEST_MODE_COUNT)
    {
        //在界面通知未知测试模式
        return;
    }

    //先将信息存入本如本地数据库，存入成功再设置
    m_objTestGuideManager.setTestMode( shTestMode );
}

void CMainFrame::onDevConnectChangeSteDev(bool bConnec, void *pSteDev)
{
    resetLastIdentifyInfo();

    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev invalid");
        return;
    }

    CDevHardwareBase *pDev = reinterpret_cast<CDevHardwareBase *>(pSteDev);
    if(bConnec)
    {
        m_objTestGuideManager.connectSteDev(pDev);

        //若是第一个连接上来的设备，需要将灯光变更为未测试状态
        if(m_objSteDevMgr.getAllDev().size() <= 1)
        {
            if(m_objAssistDevMgr.getDevPowerUart() != nullptr)
            {
                m_objAssistDevMgr.getDevPowerUart()->setLightBelt(LED_BELT_MODE_DEFUALT);
            }
            else
            {
                LOGERROR("Dev Power is nullptr!");
            }
        }
    }
    else
    {
        m_objTestGuideManager.disconnectSteDev(pDev);
    }
}

//体测设备变更涉及到模式的切换以及设备类型的切换(这里根据判断进行设备类型的具体界面切换)
void CMainFrame::onDevConnectChangeSteFactory()
{
    resetLastIdentifyInfo();

    bool bDirect = CDevDataCfg::getDevDirectConnectFlag();

    int nModel;
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevTestMode(nModel);
    short shTestMode = static_cast<short>(nModel);
    short shDevType = m_objSteDevMgr.getType();

    //设置测试指导，需要清空当前所有设备及绑定人员
    m_objTestGuideManager.clearAll();
    m_objTestGuideManager.setTestMode(shTestMode);

    //获取当前所有设备重新发送连接消息即可
    auto mapDev = m_objSteDevMgr.getAllDev();
    for(auto &element : mapDev)
    {
        element.second->sendConnectState(true);
    }

    if(!m_timerGetDevConnectCount.isActive())
    {
        m_timerGetDevConnectCount.start(m_iIntervalGetDevInfo);
    }

    //界面设置类型需要在一个函数中设置，避免中间多次切换闪烁
    m_ui.setDevStatus(shDevType, bDirect, shTestMode, !m_objSteDevMgr.isStaionSte());

    //设置助手设备管理，变更会影响摄像头、跑步刷卡设备的查找
    m_objAssistDevMgr.onSteDevTypeChange(shDevType, bDirect);

    if(shDevType != DEV_TYPE_UNKNOW)
    {
        CTextToSpeech::GetInstance().textToSpeech("欢迎使用", 100);
    }
}

void CMainFrame::enableUserIdentify()
{
    m_objIdentifyUser.resum();
}

void CMainFrame::onCfgChange()
{
    auto &sysSetting = m_ui.getSystemSettingMgr();

    auto setChangedPage = sysSetting.getChangedPage();

    CAbstractSettingElement *pConfigPage = nullptr;
    unsigned long lChangeItem = changeItem::CHANGE_NO;
    for(auto &page : setChangedPage)
    {
        pConfigPage = sysSetting.getConfig(page);
        lChangeItem = pConfigPage->getChangeItem();

        if (lChangeItem == changeItem::CHANGE_NO)
        {
            continue;
        }

        switch (page) {
        case CProjectEnums::SystemSettingPage::PAGE_HOST:{
            bool bSuccess = true;
            //变更信道类型获取当前设备类型和信道，然后设置到指定设备中,   待加入是否自动切换
            if(lChangeItem & changeItem::host::CHANGE_DEV_Channel)
            {
                uint maxCnt = pConfigPage->getSpecificDataElement( CHostSettingConfig::HostElementData::MaxConnectedCnt )
                        .value( "maxConnectCnt" ).toUInt();

                uint curDevChannel = pConfigPage->getSpecificDataElement( CHostSettingConfig::HostElementData::CurDevChannel )
                        .value( "curDevChannel" ).toUInt();
                bSuccess = m_objSteDevMgr.setChannelInfo( maxCnt, curDevChannel );
            }
            else if(lChangeItem & changeItem::host::CHANGE_DEV_CONNECTED_MODE){
                onDevConnectChangeSteFactory();
            }
            else if(lChangeItem & changeItem::host::CHANGE_DEV_SYNC_MODE)
            {
                int testMode = pConfigPage->getSpecificDataElement( CHostSettingConfig::HostElementData::SyncTestModel ).value( "syncTestModel" ).toInt();

                if(m_objTestGuideManager.setTestMode(short(testMode)))
                {
                    m_objTestGuideManager.cancelTest(false,true);
                }
            }

            //通知界面更新配置结果
            emit sigUpdateDevConfig(bSuccess);
            break;
        }
        case CProjectEnums::SystemSettingPage::PAGE_DEV:{
            //如果分级编号有改变
            if( lChangeItem & changeItem::dev::CHANGE_DEV_EXTENSION_NO ){
                QVariantList devNumberList = pConfigPage->getSpecificDataElement( CDevSettingConfig::DevElementData::DevNumberInfos ).
                        value( "devChannelConfig" ).toList();

                for( auto &ele : devNumberList ){
                    QVariantMap tmpDevInfo = ele.toMap();
                    onSetDevExtensionNum( tmpDevInfo.value( "firstColumn" ).toString(), tmpDevInfo.value( "secondColumn" ).toInt() );
                }
            }

            break;
        }
        case CProjectEnums::SystemSettingPage::PAGE_USING:{
            if( lChangeItem & changeItem::user::CHANGE_TEST_ITEM)
            {
                m_objTestGuideManager.onTestItemChanged();
            }
            else if( lChangeItem & changeItem::user::CHANGE_START_POS)
            {
                int startPos = pConfigPage->getSpecificDataElement( CUsingSettingConfig::CUsingElementData::TestStartPos ).
                        value( "satrtPosConfig" ).toInt();
                setSBJStartPos(startPos);
            }
            else if( lChangeItem & changeItem::user::CHANGE_ROPE_DELAY)
            {
                m_objTestGuideManager.initRopeSkipDelayTime();
            }
            break;
        }
        }

        pConfigPage->resetChangeItem();
    }
}

void CMainFrame::queryStudentByUniqueID(QString stuUniqueID)
{
#if 1
    CUserInfo userInfo;
    static vector<CUserInfo> info;
    userInfo.m_bUseGenderToSelect = true;
    userInfo.m_bGender = true;

    if(info.size() == 0)
    {
        CDatabaseManage::GetInstance().getTableUserIdentityInfo()->getUserInfo(userInfo, info);
    }

    static unsigned int s_iIndex = 0;
    unsigned int uiCurrent = 0;
    if(s_iIndex >= info.size())
    {
        s_iIndex = 0;
    }
    uiCurrent = info.size() - (s_iIndex++ + 1);
    QString strCardId;
    if(info.size() != 0)
    {
        strCardId = info[ uiCurrent ].m_strStudentId;
    }

    m_objIdentifyUser.setFindSrcStudentID(strCardId);
#else
    CUserInfo targetUserInfo;
    bool ok = CDatabaseManage::GetInstance().getTableUserIdentityInfo()->queryStudentByUniqueID( "12037", targetUserInfo );

    if( !ok ){
        return;
    }

    m_ui.onFaceRecogSuccess( targetUserInfo );
#endif

}

void CMainFrame::testDisplayCode(short shDevType, short shTestMode /*= TEST_ASYNC*/, bool bIsStation /*= false*/)
{
    m_ui.setDevStatus(shDevType, !bIsStation, shTestMode);

    //设置测试指导，需要清空当前所有设备及绑定人员
    m_objTestGuideManager.clearAll();
    m_objTestGuideManager.setTestMode(shTestMode);

    //设置助手设备管理，变更会影响摄像头、跑步刷卡设备的查找
    m_objAssistDevMgr.onSteDevTypeChange(shDevType, !bIsStation);
}

void CMainFrame::sendSyncUserCommand()
{
    m_syncUserInfoRunning = true;
    if(nullptr == m_pRequestMgr)
    {
        m_pRequestMgr = CHttpRequestMgr::getInstance();
    }

    //2022.12.13 直接从数据库查询最大时间戳
#if 0
    QDateTime UserTime;
    CDatabaseManage::GetInstance().getTabDataSyncTime()->getSyncTime(CTableDataSyncTime::TABLE_TYPE_USER_INFO, UserTime);

    //得到时间戳
    long long llTimeStamp = UserTime.toMSecsSinceEpoch();
#else
    QString &&maxTimeStamp = CDatabaseManage::GetInstance().getTableUserIdentityInfo()->queryMaxTimeStamp();
#endif

    m_pRequestMgr->syncUserInfo( maxTimeStamp, TMEc_Soft );
}

void CMainFrame::sendSyncClassCommand()
{
    if(nullptr == m_pRequestMgr)
    {
        m_pRequestMgr = CHttpRequestMgr::getInstance();
    }

    QDateTime ClassTime;
    CDatabaseManage::GetInstance().getTabDataSyncTime()->getSyncTime(CTableDataSyncTime::TABLE_TYPE_CLASS_INFO, ClassTime);

    //得到时间戳
    long long llTimeStamp = ClassTime.toMSecsSinceEpoch();

    m_pRequestMgr->syncClassInfo(QString::number(llTimeStamp));
}

void CMainFrame::sendSyncScoreCommand()
{
    if(nullptr == m_pRequestMgr)
    {
        m_pRequestMgr = CHttpRequestMgr::getInstance();
    }

    m_pRequestMgr->syncScoreStd();
}

void CMainFrame::initDataSync()
{
    m_pDataSync = new CDataSynchronize();
    m_pSyncThread = new QThread();
    //0.删除消息绑定
    connect(m_pSyncThread, &QThread::finished, m_pDataSync, &QObject::deleteLater);
    connect(m_pSyncThread, &QThread::finished, m_pSyncThread, &QObject::deleteLater);
    connect( m_pDataSync, &CDataSynchronize::sigStopWork, this, &CMainFrame::onStopSync );
    //1.同步状态变化
    connect( m_pDataSync, &CDataSynchronize::sigSyncStatusChanged, this, &CMainFrame::onSyncStatusChanged );
    //2.同步用户信息
    connect( this, &CMainFrame::sigUpdateUserInfo, m_pDataSync, &CDataSynchronize::onUpdateUserInfo );
    //3.同步班级信息
    connect( this, &CMainFrame::sigUpdateClassInfo, m_pDataSync, &CDataSynchronize::onUpdateClassInfo );
    //4.同步成绩信息
    connect( this, &CMainFrame::sigUpdateScoreInfo, m_pDataSync, &CDataSynchronize::onUpdateScoreInfo );
    //5.状态更新信息
    connect(&m_objSteDevMgr.getTestStateMgr(), &CTestStateManage::signalTestStateChange, m_pDataSync, &CDataSynchronize::onDevTestStateChange);

    //将整个流程移入单独的线程
    m_pDataSync->moveToThread(m_pSyncThread);
    m_pSyncThread->start();

    //理论一次运行只会开启一次
    LOGINFO("++++++++++ Start Sync Thread ++++++++++");
}

void CMainFrame::onTimerOutGetDevConnectNum()
{
    //上一次识别信息有效
    if(m_llLastIdentify != -1)
    {
        if(QDateTime::currentMSecsSinceEpoch() - m_llLastIdentifyTime >= m_iLastIdentifyResetTime)
        {
            resetLastIdentifyInfo();
        }
    }

    auto mapDev = m_objSteDevMgr.getAllDev();
    static QMap<unsigned int, int> mapConnectDev;
    mapConnectDev.clear();

    //需要校准的设备类型
    static set<short> setCalibrationType;
    if(setCalibrationType.size() == 0)
    {
        setCalibrationType.insert(DEV_TYPE_SFB);
        setCalibrationType.insert(DEV_TYPE_BODY);
        setCalibrationType.insert(DEV_TYPE_GRIP);
    }

    CDevHardwareBase *pDev;
    //特殊设备 跳绳 中长跑 短跑不需要设备编号
    int nDevCount = 0;
    if(CDevDataCfg::isSpecialDev(m_objSteDevMgr.getType()))
    {
        //短跑：设备数量取决于可用跑道数
        if(m_objSteDevMgr.getType() == DEV_TYPE_RUN_SHORT)
        {
            //将设备转换为短跑
            if(mapDev.size() > 0)
            {
                CDevSteRunShort *pDevRunShort = reinterpret_cast<CDevSteRunShort *>(mapDev.begin()->second);
                if(nullptr != pDevRunShort)
                {
                    nDevCount = pDevRunShort->getConnectSubElementNum();
                }
            }

        }
        else
        {
            nDevCount = static_cast<int>(mapDev.size());
        }
    }
    //其它设备需要判断分机编号
    else
    {
        for(auto &itdev : mapDev)
        {
            pDev = itdev.second;
            if(pDev == nullptr)
                continue;

            //校准设备的逻辑（不需要判断分机编号）
            if(setCalibrationType.find(pDev->getDevType()) != setCalibrationType.end())
            {
                mapConnectDev[pDev->getMacID()] = pDev->getExtensionNo();
            }

            //判断有效设备需要判断设备分机编号有效
            if(CDevDataCfg::isValidExtensionNum(pDev->getExtensionNo()))
                nDevCount++;
        }
    }


    m_ui.setConnectedDevNumbers(mapConnectDev);
    if(!CDevDataCfg::getDevDirectConnectFlag())
        m_ui.setMultiDevCount(QString::number(nDevCount));

    //设置设备信息
    //onGetAllSettingViewInfo();

    //只主动发一次心跳
    static bool firstTime = true;

    if( !firstTime ){
        return;
    }

    auto mapDevv = m_objSteDevMgr.getAllDev();


    //存在为空的情况
    if( mapDevv.size() <= 0 ){
        return;
    }

    CDevHardwareBase *pTempDev = nullptr;
    for(auto &element : mapDevv)
    {
        pTempDev = element.second;
    }

    sendMultiHeartBeat();

    firstTime = false;
}

void CMainFrame::onTimerSyncServerInfo()
{
    /// 0.安全保护，没有网络连接直接返回
    if(!m_bNetworkFlag)
    {
        return;
    }

    /// 1.以下所有同步都需要可以发送，且数据库未同步过
    //1.1 判断是否需要同步成绩信息
    if(!m_bCompleteScoreFlag && m_bSyncScoreFlag)
    {
        sendSyncScoreCommand();
        LOGINFO("Send Sync Score Command By Timer");
    }

    //1.2 判断是否需要同步用户信息
    if(!m_bCompleteUserFlag && m_bSyncUserFlag)
    {
        sendSyncUserCommand();
        LOGINFO("Send Sync User Command By Timer");
    }

    //1.3 判断是否需要同步班级信息
    if(!m_bCompleteClassFlag && m_bSyncClassFlag)
    {
        sendSyncClassCommand();
        LOGINFO("Send Sync User Command By Timer");
    }
}

void CMainFrame::onSpeakState(bool bSpeak)
{
    //当前限制的主要是在进行测试时限制界面按钮的使用
    bool bStation = !CDevDataCfg::getDevDirectConnectFlag();
    short shTestMode = m_objTestGuideManager.getTestMode();

    set<short> setBtnType;

    //不在测试界面不进行处理
    if(!CDevDataCfg::isTestPage(m_ui.getCurPage(), shTestMode))
        return;

    //取消按钮 重测
    static bool bEnable;

    if(bStation)
    {
        setBtnType.insert(CProjectEnums::OTM_CANCEL_TEST_BTN);
        setBtnType.insert(CProjectEnums::OTM_RESTART_TEST_BTN);
        setBtnType.insert(CProjectEnums::OTM_ASYNC_COMMIT_BTN);
        setBtnType.insert(CProjectEnums::OTM_SYNC_COMMIT_BTN);


        if(shTestMode == TEST_ASYNC && bSpeak)
        {
            bEnable = m_ui.getBtnEnableState(CProjectEnums::OTM_ASYNC_COMMIT_BTN);
        }
        else if (shTestMode == TEST_SYNC && bSpeak)
        {
            bEnable = m_ui.getBtnEnableState(CProjectEnums::OTM_SYNC_COMMIT_BTN);
        }

    }
    //返回 提交 首页
    else
    {
        setBtnType.insert(CProjectEnums::OTO_TEST_BACK_INFO_BTN);
        setBtnType.insert(CProjectEnums::OTO_TEST_BACK_HOME_BTN);
    }

    for(auto &shBtnType : setBtnType)
    {
        if((shBtnType == CProjectEnums::OTM_SYNC_COMMIT_BTN || shBtnType == CProjectEnums::OTM_ASYNC_COMMIT_BTN) &&
                !bSpeak)
        {
            m_ui.setBtnEnable(shBtnType, bEnable);
        }
        else
        {
            m_ui.setBtnEnable(shBtnType, !bSpeak);
        }
    }
}

void CMainFrame::deleteUnnecessaryFolder()
{
    //删除空文件夹
    string strPath = CFilePath::getAppDirPath() + "/" + TEST_PROC_SRC;
    CFilePath::deleteEmptyFolder(strPath);
}

bool CMainFrame::setCorrectProgressBody(CDevHardwareBase *pDev, const int &nStep, const float &fValue)
{
    if(pDev == nullptr || !pDev->isValid())
        return false;

    CDevSteBody *pDevBody = reinterpret_cast<CDevSteBody *>(pDev);

    bool bRet = false;

    short shItem = m_ui.getCurCorrectItem();
    //0、1校准体重（分别为0kg和50kg） 2校准身高 1m
    switch (shItem) {
    case TEST_ITEM_WEIGHT:
        if(nStep == 0)
        {
            bRet = pDevBody->calibrateWeight(fValue);
        }
        else if(nStep == 1)
        {
            bRet = pDevBody->calibrateWeight(fValue);
        }
        break;
    case TEST_ITEM_HEIGHT:
        if(nStep == 0)
        {
            bRet = pDevBody->calibrateHeight(fValue);
        }
        break;
    default:
        break;
    }

    return bRet;
}

bool CMainFrame::setCorrectProgressGrip(CDevHardwareBase *pDev, const int &nStep, const float &fValue)
{
    if(pDev == nullptr || !pDev->isValid())
        return false;

    bool bRet = false;
    if(nStep == 0)
    {
        //校准0刻度线
        CDevSteGrip *pDevGrip = reinterpret_cast<CDevSteGrip *>(pDev);
        bRet = pDevGrip->calibrateGrip(0.0f);
    }

    return bRet;
}

bool CMainFrame::setCorrectProgressSFB(CDevHardwareBase *pDev, const int &nStep, const float &fValue)
{
    if(pDev == nullptr || !pDev->isValid())
        return false;

    bool bRet = true;
    if(nStep == 0)
    {
        //校准0刻度线
        CDevSteSFB *pDevSFB = reinterpret_cast<CDevSteSFB *>(pDev);
        bRet = pDevSFB->calibrateScale(0.0f);
    }

    return bRet;
}

bool CMainFrame::setCorrectProgressPushUp(CDevHardwareBase *pDev, const int &nStep, const float &fValue)
{
    if(pDev == nullptr || !pDev->isValid())
        return false;

    bool bRet = true;
    if(nStep == 0)
    {
        //校准0刻度线
        CDevStePushup *pDevPushup = reinterpret_cast<CDevStePushup *>(pDev);
        bRet = pDevPushup->calibrateScale(fValue);
    }

    return bRet;
}

void CMainFrame::onDevConnectChangeIdentify(bool bConnect)
{
    CIdentificDevNFC *pIdentific = m_objAssistDevMgr.getIdentifyDev();

    //身份识别设备连接状态变更通知界面即可
    if(bConnect)
    {
        connect(pIdentific, &CIdentificDevNFC::signalFindCard, this, &CMainFrame::onFindCardByIdentifyDev, Qt::UniqueConnection);
    }
    else
    {
        LOGWARN("identify disconnect");
    }

    m_ui.setViewErorrString(MERR_USER_RECONGNITION_NFC, !bConnect);
}

void CMainFrame::onFindCardByIdentifyDev(const string &strCard)
{
    m_objIdentifyUser.setFindSrcMagneticCard(QString::fromStdString(strCard));
}

void CMainFrame::onDevConnectChangeRunMidNFC()
{
    CDevRunNFC *pRunNFC = m_objAssistDevMgr.getDevRunNfc();

    bool bConnect = pRunNFC != nullptr && pRunNFC->isValid();
    if(bConnect)
    {
        m_pDevRunNFC = pRunNFC;
        connect(pRunNFC, &CDevRunNFC::signalRunNFCRecognizeCard, &m_objTestGuideManager, &CTestGuideManager::onRunMidSwipingCard);
    }
    else
    {
        if(m_pDevRunNFC != nullptr)
        {
            disconnect(pRunNFC, &CDevRunNFC::signalRunNFCRecognizeCard, &m_objTestGuideManager, &CTestGuideManager::onRunMidSwipingCard);
        }

        m_pDevRunNFC = nullptr;
    }

    if(m_objSteDevMgr.getType() == DEV_TYPE_RUN_MID)
    {
        m_ui.setViewErorrString(MERR_RUN_MID_NFC, !bConnect);
    }
}

void CMainFrame::onDevConnectChangeStartingGun(bool bConnect)
{
    m_objTestGuideManager.setStartingGunExistState(bConnect);
}

void CMainFrame::onDevConnecChangePower()
{
    CDevPowerUart *pPower = m_objAssistDevMgr.getDevPowerUart();

    bool bConnect = pPower != nullptr;

    //连接,电源设备指针不会变更，采用变量判断进行信号绑定
    if(bConnect)
    {
        m_bPowerConnect = true;

        connect(pPower, &CDevPowerUart::signalBatteryChange, this, &CMainFrame::onBatteryChange, Qt::UniqueConnection);
        connect(pPower, &CDevPowerUart::signalPowerUintBatteryChange, this, &CMainFrame::onPowerUnitBatteryChange, Qt::UniqueConnection);
        connect(pPower, &CDevPowerUart::signalPowerUintConnec220VChange, this, &CMainFrame::onPowerUnitConnect220VPower, Qt::UniqueConnection);
    }

    m_objSteDevMgr.setPowerConnectState(bConnect);

    m_ui.setViewErorrString(MERR_POWER, !bConnect);
}

void CMainFrame::onBatteryChange(short shRemindPower)
{
    if(shRemindPower < 0)
    {
        shRemindPower = 0;
    }

    if(shRemindPower > 100)
    {
        shRemindPower = 100;
    }

    //m_ui.setDisplayBattreyQuantity(shRemindPower);
    //平板电量变更通知服务器
    //m_objServerCommunication.setBattery(shRemindPower);
    //    m_objTestGuideManager
    //    m_pRequestMgr->sendHeartBeat(0, QString::number(shRemindPower), "dev", m_objTestGuideManager)
}

void CMainFrame::onPowerUnitConnect220VPower(bool bConnect)
{
    //电源变更（220v或者电池供电）通知界面
    m_ui.setDisplayChargeState(bConnect);
}

void CMainFrame::onPowerUnitBatteryChange(short shRemindPower)
{
    //电池电量变更
    if(shRemindPower < 0)
    {
        shRemindPower = 0;
    }

    if(shRemindPower > 100)
    {
        shRemindPower = 100;
    }

    m_ui.setDisplayBattreyQuantity(shRemindPower);
}

void CMainFrame::onIdentifySuccess(CUserInfo userInfo)
{
    m_userInfo = userInfo;
    m_ui.setCurUserName( userInfo.m_strName );

    auto llLastId = m_llLastIdentify;
    updateIdentifyInfo(userInfo.m_llUniqueID);

    //不进行重复的语音报告操作
    if(llLastId != -1 && llLastId == userInfo.m_llUniqueID)
        return;

    if(m_bAvailable)
    {
        short recogRet = m_objTestGuideManager.bindUserInfo(userInfo, m_objSteDevMgr.getType());

        //暂时保留中长跑的调试代码
        //        static int size = 0;
        //        if(size >= 9)
        //        {
        //            size = 0;
        //            //onCancelTest();
        //            //m_ui.clearModelData(CInterAction::COMMON_DATA_MODEL);
        //        }
        //        else {
        //            //中长跑调试代码一直进行绑卡，暂时保留
        //            if(SUCESS_BIND_EXTRA == recogRet)
        //            {
        //                size++;
        //            }

        //            emit CInterAction::Get_Instance().sigTestSignal("11");
        //        }

        procBindUserReturnValue(recogRet);
        if( ALREADY_BIND == recogRet ){
            m_ui.advancedReportToViewRecogRet( ALREADY_BIND );
        }else if( NOT_AVAILABLE_TEST == recogRet ){
            m_ui.advancedReportToViewRecogRet( NOT_AVAILABLE_TEST );
        }else{
            m_ui.advancedReportToViewRecogRet( SUCESS_BIND );
        }
    }
    //正在加载得分
    else if(CTestGuideBase::getScoreInfo().getLoadScoreState())
    {
        QString qs = CErrorCode::getErrorDescribe(MINFO_LOAD_SCORE);
        CTextToSpeech::GetInstance().textToSpeech(qs);
    }
    else
    {
        QString qs = CErrorCode::getErrorDescribe(CErrorCode::getErrorCode());
        CTextToSpeech::GetInstance().textToSpeechBlock(qs);
    }

}

void CMainFrame::onIdentifyFail()
{
    resetLastIdentifyInfo();
    //在界面显示识别失败，同时进行语音提示
    m_ui.advancedReportToViewRecogRet(BANDING_FALSE);

    CTextToSpeech::GetInstance().textToSpeech("识别失败", 1000);
}

void CMainFrame::onPageChanged()
{
    resetLastIdentifyInfo();

    m_objQmlEngine.trimComponentCache();

    short shPage = m_ui.getCurPage();
#if QT_NO_DEBUG
#else
    QMetaEnum tmpEnum = QMetaEnum::fromType<CProjectEnums::ViewPage>();
    qDebug() << "onPageChanged() get page-> " << tmpEnum.valueToKey( shPage );
#endif
    switch (shPage){
    //进行初始化服务
    case CProjectEnums::PAGE_SYSTEM_SELFCHECK:
    {
        //发送空心跳包
        sendMultiHeartBeat();

        //注册摄像头观察ui资源
        startIdentify();

        //从数据库中加载得分规则
        CTestGuideBase::getScoreInfo().onReloadScoreRule();

        //设置电源未连接、设置身份扫描设备未连接
        m_ui.setViewErorrString(MERR_USER_RECONGNITION_NFC, true);
        m_ui.setViewErorrString(MERR_POWER, true);

        //开启其他设备,主要是电源设备先供电

        //等待电源设备连接
        QTime timeTarget = QTime::currentTime().addMSecs(2000);
        {
            while(!m_bPowerConnect && QTime::currentTime() < timeTarget)
                //获取当前时间
                QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
        }

        //加载设备
        m_objSteDevMgr.startActive();

        //testDisplayCode( DEV_TYPE_FOOTBALL, TEST_SYNC, false );
    }
        break;
    default:
        break;
    }

    CDevCamera &camer = m_objAssistDevMgr.getIdentifyCamera();

    //在主页进行摄像头和识别逻辑的恢复
    if(CDevDataCfg::isHomePage(shPage))
    {
        //直连设备返回主页停止所有测试
        if(CDevDataCfg::getDevDirectConnectFlag())
        {
            m_objTestGuideManager.cancelTest();
        }

        camer.openCamer();
        camer.setState(true);

        //设置识别类型,主要只进行人脸识别延时即可
        m_objIdentifyUser.setIdentifyType(CFindUserBasic::IDENTIFY_BY_MAG_CARD | CFindUserBasic::IDENTIFY_BY_USER_ID);
        m_objIdentifyUser.resum();

        //恢复摄像头后，不立即进行识别，延时识别人脸
        QTimer::singleShot( 1000, this, &CMainFrame::onTimeOutStartIdentify );

        //测试完成返回首页设置灯光为未测试灯光
        if(m_objAssistDevMgr.getDevPowerUart() != nullptr)
        {
            m_objAssistDevMgr.getDevPowerUart()->setLightBelt(LED_BELT_MODE_DEFUALT);
        }
    }
    //不存在身份识别的界面暂停识别
    else
    {
        camer.pause();
        camer.setState(false);

        m_objIdentifyUser.pause();
    }

    if(shPage != CProjectEnums::PAGE_SYSTEM_SELFCHECK && !m_bFirstInChange)
    {
        m_bFirstInChange = true;
    }

    //这里的页面变更是指除开弹窗（设置界面）、视频播放之后的正常界面
    bool bPageChange = false;
    if(shPage != CProjectEnums::PAGE_SETTING && shPage != CProjectEnums::PAGE_VIDEO)
    {
        if(m_shCurPage != shPage)
        {
            bPageChange = true;
            m_shCurPage = shPage;
        }
    }

    if(bPageChange)
    {
        m_objTestGuideManager.clearDisconnectUserInfo();
    }

    //界面变更关闭签字板
    m_ui.startSignature("0");
}

void CMainFrame::onClickBtnStartTest(bool bTouristMode)
{
    //游客模式开始测试,在直连设备时才会存在，不需要绑定的返回值
    if(bTouristMode)
    {
        CUserInfo userInfo;
        userInfo.m_llUniqueID = TOURIST_MODE_ID;

        //游客模式删除所有被绑定人员
        m_objTestGuideManager.bindUserInfo(userInfo);
    }
    else
    {
        m_objTestGuideManager.startTest();
    }
}

void CMainFrame::onClickBtnBackHome()
{
    m_objTestGuideManager.cancelTest(true);
}

void CMainFrame::onClickBtnTestPageBack()
{
    m_objTestGuideManager.cancelTest(true);
}

void CMainFrame::onClickBtnInfoPageBack()
{
    m_objTestGuideManager.cancelTest(true);
}

//重新测试(此按钮点过一次后就会禁用，需要手动重新启用)
void CMainFrame::onReTest()
{
    //重新开始所有设备
    m_objTestGuideManager.reStartTest();
}

//取消测试按钮触发消息
void CMainFrame::onCancelTest()
{
    m_objTestGuideManager.cancelTest(true);
}

//点击提交展示成绩
void CMainFrame::onClickBtnSubmitResult()
{
    //调用之前先清除要删除的空间
    m_objTestGuideManager.clearDeleteContent();
    m_objTestGuideManager.dispTestResult();
}

void CMainFrame::onClickBtnVerifySubmitResult()
{
    //强制完成所有测试
    m_objTestGuideManager.forceExitTest();

    //处理签字，主要是设置本次测试性质 正式测试 和 非正式测试
    m_objTestGuideManager.procSignature( m_ui.getSignatureFilePath() );

    //提交当前成绩
    m_objTestGuideManager.uploadTestRestltToServer();

    //提交完成成绩重置（测试状态变更）
    resetLastIdentifyInfo();
}

void CMainFrame::onDelScore(QString qsNumber)
{
    m_objTestGuideManager.deleteScore(qsNumber);
}

//设备配置信息改变：如 中长跑一圈长度 / 跳绳延时数据接收时间
void CMainFrame::onDevConfigInfoChanged(QVariantMap vMapInfo)
{
    short shTestItem = static_cast<short>(vMapInfo.value( "devType" ).toInt());
    //跳绳设备
    if(shTestItem == TEST_ITEM_ROPE_SKIP)
    {
        int iDelayTime = vMapInfo.value( "ropeSkipDelay" ).toInt();

        //保存至数据库
        if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateRopeDelayTime(iDelayTime))
        {
            LOGERROR("跳绳数据接收延时配置更新至数据库失败！");
            return;
        }

        m_objTestGuideManager.setRopeSkipDelayTime(iDelayTime);
    }
}

short CMainFrame::convertLocalNatureTypeToServer(const int curNatureType)
{
    short shNature = CServerCommunication::TEST_NATURE_EXERCISE;

    if(curNatureType == CTestResultInfo::TEST_INFORMAL)
    {
        shNature = testType::exerciseType;
    }

    else if (curNatureType == CTestResultInfo::TEST_FORMAL)
    {
        shNature = testType::examType;
    }
    return shNature;
}


void CMainFrame::onQueryHistoryScore(int testItem, int timeInterval, int type)
{
    QString logInfo = QString( "query history score, query condition:: userId = %1, testItem = %2, timeInterval = %3, scoreType = %4" );

    //在此处写查询成绩的逻辑
    //过滤掉两边相同的数据
    //testItem为本地数据库类型，查询时需转换为Http对应测试类型
    QVariantMap params;
    params.insert( "userId", m_objTestGuideManager.getCurrentUserID() );
    params.insert( "testItem", CDevDataCfg::getHttpTestItemFromItem(short(testItem)) );
    params.insert( "timeInterval", timeInterval );
    params.insert( "scoreType", type);

    //测试代码
    //    params.insert( "userId", 12037 );

    logInfo = logInfo.arg( m_objTestGuideManager.getCurrentUserID() )
            .arg( QString::number( testItem ) )
            .arg( QString::number( timeInterval ) )
            .arg( QString::number( type ) );
#if QT_NO_DEBUG
    LOGINFO( logInfo.toStdString() );
#else
    qDebug() << logInfo;
#endif

    //当前查询项目
    m_shQuertTestItem = short(testItem);

    vector<CTestResultInfo> vecLocalAllResults;

    //查询条件
    queryTimeInterval tmpTimeInterval   = static_cast<queryTimeInterval>( timeInterval );
    long long         dateBaseTimeStamp = CHttpQueryHistoryScore::getQueryStartDate( tmpTimeInterval );
    short             localTestItem     = short(testItem);//CDevDataCfg::getTestItemFromHttpItem( testItem );
    long long         userId            = params.value( "userId" ).toLongLong();
    short             socreType         = CDevDataCfg::getScoreTypeFromView( type );

    QVariantList localHumanCompositionRetList;

    //如果查询的是，身高体重，在if 里面单独处理， 查询出来的结果存放在 localHumanCompositionRetList
    if( localTestItem == TEST_ITEM_WEIGHT || localTestItem == TEST_ITEM_HEIGHT )
    {
        QVariantMap queryConditions ={
            { "userId", userId },
            { "scoreType", socreType },
            { "dateBaseTimeStamp", QString::number( dateBaseTimeStamp ) }};

        //查询本地数据库中身高体重
        queryLocalHumanCompositionTestResult( localHumanCompositionRetList, queryConditions );

    }
    else{
        //从本地查询成绩
        CDatabaseManage::GetInstance().getTabTestItemResult()->getResultByInfo( vecLocalAllResults,
                                                                                userId,
                                                                                testItem,
                                                                                dateBaseTimeStamp,
                                                                                socreType
                                                                                );
    }

    QString sizeLogIngo = QString( "local score count = " );
    sizeLogIngo = sizeLogIngo.append( QString::number( vecLocalAllResults.size() ) );

#if QT_NO_DEBUG
    LOGINFO( sizeLogIngo.toStdString() );
#else
    qDebug() << sizeLogIngo;
#endif

    //遍历查询出来的成绩,,
    QMap<QString, QString>  retMap;
    QStringList infoList;
    QString maxRet;
    QString recordTime;
    QString score;
    QVector<QStringList>    vecDisplayRets;
    int iRet;
    string tmpMaxRet = "";

    //此处要对身高体重做单独处理
    if( localTestItem == TEST_ITEM_WEIGHT || localTestItem == TEST_ITEM_HEIGHT )
    {
        // 为兼容普通成绩的接口, 按照 时间，身高，体重的顺序组装 QStringList，并追加到 vecDisplayRets
        QStringList tmpRecordInfos;
        QString recordTime = "";
        QString height = "";
        QString weight = "";

        for( auto &ele : localHumanCompositionRetList ){
            tmpRecordInfos.clear();
            recordTime = ele.toMap().value( "recordTime" ).toString();
            height     = ele.toMap().value( "height" ).toString();
            weight     = ele.toMap().value( "weight" ).toString();
            tmpRecordInfos << recordTime << height << weight;
            vecDisplayRets << tmpRecordInfos;
        }

    }
    else{
        for( auto &eachEle : vecLocalAllResults ){
            infoList.clear();
            if(eachEle.isLogData())
                continue;

            iRet = eachEle.getValidMaxTestResult( tmpMaxRet );
            maxRet = QString::fromStdString( tmpMaxRet );
            if( iRet < 0 ){
                maxRet = "无效成绩";
                score = "-";
            } else {
                score      = CTestGuideBase::getTestScore(true, eachEle, m_userInfo);
            }

            recordTime = QDateTime::fromString( QString::fromStdString( eachEle.m_strStartTime ), "yyyyMMddhhmmss" ).toString( "yyyy-MM-dd hh:mm:ss" );

            infoList << recordTime << maxRet << score;
            vecDisplayRets << infoList;
        }
    }


    //放进去之前先清除
    CHttpRequestMgr::getInstance()->getPQueryHistoryScore()->clearDisplayRets();
    CHttpRequestMgr::getInstance()->getPQueryHistoryScore()->setVecDisplayRets( vecDisplayRets );

    //本地查询完成过后,再向服务器请求数据
    CHttpRequestMgr::getInstance()->queryHistoryScore( params );
}

void CMainFrame::onQueryScoreFinished(bool ok , QVariant data)      //CHttpRequestMgr::onQueryHistoryScoreResultAvailable
{
    QVector< QStringList > vecAllHistoryScores = CHttpRequestMgr::getInstance()->getPQueryHistoryScore()->getVecDisplayRets();
    //如果向服务器查询失败,则直接往界面的表格填
    if( !ok ){

        //如果向服务器查询失败，并且当前查询的是身高体重，那么在if区域内单独处理, 直接填入界面表格
        TEST_ITEM curTestItem = m_pRequestMgr->getPQueryHistoryScore()->getCurTestItem();
        if( HTTP_TEST_ITEM_HEIGHT ==  curTestItem || HTTP_TEST_ITEM_WEIGHT == curTestItem ){
            //将本地查询出来的身高体重成绩在这里与服务器查询出来的成绩进行合并，并去重
            m_pRequestMgr->getPQueryHistoryScore()->mergeAndRemoveCommonBodyTestResults( vecAllHistoryScores );

            //获取合并后的成绩
            QVariantList totalBodyResultRecords = m_pRequestMgr->getPQueryHistoryScore()->getHuamenCompositionList();

            //去重完毕后， 通过信号发送到界面
            qDebug() << "display score size = " << totalBodyResultRecords.size();
            emit m_ui.sigHumanCompositionAvailable( totalBodyResultRecords );
            m_pRequestMgr->getPQueryHistoryScore()->cleanHumanScoreList();
            return;
        }
        else{
            QVector< QStringList > displayList ;
            procHttpQueryRets(displayList, vecAllHistoryScores);
            for( auto &eachEle : displayList ){
                m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, eachEle, false );
            }
        }
    }
    //如果向服务器查询成功,则去除相同的成绩（根据时间戳筛选）
    else{
        //先判断是不是查询的身高体重成绩, 在这个if区域内单独处理
        TEST_ITEM curTestItem = m_pRequestMgr->getPQueryHistoryScore()->getCurTestItem();

        if( HTTP_TEST_ITEM_HEIGHT ==  curTestItem || HTTP_TEST_ITEM_WEIGHT == curTestItem ){

            //将本地查询出来的身高体重成绩在这里与服务器查询出来的成绩进行合并，并去重
            m_pRequestMgr->getPQueryHistoryScore()->mergeAndRemoveCommonBodyTestResults( vecAllHistoryScores );

            //获取合并后的成绩
            QVariantList totalBodyResultRecords = m_pRequestMgr->getPQueryHistoryScore()->getHuamenCompositionList();

            //去重完毕后， 通过信号发送到界面
            qDebug() << "display score size = " << totalBodyResultRecords.size();
            emit m_ui.sigHumanCompositionAvailable( totalBodyResultRecords );
            m_pRequestMgr->getPQueryHistoryScore()->cleanHumanScoreList();
            return;
        }


        QVector< QStringList > vecServerHistorys = data.value<QVector<QStringList>>();

        //服务器端成绩匹配本地得分标准
        for(auto &eachEle : vecServerHistorys)
        {
            eachEle[retKeys::index_testScore] = getScoreByLocalRules(eachEle[retKeys::index_testRet]);
        }

        //将本地的成绩和服务器查询出来的成绩放到一起
        vecAllHistoryScores += vecServerHistorys;
        CHttpRequestMgr::getInstance()->getPQueryHistoryScore()->setVecDisplayRets( vecAllHistoryScores );

        //进行去重
        CHttpRequestMgr::getInstance()->getPQueryHistoryScore()->removeCommonDatas();

        vecAllHistoryScores.clear();
        vecAllHistoryScores = CHttpRequestMgr::getInstance()->getPQueryHistoryScore()->getVecDisplayRets();
        QVector< QStringList > displayList ;
        procHttpQueryRets(displayList, vecAllHistoryScores);

        //2022.12.21 检查是否需要转换分秒格式, 主要针对长跑类测试项
        if( isLongTimeKeepingTestItem( curTestItem ) ){
            for( auto &ele : displayList ){
                ele[ 1 ] = convertSecs2FmtContent( ele[ 1 ].toDouble() );
            }
        }


        for( auto &eachEle :  displayList){
            m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, eachEle, false );
        }
    }

    m_ui.closeRootPopup();

}

QString CMainFrame::getScoreByLocalRules(QString testResult)
{
    QString qsScore("");
    double lfCurScore = -1;

    lfCurScore = static_cast<double>(CTestGuideBase::getScoreInfo().getScore(m_shQuertTestItem, m_userInfo, testResult.toFloat()));
    if(lfCurScore >= 0)
    {
        qsScore = QString::number(lfCurScore, 'f', 0);

        int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
        if (nSoftVersion == VERSION_CD_PSB) {
            if(lfCurScore > 0.9)
            {
                qsScore = "合格";
            }
            else
            {
                qsScore = "不合格";
            }
        }
    }
    else
    {
        qsScore = "没有得分指标";
    }

    return qsScore;
}


void CMainFrame::onTestModelChanged(short shModel)
{
    //将设备存储到数据库
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateDevTestMode(shModel))
    {
        LOGERROR("更新同步/异步测试模式至数据库失败！");
        return;
    }

    if(m_objTestGuideManager.setTestMode(shModel))
    {
        m_objTestGuideManager.cancelTest(false,true);
    }
}

void CMainFrame::onChangeChannelConfig(int channel, int devMaxCount)
{
    bool bSuccess = false;
    //修改信道配置
    bSuccess = m_objSteDevMgr.setChannelInfo(static_cast<unsigned int>(devMaxCount), static_cast<unsigned int>(channel));

    //通知界面更新配置结果
    emit sigUpdateDevConfig(bSuccess);
}

void CMainFrame::initSignPaint()
{
    killProcess("SignPainter.exe");
    initPaintServer();
    initPaint();
}

void CMainFrame::initPaint()
{
    std::wstring operate = QString("runas").toStdWString();
    std::wstring path = QGuiApplication::applicationDirPath().toStdWString();
    std::wstring file= QString(QGuiApplication::applicationDirPath()+"/SignPainter.exe").toStdWString();
    ShellExecute( nullptr, operate.c_str(), file.c_str(), nullptr, path.c_str(), SW_HIDE );

    m_bPainetrRunning = true;
}

void CMainFrame::initPaintServer()
{
    if( nullptr == m_pLocalServer ){
        m_pLocalServer = new QLocalServer( this );
    }
    QScreen *screen   = QGuiApplication::primaryScreen();
    QRect screenRect  = screen->geometry() ;
    int iHeight       = screenRect.height();

    m_dFactor =  iHeight == 1080.0 ? ( iHeight - 100 )/ 2880.0 : iHeight / 2880.0;

    //初始化画板
    connect( m_pLocalServer, &QLocalServer::newConnection, this, &CMainFrame::onSignPainterServerNewConnection, Qt::UniqueConnection );


    QString painterServerName = SIGN_EXECUTE_NAME;
    if( !m_pLocalServer->listen(painterServerName))
    {
        //监听失败,可能是程序崩溃，残留进程服务导致的，移除之
        if(m_pLocalServer->serverError() == QAbstractSocket::AddressInUseError)
        {
            QLocalServer::removeServer( painterServerName );
            //再次监听
            m_pLocalServer->listen( painterServerName );
        }
    }
}

void CMainFrame::killProcess(const QString &exeName)
{
    QStringList args;
    args << "/F" << "/IM" << exeName << "/T";
    QProcess process;
    process.start(QString("TASKKILL"), args );
    process.waitForStarted();
    process.waitForFinished(2000);
}

void CMainFrame::onShowWidgetPainterFlagChanged(QString qsShowFlag)
{
    if( !m_bPainetrRunning )
    {
        initPaint();
    }
    if( nullptr == m_pLocalPainterSocket )
    {
        initPaintServer();
    }

    QString tmpCmd = SIGN_CMD_HIDE;
    if( "1" == qsShowFlag ){
        tmpCmd = SIGN_CMD_DISPLAY;
    }

    if(nullptr != m_pLocalPainterSocket)
    {
        LOGINFO( "send to signpainter = " + tmpCmd.toStdString() );
        m_pLocalPainterSocket->write( tmpCmd.toLocal8Bit() );
        m_pLocalPainterSocket->flush();
    }
}

void CMainFrame::onClearWidgetPainter()
{
    if( !m_bPainetrRunning )
    {
        initPaint();
    }
    if( nullptr == m_pLocalPainterSocket )
    {
        initPaintServer();
    }

    if(nullptr != m_pLocalPainterSocket)
    {
        LOGINFO( "clear cmd = " + SIGN_CMD_CLEAR.toStdString() );
        m_pLocalPainterSocket->write( SIGN_CMD_CLEAR.toLocal8Bit() );
        m_pLocalPainterSocket->flush();
    }
}

void CMainFrame::onChangedSignPainter(QString cmd)
{
    if( !m_bPainetrRunning )
    {
        initPaint();
    }
    if( nullptr == m_pLocalPainterSocket )
    {
        initPaintServer();
    }

    if(m_pLocalPainterSocket != nullptr)
    {
        int count = m_pLocalPainterSocket->write( cmd.toLocal8Bit() );
        qDebug() << "byte count = " << count;
    }
}

void CMainFrame::onSaveWidgetToPng(QString qsShowFlag)
{
    if( !m_bPainetrRunning )
    {
        initPaint();
    }
    if( nullptr == m_pLocalPainterSocket )
    {
        initPaintServer();
    }

    if(nullptr != m_pLocalPainterSocket)
    {
        QString tmpCmd = SIGN_CMD_SAVE + SIGN_CMD_BLANK + qsShowFlag;
        m_pLocalPainterSocket->write( tmpCmd.toLocal8Bit() );
    }
}

void CMainFrame::procBindUserReturnValue(const short &shRet)
{
    QString qsText("");
    switch (shRet) {
    case SUCESS_BIND://将识别成功改为 请选择n号设备, n为所对应设备的分机编号
        if( getActualTestMode(CDevDataCfg::getDevDirectConnectFlag(), m_objTestGuideManager.getTestMode()) == TEST_SYNC)
        {
            int nBindNo = m_objTestGuideManager.getLastBindInfo();
            if(nBindNo < 0)
            {
                qsText = "绑定成功";
            }
            else
            {
                if(m_objTestGuideManager.getLastBindDevType() != DEV_TYPE_RUN_SHORT)
                {
                    qsText = QString("请选择%1号设备").arg(nBindNo);
                }
                else
                {
                    qsText = QString("请进入%1号跑道").arg(nBindNo);
                }
            }
        }
        break;
    case SUCESS_BIND_EXTRA:
        if(m_objTestGuideManager.getTestMode() == TEST_SYNC)
        {
            qsText = "配对成功,请同学就位，等待测试开始";
        }
        break;
    case ALREADY_BIND:
        qsText = "用户已绑定";
        break;
    case NOT_AVAILABLE:
        qsText = "无可用设备";
        break;
    case NOT_AVAILABLE_TEST:
        qsText = "绑定人数达到上限";
        break;
    case START_TEST_FALSE:
        break;
    case STARTING_TEST:
        qsText = "用户已绑定，开始测试";
        break;
    case TEST_HAVE_BEGIN:
        qsText = "测试中";
        break;
    case BANDING_OUTTIME:
        qsText = "绑定超时";
        break;
    case BANDING_CANCEL:
        qsText = "取消绑定";
        break;
    case BANDING_FALSE:
        qsText = "绑定失败";
        if(CDevDataCfg::isSpecialDev(m_objSteDevMgr.getType()))
        {
            qsText = "配对失败";
        }
        break;
    case EXTRA_INFO_HAS_BIND:
        qsText = "已经被绑定";
        break;
    case DEV_NOT_READY:
        qsText = "设备未就绪";
        break;
    case UNKNOWN:
        break;
    default:
        break;
    }

    CTextToSpeech::GetInstance().textToSpeech(qsText/*, 1300*/);
}

void CMainFrame::onStartProcAddUserID(bool bStart, QString id)
{
    //点开录入信息
    if(id.isEmpty())
    {
        m_bIDRecgOnlyFlag = bStart;
        if(bStart)
        {
            m_objIdentifyUser.setIdentifyType(CFindUserBasic::IDENTIFY_BY_USER_ID);
        }
        else
        {
            //恢复摄像头后，不立即进行识别，延时识别人脸
            QTimer::singleShot( 1000, this, &CMainFrame::onTimeOutStartIdentify );
        }
    }
    else
    {
        m_objIdentifyUser.setFindSrcStudentID(id);
    }
}

void CMainFrame::onProcScanUserID(QString id)
{
    //在识别界面支持扫码枪触发
    if(CDevDataCfg::isHomePage(m_ui.getCurPage()))
    {
        m_objIdentifyUser.setFindSrcStudentID(id);
    }
}

//延迟开启人脸识别函数
void CMainFrame::onTimeOutStartIdentify()
{
    if(CDevDataCfg::isHomePage(m_ui.getCurPage()))
    {
        //当录入信息标志为true时，仅开启ID识别
        if(m_bIDRecgOnlyFlag)
        {
            m_objIdentifyUser.setIdentifyType(CFindUserBasic::IDENTIFY_BY_USER_ID);
        }
        else
        {
            m_objIdentifyUser.setIdentifyType(CFindUserBasic::IDENTIFY_BY_MAG_CARD | CFindUserBasic::IDENTIFY_BY_USER_ID
                                              | CFindUserBasic::IDENTIFY_BY_FACE_FEATURE);
        }

    }
}

void CMainFrame::onGetAllDevExtensionNum()
{
    m_ui.clearModelData(CInterAction::DEV_NUM_DATA_MODEL);


    auto mapDev = m_objSteDevMgr.getAllDev();

    //设置的内容为 设备号 分机编号 是否设置成功
    QStringList listContent;
    CDevHardwareBase *pTempDev;

    for(auto &element : mapDev)
    {
        pTempDev = element.second;

        listContent.clear();

        //初始化分机编号之后的设备都可以进行设置
        if(pTempDev->isInitExtensionNo())
        {
            listContent.append(QString::number(pTempDev->getMacID()));
            listContent.append(QString::number(pTempDev->getExtensionNo()));
            listContent.append("");

            m_ui.appendModelData(CInterAction::DEV_NUM_DATA_MODEL, listContent);
        }
    }
}

void CMainFrame::onSetDevExtensionNum(QString devID, int devNum)
{
    unsigned int nDevId = devID.toUInt();
    bool bRet = false;

    bool bHaveExtensionNo = false;
    //分机编号有重复就不能设置成功
    CDevHardwareBase *pDev = nullptr;

    auto mapAllDev = m_objSteDevMgr.getAllDev();
    for(auto &element : mapAllDev)
    {
        if(element.second == nullptr)
            continue;

        if(element.second->getExtensionNo() == devNum)
        {
            bHaveExtensionNo = true;
            break;
        }

        if(element.second->getMacID() == nDevId)
        {
            pDev = element.second;
        }
    }


    if(pDev != nullptr /*&& CDevDataCfg::isValidExtensionNum(devNum)*/ && !bHaveExtensionNo)
    {
        bRet = pDev->setExTensionNo(devNum);
    }

    QString qsTip = bRet ? "true" : "false";

    //设置状态
    m_ui.updateModelCellData(CInterAction::DEV_NUM_DATA_MODEL, 0, devID, 2, qsTip);

    //更新分机编号
    if(bRet)
    {
        m_ui.updateModelCellData(CInterAction::DEV_NUM_DATA_MODEL, 0, devID, 1, QString::number(devNum));
    }
}

bool CMainFrame::onSetCorrectProgress(const int &nStep, const unsigned int &nDevID, const QVariant &val)
{
    auto pDev = m_objSteDevMgr.getDevSteByID(nDevID);
    if(pDev == nullptr || !pDev->isValid())
        return false;

    bool bRet = false;
    short shDevtype = pDev->getDevType();
    float fValue = val.toFloat();

    switch (shDevtype)
    {
    case DEV_TYPE_BODY:
        bRet = setCorrectProgressBody(pDev, nStep, fValue);
        break;
    case DEV_TYPE_SFB:
        bRet = setCorrectProgressSFB(pDev, nStep, fValue);
        break;
    case DEV_TYPE_GRIP:
        bRet = setCorrectProgressGrip(pDev, nStep, fValue);
        break;
    case DEV_TYPE_PUSH_UP:
        bRet = setCorrectProgressGrip(pDev, nStep, fValue);
        break;
    default:
        break;
    }

    if(bRet)
        emit m_ui.sigEnableCorrectViewBtn();

    return bRet;
}

void CMainFrame::onGetCurDevChannel()
{
    //测试信道
    int channel = int(m_objSteDevMgr.getCurDevChannel());

    emit sigUpdateDevChannel( channel );
}

void CMainFrame::onGetCurDevConnectNum()
{
    //连接数量
    int num = int(m_objSteDevMgr.getCurDevMaxConnectNum());

    emit sigUpdateDevMaxConnectNum(num);
}

void CMainFrame::onGetAllSettingViewInfo(int viewIndex)
{
    switch( viewIndex ){
    case  CProjectEnums::SystemSettingPage::PAGE_HOST:
        getHostPageInfo();
        break;

    case  CProjectEnums::SystemSettingPage::PAGE_DEV:
        getDevPageInfo();
        break;

    case  CProjectEnums::SystemSettingPage::PAGE_USING:
        getUsingPageInfo();
        break;

    case  CProjectEnums::SystemSettingPage::PAGE_DATAMANAGE:
        break;

    default:
        getHostPageInfo();
        getDevPageInfo();
        getUsingPageInfo();
        break;
    }
}

void CMainFrame::getMultiDevStateSetToServer()
{
    //QVariantList listInfos;
}

void CMainFrame::resetLastIdentifyInfo()
{
    m_llLastIdentify = -1;
}

void CMainFrame::updateIdentifyInfo(const long long &llUserID)
{
    m_llLastIdentify = llUserID;
    m_llLastIdentifyTime = QDateTime::currentMSecsSinceEpoch();
}

bool CMainFrame::setSBJStartPos(const int &nPos)
{
    auto mapDev = m_objSteDevMgr.getAllDev();

    bool bRet = CDatabaseManage::GetInstance().getDevTestInfoConfig()->setStadingJumpStartPos( nPos );
    if(!bRet)
        return false;

    CDevHardwareBase *pDev;
    CDevSteSBJ *pDevSBJ = nullptr;
    for(auto &element : mapDev)
    {
        pDev = element.second;
        if(pDev != nullptr && pDev->getDevType() == DEV_TYPE_SBJ)
        {
            pDevSBJ = reinterpret_cast<CDevSteSBJ *>(pDev);
            pDevSBJ->setStartPos(nPos);
        }
    }

    //CDevDataCfg::delayMSec(2000);

    for(auto &element : mapDev)
    {
        pDev = element.second;
        if(pDev != nullptr && pDev->getDevType() == DEV_TYPE_SBJ)
        {
            pDevSBJ = reinterpret_cast<CDevSteSBJ *>(pDev);
            bRet &= pDevSBJ->getCalibrationSPosFlag();
            if(!pDevSBJ->getCalibrationSPosFlag())
                break;
        }
    }

    return bRet;
}

void CMainFrame::initQmlEngine()
{
    CInterAction *pGUI = &m_ui;
    m_objQmlEngine.rootContext()->setContextProperty( "InterAction", pGUI );
    m_objQmlEngine.rootContext()->setContextProperty( "commonDataModel", &pGUI->m_commonDataModel );
    m_objQmlEngine.rootContext()->setContextProperty( "studentDataModel", &pGUI->m_studentDataModel );
    m_objQmlEngine.rootContext()->setContextProperty( "scoreDataModel", &pGUI->m_scoreDataModel );
    m_objQmlEngine.rootContext()->setContextProperty( "DevNumModel", &pGUI->m_devNumDataModel );
    m_objQmlEngine.addImageProvider( QLatin1String( "imageProvider" ), &pGUI->m_objImgProvider );

    m_objQmlEngine.addImportPath( ":/XTC" );

    qmlRegisterType<CProjectEnums>( "ProjectEnums", 1, 0, "Enums" );

    qmlRegisterType<CustomCurveView>( "CurveChartView", 1, 0, "CustomCruveView" );
    qmlRegisterType<CCustVTChartView>( "VTCurveChartView", 1, 0, "VTCurveChartView");
    qmlRegisterType<CCustFVChartView>( "FVCurveChartView", 1, 0, "FVCurveChartView" );
    qmlRegisterType<CCustRTChartView>( "RTCurveChartView", 1, 0, "RTCurveChartView" );

    qmlRegisterType<COpencvVideoPlayer>("CustVideoPlayer", 1, 0, "OpencvVideoPlayer");
    qmlRegisterType<COpencvVideoControl>("CustVideoPlayer", 1, 0, "OpencvVideoControl");
}

void CMainFrame::initConnection()
{
    ///协助设备管理的相关消息
    //身份识别nfc
    connect(&m_objAssistDevMgr, &CAssistDevManage::signalDevIdentifyConnectChange, this, &CMainFrame::onDevConnectChangeIdentify);

    //电源管理设备
    connect(&m_objAssistDevMgr, &CAssistDevManage::signalDevPowerConnectChange, this, &CMainFrame::onDevConnecChangePower, Qt::QueuedConnection);

    //发令枪信号
    connect(&m_objAssistDevMgr, &CAssistDevManage::signalStartingGun, &m_objTestGuideManager, &CTestGuideManager::onStartingGun);

    //身份识别
    connect(&m_objIdentifyUser, &CIdentifyUser::signalIdentifySucess, this, &CMainFrame::onIdentifySuccess, Qt::ConnectionType::DirectConnection);
    connect(&m_objIdentifyUser, &CIdentifyUser::signalIdentifyFalse, this, &CMainFrame::onIdentifyFail, Qt::ConnectionType::DirectConnection);

    //跑步刷卡nfc设备
    connect(&m_objAssistDevMgr, &CAssistDevManage::signalDevRunNfcConnectChange, this, &CMainFrame::onDevConnectChangeRunMidNFC);

    //发令枪设备
    connect(&m_objAssistDevMgr, &CAssistDevManage::signalDevStartingGunConnectChange, this, &CMainFrame::onDevConnectChangeStartingGun);

    //体侧设备变更
    connect(&m_objSteDevMgr, &CSteDeviceManager::signalSteDevConnectChange, this, &CMainFrame::onDevConnectChangeSteDev, Qt::DirectConnection);
    connect(&m_objSteDevMgr, &CSteDeviceManager::signalFactoryConnectChange, this, &CMainFrame::onDevConnectChangeSteFactory, Qt::DirectConnection);
    connect(&m_timerGetDevConnectCount, &QTimer::timeout, this, &CMainFrame::onTimerOutGetDevConnectNum);

    //犯规类型变更
    connect(&m_objAssistDevMgr, &CAssistDevManage::signalFoulType, &m_objTestGuideManager, &CTestGuideManager::onSignalFoulType);

    //本地数据与服务器同步
    connect(&m_timerSyncServerInfo, &QTimer::timeout, this, &CMainFrame::onTimerSyncServerInfo);

    //处理语音
    connect(&CTextToSpeech::GetInstance(), &CTextToSpeech::signalSpeakStateChange, this, &CMainFrame::onSpeakState, Qt::DirectConnection);

    //qml页面切换
    CInterAction *pGui = &m_ui;
    connect( pGui, &CInterAction::signalPageChanged, this, &CMainFrame::onPageChanged);

    ///按钮点击,全部使用队列信息连接方式
    connect( pGui, &CInterAction::signalClickBtnStartTest, this, &CMainFrame::onClickBtnStartTest, Qt::QueuedConnection);
    connect( pGui, &CInterAction::signalClickBtnBackHome, this, &CMainFrame::onClickBtnBackHome, Qt::QueuedConnection);
    connect( pGui, &CInterAction::signalClickBtnTestPageBack, this, &CMainFrame::onClickBtnTestPageBack, Qt::QueuedConnection);
    connect( pGui, &CInterAction::signalClickBtnInfoPageBack, this, &CMainFrame::onClickBtnInfoPageBack, Qt::QueuedConnection);
    connect( pGui, &CInterAction::signalClickBtnSubmitResult, this, &CMainFrame::onClickBtnSubmitResult/*, Qt::QueuedConnection*/);
    connect( pGui, &CInterAction::signalClickBtnVerifySubmitResult, this, &CMainFrame::onClickBtnVerifySubmitResult/*, Qt::QueuedConnection*/);
    //重新测试(同步模式：短跑、跳绳、中长跑)
    connect( pGui, &CInterAction::signalClickBtnReTest, this, &CMainFrame::onReTest, Qt::QueuedConnection);
    //取消测试
    connect( pGui, &CInterAction::signalClickBtnCancelTest, this, &CMainFrame::onCancelTest, Qt::QueuedConnection);

    connect( pGui, &CInterAction::sigDelScore, this, &CMainFrame::onDelScore);

    //界面设置消息(参数：QvariantMap)
    connect( pGui, &CInterAction::sigDevConfigInfoChanged, this, &CMainFrame::onDevConfigInfoChanged);

    //查询成绩
    //    connect( m_pQueryScore, &CHttpAbstractNetWork::sigRequestRetAvailable, this, &CMainFrame::onQueryScoreFinished );

    //同步异步模式切换
    connect( pGui, &CInterAction::sigTestModelChanged, this, &CMainFrame::onTestModelChanged);

    //信道配置修改
    connect( pGui, &CInterAction::sigDevChannelConfigChanged, this, &CMainFrame::onChangeChannelConfig);

    //签字板显隐
    connect( pGui, &CInterAction::signalShowSignPainterWidget, this, &CMainFrame::onShowWidgetPainterFlagChanged );

    //清除签字板内容
    connect( pGui, &CInterAction::signalClearPainter, this, &CMainFrame::onClearWidgetPainter );

    //改变签字版的尺寸和位置
    connect( pGui, &CInterAction::sigChangeSignPainter, this, &CMainFrame::onChangedSignPainter );

    //保存签字板内容
    connect( pGui, &CInterAction::signalSaveToPng, this, &CMainFrame::onSaveWidgetToPng, Qt::ConnectionType::DirectConnection);

    //界面向后台发起查询成绩请求
    connect( pGui, &CInterAction::signalQueryHistoryScore, this, &CMainFrame::onQueryHistoryScore );

    //设备配置修改成功与否
    connect( this, &CMainFrame::sigUpdateDevConfig, pGui, &CInterAction::sigUpdateHWConfig);

    //是否增加识别学生id
    connect( pGui, &CInterAction::sigProcAddUserID, this, &CMainFrame::onStartProcAddUserID);

    //处理扫码枪识别学生id
    connect( pGui, &CInterAction::sigCatchScanCodeInfo, this, &CMainFrame::onProcScanUserID);

    //界面获取所有设备的信息
    connect( pGui, &CInterAction::sigGetAllDevToSetNum, this, &CMainFrame::onGetAllDevExtensionNum/*, Qt::QueuedConnection*/);
    //设置设备分机编号
    connect( pGui, &CInterAction::sigSetDevNumber, this, &CMainFrame::onSetDevExtensionNum, Qt::QueuedConnection);

    //设置校准步骤
    connect( pGui, &CInterAction::sigCorrectProgress, this, &CMainFrame::onSetCorrectProgress);

    connect( pGui, &CInterAction::sigTestSignal, this, &CMainFrame::queryStudentByUniqueID, Qt::QueuedConnection );

    //    //测试信号,用于调试使用
    //    connect( pGui, &CInterAction::sigTestSignal, this, [=]( QString stuId ){
    ////        CUserInfo userInfo;
    ////        vector<CUserInfo> info;
    ////        userInfo.m_bUseGenderToSelect = true;
    ////        userInfo.m_bGender = true;
    ////        CDatabaseManage::GetInstance().getTableUserIdentityInfo()->getUserInfo(userInfo, info);
    ////        static unsigned int s_Index = info.size() - 1;


    ////        if(info.size() != 0)
    ////        {
    ////            stuId = info[ s_Index ].m_strStudentId;
    ////        }

    ////        m_objIdentifyUser.setFindSrcStudentID(stuId);
    //    });

    //设备信道号
    connect( this, &CMainFrame::sigUpdateDevChannel, pGui, &CInterAction::sigSetDevChannelInfo);
    connect(pGui, &CInterAction::sigGetCurDevChannel, this, &CMainFrame::onGetCurDevChannel);

    //设置设备最大连接数量
    connect( this, &CMainFrame::sigUpdateDevMaxConnectNum, pGui, &CInterAction::sigSetDevMaxConnectNum);
    connect(pGui, &CInterAction::sigGetCurDevConnectNum, this, &CMainFrame::onGetCurDevConnectNum);


    connect( pGui, &CInterAction::sigGetSpecificSettingViewInfo, this, &CMainFrame::onGetAllSettingViewInfo );

    connect(qApp,&QApplication::aboutToQuit,this,&CMainFrame::close);
    connect(qApp,&QApplication::lastWindowClosed,this,&CMainFrame::close);

    connect(this, &CMainFrame::sigQuite, pGui, &CInterAction::quit);

}



void CMainFrame::initNetWorkConnection()
{

    if( nullptr == m_pRequestMgr )
    {
        m_pRequestMgr =CHttpRequestMgr::getInstance();

    }
    connect( m_pRequestMgr, &CHttpRequestMgr::sigGetSampleVideoFinished, this, &CMainFrame::onSampleVideoUrlsFinished );
    connect( m_pRequestMgr, &CHttpRequestMgr::sigSyncScoreStdFinished, this, &CMainFrame::onSyncScoreStdFinished );
    connect( m_pRequestMgr, &CHttpRequestMgr::sigSyncUserInfoFinished, this, &CMainFrame::onSyncUserInfoFinished );
    connect( m_pRequestMgr, &CHttpRequestMgr::sigSyncClassInfoFinished, this, &CMainFrame::onSyncClassInfoFinished );
    connect( m_pRequestMgr, &CHttpRequestMgr::sigUserIdentifyFinished, this, &CMainFrame::onUserIdentifyFinished );
    connect( m_pRequestMgr, &CHttpRequestMgr::sigHeartBeatFinished, this, &CMainFrame::onSendHeartBeatFinished );
    connect( m_pRequestMgr, &CHttpRequestMgr::sigQueryHistoryScoreFinished, this, &CMainFrame::onQueryScoreFinished, Qt::QueuedConnection );

    //2022.12.13 绑定批量心跳返回结果
    connect( m_pRequestMgr, &CHttpRequestMgr::sigSendMultiHeartBeatFinished, this, &CMainFrame::onSendMultiHeartBeatFinished, Qt::QueuedConnection );
    //    connect( m_pRequestMgr, &CHttpRequestMgr::sigUploadCommonScore, this, &CMainFrame::onUploadCommonScoreFinished );
    //    connect( m_pRequestMgr, &CHttpRequestMgr::sigGetSampleVideoFinished, this, &CMainFrame::onSampleVideoUrlsFinished );
    //    connect( m_pRequestMgr, &CHttpRequestMgr::sigUploadBodyScore, this, &CMainFrame::onUploadBodyScoreFinished );
    //    connect( m_pRequestMgr, &CHttpRequestMgr::sigUploadEyeSightScore, this, &CMainFrame::onUploadEyeSightScoreFinished );
    //    connect( m_pRequestMgr, &CHttpRequestMgr::sigTouristUsageRecordFinished, this, &CMainFrame::onTouristUsageRecordFinished );
}

void CMainFrame::syncScoreStd()
{
    emit sigSyncScoreStd();
}

void CMainFrame::syncUserInfo(QString timeStamp)
{
    emit sigSyncUserInfo( timeStamp );
}

void CMainFrame::syncClassInfo(QString timeStamp)
{
    emit sigSyncClassInfo( timeStamp );
}

void CMainFrame::userIdentify(QString faceFeature, QString cardNo)
{
    emit sigUserIdentify( faceFeature, cardNo );
}

void CMainFrame::sendHeartBeat(devStatus status, QString battery, QString devCode, short devType)
{
    if( nullptr == m_pRequestMgr ){
        m_pRequestMgr = CHttpRequestMgr::getInstance();
    }

    m_pRequestMgr->sendHeartBeat( status, battery, devCode, devType);//onSendHeartBeat
}

void CMainFrame::upLoadCommonScore(QMap<QString, QVariant> infos)
{
    QVariant data;
    data.setValue(infos );
    //    emit sigUploadCommonScore( data );
}

void CMainFrame::upLoadBodyScore(QMap<QString, QVariant> infos)
{
    QVariant data;
    data.setValue(infos );
    emit sigUploadBodyScore( data );
}

void CMainFrame::upLoadEyeSightScore(QMap<QString, QVariant> infos)
{
    QVariant data;
    data.setValue(infos );
    emit sigUploadEyeSightScore( data );
}

void CMainFrame::touristUsageRecord(QString devCode, DEV_TYPE devType)
{

}

void CMainFrame::sendMultiHeartBeat()
{
    QVariantMap tmpDevInfo;
    QVariantList devInfoList;

    CDevHardwareBase *pTempDev;

    auto mapDev = m_objSteDevMgr.getAllDev();


    // 存在设备为空的情况
    if( mapDev.size() <= 0 ){
        sendEmptyHeartbeatPkt();
        return;
    }

    for( auto &ele : mapDev ){
        pTempDev = ele.second;

        tmpDevInfo[ HEARTBEAT_KEY_CODE ]   =  pTempDev->getMacID();
        tmpDevInfo[ HEARTBEAT_KEY_TYPE ]   =  CDevDataCfg::getHttpDevTypeFromType( pTempDev->getDevType() );
        tmpDevInfo[ HEARTBEAT_KEY_STATUS ] =  devStatus::STATUS_CONNECTED;

        devInfoList << tmpDevInfo;
    }

    m_pRequestMgr->sendMultiHeartBeat( devInfoList );
}

bool CMainFrame::procHttpQueryRets(QVector<QStringList> &displayVec, const QVector<QStringList> &vecStrList)
{
    bool bRes = false;
    displayVec.clear();
    TEST_ITEM curTestItem = m_pRequestMgr->getPQueryHistoryScore()->getCurTestItem();
    if(CDevDataCfg::isNeedViolationInfoItem(curTestItem))
    {
        for(auto eleStrList : vecStrList)
        {
            if(eleStrList.size() == 3)
            {
                //结果与违规次数组装
                if( eleStrList[1].contains('-'))
                {
                    QStringList resultList = eleStrList[2].split('-');
                    if(resultList.size() == 2)
                    {
                        QStringList displayList;
                        displayList.append(eleStrList[0]);
                        displayList.append(resultList[0]);
                        displayList.append(resultList[1]);
                        displayList.append(eleStrList[2]);
                        displayVec.push_back(displayList);
                    }
                }
                //处理无效成绩状况
                else
                {
                    QStringList displayList;
                    displayList.append(eleStrList[0]);
                    displayList.append(eleStrList[1]);
                    displayList.append("-");
                    displayList.append(eleStrList[2]);
                    displayVec.push_back(displayList);
                }
                bRes = true;
            }
            else
            {
                bRes = false;
            }
        }
    }

    if(!bRes)
    {
        displayVec = vecStrList;
    }

    return bRes;
}

void CMainFrame::onSampleVideoUrlsFinished(bool ok)
{
    //请求失败
    if( !ok ){
        qDebug() << "error string = " << m_pRequestMgr->getErrorString();
        return;
    }
    //存放指导视频的链接相关信息
    QVector<QMap< QString, QString >> urls = m_pRequestMgr->getVecSampleVieoUrls();
}

//本地数据库 同步成绩标准信息
void CMainFrame::onSyncScoreStdFinished(bool ok)
{
    //0.判断是否更新成功
    if( !ok )
    {
        LOGERROR("Get Sync Score Faild ! ErrorCode is : " + m_pRequestMgr->getErrorString().toStdString());

        //失败后允许下一次发送请求
        m_bSyncScoreFlag = true;

        //没有更新数据库，则启动计时器
        if(!m_bCompleteScoreFlag && !m_timerSyncServerInfo.isActive())
        {
            m_timerSyncServerInfo.start(m_iSyncServerInfoTime);
        }
    }
    else
    {
        LOGINFO("Get Sync Score Success !");

        if(!m_bCompleteScoreFlag)
        {
            m_bCompleteScoreFlag = true;

            //发送消息同步数据库
            if(nullptr == m_pDataSync)
            {
                initDataSync();
            }

            emit sigUpdateScoreInfo();  //onUpdateScoreInfo
        }
    }
}

//本地数据库 同步用户数据信息
void CMainFrame::onSyncUserInfoFinished(bool ok)
{
    //0.判断是否更新成功
    if( !ok )
    {
        LOGERROR("Get Sync User Faild ! ErrorCode is : " + m_pRequestMgr->getErrorString().toStdString());

        //失败后允许下一次发送请求
        m_bSyncUserFlag = true;

        //没有更新数据库，则启动计时器
        if(!m_bCompleteUserFlag && !m_timerSyncServerInfo.isActive())
        {
            m_timerSyncServerInfo.start(m_iSyncServerInfoTime);
        }
    }
    else
    {
        LOGINFO("Get Sync User Success !");

        if(!m_bCompleteUserFlag)
        {
            m_bCompleteUserFlag = true;

            //发送消息同步数据库
            if(nullptr == m_pDataSync)
            {
                initDataSync();
            }

            emit sigUpdateUserInfo();   //onUpdateUserInfo
        }
    }

    m_syncUserInfoRunning = false;
}

//本地数据库 同步班级数据信息
void CMainFrame::onSyncClassInfoFinished(bool ok)
{
    //0.判断是否更新成功
    if( !ok )
    {
        LOGERROR("Get Sync Class Faild ! ErrorCode is : " + m_pRequestMgr->getErrorString().toStdString());

        //失败后允许下一次发送请求
        m_bSyncClassFlag = true;

        //若没有更新数据库，则启动计时器
        if(!m_bCompleteClassFlag && !m_timerSyncServerInfo.isActive())
        {
            m_timerSyncServerInfo.start(m_iSyncServerInfoTime);
        }
    }
    else
    {
        LOGINFO("Get Sync Class Success !");

        if(!m_bCompleteClassFlag)
        {
            m_bCompleteClassFlag = true;

            //发送消息同步数据库
            if(nullptr == m_pDataSync)
            {
                initDataSync();
            }

            emit sigUpdateClassInfo();
        }
    }
}

void CMainFrame::onUserIdentifyFinished(bool ok)
{
    if( !ok ){
        qDebug() << "error string = " << m_pRequestMgr->getErrorString();
        return;
    }
    //存放识别出来的用户信息
    QMap<QString, QString> userInfo = m_pRequestMgr->getMapUserInfo();
}

void CMainFrame::onSendHeartBeatFinished(bool ok)
{
    //判断心跳是否发送成功
    if( !ok ){
        m_bNetworkFlag = false;
        qDebug() << "error string = " << m_pRequestMgr->getErrorString();
        return;
    }

    m_bNetworkFlag = true;


    //开启数据同步逻辑
    CServerCommunication::Get_Instance().setUpdateType(CServerCommunication::UPDATE_TYPE_TEST_RESULT);
    CServerCommunication::Get_Instance().setUpdateType(CServerCommunication::UPDATE_TYPE_TOURIST_USAGE);

    //注：心跳发送成功后，启动同步机制(此处仅触发一次，后续会在定时器中处理)
    if(m_bFirstSyncFlag)
    {
        m_bFirstSyncFlag = false;
        if(m_bSyncUserFlag)
        {
            m_bSyncUserFlag = false;

            sendSyncUserCommand();

            LOGINFO("Send Sync User Command");
        }
        if(m_bSyncClassFlag)
        {
            m_bSyncClassFlag = false;

            sendSyncClassCommand();

            LOGINFO("Send Sync Class Command");
        }
        if(m_bSyncScoreFlag)
        {
            m_bSyncScoreFlag = false;

            sendSyncScoreCommand();

            LOGINFO("Send Sync Score Command");
        }
    }
}

void CMainFrame::onUploadCommonScoreFinished(bool ok)
{
    //判断一般成绩是否上传成功
    if( !ok ){
        qDebug() << "error string = " << m_pRequestMgr->getErrorString();
        return;
    }
    qDebug() << "upload common socre std success...";
}

void CMainFrame::onUploadBodyScoreFinished(bool ok)
{
    //判断人体成分成绩是否上传成功
    if( !ok ){
        qDebug() << "error string = " << m_pRequestMgr->getErrorString();
        return;
    }
    qDebug() << "UploadBodyScore success...";
}

void CMainFrame::onUploadEyeSightScoreFinished(bool ok)
{
    //判断视力成绩是否上传成功
    if( !ok ){
        qDebug() << "error string = " << m_pRequestMgr->getErrorString();
        return;
    }
    qDebug() << "UploadEyeSightScore success...";
}

void CMainFrame::onTouristUsageRecordFinished(bool ok)
{
    //判断游客使用记录是否上传成功
    if( !ok ){
        qDebug() << "error string = " << m_pRequestMgr->getErrorString();
        return;
    }
    qDebug() << "TouristUsageRecord success...";
}

void CMainFrame::onSyncStatusChanged(short shSyncStatus)
{
    //1.通知UI修改显示
    switch (shSyncStatus)
    {
    case CDataSynchronize::SYNC_PROCESS_USER:
        CErrorCode::setErrorCode(MERR_SYNC_USER);
        m_ui.setViewErorrString(MERR_SYNC_USER);
        m_bAvailable = false;
        break;
    case CDataSynchronize::SYNC_PROCESS_CLASS:
        CErrorCode::setErrorCode(MERR_SYNC_CLASS);
        m_ui.setViewErorrString(MERR_SYNC_CLASS);
        m_bAvailable = false;
        break;
    case CDataSynchronize::SYNC_PROCESS_SCORE:
        CErrorCode::setErrorCode(MERR_SYNC_SCORE);
        m_ui.setViewErorrString(MERR_SYNC_SCORE);
        m_bAvailable = false;
        break;
    case CDataSynchronize::SYNC_COMPLETE_USER:
        m_bAvailable = true;
        m_ui.setViewErorrString(MERR_SYNC_USER,false);
        //用户同步完成，通知重新加载本地人脸特征
        m_objIdentifyUser.reloadFaceFeature();
        break;
    case CDataSynchronize::SYNC_COMPLETE_CLASS:
        m_bAvailable = true;
        m_ui.setViewErorrString(MERR_SYNC_CLASS,false);
        break;
    case CDataSynchronize::SYNC_COMPLETE_SCORE:
        m_bAvailable = true;
        m_ui.setViewErorrString(MERR_SYNC_SCORE,false);
        //成绩标准更新完成重新加载
        CTestGuideBase::getScoreInfo().onReloadScoreRule();
        break;
    case CDataSynchronize::SYNC_FAILED_USER:
    case CDataSynchronize::SYNC_FAILED_CLASS:
    case CDataSynchronize::SYNC_FAILED_SCORE:
        m_bAvailable = true;
        break;
    default:
        m_bAvailable = true;
        break;
    }
}

void CMainFrame::onStopSync()
{
    //清除同步线程
    if(nullptr != m_pSyncThread)
    {
        if(m_pSyncThread->isRunning())
        {
            m_pSyncThread->quit();
            m_pSyncThread->wait();
        }
    }

    m_pDataSync = nullptr;
    m_pSyncThread = nullptr;

    //停止同步计时器
    m_timerSyncServerInfo.stop();
}

void CMainFrame::onSendMultiHeartBeatFinished(bool ok)
{
    if( !ok ){
        LOGERROR( "heart beat failed" );
        return;
    }

    LOGINFO( "heart beat success" );

    int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
    if(nSoftVersion == VERSION_QLNJXX)
    {
        //2023.03.10 新增，如果获取到的schoolID不为空,则开始同步数据
        QString schoolID = CHttpRequestMgr::getInstance()->getPMultiHeartBeatInfos()->schoolId();
        if( schoolID.isEmpty() ){
            return;
        }
    }

    if( nullptr == m_pRequestMgr ){
        m_pRequestMgr = CHttpRequestMgr::getInstance();
    }

    long long localMaxTimeStamp = CDatabaseManage::GetInstance().getTableUserIdentityInfo()->queryMaxTimeStamp().toLongLong();
    localMaxTimeStamp = QDateTime::fromMSecsSinceEpoch( localMaxTimeStamp ).toString( "yyyyMMddhhmmss" ).toLongLong();
    long long serverMaxTimeStamp = m_pRequestMgr->getPMultiHeartBeatInfos()->lastSyncTime().toLongLong();

    //如果本地的最大时间戳不小于服务器的最大时间戳,则不用发起同步用户信息请求
    if( localMaxTimeStamp >= serverMaxTimeStamp ){
        return;
    }

    //如果正在处理同步用户信息的其请求，则等待处理完过后，下次心跳响应时再发起请求
    if( m_syncUserInfoRunning ){
        return;
    }

    m_bCompleteUserFlag = false;
    sendSyncUserCommand();
}

void CMainFrame::queryLocalHumanCompositionTestResult(QVariantList &srcTestRetList, const QVariantMap &queryConditions)
{
    //查询本地身高体重成绩
    vector<CTestResultInfo> vecUserHeights;
    vector<CTestResultInfo> vecUserWeights;

    long long userId = queryConditions.value( "userId" ).toLongLong();
    long long databaseTimeStamp = queryConditions.value( "dateBaseTimeStamp" ).toLongLong();
    short scoreType = queryConditions.value( "scoreType" ).toInt();

    //查询身高
    CDatabaseManage::GetInstance().getTabTestItemResult()->getResultByInfo( vecUserHeights, userId, TEST_ITEM_HEIGHT, databaseTimeStamp, scoreType );

    //查询体重
    CDatabaseManage::GetInstance().getTabTestItemResult()->getResultByInfo( vecUserWeights, userId, TEST_ITEM_WEIGHT, databaseTimeStamp, scoreType );

    //以时间戳为索引，组装身高和体重结果
    QVariantList tmpRetList;

    QVariantMap tmpHeightRetMap;
    for( auto &ele : vecUserHeights ){
        tmpHeightRetMap[ "recordTime" ]    = QString::fromStdString( ele.m_strStartTime );
        tmpHeightRetMap[ "heightRecord" ]  = QString::fromStdString( ele.m_strMulitResult ).split( ";" ).first();

        tmpRetList << tmpHeightRetMap;
    }

    QVariantMap tmpWeightRetMap;
    for( auto &ele : vecUserWeights ){
        tmpWeightRetMap[ "recordTime" ]       = QString::fromStdString( ele.m_strStartTime );
        tmpWeightRetMap[ "weightRecord" ]     = QString::fromStdString( ele.m_strMulitResult ).split( ";" ).first();

        tmpRetList << tmpWeightRetMap;
    }


    //去除身高和体重不能按时间戳对应起来的成绩
    for( int index = 0; index < tmpRetList.size(); index++ ){

        QString heightRecordTime = tmpRetList.at( index ).toMap().value( "recordTime" ).toString();

        for( int findIndex = index + 1; findIndex < tmpRetList.size(); findIndex++ ){

            if( heightRecordTime == tmpRetList.at( findIndex ).toMap().value( "recordTime" ).toString() ){
                QString heightVal = tmpRetList.at( index ).toMap().value( "heightRecord" ).toString();
                QString weightVal = tmpRetList.at( findIndex ).toMap().value( "weightRecord" ).toString();
                QVariantMap tmpRecord = { { "recordTime", heightRecordTime }, { "height", heightVal }, { "weight", weightVal } };
                srcTestRetList << tmpRecord;
                break;
            }
        }
    }
}

void CMainFrame::removeSingleRecord(vector<CTestResultInfo> &vecHeightRecords, vector<CTestResultInfo> &vecWeightRecords)
{
    for( auto &ele : vecHeightRecords ){

    }
}

void CMainFrame::getHostPageInfo()
{
    //设置当前信道
    int channelNumber = m_objSteDevMgr.getCurDevChannel();
    m_ui.getSystemSettingMgr().hostConfiger().setCurDevChannel( QString::number( channelNumber ) );

    //获取提交配置
    CCommitPopupConfig infos;
    CDatabaseManage::GetInstance().getPCommitTableConfig()->queryPopupConfig( infos );
    QVariantMap commitCfgMap = { { "requireSignature", infos.m_signatureExisted },
                                 { "delScoreBtnEnable", infos.m_delBtnEnable },
                                 { "autoCommitTime", infos.m_autoCommitCountTime },
                                 { "requiredUserInfoPage", infos.m_userInfoPageExisted }
                               };


    //设置是否需要签名
    m_ui.getSystemSettingMgr().setSpecificElementData( CProjectEnums::PAGE_HOST,
                                                       CHostSettingConfig::HostElementData::RequireSignature, commitCfgMap );

    //设置是否开启取消成绩按钮
    m_ui.getSystemSettingMgr().setSpecificElementData( CProjectEnums::PAGE_HOST,
                                                       CHostSettingConfig::HostElementData::DelScoreBtnEnable, commitCfgMap );

    //设置自动提交倒计时
    m_ui.getSystemSettingMgr().setSpecificElementData( CProjectEnums::PAGE_HOST,
                                                       CHostSettingConfig::HostElementData::AutoCommitTime, commitCfgMap );

    //设置是否开启用户信息过渡页
    m_ui.getSystemSettingMgr().setSpecificElementData( CProjectEnums::PAGE_HOST,
                                                       CHostSettingConfig::HostElementData::RequireUserInfoView, commitCfgMap );
}

void CMainFrame::getDevPageInfo()
{
    //获取设备分机编号
    m_ui.getSystemSettingMgr().devConfiger().setDevNumberMap( m_objSteDevMgr.getDevNumbers() );

    //获取外接设备列表( 目前只有发令枪 )( 暂不设置 )

}

void CMainFrame::getUsingPageInfo()
{
    //设置当前设备的测试项， 设置设备后，CDevSettingConfig会自动设置对应的测试项
    //    QVariantMap usingPageInfoMap = { { "devCorrespondingTestItem", m_objSteDevMgr.getType() } };
    //    m_ui.getSystemSettingMgr().setSpecificElementData
    //            ( CProjectEnums::PAGE_USING, CUsingSettingConfig::CUsingElementData::DevTYPECorresponding, usingPageInfoMap );

    //获取无操作超时时间

    //获取测试次数选择

}

void CMainFrame::onSignPainterServerNewConnection()
{
    m_pLocalPainterSocket = m_pLocalServer->nextPendingConnection();
    if(nullptr == m_pLocalPainterSocket )
    {
        LOGERROR("Init PainterSocket fail");
        return ;
    }

    connect( m_pLocalPainterSocket, &QLocalSocket::readyRead, this, &CMainFrame::onSignPainterSocketReadyRead, Qt::UniqueConnection );

    int width  = static_cast<int>(2800 * m_dFactor);
    int height = static_cast<int>(1245  * m_dFactor);
    int posX   = static_cast<int>(500  * m_dFactor);
    int posY   = static_cast<int>(2260 * m_dFactor);

    //        int width  = 572;
    //        int height = 163;
    //        int posX   = 40;
    //        int posY   = 589;

    QString signpainterPara = QString( "%1, %2, %3, %4" ).arg( QString::number( width ) )
            .arg( QString::number( height ) )
            .arg( QString::number( posX ) )
            .arg( QString::number( posY ) );

    emit m_ui.sigDisplaySignpainterParam( signpainterPara );

    QString tmpCmd = SIGN_CMD_POSTION
            .arg( QString::number(width )).arg( QString::number(height))
            .arg( QString::number(posX )).arg( QString::number(posY));
    m_pLocalPainterSocket->write( tmpCmd.toLocal8Bit() );
}

void CMainFrame::onSignPainterSocketReadyRead()
{
    QString content = QString::fromUtf8( m_pLocalPainterSocket->readAll() );
    //获取签字版保存结果, 成功返回 saveRet+success   失败返回saveRet+failed
    if( content.startsWith( "saveRet" )){
        QStringList list = content.split( "+" );
        //如果保存成功
        if( "success" == list[1] ){

        }else{
            //如果保存失败，
            qDebug() << "save faield...";
        }
    }
}

QString CMainFrame::convertSecs2FmtContent(const double &seconds)
{
    QTime tmpTime( 0, 0 );
    tmpTime = tmpTime.addMSecs( seconds * 1000 );
    QString ret = tmpTime.toString( "mm·ss" );
    return ret;
}

bool CMainFrame::isLongTimeKeepingTestItem(const TEST_ITEM &srcTestItem)
{
    switch( srcTestItem ){
    case HTTP_TEST_ITEM_RUN_400:
    case HTTP_TEST_ITEM_RUN_4_100:
    case HTTP_TEST_ITEM_RUN_1500:
    case HTTP_TEST_ITEM_RUN_1000:
    case HTTP_TEST_ITEM_RUN_800:
    case HTTP_TEST_ITEM_RUN_2000:
        return true;
    default:
        return false;
    }
    return false;
}

void CMainFrame::sendEmptyHeartbeatPkt()
{
    //软件启动阶段先发送一次空心跳, 验证网络情况
    QVariantMap  emptyHeartbeatPkt;
    QVariantList emptyHeartbeatPktList;
    emptyHeartbeatPkt[ HEARTBEAT_KEY_STATUS ] = devStatus::STATUS_CONNECTED;
//    emptyHeartbeatPkt[ HEARTBEAT_KEY_CODE ] = "INIT_1_10025";
//    emptyHeartbeatPkt[ HEARTBEAT_KEY_TYPE ] = "1";
    emptyHeartbeatPktList << emptyHeartbeatPkt;

    m_pRequestMgr->sendMultiHeartBeat( emptyHeartbeatPktList );  // //onSendMultiHeartBeat
}

void CMainFrame::getSampleVideoUrls(QString timeStamp)
{
    emit sigGetSampleVideoUrls( timeStamp );
}

bool CMainFrame::startIdentify()
{
    m_objAssistDevMgr.getIdentifyCamera().registerNotifyObj(m_objIdentifyUser.getFeaceRecongition());
    m_objAssistDevMgr.getIdentifyCamera().registerNotifyObj(&m_ui);
    m_objIdentifyUser.start();
    return true;
}

void CMainFrame::stopIdentify()
{
    m_objIdentifyUser.stop();
}


