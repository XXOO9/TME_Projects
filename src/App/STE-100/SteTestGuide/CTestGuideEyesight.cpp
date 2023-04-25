#include "CTestGuideEyesight.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementEyesight.h"
#include "CDevDataCfg.h"

using namespace TestTableCfg;

static const char g_cEyeightSplite('#');

CTestGuideEyesight::CTestGuideEyesight()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_EYESIGHT;
}

void CTestGuideEyesight::setSteDevice(CDevHardwareBase *pSteDev)
{
    //第一次连接时才进行设置
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_EYESIGHT);
    }

    CTestGuideBase::setSteDevice(pSteDev);
}

void CTestGuideEyesight::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
{
    CSubElementBase *pSubElement = nullptr;
    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo false");
        return;
    }

    CSubElementEyesight *pElement = reinterpret_cast<CSubElementEyesight *>(pSubElement);

    if(nullptr == pElement)
    {
        return;
    }

    float fLeftData  = 0.0f;
    float fRightData = 0.0f;
    bool  bRightData = false;

    //判断 左眼数据 或 右眼数据
    if(cDataStstus == EyeSightStatus::EYE_LEFT_DATA)
    {
        fLeftData  = pElement->getDataLEyesight();
    }
    else if(cDataStstus == EyeSightStatus::EYE_RIGHT_DATA)
    {
        fLeftData  = pElement->getDataLEyesight();
        fRightData = pElement->getDataREyesight();

        bRightData = true;
    }
    else
    {
        LOGERROR("Invalid EyeSight Data Status");
        return;
    }

    //拼装显示数据
    QString qsDispValue;

    //目前定义小于 0.1 的数值为 未测试
    if(fLeftData <= 0.1f)
    {
        qsDispValue += DEFAULT_DISP_VALUE;
    }
    else
    {
        qsDispValue += value2DispValue(TEST_ITEM_EYESIGHT, fLeftData);
    }

    qsDispValue += g_cEyeightSplite;

    if(fRightData <= 0.1f)
    {
        qsDispValue += DEFAULT_DISP_VALUE;
    }
    else
    {
        qsDispValue += value2DispValue(TEST_ITEM_EYESIGHT, fRightData);
    }

    updateTestResult(qsDispValue, *pElementInfo,bRightData,fLeftData);
}

void CTestGuideEyesight::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
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
        if(CDevDataCfg::getDevDirectConnectFlag() && elementInfo.m_pUserInfo->isTouristInfo())
        {
            bCredible = true;
        }
        else
        {
            bCredible =  elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID;
        }
    }

    //记录本次成绩
    string strTarget;
    CSubElementEyesight *pElementEyeight = reinterpret_cast<CSubElementEyesight *>(pElement);
    getSerializationEyeightData(strTarget, pElementEyeight->getDataLEyesight(), pElementEyeight->getDataREyesight());
    resultInfo.addTestResult(strTarget, bCredible);
}

void CTestGuideEyesight::procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete)
{
    auto &testResultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];

    //通过结果获取成绩，同时根据置信度判断成绩是否可用
    QStringList listContent;

    //同步异步测试表格相同，将测试结果更新到界面
    static CLocation sLocation;
    sLocation.m_iSteDevMacID = m_iDevID;
    sLocation.m_shElementIndex = pElement->getIndex();

    //获取 最后的测试结果
    string strLastResult;
    testResultInfo.getLastTestResult(strLastResult);

    //获取 左眼视力 右眼视力
    float fLeftData, fRightData;
    getDeserializationEyeightData(fLeftData,fRightData,strLastResult);

    bool bVisitorsMode = pElement->isVisitorMode();
    //区分当前模式为 直连 | 组网
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        short shSerialNo = elementInfo.m_objTestProcManage.getCurTestItemTimesIndex();
        listContent.append(QString::number(shSerialNo));
        listContent.append(QString::number(static_cast<double>(fLeftData)));
        listContent.append(QString::number(static_cast<double>(fRightData)));

        //游客：序号 测试值
        //用户：序号 测试值(判断值有效无效) 成绩
        bool    bOrdered = true;
        //游客不设置成绩
        if(!bVisitorsMode)
        {
            //设置提交按钮可用
            m_ui.setBtnEnableCommit(true);
        }
        else
        {
            bOrdered = false;
        }
        m_ui.appendModelData(CInterAction::COMMON_DATA_MODEL, listContent, false);
    }
    else
    {
        short shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testResultInfo.m_nItem));
        if(shDevType == DEV_TYPE_UNKNOW)
        {
            LOGERROR("getDevTypeByTestItem false testitem = " + to_string(testResultInfo.m_nItem));
            return;
        }

        listContent = CDevDataCfg::getTableContent(sLocation, TEST_ASYNC, *elementInfo.m_pUserInfo);

        short shUserIDIndex   = CDevDataCfg::getColTypeIndex(COL_USER_ID, shDevType, TEST_ASYNC);

        short shLeftEyeIndex  = CDevDataCfg::getColTypeIndex(COL_LEFT_EYE, shDevType, TEST_ASYNC);
        short shRightEyeIndex = CDevDataCfg::getColTypeIndex(COL_RIGHT_EYE, shDevType, TEST_ASYNC);

        if(shUserIDIndex < 0 || shLeftEyeIndex < 0 || shRightEyeIndex < 0)
        {
            LOGERROR("shUserIDIndex : " + to_string(shUserIDIndex) + "shLeftEyeIndex : " + to_string(shLeftEyeIndex) +"shRightEyeIndex : " + to_string(shRightEyeIndex));
            return;
        }

        QString qsUserID = getUserID(elementInfo);
        listContent[shLeftEyeIndex]  = QString::number(static_cast<double>(fLeftData));
        listContent[shRightEyeIndex] = QString::number(static_cast<double>(fRightData));

        //根据犯规类型显示成绩
        m_ui.updateModelRowData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, qsUserID, listContent);

        //一对多完成测试
        if(bComplete)
        {
            //异步模式，将结果放到结果表格
            if(s_shTestMode == TEST_ASYNC)
            {
                //删除默认表格中的内容
                m_ui.deleteModelData(CInterAction::COMMON_DATA_MODEL, shUserIDIndex, qsUserID);

                //将目前所有结果传到结果表格
                m_ui.appendModelData(CInterAction::STUDENT_DATA_MODEL, listContent);
            }
        }
    }

    if(bComplete)
    {
        signalTestComplete(sLocation);
    }
}

void CTestGuideEyesight::updateTestResult(QString &qsDisp, const CElementInfo &elementInfo, bool bRightData, float fLeftData)
{
    //直连
    if(CDevDataCfg::getDevDirectConnectFlag())
    {
        m_ui.setCurTestRet(qsDisp);
    }
    //组网
    else
    {
        if(!bRightData)
        {
            short shColumnIndex = CDevDataCfg::getColTypeIndex(COL_USER_ID, DEV_TYPE_EYESIGHT, TEST_ASYNC);
            short shUpdateIndex = CDevDataCfg::getColTypeIndex(COL_LEFT_EYE, DEV_TYPE_EYESIGHT, TEST_ASYNC);

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
                                     getUserID(elementInfo), shUpdateIndex, value2DispValue(TEST_ITEM_EYESIGHT, fLeftData));
        }
    }
}

void CTestGuideEyesight::getSerializationEyeightData(string &strTarget, const float &fLEyeightData, const float &fREyeightData)
{
    QString qsLEyesight(value2DispValue(TEST_ITEM_EYESIGHT, fLEyeightData))
            , qsREyesight(value2DispValue(TEST_ITEM_EYESIGHT, fREyeightData));

    strTarget = qsLEyesight.toStdString() + g_cEyeightSplite + qsREyesight.toStdString();
}

bool CTestGuideEyesight::getDeserializationEyeightData(float &fLEyeightData, float &fREyeightData, const string &strSrc)
{
    vector<string> vecRes;
    string strSplit(&g_cEyeightSplite, 1);
    CTestResultInfo::splitString(strSrc, strSplit, vecRes);

    if(vecRes.size() != 2)
        return false;

    fLEyeightData = static_cast<float>(atof(vecRes[0].c_str()));
    fREyeightData = static_cast<float>(atof(vecRes[1].c_str()));
    return true;
}

bool CTestGuideEyesight::getDeserializationEyeightData(QString &qsLEyeightData, QString &qsREyeightData, const string &strSrc)
{
    vector<string> vecRes;
    string strSplit(&g_cEyeightSplite, 1);
    CTestResultInfo::splitString(strSrc, strSplit, vecRes);

    if(vecRes.size() != 2)
        return false;

    qsLEyeightData = QString::fromStdString(vecRes[0]);
    qsREyeightData = QString::fromStdString(vecRes[1]);
    return true;
}
