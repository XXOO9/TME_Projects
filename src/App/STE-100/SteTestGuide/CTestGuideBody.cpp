#include "CTestGuideBody.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementBody.h"
#include "CDevDataCfg.h"
#include "CDatabaseManage.h"
using namespace TestTableCfg;

static const char g_cHBCSplite('-');

CTestGuideBody::CTestGuideBody()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_BODY;
}

void CTestGuideBody::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(pSteDev != nullptr)
    {
        onTestItemChanged();
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuideBody::getHBCTestRes(const float &fHeight, const float &hWeight, bool bGender, short shAge, const string &strHBCRes, const vector<short> &vecAnalysisItem, vector<CAnalysisHBC::HBCComponentDes> &vecRes)
{
    vector<double> vecHBCValue;
    getDeserializationHBC(vecHBCValue, strHBCRes);

    CAnalysisHBC analysisHBC;

    bool bRet = analysisHBC.getAnalysisResult(vecAnalysisItem, vecRes, static_cast<double>(fHeight), \
                                  bGender, shAge, static_cast<double>(hWeight), vecHBCValue);

    if(!bRet)
    {
        LOGERROR("getAnalysisResult false");
    }
}

void CTestGuideBody::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
{
    CSubElementBase *pSubElement = nullptr;
    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo false");
        return;
    }

    CSubElementBody *pElement = reinterpret_cast<CSubElementBody *>(pSubElement);
    if(pElement == nullptr)
    {
        LOGERROR("pElement invalid");
        return;
    }

    if(!pElement->getStartFlag())
    {
        return;
    }

    //身高体重的数据消息，只有身高和体重需要进行实时显示，人体成分数据在结束时才进行显示
    auto &shTestItem = pElement->getCurTestItem();

    //身高体重数据采用 float进行填充,每次同时获取身高和体重
    if(shTestItem != TEST_ITEM_HEIGHT && shTestItem != TEST_ITEM_WEIGHT)
        return;

    m_fHeight = pElement->getResHeight();
    m_fWeight = pElement->getResWeight();

    //体重数据超过5kg之后开始测试身高,当前身高的标志为未测试
//    if(m_fWeight >= 5 && m_bFirstWeight)
//    {
//        m_bFirstWeight = false;
//        emit signaldealTestProc(pElement->getIndex(), false, true);
//    }

    //将值设置到ui
    QString qsDispHegiht = value2DispValue(TEST_ITEM_HEIGHT, m_fHeight);
    QString qsDispWeight = value2DispValue(TEST_ITEM_WEIGHT, m_fWeight);

    //判断身高体重是否符合实际范围，如果不符合不进行处理
    if(!isDataCorrent(m_fHeight, TEST_ITEM_HEIGHT)) // cm
        qsDispHegiht = "";

    if(!isDataCorrent(m_fWeight, TEST_ITEM_WEIGHT)) //kg
        qsDispWeight = "0";

    //设备直连
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        m_ui.setCurBodyTestRet(qsDispWeight, qsDispHegiht);
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

        short shUpdateWeight = CDevDataCfg::getColTypeIndex(COL_WEIGHT, shDevType, TEST_ASYNC);
        short shUpdateHeight = CDevDataCfg::getColTypeIndex(COL_HEIGHT, shDevType, TEST_ASYNC);
        //更新体重
        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                                             getUserID(*pElementInfo), shUpdateWeight, qsDispWeight);
        //更新身高
        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                                             getUserID(*pElementInfo), shUpdateHeight, qsDispHegiht);
    }
}

void CTestGuideBody::procTestBegin(const unsigned short &shElementIndex)
{
    CSubElementBase *pSubElement;
    CElementInfo *pElementInfo;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo err shElementIndex ==" + to_string(shElementIndex));
        return;
    }

    if(pSubElement->getCurTestItem() == TEST_ITEM_WEIGHT)
    {
        m_bFirstWeight = true;
        m_fHeight = 0.0f;
    }

    bool &bActuallyWorking = pElementInfo->m_bActuallyWorking;
    if(!bActuallyWorking)
    {
        bActuallyWorking = true;
        //真正开始给ui置空显示即可(所有直连设备适用)
        if(CDevDataCfg::getDevDirectConnectFlag())
        {
            m_ui.setCurTestRet(DEFAULT_DISP_VALUE);
        }
        else
        {
            short shDevType = getDevType();
            short shColumnIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);
            short shUpdateHeight = CDevDataCfg::getColTypeIndex(COL_HEIGHT, shDevType, TEST_ASYNC);
            short shUpdateWeight = CDevDataCfg::getColTypeIndex(COL_WEIGHT, shDevType, TEST_ASYNC);

            //身高体重数据需要特殊处理
            if(shColumnIndex >= 0 && shUpdateHeight >= 0 && shUpdateWeight >= 0)
            {
                m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                         getUserID(*pElementInfo), shUpdateHeight, DEFAULT_DISP_VALUE);
                m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                         getUserID(*pElementInfo), shUpdateWeight, DEFAULT_DISP_VALUE);
            }
        }
    }
}

void CTestGuideBody::procTestEnd(const unsigned short &shElementIndex)
{
    //本次测试经历了软件设置开始，底层硬件实际开始 底层硬件结束才是一次有效的测试
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;

    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("procTestEnd:getElementInfo false, shIndex = " + to_string(shElementIndex));
        return;
    }

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("procTestEnd:test completed but dev invalide invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    CElementInfo &elementInfo = *pElementInfo;

    if(elementInfo.m_pUserInfo == nullptr)
        return;

    auto szCompleteItemSize = m_setCompleteItem.size();
    //不是超时结束（需要在界面上显示超时） 和 强制结束（需要考虑下） 才进行数据保存
    if(elementInfo.m_cHardwareDataStateCode != DEV_TEST_STATE_TIME_OUT_NOT_TEST && !m_bForceComplete)
    {
        //这一块处理实际的数据需要在不同的继承类中进行处理，肺活量 身高体重等相同处理逻辑的设备，存储的数据格式不同
        saveDataToTestResult(elementInfo, pElement);
    }

    //判断没有完成项目直接返回
    if(szCompleteItemSize == m_setCompleteItem.size())
    {
        return;
    }

    //判断是否所有测试项都进行了测试
    bool bCompletedTest = m_setCompleteItem.size() == m_vecTestItem.size();

    //超时也算完成
    bCompletedTest |= (elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_TIME_OUT_NOT_TEST);

    //清除上次所有错误
    pElement->clearErr();

    //处理本次完成结果,显示到ui 发送到外面进行统一保存
    bCompletedTest = true;
    CSubElementBody *pElementBody = reinterpret_cast<CSubElementBody *>(pElement);
    for(auto &element : m_vecTestItem)
    {
        bCompletedTest &= pElementBody->isCompleteFlagSame(element);
    }

    procTestResult(elementInfo, pElement, bCompletedTest);

    //强制完成直接结束
    if(bCompletedTest || m_bForceComplete)
    {
        //测试完成设置开始标记为false
        pElementInfo->m_bActuallyWorking = false;

        //正常完成不再需要发送停止命令，身高体重仪器出现命令发送不成功概率较高
//        if(m_bForceComplete)
//        {
//            for(int i = 0; i < 3; i++)
//            {
        pSteDev->stopWork(shElementIndex, false);
//            }
//        }

        setTestState(CTestStateManage::TEST_COMPLETED, elementInfo);
        //直连模式语音结束(强制结束不进行语音播报)
        if(CDevDataCfg::getDevDirectConnectFlag() && !m_bForceComplete)
        {
            CTextToSpeech::GetInstance().textToSpeech(elementInfo.m_objTestProcManage.getTestProcContent());
        }
        return;
    }
    else
    {
        LOGINFO("dev:" + to_string(m_iDevID) + "  elementIndex:" + to_string(pElement->getIndex()) + "completed signal test");
    }
}

void CTestGuideBody::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
{
    if(pElement == nullptr)
    {
        LOGERROR("pElement invalid");
        return;
    }

    CSubElementBody *pElementBody = reinterpret_cast<CSubElementBody *>(pElement);

    //判断是否是身高测试完成 当前测试结果为空同时测试状态为完成状态
    //身高和体重完成测试才进行人体阻抗的设置
    auto *pTestResHeight = getTestRes(elementInfo, TEST_ITEM_HEIGHT);
    auto *pTestResWeight = getTestRes(elementInfo, TEST_ITEM_WEIGHT);
    auto *pTestResHBC    = getTestRes(elementInfo, TEST_ITEM_HBC);
//    if(pTestResHeight == nullptr || pTestResWeight == nullptr || pTestResHBC == nullptr)
//    {
//        LOGERROR("element not Exist height or weight");
//        return;
//    }

    string strRes;
    //若是身高完成测试
    if(pTestResHeight != nullptr && pElementBody->isCompleteFlagDif(TEST_ITEM_HEIGHT))
    {
        strRes = value2SaveValue(static_cast<short>(pTestResHeight->m_nItem), pElementBody->getResHeight());
        pTestResHeight->m_strMulitResult.clear();
        pTestResHeight->addTestResult(strRes, true);
        m_setCompleteItem.insert(TEST_ITEM_HEIGHT);

        //保证身高测试完成
        pElementBody->setComplete(TEST_ITEM_HEIGHT);
    }

    //若是体重完成测试
    if(pTestResWeight != nullptr && pElementBody->isCompleteFlagDif(TEST_ITEM_WEIGHT))
    {
        strRes = value2SaveValue(static_cast<short>(pTestResHeight->m_nItem), pElementBody->getResWeight());
        pTestResWeight->m_strMulitResult.clear();
        pTestResWeight->addTestResult(strRes, true);
        m_setCompleteItem.insert(TEST_ITEM_WEIGHT);

        //保证体重测试完成
        pElementBody->setComplete(TEST_ITEM_WEIGHT);

        //体重测试完成发送测试身高
        emit signaldealTestProc(pElement->getIndex(), false, true);
    }

    //身高体重测试完成，同时存在人体阻抗，开始测试人体阻抗
    if(pTestResWeight != nullptr && pTestResHeight != nullptr && !pTestResHeight->m_strMulitResult.empty() && !pTestResWeight->m_strMulitResult.empty()
            && pElementBody->isCompleteFlagSame(TEST_ITEM_HEIGHT) && pElementBody->isCompleteFlagSame(TEST_ITEM_WEIGHT))
    {
        auto itFind = find(m_vecTestItem.begin(), m_vecTestItem.end(), TEST_ITEM_HBC);
        if(itFind != m_vecTestItem.end())
            emit signaldealTestProc(pElement->getIndex(), false, true);
    }

    //若是人体阻抗完成测试
    if(pElementBody->isCompleteFlagDif(TEST_ITEM_HBC) /*&& pTestResHBC->m_strMulitResult.empty()*/)
    {
        //记录结果是否可信
        bool bCredible = (elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID);

        //人体成分存在结果,存储在结果中存储人体阻抗值
        //根据当前配置获取要测试的项（人体成分需要根据中低高配进行配置）
        float arrHBC[CElementHBC::HBC_DATA_NUM];
        unsigned int nSize = pElementBody->getResHBC(arrHBC, CElementHBC::HBC_DATA_NUM);

        if(nSize != 0)
        {
            getSerializationHBC(strRes, arrHBC, CElementHBC::HBC_DATA_NUM);
            pTestResHBC->m_strMulitResult.clear();
            pTestResHBC->addTestResult(strRes, bCredible);
        }

        m_setCompleteItem.insert(TEST_ITEM_HBC);
        //保证体成分测试完成
        pElementBody->setComplete(TEST_ITEM_HBC);
    }

    //若完成测试
    if(m_setCompleteItem.size() == m_vecTestItem.size())
    {
        setResendCmd();
    }
}

void CTestGuideBody::procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete)
{
    auto &testResultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];
    if(testResultInfo.m_strMulitResult.empty())
    {
        return;
    }

    //通过结果获取成绩，同时根据置信度判断成绩是否可用
    QStringList listContent;

    //同步异步测试表格相同，将测试结果更新到界面
    static CLocation sLocation;
    sLocation.m_iSteDevMacID = m_iDevID;
    sLocation.m_shElementIndex = pElement->getIndex();

    //身高体重
    QString qsWeight(DEFAULT_DISP_VALUE), qsHeight(DEFAULT_DISP_VALUE);

    auto pTestResHeight = getTestRes(elementInfo, TEST_ITEM_HEIGHT);
    auto pTestResWeight = getTestRes(elementInfo, TEST_ITEM_WEIGHT);
//    if(pTestResHeight == nullptr || pTestResWeight == nullptr)
//    {
//        LOGERROR("pTestResHeight or pTestResWeight == nullptr");
//        return;
//    }

    if(pTestResHeight != nullptr)
    {
        if(!pTestResHeight->m_strMulitResult.empty())
        {
            qsHeight = getTestResult(false, *pTestResHeight);
        }
    }

    if(pTestResWeight != nullptr)
    {
        if(!pTestResWeight->m_strMulitResult.empty())
        {
            qsWeight = getTestResult(false, *pTestResWeight);
        }
    }

    //更新ui界面上的显示
    m_ui.setCurBodyTestRet(qsWeight, qsHeight);

    string strRes;
    qDebug() << "$$$" << QString::fromStdString(testResultInfo.m_strMulitResult);
    bool bValid = testResultInfo.getLastTestResult(strRes);

    //直连
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        //游客用户显示相同  描述 测试值
        for(auto &completeItem : m_setCompleteItem)
        {
            listContent.clear();
            switch (completeItem) {
            case TEST_ITEM_HEIGHT:
            {
                listContent.append("身高(cm)");
                listContent.append(qsHeight);
                m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, listContent);
            }
                break;
            case TEST_ITEM_WEIGHT:
            {
                listContent.append("体重(kg)");
                listContent.append(qsWeight);
                m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, listContent);
            }
                break;
            }
        }

        //身高体重测试完成设置提交按钮可用
        if(elementInfo.m_shCurTestItemIndex >= 1)
        {
            m_ui.setBtnEnableCommit(true);
        }

        m_setCompleteItem.clear();

        if(testResultInfo.m_nItem == TEST_ITEM_HBC)
        {
            short shAge = getHBCAge(elementInfo.m_pUserInfo);

            vector<short> vecAnalysisItem;
            getHBCItem(vecAnalysisItem);
            vector<CAnalysisHBC::HBCComponentDes> vecRes;

            //传入计算的单位为cm，原始单位为mm
            getHBCTestRes(qsHeight.toFloat(), qsWeight.toFloat(), \
                          elementInfo.m_pUserInfo->m_bGender, shAge \
                          , strRes, vecAnalysisItem, vecRes);

            for(auto &element : vecRes)
            {
                listContent.clear();

                if(element.shAnalysisItem == CAnalysisHBC::HBC_BMI)
                {
                    listContent.append("BMI");
                    //listContent.append("BMI(" + element.qsDescribe + ")");
                }
                else
                {
                    if(element.qsUnit.isEmpty())
                    {
                        listContent.append(element.qsDescribe);
                    }
                    else
                    {
                        listContent.append(element.qsDescribe + "(" + element.qsUnit + ")");
                    }
                }

                //数据有效采用数据值，数据无效采用 - （身高体重仪只对人体阻抗进行有效值判断）
                if(bValid)
                {
                    listContent.append(QString::number(element.lfResult, 'f', 2));
                }
                else
                {
                    listContent.append("-");
                }

                m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, listContent);
            }
        }
    }
    //组网
    else
    {
        short shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testResultInfo.m_nItem));
        if(shDevType == DEV_TYPE_UNKNOW)
        {
            LOGERROR("getDevTypeByTestItem false testitem = " + to_string(testResultInfo.m_nItem));
            return;
        }

        listContent = CDevDataCfg::getTableContent(sLocation, TEST_ASYNC, *elementInfo.m_pUserInfo);

        short shUserIDIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);

        short shHeightIndex = CDevDataCfg::getColTypeIndex(COL_HEIGHT, shDevType, TEST_ASYNC);
        short shWeightIndex = CDevDataCfg::getColTypeIndex(COL_WEIGHT, shDevType, TEST_ASYNC);

        if(shUserIDIndex < 0 || shHeightIndex < 0 || shWeightIndex < 0)
        {
            LOGERROR("shUserIDIndex : " + to_string(shUserIDIndex) + "shHeightIndex : " + to_string(shHeightIndex) +"shWeightIndex : " + to_string(shWeightIndex));
            return;
        }

        QString qsUserID = getUserID(elementInfo);
        listContent[shHeightIndex] = qsHeight;
        listContent[shWeightIndex] = qsWeight;

        //根据犯规类型显示成绩
        m_ui.updateModelRowData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, qsUserID, listContent);

        //异步模式，将结果放到结果表格
        if(bComplete && s_shTestMode == TEST_ASYNC)
        {
            //删除默认表格中的内容
            m_ui.deleteModelData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, qsUserID);
        }
    }

    if(bComplete)
    {
        signalTestComplete(sLocation);
    }
}

CTestResultInfo *CTestGuideBody::getTestRes(CElementInfo &elementInfo, short shTestItem)
{
    CTestResultInfo *pTestResult = nullptr;
    for(int i = 0; i < CElementInfo::TestResultSize; i++)
    {
        if(elementInfo.m_arrTestResultInfo[i].m_nItem == shTestItem)
        {
            pTestResult = &elementInfo.m_arrTestResultInfo[i];
        }
    }

    return pTestResult;
}

void CTestGuideBody::getHBCItem(vector<short> &vecAnalysisItem)
{
    vecAnalysisItem.clear();
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_BMI);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_BODY_FAT_PERCENT);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_HEAD_WEIGHT);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_HEAD_FAT);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_HEAD_MUSCLE);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_TBMM);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_WBIS_);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_TBW);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_WHR);
    vecAnalysisItem.push_back(CAnalysisHBC::HBC_VFA);
}

void CTestGuideBody::onTestItemChanged()
{
    vector<short> vecTestItem;
    CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsVec(DEV_TYPE_BODY, vecTestItem);

    bool bTestHBC = std::find(vecTestItem.begin(), vecTestItem.end(), TEST_ITEM_HBC) != vecTestItem.end();

    m_vecTestItem.clear();
    m_vecTestItem.push_back(TEST_ITEM_WEIGHT);
    m_vecTestItem.push_back(TEST_ITEM_HEIGHT);
    if(bTestHBC)
    {
        m_vecTestItem.push_back(TEST_ITEM_HBC);
    }

    LOGINFO("body test item size = " + toString(m_vecTestItem.size()));

    CTestGuideBase::onTestItemChanged();
}

short CTestGuideBody::getHBCAge(const CUserInfo *pUserInfo)
{
    short shAge = 24;

    if(pUserInfo != nullptr && !pUserInfo->isTouristInfo())
    {
        shAge = pUserInfo->getAge();
    }
    if(shAge < 0)
    {
        shAge = 24;
    }

    return shAge;
}

bool CTestGuideBody::getSerializationHBC(string &strTarget, float *pSrcData, int nSize)
{
    if(pSrcData == nullptr || nSize != CElementHBC::HBC_DATA_NUM)
        return false;

    strTarget.clear();
    for(int i = 0; i < nSize; i++)
    {
        strTarget += (QString::number(static_cast<double>(pSrcData[i]), 'f', 3).toStdString() + g_cHBCSplite);
    }

    return true;
}

bool CTestGuideBody::getDeserializationHBC(float *pSrcData, int nSize, const string &strSrc)
{
    if(pSrcData == nullptr || nSize != CElementHBC::HBC_DATA_NUM)
        return false;

    vector<string> vecRes;
    string strSplit(&g_cHBCSplite, 1);
    CTestResultInfo::splitString(strSrc, strSplit, vecRes);

    if(vecRes.size() != CElementHBC::HBC_DATA_NUM)
        return false;

    for(unsigned int i = 0; i < vecRes.size(); i++)
    {
        pSrcData[i] = static_cast<float>(atof(vecRes[i].c_str()));
    }

    return true;
}

bool CTestGuideBody::getDeserializationHBC(vector<double> &vecTarget, const string &strSrc)
{
    vecTarget.clear();

    vector<string> vecRes;
    string strSplit(&g_cHBCSplite, 1);
    CTestResultInfo::splitString(strSrc, strSplit, vecRes);

    if(vecRes.size() != CElementHBC::HBC_DATA_NUM)
        return false;

    for(unsigned int i = 0; i < vecRes.size(); i++)
    {
        vecTarget.push_back(atof(vecRes[i].c_str()));
    }

    return true;
}

bool CTestGuideBody::isDataCorrent(const float &fData, const short &shTestItem)
{
    bool bCorrent = true;

    if(shTestItem == TEST_ITEM_HEIGHT)
    {
        bCorrent = !(fData < 0 || fData > 230); //cm
    }

    if(shTestItem == TEST_ITEM_WEIGHT)
    {
        bCorrent = !(m_fWeight > 250 || m_fWeight < 0); //kg
    }

    return bCorrent;
}

bool CTestGuideBody::initElementInfo(CElementInfo &elemetInfo)
{
    m_setCompleteItem.clear();
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("procTestEnd:test completed but dev invalide invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
    }
    for(int i = 0; i < 3; i++)
    {
        pSteDev->stopWork(false, false);
    }

    return  CTestGuideBase::initElementInfo(elemetInfo);
}
