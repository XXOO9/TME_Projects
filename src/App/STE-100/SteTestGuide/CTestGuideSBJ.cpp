#include "CTestGuideSBJ.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementSBJ.h"

CTestGuideSBJ::CTestGuideSBJ()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_SBJ;
}

void CTestGuideSBJ::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_SBJ);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuideSBJ::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
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

    CSubElementSBJ *pElement = reinterpret_cast<CSubElementSBJ *>(pSubElement);

    //立定跳远数据
    //将数据设置到ui
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), pElement->getDataSBJ()));
    dispTestResult(qsDisp, *pElementInfo);
}

void CTestGuideSBJ::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
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
    bool bCredible = elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID;

    CSubElementSBJ *pSubElement = reinterpret_cast<CSubElementSBJ *>(pElement);

    //记录本次成绩
    string strValueDisp = value2SaveValue(static_cast<short>(resultInfo.m_nItem), pSubElement->getDataSBJ());
    resultInfo.addTestResult(strValueDisp, bCredible);

    //将数据设置到ui
    float fSbjData = static_cast<float>(pSubElement->getDataSBJ());
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), fSbjData));
    dispTestResult(qsDisp, elementInfo);
}
