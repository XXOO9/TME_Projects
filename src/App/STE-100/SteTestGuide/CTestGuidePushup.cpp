#include "CTestGuidePushup.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementPushup.h"

CTestGuidePushup::CTestGuidePushup()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_PUSH_UP;
}

void CTestGuidePushup::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_PUSHUP);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuidePushup::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
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

    CSubElementPushup *pElement = reinterpret_cast<CSubElementPushup *>(pSubElement);
    if(pElement->existAntiCheating())
    {
        pElement->getAntiCheating()->record_();
    }

    //俯卧撑数据
    //将数据设置到ui
    QString qsDisp = value2DispValue(pElement->getCurTestItem(), static_cast<float>(pElement->getDataPushup()));
    dispTestResult(qsDisp, *pElementInfo);
}

void CTestGuidePushup::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
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

    CSubElementPushup *pSubElement = reinterpret_cast<CSubElementPushup *>(pElement);
    //记录本次成绩
    QString qsValueDisp = value2DispValue(static_cast<short>(resultInfo.m_nItem), static_cast<float>(pSubElement->getDataPushup()));
    resultInfo.addTestResult(qsValueDisp.toStdString(), bCredible);

    //将数据设置到ui
    float fSbjData = static_cast<float>(pSubElement->getDataPushup());
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), fSbjData));
    dispTestResult(qsDisp, elementInfo);
}
