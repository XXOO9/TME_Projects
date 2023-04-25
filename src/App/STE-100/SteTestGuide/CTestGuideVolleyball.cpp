#include "CTestGuideVolleyball.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementVolleyball.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "CDevDataCfg.h"
using namespace TestTableCfg;

CTestGuideVolleyball::CTestGuideVolleyball()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_VOLLEYBALL;
}

void CTestGuideVolleyball::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_VOLLEYBALL);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuideVolleyball::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
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

    CSubElementVolleyball *pElement = reinterpret_cast<CSubElementVolleyball *>(pSubElement);
    //肺活量数据
    if(pElement->existAntiCheating())
    {
        pElement->getAntiCheating()->record_();
    }

    //将数据设置到ui
    QString qsDisp(value2DispValue(pElement->getCurTestItem(), pElement->getData()));
    dispTestResult(qsDisp, *pElementInfo);
}

void CTestGuideVolleyball::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
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
        //游客模式结果为真
        if(elementInfo.m_pUserInfo->isTouristInfo())
        {
            bCredible = true;
        }
        else
        {
            bCredible =  elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID;
        }
    }

    //排球不存在犯规
    bCredible = true;

    //中考体测逻辑
    //dealCompleteTestFoulDisp(bCredible, getCurTestIndex(elementInfo));

    //记录本次成绩
    CSubElementVolleyball *pSubElement = reinterpret_cast<CSubElementVolleyball *>(pElement);
    float fData = static_cast<float>(pSubElement->getData());

    string strResult = value2SaveValue(static_cast<short>(resultInfo.m_nItem), fData);

    //排球限制犯规次数不大于结果次数
    if(static_cast<unsigned int>(fData) < elementInfo.m_nIllegalTimes)
    {
        elementInfo.m_nIllegalTimes = static_cast<unsigned int>(fData);
    }

    resultInfo.addTestResult(strResult, bCredible, elementInfo.m_nIllegalTimes);

    //将数据设置到ui显示(设备数据、违例次数、最终结果)
    QString qsDeviceData = value2DispValue(pElement->getCurTestItem(), fData);

    dispTestResult(qsDeviceData, elementInfo);
}
