///////////////////////////////////////////////////////////
//  CTestGuideManager.cpp
//  Implementation of the Class CTestGuideManager
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CTestGuideManager.h"
#include "CTestGuideBody.h"
#include "CTestGuideEyesight.h"
#include "CTestGuideGrip.h"
#include "CTestGuidePullup.h"
#include "CTestGuidePushup.h"
#include "CTestGuideRunMid.h"
#include "CTestGuideRunShort.h"
#include "CTestGuideSBJ.h"
#include "CTestGuideSFB.h"
#include "CTestGuideSitUp.h"
#include "CTestGuideSkip.h"
#include "CTestGuideVC.h"
#include "CTestGuidePVV.h"
#include "CTestGuideBasketball.h"
#include "CTestGuideFootball.h"
#include "CTestGuideVolleyball.h"
#include "CTestGuideSolidball.h"
#include "CTestGuidePUO.h"
#include "CTestGuideVerticalJump.h"
#include "CTestGuideBase.h"

#include "SteDevManager/SteDevice/SteSubElement/CSubElementRunMid.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "CDevDataCfg.h"
#include "CFilePath.h"
using namespace BindValue;
using namespace TestTableCfg;

map<short, QString> CTestGuideManager::s_mapImpedanceAndSetverValue(CTestGuideManager::initImpedanceAndSetverValue());
map<short, int> CTestGuideManager::s_mapImpedanceAndSetverValueNew(CTestGuideManager::initImpedanceAndSetverValueNew());

vector<CCompleteTestResult> CTestGuideManager::m_vecCompleteTestRes;
vector<CUserInfo> CTestGuideManager::m_vecDeleteTestRes;

//是否使用批量提交
static bool s_bUsebulkSubmit = true;

CTestGuideManager::CTestGuideManager() :
    m_pReqeuestMgr( nullptr )
{
    //提前注册所有指导类
    new CProductRegistrar<CTestGuideBase,CTestGuideBody>(DEV_TYPE_BODY);
    new CProductRegistrar<CTestGuideBase,CTestGuideVC>(DEV_TYPE_VC);
    new CProductRegistrar<CTestGuideBase,CTestGuideSBJ>(DEV_TYPE_SBJ);
    new CProductRegistrar<CTestGuideBase,CTestGuideSFB>(DEV_TYPE_SFB);
    new CProductRegistrar<CTestGuideBase,CTestGuidePullup>(DEV_TYPE_PULL_UP);
    new CProductRegistrar<CTestGuideBase,CTestGuideSitUp>(DEV_TYPE_SIT_UP);
    new CProductRegistrar<CTestGuideBase,CTestGuideRunShort>(DEV_TYPE_RUN_SHORT);
    new CProductRegistrar<CTestGuideBase,CTestGuideRunMid>(DEV_TYPE_RUN_MID);
    new CProductRegistrar<CTestGuideBase,CTestGuideSkip>(DEV_TYPE_ROPE_SKIP);
    new CProductRegistrar<CTestGuideBase,CTestGuideGrip>(DEV_TYPE_GRIP);
    new CProductRegistrar<CTestGuideBase,CTestGuidePushup>(DEV_TYPE_PUSH_UP);
    new CProductRegistrar<CTestGuideBase,CTestGuideEyesight>(DEV_TYPE_EYESIGHT);
    new CProductRegistrar<CTestGuideBase,CTestGuidePVV>(DEV_TYPE_PVV);
    new CProductRegistrar<CTestGuideBase,CTestGuideBasketball>(DEV_TYPE_BASKETBALL);
    new CProductRegistrar<CTestGuideBase,CTestGuideFootball>(DEV_TYPE_FOOTBALL);
    new CProductRegistrar<CTestGuideBase,CTestGuideVolleyball>(DEV_TYPE_VOLLEYBALL);
    new CProductRegistrar<CTestGuideBase,CTestGuideSolidball>(DEV_TYPE_SOLIDBALL);
    new CProductRegistrar<CTestGuideBase,CTestGuidePUO>(DEV_TYPE_PUO);
    new CProductRegistrar<CTestGuideBase,CTestGuideVerticalJump>(DEV_TYPE_VJUMP);

    initSig();

    connect(&m_timer, &QTimer::timeout, this, &CTestGuideManager::onTimeOut);

    //注册类型在信号传递时使用，中长跑测试指导中会使用对应的信号变量
    qRegisterMetaType<CLocation>( "CLocation" );
    qRegisterMetaType<QVector<CMultiUploadRetInfo>>( "QVector<CMultiUploadRetInfo>" );

    //设备状态变更通知当前函数
    connect(&m_objSteDevMgr.getTestStateMgr(), &CTestStateManage::signalTestStateChange, this, &CTestGuideManager::onDevTestStateChange);

    //界面消息
    connect(&m_ui, &CInterAction::sigCloseBindPopup, this, &CTestGuideManager::onCancelNFCBind);
    connect(&m_ui, &CInterAction::sigNfcValidTimeOut, this, &CTestGuideManager::onUiNFCValidTimeout);
    connect(&m_ui, &CInterAction::sigUnbindUser, this, &CTestGuideManager::onUnbindUser);
    connect(&m_ui, &CInterAction::sigTestItemChanged, this, &CTestGuideManager::onTestItemChanged, Qt::QueuedConnection);

    //初始化跳绳延时时间
    initRopeSkipDelayTime();

    startCheackTimeTimer();
}

CTestGuideManager::~CTestGuideManager()
{
    stopCheackTimeTimer();
    //清除所有指导类对象
    CProductFactory<CTestGuideBase>::Instance().Clear(true);
}

bool CTestGuideManager::connectSteDev(CDevHardwareBase *pSteDev)
{
    if(nullptr == pSteDev)
        return false;

    auto &nDevId = pSteDev->getMacID();

    if(checkDeviceAdded(nDevId))
        return true;

    auto &shDevType = pSteDev->getDevType();

    //中长跑设备只对终点设备进行创建对应测试指导类
    if(shDevType == DEV_TYPE_RUN_MID)
    {
        auto pRunMid = reinterpret_cast<CDevSteRunMid *>(pSteDev);
        if(pRunMid->getRunMidDevType() != CDevSteRunMid::RUN_MID_TYPE_DESTINATION)
            return true;
    }

    auto pTestGuidBase = getTestGuid(nDevId);

    if(nullptr != pTestGuidBase)
    {
        if(shDevType != pTestGuidBase->getDevType())
        {
            deleteSteDevice(pSteDev);
            pTestGuidBase = nullptr;
        }
    }

    //若不存在
    if(pTestGuidBase == nullptr)
        pTestGuidBase = CProductFactory<CTestGuideBase>::Instance().GetProduct(shDevType);

    if(nullptr == pTestGuidBase)
        return false;

    connectSignal(pSteDev, pTestGuidBase);

    m_mapTestGuide[nDevId] = pTestGuidBase;

    //设置设备
    pTestGuidBase->setSteDevice(pSteDev);

    pTestGuidBase->setActiveState(true);

    rebindDisconnUser(nDevId);
    return true;
}

void CTestGuideManager::disconnectSteDev(CDevHardwareBase *pSteDev)
{
    if(pSteDev == nullptr)
        return;

    auto &nDevID = pSteDev->getMacID();
    auto pTestGuidBase = getTestGuid(nDevID);

    //删除设备相关连接信号
    disconnectSignal(pSteDev, pTestGuidBase);

    if(pTestGuidBase != nullptr)
        pTestGuidBase->setActiveState(false);

    //deleteBindInfoByDevID(nDevID);
    //删除所有绑定信息

    //避免中长跑一直在循环中
    if(m_mapTestGuide.size() > 0 && (*m_mapTestGuide.begin()).second->getDevType() == DEV_TYPE_RUN_MID)
    {
        CTestGuideBase::setContinueTestFlag(false);
    }
}

void CTestGuideManager::deleteSteDevice(CDevHardwareBase *pSteDev)
{
    if(pSteDev == nullptr)
        return;

    auto &nDevID = pSteDev->getMacID();
    auto pTestGuidBase = getTestGuid(nDevID);

    disconnectSignal(pSteDev, pTestGuidBase);

    if(pTestGuidBase != nullptr)
        pTestGuidBase->setActiveState(false);

    //删除所有绑定信息
    deleteBindInfoByDevID(nDevID);

    auto iter = m_mapTestGuide.find(nDevID);

    //删除对应的测试指导
    if(iter != m_mapTestGuide.end())
    {
        safeDeleteClassPoint(&(iter->second));
        m_mapTestGuide.erase(iter);

        //删除指导类时通知外部
        qDebug()<<"delete testguide class";
    }
}

void CTestGuideManager::deleteSteDevice(const unsigned int &uiDevID)
{
    unsigned int nDevID = uiDevID;

    deleteBindInfoByDevID(nDevID);

    auto iter = m_mapTestGuide.find(nDevID);

    //删除对应的测试指导
    if(iter != m_mapTestGuide.end())
    {
        disconnect(iter->second, &CTestGuideBase::sigTestPrepare, this, &CTestGuideManager::onTestPrepare);
        safeDeleteClassPoint(&(iter->second));
        m_mapTestGuide.erase(iter);

        //删除指导类时通知外部
        qDebug()<<"delete testguide class";
    }
}

void CTestGuideManager::deleteAllDevice()
{
    for(auto &element : m_mapTestGuide)
    {
        deleteBindInfoByDevID(element.first);

        safeDeleteClassPoint(&(element.second));
    }

    m_mapTestGuide.clear();
}

bool CTestGuideManager::setTestMode(const short &shTestMode)
{
    if(shTestMode >= TEST_MODE_COUNT || CTestGuideBase::s_shTestMode == shTestMode)
        return false;

    CTestGuideBase::s_shTestMode  = shTestMode;
    //    for(auto &element : m_mapTestGuide)
    //    {
    //        element.second->setTestMode(shTestMode);
    //    }

    return true;
}

//绑定用户信息，通用处理
short CTestGuideManager::bindUserInfo(const CUserInfo &userInfo, int nDevType)
{
    /// 直连处理：提前清除所有绑定信息
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        deleteAllBindInfo();
    }

    /// 0.安全保护(设备是否存在，是否允许绑定，用户是否绑定，是否存在可用绑定)
    //0.1 设备不存在
    if(m_mapTestGuide.size() == 0 || m_objSteDevMgr.getAllDev().size() == 0)
    {
        return NOT_AVAILABLE;
    }

    //0.2 同步模式仅允许 测试未开始 时绑定
    bool bDirectConnect = CDevDataCfg::getDevDirectConnectFlag();
    short shTestMode = getActualTestMode(bDirectConnect, CTestGuideBase::s_shTestMode);
    if(shTestMode == TEST_SYNC)
    {
        if(m_bTestState)
        {
            return TEST_HAVE_BEGIN;
        }
        if(m_bStartingTest)
        {
            return STARTING_TEST;
        }
    }

    //0.3 用户已被绑定
    if(!userInfo.isTouristInfo() && checkUserBound(userInfo, nDevType))
    {
        return ALREADY_BIND;
    }

    //0.4 是否有可用设备(不同设备判断条件不同，单独重载内部函数)
    CLocation location;
    if(!findAvailableElement(location, nDevType))
    {
        return NOT_AVAILABLE_TEST;
    }

    /// 1.绑定用户信息(常规绑定，额外绑定)
    short shRet = NOT_AVAILABLE;
    switch (nDevType)
    {
    //中长跑(绑卡)、跳绳(绑绳)
    case DEV_TYPE_RUN_MID:
    case DEV_TYPE_ROPE_SKIP:
        shRet = bindUserInfoForExtra(userInfo, location, nDevType);
        break;
    default:
        shRet = bindUserInfoForCommon(userInfo, location);
        break;
    }

    /// 2.成功绑定
    if(shRet == SUCESS_BIND || shRet == SUCESS_BIND_EXTRA)
    {
        const CLocation *pLocation = nullptr;
        CUserInfo *pUserInfo = nullptr;
        bool bFindBindInfo = false;
        //2.1 绑定核查(此步不能删除，确保绑定成功，且获取最新用户信息)
        bFindBindInfo = getBindInfoByUserInfo(userInfo, &pLocation, &pUserInfo, nDevType);

        //2.2 核查成功
        if(bFindBindInfo)
        {
            setLastBindInfo(userInfo, pLocation);

            //删除断连人员信息
            deleteDisconnUserInfo(userInfo);

            //设置绑定用户ID
            m_qsUserID = QString::number(userInfo.m_llUniqueID);

            //设置设备类型
            pUserInfo->m_shDevType = m_objSteDevMgr.getType();

            //更新UI
            bindUserToUi(*pUserInfo, *pLocation, CTestGuideBase::s_shTestMode);

            //日志信息：因出现空白行，每绑定一个人，就打出此人信息
            LOGINFO("## Bind User Info: " + pUserInfo->m_strName.toStdString());

            //保存用户识别顺序，在同步模式会使用
            CRecognitionTableInfo info;
            info.m_pLocation = pLocation;
            info.m_pUserInfo = pUserInfo;
            m_vecRecognitionSequence.push_back(info);

            //异步或直连的游客直接开始测试
            if(shTestMode == TEST_ASYNC || (bDirectConnect && info.m_pUserInfo->isTouristInfo()))
            {
                bool bFlag = m_vecCompleteTestRes.size() > 0;
                m_ui.setStuTestStatus(bFlag, false);
                startTest(*pLocation);
                shRet = SUCESS_BIND;
            }
        }
        else
        {
            shRet = BANDING_FALSE;
        }
    }

    updateBindUserNum();

    return shRet;
}

void CTestGuideManager::unbindUserInfo(const CUserInfo &userInfo, int nTestItem)
{
    deleteBindInfoByUser(userInfo, nTestItem);
}

//这里仅仅是触发开始，真正的处理逻辑在对应的测试指导中，需要保证开启必须成功
bool CTestGuideManager::startTest()
{
    //删除可能存在签字文件
    CFilePath::deleteFileOrFolder( m_ui.getSignatureFilePath(true).toStdString() );

    m_bStartingTest = true;

    //设置提交按钮不可用
    m_ui.setBtnEnableCommit(false);

    //同步模式一对多需要重新填写表格名称
    short shTestMode = getActualTestMode(CDevDataCfg::getDevDirectConnectFlag(), CTestGuideBase::s_shTestMode);

    if(shTestMode == TEST_SYNC)
    {
        m_ui.clearModelData(CInterAction::COMMON_DATA_MODEL);
        CDevDataCfg::delayMSec(200);

        for(auto &element : m_vecRecognitionSequence)
        {
            bindUserToUi(*element.m_pUserInfo, *element.m_pLocation, TEST_ASYNC);
        }
    }

    startDevice();

    m_bStartingTest = false;
    return m_bTestState;
}

void CTestGuideManager::startDevice()
{
    short shDevType = DEV_TYPE_UNKNOW;
    shDevType = m_objSteDevMgr.getType();

    set<unsigned int> setDevID;
    for(auto &element : m_mapUserInfo)
    {
        setDevID.insert(element.first.m_iSteDevMacID);
    }

    ///单个设备和多各设备进行相同的开始处理
    bool bCast = setDevID.size() > 1; //变量表示是否广播发送所有设备开始
    //设置统一开始时间，内部在设置测试单元信息时会根据这个时间来进行设置
    CTestGuideBase::setStartTestTime();

    //设置初始标志为true
    CTestGuideBase::setDevReadyFlag(true);

    //设置继续开始为true
    CTestGuideBase::setContinueTestFlag(true);

    //设置跳绳广播开始为true
    CTestGuideSkip::setFirstStartFlag(true);

    //短跑设备的特殊处理,后续转移为通用逻辑
    CTestGuideRunShort::setStartGunExistFlag(m_bStartingGunExist);
    if(shDevType == DEV_TYPE_RUN_SHORT && m_bStartingGunExist)
    {
        m_bStartTestReady = true;
        return;
    }

    //初始化对应的测试单元过程
    bool bExcuteStart = false;

    for(auto &element : setDevID)
    {
        bExcuteStart = (&element == &*setDevID.rbegin());
        m_bTestState &= m_mapTestGuide[element]->startTest(bExcuteStart, bCast);

        if(!CTestGuideBase::getContinueTestFlag())
        {
            break;
        }
    }

    if(!CTestGuideBase::getContinueTestFlag())
    {
        for(auto &element : setDevID)
        {
            bExcuteStart = element == *setDevID.rbegin();
            m_bTestState &= m_mapTestGuide[element]->stopTest(bExcuteStart, bCast);
        }
        return;
    }

    //检测命令是否发送成功
    m_bCheckCastStart = bCast;
    if(m_bCheckCastStart)
    {
        m_llCheckCastStartTime = QDateTime::currentMSecsSinceEpoch();
    }
}

bool CTestGuideManager::stopTest()
{
    set<unsigned int> setDevID;
    for(auto &element : m_mapUserInfo)
    {
        setDevID.insert(element.first.m_iSteDevMacID);
    }

    ///单个设备和多各设备进行相同的开始处理
    bool bCast = setDevID.size() > 1; //变量表示是否广播发送所有设备开始

    //无论是单个设备开启还是多个设备开启,最后一个设备才执行开始
    bool bExcuteStart = false;
    for(auto &element : setDevID)
    {
        bExcuteStart = element == *setDevID.rbegin();

        //最后一个设备才进行统一的开始指令
        m_bTestState &= m_mapTestGuide[element]->stopTest(bExcuteStart, bCast);
    }
    return false;
}

//按钮响应消息：目前只有 短跑、中长跑、跳绳 同步模式才会调用
void CTestGuideManager::reStartTest()
{
    //新增测试项目不同逻辑处理
    short shDevType = DEV_TYPE_UNKNOW;
    shDevType = m_objSteDevMgr.getType();

    //无起点模式的短跑存在准备时间，在此期间禁止更新结果
    if(shDevType == DEV_TYPE_RUN_SHORT)
    {
        CTestGuideRunShort *pTestGuideRunShort = nullptr;
        auto itFind = m_mapTestGuide.find(m_mapUserInfo.begin()->first.m_iSteDevMacID);
        if(itFind != m_mapTestGuide.end())
        {
            pTestGuideRunShort = reinterpret_cast<CTestGuideRunShort *>(itFind->second);
            if(nullptr != pTestGuideRunShort)
            {
                pTestGuideRunShort->reStartByNoPoint();
            }
        }
    }

    //0.清空表格内容
    m_ui.clearModelData(CInterAction::COMMON_DATA_MODEL);

    //1.重新加载表格内容
    for(auto &element : m_vecRecognitionSequence)
    {
        bindUserToUi(*element.m_pUserInfo, *element.m_pLocation, TEST_ASYNC);
    }

    for(auto &element : m_mapUserInfo)
    {
        element.second.m_bCheckTime = false;
    }

    //3.重新测试证明不需要前面的结果，因此需要从数据库中删除
    for(auto &element : m_vecCompleteTestRes)
    {
        deleteDatabaseTestResult(element.m_vecTestResInfo);
    }

    //针对中长跑的处理,将所有卡号重新绑定
    if(m_mapUserInfo.size() > 0 && shDevType == DEV_TYPE_RUN_MID)
    {
        CTestGuideBase *pTestGuid = getTestGuid((*m_mapUserInfo.begin()).first.m_iSteDevMacID);
        if(pTestGuid != nullptr)
        {
            for(auto &element : m_mapUserInfo)
            {
                while(!pTestGuid->bindUserToElement(element.first.m_shElementIndex, element.second))
                {
                    if(!CTestGuideBase::getContinueTestFlag())
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            LOGERROR("pTestGuid == nullptr")
        }

    }

    //4.清空测试结果 及 显示
    m_vecCompleteTestRes.clear();
    m_ui.setTestState("");
    m_ui.setTestTipText("");

    //5.重新调用开始命令
    startDevice();

    //6.目前仅用于短跑、跳绳，跳绳会在删除命令时启用按钮
    if(shDevType == DEV_TYPE_RUN_SHORT || shDevType == DEV_TYPE_RUN_MID || shDevType == DEV_TYPE_ROPE_SKIP)
    {
        m_ui.setBtnEnable(CProjectEnums::OTM_RESTART_TEST_BTN, true);
    }

    //设置提交按钮可用
    m_ui.setBtnEnableCommit(m_vecCompleteTestRes.size() > 0);
}

void CTestGuideManager::cancelTest(const bool &bVoluntaryCancel/* = false */, const bool &bModeChangeFlag/* = false*/, const bool bSpeak)
{
    m_bInCancelTestState = true;

    m_bCheckCastStart = false;

    //开始命令期间会持续一段时间，将继续标志设置为false
    CTestGuideBase::setContinueTestFlag(false);

    //停止所有语音
    m_objSpeak.stop();

    //使用中间变量进行单个测试单元的取消测试，避免删除单个测试单元时遍历出错的问题
    set<CLocation> setBindLocation;
    for(auto &element : m_mapUserInfo)
    {
        setBindLocation.insert(element.first);
    }

    //删除单个绑定信息
    for(auto &location : setBindLocation)
    {
        cancelTest(location, bVoluntaryCancel);
    }

    //清空表格显示
    m_ui.clearModelData(CInterAction::COMMON_DATA_MODEL);

    short shActualTestMode = getActualTestMode(CDevDataCfg::getDevDirectConnectFlag(), CTestGuideBase::s_shTestMode);

    //异步模式取消测试后，去掉弹窗，同时判断提交是否可用
    if(shActualTestMode == TEST_ASYNC)
    {
        m_ui.setStuTestStatus(true, true);

        bool bFlag = m_vecCompleteTestRes.size() > 0;

        m_ui.setBtnEnable(CProjectEnums::OTM_ASYNC_COMMIT_BTN, bFlag);
    }

    //切换模式后，需要清空所有表格信息
    if(bModeChangeFlag)
    {
        m_ui.cleanDataModel();
    }

    //播放语音
    if(shActualTestMode == TEST_SYNC && bSpeak && !bModeChangeFlag)
    {
        m_objSpeak.textToSpeech("取消测试");
    }

    m_bInCancelTestState = false;

    //更新绑定信息
    updateBindUserNum();

    if(shActualTestMode == TEST_ASYNC && !bModeChangeFlag)
    {
        //弹出成绩窗口
        dispTestResult();
    }
    else
    {
        //清空保存的测试数据
        m_vecCompleteTestRes.clear();
    }
}

void CTestGuideManager::procSignature(const QString &qsSignalPath)
{
    if(m_vecCompleteTestRes.size() == 0)
        return;

    bool bStation = !CDevDataCfg::getDevDirectConnectFlag();
    short shActualTestMode = getActualTestMode(!bStation, CTestGuideBase::s_shTestMode);
    short shTestNature = CTestResultInfo::TEST_INFORMAL;

    //测试资源路径,同步模式创建 同步提交的签字保存在一个地方 测试时间 + '-' + 项目
    string strRootPath;
    if(shActualTestMode == TEST_SYNC)
    {
        //找到第一个存在测试项目的
        for(auto &completeTestRes : m_vecCompleteTestRes)
        {
            auto &vecTestRes = completeTestRes.m_vecTestResInfo;
            if(vecTestRes.size() > 0)
            {
                strRootPath = CTestGuideBase::getResPath() + vecTestRes[0].m_strStartTime \
                        + '-' + CDevDataCfg::getDevDescribe(CDevDataCfg::getDevTypeByTestItem(static_cast<short>(vecTestRes[0].m_nItem))) + '/';
                break;
            }
        }
    }

    for(auto &completeTestRes : m_vecCompleteTestRes)
    {
        CUserInfo &userInfo = completeTestRes.m_userInfo;
        auto &vecTestRes = completeTestRes.m_vecTestResInfo;

        if(vecTestRes.size() <= 0)
            continue;

        ///根据当前测试模式重新对路径进行赋值
        auto &testRes = vecTestRes[0];

        //将资源根目录设置到测试结果中
        switch (shActualTestMode) {
        //异步模式
        case TEST_ASYNC:
            testRes.setResourceRootPath(to_string(testRes.m_llUniqueID), strRootPath, testRes.m_strStartTime, \
                                        testRes.m_sDeviceID, CDevDataCfg::getDevDescribe(CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testRes.m_nItem))));
            break;
            //同步模式
        case TEST_SYNC:
            testRes.m_strPathResourceRoot = strRootPath;
            break;
        default:
            //直连模式
            if(!bStation)
            {
                testRes.setResourceRootPath(to_string(testRes.m_llUniqueID), strRootPath, testRes.m_strStartTime, \
                                            testRes.m_sDeviceID, CDevDataCfg::getDevDescribe(CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testRes.m_nItem))));
            }
            break;
        }

        //保存签字
        bool bSaveSignal = false;

        //自动提交都是正式测试
        if(CDatabaseManage::GetInstance().getPCommitTableConfig()->isAutoCommit())
        {
            shTestNature = CTestResultInfo::TEST_FORMAL;
        }
        //存在签字 是 正式测试，然后判断是游客就为自由测试
        else
        {
            bool bHaveSignal = !qsSignalPath.isEmpty() && CFilePath::isFileExist(qsSignalPath.toStdString());
            shTestNature = bHaveSignal ? CTestResultInfo::TEST_FORMAL : CTestResultInfo::TEST_INFORMAL;

            if(userInfo.isTouristInfo())
            {
                shTestNature = CTestResultInfo::TEST_FREE;
            }

            //正式模式同时存在签字路径
            bSaveSignal = shTestNature == CTestResultInfo::TEST_FORMAL && !testRes.m_strPathResourceRoot.empty();
        }

        //将资源放到指定的目录
        if(bSaveSignal)
        {
            CFilePath::isDirExist(testRes.m_strPathResourceRoot, true);
            CFilePath::cpyFile(qsSignalPath.toStdString(), testRes.m_strPathResourceRoot + "mark.png");
        }

        //对每一个结果设置测试性质
        for(auto &testRes : vecTestRes)
        {

            int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();

            //不需要判断得分规则,邛崃南街小学
            if(nSoftVersion == soft::VERSION_QLNJXX)
            {
                testRes.m_nNature = shTestNature;
            }
            else
            {
                //没有得分标准都是非正式测试:练习成绩
                QString curScore = CTestGuideBase::getTestScore(true, testRes, userInfo);
                if( curScore == "没有得分指标")
                {
                    testRes.m_nNature = CTestResultInfo::TEST_INFORMAL;
                }
                else
                {
                    testRes.m_nNature = shTestNature;
                }
            }

            testRes.m_bQueryWithNature = false;

            //设置签名路径
            if(bSaveSignal)
            {
                testRes.m_strResSignature = testRes.m_strPathResourceRoot + "mark.png";
            }

            //更新数据
            CDatabaseManage::GetInstance().getTabTestItemResult()->updateResultInfo(testRes);
        }
    }
}

//单个用户取消
void CTestGuideManager::cancelTest(const CLocation& location, const bool &bVoluntaryCancel)
{
    //0.安全保护(保证存在测试指导)
    CTestGuideBase* pTestGuide = getTestGuid(location.m_iSteDevMacID);
    if(nullptr == pTestGuide)
    {
        LOGERROR("pTestGuide is nullptr !");
        return ;
    }

    bool bSaveCancelTestState = m_bInCancelTestState;
    m_bInCancelTestState = true;

    //1.组网/自愿 直接结束测试 || 直连强制退出需要保存有效结果
    bool bStation = !CDevDataCfg::getDevDirectConnectFlag();

    if(bStation || bVoluntaryCancel)
    {
        pTestGuide->stopTest(location.m_shElementIndex);
        pTestGuide->clearTestRes(location.m_shElementIndex);
    }
    else
    {
        pTestGuide->forceExitTest();
    }

    /* 注释：直接在 cancelTest 中清空所有表格
    //2.1 清空表格对应信息
    deleteTableByLocation(CInterAction::COMMON_DATA_MODEL,location);
    if(s_shTestMode != TEST_ASYNC)
    {
        deleteTableByLocation(CInterAction::STUDENT_DATA_MODEL,location);
        deleteTableByLocation(CInterAction::SCORE_DATA_MODEL,location);
    }
    */

    //2.组网模式 其他处理
    if(bStation)
    {
        /// 注：删除顺序不允许修改，涉及到指针变量无效问题
        if(CTestGuideBase::s_shTestMode == TEST_SYNC)
        {
            //2.3 删除对应测试结果(包括数据库)
            deleteResultByLocation(location,true);
        }

        //2.4 解除用户绑定信息
        deleteBindInfoByLocation(location);
    }
    //3.直连模式 其他处理
    else
    {
        bool bBackHomePage = CDevDataCfg::isHomePage(m_ui.getCurPage());

        //定时器超时，提交结果信息(后台提交)
        if(!bVoluntaryCancel)
        {
            vector<vector<CTestResultInfo>> vecTemp;
            for(auto &element : m_vecCompleteTestRes)
            {
                vecTemp.push_back(element.m_vecTestResInfo);
            }

            uploadTestResult(vecTemp, m_pReqeuestMgr, DataBase);
        }
        //自主退出，删除数据库中对应的数据
        else
        {
            deleteResultByLocation(location,true);
        }

        //包含显示的数据值和测试状态显示信息
        m_ui.setTestState(""); //设置测试状态为空
        m_ui.setCurTestRet(DEFAULT_DISP_VALUE); //设置数据为空

        //返回首页 需要删除绑定信息
        if(bBackHomePage)
        {
            deleteBindInfoByLocation(location);
        }
    }

    //恢复状态
    m_bInCancelTestState = bSaveCancelTestState;
}

//成绩表格中点击取消成绩,目前取消成绩仅有同步模式可以，异步模式是取消测试
//同时这里只是暂时删除表格内容，在下一次点开之后重新往里面写入
void CTestGuideManager::deleteScore(QString qsNumber)
{
//    short shRealTestModel = getActualTestMode(CDevDataCfg::getDevDirectConnectFlag(), CTestGuideBase::s_shTestMode);
//    if(shRealTestModel != TEST_SYNC)
//        return;

    //从表格获取准考证号（目前是唯一的，同步仅有一个人的成绩)
    int iCol = CDevDataCfg::getColTypeIndex(TestTableCfg::COL_USER_ID, m_objSteDevMgr.getType(), CTestGuideBase::s_shTestMode, true);

    if(iCol < 0)
    {
        LOGERROR("iCol < 0 when qsNumber = " + qsNumber.toStdString());
        return;
    }

    //这里的row - 1是因为表格中的起始点不包含展示出来的首行
    CUserInfo userInfo;
    userInfo.m_strStudentId = m_ui.getModelCellData(CInterAction::SCORE_DATA_MODEL, qsNumber.toInt() - 1, iCol);

    m_vecDeleteTestRes.push_back(userInfo);
//    //根据序号删除已完成的测试结果（临时结果和已经完成的结果放在了临时容器当中）
//    size_t szResNum = m_vecTempTestRes.size();
//    size_t szCompleteNum = m_vecCompleteTestRes.size();

//    auto iIndex = qsNumber.toUInt() - 1;

//    if(iIndex >= szResNum)
//        return;

//    //临时结果数量
//    size_t szTempResNum = szResNum - szCompleteNum;

//    //暂不考虑按下取消之后瞬间成绩变动问题，就是索引是相同的

//    //结果为已经完成的测试成绩
//    if(iIndex >= szTempResNum)
//    {
//        unsigned int iCompleteIndex = static_cast<unsigned int>(iIndex - szTempResNum);

//        if(iCompleteIndex < szCompleteNum)
//        {
//            //删除本地数据库成绩
//            deleteDatabaseTestResult(m_vecCompleteTestRes[iIndex].m_vecTestResInfo);

//            //删除测试内容
//            m_vecCompleteTestRes.erase(m_vecCompleteTestRes.begin() + iCompleteIndex);
//        }
//    }
//    //还未完成的成绩，将其中已经完成的成绩清空
//    else
//    {
//        auto &completeRes = m_vecTempTestRes[iIndex];
//        set<CLocation> setLocation = getBindUserInfo(completeRes.m_userInfo);
//        if(setLocation.size() > 0)
//        {
//            CLocation location(*(setLocation.begin()));
//            auto pTestGuid = getTestGuid(location.m_iSteDevMacID);
//            if(pTestGuid != nullptr)
//                pTestGuid->clearTestRes(location.m_shElementIndex);
//        }
//    }

//    if(m_vecCompleteTestRes.size() > iIndex)
//    {
//        //删除本地数据库成绩
//        deleteDatabaseTestResult(m_vecCompleteTestRes[iIndex].m_vecTestResInfo);

//        //删除测试内容
//        m_vecCompleteTestRes.erase(m_vecCompleteTestRes.begin() + iIndex);
//    }

    //若不存在测试结果，则直接关闭窗口，禁用提交按钮
    if(m_vecTempTestRes.size() == m_vecDeleteTestRes.size())
    {
        m_ui.closeConfirmPopup();
    }
    //若还存在结果，则主动刷新成绩表格
    else
    {
        dispTestResult();
    }
}

//设备变更时会调用清除所有
void CTestGuideManager::clearAll()
{
    //清除识别缓存队列
    m_vecRecognitionSequence.clear();

    //清除完成的测试结果，暂不删除数据库中内容
    m_vecCompleteTestRes.clear();

    //将当前所有设备设置为断开,同时删除绑定信息
    CTestGuideBase *pTestGuid;
    CDevHardwareBase *pDev;
    for(auto &element : m_mapTestGuide)
    {
        pTestGuid = element.second;
        if(pTestGuid == nullptr)
            continue;

        pDev = pTestGuid->getSteDev();
        if(pDev == nullptr)
            continue;

        disconnectSteDev(pDev);
    }

    //删除所有绑定信息
    deleteAllBindInfo();
}

//查找测试人员是否已经被绑定，若测试项确定指定查找对应的项，若不确定只需要找到对应的值即可
bool CTestGuideManager::checkUserBound(const CUserInfo &userinfo, int nDevType)
{
    const CLocation *pLocation;
    CUserInfo *pUserInfo;

    return getBindInfoByUserInfo(userinfo, &pLocation, &pUserInfo, nDevType);
}

set<CLocation> CTestGuideManager::getBindUserInfo(const CUserInfo &userinfo, int nDevType)
{
    bool bMatchingType = nDevType != DEV_TYPE_UNKNOW;
    CTestGuideBase *pTestGuide;
    set<CLocation> setLoction;

    for(auto &element : m_mapUserInfo)
    {
        if(element.second.m_llUniqueID != userinfo.m_llUniqueID)
        {
            continue;
        }

        //绑定之前判断设备在不在
        pTestGuide = getTestGuid(element.first.m_iSteDevMacID);
        if(pTestGuide == nullptr)
        {
            continue;
        }

        if(bMatchingType)
        {
            if(pTestGuide->getDevType() != nDevType)
            {
                continue;
            }
        }

        setLoction.insert(element.first);
    }

    return setLoction;
}

bool CTestGuideManager::getBindInfoByLocation(const CLocation &findCondition, const CLocation **pLocation, CUserInfo **pUserInfo, int nDevType)
{
    bool bMatchingType = nDevType != DEV_TYPE_UNKNOW;

    auto itFindUser = m_mapUserInfo.find(findCondition);
    if(itFindUser == m_mapUserInfo.end())
        return false;

    auto itFindTestGuid = m_mapTestGuide.find(findCondition.m_iSteDevMacID);
    if(itFindTestGuid == m_mapTestGuide.end())
        return false;

    auto &pTestGuid = itFindTestGuid->second;
    if(bMatchingType && pTestGuid != nullptr)
    {
        if(pTestGuid->getDevType() != nDevType)
            return false;
    }

    *pLocation = &(itFindUser->first);
    *pUserInfo = &(itFindUser->second);

    return true;
}

bool CTestGuideManager::getBindInfoByUserInfo(const CUserInfo &findCondition, const CLocation **pLocation, CUserInfo **pUserInfo, int nDevType)
{
    bool bMatchingType = nDevType != DEV_TYPE_UNKNOW;
    CTestGuideBase *pTestGuide;
    set<CLocation> setLoction;

    for(auto &element : m_mapUserInfo)
    {
        if(element.second.m_llUniqueID != findCondition.m_llUniqueID)
            continue;

        //绑定之前判断设备在不在
        pTestGuide = getTestGuid(element.first.m_iSteDevMacID);
        if(pTestGuide == nullptr)
            continue;

        if(bMatchingType)
        {
            if(pTestGuide->getDevType() != nDevType)
                continue;
        }

        //若人员绑定的设备是无效，表示对应的设备断连了，也算作没有绑定
        if(!pTestGuide->getActiveState())
            continue;

        *pLocation = &(element.first);
        *pUserInfo = &(element.second);

        if(nullptr == pLocation || nullptr == pUserInfo)
        {
            return false;
        }

        return true;
    }

    return false;
}

bool CTestGuideManager::findAvailableElement(CLocation &location, int nDevType)
{
    short shDevType = static_cast<short>(nDevType);
    CSubElementBase* pSubElement = nullptr;
    CTestGuideBase *pTestGuide;
    CDevHardwareBase *pSteDev, *pDevMinExtentionNo(nullptr), *pDevMinUseNum(nullptr);
    bool bMatchingType = shDevType != DEV_TYPE_UNKNOW;
    bool bSpeacialDev = CDevDataCfg::isSpecialDev(shDevType);
    bool bDirectConnect = CDevDataCfg::getDevDirectConnectFlag();

    set<int> setDevUseNum;

    bool bFindValid = false;
    int nTempExtensionNo = 0, nTempUseNum = 0, nMaxUseNum = 0;
    short shCurDevType;

    for (auto &iter : m_mapTestGuide)
    {
        pTestGuide = iter.second;
        if(pTestGuide == nullptr)
            continue;

        //对应的测试指导断开
        if(!pTestGuide->getActiveState())
        {
            continue;
        }

        shCurDevType = pTestGuide->getDevType();

        if(bMatchingType)
        {
            if(shCurDevType != shDevType)
                continue;
        }

        pSteDev = pTestGuide->getSteDev();
        if(pSteDev == nullptr || !pSteDev->isValid())
            continue;

        //立定跳远设备没有校准不允许使用
        if(shCurDevType == DEV_TYPE_SBJ)
        {
            if(!reinterpret_cast<CDevSteSBJ *>(pSteDev)->getCalibrationSPosFlag())
                continue;
        }

        pSubElement = pSteDev->getFirstAvailableElement();
        if(pSubElement == nullptr)
            continue;

        //特殊设备和直连设备找到第一个可用设备即返回
        if(bSpeacialDev || bDirectConnect || !bMatchingType)
        {
            location.m_shElementIndex = pSubElement->getIndex();
            location.m_iSteDevMacID = iter.first;
            bFindValid = true;
            break;
        }
        else
        {
            nTempExtensionNo = pSteDev->getExtensionNo();
            nTempUseNum = pSteDev->getUseNum();

            if(CDevDataCfg::isValidExtensionNum( nTempExtensionNo ))
            {
                setDevUseNum.insert(nTempUseNum);

                if(pDevMinExtentionNo == nullptr){
                    pDevMinExtentionNo = pSteDev;
                }
                else {
                    if(nTempExtensionNo < pDevMinExtentionNo->getExtensionNo())
                        pDevMinExtentionNo = pSteDev;
                }

                if(pDevMinUseNum == nullptr){
                    pDevMinUseNum = pSteDev;
                }
                //使用次数相同优先选分机标号小的设备
                else {
                    bool bSet = nTempUseNum < pDevMinUseNum->getUseNum() ||
                            (nTempUseNum == pDevMinUseNum->getUseNum() && nTempExtensionNo < pDevMinUseNum->getExtensionNo());
                    if(bSet)
                        pDevMinUseNum = pSteDev;
                }

                if(nTempUseNum > nMaxUseNum)
                    nMaxUseNum = nTempUseNum;
            }
        }
    }

    //非特殊设备在组网情况下存在分机编号，需要从小到大进行绑定,添加循环绑定逻辑（比如 1、2号设备当前使用次数为 3，新连接
    //的3号设备此时使用次数为0，在连接时优先使用3号设备）
    if(!(pDevMinUseNum == nullptr || pDevMinExtentionNo == nullptr))
    {
        int nSetUseNum = 0;
        //所有设备使用次数都相同，选择最小的分机编号设备，同时设备使用次数+1（同时需要将当前的使用次数设置到所有已绑定设备）
        if(setDevUseNum.size() == 1)
        {
            pSteDev = pDevMinExtentionNo;
            nSetUseNum = pSteDev->getUseNum() + 1;
        }
        //选择使用次数最少的设备，将使用次数最少的设置为当前最大的
        else
        {
            pSteDev = pDevMinUseNum;
            nSetUseNum = nMaxUseNum;
        }

        pSteDev->setUseNum(nSetUseNum);

        //设置全部已绑定设备的使用次数
        if(nSetUseNum != nMaxUseNum){
            CDevHardwareBase *pTemp;
            for(auto &element : m_mapUserInfo)
            {
                pTemp = m_objSteDevMgr.getDevSteByID(element.first.m_iSteDevMacID);
                if(pTemp != nullptr)
                {
                    pTemp->setUseNum(nSetUseNum);
                    qDebug() << pTemp->getExtensionNo() << "set num" << nSetUseNum;
                }
            }
        }

        pSubElement = pSteDev->getFirstAvailableElement();
        if(pSubElement == nullptr)
            return false;

        location.m_shElementIndex = pSubElement->getIndex();
        location.m_iSteDevMacID = pSteDev->getMacID();
        bFindValid = true;
    }

    return bFindValid;
}

bool CTestGuideManager::bindUserToElement(const CUserInfo &userInfo, const CLocation &location, unsigned int nExtraInfoID/* = 0*/)
{
    bool bRet = false;

    m_mapUserInfo[location] = userInfo;
    m_mapUserInfo[location].m_nExtraInfoID = nExtraInfoID; //这里的id 中长跑中表示手环id 跳绳表示跳绳id 其它设备表示分机编号

    CTestGuideBase *pTestGuid = getTestGuid(location.m_iSteDevMacID);
    if(pTestGuid != nullptr)
    {
        //设置测试信息
        bRet = pTestGuid->bindUserToElement(location.m_shElementIndex, m_mapUserInfo[location]);
        if(!bRet)
        {
            pTestGuid = getTestGuid(location.m_iSteDevMacID);
            if(pTestGuid != nullptr)
            {
                pTestGuid->unbindUserToElement(location.m_shElementIndex);
            }

            auto itFind = m_mapUserInfo.find(location);
            if(itFind != m_mapUserInfo.end())
            {
                m_mapUserInfo.erase(itFind);
            }
        }
        else
        {

        }
    }

    return bRet;
}

bool CTestGuideManager::bindUserToUi(const CUserInfo &userInfo, const CLocation &location, short shTestMode)
{
    //直连模式
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        m_ui.onFaceRecogSuccess(userInfo);
        return true;
    }

    //设备断连，用户还能绑定另外一个设备，表格中可能存在断连的设备绑定信息，在进行新的绑定时判断有没有对应的设备进行更新
    short shTableContentDevIDIndex = CDevDataCfg::getColTypeIndex(COL_DEV_ID, m_objSteDevMgr.getType(), shTestMode);
    QString qsDevID = CDevDataCfg::getContentByColDataType(COL_DEV_ID, location, userInfo);

    //异步模式在表格中会存在对应人员的历史成绩，这里需要做一个操作，仅检查在表格中存在的绑定信息行，同步是检测全部，异步需要根据当前成绩进行检查
    bool bUpdate = false;
    if(shTableContentDevIDIndex >= 0)
    {
        int nLimitRowIndex = -1;
        if(shTestMode == TEST_ASYNC)
        {
            nLimitRowIndex = m_ui.getModelDataRows(CInterAction::COMMON_DATA_MODEL) - (static_cast<int>(m_vecCompleteTestRes.size())) - 1;
        }

        if(nLimitRowIndex >= 0)
            bUpdate = m_ui.checkModelDataExist(CInterAction::COMMON_DATA_MODEL, shTableContentDevIDIndex, qsDevID, nLimitRowIndex);
    }

    QStringList listData = CDevDataCfg::getTableContent(location, shTestMode, userInfo);

    qDebug()<<listData;

    if(bUpdate)
    {
        m_ui.updateModelRowData(CInterAction::COMMON_DATA_MODEL, shTableContentDevIDIndex, qsDevID, listData);
    }
    else
    {
        //添加新的人员时，由于异步模式需要独特处理，将最新绑定的人员放到列表头即可
        bool bAddToHead = CTestGuideBase::s_shTestMode == TEST_ASYNC;

        //最后一个参数为false表示添加到头
        m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, listData, !bAddToHead);
    }

    return true;
}

CTestGuideBase *CTestGuideManager::getTestGuid(const unsigned int &nDevId)
{
    CTestGuideBase *pTarget = nullptr;
    auto itFind = m_mapTestGuide.find(nDevId);

    if(itFind != m_mapTestGuide.end())
    {
        pTarget = itFind->second;
    }

    return pTarget;
}

bool CTestGuideManager::startTest(const CLocation &location)
{
    //根据设备ID获取对应的测试指导类
    CTestGuideBase *pGuide = getTestGuid(location.m_iSteDevMacID);

    if(nullptr == pGuide)
    {
        LOGERROR("Test Guide Class Invalid");
        return false;
    }

    //设置开始时间
    CTestGuideBase::setStartTestTime();

    return pGuide->startTest(location.m_shElementIndex, true, true);
}

short CTestGuideManager::bindUserInfoForCommon(const CUserInfo &userInfo, const CLocation &location)
{
    bool bDirectConnect = CDevDataCfg::getDevDirectConnectFlag();
    short shRet = SUCESS_BIND;
    if( bDirectConnect )
    {
        m_ui.onFaceRecogSuccess( userInfo );
    }

    //组网检查设备是否就绪
    int iExtentionNo = -1;
    if(!bDirectConnect && !CDevDataCfg::isSpecialDev(m_objSteDevMgr.getType()))
    {
        auto pDev = m_objSteDevMgr.getDevSteByID(location.m_iSteDevMacID);
        if(pDev != nullptr)
        {
            iExtentionNo = pDev->getExtensionNo();
            if(iExtentionNo <= 0)
                return DEV_NOT_READY;
        }
    }

    if(m_objSteDevMgr.getType() == DEV_TYPE_RUN_SHORT)
        iExtentionNo = location.m_shElementIndex + 1;

    //同步模式和异步模式相同，都是绑定相应的模块，
    //不同点在于异步开启之后会命令对应的底层开始进行测试
    if(!bindUserToElement(userInfo, location, static_cast<unsigned int>(iExtentionNo)))
    {
        return BANDING_FALSE;
    }

    return shRet;
}

//需要额外绑定信息的设备会进入此函数
//跳绳：绑定绳子设备 中长跑：绑卡
short CTestGuideManager::bindUserInfoForExtra(const CUserInfo &userInfo, const CLocation &location, int nDevType)
{
    //0.重置绑定信息
    resetExtraBindInfo();

    //1.语音提示额外信息绑定，目前统一
    QString qsTip(userInfo.m_strName);
    qsTip += nDevType == DEV_TYPE_RUN_MID ? "同学，请按图示进行腕带标签配对" : "同学，请按图示进行跳绳配对";
    m_objSpeak.textToSpeech(qsTip);

    //2.开启绑定倒计时，目前为 10s
    bool bFirstCheck = true;
    QDateTime ProtectTime = QDateTime::currentDateTime().addMSecs(2000);
    QDateTime datetime = QDateTime::currentDateTime().addMSecs(m_nExtraCountdownMsec);
    m_ui.setNFCVaildTime(m_nExtraCountdownMsec);

    while(!(QDateTime::currentDateTime() > datetime || m_bExtraUnBindFlag))
    {
        m_nExtraInfoID = simulationRunMidCard();//模拟刷卡

        if(m_bExtraBindFlag)
        {
            //已经被绑定，修改标志判断此卡号无效，不退出循环，允许继续绑定
            if(checkRunCardBindState(m_nExtraInfoID))
            {
                m_bExtraBindFlag = false;

                if(nDevType == DEV_TYPE_RUN_MID)
                {
                    //两个标志主要目的是保证语音的完整性
                    if(!m_objSpeak.isInSpeaking() || bFirstCheck)
                    {
                        bFirstCheck = false;
                        m_objSpeak.textToSpeech("已经被绑定");
                    }
                }
                // 跳绳设备处理：延缓 2s 进行语音播报，不影响绑定逻辑
                else if(nDevType == DEV_TYPE_ROPE_SKIP)
                {
                    if(QDateTime::currentDateTime() > ProtectTime && (!m_objSpeak.isInSpeaking() || bFirstCheck))
                    {
                        bFirstCheck = false;
                        m_objSpeak.textToSpeech("已经被绑定");
                    }
                }
            }
            //若绑定成功，直接退出循环
            else
            {
                break;
            }
        }

        //处理卡号更新消息
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    //若卡号有效，中长跑设备需要在规定时间内一直进行绑定,绑定的时候再判断一次卡号是否可以绑定
    bool bBindSucess = false;
    if(nDevType == DEV_TYPE_RUN_MID && m_bExtraBindFlag)
    {
        while(!(QDateTime::currentDateTime() > datetime || bBindSucess || m_bExtraUnBindFlag))
        {
            if(!checkRunCardBindState(m_nExtraInfoID))
            {
                bBindSucess = bindUserToElement(userInfo,location,m_nExtraInfoID);
            }

            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        }
    }

    m_ui.stopNFCValidTimer();

    //判断是否手动取消绑定
    if(m_bExtraUnBindFlag)
    {
        return BANDING_CANCEL;
    }

    //3.判断绑定是否超时
    if(!m_bExtraBindFlag)
    {
        return BANDING_OUTTIME;
    }

    //5.绑定卡号(中长跑往下发送绑定命令可能失败)
    bool bFlag = false;
    if(nDevType == DEV_TYPE_RUN_MID)
    {
        bFlag = bBindSucess;
    }
    else
    {
        bFlag = bindUserToElement(userInfo,location,m_nExtraInfoID);
    }

    if(bFlag)
    {
        return SUCESS_BIND_EXTRA;
    }
    else
    {
        return BANDING_FALSE;
    }
}

void CTestGuideManager::resetExtraBindInfo()
{
    m_bExtraBindFlag = false;
    m_bExtraUnBindFlag = false;
}

bool CTestGuideManager::checkRunCardBindState(const unsigned int &nCardId)
{
    bool bRet = false;

    for(auto &element : m_mapTestGuide)
    {
        if(element.second->getDevType() == DEV_TYPE_RUN_MID)
        {
            if(reinterpret_cast<CTestGuideRunMid *>(element.second)->checkRunCardBindState(nCardId))
            {
                bRet = true;
                break;
            }
        }
        else if(element.second->getDevType() == DEV_TYPE_ROPE_SKIP)
        {
            if(reinterpret_cast<CTestGuideSkip *>(element.second)->checkSkipBindState(nCardId))
            {
                bRet = true;
                break;
            }
        }
    }

    return bRet;
}

bool CTestGuideManager::checkSkipRopeBindState(const unsigned int &nCardId)
{
    bool bRet = false;

    for(auto &element : m_mapTestGuide)
    {
        if(element.second->getDevType() == DEV_TYPE_ROPE_SKIP)
        {
            if(reinterpret_cast<CTestGuideSkip *>(element.second)->checkSkipBindState(nCardId))
            {
                bRet = true;
                break;
            }
        }
    }

    return bRet;
}

//时间超时检测，检测测试时间是否超时，主动结束超时设备
void CTestGuideManager::onTimeOut()
{
    checkUserTestTimeOut();

    checkCastStartTestSucess();
}

void CTestGuideManager::checkUserTestTimeOut()
{
    //0.声明对应知道类变量(中长跑、跳绳)
    CTestGuideRunMid *pTestGuidRunMid = nullptr;
    CTestGuideSkip *pTestGuidSkip = nullptr;

    //1.用新的容器保存用户信息，避免循环中用户信息被删除
    map<CLocation, CUserInfo> mapUserInfo(m_mapUserInfo);

    //2.增加延时强制结束功能
    for(auto element = mapUserInfo.begin(); element != mapUserInfo.end(); element++)
    {
        CUserInfo &userInfo = element->second;
        const CLocation &location = element->first;

        if(userInfo.m_bCheckTime)
        {
            switch (userInfo.m_shDevType)
            {
            case DEV_TYPE_RUN_MID:
            {
                pTestGuidRunMid = reinterpret_cast<CTestGuideRunMid *>(getTestGuid(location.m_iSteDevMacID));
                if(pTestGuidRunMid != nullptr)
                {
                    //中长跑时间大于6分钟强制结束
                    if(QDateTime::currentDateTime().toMSecsSinceEpoch() - userInfo.m_llTestActualStartTime > 6 * 60 * 1000)
                    {
                        pTestGuidRunMid->procTestEnd(location.m_shElementIndex);
                    }
                }
            }
                break;
            case DEV_TYPE_ROPE_SKIP:
            {
                pTestGuidSkip = reinterpret_cast<CTestGuideSkip *>(getTestGuid(location.m_iSteDevMacID));
                if(pTestGuidSkip != nullptr)
                {
                    qint64 timeDif = QDateTime::currentDateTime().toMSecsSinceEpoch() - userInfo.m_llTestActualStartTime;
                    //因为存在标志位，所以只会进入一次，仅当时间满足后消除
                    if(m_bPrepareStatus && timeDif/m_nTimerTimeOutInterval > 3)
                    {
                        m_bPrepareStatus = false;

                        m_ui.setBtnEnable(CProjectEnums::OTM_CANCEL_TEST_BTN, true);
                        m_ui.setBtnEnable(CProjectEnums::OTM_RESTART_TEST_BTN, true);

                        //以下两个函数，目的均是为了保证命令被成功取消
                        //删除队列中的发送命令
                        unsigned short usElement = ELEMENT_NUM_CUR_DEV;
                        pTestGuidSkip->removeCommand(usElement);

                        //获取设备，设置成功标志
                        CDevSteSkip *pDevSkip = reinterpret_cast<CDevSteSkip *>(pTestGuidSkip->getSteDev());
                        if(nullptr != pDevSkip)
                        {
                            pDevSkip->setSuccessFlag(true);
                        }
                    }
                    //2.若已经超过限制时间，则进行是否强制结束的判断
                    float  fOverTime = static_cast<float>((timeDif - m_iSkipTime) / 1000.0f);
                    if(fOverTime > m_iDelayTime)
                    {
                        int iSkipState = pTestGuidSkip->getSkipTestState(location.m_shElementIndex);
                        if(iSkipState != -1 && iSkipState != CTestStateManage::TEST_COMPLETED)
                        {
                            //不记成绩，显示超时结束
                            pTestGuidSkip->forcedStopTest(location.m_shElementIndex);
                        }
                    }
                }
            }
                break;
            default:
            {
                LOGERROR("Dev Type does not exist!");
            }
                break;
            }
        }
    }
}

void CTestGuideManager::checkCastStartTestSucess()
{
    if(!m_bCheckCastStart || QDateTime::currentMSecsSinceEpoch() - m_llCheckCastStartTime < 3000)
        return;

    CTestGuideBase *pTestGuid, *pTestGuidSendCast(nullptr);
    unsigned int nDevID;
    set<CTestGuideBase*> setStart, setNotStart;
    set<CTestGuideBase*>::iterator itStart, itNotStart;
    set<unsigned short> setCastCmd;
    for(auto &element : m_mapUserInfo)
    {
        nDevID = element.first.m_iSteDevMacID;
        pTestGuid = getTestGuid(nDevID);
        if(pTestGuid == nullptr)
        {
            LOGERROR("pTestGuid == nullptr id = " + to_string(nDevID));
            continue;
        }

        //设备只要发送命令之后启动过就算开启成功，后续重连在测试指导中有对应逻辑，这里仅处理最初发送命令不成功
        itStart = setStart.find(pTestGuid);
        if(itStart != setStart.end())
            continue;

        if(pTestGuid->isTestStart(element.first.m_shElementIndex))
        {
            setStart.insert(pTestGuid);
            setNotStart.erase(pTestGuid);
        }
        else
            setNotStart.insert(pTestGuid);

        setCastCmd = pTestGuid->getResendCmd();
        if(setCastCmd.size() > 0)
        {
            pTestGuidSendCast = pTestGuid;
        }
    }

    //全部开启
    if(setNotStart.size() == 0)
    {
        m_bCheckCastStart = false;
    }
    //全部关闭
    else if(setStart.size() == 0)
    {
        bool bSwitchTestGuid = pTestGuidSendCast == nullptr;
        //若没有对应的测试指导和对应的测试指导没活动，那么挑选一个正常活动的测试指导进行发送广播开始
        if(!bSwitchTestGuid)
        {
            bSwitchTestGuid = !pTestGuid->getActiveState();
            pTestGuid->setResendCmd();
            pTestGuid->setDelResendCmd();

            //找到可用的测试指导发送开始
            if(bSwitchTestGuid)
            {
                for(auto &element : setNotStart)
                {
                    if(element->getActiveState())
                    {
                        //通过统一的结构进行命令发送
                        pTestGuid->setResendCmdElement(ELEMENT_NUM_CUST);
                    }
                }
            }
        }
    }
    //部分开启部分关闭，逻辑完成之后将标志设置为false，转到自己设备中进行
    else
    {
        for(auto &element : setNotStart)
        {
            if(!element->isTestStart(0)) //普通设备仅一个测试单元
            {
                element->setResendCmdElement(0);
            }
        }
        m_bCheckCastStart = false;
    }
}

void CTestGuideManager::startRunShortSyncByStartingGun()
{
    //0.至少存在一个测试用户
    if(m_mapUserInfo.size() == 0)
    {
        LOGWARN("no user");
        return;
    }

    //1.至少存在一个测试指导
    CTestGuideRunShort *pDevRunShort = nullptr;
    auto itFind = m_mapTestGuide.find(m_mapUserInfo.begin()->first.m_iSteDevMacID);
    if(itFind == m_mapTestGuide.end())
    {
        LOGERROR("m_mapTestGuide not exist devID" + to_string(m_mapUserInfo.begin()->first.m_iSteDevMacID));
        return;
    }

    pDevRunShort = reinterpret_cast<CTestGuideRunShort *>(itFind->second);

    if(nullptr == pDevRunShort)
    {
        LOGERROR("CTestGuideRunShort is nullptr");
        return;
    }

    pDevRunShort->startTest(true,true);
}

void CTestGuideManager::startRopeSkipSyncByStartingGun()
{
    //0.至少存在一个测试用户
    if(m_mapUserInfo.size() == 0)
    {
        LOGWARN("no user");
        return;
    }

    //1.至少存在一个测试指导
    CTestGuideSkip *pDevRopeSkip = nullptr;
    auto itFind = m_mapTestGuide.find(m_mapUserInfo.begin()->first.m_iSteDevMacID);
    if(itFind == m_mapTestGuide.end())
    {
        LOGERROR("m_mapTestGuide not exist devID" + to_string(m_mapUserInfo.begin()->first.m_iSteDevMacID));
        return;
    }

    pDevRopeSkip = reinterpret_cast<CTestGuideSkip *>(itFind->second);

    if(nullptr == pDevRopeSkip)
    {
        LOGERROR("CTestGuideRunShort is nullptr");
        return;
    }

    pDevRopeSkip->startTest(true,true);
}

//启动跳绳的消息(安全机制：每根绳子的超时判断)
void CTestGuideManager::onStartSkipRope(const CLocation &location,const QDateTime &startTime)
{
    //0.找到对应位置的UserInfo
    bool bFindFlag = false;
    auto iter = m_mapUserInfo.begin();
    for(; iter != m_mapUserInfo.end(); iter++)
    {
        if(iter->first.m_shElementIndex == location.m_shElementIndex)
        {
            bFindFlag = true;
            break;
        }
    }

    //1.判断是否存在对应信息
    if(!bFindFlag)
    {
        LOGWARN("Error elementIndex in mapUserInfo");
        return;
    }
    //2.改变对应属性
    else
    {
        iter->second.m_llTestActualStartTime = startTime.toMSecsSinceEpoch();
        iter->second.m_bCheckTime = true;
    }
}

void CTestGuideManager::onSkipBind(const unsigned int &uiElementID)
{
    m_bExtraBindFlag = true;
    m_nExtraInfoID = uiElementID;
}

//跳绳设备唯一ID编号， 是否可用标志
void CTestGuideManager::onCheckSubElement(const unsigned int &iSubID, const bool &bFlag)
{
    short shDevStatus = CDevDataCfg::getColTypeIndex(COL_DEV_STATE, DEV_TYPE_ROPE_SKIP, TEST_ASYNC);
    short shIndex = CDevDataCfg::getColTypeIndex(COL_DEV_ID, DEV_TYPE_ROPE_SKIP, TEST_ASYNC);

    if(bFlag)
    {
        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shIndex, \
                                 QString::number(iSubID), shDevStatus, "测试中");
    }
    else
    {
        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shIndex, \
                                 QString::number(iSubID), shDevStatus, "设备断连");
    }
}

void CTestGuideManager::onTestPrepare(const CLocation &location)
{
    auto pTestGuid = getTestGuid(location.m_iSteDevMacID);
    if(pTestGuid == nullptr)
    {
        LOGERROR("pTestGuid == nullptr");
        return;
    }

    //通用语音，请准备(中长跑等设备存在自己的处理语音)
    if(!CDevDataCfg::isSpecialDev(pTestGuid->getDevType()))
    {
        pTestGuid->setTestState(CTestStateManage::TEST_PREPARE, location.m_shElementIndex, true, true, false);
    }
    else
    {
        //当为非直连模式，且为异步跳绳时，需要进行等待
        short shTestMode = getActualTestMode(CDevDataCfg::getDevDirectConnectFlag(), CTestGuideBase::s_shTestMode);
        if(pTestGuid->getDevType() == DEV_TYPE_ROPE_SKIP)
        {
            if(shTestMode == TEST_SYNC)
            {
                //进入准备阶段，同时禁用按钮，播放语音
                m_ui.setBtnEnable(CProjectEnums::OTM_CANCEL_TEST_BTN, false);
                m_ui.setBtnEnable(CProjectEnums::OTM_RESTART_TEST_BTN, false);
                m_ui.setBtnEnable(CProjectEnums::OTM_SYNC_COMMIT_BTN, false);
                m_bPrepareStatus = true;
                m_objSpeak.textToSpeech("请在跳绳手柄震动后开始测试");
            }
            else
            {
                m_objSpeak.playCountdownSpeech(10);
            }
        }
    }
}

void CTestGuideManager::onStartElement(const CLocation &location)
{
    //单个启动肯定是直连或者异步测试,不需要再进行判断
    auto pTestGuid = getTestGuid(location.m_iSteDevMacID);
    if(pTestGuid == nullptr)
    {
        LOGERROR("not exist dev id = " + to_string(location.m_iSteDevMacID));
        return;
    }

    //直连或者异步才进行语音播报，要不就
    bool bSpeak = CDevDataCfg::getDevDirectConnectFlag() || CTestGuideBase::s_shTestMode == TEST_ASYNC;

    short shDevType = pTestGuid->getDevType();
    switch (shDevType) {
    case DEV_TYPE_RUN_MID:
        bSpeak = false;
        m_objSpeak.textToSpeechBlock("预备");
        if(actualStartElement(pTestGuid, location, bSpeak))
        {

            m_objSpeak.textToSpeech("跑");
        }
        break;
    case DEV_TYPE_ROPE_SKIP:
        break;
    default:
        actualStartElement(pTestGuid, location, bSpeak);
        break;
    }
}

void CTestGuideManager::onStartAllElement(const unsigned int &iDevId)
{
    //多个设备开启成功之后统一进行开始语音播报,同时设置其中有效测试单元开启状态
    auto pTestGuid = getTestGuid(iDevId);
    if(pTestGuid == nullptr)
    {
        LOGERROR("not exist dev id = " + to_string(iDevId));
        return;
    }

    //统一的命令发送开始
    //若发令枪存在，使用发令枪进行开启设备触发
    if(m_bStartingGunExist)
    {
        m_bStartTestReady = true;
    }
    //常用的触发进行测试开启
    else
    {
        short shDevType = pTestGuid->getDevType();

        //若是中长跑需要在这里进行语音播报
        if(shDevType == DEV_TYPE_RUN_MID)
        {
            m_objSpeak.playStartSpeech();
        }

        //更新同步开始的时间
        CTestGuideRunMid::upDateSyncActualTime();

        //在播放语音期间限制按钮的使用
        actualStartAllElement(pTestGuid, true);
    }
}

void CTestGuideManager::onStartCast()
{
    size_t szSize = m_mapTestGuide.size();
    if(szSize == 0)
    {
        return;
    }

    short shDevType = m_objSteDevMgr.getType();

    //统一的命令发送开始
    //若发令枪存在，使用发令枪进行开启设备触发
    if(m_bStartingGunExist)
    {
        m_bStartTestReady = true;
    }
    //常用的触发进行测试开启
    else
    {
        //若是中长跑需要在这里进行语音播报
        if(shDevType == DEV_TYPE_RUN_MID)
        {
            m_objSpeak.playStartSpeech();
        }

        //更新同步开始的时间
        CTestGuideRunMid::upDateSyncActualTime();

        //在播放语音期间限制按钮的使用
        actualStartCast(true);
    }
}

void CTestGuideManager::onTestComplete(const CLocation &location)
{
    //0.安全保护，保证测试用户存在
    auto itFindUser = m_mapUserInfo.find(location);
    if(itFindUser == m_mapUserInfo.end())
    {
        LOGERROR("Invalid Test User");
        return;
    }

    bool bDirectConnect = CDevDataCfg::getDevDirectConnectFlag();
    short shTestMode = getActualTestMode(bDirectConnect, CTestGuideBase::s_shTestMode);

    //存储完成测试用户信息
    CCompleteTestResult completeTestRes;
    completeTestRes.m_userInfo = itFindUser->second;
    CUserInfo &userInfo = completeTestRes.m_userInfo;

    switch (shTestMode) {
    // 同步模式
    case TEST_SYNC:{
        // UI显示完成测试人员
        QString qsTip(userInfo.m_strName + "测试完成");
        m_ui.setTestTipText(qsTip);
        break;
    }
        // 异步模式
    case TEST_ASYNC:{
        // 取消用户绑定
        unbindUserInfo(itFindUser->second);

        // 若不存在测试用户,则不显示警告窗口
        if(m_mapUserInfo.size() <= 0)
        {
            m_ui.setStuTestStatus(true, true);
        }
        break;
    }
    default:
        break;
    }

    //4.保存测试结果到数据库,同时删除测试指导中的临时结果
    //注：所有结果都会保存，在主动退出时会删除
    CTestGuideBase *pTestGuid = getTestGuid(location.m_iSteDevMacID);
    if(pTestGuid == nullptr)
    {
        LOGERROR("onTestComplete:not find pTestGuid");
        return;
    }

    vector<CTestResultInfo> vecTestResult;
    pTestGuid->getTestResult(vecTestResult, location.m_shElementIndex, true);
    if(vecTestResult.size() == 0)
    {
        LOGERROR("getTestResult false");
        return;
    }

    saveTestResultToLoacl(vecTestResult);

    //游客模式触发游客上传
    if(userInfo.isTouristInfo())
    {
        CServerCommunication::Get_Instance().setUpdateType(CServerCommunication::UPDATE_TYPE_TOURIST_USAGE);
        return;
    }

    //5.保存测试结果到内存区，用于后续查找删除
    completeTestRes.m_vecTestResInfo = vecTestResult;

    addCompletTestResToCompletContent(location, shTestMode, completeTestRes);

    //测试完成更新显示成绩
    dispTestResult();

    //6.直连模式 直接进入自动提交模式
    if(bDirectConnect)
    {
        m_ui.autoCommite();
    }
    //同步模式
    else if(TEST_SYNC == shTestMode)
    {
        bool bTestFinished = m_vecCompleteTestRes.size() == m_mapUserInfo.size();

        if(bTestFinished)
        {
            m_objSpeak.textToSpeech("测试完成");

            m_ui.autoCommite();
        }

        m_ui.setStuTestStatus(true, bTestFinished);
    }
    //异步模式
    else if(TEST_ASYNC == shTestMode)
    {
        //判断用户是不是正常完成，根据项目测试次数进行判断
        int nTimes = 1;
        CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestTimes(userInfo.m_shDevType, nTimes);
        if(vecTestResult[0].getTestTimes() >= nTimes)
        {
            // 语音提示具体用户完成测试
            QString qsText = userInfo.m_strName + "同学测试完成";
            m_objSpeak.textToSpeech(qsText);
        }
    }

    //设置提交按钮可用
    m_ui.setBtnEnableCommit(m_vecCompleteTestRes.size() > 0);
}

void CTestGuideManager::startCheackTimeTimer()
{
    if(!m_timer.isActive())
    {
        m_timer.start(m_nTimerTimeOutInterval);
    }
}

void CTestGuideManager::stopCheackTimeTimer()
{
    m_timer.stop();
}

short CTestGuideManager::convertC2STestNature(const short &shClientType)
{
    short shNature = CServerCommunication::TEST_NATURE_EXERCISE;

    QString logInfo = QString( "src score type = " + QString::number( shClientType ) );

    LOGINFO( logInfo.toStdString() );
    if(shClientType == CTestResultInfo::TEST_INFORMAL)
    {
        shNature = CServerCommunication::TEST_NATURE_EXERCISE;
    }
    else if (shClientType == CTestResultInfo::TEST_FORMAL)
    {
        shNature = CServerCommunication::TEST_NATURE_TEST;
    }
    else{
         shNature = CServerCommunication::TEST_NATURE_EXERCISE;
    }

    QString ret = "score type ret = "  + QString::number( shNature );

    LOGINFO( ret.toStdString() );

    return shNature;
}

void CTestGuideManager::updateBindUserNum()
{
    //在取消测试状态时刷新界面绑定人数和按钮是否可用
    if(m_bInCancelTestState)
        return;

    m_ui.setMultiUserCount(QString::number(m_mapUserInfo.size()));
    bool bFlag = m_mapUserInfo.size() > 0;
    m_ui.setBtnEnable(CProjectEnums::OTM_START_TEST_BTN, bFlag);
    //异步模式下，绑定人员即可点击提交，此时弹窗提示是否取消测试
    if(m_vecCompleteTestRes.size() == 0)
        m_ui.setBtnEnable(CProjectEnums::OTM_ASYNC_COMMIT_BTN, bFlag);
}

void CTestGuideManager::dealUploadRetValue(CHttpUploadInfos &infos)
{
    //判断是否上传成功
    bool ok = infos.m_isSuccess;
    string dataBaseTime = CTestResultInfo::getSaveTimeStamp(infos.m_recordTime);
    QString logInfo = QString( "onUploadCommonScoreFinished ret = %1, "
                               "userId = %2, "
                               "is Finished = %3, "
                               "testItem = %4, "
                               "record time = %5,"
                               "current server state = %6,"
                               "data src = %7, "
                               "commit progress = %8"
                               "test result = %9" ).arg( QString::number( ok ),
                                                         infos.m_userId,
                                                         QString::number( infos.m_isFinished ), //m_isFinished 控制是否当前提交的最后一条
                                                         infos.m_testItem,
                                                         QString::fromStdString( dataBaseTime ),
                                                         QString::number( infos.m_isServerOnline),
                                                         QString::number( infos.m_dataSrc ),
                                                         infos.m_commitProgress,
                                                         infos.m_testRet
                                                         );

    qDebug() << logInfo;
    LOGINFO( logInfo.toStdString() );

    //先写死， 在界面上必定显示成功, 其实可能失败, 通过错误码判断
    ok = true;

    //如果提交时没有网，则直接先在界面显示提交成功
    if( !infos.m_isServerOnline && infos.m_dataSrc == UserExam ){
        //进行界面操作
        m_ui.setUpdateCurTestRet( ok );
        submitCommonSuccess();
    }

    if(infos.m_isFinished && infos.m_dataSrc == UserExam)
    {
        //进行界面操作
        m_ui.setUpdateCurTestRet( ok );

        if( !ok ){
            m_ui.setErrorString( m_pReqeuestMgr->getErrorString() );
        }else{
            submitCommonSuccess();
        }
    }

    //有网状态下 只要不是超时返回，设置数据库中的内容为true
    if( infos.requestCode == 0 )
    {
        if(infos.requestCode != 0)
        {
            LOGERROR("");
        }

        //若当前是身高体重，需要更新 身高体重和人体成分的标志
        int nTestItem = infos.m_testItem.toInt();
        short shTestItem = CDevDataCfg::getTestItemFromHttpItem(nTestItem);
        short shDevType = CDevDataCfg::getDevTypeByTestItem(shTestItem);

        if(shDevType == DEV_TYPE_BODY)
        {
            updateTestResultSyncFlag(infos.m_userId, TEST_ITEM_HEIGHT, infos.m_recordTime);
            updateTestResultSyncFlag(infos.m_userId, TEST_ITEM_WEIGHT, infos.m_recordTime);
            updateTestResultSyncFlag(infos.m_userId, TEST_ITEM_HBC, infos.m_recordTime);
        }
        else
        {
            updateTestResultSyncFlag(infos.m_userId, nTestItem, infos.m_recordTime);
        }
    }
}

void CTestGuideManager::updateTestResultSyncFlag(const QString &qsUserID,  const int &nTestItem, const QString &qsTimeStamp)
{
    auto pTestResultTable = CDatabaseManage::GetInstance().getTabTestItemResult();
    if(pTestResultTable == nullptr)
    {
        LOGERROR("pTestResultTable == nullptr");
        return;
    }

    pTestResultTable->updateSync(qsUserID.toLongLong(), nTestItem, CTestResultInfo::getSaveTimeStamp(qsTimeStamp), true);
}

void CTestGuideManager::updateTestResultSyncFlag(const vector<CTestResultInfo> vecSingleTest)
{
    auto pTestResultTable = CDatabaseManage::GetInstance().getTabTestItemResult();
    if(pTestResultTable == nullptr)
    {
        LOGERROR("pTestResultTable == nullptr");
        return;
    }

    for(auto &element : vecSingleTest)
    {
        pTestResultTable->updateSync(element.m_llUniqueID, element.m_nItem, element.m_strStartTime, true);
    }
}

void CTestGuideManager::connectSignal(CDevHardwareBase *pSteDev, CTestGuideBase *pTestGuidBase)
{
    if(pSteDev == nullptr || pTestGuidBase == nullptr)
        return;

    auto &shDevType = pSteDev->getDevType();
    if(shDevType == DEV_TYPE_ROPE_SKIP)
    {
        //CDevSteSkip *pDevSkip = reinterpret_cast<CDevSteSkip *>(pTestGuidBase->getSteDev());
        CDevSteSkip *pDevSkip = reinterpret_cast<CDevSteSkip *>(pSteDev);
        connect(pDevSkip, &CDevSteSkip::signalStartSkipRope, this, &CTestGuideManager::onStartSkipRope);
        connect(pDevSkip, &CDevSteSkip::signalSkipBind, this, &CTestGuideManager::onSkipBind);
        connect(pDevSkip, &CDevSteSkip::signalCheckSubElement, this, &CTestGuideManager::onCheckSubElement);
    }
    else
    {
        Qt::ConnectionType type = Qt::AutoConnection;
        if(shDevType == DEV_TYPE_RUN_MID)
        {
            type = Qt::DirectConnection;

            CTestGuideRunMid *pRunMid = reinterpret_cast<CTestGuideRunMid *>(pTestGuidBase);
            connect(&m_ui, &CInterAction::sigRunMidLengthChanged, pRunMid, &CTestGuideRunMid::onRunMidLenthChanged);
            connect(&m_ui, &CInterAction::sigRunMidLengthChanged, pRunMid, &CTestGuideRunMid::onRunMidLenthChanged);
        }

        connect(pSteDev, &CDevHardwareBase::signalStartElement, this, &CTestGuideManager::onStartElement, type);
        connect(pSteDev, &CDevHardwareBase::signalStartAllElement, this, &CTestGuideManager::onStartAllElement, type);
        connect(pSteDev, &CDevHardwareBase::signalStartCast, this, &CTestGuideManager::onStartCast, type);
    }

    connect(pTestGuidBase, &CTestGuideBase::signalErrStateChange, this, &CTestGuideManager::onDevErrStateChange, Qt::QueuedConnection);
    connect(pTestGuidBase, &CTestGuideBase::signalTestComplete, this, &CTestGuideManager::onTestComplete, Qt::DirectConnection);
    connect(pTestGuidBase, &CTestGuideBase::signalDevDisconn, this, &CTestGuideManager::onDevDisconn, Qt::DirectConnection);

    connect(pTestGuidBase, &CTestGuideBase::sigTestPrepare, this, &CTestGuideManager::onTestPrepare, Qt::DirectConnection);
}

void CTestGuideManager::disconnectSignal(CDevHardwareBase *pSteDev, CTestGuideBase *pTestGuidBase)
{
    if(pSteDev == nullptr || pTestGuidBase == nullptr)
        return;

    auto &shDevType = pSteDev->getDevType();
    if(shDevType == DEV_TYPE_ROPE_SKIP)
    {
        //CDevSteSkip *pDevSkip = reinterpret_cast<CDevSteSkip *>(pTestGuidBase->getSteDev());
        CDevSteSkip *pDevSkip = reinterpret_cast<CDevSteSkip *>(pSteDev);
        disconnect(pDevSkip, &CDevSteSkip::signalStartSkipRope, this, &CTestGuideManager::onStartSkipRope);
        disconnect(pDevSkip, &CDevSteSkip::signalSkipBind, this, &CTestGuideManager::onSkipBind);
        disconnect(pDevSkip, &CDevSteSkip::signalCheckSubElement, this, &CTestGuideManager::onCheckSubElement);
    }
    else
    {
        disconnect(pSteDev, &CDevHardwareBase::signalStartElement, this, &CTestGuideManager::onStartElement);
        disconnect(pSteDev, &CDevHardwareBase::signalStartAllElement, this, &CTestGuideManager::onStartAllElement);
        disconnect(pSteDev, &CDevHardwareBase::signalStartCast, this, &CTestGuideManager::onStartCast);

        if(shDevType == DEV_TYPE_RUN_MID)
        {
            CTestGuideRunMid *pRunMid = reinterpret_cast<CTestGuideRunMid *>(pTestGuidBase);
            disconnect(&m_ui, &CInterAction::sigRunMidLengthChanged, pRunMid, &CTestGuideRunMid::onRunMidLenthChanged);
        }
    }

    disconnect(pTestGuidBase, &CTestGuideBase::signalErrStateChange, this, &CTestGuideManager::onDevErrStateChange);
    disconnect(pTestGuidBase, &CTestGuideBase::signalTestComplete, this, &CTestGuideManager::onTestComplete);
    disconnect(pTestGuidBase, &CTestGuideBase::signalDevDisconn, this, &CTestGuideManager::onDevDisconn);

    disconnect(pTestGuidBase, &CTestGuideBase::sigTestPrepare, this, &CTestGuideManager::onTestPrepare);
}

void CTestGuideManager::dispTestResultForOto()
{
    //当前是自动提交才进行设置
    if(!CDatabaseManage::GetInstance().getPCommitTableConfig()->isAutoCommit())
        return;

    //获取临时完成的成绩,并且不影响当前的测试状态
    vector<CCompleteTestResult> vecCompleteTemp;
    getTemporaryRes(vecCompleteTemp);

    for(auto &element : m_vecCompleteTestRes)
    {
        vecCompleteTemp.push_back(element);
    }

    if(vecCompleteTemp.size() == 0)
        return;

    auto vecTemp = (*vecCompleteTemp.rbegin()).m_vecTestResInfo;
    string strTemp;

    if(m_objSteDevMgr.getType() == DEV_TYPE_BODY)
    {
        QString qsWeight, qsHeight;
        for(auto &testResult : vecTemp)
        {
            switch (testResult.m_nItem) {
            case TEST_ITEM_WEIGHT:
                testResult.getLastTestResult(strTemp);
                qsWeight = CTestGuideBase::value2DispValue(static_cast<short>(testResult.m_nItem), strTemp);
                break;
            case TEST_ITEM_HEIGHT:
                testResult.getLastTestResult(strTemp);
                qsHeight = CTestGuideBase::value2DispValue(static_cast<short>(testResult.m_nItem), strTemp);
                break;
            default:
                break;
            }
        }

        m_ui.setOtoBodyValue(qsWeight, qsHeight);
    }
    else
    {
        if(vecTemp.size() > 0)
        {
            QString qsDisp = CTestGuideBase::getTestResult(true, vecTemp[0]);
            m_ui.setOtoMaxRet(qsDisp);
        }
    }
}

void CTestGuideManager::checkAllUserCommitable()
{
    string strValidResult;

    if((m_mapUserInfo.size() != m_vecTempTestRes.size() && CTestGuideBase::s_shTestMode == TEST_SYNC) ||
            (m_mapUserInfo.size() != m_vecTempTestRes.size() && m_mapUserInfo.size() != 0 && CTestGuideBase::s_shTestMode == TEST_ASYNC))
    {
        m_ui.setStuTestStatus(true, false);
        return;
    }

    for(auto userEle : m_mapUserInfo)
    {
        for(auto testTempResEle : m_vecTempTestRes)
        {
            if(testTempResEle.m_vecTestResInfo.size())
            {
                if(testTempResEle.m_vecTestResInfo[0].m_llUniqueID == userEle.second.m_llUniqueID && testTempResEle.m_vecTestResInfo[0].getValidMaxTestResult(strValidResult) == -1)
                {
                    m_ui.setStuTestStatus(true, false);
                    return;
                }
            }
        }
    }

    m_ui.setStuTestStatus(true, true);
}

void CTestGuideManager::onSignalFoulType(unsigned short foulType)
{
    CDevHardwareBase *pTemp;
    CTestGuideBase *pTestGuid;
    for(auto &element : m_mapUserInfo)
    {
        pTemp = m_objSteDevMgr.getDevSteByID(element.first.m_iSteDevMacID);
        if(pTemp != nullptr)
        {
            short testState = pTemp->getElement(element.first.m_shElementIndex)->getTestState();
            if(testState == CTestStateManage::TEST_IN_PROCESS)
            {
                pTestGuid = getTestGuid(element.first.m_iSteDevMacID);

                if(pTestGuid != nullptr)
                {
                    pTestGuid->setDevFoulType(element.first.m_shElementIndex, foulType);
                }
            }
        }

    }
}

void CTestGuideManager::dispTestRestltForOtm()
{
    m_ui.clearModelData(CInterAction::SCORE_DATA_MODEL);

    //获取临时完成的成绩,并且不影响当前的测试状态，这里获取所有测试结果的逻辑会
    //影响到一对多中查看成绩界面的删除逻辑
    getTemporaryRes(m_vecTempTestRes);

    for(auto &element : m_vecCompleteTestRes)
    {
        m_vecTempTestRes.push_back(element);
    }

    if(m_vecTempTestRes.size() == 0)
        return;

    short shDevType, i = 0;
    QStringList listContent;

    checkAllUserCommitable();

    bool bVerifySubmitBtnUsable = false;
    for(auto element : m_vecTempTestRes)
    {
        vector<CTestResultInfo> &vecTestRes = element.m_vecTestResInfo;
        CUserInfo &userInfo = element.m_userInfo;

        if(vecTestRes.size() == 0 || isDelTestRes(userInfo))
            continue;

        bVerifySubmitBtnUsable = true;
        shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(vecTestRes[0].m_nItem));
        listContent = CDevDataCfg::getScoreTableContent(userInfo, vecTestRes, shDevType);

        //更新序号
        if(listContent.size() > 0)
        {
            listContent[0] = QString::number(i + 1);
        }

        m_ui.appendModelData(CInterAction::SCORE_DATA_MODEL, listContent);
        i++;
    }

    m_ui.setBtnEnable(CProjectEnums::OTM_VERIFY_SIGN_BTN, bVerifySubmitBtnUsable);
}

void CTestGuideManager::initSig()
{
    if( nullptr == m_pReqeuestMgr ){
        m_pReqeuestMgr = CHttpRequestMgr::getInstance();
    }

    connect( m_pReqeuestMgr, &CHttpRequestMgr::sigUploadCommonScore,
             this, &CTestGuideManager::onUploadCommonScoreFinished, Qt::QueuedConnection);

    connect( m_pReqeuestMgr, &CHttpRequestMgr::sigUploadBodyScore,
             this, &CTestGuideManager::onUploadBodyScoreFinished, Qt::QueuedConnection);

    connect( m_pReqeuestMgr, &CHttpRequestMgr::sigUploadEyeSightScore,
             this, &CTestGuideManager::onUploadEyeSightScoreFinished, Qt::QueuedConnection);


    connect( m_pReqeuestMgr, &CHttpRequestMgr::sigUploadMultiCommonScoreFinished,
             this, &CTestGuideManager::onUploadMulityScoreFinished, Qt::QueuedConnection );


    connect( m_pReqeuestMgr, &CHttpRequestMgr::sigUploadMultiBodyScoreFinished,
             this, &CTestGuideManager::onUploadMulityScoreFinished, Qt::QueuedConnection );


    connect( m_pReqeuestMgr, &CHttpRequestMgr::sigUploadMultiEyeSightScoreFinished,
             this, &CTestGuideManager::onUploadMulityScoreFinished, Qt::QueuedConnection );

    connect( m_pReqeuestMgr, &CHttpRequestMgr::sigSyncMultiLocalScoresFinished,
             this, &CTestGuideManager::onUploadMulityScoreFinished, Qt::QueuedConnection );
}

int CTestGuideManager::convertCommonTestRes2ServerFormat(QMap<QString, QVariant> &retInfo, const vector<CTestResultInfo> &vecTestResult, bool bNew)
{
    int nRet = HTTP_TEST_ITEM_UNKNOW;
    retInfo.clear();

    if(vecTestResult.size() != 1){
        LOGERROR("uploadCommonScore vecTestResult size  != 1");
        return nRet;
    }

    const CTestResultInfo &testResult = vecTestResult[0];

    addCommonServerFormatInfo(retInfo, testResult);

    string strValidResult;

    //不存在有效成绩,则将结果赋值为 0
    uint nIllegalTimes;
    int nValideIndex = testResult.getValidMaxTestResult(strValidResult, nIllegalTimes);
    if(nValideIndex < 0)
    {
        LOGWARN("not exist valid data");
        strValidResult = "-50";
    }

    //添加过程资源,新旧版本区别在于过程资源的添加
    else
    {
        QStringList processPic;
        auto vecSrc = testResult.getProcSrc(nValideIndex);
        for(auto &element : vecSrc)
        {
            if(!CFilePath::isFileExist(element))
                continue;

            if(bNew){
                retInfo.insertMulti( HTTP_KEYS::files, QString::fromStdString(element) );
            }
            else {
                processPic << QString::fromStdString(element);
                if(&element == &(*vecSrc.rbegin()))
                {
                    retInfo.insert(HTTP_KEYS::files, processPic);
                }
            }
        }
    }

    nRet = CDevDataCfg::getHttpTestItemFromItem(static_cast<short>(testResult.m_nItem));
    retInfo.insert( HTTP_KEYS::testItem, nRet);

    if(testResult.m_nItem == TEST_ITEM_VOLLEYBALL)
    {
        QString qsTemp = QString::number(atoi(strValidResult.c_str()) - static_cast<int>(nIllegalTimes));
        retInfo.insert( HTTP_KEYS::result, qsTemp );
    }
    else
    {
        retInfo.insert( HTTP_KEYS::result, QString::fromStdString( strValidResult ) );
    }


#if QT_NO_DEBUG

    QString logInfo = "config multi upload score info:: ";

    logInfo += variantMap2Json( retInfo );
    LOGINFO( logInfo.toStdString() );
#else
    qDebug() << "";
#endif

    return nRet;
}

bool CTestGuideManager::convertBodyTestRes2ServerFormat(QMap<QString, QVariant> &retInfo, const vector<CTestResultInfo> &vecTestResult, bool bNew)
{
    //Tips: 一次http请求只能上传一个人的成绩, 如果是多人测试, 依次循环上传
    if(vecTestResult.size() < 2)
    {
        LOGERROR("vecTestResult size < 2");
        return false;
    }

    string strTemp;
    float fHeight = 160, fWeight = 50;

    const CTestResultInfo &testResult = vecTestResult[0];

    addCommonServerFormatInfo(retInfo, testResult);

    //遍历vec获取身高体重
    //填入人体成分信息
    short shState = 0;

    //遍历vec获取身高体重
    //填入人体成分信息
    QMap<int, QString> mapItemRes;
    int nHttpItem, nIndexHBC = -1, nIndexTemp = 0;

    for(auto &testResultElement : vecTestResult)
    {
        testResultElement.getLastTestResult(strTemp);

        if(bNew){
            nHttpItem = CDevDataCfg::getHttpTestItemFromItem(static_cast<short>(testResultElement.m_nItem));
        }

        switch (testResultElement.m_nItem) {
        case TEST_ITEM_HEIGHT:
            if(bNew){
                mapItemRes.insert(nHttpItem, QString::fromStdString( strTemp ));
            }
            else{
                retInfo.insert( UPLOADBODYSCORE_KEY_HEIGHT, QString::fromStdString( strTemp ) );
            }

            fHeight = CStdString::getValue<float>(strTemp);
            shState |= 1;
            break;
        case TEST_ITEM_WEIGHT:
            if(bNew){
                mapItemRes.insert(nHttpItem, QString::fromStdString( strTemp ));
            }
            else {
                retInfo.insert( UPLOADBODYSCORE_KEY_WEIGHT, QString::fromStdString( strTemp ) );
            }

            fWeight = CStdString::getValue<float>(strTemp);
            shState |= 1 << 1;
            break;
        case TEST_ITEM_HBC:
            nIndexHBC = nIndexTemp;

            break;
        default:
            break;
        }

        nIndexTemp++;
    }

    //身高体重不存在返回false
    if(shState != 3)
        return false;

    //存在体成分数据再进行添加
    if(nIndexHBC >= 0)
    {
        if(bNew){
            addHBCFormatInfo(mapItemRes, vecTestResult[static_cast<unsigned int>(nIndexHBC)], fHeight, fWeight);
        }
        else {
            addHBCFormatInfo(retInfo, vecTestResult[static_cast<unsigned int>(nIndexHBC)], fHeight, fWeight);
        }
    }

    if(bNew)
    {
        QString qsRes = CHttpRequestMgr::getSerializationResList(mapItemRes);
        retInfo.insert( HTTP_KEYS::recordListStr, qsRes );
    }

    return true;
}

bool CTestGuideManager::convertEyeTestRes2ServerFormat(QMap<QString, QVariant> &retInfo, const vector<CTestResultInfo> &vecTestResult, bool bNew)
{
    string strValidResult;

    if(vecTestResult.size() != 1)
    {
        LOGERROR("");
        return false;
    }

    const CTestResultInfo &testResult = vecTestResult[0];
    addCommonServerFormatInfo(retInfo, testResult);

    //填入左眼和右眼视力
    QString leftEyeSight, rightEyeSight;

    string strRes;
    testResult.getLastTestResult(strRes);
    if(!CTestGuideEyesight::getDeserializationEyeightData(leftEyeSight, rightEyeSight, strRes))
    {
        LOGERROR("getDeserializationEyeightData false strRes = " + strRes);
        return false;
    }

    if(bNew){
        QMap<int, QString> mapItemRes;
        mapItemRes.insert( HTTP_TEST_ITEM_VISION_LEFT, leftEyeSight );
        mapItemRes.insert( HTTP_TEST_ITEM_VISION_RIGHT, rightEyeSight );

        QString qsRes = CHttpRequestMgr::getSerializationResList(mapItemRes);

        retInfo.insert( HTTP_KEYS::recordListStr, qsRes );
    }
    else {
        retInfo.insert( HTTP_KEYS::leftEyeSight,  leftEyeSight );
        retInfo.insert( HTTP_KEYS::rightEyeSight, rightEyeSight );
    }

    return true;
}

void CTestGuideManager::addCommonServerFormatInfo(QMap<QString, QVariant> &retInfo, const CTestResultInfo &testResult)
{
    //用户id
    retInfo.insert( HTTP_KEYS::userId,     QString::number( testResult.m_llUniqueID ));
    //设备id
    retInfo.insert( HTTP_KEYS::devCode,    QString::fromStdString( testResult.m_sDeviceID ) );

    //设备类型
    short shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testResult.m_nItem));
    retInfo.insert( HTTP_KEYS::devType,    CDevDataCfg::getHttpDevTypeFromType(shDevType));

    //正式测试或者非正式测试,   将本地的成绩类型转换为服务器的成绩类型
    short shTestNature = static_cast<short>(testResult.m_nNature);
    retInfo.insert( HTTP_KEYS::type,       QString::number( CTestGuideManager::convertC2STestNature(shTestNature)));

    QString log = QString( "map score type = " ) + retInfo.value( HTTP_KEYS::type ).toString();

    LOGINFO( log.toStdString() );

    //测试时间
    QString timeStamp = testResult.getTimeStamp(testResult.m_strStartTime);
    retInfo.insert( HTTP_KEYS::recordTime, timeStamp );

    //签字文件
    retInfo.insert( HTTP_KEYS::signPic, QString::fromStdString( testResult.m_strResSignature ));
}

void CTestGuideManager::addHBCFormatInfo(QMap<QString, QVariant> &retInfo, const CTestResultInfo &testResult, const float &fHeight, const float &fWeight)
{
    bool bGenger = true;
    CUserInfo userInfo;
    vector<CUserInfo> vecUserInfo;

    auto pTableUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
    if(pTableUserInfo == nullptr)
    {
        LOGERROR("pTableUserInfo == nullptr");
        return;
    }

    userInfo.m_llUniqueID = testResult.m_llUniqueID;
    pTableUserInfo->getUserInfo(userInfo, vecUserInfo);
    if(vecUserInfo.size() == 0)
    {
        LOGERROR("vecUserInfo.size() == 0");
        return;
    }

    bGenger = vecUserInfo[0].m_bGender;

    vector<short> vecAnalysisItem;
    CTestGuideBody::getHBCItem(vecAnalysisItem);
    vector<CAnalysisHBC::HBCComponentDes> vecRes;

    string strTemp;
    short shAge = CTestGuideBody::getHBCAge(&userInfo);
    testResult.getLastTestResult(strTemp);
    CTestGuideBody::getHBCTestRes(fHeight, fWeight, bGenger, shAge, strTemp, vecAnalysisItem, vecRes);
    QString qsTemp;

    auto itFind = s_mapImpedanceAndSetverValue.begin();
    for(auto &item : vecRes)
    {
        qsTemp = QString::number(item.lfResult, 'f', 2);

        itFind = s_mapImpedanceAndSetverValue.find(item.shAnalysisItem);
        if(itFind == s_mapImpedanceAndSetverValue.end())
        {
            LOGWARN("Set the corresponding human impedance value");
            continue;
        }

        retInfo.insert( itFind->second, qsTemp);
    }
}

void CTestGuideManager::addHBCFormatInfo(QMap<int, QString> &retInfo, const CTestResultInfo &testResult, const float &fHeight, const float &fWeight)
{
    bool bGenger = true;
    CUserInfo userInfo;
    vector<CUserInfo> vecUserInfo;

    auto pTableUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
    if(pTableUserInfo == nullptr)
    {
        LOGERROR("pTableUserInfo == nullptr");
        return;
    }

    userInfo.m_llUniqueID = testResult.m_llUniqueID;
    pTableUserInfo->getUserInfo(userInfo, vecUserInfo);
    if(vecUserInfo.size() == 0)
    {
        LOGERROR("vecUserInfo.size() == 0");
        return;
    }

    bGenger = vecUserInfo[0].m_bGender;

    vector<short> vecAnalysisItem;
    CTestGuideBody::getHBCItem(vecAnalysisItem);
    vector<CAnalysisHBC::HBCComponentDes> vecRes;

    string strTemp;
    short shAge = CTestGuideBody::getHBCAge(&userInfo);
    testResult.getLastTestResult(strTemp);
    CTestGuideBody::getHBCTestRes(fHeight, fWeight, bGenger, shAge, strTemp, vecAnalysisItem, vecRes);
    QString qsTemp;

    auto itFind = s_mapImpedanceAndSetverValueNew.begin();
    for(auto &item : vecRes)
    {
        qsTemp = QString::number(item.lfResult, 'f', 2);

        itFind = s_mapImpedanceAndSetverValueNew.find(item.shAnalysisItem);
        if(itFind == s_mapImpedanceAndSetverValueNew.end())
        {
            LOGWARN("Set the corresponding human impedance value");
            continue;
        }

        retInfo.insert( itFind->second, qsTemp);
    }
}

bool CTestGuideManager::saveTestResultToLoacl(vector<CTestResultInfo> &vecTestResult)
{
    bool bRet = false;

    CTableTestItemResult *pTestResult = nullptr;
    pTestResult = CDatabaseManage::GetInstance().getTabTestItemResult();
    if(pTestResult == nullptr)
    {
        LOGERROR("CTableTestItemResult == nullptr");
        return bRet;
    }

    bRet = pTestResult->addResultInfo(vecTestResult);

    return bRet;
}

bool CTestGuideManager::deleteDatabaseTestResult(const vector<CTestResultInfo> &vecTestResult)
{
    bool bRet = false;

    CTableTestItemResult *pTestResult = nullptr;
    pTestResult = CDatabaseManage::GetInstance().getTabTestItemResult();
    if(pTestResult == nullptr)
    {
        LOGERROR("CTableTestItemResult == nullptr");
        return bRet;
    }

    bRet = pTestResult->deleteResultInfo(vecTestResult);

    //删除对应的过程资源文件夹
    if(vecTestResult.size() != 0 && !vecTestResult[0].m_strPathResourceRoot.empty())
    {
        CFilePath::deleteFileOrFolder(vecTestResult[0].m_strPathResourceRoot);
    }

    return bRet;
}

//处理界面提交成功的消息，一次提交只会触发一次
//目前是根据最后一条数据提交成功与否判断
void CTestGuideManager::submitCommonSuccess()
{
    //0.判断当前连接模式
    bool bStation = !CDevDataCfg::getDevDirectConnectFlag();

    //1.组网模式
    if(bStation)
    {
        //1.1 同步模式
        if(CTestGuideBase::s_shTestMode == TEST_SYNC)
        {
            //1.1.1 设置所有状态为未测试
            CTestGuideBase* pTestGuide = nullptr;
            for(auto &element : m_mapUserInfo)
            {
                pTestGuide = getTestGuid(element.first.m_iSteDevMacID);
                if(nullptr != pTestGuide)
                {
                    pTestGuide->setTestState(CTestStateManage::TEST_NOT_START,element.first.m_shElementIndex);
                }
            }

            //1.1.2 解除绑定
            deleteAllBindInfo();

            //1.1.3 清空识别存储
            m_vecRecognitionSequence.clear();

            //1.1.4 清空UI界面显示
            m_ui.setTestState("");
            m_ui.cleanDataModel();

            //1.1.5 禁用提交按钮
            m_ui.setBtnEnable(CProjectEnums::OTM_SYNC_COMMIT_BTN, false);
        }
        //1.2 异步模式
        else
        {
            //1.2.1 清空UI界面显示
            m_ui.cleanDataModel();

            //1.2.2 禁用提交按钮
            m_ui.setBtnEnable(CProjectEnums::OTM_ASYNC_COMMIT_BTN, false);
        }
    }
    //2.直连模式
    else
    {
        //2.1 解除绑定
        deleteAllBindInfo();

        //2.2 清空UI界面显示
        m_ui.setTestState("");
        m_ui.setCurTestRet("");
        m_ui.clearModelData(CInterAction::COMMON_DATA_MODEL);

        //2.3 禁用提交按钮
        m_ui.setBtnEnable(CProjectEnums::OTO_COMMIT_BTN, false);
    }

    //3.清空完成结果
    m_vecCompleteTestRes.clear();
}

bool CTestGuideManager::actualStartElement(CTestGuideBase *pTestGuid, const CLocation &location, bool bSpeak)
{
    if(pTestGuid == nullptr)
    {
        LOGERROR("pTestGuid == nullptr");
        return false;
    }

    //安全保护，当前需要在测试界面才行
    if(!CDevDataCfg::isTestPage(m_ui.getCurPage(), CTestGuideBase::s_shTestMode))
    {
        LOGERROR("curpage is : " + to_string(m_ui.getCurPage()) + "which not belong to test page");
        return false;
    }

    const short &shDevType = pTestGuid->getDevType();
    switch (shDevType) {
    case DEV_TYPE_RUN_MID:
    {
        //中长跑语音在外部进行，这里只处理通用的语音
        bSpeak = false;

        //中长跑在这里触发开始
        pTestGuid->procTestBegin(location.m_shElementIndex);

        auto itFindUserInfo = m_mapUserInfo.find(location);
        if(itFindUserInfo == m_mapUserInfo.end())
        {
            LOGERROR("m_mapUserInfo find false = " + to_string(location.m_iSteDevMacID) + " shelementindex = " + to_string(location.m_shElementIndex));
            return false;
        }

        CTestGuideRunMid *pDevRunMid = reinterpret_cast<CTestGuideRunMid *>(pTestGuid);
        CUserInfo &userInfo = itFindUserInfo->second;

        if(!pDevRunMid->getActualStartTime(location.m_shElementIndex, userInfo.m_llTestActualStartTime))
        {
            LOGERROR("getActualStartTime false devID" + to_string(location.m_iSteDevMacID) + " shelementindex = " + to_string(location.m_shElementIndex));
        }

        userInfo.m_bCheckTime = true;
    }
        break;
    case DEV_TYPE_ROPE_SKIP:
        break;
    default:
        break;
    }

    bool bSendToUi = true;
    //通用语音开始测试(坐位体前屈在直连的时候不需要进行语音播报)(真正的语音播报在测试状态变更中的开始信号位置处)
    if(shDevType == DEV_TYPE_SFB && CDevDataCfg::getDevDirectConnectFlag())
    {
        bSendToUi = bSpeak = false;
    }

    pTestGuid->setTestState(CTestStateManage::TEST_CHECKED, location.m_shElementIndex, bSpeak, false, false);

    //通用设置开始状态
    pTestGuid->setTestState(CTestStateManage::TEST_IN_PROCESS, location.m_shElementIndex, false, false, bSendToUi);

    return true;
}

bool CTestGuideManager::actualStartAllElement(CTestGuideBase *pTestGuid, bool bSpeak)
{
    if(pTestGuid == nullptr)
    {
        LOGERROR("pTestGuid == nullptr");
        return false;
    }

    auto pDev = pTestGuid->getSteDev();
    if(pDev == nullptr)
    {
        LOGERROR("pDev == nullptr");
        return false;
    }

    //可以从设备中找到所有已经绑定的测试单元 可以从绑定的人员中找到所有的绑定测试单元视情况而定
    bool bRet = true;
    CLocation location(pDev->getMacID(), 0);

    bool bSpeakTemp;
    auto vecBindElementIndex = pTestGuid->getAllBindElementIndex();

    for(const auto &index : vecBindElementIndex)
    {
        location.m_shElementIndex = index;

        //最后一个开启语音才进行语音播报
        bSpeakTemp = bSpeak && &index == &vecBindElementIndex.back();
        bRet = actualStartElement(pTestGuid, location, bSpeakTemp);

        if(!bRet)
        {
            LOGERROR("actualStartElement return false dev id = " + to_string(location.m_iSteDevMacID) + "elementindex = " + to_string(index));
            break;
        }
    }

    return bRet;
}

//这里需要根据实际开启的设备进行设置（m_mapTestGuide。size = 3 实际为2）
bool CTestGuideManager::actualStartCast(bool bSpeak)
{
    map<unsigned int, CTestGuideBase*> mapTemp;
    for(auto &element : m_mapTestGuide)
    {
        if(element.second->getActiveState())
        {
            mapTemp[element.first] = element.second;
        }
    }

    size_t szSize = mapTemp.size();
    size_t i = 0;

    bool bSpeakTemp;
    bool bRet = true;

    for(auto &element : mapTemp)
    {
        bSpeakTemp = bSpeak && i == szSize - 1;
        bRet = actualStartAllElement(element.second, bSpeakTemp);
        if(!bRet)
        {
            LOGERROR("actualStartAllElement return false dev id = " + to_string(element.second->getSteDev()->getMacID()));
            break;
        }
        i++;
    }

    return bRet;
}

void CTestGuideManager::initRopeSkipDelayTime()
{
    int iTime = 0;
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->getRopeDelayTime(iTime))
    {
        iTime = 20;
        LOGERROR("数据库获取默认配置失败，延时时间默认为20")
    }

    setRopeSkipDelayTime(iTime);
}

map<short, QString> CTestGuideManager::initImpedanceAndSetverValue()
{
    map<short, QString> mapTemp;
    mapTemp[CAnalysisHBC::HBC_BMI]              = UPLOADBODYSCORE_KEY_BMI;
    mapTemp[CAnalysisHBC::HBC_BODY_FAT_PERCENT] = UPLOADBODYSCORE_KEY_FARATE;
    mapTemp[CAnalysisHBC::HBC_HEAD_WEIGHT]      = UPLOADBODYSCORE_KEY_HEADWEIGHT;
    mapTemp[CAnalysisHBC::HBC_HEAD_FAT]         = UPLOADBODYSCORE_KEY_HEADFAT;
    mapTemp[CAnalysisHBC::HBC_HEAD_MUSCLE]      = UPLOADBODYSCORE_KEY_HEADMUSCLE;
    mapTemp[CAnalysisHBC::HBC_TBMM]             = UPLOADBODYSCORE_KEY_TBMM;
    mapTemp[CAnalysisHBC::HBC_WBIS_]            = UPLOADBODYSCORE_KEY_WBIS;
    mapTemp[CAnalysisHBC::HBC_TBW]              = UPLOADBODYSCORE_KEY_TBW;
    mapTemp[CAnalysisHBC::HBC_WHR]              = UPLOADBODYSCORE_KEY_WHR;
    mapTemp[CAnalysisHBC::HBC_VFA]              = UPLOADBODYSCORE_KEY_VFA;

    return mapTemp;
}

map<short, int> CTestGuideManager::initImpedanceAndSetverValueNew()
{
    map<short, int> mapTemp;
    mapTemp[CAnalysisHBC::HBC_BMI]              = TEST_TEST_ITEM_BMI;
    mapTemp[CAnalysisHBC::HBC_BODY_FAT_PERCENT] = TEST_TEST_ITEM_FAT_PERCENT;
    mapTemp[CAnalysisHBC::HBC_HEAD_WEIGHT]      = TEST_TEST_ITEM_HEAD_WEIGHT;
    mapTemp[CAnalysisHBC::HBC_HEAD_FAT]         = TEST_TEST_ITEM_HEAD_FAT;
    mapTemp[CAnalysisHBC::HBC_HEAD_MUSCLE]      = TEST_TEST_ITEM_HEAD_MUSCLE;
    mapTemp[CAnalysisHBC::HBC_TBMM]             = TEST_TEST_ITEM_TBMM;
    mapTemp[CAnalysisHBC::HBC_WBIS_]            = TEST_TEST_ITEM_WBIS;
    mapTemp[CAnalysisHBC::HBC_TBW]              = TEST_TEST_ITEM_TBW;
    mapTemp[CAnalysisHBC::HBC_WHR]              = TEST_TEST_ITEM_WHR;
    mapTemp[CAnalysisHBC::HBC_VFA]              = TEST_TEST_ITEM_VFA;

    return mapTemp;
}

void CTestGuideManager::deleteDisconnUserInfo(const CUserInfo &userInfo)
{
    for(auto it = m_mapUserInfoDisconn.begin(); it != m_mapUserInfoDisconn.end(); it++)
    {
        if(it->second.m_llUniqueID == userInfo.m_llUniqueID)
        {
            //删除对应的绑定信息，主要是设备中的绑定信息
            auto pTestGuide = getTestGuid(it->first.m_iSteDevMacID);
            if(pTestGuide != nullptr)
            {
                pTestGuide->unbindUserToElement(it->first.m_shElementIndex);
            }

            m_mapUserInfoDisconn.erase(it);
            break;
        }
    }
}

void CTestGuideManager::rebindDisconnUser(const unsigned int &nDevId)
{
    auto pDev = m_objSteDevMgr.getDevSteByID(nDevId);
    if(pDev == nullptr)
        return;

    //bind会变更m_mapUserInfoDisconn中的消息
    auto mapTemp = m_mapUserInfoDisconn;

    for(auto it = mapTemp.begin(); it != mapTemp.end();)
    {
        if(it->first.m_iSteDevMacID != nDevId)
        {
            it++;
            continue;
        }

        if(bindUserInfo(it->second, pDev->getDevType()) == SUCESS_BIND)
        {
            it = mapTemp.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void CTestGuideManager::getTemporaryRes(vector<CCompleteTestResult> &vec)
{
    vec.clear();
    for(auto &element : m_mapTestGuide)
    {
        auto &pTestGuid = element.second;
        if(pTestGuid == nullptr)
            continue;

        element.second->getTemporaryRes(vec, false);
    }
}

bool CTestGuideManager::isTestResInCurCompletedTestList(const vector<CTestResultInfo> &vecSingleTest)
{
    bool bRet = false;
    if(vecSingleTest.size() == 0)
        return false;

    const CTestResultInfo &testResult = vecSingleTest[0];

    for(auto &element : m_vecCompleteTestRes)
    {
        //粗略判断人员时间一致就是在列表中含有的
        if(element.m_vecTestResInfo.size() == 0)
            continue;

        const CTestResultInfo &testResultTemp = element.m_vecTestResInfo[0];

        if(testResult.m_llUniqueID == testResultTemp.m_llUniqueID || testResult.m_strStartTime == testResultTemp.m_strStartTime)
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

void CTestGuideManager::forceExitTest()
{
    for(auto &element : m_mapTestGuide)
    {
        auto &pTestGuid = element.second;
        if(pTestGuid == nullptr)
            continue;

        element.second->forceExitTest();
    }
}

void CTestGuideManager::changeDataBseScoreFlag(CMultiUploadRetInfo &info)
{
    QString userId = info.userId();
    QString timeStamp = info.timeStamp();

    //若当前是身高体重，需要更新 身高体重和人体成分的标志
    int nTestItem = info.testItem();
    short shTestItem = CDevDataCfg::getTestItemFromHttpItem(nTestItem);
    short shDevType = CDevDataCfg::getDevTypeByTestItem(shTestItem);

    if( shDevType == DEV_TYPE_BODY ){
        updateTestResultSyncFlag( userId, TEST_ITEM_HEIGHT, timeStamp );
        updateTestResultSyncFlag( userId, TEST_ITEM_WEIGHT, timeStamp );
        updateTestResultSyncFlag( userId, TEST_ITEM_HBC, timeStamp );
    }else{
        updateTestResultSyncFlag( userId, shTestItem, timeStamp );
    }
}

//发令枪的触发函数
void CTestGuideManager::onStartingGun()
{
    if(m_bStartTestReady)
    {
        m_bStartTestReady = false;
        if(m_objSteDevMgr.getType() == DEV_TYPE_RUN_SHORT)
        {
            startRunShortSyncByStartingGun();
        }
        else if (m_objSteDevMgr.getType() == DEV_TYPE_RUN_MID)
        {
            //更新同步开始的时间
            CTestGuideRunMid::upDateSyncActualTime();
            actualStartCast(false);
        }
        else if (m_objSteDevMgr.getType() == DEV_TYPE_ROPE_SKIP)
        {
            startRopeSkipSyncByStartingGun();
        }
        else
        {
            LOGERROR("Invalid DevType For StartingGun");
        }
    }
}

void CTestGuideManager::onTestItemChanged()
{
    for(auto &element : m_mapTestGuide)
    {
        auto &pTestGuid = element.second;
        if(pTestGuid != nullptr)
        {
            //中止测试流程：清空列表信息等 （匹配短跑原有逻辑）
            cancelTest(false,true,false);

            //切换测试项目
            pTestGuid->onTestItemChanged();
        }
    }
}

QString CTestGuideManager::variantMap2Json(const QVariantMap &srcMap)
{
    static QJsonDocument jsDoc;

    jsDoc = QJsonDocument::fromVariant( srcMap );

    return QString::fromUtf8( jsDoc.toJson() );
}

void CTestGuideManager::setLastBindInfo(const CUserInfo &userInfo, const CLocation *pLocation)
{
    m_iLastbindDevNo = -1;
    m_shLastBindDevType = DEV_TYPE_UNKNOW;

    if(!userInfo.isTouristInfo() && pLocation != nullptr)
    {
        auto pDev = m_objSteDevMgr.getDevSteByID(pLocation->m_iSteDevMacID);

        //跳绳和中长跑不会用到当前设备，仅短跑和其他通用设备会使用到
        if(pDev != nullptr)
        {
            m_shLastBindDevType = pDev->getDevType();
            if(m_shLastBindDevType != DEV_TYPE_RUN_SHORT)
            {
                m_iLastbindDevNo = pDev->getExtensionNo();
            }
            else
            {
                m_iLastbindDevNo = pLocation->m_shElementIndex + 1;
            }
        }
    }
}

bool CTestGuideManager::isDelTestRes(const CUserInfo &user)
{
    bool bRet = false;
    for(auto &element : m_vecDeleteTestRes)
    {
        if(element == user)
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

void CTestGuideManager::addCompletTestResToCompletContent(const CLocation &location, short shTestMode, const CCompleteTestResult &completeRes)
{
    if(completeRes.m_vecTestResInfo.size() == 0)
        return;

    bool bAppend = true;

    QStringList listContent;
    //异步模式需要判断成绩的好坏然后用好的成绩替换老成绩
    if(shTestMode == TEST_ASYNC)
    {
        bool bReplace, bExist = false;
        //判断结果中有无对应的人员成绩
        for(auto &existRes : m_vecCompleteTestRes)
        {
            if(completeRes.m_userInfo == existRes.m_userInfo)
            {
                bExist = true;
                //判定哪个成绩更好,一般情况对比第一个即可，同时这里成绩是肯定有效的
                bReplace = CTestResultInfo::compare(completeRes.m_vecTestResInfo[0], existRes.m_vecTestResInfo[0]) > 0;

                //删除现在的或者以前的测试完成成绩
                if(bReplace)
                {
                    const CUserInfo &userInfo = completeRes.m_userInfo;
                    deleteDatabaseTestResult(existRes.m_vecTestResInfo);
                    existRes = completeRes;

                    //在公共表格中更新成绩(以前的异步状态完成之后需要将状态显示为成绩，后来做中考感觉不需要显示成绩，保留完成状态即可)
                    listContent = CDevDataCfg::getTableContent(location, shTestMode, userInfo, completeRes.m_vecTestResInfo, /*true*/false);

                    //找到表格中相同的用户id
                    short shUserIDIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, m_objSteDevMgr.getType(), shTestMode);

                    if(shUserIDIndex >= 0)
                        m_ui.updateModelRowData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, userInfo.m_strStudentId, listContent);
                }
                else
                {
                    deleteDatabaseTestResult(completeRes.m_vecTestResInfo);
                }

                bAppend = false;
                break;
            }
        }

        if(!bExist)
        {
            //将成绩添加到公共表格末尾
            const CUserInfo &userInfo = completeRes.m_userInfo;
            listContent = CDevDataCfg::getTableContent(location, shTestMode, userInfo, completeRes.m_vecTestResInfo, true);
            m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, listContent);
        }
    }

    if(bAppend)
        m_vecCompleteTestRes.push_back(completeRes);
}

unsigned int CTestGuideManager::simulationRunMidCard()
{
    m_bExtraBindFlag = true;
    static unsigned int nTemp = 2200;
    return nTemp++;
}

void CTestGuideManager::uploadTestRestltToServer()
{
    vector<vector<CTestResultInfo>> vecTemp, vecDel;
    for(auto &element : m_vecCompleteTestRes)
    {
        if(!isDelTestRes(element.m_userInfo))
            vecTemp.push_back(element.m_vecTestResInfo);
        else
            vecDel.push_back(element.m_vecTestResInfo);
    }

    m_vecCompleteTestRes.clear();
    //删除在提交界面的取消成绩对应的数据
    for(auto &del : vecDel)
    {
        //删除本地数据库成绩
        deleteDatabaseTestResult(del);
    }

    uploadTestResult(vecTemp, m_pReqeuestMgr, UserExam);
}

void CTestGuideManager::onUploadCommonScoreFinished(QVariant data)
{
    CHttpUploadInfos infos = CHttpRequestMgr::Variant2UploadInfos( data );
    dealUploadRetValue(infos);
}

void CTestGuideManager::onUploadBodyScoreFinished(QVariant data)
{
    CHttpUploadInfos infos = CHttpRequestMgr::Variant2UploadInfos( data );
    dealUploadRetValue(infos);
}

void CTestGuideManager::onUploadEyeSightScoreFinished(QVariant data)
{
    CHttpUploadInfos infos = CHttpRequestMgr::Variant2UploadInfos( data );
    dealUploadRetValue(infos);
}

void CTestGuideManager::onUploadMulityScoreFinished(QVector<CMultiUploadRetInfo> totalSyncScoreInfos)
{
    //全部成功的标志，
    bool ok = true;

    QString userId = "";
    QString uploadSuccess = "true";
    QString timeStamp = "";
    QString testItem = "";
    QString errorCode = "";
    QString errorString = "";

    //同一组数据的数据源必定是一样的
    DATA_SRC dataSrc = totalSyncScoreInfos.first().scoreDataSrc();
    QString dataSrcStr = dataSrc == UserExam ? "UserExam" : "DataBase";

    QString uploadType = "Common";
    QString logInfo = QString( "upload or sync score info: userId = %1, testItem = %2, recordTime = %3, score src = %4, errorCode = %5, errorMsg = %6" );

    for( CMultiUploadRetInfo &ele : totalSyncScoreInfos ){

        //如果存在上传失败的项目, 就把总的成功失败标志置为false
        if( !ele.upLoadSuccess() && ele.errorCode() != QString::number( SUCCESS ) ){
            ok = false;

            //非正确上传时，在软件中进行记录，若失败错误不为超时，同时上传了两次，那么将对应项目标记为上传成功
            //同时将错误上传的数据在日志中进行打印

        }else{
            //如果成功, 就更改该条成绩在数据库中的标志
            changeDataBseScoreFlag( ele );
        }

        userId = ele.userId();
        uploadSuccess = ele.upLoadSuccess() ? "true" : "false";
        errorCode = ele.errorCode();
        errorString = ele.errorMsg();
        testItem = QString::number( ele.testItem() );
        uploadType = QString::number( ele.UploadType() );
        timeStamp = QString::fromStdString( CTestResultInfo::getSaveTimeStamp( ele.timeStamp() ) );
        logInfo = logInfo.arg( userId ).arg( testItem ).arg( timeStamp ).arg( dataSrcStr ).arg( errorCode ).arg( errorString );
        #if QT_NO_DEBUG
                LOGINFO( logInfo.insert( 0, "upload common score info = " ).toStdString() );
        #else
                qDebug() << logInfo.insert( 0, "upload common score info = " );
        #endif
    }

    //暂定默认全部成功
    ok = true;

    if( dataSrc == UserExam ){
        //进行界面操作
        LOGINFO( "即将关闭批量提交弹窗..." );
        m_ui.setUpdateCurTestRet( ok );

        if( !ok ){
            m_ui.setErrorString( m_pReqeuestMgr->getErrorString() );
        }else{
            submitCommonSuccess();
        }
    }
}

//这里只是在同步测试时才进行触发处理，同时只判断是否完成若完成设置测试状态为完成状态
void CTestGuideManager::onDevTestStateChange()
{
    //同步模式相关设置
    if(CTestGuideBase::s_shTestMode == TEST_SYNC)
    {
        short shTestState = m_objSteDevMgr.getTestStatus();

        if(shTestState == CTestStateManage::TEST_COMPLETED)
        {
            m_ui.setTestState( "测试完成" );
        }

        //灯光设置
        unsigned short shMode = CDevHardwareBase::convertTestState2LightState(shTestState);

        auto pPower = CAssistDevManage::getDevPowerUart();
        if(pPower != nullptr)
        {
            pPower->setLightBelt(shMode);
        }
    }
}

void CTestGuideManager::dispTestResult()
{
    m_objSpeak.stop();

    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        dispTestResultForOto();
    }
    else
    {
        dispTestRestltForOtm();
    }
}

bool CTestGuideManager::checkDeviceAdded(const unsigned int &nDevId)
{
    auto pTestGuid = getTestGuid(nDevId);
    return pTestGuid != nullptr && pTestGuid->getActiveState();
}

void CTestGuideManager::onCommitPersentChanged(QString val)
{
    qDebug() << "commit persent = " << val;
    m_ui.setCommitPersent( val );
}

void CTestGuideManager::onUiNFCValidTimeout()
{
    //界面提前结束绑定时间
    //m_bExtraTimeFlag = true;
}

void CTestGuideManager::onCancelNFCBind()
{
    //界面取消绑定过程
    m_bExtraUnBindFlag = true;
}

void CTestGuideManager::onUnbindUser(QString qsStudentID)
{
    cancelUserBind(qsStudentID);
}

void CTestGuideManager::onDevErrStateChange()
{
    //获取所有设备是否正常
    bool bNormal = true;
    for(auto &element : m_mapTestGuide)
    {
        auto &pTestGuid = element.second;
        //只获取有效的测试指导进行判断
        if(pTestGuid == nullptr || !pTestGuid->getActiveState())
            continue;

        if(element.second->existErrInBindElement())
        {
            bNormal = false;
        }
    }

    if(bNormal)
    {
        LOGINFO("&&&&&&&&&&&&&&&333");
        CTestGuideBase::setDevReadyFlag(true);
    }
}

void CTestGuideManager::onDevDisconn(const CLocation &location)
{
    auto itFind = m_mapUserInfo.find(location);
    if(itFind == m_mapUserInfo.end())
        return;

    m_mapUserInfoDisconn[location] = itFind->second;
}

void CTestGuideManager::deleteBindInfoByDevID(unsigned int devID)
{
    CTestGuideBase *pTestGuide = nullptr;

    for(auto it = m_mapUserInfo.begin(); it != m_mapUserInfo.end();)
    {
        if(it->first.m_iSteDevMacID == devID)
        {
            pTestGuide = getTestGuid(devID);
            if(pTestGuide != nullptr)
            {
                pTestGuide->unbindUserToElement(it->first.m_shElementIndex);
            }

            deleteRecognitionByLocation(it->first);
            it = m_mapUserInfo.erase(it);
        }
        else
        {
            it++;
        }
    }

    updateBindUserNum();
}

void CTestGuideManager::deleteBindInfoByUser(const CUserInfo &userInfo, int nDevType)
{
    LOGWARN("deleteBindInfoByUser user ID = " + to_string(userInfo.m_llUniqueID) + " name =" + userInfo.m_strName.toStdString());

    if(m_mapUserInfo.size() == 0)
    {
        m_vecRecognitionSequence.clear();
        return;
    }

    bool bMatchingType = nDevType != DEV_TYPE_UNKNOW;
    CTestGuideBase *pTestGuide = nullptr;

    for(auto it = m_mapUserInfo.begin(); it != m_mapUserInfo.end();)
    {
        if(it->second.m_llUniqueID != userInfo.m_llUniqueID)
        {
            it++;
            continue;
        }

        pTestGuide = getTestGuid(it->first.m_iSteDevMacID);
        if(pTestGuide == nullptr)
        {
            it++;
            continue;
        }

        if(bMatchingType)
        {
            if(pTestGuide->getDevType() != nDevType)
            {
                it++;
                continue;
            }
        }

        pTestGuide->unbindUserToElement(it->first.m_shElementIndex);

        deleteRecognitionByLocation(it->first);
        it = m_mapUserInfo.erase(it);
        //删除识别顺序
        break;
    }

    updateBindUserNum();
}

void CTestGuideManager::deleteBindInfoByLocation(const CLocation &location)
{
    auto itFind = m_mapUserInfo.find(location);
    if(itFind != m_mapUserInfo.end())
    {
        LOGWARN("deleteBindInfoByUser user ID = " + to_string(itFind->second.m_llUniqueID) + " name =" + itFind->second.m_strName.toStdString());

        deleteRecognitionByLocation(location);
        m_mapUserInfo.erase(itFind);
    }

    CTestGuideBase *pTestGuid = getTestGuid(location.m_iSteDevMacID);
    if(pTestGuid != nullptr)
    {
        pTestGuid->unbindUserToElement(location.m_shElementIndex);
    }

    updateBindUserNum();
}

void CTestGuideManager::deleteAllBindInfo()
{
    CTestGuideBase *pTestGuid;
    for(auto &element : m_mapUserInfo)
    {
        pTestGuid = getTestGuid(element.first.m_iSteDevMacID);
        if(pTestGuid != nullptr)
        {
            pTestGuid->unbindUserToElement(element.first.m_shElementIndex);
        }
    }

    m_mapUserInfo.clear();
    m_vecRecognitionSequence.clear();

    updateBindUserNum();
}

void CTestGuideManager::deleteRecognitionByLocation(const CLocation &location)
{
    for(unsigned int i = 0; i < m_vecRecognitionSequence.size(); i++)
    {
        if(*m_vecRecognitionSequence[i].m_pLocation == location)
        {
            m_vecRecognitionSequence.erase(m_vecRecognitionSequence.begin() + i);
            break;
        }
    }
}

void CTestGuideManager::deleteTableByLocation(const int &iTableType, const CLocation &location)
{
    //通过 location 找到对应的 UserInfo
    auto itFind = m_mapUserInfo.find(location);
    if(itFind == m_mapUserInfo.end())
    {
        LOGERROR("location is not exist!");
        return;
    }

    //得到 唯一的学生ID 设备类型
    QString qsStudentID = itFind->second.m_strStudentId;
    short   shDevType   = itFind->second.m_shDevType;

    //得到表格判断列数
    short shUserIDIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, CTestGuideBase::s_shTestMode);

    //删除对应信息
    m_ui.deleteModelData(iTableType,shUserIDIndex,qsStudentID);
}

void CTestGuideManager::deleteResultByLocation(const CLocation &location, bool bDeleteSQL/* = false*/)
{
    //通过 location 找到对应的 UserInfo
    auto itFind = m_mapUserInfo.find(location);
    if(itFind == m_mapUserInfo.end())
    {
        LOGERROR("location is not exist!");
        return;
    }

    //通过 UserInfo 找到对应的结果信息
    for(unsigned int i = 0; i < m_vecCompleteTestRes.size(); i++)
    {
        if(m_vecCompleteTestRes[i].m_userInfo.m_llUniqueID == itFind->second.m_llUniqueID)
        {
            //删除数据库中数据
            if(bDeleteSQL)
            {
                deleteDatabaseTestResult(m_vecCompleteTestRes[i].m_vecTestResInfo);
            }

            //删除对应存储
            m_vecCompleteTestRes.erase(m_vecCompleteTestRes.begin() + i);
            break;
        }
    }
}

void CTestGuideManager::onRunMidSwipingCard(unsigned int nCardID)
{
    //卡号保护
    if(nCardID == 0)
    {
        return;
    }

    m_bExtraBindFlag = true;
    m_nExtraInfoID = nCardID;
}

short CTestGuideManager::getLocationTestState(const CLocation &location)
{
    short shTestRes = CTestStateManage::TEST_ERR;

    CDevHardwareBase *pDev = m_objSteDevMgr.getDevSteByID(location.m_iSteDevMacID);
    if(pDev == nullptr)
        return shTestRes;

    CSubElementBase *pElement = pDev->getElement(location.m_shElementIndex);
    if(pElement == nullptr)
        return shTestRes;

    shTestRes = pElement->getTestState();

    return shTestRes;
}

//只有在界面设置保存成功后，才会进入此函数
void CTestGuideManager::setRopeSkipDelayTime(const int &iDelayTime)
{
    //设置延时时间
    m_iDelayTime = iDelayTime;

    LOGINFO("Set Rope Skip Delay Time :" + to_string(m_iDelayTime));
}

//取消绑定消息，接收到的是准考证号
void CTestGuideManager::cancelUserBind(QString qsStudentID)
{
    //0.根据发送的用户ID，查找是否存在符合的用户
    bool bFind = false;
    CLocation location;
    CUserInfo userInfo;
    for(auto iter = m_mapUserInfo.begin(); iter != m_mapUserInfo.end(); iter++)
    {
        if(iter->second.m_strStudentId == qsStudentID)
        {
            bFind = true;
            location = iter->first;
            userInfo = iter->second;
        }
    }

    if(bFind)
    {
        //异步模式是自动启动，因此需要优先取消测试
        if(CTestGuideBase::s_shTestMode == TEST_ASYNC)
        {
            cancelTest(location,true);
        }

        deleteBindInfoByUser(userInfo);
    }

    m_ui.deleteModelData(CInterAction::COMMON_DATA_MODEL,2,qsStudentID);
}

bool CTestGuideManager::uploadTestResult(const vector<vector<CTestResultInfo> > &vecTestResult, CHttpRequestMgr *pReqeuestMgr, DATA_SRC src)
{
    if(pReqeuestMgr == nullptr)
        return false;

    QMap<QString, QVariant> info;
    QVector<QMap<QString, QVariant> > vecInfosBody, vecInfosCommon, vecEyesight;
    QVector<QMap<QString, QVariant>> *pVecTemp;

    QVariantList listInfosBody, listInfosCommon, listEyesight;
    QVariantList *pListTemp;

    //批量上传成绩存在上传不同项目上传的项目不正确的问题，上传成绩时进行区分,身高体重和视力有专门的上传结构，不需要进行区分
    //区分的是其他的公共上传， map中的int为http的项目，后面的为区分项目之后的listInfosCommon
    map<int, QVariantList> mapInfosCommon;
    map<int, QVariantList>::iterator itFInd;

    short shDevType;
    bool bSucess;
    int nHttpItem = HTTP_TEST_ITEM_UNKNOW;
    //2022.12.01发现上传成绩存在bug，不能不同项目上传
    //
    bool bUsebulkSubmit = s_bUsebulkSubmit;

    //成都市公安局使用老版本提交
    int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
    if(nSoftVersion == soft::VERSION_CD_PSB)
    {
        bUsebulkSubmit = false;
    }

    for( auto &singleTestResult : vecTestResult )
    {
        nHttpItem = HTTP_TEST_ITEM_UNKNOW;

        if(singleTestResult.size() < 1)
            continue;

        if(singleTestResult[0].m_llUniqueID < 0)
            continue;

        //若仅仅是记录数据将数据直接标记为已经上传
        if(singleTestResult[0].isLogData())
        {
            updateTestResultSyncFlag(singleTestResult);
            continue;
        }

        //若是后台发送的包含在当前测试实验中完成的那么不进行上传
        if(src == DataBase)
        {
            if(isTestResInCurCompletedTestList(singleTestResult))
                continue;
        }

        //确认本次设备类型
        shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(singleTestResult[0].m_nItem));
        if(shDevType == DEV_TYPE_UNKNOW)
        {
            //数据存在问题，需要在原始结构中进行删除，数据库或者原始测试数据,为安全只设置同步标志为true
            LOGERROR( "DEVTYPE is DEV_TYPE_UNKNOW" );
            updateTestResultSyncFlag(singleTestResult);
            continue;
        }

        switch (shDevType) {
        case DEV_TYPE_BODY:
            bSucess = convertBodyTestRes2ServerFormat(info, singleTestResult, bUsebulkSubmit);
            pVecTemp = &vecInfosBody;
            pListTemp = &listInfosBody;
            break;

        case DEV_TYPE_EYESIGHT:
            bSucess = convertEyeTestRes2ServerFormat(info, singleTestResult, bUsebulkSubmit);
            pVecTemp = &vecEyesight;
            pListTemp = &listEyesight;
            break;

        default:
            nHttpItem = convertCommonTestRes2ServerFormat(info, singleTestResult, bUsebulkSubmit);
            bSucess = nHttpItem != HTTP_DEV_TYPE_UNKNOW;
            pVecTemp = &vecInfosCommon;
            pListTemp = &listInfosCommon;
            break;
        }

        //将所有存在问题结果都设置为同步成功
        if(!bSucess){
            updateTestResultSyncFlag(singleTestResult);
        }else{
            (*pVecTemp) << info;

            //这里面对应身高体重
            if(nHttpItem == HTTP_DEV_TYPE_UNKNOW)
            {
                (*pListTemp) << info;
            }
            //这里是类似中长跑的 800m和1000m项目存储
            else
            {
                itFInd = mapInfosCommon.find(nHttpItem);
                if(itFInd == mapInfosCommon.end())
                {
                    mapInfosCommon[nHttpItem] = listInfosCommon;
                    mapInfosCommon[nHttpItem] << info;
                }
                else
                {
                    itFInd->second << info;
                }
            }
        }
    }

    //没有需要上传的直接返回true;
    if( vecInfosBody.isEmpty() && vecInfosCommon.isEmpty() && vecEyesight.isEmpty() )
    {
        if(src == UserExam)
        {
            CInterAction::Get_Instance().setUpdateCurTestRet( false );
        }
        return true;
    }

    //src = UserExam;
    if(bUsebulkSubmit)
    {
        if( src == DataBase ){
            listInfosCommon.clear();
            //若有身高体重和视力先传输
            pReqeuestMgr->syncMultiLocalScores( listInfosCommon, listInfosBody, listEyesight );

            //再传输不同的项目
            listInfosBody.clear();
            listEyesight.clear();
            for(auto &element : mapInfosCommon)
            {
                pReqeuestMgr->syncMultiLocalScores( element.second, listInfosBody, listEyesight );
            }
        }else{
            pReqeuestMgr->uploadScore(DEV_TYPE_BODY, listInfosBody, src);
            pReqeuestMgr->uploadScore(DEV_TYPE_EYESIGHT, listEyesight, src);

            for(auto &element : mapInfosCommon)
            {
                pReqeuestMgr->uploadScore(DEV_TYPE_UNKNOW, element.second, src);
            }
        }
    }
    else
    {
        pReqeuestMgr->uploadScore(DEV_TYPE_BODY, vecInfosBody, src);
        pReqeuestMgr->uploadScore(DEV_TYPE_EYESIGHT, vecEyesight, src);
        pReqeuestMgr->uploadScore(DEV_TYPE_UNKNOW, vecInfosCommon, src);
    }

    return true;
}
