#include "CTestGuidePVV.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementPVV.h"
#include "CProjectEnums.h"
#include "CDatabaseManage.h"

static const float s_SampleRate = 50.0f;

CTestGuidePVV::CTestGuidePVV()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_PVV;
    m_objEFV.setSampleRate(s_SampleRate);
    m_objEFV.startThread();
    m_ui.setPVVSamplingRate(static_cast<int>(s_SampleRate));
}

CTestGuidePVV::~CTestGuidePVV()
{
    m_objEFV.stopThread();
    if(m_pDataFlowOrVelocity != nullptr)
    {
        delete [] m_pDataFlowOrVelocity;
    }
}

void CTestGuidePVV::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_PVV);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuidePVV::setLungAnlsItem(short shAnlsItem)
{
    m_shCurAnlsLungItem = shAnlsItem;
}

void CTestGuidePVV::onTestItemChanged()
{
    vector<short> vecTestItem;
    CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsVec(DEV_TYPE_PVV, vecTestItem);

    if(vecTestItem.size() > 0)
    {
        setLungAnlsItem(vecTestItem[0]);
    }

    CTestGuideBase::onTestItemChanged();
}

void CTestGuidePVV::procTestEnd(const unsigned short &shElementIndex)
{
    if(shElementIndex <= 0)
        return;
}

void CTestGuidePVV::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
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

    CSubElementPVV *pElement = reinterpret_cast<CSubElementPVV *>(pSubElement);

    //肺通气量数据设置到ui    
    auto &vecNewPoint = pElement->getDataNew();
    auto &vecAllPoint = pElement->getDataAll();
    size_t szNewDataSize = vecNewPoint.size();
    size_t szAllDataSize = vecAllPoint.size();

    if(szNewDataSize > m_lDataLen)
    {
        if(m_pDataFlowOrVelocity != nullptr)
        {
            delete [] m_pDataFlowOrVelocity;
        }

        m_pDataFlowOrVelocity = new float[szNewDataSize];
    }

    if(m_pDataFlowOrVelocity == nullptr)
        return;

    //证明是初次进行不使用上一次的值进行获取变量
    bool bHaveLastData = szNewDataSize != szAllDataSize;

    CAnalysisLungFunc::getDataVelocityAndFlow(vecNewPoint.data(), m_pDataFlowOrVelocity, CAnalysisLungFunc::Data_Velocity,
                                              static_cast<ulong>(szNewDataSize), s_SampleRate, bHaveLastData, m_fLastVelocity, m_fLastFlow);

    qDebug() << "&&&&"<< szNewDataSize;
    for(size_t i = 0; i < szNewDataSize; i++)
    {
        m_ui.appendPVVChartPoint(CProjectEnums::VT_Chart, 0, m_pDataFlowOrVelocity[i]);


        m_ui.appendPVVChartPoint(CProjectEnums::RT_Chart, 0, vecNewPoint[i]);

        if(i == szNewDataSize - 1)
        {
            m_fLastFlow = m_pDataFlowOrVelocity[i];
            m_fLastVelocity = vecNewPoint[i];
        }
    }

    //将数据加入呼吸环分析中
    m_objEFV.addNewData(vecNewPoint);

    pElement->clearNewData();
}

void CTestGuidePVV::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
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

//    //记录结果是否可信
//    bool bCredible = (elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID);

//    //记录本次成绩
//    QString qsValueDisp = value2DispValue(static_cast<short>(resultInfo.m_nItem), m_fVC);
//    resultInfo.addTestResult(qsValueDisp.toStdString(), bCredible);

//    //通过结果获取成绩，同时根据置信度判断成绩是否可用
//    QVector<QString> vecContent;
//    short shSerialNo = elementInfo.m_objTestProcManage.getCurTestItemTimesIndex();
//    vecContent.append(QString::number(shSerialNo));
//    vecContent.append(qsValueDisp);
//    CInterAction::Get_Instance().appendModelData(CInterAction::COMMON_DATA_MODEL, vecContent);


    //测试完成之后获取测试单元所有数据进行数据分析，将分析值传入数据表格
    dispTestResToUi(elementInfo, pElement);
}

void CTestGuidePVV::dispTestResToUi(CElementInfo &elementInfo, CSubElementBase *pElement)
{
    if(pElement == nullptr || elementInfo.m_pUserInfo == nullptr)
    {
        LOGERROR("pElement invalid");
        return;
    }

    auto pSubElement = reinterpret_cast<CSubElementPVV *>(pElement);
    auto &vecData = pSubElement->getDataAll();
    ulong lDataLen = static_cast<ulong>(vecData.size());

    m_ui.clearModelData(CInterAction::COMMON_DATA_MODEL);

    switch (m_shCurAnlsLungItem) {
    case TEST_ITEM_PVV_VC:
    {
        CAnlsResultVC result;
        if(!m_objAnlsLung.anlsTidalVolume(vecData.data(), lDataLen, s_SampleRate, result))
            return;

        dispResToUi(result);
    }
        break;
    case TEST_ITEM_PVV_FVC:
    {
        CAnlsResultFVC result;
        if(!m_objAnlsLung.anlsForceVitalCapacity(vecData.data(), lDataLen, s_SampleRate, result))
            return;

        dispResToUi(result);
    }
        break;
    case TEST_ITEM_PVV_MPV:
    {
        CAnlsResultMPV result;
        if(!m_objAnlsLung.anlsMaxPulmonaryVentilation(vecData.data(), lDataLen, s_SampleRate, result))
            return;

        dispResToUi(result);
    }
        break;
    default:
        break;
    }

}

void CTestGuidePVV::dispResToUi(const CAnlsResultVC &result)
{
    QStringList listContent;
    listContent.reserve(2);

    listContent[0] = "潮气量(VT) mL";
    listContent[1] = QString::number( result.TV, 'f', 1);
    appendData(listContent);

    listContent[0] = "呼吸频率(R) cpm";
    listContent[1] = QString::number( result.RR, 'f', 1);
    appendData(listContent);

    listContent[0] = "每分呼气量(VE=VT*R) ml";
    listContent[1] = QString::number( result.VE, 'f', 1);
    appendData(listContent);

    listContent[0] = "肺活量(VC) ml";
    listContent[1] = QString::number( result.VC, 'f', 1);
    appendData(listContent);

    listContent[0] = "深吸气量(IC) ml";
    listContent[1] = QString::number( result.IC, 'f', 1);
    appendData(listContent);

    listContent[0] = "补吸气量(IRV=IC-VT) ml";
    listContent[1] = QString::number( result.IRV, 'f', 1);
    appendData(listContent);

    listContent[0] = "补呼气量(ERV=VC-IC) ml";
    listContent[1] = QString::number( result.ERV, 'f', 1);
    appendData(listContent);

    listContent[0] = "深呼气量(EC=VT+ERV) ml";
    listContent[1] = QString::number( result.EC, 'f', 1);
    appendData(listContent);
}

void CTestGuidePVV::dispResToUi(const CAnlsResultFVC &result)
{
    QStringList listContent;
    listContent.reserve(2);

    listContent[0] = "用力肺活量(FVC) mL";
    listContent[1] = QString::number( result.FVC, 'f', 1);
    appendData(listContent);

    listContent[0] = "第1秒用力呼气量(FEV1) cpm";
    listContent[1] = QString::number( result.FEV1, 'f', 1);
    appendData(listContent);

    listContent[0] = "第1秒用力呼气量百分比(FEV1%=(FEV1/FVC)*100%) %";
    listContent[1] = QString::number( result.FEV1R, 'f', 1);
    appendData(listContent);

    listContent[0] = "第2秒用力呼气量(FEV2) ml";
    listContent[1] = QString::number( result.FEV2, 'f', 1);
    appendData(listContent);

    listContent[0] = "第2秒用力呼气量百分比(FEV2%=(FEV2/FVC)*100%) %";
    listContent[1] = QString::number( result.FEV2R, 'f', 1);
    appendData(listContent);

    listContent[0] = "第3秒用力呼气量(FEV3) ml";
    listContent[1] = QString::number( result.FEV3, 'f', 1);
    appendData(listContent);

    listContent[0] = "第3秒用力呼气量百分比(FEV3%=(FEV3/FVC)*100%) %";
    listContent[1] = QString::number( result.FEV3R, 'f', 1);
    appendData(listContent);

    listContent[0] = "呼气时间(秒) s";
    listContent[1] = QString::number( result.EXTIME, 'f', 1);
    appendData(listContent);
}

void CTestGuidePVV::dispResToUi(const CAnlsResultMPV &result)
{
    QStringList listContent;
    listContent.reserve(2);

    listContent[0] = "每分平静通气量(VE=TV1*R1) ml/min";
    listContent[1] = QString::number( result.VE, 'f', 1);
    appendData(listContent);

    listContent[0] = "最大通气量(MVV=TV2*R2) ml/min";
    listContent[1] = QString::number( result.MVV, 'f', 1);
    appendData(listContent);

    listContent[0] = "通气储备百分比(VSP=(MVV-VE)/MVV*100%) %";
    listContent[1] = QString::number( result.MVV, 'f', 1);
    appendData(listContent);
}

void CTestGuidePVV::appendData(const QStringList &list)
{
    m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, list);
}
