///////////////////////////////////////////////////////////
//  CTestFlowRunMid.cpp
//  Implementation of the Class CTestFlowRunMid
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CTestGuideRunMid.h"
#include "CDatabaseManage.h"
#include "CDevDataCfg.h"
using namespace  TestTableCfg;

long long CTestGuideRunMid::s_llSyncTime;
CTestGuideRunMid::CTestGuideRunMid()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_RUN_MID;
    calculateThroughTimes();

    connect(&m_timerCheckViaPtDev, &QTimer::timeout, this, &CTestGuideRunMid::onTimeOutCheckViaPtDev);
}

CTestGuideRunMid::~CTestGuideRunMid()
{
    m_timerCheckViaPtDev.stop();
}

void CTestGuideRunMid::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_RUN_1000);
    }

    CTestGuideBase::setSteDevice(pSteDev);

    m_timerCheckViaPtDev.start(1000);
}

bool CTestGuideRunMid::checkRunCardBindState(const unsigned int &nCardId)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    auto pDevRunMid = reinterpret_cast<CDevSteRunMid *>(pSteDev);
    return pDevRunMid->checkRunCardBindState(nCardId);
}

//绑定之前设置对应测试单元id
bool CTestGuideRunMid::bindUserToElement(const unsigned short &shElementIndex, const CUserInfo &userInfo)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    auto pDevRunMid = reinterpret_cast<CDevSteRunMid *>(pSteDev);

    bool bRet = false;
    //发送绑卡命令,绑卡成功在设置对应测试单元
    if(pDevRunMid->bindCard(userInfo.m_nExtraInfoID, 1, shElementIndex))
    {
         bRet = CTestGuideBase::bindUserToElement(shElementIndex, userInfo);
    }

    return bRet;
}

void CTestGuideRunMid::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
{
    CSubElementBase *pSubElement = nullptr;
    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo false");
        return;
    }

    bool &bActuallyWorking = pElementInfo->m_bActuallyWorking;

    //软件并未发送真正的开始开启对应测试单元
    if(!bActuallyWorking)
    {
        return;
    }

    //中长跑数据,数据量较小，每次都全部分析一次
    //更新圈数到ui界面，异步模式和异步模式更新结果相同
    float fTimes;
    int nCurThroughTimes;
    if(!getTestResult(fTimes, nCurThroughTimes, pSubElement))
    {
        return;
    }

    QString qsDisp(QString::number(nCurThroughTimes));

    short shDevType = getDevType();
    if(shDevType == DEV_TYPE_UNKNOW)
    {
        LOGERROR("dispTestResult devtype err");
        return;
    }

    ///增加中长跑判断途经点设备逻辑，标签经过终点时继续计圈数，在圈数记完之后
    ///

    //中长跑使用卡号id（跑步刷卡的NFC）来进行对应成绩的更新,在设备类型一栏
    short shColIndexCardID = CDevDataCfg::getColTypeIndex(COL_DEV_ID, shDevType, TEST_ASYNC);
    QString qsDevID = QString::number(pElementInfo->m_pUserInfo->m_nExtraInfoID);

    //更新圈数
    short shColIndexThroughTimes = CDevDataCfg::getColTypeIndex(COL_THROUGH_TIMES, shDevType, TEST_ASYNC);
    m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColIndexCardID, \
                      qsDevID, shColIndexThroughTimes, qsDisp);

    //更新圈数对应的时间,800 1000对应的时间显示格式相同
    short shColIndexResult = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, shDevType, TEST_ASYNC);
    qsDisp = value2DispValue(TEST_ITEM_RUN_800, fTimes);
    m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColIndexCardID, \
                      qsDevID, shColIndexResult, qsDisp);


    //若测试完成，那么通知结束成绩
    int nThroughTimes = pElementInfo->m_pUserInfo->m_bGender ? m_nMaleThroughTimes : m_nFemaleThroughTimes;
    if(nCurThroughTimes >= nThroughTimes)
    {
        //在定时器中进行结束判断处理
        m_mapCheckViaPtDevIndex[shElementIndex] = QDateTime::currentDateTime().toMSecsSinceEpoch();
    }
}

void CTestGuideRunMid::onRunMidViaPtDevDataChange(const unsigned short &shElementIndex)
{
    auto it = m_mapElementInfo.find(shElementIndex);
    if(it == m_mapElementInfo.end())
    {
        LOGWARN("Via pt Dev not exist shElementIndex = " + to_string(shElementIndex));
        return;
    }
}

void CTestGuideRunMid::getCurTestRunMidItem(short &shMaleTestItem, short &shFemaleTestItem)
{
    vector<short> vecTestItem;
    CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsVec(DEV_TYPE_RUN_MID, vecTestItem);
    if(vecTestItem.size() > 1 || vecTestItem.size() == 0)
    {
        shMaleTestItem = TEST_ITEM_RUN_1000;
        shFemaleTestItem = TEST_ITEM_RUN_800;
    }
    //代表默认，男子1000 女子800
    else
    {
        shMaleTestItem = shFemaleTestItem = vecTestItem[0];
    }
}

int CTestGuideRunMid::getRunMidItemLen(const short &shTestItem)
{
    int nRet = 800;
    switch (shTestItem) {
    case TEST_ITEM_RUN_800:
        nRet = 800;
        break;
    case TEST_ITEM_RUN_1000:
        nRet = 1000;
        break;
    case TEST_ITEM_RUN_1500:
        nRet = 1500;
        break;
    case TEST_ITEM_RUN_2000:
        nRet = 2000;
        break;
    default:
        nRet = 800;
        break;
    }

    return nRet;
}

void CTestGuideRunMid::saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement)
{
    if(pElement == nullptr)
    {
        LOGERROR("pElement invalid");
        return;
    }

    auto &resultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];

    //查看途经点数据是否满足需求
    float fData;
    bool bValidData = checkViaPtDevData(pElement->getIndex());

    int nCurThroughTime;
    getTestResult(fData, nCurThroughTime, pElement);
    //记录结果是否可信
    bool bCredible = (elementInfo.m_nFoulCode == NOT_FOUL && elementInfo.m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID && bValidData);

    //记录本次成绩
    string qsValueDisp = value2SaveValue(static_cast<short>(resultInfo.m_nItem), fData);
    resultInfo.addTestResult(qsValueDisp, bCredible);
}

bool CTestGuideRunMid::getTestResult(float &fTime, int &nCurThroughTimes, CSubElementBase *pElement)
{
    if(pElement == nullptr)
        return false;

    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pElement, &pElementInfo, pElement->getIndex()))
    {
        LOGERROR("getElementInfo false");
        return false;
    }

    CSubElementRunMid *pSubElement = reinterpret_cast<CSubElementRunMid *>(pElement);

    auto &llActualStartTime = pSubElement->getStartActualTime();
    auto &llCmdStartTime = pSubElement->getStartCmdSendTime();

    auto &vecPassTime = pSubElement->getDataRunMid();

    //获取目标通过次数
    int nTargetThroughTimes = pElementInfo->m_pUserInfo->m_bGender ? m_nMaleThroughTimes : m_nFemaleThroughTimes;

    float fCompleteTime = 0.0f;
    //自动完成的标志为真实开始的时间之后的触发次数大于等于当前的触发次数
    //第一次大于10s才开始计圈数
    nCurThroughTimes = 0;

    bool bJudge = llActualStartTime > llCmdStartTime;
    float fDisTime = ((llActualStartTime - llCmdStartTime) / 1000);

    //10s判断
    float fTimeTemp = 10.0f;
    //若开始时间符合，加上开始时间
    if(bJudge)
    {
        fTimeTemp += fDisTime;
    }

    for(auto &element : vecPassTime)
    {
        if(element.m_fPassTime >= fTimeTemp)
        {
            nCurThroughTimes++;
            fCompleteTime = element.m_fPassTime;
            if(nCurThroughTimes >= nTargetThroughTimes)
            {
                break;
            }
        }
    }

    //减去开始时间
    fTime = fCompleteTime;
    if(bJudge)
    {
        fTime -= fDisTime;
    }

    return nCurThroughTimes > 0;
}

bool CTestGuideRunMid::checkViaPtDevData(const unsigned short &shElementIndex)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    auto pDevRunMid = reinterpret_cast<CDevSteRunMid *>(pSteDev);

    CSubElementBase *pElement = nullptr;
    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo false");
        return false;
    }

    CSubElementRunMid *pElementRunMidTemp, *pSubElement = reinterpret_cast<CSubElementRunMid *>(pElement);
    auto &vecPassTime = pSubElement->getDataRunMid();

    //获取所有途经点经过时间
    auto setDev = pDevRunMid->getViaPtDev();
    vector<vector<CElementDataRunMid>> vecAllViaPassTime;
    for(auto &viaPtDev : setDev)
    {
        if(viaPtDev == nullptr)
        {
            LOGWARN("viaPtDev == nullptr");
            continue;
        }

        pElementRunMidTemp = reinterpret_cast<CSubElementRunMid *>(viaPtDev->getElement(shElementIndex));
        if(pElementRunMidTemp == nullptr)
        {
            LOGERROR("pElementRunMidTemp == nullptr");
            continue;
        }

        vecAllViaPassTime.push_back(pElementRunMidTemp->getDataRunMid());
    }

    //获取目标通过次数
    if(pElementInfo->m_pUserInfo == nullptr)
    {
        LOGERROR("pElementInfo->m_pUserInfo == nullptr");
        return false;
    }

    int nTargetThroughTimes = pElementInfo->m_pUserInfo->m_bGender ? m_nMaleThroughTimes : m_nFemaleThroughTimes;

    float fCompleteTime = 0.0f;
    //自动完成的标志为真实开始的时间之后的触发次数大于等于当前的触发次数
    //第一次大于10s才开始计圈数
    int nCurThroughTimes = 0;
    bool bRet = true;
    for(auto &elementCurPassTime : vecPassTime)
    {
        if(elementCurPassTime.m_fPassTime >= 10)
        {
            //判断是否所有途径点数据都存在满足条件的数据
            if(!dealViaPtDev(elementCurPassTime.m_fPassTime, vecAllViaPassTime))
            {
                bRet = false;
                break;
            }

            nCurThroughTimes++;
            if(nCurThroughTimes >= nTargetThroughTimes)
            {
                fCompleteTime = elementCurPassTime.m_fPassTime;
                break;
            }
        }
    }

    return bRet;
}

bool CTestGuideRunMid::dealViaPtDev(const float &fCurTime, vector<vector<CElementDataRunMid> > &vecViaTime)
{
    //没有途经点表明当前处理正确
    if(vecViaTime.size() == 0)
        return true;

    //判断需要所有途经点都要存在小于当前值的值
    bool bRet = true;
    for(auto &vecPassTime : vecViaTime)
    {
        if(vecPassTime.size() > 0)
        {
            bRet &= ((*vecPassTime.begin()).m_fPassTime < fCurTime);
        }
        else
        {
            bRet = false;
        }

        //不满足条件
        if(!bRet)
            break;
    }

    //若满足所有途经点都存在小于当前值的值，那么删除所有小于当前值的值
    if(!bRet)
    {
        return bRet;
    }

    for(auto &vecPassTime : vecViaTime)
    {
        for(auto it = vecPassTime.begin(); it != vecPassTime.end();)
        {
            //vecPassTime按照从小到大配置，若存在小于的就判断为true
            if((*it).m_fPassTime < fCurTime)
            {
                it = vecPassTime.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    return bRet;
}

void CTestGuideRunMid::procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete)
{
    if(pElement == nullptr)
    {
        string strTemp("pElement == nullptr when runNFCID = ");
        if(elementInfo.m_pUserInfo != nullptr)
        {
            strTemp += to_string(elementInfo.m_pUserInfo->m_nExtraInfoID);
        }
        LOGERROR(strTemp);
        return;
    }

    auto &testResultInfo = elementInfo.m_arrTestResultInfo[elementInfo.m_shCurTestItemIndex];

    //通过结果获取成绩，同时根据置信度判断成绩是否可用
    QStringList listContent;

    //同步异步测试表格相同，将测试结果更新到界面
    static CLocation sLocation;
    sLocation.m_iSteDevMacID = m_iDevID;
    sLocation.m_shElementIndex = pElement->getIndex();

    string strCurRes;
    QString qsCurTestRes;
    bool bRet = testResultInfo.getLastTestResult(strCurRes);

    //没有数据，若是强制退出不进行处理，若是超时，需要进行赋值
    if(!bRet)
    {
        if(m_bForceComplete)
        {
            return;
        }

        //这里表示超时
        qsCurTestRes = "超时结束";
    }

    //跑步需要将s(秒)数转为分秒显示
    qsCurTestRes = value2DispValue(TEST_ITEM_RUN_800, strCurRes);
    if(!bRet)
    {
        qsCurTestRes = "未经过途经点";
    }


    short shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testResultInfo.m_nItem));
    if(shDevType == DEV_TYPE_UNKNOW)
    {
        LOGERROR("getDevTypeByTestItem false testitem = " + to_string(testResultInfo.m_nItem));
        return;
    }

    if(elementInfo.m_pUserInfo == nullptr)
    {
        LOGERROR("elementInfo.m_pUserInfo == nullptr");
        return;
    }

    listContent = CDevDataCfg::getTableContent(sLocation, TEST_ASYNC, *elementInfo.m_pUserInfo);

    //获取需要更新的索引位置
    short shDevIdIndex = CDevDataCfg::getColTypeIndex(COL_DEV_ID, shDevType, TEST_ASYNC);
    short shThroughTimes = CDevDataCfg::getColTypeIndex(COL_THROUGH_TIMES, shDevType, TEST_ASYNC);
    short shResultIndex = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, shDevType, TEST_ASYNC);

    QString qsDevID = QString::number(elementInfo.m_pUserInfo->m_nExtraInfoID);

    //获取需要更新的数据分别为 圈数和结果，圈数根据实际圈数显示，对应测试单元中的圈数
    float fTimes;
    int nCurThroughTimes;
    getTestResult(fTimes, nCurThroughTimes, pElement);

    int nlistContentSize = listContent.size();
    if(shThroughTimes >= 0 && shThroughTimes < nlistContentSize \
            && shResultIndex >= 0 && shResultIndex < nlistContentSize)
    {
        listContent[shThroughTimes] = QString::number(nCurThroughTimes);
        listContent[shResultIndex] = qsCurTestRes;
    }
    else
    {
        LOGERROR("shThroughTimes = " + to_string(shThroughTimes) + " shResultIndex = " + to_string(shResultIndex) \
                 + " nlistContentSize = " + to_string(nlistContentSize) + " qsDevID =" + qsDevID.toStdString());
    }

    m_ui.updateModelRowData(CInterAction::COMMON_DATA_MODEL, shDevIdIndex, qsDevID, listContent);

    //一对多完成测试，异步模式，将结果放到结果表格
    if(bComplete && s_shTestMode == TEST_ASYNC)
    {
        //删除默认表格中的内容
        m_ui.deleteModelData(CInterAction::COMMON_DATA_MODEL, shDevIdIndex, qsDevID);
    }

    if(bComplete)
    {
        emit signalTestComplete(sLocation);
    }
}

void CTestGuideRunMid::onTimeOutCheckViaPtDev()
{
    if(!getActiveState())
    {
        return;
    }

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    auto pDevRunMid = reinterpret_cast<CDevSteRunMid *>(pSteDev);

    //获取当前设备途经点设备
    auto setViaPtDev = pDevRunMid->getViaPtDev();
    bool bExistViaPtDev = setViaPtDev.size() > 0;

    long long llDelayTime, llTargetTime, llCurTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;
    CSubElementRunMid *pElementRunMid = nullptr;
    bool bStop;

    for(auto it = m_mapCheckViaPtDevIndex.begin(); it != m_mapCheckViaPtDevIndex.end();)
    {
        if(!getElementInfo(&pElement, &pElementInfo, it->first))
        {
            LOGERROR("procTestBegin:getElementInfo false");
            continue;
        }
        pElementRunMid = reinterpret_cast<CSubElementRunMid *>(pElement);


        //

        //存在途经点需要在10s内判断途经点是否满足经过条件
        llTargetTime = bExistViaPtDev ? it->second + m_nCheckViaPtDevCheckTime : it->second;
        llDelayTime = it->second + m_nDelayTimeStop;

        //若当前时间小于等待时间，这里存在两个逻辑
        //1.存在途经点，需要有10s的时间判断是否所有途经点都满足时间，若不满足，设置标志为false
        bStop = true;
        if(llCurTime < llTargetTime)
        {
            //若所有途经点结果不满足需求，那么设置结束标志为false
            bStop = checkViaPtDevData(it->first);
        }

        //2.不存在途经点，2022.11.23 9:00 益康销售反馈没过终点就出现结果，限制时间延后两秒
        //延迟2s出结果针对所有类型
        if(llCurTime < llDelayTime)
        {
            bStop = false;
        }

        //满足结束条件那么结束当前测试单元
        if(bStop)
        {
            pElementRunMid->setCheckedViaPtDev(true);
            procTestEnd(it->first);

            it = m_mapCheckViaPtDevIndex.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void CTestGuideRunMid::onTestItemChanged()
{
    vector<short> vecTestItem;
    CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsVec(DEV_TYPE_RUN_MID, vecTestItem);
    calculateThroughTimes();

    CTestGuideBase::onTestItemChanged();
}

void CTestGuideRunMid::calculateThroughTimes()
{
    //计算当前需要经过的圈数
    int nCirculLenth = 0, nMaleLenth = 0, nFemaleLenth = 0;
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevRunMidLength(nCirculLenth))
    {
        nCirculLenth = 400;
        LOGERROR("数据库获取默认配置失败，长度默认400m")
    }

    if(nCirculLenth <= 0)
    {
        nCirculLenth = 400;
    }

    vector<short> vecTestItem;
    short shMaleItem, shFemaleItem;
    getCurTestRunMidItem(shMaleItem, shFemaleItem);

    nMaleLenth = getRunMidItemLen(shMaleItem);
    nFemaleLenth = getRunMidItemLen(shFemaleItem);

    m_nMaleThroughTimes = nMaleLenth / nCirculLenth;
    if(nMaleLenth % nCirculLenth != 0)
    {
        m_nMaleThroughTimes++;
    }

    m_nFemaleThroughTimes = nFemaleLenth / nCirculLenth;
    if(nFemaleLenth % nCirculLenth != 0)
    {
        m_nFemaleThroughTimes++;
    }
}

void CTestGuideRunMid::onRunMidLenthChanged()
{
    calculateThroughTimes();
}

void CTestGuideRunMid::procTestBegin(const unsigned short &shElementIndex)
{
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;
    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("procTestBegin:getElementInfo false");
        return;
    }

    //无论异步同步，这里只会经过一次,若出现多次，表示逻辑出现错误
    if(!pElementInfo->m_bActuallyWorking)
    {
        //异步开始更新实际开始时间
        if(s_shTestMode == TEST_ASYNC)
        {
            reinterpret_cast<CSubElementRunMid *>(pElement)->updateStartActualTime();
        }
        //同步开始,在外部进行统一设置时间，这里进行赋值
        else
        {
            reinterpret_cast<CSubElementRunMid *>(pElement)->setStartActualTime(s_llSyncTime);
        }

        pElementInfo->m_bActuallyWorking = true;
    }
    else
    {
        LOGERROR("procTestBegin:reentry");
    }
}

void CTestGuideRunMid::procTestEnd(const unsigned short &shElementIndex)
{
    //当前测试单元未进行检测途径点，那么不进行处理
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    auto pElement = pSteDev->getElement(shElementIndex);
    if(pElement == nullptr)
    {
        LOGERROR("pElement invalid");
        return;
    }

    auto pElementRunMid = reinterpret_cast<CSubElementRunMid *>(pElement);
    if(!pElementRunMid->getCheckedViaPtDev())
    {
        return;
    }

    CTestGuideBase::procTestEnd(shElementIndex);
}

bool CTestGuideRunMid::getActualStartTime(const unsigned short &shElementIndex, qint64 &llTimeStamp)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("invalide pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    auto pElement = pSteDev->getElement(shElementIndex);
    if(pElement == nullptr)
    {
        LOGERROR("getElement false shElementIndex = " + to_string(shElementIndex) + " devid =" + to_string(m_iDevID));
        return false;
    }

    llTimeStamp = reinterpret_cast<CSubElementRunMid *>(pElement)->getStartActualTime();

    return true;
}
