#include "CTestGuideSkip.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementRopeSkip.h"
#include "CDevDataCfg.h"
using namespace TestTableCfg;

atomic<bool> CTestGuideSkip::s_bFirstStartFlag{true};

CTestGuideSkip::CTestGuideSkip()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_ROPE_SKIP;
}

CTestGuideSkip::~CTestGuideSkip()
{

}

void CTestGuideSkip::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_ROPE_SKIP);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

bool CTestGuideSkip::checkSkipBindState(const unsigned int &nCardId)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    auto pDevSkip = reinterpret_cast<CDevSteSkip *>(pSteDev);

    return pDevSkip->checkSkipBindState(nCardId);
}

void CTestGuideSkip::stopUpdate(const unsigned short &shElementIndex)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));

    if(pElement != nullptr)
    {
        pElement->stopUpdate();
    }
}

bool CTestGuideSkip::checkStartSuccess(const unsigned short &shElementIndex)
{
    bool bFlag = true;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return bFlag;
    }

    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));

    if(pElement != nullptr)
    {
        bFlag = pElement->getStartSuccessFlag();
    }

    return bFlag;
}

int CTestGuideSkip::getSkipTestState(const unsigned short &shElementIndex)
{
    int iState = -1;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return iState;
    }

    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));

    if(pElement != nullptr)
    {
        iState = pElement->getTestState();
    }

    return iState;
}

void CTestGuideSkip::setRestartFlag(const unsigned short &shElementIndex, const bool &bFlag)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));

    if(pElement != nullptr)
    {
        pElement->setRestartFlag(bFlag);
    }
}

bool CTestGuideSkip::getRestartFlag(const unsigned short &shElementIndex)
{
    bool bFlag = true;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return bFlag;
    }

    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));

    if(pElement != nullptr)
    {
        bFlag = pElement->getRestartFlag();
    }

    return bFlag;
}

void CTestGuideSkip::forcedStopTest(const unsigned short &shElementIndex)
{
    //0.获取基本属性
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;

    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("procTestEnd:getElementInfo false, shIndex = " + to_string(shElementIndex));
        return;
    }

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }
    auto pDevSkip = reinterpret_cast<CDevSteSkip *>(pSteDev);

    CSubElementRopeSkip *pSubElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));
    if(nullptr == pSubElement)
    {
        LOGERROR("Skip SubElement Invalid");
        return;
    }

    //1.若当前设备正在执行开始命令，则需要先退出
    m_mutexResendStartCmd.lock();
    m_setDeleteElementIndex.insert(shElementIndex);
    m_mutexResendStartCmd.unlock();

    //2.停止设备工作
    if(pDevSkip != nullptr)
    {
        pDevSkip->stopWork(shElementIndex);
    }

    //3.设置测试状态为 超时结束
    setTestState(CTestStateManage::TEST_TIMEOUT, shElementIndex, false, false, false);
}

bool CTestGuideSkip::bindUserToElement(const unsigned short &shElementIndex, const CUserInfo &userInfo)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));

    if(nullptr == pElement)
    {
        LOGERROR("Skip SubElement Invalid");
        return false;
    }

    pElement->setElementID(userInfo.m_nExtraInfoID);

    return  CTestGuideBase::bindUserToElement(shElementIndex, userInfo);
}

//单个绳子测试结束(同步、异步均调用此函数结束)
void CTestGuideSkip::procTestEnd(const unsigned short &shElementIndex)
{
    //0.获取基本属性
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;

    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("procTestEnd:getElementInfo false, shIndex = " + to_string(shElementIndex));
        return;
    }

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    CSubElementRopeSkip *pSubElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));
    if(nullptr == pSubElement)
    {
        LOGERROR("Skip SubElement Invalid");
        return;
    }

    //1.停止测试
    pSubElement->stopTest();

    //2.修改测试状态为完成
    setTestState(CTestStateManage::TEST_COMPLETED, shElementIndex, false, false, false);

    //3.保存测试结果
    saveDataToResult(shElementIndex);

    static CLocation sLocation;
    sLocation.m_shElementIndex = pElement->getIndex();
    sLocation.m_iSteDevMacID = m_iDevID;

    //4.同步模式直接结束
    if(s_shTestMode == TEST_SYNC)
    {
        signalTestComplete(sLocation);
        return;
    }

    //5.完成测试(转移测试结果)，异步表格中删除
    unsigned int uiExtra = pElement->getElementID();
    short shIndex = CDevDataCfg::getColTypeIndex(COL_DEV_ID, DEV_TYPE_ROPE_SKIP, TEST_ASYNC);
    m_ui.deleteModelData(CInterAction::COMMON_DATA_MODEL, shIndex, QString::number(uiExtra));

    /*
    //将目前所有结果传到结果表格(设备、姓名、准考、结果、成绩)
    QStringList listContent;
    listContent = CDevDataCfg::getTableContent(sLocation, TEST_SYNC, *pElementInfo->m_pUserInfo);

    //结果采用同步的表格，
    short shTestResult = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, DEV_TYPE_ROPE_SKIP, TEST_SYNC);
    QString qsTestResult = QString::number(pSubElement->getCurrentRope());
    if(shTestResult >= 0 && shTestResult < listContent.size())
    {
        listContent[shTestResult] = qsTestResult;
    }

    //需要在结果后添加得分,暂为空
    float  fValue = static_cast<float>(pSubElement->getCurrentRope());
    double dScore = static_cast<double>(CTestGuideBase::getScoreInfo().getScore(TEST_ITEM_ROPE_SKIP,*pElementInfo->m_pUserInfo,fValue));
    if(dScore < 0)
    {
        listContent.append(QString::number(0));
    }
    else
    {
        listContent.append(QString::number(dScore));
    }

    m_ui.appendModelData(CInterAction::STUDENT_DATA_MODEL, listContent);
    */

    signalTestComplete(sLocation);
}

void CTestGuideSkip::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
{
    //0.安全保护
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    //1.得到子设备
    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));
    if(nullptr == pElement)
    {
        LOGERROR("Skip SubElement Invalid");
        return;
    }

    //2.更改界面显示信息
    short shIndex = CDevDataCfg::getColTypeIndex(COL_DEV_ID, DEV_TYPE_ROPE_SKIP, TEST_ASYNC);
    short shTestRest = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, DEV_TYPE_ROPE_SKIP, TEST_ASYNC);
    short shDevPower = CDevDataCfg::getColTypeIndex(COL_DEV_POWER, DEV_TYPE_ROPE_SKIP, TEST_ASYNC);

    unsigned int uiNum = pElement->getCurrentRope();
    unsigned int uiExtra = pElement->getElementID();
    m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shIndex, \
                             QString::number(uiExtra), shTestRest, QString::number(uiNum));

    unsigned char ucPower = pElement->getPowerNum();
    m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shIndex, \
                             QString::number(uiExtra), shDevPower, QString::number(ucPower));


    //3.针对状态，进行对应逻辑处理
    //设置状态为 测试中
    if(cDataStstus == RopeSkipStatus::SKIP_IN_TEST)
    {
        setTestState(CTestStateManage::TEST_CHECKED, shElementIndex, false, false, true);
        setTestState(CTestStateManage::TEST_IN_PROCESS, shElementIndex, false, false, true);
    }
    //逻辑处理 测试完成
    else if(cDataStstus == RopeSkipStatus::SKIP_COMPLETE_TEST)
    {
        procTestEnd(shElementIndex);
    }
}

void CTestGuideSkip::saveDataToResult(const unsigned short &shElementIndex)
{
    CSubElementBase *pSubElement = nullptr;
    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo false");
        return;
    }

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    CSubElementRopeSkip *pElement = reinterpret_cast<CSubElementRopeSkip *>(pSteDev->getElement(shElementIndex));
    if(nullptr == pElement)
    {
        LOGERROR("Skip SubElement Invalid");
        return;
    }

    auto &resultInfo = pElementInfo->m_arrTestResultInfo[pElementInfo->m_shCurTestItemIndex];

    if(!resultInfo.m_strPathResourceRoot.empty())
    {
        dealTestAntiCheating(resultInfo, pSubElement);
    }

    //记录结果是否可信(跳绳只判断标志是否准确即可)
    bool bCredible = false;
    if(pElementInfo->m_pUserInfo == nullptr)
    {
        LOGERROR("elementInfo.m_pUserInfo = nullptr");
        bCredible = false;
    }
    else
    {
        bCredible =  pElementInfo->m_nFoulCode == NOT_FOUL && pElementInfo->m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID;
    }

    //记录本次成绩
    unsigned int uiNum = pElement->getCurrentRope();
    string strSaveValue = value2SaveValue(static_cast<short>(resultInfo.m_nItem), uiNum);
    resultInfo.addTestResult(strSaveValue, bCredible);
}
