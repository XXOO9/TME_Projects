///////////////////////////////////////////////////////////
//  CTestGuideBase.cpp
//  Implementation of the Class CTestGuideBase
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////
#include "CTestGuideBase.h"
#include "CFilePath.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "AssistDevManager/CAssistDevManage.h"
#include "CTestGuideRunMid.h"
#include "CTestGuideFootball.h"
#include "CDevDataCfg.h"
#include "CTestGuideBase.h"
#include "CProjectEnums.h"
#include "CTestGuideEyesight.h"
#include "CDatabaseManage.h"
#include "AssistDevManager/FoulControl/CDevFoulControl.h"

using namespace  TestTableCfg;
using namespace FOUL_DEV_CMD;

QDateTime CTestGuideBase::s_datetimeStartTest;
string CTestGuideBase::s_strPathTestProRes;
CScoreRuleInfo CTestGuideBase::s_objScoreRuleInfo;

bool CTestGuideBase::s_bDevReady;
atomic<bool> CTestGuideBase::s_bContinueTest;
short CTestGuideBase::s_shTestMode;

CElementInfo::CElementInfo()
{

}

CTestGuideBase::CTestGuideBase()
{
    CTestGuideBase::s_strPathTestProRes = CTestGuideBase::getResPath();
    connect(&m_timerResendStartCmd, &QTimer::timeout, this, &CTestGuideBase::onResendCmdTimeOut);
    connect(&m_timerFoul, &QTimer::timeout, this, &CTestGuideBase::onTimeOutRemoveFoul);
    //存在两个同名函数使用老式信号连接方式）
    connect(this, SIGNAL(signaldealTestProc(const unsigned short &, const bool &, const bool &))
            , this, SLOT(startTest(const unsigned short &, const bool &, const bool &)), Qt::ConnectionType::QueuedConnection);
}

CTestGuideBase::~CTestGuideBase()
{
    disconnectSignal();

    setActiveState(false);

    disconnect(&m_timerResendStartCmd, &QTimer::timeout, this, &CTestGuideBase::onResendCmdTimeOut);
    disconnect(&m_timerFoul, &QTimer::timeout, this, &CTestGuideBase::onTimeOutRemoveFoul);
    //存在两个同名函数使用老式信号连接方式
    disconnect(this, SIGNAL(signaldealTestProc(const unsigned short &, const bool &, const bool &))
            , this, SLOT(startTest(const unsigned short &, const bool &, const bool &)));
}

void CTestGuideBase::setActiveState(bool bActive)
{
    if(bActive == m_bActive)
        return;
    m_bActive = bActive;

    if(bActive)
    {
        if(!m_timerResendStartCmd.isActive())
        {
            m_timerResendStartCmd.start(500);
        }

        if(!m_timerFoul.isActive())
        {
            m_timerFoul.start(500);
        }
    }
    else
    {
        m_timerResendStartCmd.stop();
        m_timerFoul.stop();
    }
}

void CTestGuideBase::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(pSteDev == nullptr)
        return;

    m_iDevID = pSteDev->getMacID();

    //信号只绑定一次
    if(pSteDev != m_pSteDev)
    {
        connectSignal();

        m_mapElementInfo.clear();

        auto vecAllElement = pSteDev->getAllElements();

        //将测试单元信息与测试单元的测试状态绑定
        CElementInfo elementInfo;
        for(auto &element : vecAllElement)
        {
            m_mapElementInfo[element->getIndex()] = elementInfo;
            m_mapElementInfo[element->getIndex()].m_objTestProcManage.setTestStateManage(&element->getTestStateManage());
        }

        m_pSteDev = pSteDev;
    }
}

CDevHardwareBase *CTestGuideBase::getSteDev()
{
    return m_objSteDevMgr.getDevSteByID(m_iDevID);
}

//标志为 是否由当前类执行开始命令 是否是广播执行, 执行开始命令影响后一个参数的生效
//让所有都开始测试
bool CTestGuideBase::startTest(const bool &bExecuteStart, const bool &bCast)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("startTest bCast false");
        return false;
    }

    //使用默认参数相当于清空
    setResendCmd();

    auto vecAllBindElement = pSteDev->getAllBindElements();

    //当前设备没有绑定的可用测试单元，返回true
    if(vecAllBindElement.size() == 0)
        return true;

    static CLocation s_location;

    //调用单独开启设备的初始化过程，不执行其中的设备开启命令
    for(auto &element : vecAllBindElement)
    {
        if(&element == &(*vecAllBindElement.begin()))
        {
            s_location.m_iSteDevMacID = m_iDevID;
            s_location.m_shElementIndex = element->getIndex();
        }

        if(!startTest(element->getIndex(), true, false))
            return false;
    }

    //通过设备发送开始命令
    if(bExecuteStart)
    {
        //中长跑等设备存在自己的语音请准备逻辑
        emit sigTestPrepare(s_location);

        unsigned short shCmdType = bCast ? ELEMENT_NUM_CUST : ELEMENT_NUM_CUR_DEV;
        //通过统一的结构进行命令发送
        setResendCmdElement(shCmdType);
    }

    return true;
}

bool CTestGuideBase::stopTest(const bool &bExecuteStart, const bool &bCast)
{
    for(auto &element : m_mapElementInfo)
    {
        element.second.m_bContinueTest = false;
        element.second.m_bActuallyWorking = false;
    }

    s_bContinueTest = false;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("stopTest bCast false");
        return false;
    }

    //在发送开始指令位置有while循环，需要先退出循环，然后在进行使用否则容易造成死锁
    setResendCmd();

    auto vecAllBindElement = pSteDev->getAllBindElements();
    if(vecAllBindElement.size() == 0)
        return true;

    for(auto &element : vecAllBindElement)
    {
        setTestState(CTestStateManage::TEST_NOT_START, element->getIndex());
    }

    //通过设备发送停止命令
    if(bExecuteStart)
    {
        //广播通知全部设备关闭
        pSteDev->stopWork(bCast);
    }

    return true;
}

//开始测试，所有实验设备都通过统一的开始函数
//参数说明：设备序号、第一次测试、直接开始
bool CTestGuideBase::startTest(const unsigned short &shElementIndex, const bool &bFirstTest, const bool &bExecuteStart)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pSubElement = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("startTest::getElementInfo false");
        return false;
    }

    //设置开始标志(此标志为真实开始标志，底层开启后才设置为true)
    pElementInfo->m_bActuallyWorking = false;
    pElementInfo->m_nIllegalTimes = 0;

    //初始化单元信息
    bool bChangeTestItem = true;
    //注：仅第一次会调用，后续仅修改测试结果
    if(bFirstTest)
    {
        //直连设备或者异步模式，设置开始时间
        if(CDevDataCfg::getDevDirectConnectFlag() || s_shTestMode == TEST_ASYNC)
            setStartTestTime();

        initElementInfo(*pElementInfo);

        pSubElement->setFirstTestFlag(true);
    }
    else
    {
        //多种测试项目需要切换(身高、体重、人体成分)
        if(m_vecTestItem.size() > 1)
            pElementInfo->m_shCurTestItemIndex++;
        else
            bChangeTestItem = false;
    }

    //游客模式每次需要设置时间
    if(pSubElement->isVisitorMode())
    {
        setStartTestTime();
        for(size_t i = 0; i < m_vecTestItem.size(); i++)
        {
            pElementInfo->m_arrTestResultInfo[i].m_strStartTime =  CTestResultInfo::getSaveTimeStamp(s_datetimeStartTest);
        }
    }

    //设置当前测试项目
    if(!setCurTestItem(shElementIndex,bChangeTestItem))
    {
        LOGERROR("Invalid Test Item");
        return false;
    }

    //当为直连时，播放准备语音：请准备，为了保持和统一的开始命令保持统一，使用信号发送
    //要不直接将变量传入函数即可
    //bool bSpeak = bExecuteStart && !pSteDev->isStaionSte();
    setTestState(CTestStateManage::TEST_PREPARE, *pElementInfo, false, false, true);
    if(bExecuteStart)
    {
        static CLocation location;
        location.m_iSteDevMacID = m_iDevID;
        location.m_shElementIndex = shElementIndex;
        emit sigTestPrepare(location);
    }

    pElementInfo->m_bContinueTest = true;

    //若存在异常将异常通知外部
    pSubElement->notifyAbnormal();

    if(!pElementInfo->m_bContinueTest)
        return false;

    //发送开始命令
    if(bExecuteStart)
    {
        //将命令统一放到发送结构中进行发送,统一在开始成功之后在外部进行开始语音的处理
        setResendCmdElement(shElementIndex);
    }

    return true;
}

void CTestGuideBase::onTestItemChanged()
{
    //若当前存在测试项目变更，加载当前测试项目对应的测试配置，包含配置
    for(auto &element : m_mapElementInfo)
    {
        element.second.m_objTestProcManage.loadTestCfg();
    }
}

void CTestGuideBase::onErr(const unsigned short shElementIndex)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    CSubElementBase* pElement;
    CElementInfo *pElementInfo;
    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("pElement == nullptr shElementIndex = " + to_string(shElementIndex));
        return;
    }

    if (DEVICE_ERR_TYPE_UNKNOWN & pElement->getErrType() ||
            DEVICE_ERR_TYPE_MOUDLE  & pElement->getErrType())
    {
        pElementInfo->m_TestExcept = 1;
        m_ui.showAbNormalPopup();
        procTestEnd(shElementIndex);
    }

    //通知外部错误状态变更
    emit signalErrStateChange();

    bool bSetErrInfo = false;
    if(!(pElement->getErrCode() != DEVICE_NORMAL || pElement->getLastErrType() != TYPE_NORMAL))
    {
        return;
    }

    QString qsExpTip = QString("FOUL:"); //底层根据这个标志判断犯规标志，同时置红显示
    qsExpTip.clear(); //中考体测不显示foul标志

    string errDesc = pSteDev->getElement(shElementIndex)->getErrDesc();
    if(errDesc.empty())
    {
        return;
    }

    qsExpTip += QString::fromLocal8Bit(errDesc.c_str());
    qDebug() << "+++" << qsExpTip;
    LOGINFO(qsExpTip.toStdString());

    //未工作状态，进行异常检测
    if(!pElementInfo->m_bActuallyWorking)
    {
        //设置设备就绪标志为false,这里未开始阶段才将准备就绪标志置为false
        setDevReadyFlag(false);

        //异物遮挡不在未测试模式和测试完成播报(主要是立定跳远开放了在非测试状态发送测试错误)
        short shTestState = pElement->getCurTestState();
        if(shTestState == CTestStateManage::TEST_NOT_START || shTestState == CTestStateManage::TEST_COMPLETED)
        {
            return;
        }

        //准备阶段存在倒杆可能是上一次未清除，在测试中会自动判断，所以直接清除
        if(DEVICE_ERR_TYPE_STOP & pElement->getErrType())
        {
            pElement->checkErr();
        }
        else
        {
            bSetErrInfo = true;
            m_speak.textToSpeech(QString::fromLocal8Bit(errDesc.c_str()));
        }
    }
    else
    {
        //进行犯规检测，直接终止流程（倒杆的时候不再说话）
        if(DEVICE_ERR_TYPE_STOP & pElement->getErrType())
        {
            short shTestState = pElement->getCurTestState();
            if(shTestState == CTestStateManage::TEST_NOT_START || shTestState == CTestStateManage::TEST_COMPLETED)
            {
                return;
            }

            bSetErrInfo = true;

            //将违规次数置0，用于区分语音
            m_mapElementInfo[shElementIndex].m_nIllegalTimes = 0;
            m_mapElementInfo[shElementIndex].m_nFoulCode = CONTROL_STOP;

            procTestEnd(shElementIndex);
        }
        //进行违例检测，不影响流程使用
        else if(DEVICE_ERR_TYPE_FOUL & pElement->getErrType())
        {
            bSetErrInfo = true;
            m_mapElementInfo[shElementIndex].m_nFoulCode = CONTROL_FOUL;
            m_speak.textToSpeech(QString::fromLocal8Bit(errDesc.c_str()));
        }
    }

    //若1.5s没有犯规标记，那么清空本次犯规提醒并将犯规标志置为false
    m_mapFoul[shElementIndex] = QDateTime::currentDateTime().toMSecsSinceEpoch();

    if(!bSetErrInfo)
    {
        return;
    }

    //设置界面显示显示对应的错误信息
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        m_ui.setTestState(qsExpTip);

        short shTestState = pElement->getCurTestState();
        if(shTestState == CTestStateManage::TEST_NOT_START || shTestState == CTestStateManage::TEST_COMPLETED)
        {
            return;
        }

        //中考体测犯规提示
        //int nIndex = getCurTestIndex(m_mapElementInfo[shElementIndex]);
        //emit sigChangeErrDescribe(nIndex, qsExpTip);

    }
    else
    {
        short shDevType = getDevType();
        short shDevState = CDevDataCfg::getColTypeIndex(COL_DEV_STATE, shDevType, TEST_ASYNC);
        short shUserID = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);
        auto itFind = m_mapElementInfo.find(shElementIndex);
        if(itFind != m_mapElementInfo.end() && shDevType >= 0 && shUserID >= 0)
        {
            QString qsUserID = getUserID(itFind->second);

            //根据犯规类型显示成绩
            m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shUserID, qsUserID,\
                                     shDevState, qsExpTip);
        }
    }
}

//测试单元状态变更需要更新到ui
void CTestGuideBase::onElementTestStateChange(const unsigned short &shElementIndex, short shTestState)
{
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;
    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("startTest::getElementInfo false");
        return;
    }

    if(CDevDataCfg::getDevDirectConnectFlag())
        return;

    qDebug() << "rrr" << shElementIndex << "gggg" << pElement->getTestState();
    //下面逻辑不会对未进行测试的设备进行处理
    if(!pElement->getBindFlag())
        return;

    if(shTestState == CTestStateManage::TEST_ERR)
    {
        setActiveState(false);
        devDisconnect(pElement, *pElementInfo);
    }

    if(shTestState == CTestStateManage::TEST_TIMEOUT)
    {
        static CLocation sLocation;
        sLocation.m_iSteDevMacID = m_iDevID;
        sLocation.m_shElementIndex = shElementIndex;
        emit signalTestComplete(sLocation);
        //stopTest(shElementIndex);
    }

    short shTestMode = s_shTestMode;

    //************************** 更新界面显示状态 *******************************
    //同步模式分为两种，一种是在识别界面，一种是在测试界面(在测试界面就使用异步的表格)
    if(shTestMode == TEST_SYNC)
        shTestMode = CDevDataCfg::isHomePage(m_ui.getCurPage()) ? TEST_SYNC : TEST_ASYNC;

    short shDevType = getDevType();
    short shDevStateIndex = CDevDataCfg::getColTypeIndex(COL_DEV_STATE, shDevType, shTestMode);
    short shUserIDIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, shTestMode);

    QString qsUserID = getUserID(*pElementInfo);
    QString qsDevState;

    //变更为描述(在测试界面使用文字)
    qsDevState = CDevDataCfg::convertStatusToDescrip(shTestState);

    if(shUserIDIndex >= 0 && shDevStateIndex >= 0)
    {
        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, qsUserID,\
                                 shDevStateIndex, qsDevState);
    }
}

bool CTestGuideBase::stopTest(unsigned short shElementIndex, bool bSetTestState, short shTestState)
{
    //设置对应测试单元状态
    auto itFindElementInfo = m_mapElementInfo.find(shElementIndex);
    if(itFindElementInfo != m_mapElementInfo.end())
    {
        itFindElementInfo->second.m_bContinueTest = false;
        itFindElementInfo->second.m_bActuallyWorking = false;
    }
    else
    {
        LOGERROR("m_mapElementInfo not exist shelementIndex = " + to_string(shElementIndex));
    }

    //设置整体状态
    if(bSetTestState)
    {
        setTestState(shTestState, shElementIndex);
    }

    //设置设备对应的测试单元
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    return pSteDev->stopWork(shElementIndex, true);
}

//存在错误返回true
bool CTestGuideBase::existErrInBindElement()
{
    bool bRet = false;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev != nullptr)
    {
        bRet = pSteDev->existErrInBindElement();
    }
    else
    {
        LOGERROR("invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
    }

    return bRet;
}

void CTestGuideBase::setStartTestTime()
{
    s_datetimeStartTest = QDateTime::currentDateTime();
}

bool CTestGuideBase::bindUserToElement(const unsigned short &shElementIndex, const CUserInfo &userInfo)
{
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;

    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("bindUserToElement::getElementInfo false");
        return false;
    }

    pElementInfo->m_pUserInfo = &userInfo;
    short shUserMode = userInfo.m_llUniqueID != TOURIST_MODE_ID ? CTestStateManage::USER_MODE : CTestStateManage::VISITORS;
    pElement->getTestStateManage().setTestModel(shUserMode);

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    return pSteDev->setElementBindFlag(shElementIndex, true);
}

void CTestGuideBase::unbindUserToElement(const unsigned short &shElementIndex)
{
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;
    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("unbindUserToElement::getElementInfo false");
        return;
    }

    pElementInfo->m_pUserInfo = nullptr;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev != nullptr)
    {
        pSteDev->setElementBindFlag(shElementIndex, false);
    }
    else
    {
        LOGERROR("invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
    }
}

string CTestGuideBase::getResPath()
{
    string strAppPath = CFilePath::getAppDirPath();
    strAppPath += "/";
    strAppPath +=  + TEST_PROC_SRC;
    CFilePath::isDirExist(strAppPath, true);
    strAppPath += "/";
    return strAppPath;
}

CScoreRuleInfo &CTestGuideBase::getScoreInfo()
{
    return s_objScoreRuleInfo;
}

short CTestGuideBase::getCurTestItem(const unsigned short &shElementIndex)
{
    short shTestItem = TEST_ITEM_UNKNOW;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev != nullptr)
    {
        auto pElement = pSteDev->getElement(shElementIndex);
        if(pElement != nullptr)
        {
            shTestItem = pElement->getCurTestItem();
        }
    }
    else
    {
        LOGERROR("invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
    }

    return shTestItem;
}

void CTestGuideBase::getTestResult(vector<CTestResultInfo> &vecResult, const unsigned short &shElementIndex, bool bDeleteRes/* = false*/)
{
    vecResult.clear();

    auto itFind = m_mapElementInfo.find(shElementIndex);
    if(itFind == m_mapElementInfo.end())
    {
        LOGERROR("m_mapElementInfo not exist shElementIndex = " + to_string(shElementIndex));
        return;
    }

    CElementInfo &elementInfo = itFind->second;

    //需要测试几个项目就存在几个
    size_t szResultSize = m_vecTestItem.size();
    for(size_t i = 0; i < szResultSize; i++)
    {
        //数据有效才存入
        CTestResultInfo &testResult = elementInfo.m_arrTestResultInfo[i];
        if(testResult.m_strMulitResult.empty())
            continue;

        vecResult.push_back(testResult);

        if(bDeleteRes)
            testResult.clearResult(false);
    }
}

QString CTestGuideBase::getUserID(const CElementInfo &elementInfo)
{
    QString qsUserID;

    if(elementInfo.m_pUserInfo != nullptr)
    {
        qsUserID = elementInfo.m_pUserInfo->m_strStudentId;
    }

    return qsUserID;
}

void CTestGuideBase::onHardwareStateChange(const unsigned short &shElementIndex, unsigned char cTestState)
{
    switch (cTestState){
    case DEV_TEST_STATE_INVALID:
        break;
    case DEV_TEST_STATE_TESTING:
        procTestBegin(shElementIndex);
        break;
    case DEV_TEST_STATE_EOT_INVALID://测试异常结束设置结束标志码
    case DEV_TEST_STATE_TIME_OUT_NOT_TEST:
    case DEV_TEST_STATE_EOT_VALID:{
        LOGINFO("test complete state = " + to_string(cTestState));
        auto itFind = m_mapElementInfo.find(shElementIndex);
        if(itFind != m_mapElementInfo.end())
        {
            itFind->second.m_cHardwareDataStateCode = cTestState;
        }
        else
        {
            LOGERROR("onTestStateChange shElementIndex invalid");
        }
        procTestEnd(shElementIndex);
        break;
    }
    default:
        break;
    }
}

//如坐位体前屈需要在测试开始阶段就进行
void CTestGuideBase::procTestBegin(const unsigned short &shElementIndex)
{
    CSubElementBase *pSubElement;
    CElementInfo *pElementInfo;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo err shElementIndex ==" + to_string(shElementIndex));
        return;
    }

    bool &bActuallyWorking = pElementInfo->m_bActuallyWorking;

    if(!bActuallyWorking)
    {
        //作为体前屈真正开始，此时才能设置界面显示值
        if(getDevType() == DEV_TYPE_SFB)
            m_ui.testStarted();

        bActuallyWorking = true;

        //将错误信息置为没有错误
        pElementInfo->m_nFoulCode = NOT_FOUL;

        //真正开始 给ui置空显示即可(所有直连设备适用)
        if(CDevDataCfg::getDevDirectConnectFlag())
        {
            m_ui.setCurTestRet(DEFAULT_DISP_VALUE);

            //坐位体前屈播报真正开始语音(直连设备)
            if(getDevType() == DEV_TYPE_SFB)
            {
                setTestState(CTestStateManage::TEST_CHECKED, shElementIndex, true, false, false);
                //通用设置开始状态
                setTestState(CTestStateManage::TEST_IN_PROCESS, shElementIndex, false, false, true);
            }
        }
        else
        {
            short shDevType = getDevType();
            short shColumnIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);
            short shUpdateIndex = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, shDevType, TEST_ASYNC);

            //身高体重数据需要特殊处理
            if(shColumnIndex >= 0 && shUpdateIndex >= 0)
            {
                m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                         getUserID(*pElementInfo), shUpdateIndex, DEFAULT_DISP_VALUE);
            }
        }

        //底层正式开始进行测试，若存在防作弊那么开启防作弊,在直连模式下用户使用开启防作弊
        cfgAnticheating(*pElementInfo, pSubElement);
    }
    //一个测试单元进入多次，可能引起软件工作不正常，进行日志打印
    else
    {
        LOGERROR("Many times to enter shElementIndex : " + to_string(shElementIndex));
    }
}

//肺活量类似通用流程
void CTestGuideBase::procTestEnd(const unsigned short &shElementIndex)
{
    //本次测试经历了软件设置开始，底层硬件实际开始 底层硬件结束才是一次有效的测试
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;

    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("procTestEnd:getElementInfo false, shIndex = " + to_string(shElementIndex));
        return;
    }

    bool &bActuallyWorking = pElementInfo->m_bActuallyWorking;
    if(!bActuallyWorking && !m_bForceComplete)
    {
        LOGWARN("procTestEnd:Last data exists" + to_string(shElementIndex));
        return;
    }
    bActuallyWorking = false;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("procTestEnd:test completed but dev invalide invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    pSteDev->stopWork(shElementIndex, false);

    //实际上使用时排球出现停止然后立即开始会出问题，这里排球尽力等待停止

    CElementInfo &elementInfo = *pElementInfo;

    //变更测试状态
    setTestState(CTestStateManage::TEST_COMPLETED, elementInfo);

    //判断用户是否有效
    if(elementInfo.m_pUserInfo == nullptr)
        return;

    //设置当前测试单元完成状态为true
    pElement->setComplete();

    //不是超时结束（需要在界面上显示超时） 进行数据保存
    CTestResultInfo &testResultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];

    if(!m_bForceComplete)
    {
        if((elementInfo.m_cHardwareDataStateCode != DEV_TEST_STATE_TIME_OUT_NOT_TEST)
                || elementInfo.m_bAbortTest)
        {
            //若存在测试过程防作弊资源，需要进行资源的处理
            if(!pElement->existAntiCheating())
            {
                dealTestAntiCheating(testResultInfo, pElement);
            }

            //这一块处理实际的数据需要在不同的继承类中进行处理，肺活量 身高体重等相同处理逻辑的设备，存储的数据格式不同
            saveDataToTestResult(elementInfo, pElement);
        }
    }

    //判断单次测量项的项是否完成，内部设置单个项目的重复次数（例如肺活量测试2次）
    bool bSignalItemComplete = elementInfo.m_objTestProcManage.getTestCompleteFlag();

    //判断实验完成标志（全部完成）是所有项目测试完成（身高体重测试身高），同时单次项目也全部完成
    bool bCompletedTest = elementInfo.m_shCurTestItemIndex == m_vecTestItem.size() - 1 && bSignalItemComplete;

    //超时也算完成
    bCompletedTest |= (elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_TIME_OUT_NOT_TEST);

    string errDesc = pElement->getErrDesc();
    QString qsErrDesp = QString::fromLocal8Bit(errDesc.c_str());

    //清除上次所有错误
    pElement->clearErr();

    //处理本次完成结果,显示到ui 发送到外面进行统一保存
    procTestResult(elementInfo, pElement, bCompletedTest);

    //强制完成直接结束,中止测试也是直接结束
    if(bCompletedTest || m_bForceComplete || elementInfo.m_bAbortTest)
    {
        //直连模式语音结束(强制结束不进行语音播报)
        setTestState(CTestStateManage::TEST_COMPLETED, elementInfo, false, false, false);

        if(CDevDataCfg::getDevDirectConnectFlag() && !m_bForceComplete && !elementInfo.m_bAbortTest)
        {
            setTestState(CTestStateManage::TEST_COMPLETED, elementInfo, false, false, true);

            //当为足球设备，且成绩无效时，需要提示无效原因
            if(getDevType() == DEV_TYPE_FOOTBALL)
            {
                //1.按钮结束
                if(elementInfo.m_nFoulCode == CONTROL_STOP && elementInfo.m_nIllegalTimes > 0)
                {
                    m_speak.textToSpeech("犯规结束");
                }
                //2.倒杆结束
                else if(elementInfo.m_nFoulCode == CONTROL_STOP)
                {
                    m_speak.textToSpeech("犯规结束，犯规原因：岛甘");
                }
                //2.漏杆结束
                else if(elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_INVALID)
                {
                    m_speak.textToSpeech("犯规结束，犯规原因：" + qsErrDesp);
                }
                else
                {
                    m_speak.textToSpeech(elementInfo.m_objTestProcManage.getTestProcContent());
                }
            }
            else
            {
                m_speak.textToSpeech(elementInfo.m_objTestProcManage.getTestProcContent());
            }
        }


        //跳转页面：足球、篮球违规结束会返回首页(其他设备均不跳转)(中考需求)
//        bool bJumpPage = true;

//        if(CDevDataCfg::isNeedResultValid(m_shDevType))
//        {
//            //正常结束不跳转
//            if(elementInfo.m_nFoulCode != CONTROL_STOP && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID)
//            {
//                bJumpPage = false;
//            }
//        }
//        else
//        {
//            bJumpPage = false;
//        }

//        if(CDevDataCfg::getDevDirectConnectFlag() && bJumpPage)
//        {
//            CTestGuideBase::s_bGoToHomePage = true;
//            QTimer::singleShot(4700, this, [this](){
//                if(CTestGuideBase::s_bGoToHomePage)
//                {
//                    emit this->sigGoToHomePage();
//                }
//            });
//        }

        return;
    }

    emit signaldealTestProc(shElementIndex, false, true);
}

//这里写一份肺活量对应的通用类型
void CTestGuideBase::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
{
    //强制完成不保存成绩
    if(m_bForceComplete)
        return;

    if(pElement == nullptr)
    {
        LOGERROR("pElement invalid");
        return;
    }
    //获取数据根据数据类型进行相应的处理
    bool bRet = true;// = pElement->getTestResult(&fData);

    if(!bRet)
    {
        LOGERROR("getTestResult false");
        return;
    }

    auto &resultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];

    //若存在测试过程防作弊资源，需要进行资源的处理
    if(pElement->existAntiCheating())
    {
        dealTestAntiCheating(resultInfo, pElement);
    }

//  //记录是否可信
//    bool bCredible = (elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID);

    //记录本次成绩
//    QString qsValueDisp;
//    qsValueDisp = value2DispValue(static_cast<short>(resultInfo.m_nItem), fData);

//    resultInfo.m_strMulitResult += (qsValueDisp.toStdString() + g_separateCharEachTime);
}

void CTestGuideBase::procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete)
{
    auto &testResultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];

    //通过结果获取成绩，同时根据置信度判断成绩是否可用
    QStringList listContent;

    //同步异步测试表格相同，将测试结果更新到界面
    static CLocation sLocation;
    sLocation.m_iSteDevMacID = m_iDevID;
    sLocation.m_shElementIndex = pElement->getIndex();

    //直连,暂时不用判断超时，添加数据到结果表格,在游客模式已经处理为全部为有效成绩
    QString qsCurTestRes;
    qsCurTestRes = getTestResult(false, testResultInfo);
    QString qsScore("");

    //设置提交按钮可用(完成过一次测试就设置提交按钮可用)
    m_ui.setBtnEnableCommit(true);

    //是否是游客模式，在下面会用到
    bool bVisitorsMode = pElement->isVisitorMode();

    short shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testResultInfo.m_nItem));
    /******** 直连和组网都应该按照统一的计次方式进行记录，这里组网和直连采用不同的计次方式进行计数
              是因为采用结果中的计次更准确，采用测试过程中的次数可能有误差，但是游客模式改版之后只进行
              每次测试的数据保存，主要是为了进行上传使用次数时更准确                              ********/

    //直连显示原始成绩,直连中测试次数按照测试流程中的次数来计数
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        short shSerialNo = elementInfo.m_objTestProcManage.getCurTestItemTimesIndex();
        listContent.append(QString::number(shSerialNo));
        listContent.append(qsCurTestRes);

        //部分设备存在违规次数
        if(CDevDataCfg::isNeedViolationInfoDev(shDevType))
        {
            listContent.append(QString::number(elementInfo.m_nIllegalTimes));
        }

        //游客：序号 测试值
        //用户：序号 测试值(判断值有效无效) 成绩
        bool    bOrdered = true;
        //游客不设置成绩
        if(!bVisitorsMode)
        {
            //添加成绩
            qsScore = getTestScore(false, testResultInfo, *elementInfo.m_pUserInfo);

            listContent.append(qsScore);

            int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
            //山东济南不显示得分
            if(nSoftVersion == soft::VERSION_SDJN)
            {
                listContent.append( "-" );
            }
        }
        else
        {
            bOrdered = false;
        }

        m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, listContent, false);
    }
    //组网
    else
    {
        if(shDevType == DEV_TYPE_UNKNOW)
        {
            LOGERROR("getDevTypeByTestItem false testitem = " + to_string(testResultInfo.m_nItem));
            return;
        }

        listContent = CDevDataCfg::getTableContent(sLocation, TEST_ASYNC, *elementInfo.m_pUserInfo);
        short shUserIDIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);

        //用户id，若用户id存在问题，那么不进行更新
        QString qsUserID = getUserID(elementInfo);
        if(shUserIDIndex >= 0)
        {
            listContent[shUserIDIndex] = qsUserID;

            //若存在测试次数更新当前测试次数
            short shTestTimesIndex = CDevDataCfg::getColTypeIndex(COL_TEST_TIMES, shDevType, TEST_ASYNC);
            if(shTestTimesIndex >= 0)
            {
                listContent[shTestTimesIndex] = QString::number(testResultInfo.getTestTimes());
            }

            //若存在通过次数，则设置次数为 4(马仔)
            short shThroughTimesIndex = CDevDataCfg::getColTypeIndex(COL_THROUGH_TIMES, shDevType, TEST_ASYNC);
            if(shThroughTimesIndex >= 0)
            {
                if(testResultInfo.m_nItem == TEST_ITEM_RUN_BACK_8_50)
                {
                    listContent[shThroughTimesIndex] = "4";
                }
                else if(testResultInfo.m_nItem == TEST_ITEM_RUN_BACK_4_10)
                {
                    listContent[shThroughTimesIndex] = "2";
                }
                else
                {
                    listContent[shThroughTimesIndex] = "UNKNOWN";
                }
            }

            //更新当前结果
            QString qsTestRes = getTestResult(false, testResultInfo);
            short shTestResultIndex = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, shDevType, TEST_ASYNC);
            if(shTestResultIndex >= 0)
            {
                listContent[shTestResultIndex] = qsTestRes;
            }

            //更新结果到测试表格中
            m_ui.updateModelRowData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, qsUserID, listContent);

            //若是异步模式，需要将结果放到完成表格
            if(bComplete && s_shTestMode == TEST_ASYNC)
            {
                //删除默认表格中的内容（异步做了处理，保证删除的仅仅是正在测试的人员）
                m_ui.deleteModelData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, qsUserID);
            }
        }
    }

    //测试完成或者当前是游客模式触发
    if(bComplete || bVisitorsMode)
    {
        signalTestComplete(sLocation);
    }
}

void CTestGuideBase::dealTestAntiCheating(CTestResultInfo &resultInfo, CSubElementBase *pElement)
{
    if(pElement == nullptr)
    {
        LOGERROR("pElement is nullptr")
                return;
    }

    CTestProcAntiCheatingBasic *pTestProcAntiCheating = pElement->getAntiCheating();
    if(pTestProcAntiCheating == nullptr)
        return;

    //停止防作弊监视
    pTestProcAntiCheating->stop_(true);

    //获取资源,资源的筛选已经在结束时就进行了设置
    vector<string> vecAntiCheatingFile;
    int nExistAntiCheatingFile;
    pTestProcAntiCheating->getResourceList(vecAntiCheatingFile, nExistAntiCheatingFile);

    //将资源位置赋值到结果中
    if(vecAntiCheatingFile.size() == 0)
        return;

    //将路径信息发送到界面用于在界面显示防作弊资源
    size_t szCount = vecAntiCheatingFile.size();
    bool bEnd;

    for(size_t i = 0; i < szCount; i++)
    {
        //通知界面显示防作弊
        m_ui.setTestingPicture(nExistAntiCheatingFile++ + 1, QString::fromStdString(vecAntiCheatingFile[i]));

        bEnd = i == szCount - 1;

        resultInfo.addProcSrc(vecAntiCheatingFile[i], bEnd);
    }

    //若本次是空资源添加对应的次数
    if(szCount == 0)
        resultInfo.addProcSrc("", true);
}

QString CTestGuideBase::value2DispValue(const short &shTestItem, const float &fValue)
{
    QString qsDest;
    double lfValue = static_cast<double>(fValue);
    switch (shTestItem)
    {
    //cm 显示一位小数
    case TEST_ITEM_HEIGHT:
        //如果值为 170.149，那么转换应该为 170.2，系统默认函数转为 170.1，若第三位小数大于5.那么在第二位小数加1
    {
        int nTemp = static_cast<int>(lfValue) * 1000;
        if(nTemp % 10 >= 5)
        {
            lfValue += 0.01;
        }

        qsDest = QString::number( lfValue, 'f', 1);
    }
        break;
        //kg 显示一位小数
    case TEST_ITEM_GRIP:
    {
        qsDest = QString::number( lfValue, 'f', 1);
    }
        break;
        //kg 显示一位小数
    case TEST_ITEM_WEIGHT:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //百分比 体脂含量 显示1位小数
    case TEST_ITEM_HBC:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //cm 显示一位小数
    case TEST_ITEM_SFB:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //视力显示一位小数
    case TEST_ITEM_EYESIGHT:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //cm 跳远,显示0位小数
    case TEST_ITEM_SBJ:
        qsDest = QString::number( lfValue, 'f', 0);
        break;
    case TEST_ITEM_PVV:
        qsDest = QString::number( lfValue, 'f', 2);
        break;
        //s 转为 分秒显示  最终展示在面板上的值 01:07.223;
    case TEST_ITEM_RUN_800:
    case TEST_ITEM_RUN_1000:
    case TEST_ITEM_RUN_1500:
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
    {
        int nMin = static_cast<int>(lfValue / 60);
        //保留秒之后的2位小数，外部存储是保留2位小数即可
        float fSes = static_cast<float>(lfValue - nMin * 60);

        qsDest = QString("%1:%2").arg(nMin, 2, 10, QLatin1Char('0')).arg(double(fSes), 5, 'f', 2, QLatin1Char('0'));
    }
        break;
    case TEST_ITEM_BASKETBALL:
    case TEST_ITEM_FOOTBALL:
    {
        /*
        int nMin = static_cast<int>(lfValue / 60);
        float fSes = static_cast<float>(lfValue - nMin * 60);
        fSes = ceil(10 * fSes) / 10.0f;

        qsDest = QString("%1:%2").arg(nMin, 2, 10, QLatin1Char('0')).arg(double(fSes), 4, 'f', 1, QLatin1Char('0'));
        lfValue = ceil(10 * lfValue) / 10;
        qsDest = QString::number( lfValue, 'f', 1);
        */

        qsDest = QString::asprintf("%.1f", lfValue);
    }
        break;
    case TEST_ITEM_SOLIDBALL:
        qsDest = QString::number( lfValue, 'f', 2);
        break;
    //其他的基本都是个数或者标准的单位，直接转为整数进行显示
    default:
        qsDest = QString::number(qRound(lfValue));
        break;
    }

    return qsDest;
}

QString CTestGuideBase::value2DispValue(const short &shTestItem, const string &strValue)
{
    QString qsDest;
    double lfValue = atof(strValue.c_str());
    switch (shTestItem)
    {
    //cm 显示一位小数
    case TEST_ITEM_HEIGHT:
        //如果值为 170.149，那么转换应该为 170.2，系统默认函数转为 170.1，若第三位小数大于5.那么在第二位小数加1
    {
        int nTemp = static_cast<int>(lfValue) * 1000;
        if(nTemp % 10 >= 5)
        {
            lfValue += 0.01;
        }

        qsDest = QString::number( lfValue, 'f', 1);
    }
        break;
        //kg 显示一位小数
    case TEST_ITEM_GRIP:
    {
        qsDest = QString::number( lfValue, 'f', 1);
    }
        break;
        //kg 显示一位小数
    case TEST_ITEM_WEIGHT:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //百分比 体脂含量 显示1位小数
    case TEST_ITEM_HBC:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //cm 显示一位小数
    case TEST_ITEM_SFB:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //cm 跳远,显示0位小数
    case TEST_ITEM_SBJ:
        qsDest = QString::number( lfValue, 'f', 0);
        break;
    case TEST_ITEM_PVV:
        qsDest = QString::number( lfValue, 'f', 2);
        break;
        //s 转为 分秒显示  最终展示在面板上的值 01:07.223;
    case TEST_ITEM_RUN_800:
    case TEST_ITEM_RUN_1000:
    case TEST_ITEM_RUN_1500:
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
    {
        int nMin = static_cast<int>(lfValue / 60);
        //保留秒之后的2位小数，外部存储是保留2位小数即可
        double lfSes = lfValue - nMin * 60;

        //最终展示在面板上的值 01:07.22;
        char arrTemp[10] = {0};
        sprintf_s(arrTemp, 10, "%02d", nMin);

        char arrTemp1[10] = {0};
        sprintf_s(arrTemp1, 10, ":%05.02f", lfSes);

        QByteArray arr1(arrTemp, 2);
        QByteArray arr2(arrTemp1, 6);

        arr1 += arr2;
        qsDest = QString(arr1);
    }
        break;
    case TEST_ITEM_BASKETBALL:
    case TEST_ITEM_FOOTBALL:
    {
        /*
        int nMin = static_cast<int>(lfValue / 60);
        float fSes = static_cast<float>(lfValue - nMin * 60);
        fSes = ceil(10 * fSes) / 10.0f;

        qsDest = QString("%1:%2").arg(nMin, 2, 10, QLatin1Char('0')).arg(double(fSes), 4, 'f', 1, QLatin1Char('0'));

        lfValue = ceil(10 * lfValue) / 10;
        qsDest = QString::number( lfValue, 'f', 1);
        */

        qsDest = QString::asprintf("%.1f", lfValue);
    }
        break;
        //其他的基本都是个数或者标准的单位，直接转为整数进行显示
    case TEST_ITEM_SOLIDBALL:
        qsDest = QString::number( lfValue, 'f', 2);
        break;
    default:
        qsDest = QString::number(qRound(lfValue));
        break;
    }

    return qsDest;
}

string CTestGuideBase::value2SaveValue(const short &shTestItem, const float &fValue)
{
    QString qsDest;
    double lfValue = static_cast<double>(fValue);
    switch (shTestItem){
    //cm 显示一位小数,不能随意变更身高值标准单位，会引起人体阻抗项目的相关测量不准确
    case TEST_ITEM_HEIGHT:{
        //如果值为 170.149，那么转换应该为 170.2，系统默认函数转为 170.1，若第三位小数大于5.那么在第二位小数加1

        int nTemp = static_cast<int>(lfValue) * 1000;
        if(nTemp % 10 >= 5)
        {
            lfValue += 0.01;
        }

        qsDest = QString::number( lfValue, 'f', 1);
        break;
    }
        //kg 显示一位小数
    case TEST_ITEM_GRIP:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //kg 显示一位小数
    case TEST_ITEM_WEIGHT:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //百分比 体脂含量 显示1位小数
    case TEST_ITEM_HBC:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //cm 显示一位小数
    case TEST_ITEM_SFB:
        qsDest = QString::number( lfValue, 'f', 1);
        break;
        //cm 跳远,显示0位小数
    case TEST_ITEM_SBJ:
        qsDest = QString::number( lfValue, 'f', 0);
        break;
    case TEST_ITEM_PVV:
        qsDest = QString::number( lfValue, 'f', 2);
        break;
        //s 转为 分秒显示  最终展示在面板上的值 01:07.223;
        //保存的结果为2位小数
    case TEST_ITEM_RUN_800:
    case TEST_ITEM_RUN_1000:
    case TEST_ITEM_RUN_1500:
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
        qsDest = QString::number( lfValue, 'f', 2);
        break;
    case TEST_ITEM_BASKETBALL:
    case TEST_ITEM_FOOTBALL:
        /*
        lfValue = ceil(10 * lfValue) / 10;
        qsDest = QString::number( lfValue, 'f', 1);
        */
        qsDest = QString::asprintf("%.1f", lfValue);
        break;
        //保留两位小数
    case TEST_ITEM_SOLIDBALL:
        qsDest = QString::number( lfValue, 'f', 2);
        break;
        //其他的基本都是个数或者标准的单位，直接转为整数进行显示
    default:
        qsDest = QString::number(qRound(lfValue));
        break;
    }

    return qsDest.toStdString();
}

vector<unsigned short> CTestGuideBase::getAllBindElementIndex()
{
    vector<unsigned short> vecAllBindElementIndex;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev != nullptr)
    {
        auto vecElement = pSteDev->getAllBindElements();
        for(auto &element : vecElement)
        {
            vecAllBindElementIndex.push_back(element->getIndex());
        }
    }
    else
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
    }

    return vecAllBindElementIndex;
}

bool CTestGuideBase::getElementInfo(CSubElementBase **pSubElement, CElementInfo **pElementInfo, const unsigned short &shElementIndex)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    auto itFind = m_mapElementInfo.find(shElementIndex);
    if(itFind == m_mapElementInfo.end())
    {
        LOGERROR("m_mapElementInfo err shElementIndex ==" + to_string(shElementIndex));
        return false;
    }

    auto pElement = pSteDev->getElement(shElementIndex);
    if(pElement == nullptr)
    {
        LOGERROR("pElement invalid");
        return false;
    }

    *pSubElement = pElement;
    *pElementInfo = &itFind->second;
    return true;
}

void CTestGuideBase::onTimeOutRemoveFoul()
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        //证明当前类对应的设备断连，停止当前测试指导活动
        setActiveState(false);
        return;
    }

    bool bDirectConnect = CDevDataCfg::getDevDirectConnectFlag();
    QString qsTips;

    qint64 llCurTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    for(auto it = m_mapFoul.begin(); it != m_mapFoul.end(); )
    {
        //错误时间加上 错误清零时间 小于当前时间，那么对错误进行清除
        if(it->second + 1500 < llCurTime && pSteDev->getElement(it->first)->getErrCode() == DEVICE_NORMAL)
        {
            //清除界面显示
            qsTips = m_mapElementInfo[it->first].m_objTestProcManage.getTestProcContent();
            if(bDirectConnect)
            {
                m_ui.setTestState( qsTips );
            }
            else
            {
                short shDevType = getDevType();
                if(shDevType == DEV_TYPE_UNKNOW)
                {
                    LOGERROR("dispTestResult devtype err");
                    return;
                }

                //异步模式和异步模式更新结果相同，根据用户id进行结果更新
                short shColumnIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);
                short shUpdateIndex = CDevDataCfg::getColTypeIndex(COL_DEV_STATE, shDevType, TEST_ASYNC);

                if(shColumnIndex < 0)
                {
                    LOGERROR("shColumnIndex < 0");
                    return;
                }

                if(shUpdateIndex < 0)
                {
                    LOGERROR("shUpdateIndex < 0");
                    return;
                }

                m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                         getUserID(m_mapElementInfo[it->first]), shUpdateIndex, qsTips);
            }

            m_mapFoul.erase(it);
            it = m_mapFoul.begin();
        }
        else
        {
            it++;
        }
    }
}

bool CTestGuideBase::initElementInfo(CElementInfo &elementInfo)
{
    elementInfo.reset();

    //当前绑定单元某些操作未完成不满足开启条件
    if(elementInfo.m_pUserInfo == nullptr)
        return false;

    //初始化测试结果信息
    int nIndex = 0;
    for(auto &testItem : m_vecTestItem)
    {
        auto &testResult = elementInfo.m_arrTestResultInfo[nIndex];

        LOGINFO("+++ nIndex = " + to_string(nIndex));

        testResult.reset();
        testResult.m_nItem = testItem;
        if(getDevType() == DEV_TYPE_RUN_MID)
        {
            short shMaleItem, shFemaleItem;
            CTestGuideRunMid::getCurTestRunMidItem(shMaleItem, shFemaleItem);
            testResult.m_nItem = elementInfo.m_pUserInfo->m_bGender ? shMaleItem : shFemaleItem;
        }

        testResult.m_llUniqueID = elementInfo.m_pUserInfo->m_llUniqueID;

        testResult.m_strStartTime = CTestResultInfo::getSaveTimeStamp(s_datetimeStartTest);
        testResult.m_sDeviceID = to_string(m_iDevID);

        //若当前测试单元满足防作弊要求，生成对应资源文件
        //生成测试过程测试存放路径路径为 时间+设备类型+ 测试人员 + 设备ID
        if(CDevDataCfg::getDevDirectConnectFlag() && elementInfo.m_pUserInfo != nullptr \
                && !elementInfo.m_pUserInfo->isTouristInfo())
        {
            testResult.setResourceRootPath( to_string(testResult.m_llUniqueID), s_strPathTestProRes, \
                                            testResult.m_strStartTime , \
                                            to_string(m_iDevID), CDevDataCfg::getDevDescribe(getDevType()));

            //每次开始资源存放路径只需要进行一次判断创建即可
            CFilePath::isDirExist(testResult.m_strPathResourceRoot, true);
        }

        ++nIndex;
    }

    return true;
}

void CTestGuideBase::connectSignal()
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    auto vecElement = pSteDev->getAllElements();
    for(auto &element : vecElement)
    {
        connect(element, &CSubElementBase::signalDataChange, this, &CTestGuideBase::onDataChange, Qt::QueuedConnection);
        connect(element, &CSubElementBase::signalHardwareTestStateChange, this, &CTestGuideBase::onHardwareStateChange);
        connect(element, &CSubElementBase::signalErr, this, &CTestGuideBase::onErr, Qt::DirectConnection/*Qt::QueuedConnection*/);
        connect(element, &CSubElementBase::signalTestStateChange, this, &CTestGuideBase::onElementTestStateChange/*, Qt::DirectConnection*/);
    }

}

void CTestGuideBase::disconnectSignal()
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    auto vecElement = pSteDev->getAllElements();
    for(auto &element : vecElement)
    {
        disconnect(element, &CSubElementBase::signalDataChange, this, &CTestGuideBase::onDataChange);
        disconnect(element, &CSubElementBase::signalHardwareTestStateChange, this, &CTestGuideBase::onHardwareStateChange);
        disconnect(element, &CSubElementBase::signalErr, this, &CTestGuideBase::onErr);
        disconnect(element, &CSubElementBase::signalTestStateChange, this, &CTestGuideBase::onElementTestStateChange);
    }
}

bool CTestGuideBase::setCurTestItem(const unsigned short &shElementIndex, bool bResetTestItem)
{
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;
    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("setCurTestItem:getElementInfo false, shElementIndex = " + to_string(shElementIndex));
        return false;
    }

    if(bResetTestItem)
    {
        if(pElementInfo->m_shCurTestItemIndex >= m_vecTestItem.size())
        {
            LOGERROR("m_shCurTestItemIndex invalid");
            return false;
        }

        short &shTestItem = m_vecTestItem[pElementInfo->m_shCurTestItemIndex];
        pElementInfo->m_objTestProcManage.setCurTestItem(shTestItem);

        pElement->setCurTestItem(shTestItem);
    }
    return true;
}

void CTestGuideBase::setTestState(short shTestState, unsigned short shElementIndex, bool bSpeak, bool bWaitSpeak, bool bSendToUi)
{
    auto itFind = m_mapElementInfo.find(shElementIndex);

    if(itFind == m_mapElementInfo.end())
        return;

    setTestState(shTestState, itFind->second, bSpeak, bWaitSpeak, bSendToUi);

    //实心球出现无法结束现象，在这里进行开始
    if(shTestState == CTestStateManage::TEST_IN_PROCESS && m_shDevType == DEV_TYPE_SOLIDBALL)
    {
        procTestBegin(shElementIndex);
    }
}

void CTestGuideBase::setTestState(short shTestState, CElementInfo &elementInfo, bool bSpeak, bool bWaitSpeak, bool bSendToUi)
{
    //在内部会设置测试状态时，会同时设置对应测试单元中的测试状态，避免状态不一致
    elementInfo.m_objTestProcManage.setCurTestState(shTestState);

    //获取当前语音和界面显示的内容
    QString qsTips = elementInfo.m_objTestProcManage.getTestProcContent();

    if(bSendToUi)
    {
        m_ui.setTestState( qsTips );
    }

    //直连和同步在首页不进行语音播报，异步和同步不是第一次不进行语音播报
    if(bSpeak)
    {
        if(CDevDataCfg::getDevDirectConnectFlag())
        {
            if(CDevDataCfg::isHomePage(m_ui.getCurPage()))
                bSpeak = false;
        }
        else
        {
            if(s_shTestMode == TEST_SYNC)
            {
                if(CDevDataCfg::isHomePage(m_ui.getCurPage()))
                    bSpeak = false;
            }

            if(elementInfo.m_objTestProcManage.getCurTestItemTimesIndex() != 0)
            {
                bSpeak = false;
            }
        }
    }


    //语音可能会阻塞当前函数，将语音设置放在最后
    if(bSpeak)
    {
        if(bWaitSpeak)
        {
            m_speak.textToSpeechBlock(qsTips);
        }
        else
        {
            m_speak.textToSpeech(qsTips);
        }
    }
}

void CTestGuideBase::setValidElementState(const short &shTestState, bool bSpeak, bool bWaitSpeak, bool bSendToUi)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    auto vecElement = pSteDev->getAllBindElements();
    unsigned int nSize = static_cast<unsigned int>(vecElement.size());
    if(nSize == 0)
    {
        LOGERROR("vecElement.size == 0")
                return;
    }

    //若是开启语音也是同步开始的开启语音，函数限定只最后一个进行语音播报
    bool bSpeakTemp;
    for(unsigned int i = 0; i < nSize; i++)
    {
        bSpeakTemp = bSpeak && i == nSize - 1;
        setTestState(shTestState, vecElement[i]->getIndex(), bSpeakTemp, bWaitSpeak, bSendToUi);
    }
}

void CTestGuideBase::forceExitTest()
{
    //若当前设备正在执行开始命令，那么退出对应
    setResendCmd();

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    //若当前设备存在正在测试的测试单元,将对应测试单元关闭即可
    auto vecBindElement = pSteDev->getAllBindElements();

    //停止所有测试单元的测试
    for(auto &element : vecBindElement)
    {
        forceExitTest(element->getIndex());
    }
}

void CTestGuideBase::forceExitTest(const unsigned short &shElementIndex)
{
    //若当前设备正在执行开始命令，那么退出对应
    auto setTemp = getDelResendCmd();
    setTemp.insert(shElementIndex);
    setDelResendCmd(setTemp);

    /***************** 确认对应的测试单元在测试 **********/
    //确定测试信息存在
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pSubElement = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getTemporaryRes::getElementInfo false");
        return;
    }

    //确认测试单元在绑定状态
    if(!pSubElement->getBindFlag())
        return;

    //确认人员信息存在
    if(pElementInfo->m_pUserInfo == nullptr)
        return;

    m_bForceComplete = true;

    CLocation location(m_iDevID, shElementIndex);

    //每一次都会将结果存入，直接发送结束即可
    emit signalTestComplete(location);

    //停止对应的测试单元
    stopTest(shElementIndex);

    m_bForceComplete = false;
}

void CTestGuideBase::changeResultToFoul(const unsigned short &shElementIndex, const short &shIndex)
{
    auto itFind = m_mapElementInfo.find(shElementIndex);

    if(itFind == m_mapElementInfo.end())
    {
        LOGERROR("m_mapElementInfo err shElementIndex ==" + to_string(shElementIndex));
        return ;
    }

    CElementInfo &elementInfo = itFind->second;

    //通过设备进行不同处理
    if(m_shDevType == DEV_TYPE_FOOTBALL || m_shDevType == DEV_TYPE_BASKETBALL)
    {
        //按钮强制结束，也需要增加一次违例，用于足球区分语音
        elementInfo.m_nIllegalTimes++;

        elementInfo.m_nFoulCode = CONTROL_STOP;

        //判断当前测试状态
        auto testState = elementInfo.m_objTestProcManage.getCurTestState();

        //已经结束则直接修改结果
        if(CTestStateManage::TEST_COMPLETED == testState)
        {
            updateTestResult(shElementIndex);
        }
        //其他则强制结束
        else
        {
            procTestEnd(shElementIndex);
        }
    }
    //立定跳远，实心球 只修改对应成绩
    else if(m_shDevType == DEV_TYPE_SOLIDBALL || m_shDevType == DEV_TYPE_SBJ)
    {
        updateTestResult(shElementIndex, shIndex);
    }
}

void CTestGuideBase::changeFoulCnt(const unsigned short &shElementIndex, bool bAddFoulCnt)
{
    //增加违例
    if(bAddFoulCnt)
    {
        setDevFoulType(shElementIndex,FoulDevFoul);
    }
    //取消违例
    else
    {
        setDevFoulType(shElementIndex,FoulDevIllegal);
    }
}

void CTestGuideBase::forceCompleteTest(const unsigned short &shElementIndex)
{
    /***************** 确认对应的测试单元在测试 **********/
    //确定测试信息存在
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pSubElement = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getTemporaryRes::getElementInfo false");
        return;
    }

    //确认测试单元在绑定状态
    if(!pSubElement->getBindFlag())
        return;

    //确认人员信息存在
    if(pElementInfo->m_pUserInfo == nullptr)
        return;

    pElementInfo->m_bAbortTest = true;

    procTestEnd(shElementIndex);

    CLocation location(m_iDevID, shElementIndex);

    //每一次都会将结果存入，直接发送结束即可
    emit signalTestComplete(location);

    if(m_shDevType != DEV_TYPE_RUN_MID)
    {
        stopTest(shElementIndex);
    }
    else
    {
        //中长跑结束不变更当前状态
        stopTest(shElementIndex, false);
    }

    pElementInfo->m_bAbortTest = false;

    CTestGuideBase::procTestEnd(shElementIndex);
}

bool CTestGuideBase::getTemporaryRes(vector<CCompleteTestResult> &ret, bool bClear)
{
    CCompleteTestResult resTemp;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    //获取当前设备存在正在测试的测试单元
    auto vecBindElement = pSteDev->getAllBindElements();

    //停止所有测试单元的测试
    if(bClear)
        ret.clear();

    size_t szSrcSize = ret.size();

    for(auto &element : vecBindElement)
    {
        if(getTemporaryRes(resTemp, element->getIndex()))
        {
            ret.push_back(resTemp);
        }
    }

    return szSrcSize != ret.size();
}

bool CTestGuideBase::getTemporaryRes(CCompleteTestResult &ret, const unsigned short &shElementIndex)
{
    //确定测试信息存在
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pSubElement = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getTemporaryRes::getElementInfo false");
        return false;
    }

    //确认测试单元在绑定状态
    if(!pSubElement->getBindFlag())
        return false;

    //确认人员信息存在
    if(pElementInfo->m_pUserInfo == nullptr)
        return false;

    ret.m_userInfo = *pElementInfo->m_pUserInfo;

    //确认存在成绩
    ret.m_vecTestResInfo.clear();

    size_t szResultSize = m_vecTestItem.size();
    for(size_t i = 0; i < szResultSize; i++)
    {
        //数据有效才存入
        CTestResultInfo &testResult = pElementInfo->m_arrTestResultInfo[i];
        if(testResult.m_strMulitResult.empty())
            continue;

        ret.m_vecTestResInfo.push_back(testResult);
    }

    return ret.m_vecTestResInfo.size() > 0;
}

void CTestGuideBase::removeCommand(unsigned short &shElementIndex)
{
    auto setTemp = getDelResendCmd();
    setTemp.insert(shElementIndex);
    setDelResendCmd(setTemp);
}

//为了保证删除时不处于发送数据的循环，有删除时直接停止
void CTestGuideBase::onResendCmdTimeOut()
{
    if(m_setResendElementIndex.size() == 0)
        return;

    //存在删除时，直接返回
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        //证明当前类对应的设备断连，停止当前测试指导活动
        setActiveState(false);
        return;
    }

    set<unsigned short> setFlase, setDelCmd;
    bool bCast;

    //使用复制值进行使用
    auto setCopyResendElementIndex = getResendCmd();

    for(auto &element : setCopyResendElementIndex)
    {
        //每次重新获取是因为发送开始命令需要一定时间，若命令在删除命令中那么略过
        setDelCmd = getDelResendCmd();
        if(setDelCmd.find(element) != setDelCmd.end())
            continue;

        switch (element) {
        case ELEMENT_NUM_CUST:
        case ELEMENT_NUM_CUR_DEV:
        {
            if(!getContinueTestFlag())
                break;

            if(!s_bDevReady)
            {
                setFlase.insert(element);
                break;
            }

            bCast = element == ELEMENT_NUM_CUST;
            if(!pSteDev->startWork(bCast, true))
            {
                setFlase.insert(element);
            }
        }
            break;
        default:
            //等待对应测试单元不存在异常才进行第二次开启
        {
            auto itFind = m_mapElementInfo.find(element);
            if(itFind == m_mapElementInfo.end())
            {
                //删除当前测试单元的开始函数
                LOGERROR("m_mapElementInfo not exist elementindex = " + to_string(element));
                break;
            }

            //标志不为true，在setFlase中不会添加，相当于自动删除
            if(!itFind->second.m_bContinueTest)
            {
                break;
            }

            //若当前需要开启的测试单元存在错误，下一次定时器到了再处理,必须等到数据更新
            if(pSteDev->existErr(element) || !pSteDev->getElement(element)->getDataFrameUpdateFlag())
            {
                setFlase.insert(element);
                break;
            }

            if(!pSteDev->startWork(element, true))
            {
                setFlase.insert(element);
            }
        }
            break;
        }
    }

    //上面已经做了过滤删除命令的操作，这里不需要再进行操作
    set<unsigned short>::iterator itFind;
    setResendCmd(setFlase,true);
}

void CTestGuideBase::setResendCmdElement(const unsigned short &shElementIndex)
{
    auto setTemp = getResendCmd();
    setTemp.insert(shElementIndex);
    m_bSendNewSerialID = true;
    setResendCmd(setTemp);
}

bool CTestGuideBase::setDevFoulType(const ushort &shElementIndex, const ushort &shCmd)
{
    auto itFind = m_mapElementInfo.find(shElementIndex);

    if(itFind == m_mapElementInfo.end())
    {
        LOGERROR("m_mapElementInfo err shElementIndex ==" + to_string(shElementIndex));
        return false;
    }

    short shDevType = getDevType();

    CElementInfo &elementInfo = itFind->second;
    auto testState = elementInfo.m_objTestProcManage.getCurTestState();

    switch (shCmd) {
    case FoulDevFoul:
        //引体、斜身、排球 使用时会增加违规次数
        if((shDevType == DEV_TYPE_PULL_UP || shDevType == DEV_TYPE_PUO || shDevType == DEV_TYPE_VOLLEYBALL)
                && (testState == CTestStateManage::TEST_IN_PROCESS || testState == CTestStateManage::TEST_COMPLETED))
        {
            //语音播报一次 违例
            m_speak.textToSpeech("违例");

            elementInfo.m_nIllegalTimes++;

            onDataChange(shElementIndex,0);

            //测试结束后，需要实时改变测试结果，确认后存入数据库
            if(testState == CTestStateManage::TEST_COMPLETED)
            {
                updateTestResult(shElementIndex);
            }
        }
        break;
        //强制停止本次测试
    case FoulDevIllegal:
        //仅足球、篮球 在接收到犯规时，强制停止测试
        if( (shDevType == DEV_TYPE_FOOTBALL || shDevType == DEV_TYPE_BASKETBALL)
                && (testState == CTestStateManage::TEST_IN_PROCESS || testState == CTestStateManage::TEST_COMPLETED))
        {

            m_speak.textToSpeech("是否确定犯规");
            //发送信号弹窗提示是否结束（中考逻辑）
//            emit sigStopByButtonFoul();
        }
        //引体、斜身、排球 使用时会减少违规次数
        else if((shDevType == DEV_TYPE_PULL_UP || shDevType == DEV_TYPE_PUO || shDevType == DEV_TYPE_VOLLEYBALL)
                && (testState == CTestStateManage::TEST_IN_PROCESS || testState == CTestStateManage::TEST_COMPLETED))
        {
            //语音播报一次 取消违例
            m_speak.textToSpeech("取消违例");

            if(elementInfo.m_nIllegalTimes == 0)
            {
                elementInfo.m_nIllegalTimes = 0;
            }
            else
            {
                elementInfo.m_nIllegalTimes--;
            }

            onDataChange(shElementIndex,0);

            //测试结束后，需要实时改变测试结果，确认后存入数据库
            if(testState == CTestStateManage::TEST_COMPLETED)
            {
                updateTestResult(shElementIndex);
            }
        }
        break;
    default:
        break;
    }

    return true;
}

QString CTestGuideBase::getTestResult(bool bMaxValidRes, const CTestResultInfo &testResult)
{
    QString qsTestRes("犯规");
    string strCurData;

    bool bRet = false;
    if(bMaxValidRes)
    {
        bRet = testResult.getValidMaxTestResult(strCurData) >= 0;
    }
    else
    {
        bRet = testResult.getLastTestResult(strCurData);
    }

    if(bRet)
    {
        qsTestRes = value2DispValue(static_cast<short>(testResult.m_nItem), strCurData);
    }

    return qsTestRes;
}

void CTestGuideBase::dispTestResult(QString &qsDisp, const CElementInfo &elementInfo)
{
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        m_ui.setCurTestRet(qsDisp);
    }
    else
    {
        short shDevType = getDevType();
        if(shDevType == DEV_TYPE_UNKNOW)
        {
            LOGERROR("dispTestResult devtype err");
            return;
        }

        //异步模式和异步模式更新结果相同，根据用户id进行结果更新
        short shColumnIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);
        short shUpdateIndex = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, shDevType, TEST_ASYNC);

        if(shColumnIndex < 0)
        {
            LOGERROR("shColumnIndex < 0");
            return;
        }

        if(shUpdateIndex < 0)
        {
            LOGERROR("shUpdateIndex < 0");
            return;
        }

        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                 getUserID(elementInfo), shUpdateIndex, qsDisp);
    }
}

QString CTestGuideBase::getTestScore(bool bMax, const CTestResultInfo &testResult, const CUserInfo &userInfo)
{
    QString qsScore("");
    double lfCurScore = -1;
    short shTestItem = static_cast<short>(testResult.m_nItem);

    float fRes;
    bool bRet = testResult.getScoreRes(bMax, fRes);

    if(bRet)
    {

        lfCurScore = static_cast<double>(s_objScoreRuleInfo.getScore(shTestItem, userInfo, fRes));

        if(lfCurScore >= 0)
        {
            qsScore = QString::number(lfCurScore, 'f', 0);
        }
        else
        {
            qsScore = "没有得分指标";
        }
    }
    //非有效成绩得分为0
    else
    {
        qsScore = "0";
    }

    return qsScore;
}

void CTestGuideBase::clearTestRes(const unsigned short &shElementIndex)
{
    auto it = m_mapElementInfo.find(shElementIndex);
    if(it == m_mapElementInfo.end())
        return;

    size_t szItem = m_vecTestItem.size();
    for(size_t i = 0; i < szItem; i++)
    {
        CTestResultInfo &testRes = it->second.m_arrTestResultInfo[i];

        //删除对应的文件
        vector<string> vecPath = testRes.getProcSrc(-1);
        for(auto &element : vecPath)
        {
            CFilePath::deleteFileOrFolder(element);
        }

        testRes.clearResult(true);
    }
}

bool CTestGuideBase::isTestStart(const unsigned short &shElementIndex)
{
    bool bRet = false;
    auto it = m_mapElementInfo.find(shElementIndex);
    if(it != m_mapElementInfo.end())
    {
        bRet = it->second.m_bActuallyWorking;
    }

    return bRet;
}

void CTestGuideBase::cfgAnticheating(CElementInfo &elementInfo, CSubElementBase *pSubElement)
{
    if(elementInfo.m_pUserInfo != nullptr && pSubElement != nullptr)
    {
        if(elementInfo.m_pUserInfo->isTouristInfo())
            return;

        //不存在防作弊相机，返回
        auto &camera = CAssistDevManage::getAntiCheatingCamera();
        if(camera.getCamerIndex() < 0)
            return;

        auto pAnticheating = pSubElement->getAntiCheating();
        //项目没有防作弊
        if(pAnticheating == nullptr)
            return;

        auto &testResult = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];
        pAnticheating->setRootPath(testResult.m_strPathResourceRoot);
        pAnticheating->setCamer(&camera);
        pAnticheating->setTestItemDescribe(elementInfo.m_objTestProcManage.getcurTestItemDescribe());
        pAnticheating->start_();
    }
}

vector<CSubElementBase *> CTestGuideBase::getAllBindElements()
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    vector<CSubElementBase *> vecTemp;
    if(pSteDev != nullptr)
    {
        vecTemp = pSteDev->getAllBindElements();
    }
    else
    {
        LOGERROR("invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
    }

    return vecTemp;
}

void CTestGuideBase::devDisconnect(CSubElementBase *pElement, const CElementInfo &elementInfo)
{
    if(pElement == nullptr || pElement->getTestState() != CTestStateManage::TEST_ERR)
    {
        LOGWARN("devDisconnect == nullptr || pElement->getTestState() != CTestStateManage::TEST_ERR");
        return;
    }

    //同步测试
    if(s_shTestMode == TEST_SYNC)
    {
        //在绑定界面,触发绑定断连即可
        if(CDevDataCfg::isHomePage(m_ui.getCurPage()))
        {
            CLocation location(m_iDevID, pElement->getIndex());
            emit signalDevDisconn(location);
        }
    }
    //异步测试
    else
    {
        //没有测试结果就算没有开始测试
        //需要测试几个项目就存在几个
        bool bStartTest = false;
        size_t szResultSize = m_vecTestItem.size();
        for(size_t i = 0; i < szResultSize; i++)
        {
            //数据有效才存入
            bStartTest |= !elementInfo.m_arrTestResultInfo[i].m_strMulitResult.empty();
        }

        if(!bStartTest)
        {
            CLocation location(m_iDevID, pElement->getIndex());
            emit signalDevDisconn(location);
        }
    }
}

int CTestGuideBase::getCurTestIndex(const CElementInfo &elementInfo)
{
    auto &testProcMgr = elementInfo.m_objTestProcManage;

    //根据当前测试项目的结果
    int nRet = testProcMgr.getCurTestItemTimesIndex();
    if(testProcMgr.getCurTestState() == CTestStateManage::TEST_COMPLETED)
    {
        nRet = testProcMgr.getCurTestItemTimesIndex() - 1;
    }

    return nRet;
}



void CTestGuideBase::updateTestResult(const unsigned short &shElementIndex, short shIndex)
{
    //获取 Element
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;

    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("procTestEnd:getElementInfo false, shIndex = " + to_string(shElementIndex));
        return;
    }

    //得到最近一次的测试结果
    auto &testResultInfo = pElementInfo->m_arrTestResultInfo[pElementInfo->m_shCurTestItemIndex];

    //获取上一次结果
    int nIndex = getCurTestIndex(*pElementInfo);

    //获取当前测试次数
    nIndex = testResultInfo.getTestTimes()-1;

    string strLastResult = "";
    uint   unIllegalTimes  = 0;
    bool   bCredible = true;
    bCredible = testResultInfo.getLastTestResult(strLastResult,unIllegalTimes);

    //若为足球、篮球，则直接修改可信度
    if(m_shDevType == DEV_TYPE_FOOTBALL || m_shDevType == DEV_TYPE_BASKETBALL)
    {
        //更新存储结果
        testResultInfo.updateTestResult(static_cast<uint>(nIndex),strLastResult,false,unIllegalTimes);

        //语音提示犯规结束
        m_speak.textToSpeech("犯规结束");

        //自动跳转页面(延时2s)(中考逻辑)
//        CTestGuideBase::s_bGoToHomePage = true;
//        QTimer::singleShot(2000, this, [this](){
//            if(CTestGuideBase::s_bGoToHomePage)
//            {
//                emit this->sigGoToHomePage();
//            }
//        });
    }
    //若为实心球、立定跳远
    else if(m_shDevType == DEV_TYPE_SOLIDBALL || m_shDevType == DEV_TYPE_SBJ)
    {
        //获取指定结果
        bCredible = testResultInfo.getTestResult(static_cast<uint>(shIndex),strLastResult,unIllegalTimes);

        //更新存储结果
        testResultInfo.updateTestResult(static_cast<uint>(shIndex),strLastResult,false,unIllegalTimes);

        //修改界面显示(中考逻辑)
//        emit sigUpdateTestRes(shIndex, "犯规");
    }
    else
    {
        //增加保护，违规次数不能大于测试次数
        unsigned int nDeviceNum = stoul(strLastResult);

        if(nDeviceNum < pElementInfo->m_nIllegalTimes)
        {
            pElementInfo->m_nIllegalTimes = nDeviceNum;
        }

        //更新存储结果
        testResultInfo.updateTestResult(static_cast<uint>(nIndex),strLastResult,bCredible,pElementInfo->m_nIllegalTimes);

        //下发命令
        auto pDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
        if(pDev != nullptr)
        {
            pDev->setIllegalTimes(static_cast<int>(pElementInfo->m_nIllegalTimes));
        }

        //更新界面显示
        QString qsIllegalNum, qsModifyRes;

        if(nDeviceNum < pElementInfo->m_nIllegalTimes)
        {
            qsIllegalNum = QString::fromStdString(strLastResult);

            qsModifyRes  = "0";
        }
        else
        {
            qsIllegalNum = QString::number(pElementInfo->m_nIllegalTimes);

            qsModifyRes  = QString::number(nDeviceNum - pElementInfo->m_nIllegalTimes);
        }

//        emit sigUpdateDeviceRes(nIndex, QString::fromStdString(strLastResult));

//        emit sigUpdateViolations(nIndex, qsIllegalNum);

//        emit sigUpdateTestRes(nIndex, qsModifyRes);
    }
}

set<unsigned short> CTestGuideBase::getResendCmd()
{
    set<unsigned short> setTemp;
    m_mutexResendStartCmd.lock();
    setTemp = m_setResendElementIndex;
    m_mutexResendStartCmd.unlock();
    return setTemp;
}

void CTestGuideBase::setResendCmd(const set<unsigned short> &setTemp, bool bEndTimeOut/* = false*/)
{   
    set<unsigned short>::iterator itFind;
    m_mutexResendStartCmd.lock();
    for(auto &element : setTemp)
    {
        itFind = m_setDeleteElementIndex.find(element);
        if(itFind != m_setDeleteElementIndex.end())
        {
            m_setDeleteElementIndex.erase(itFind);
        }
    }

    //结束一次循环后，需要将新添加的命令也得到
    if(bEndTimeOut && m_bSendNewSerialID && m_shDevType == DEV_TYPE_ROPE_SKIP)
    {
        m_bSendNewSerialID = false;
        for(auto &element : setTemp)
        {
            m_setResendElementIndex.insert(element);
        }
    }
    else
    {
        m_setResendElementIndex = setTemp;
    }

    m_mutexResendStartCmd.unlock();
}

set<unsigned short> CTestGuideBase::getDelResendCmd()
{
    set<unsigned short> setTemp;
    m_mutexResendStartCmd.lock();
    setTemp = m_setDeleteElementIndex;
    m_mutexResendStartCmd.unlock();
    return setTemp;
}

void CTestGuideBase::setDelResendCmd(const set<unsigned short> &setTemp)
{
    m_mutexResendStartCmd.lock();
    m_setDeleteElementIndex = setTemp;
    m_mutexResendStartCmd.unlock();
}
