#include "CTestGuideVerticalJump.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementSolidball.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "CDevDataCfg.h"
using namespace TestTableCfg;

CTestGuideVerticalJump::CTestGuideVerticalJump()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_VJUMP;
}

void CTestGuideVerticalJump::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_VJUMP);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuideVerticalJump::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
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

    CSubElementSolidball *pElement = reinterpret_cast<CSubElementSolidball *>(pSubElement);
    //肺活量数据
    if(pElement->existAntiCheating())
    {
        pElement->getAntiCheating()->record_();
    }

    //将数据设置到ui
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), pElement->getData()));
    dispTestResult(qsDisp, *pElementInfo);
}

void CTestGuideVerticalJump::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
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
        if(elementInfo.m_pUserInfo->isTouristInfo())
        {
            bCredible = true;
        }
        else
        {
            bCredible =  elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID;
        }
    }

    //记录本次成绩
    CSubElementSolidball *pSubElement = reinterpret_cast<CSubElementSolidball *>(pElement);
    float fData = static_cast<float>(pSubElement->getData());
    string strResult = value2SaveValue(static_cast<short>(resultInfo.m_nItem), fData);
    resultInfo.addTestResult(strResult, bCredible);

    //将数据设置到ui
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), fData));
    dispTestResult(qsDisp, elementInfo);
}
