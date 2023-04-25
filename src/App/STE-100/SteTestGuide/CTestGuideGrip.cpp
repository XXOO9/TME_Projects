#include "CTestGuideGrip.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementGrip.h"

CTestGuideGrip::CTestGuideGrip()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_GRIP;
}

void CTestGuideGrip::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_GRIP);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuideGrip::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
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

    CSubElementGrip *pElement = reinterpret_cast<CSubElementGrip *>(pSubElement);
    if(pElement->existAntiCheating())
    {
        pElement->getAntiCheating()->record_();
    }

    //将数据设置到ui
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), pElement->getDataGrip()));
    dispTestResult(qsDisp, *pElementInfo);
}

void CTestGuideGrip::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
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
    bool bCredible = (elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID);

    //记录本次成绩
    CSubElementGrip *pSubElement = reinterpret_cast<CSubElementGrip *>(pElement);
    resultInfo.addTestResult(value2SaveValue(static_cast<short>(resultInfo.m_nItem), pSubElement->getDataGrip()), bCredible);

    //将数据设置到ui
    float fSbjData = static_cast<float>(pSubElement->getDataGrip());
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), fSbjData));
    dispTestResult(qsDisp, elementInfo);
}
