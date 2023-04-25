#include "CTestGuideVC.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementVC.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "CDevDataCfg.h"
using namespace TestTableCfg;

CTestGuideVC::CTestGuideVC()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_VC;
}

void CTestGuideVC::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_VC);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuideVC::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
{
    CSubElementBase *pSubElement = nullptr;
    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo false");
        return;
    }

    bool &bActuallyWorking = pElementInfo->m_bActuallyWorking;

    //底层硬件未真正经历过开始
    if(!bActuallyWorking)
    {
        return;
    }

    CSubElementVC *pElement = reinterpret_cast<CSubElementVC *>(pSubElement);
    //肺活量数据
    if(pElement->existAntiCheating())
    {
        pElement->getAntiCheating()->record_();
    }

    //将数据设置到ui
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), pElement->getDataVC()));
    dispTestResult(qsDisp, *pElementInfo);
}

void CTestGuideVC::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
{
    if(pElement == nullptr)
    {
        LOGERROR("pElement invalid");
        return;
    }

    auto &resultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];

    //若存在测试过程防作弊资源，需要进行资源的处理
    if(!resultInfo.m_strPathResourceRoot.empty())
    {
        dealTestAntiCheating(resultInfo, pElement);
    }

    //记录结果是否可信
    bool bCredible = false;
    if(elementInfo.m_pUserInfo == nullptr)
    {
        LOGERROR("elementInfo.m_pUserInfo = nullptr");
        bCredible = false;
    }
    else
    {
        //游客模式结果为真(直连和用户是游客)
        if(CDevDataCfg::getDevDirectConnectFlag() && elementInfo.m_pUserInfo->isTouristInfo())
        {
            bCredible = true;
        }
        else
        {
            bCredible =  elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID;
        }
    }

    //默认肺活量全是有效成绩
    bCredible = true;

    //记录本次成绩
    CSubElementVC *pSubElement = reinterpret_cast<CSubElementVC *>(pElement);
    QString qsValueDisp = value2DispValue(static_cast<short>(resultInfo.m_nItem), pSubElement->getDataVC());
    resultInfo.addTestResult(qsValueDisp.toStdString(), bCredible);

    //将数据设置到ui
    float fSbjData = static_cast<float>(pSubElement->getDataVC());
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), fSbjData));
    dispTestResult(qsDisp, elementInfo);
}
