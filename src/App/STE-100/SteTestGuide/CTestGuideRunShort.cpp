///////////////////////////////////////////////////////////
//  CTestFlowRunShort.cpp
//  Implementation of the Class CTestFlowRunShort
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CTestGuideRunShort.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementRunShort.h"
#include "CDevDataCfg.h"
using namespace TestTableCfg;

short CTestGuideRunShort::s_shTestItem(TEST_ITEM_RUN_50);
unsigned int CTestGuideRunShort::s_uiTargetLap(2);
bool CTestGuideRunShort::s_bUseStartFlag(true);
bool CTestGuideRunShort::s_bExistFlag(false);

const short shMaxSendTime = 3;

CTestGuideRunShort::CTestGuideRunShort()
    :CTestGuideBase ()
{
    m_shDevType = DEV_TYPE_RUN_SHORT;
}

CTestGuideRunShort::~CTestGuideRunShort()
{

}

void CTestGuideRunShort::setSteDevice(CDevHardwareBase *pSteDev)
{
    if(m_iDevID == 0)
    {
        m_vecTestItem.clear();
        m_vecTestItem.push_back(TEST_ITEM_RUN_50);
    }

    CTestGuideBase::setSteDevice(pSteDev);

    onTestItemChanged();
}

bool CTestGuideRunShort::startTest(const bool &bExecuteStart, const bool &bCast)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr == pSteDev)
    {
        return false;
    }

    auto vecAllBindElement = getAllBindElements();

    if(vecAllBindElement.size() <= 0)
    {
        return false;
    }

    unsigned int uiSendNum = 0;
    auto pDevRunShort = reinterpret_cast<CDevSteRunShort *>(pSteDev);
    if(nullptr != pDevRunShort && !s_bUseStartFlag)
    {
        //不存在发令枪，则播放语音
        if(!s_bExistFlag)
        {
            CTextToSpeech::GetInstance().playStartSpeech();
        }

        //可能存在获取失败的情况(目前默认最多获取三次)
        uiSendNum = sendGetTimeCmd(pDevRunShort);

        //证明获取失败
        if(uiSendNum >= shMaxSendTime)
        {
            CTextToSpeech::GetInstance().textToSpeech("启动异常，请重新测试");
            return false;
        }
    }

    bool bSpeakTemp = false;
    for (unsigned int i = 0; i < vecAllBindElement.size(); i++)
    {
        auto &pElement = vecAllBindElement[i];

        if(pElement == nullptr || !startTest(pElement->getIndex(), true, false))
        {
            return false;
        }

        //仅需要起点，且最后一次才进行语音
        if(s_bUseStartFlag && i == vecAllBindElement.size() - 1)
        {
            bSpeakTemp = true;
        }

        setTestState(CTestStateManage::TEST_CHECKED, pElement->getIndex(), bSpeakTemp, false, true);
        setTestState(CTestStateManage::TEST_IN_PROCESS, pElement->getIndex(), false, false, true);
    }

    //若不需要起点，则需要调用函数
    if(nullptr != pDevRunShort && !s_bUseStartFlag)
    {
        unsigned int uiDelayTime = 0;

        //获取时间的延时
        uiDelayTime = uiSendNum * 200;

        //存在发令枪，需要增加对应延时
        if(s_bExistFlag)
        {
            uiDelayTime += static_cast<unsigned int>(CDatabaseManage::GetInstance().getTestRunConfig().getRunShortTime().toInt());
        }

        pDevRunShort->setStartTime(uiDelayTime);
    }

    return true;
}

//理论上和基类相同
bool CTestGuideRunShort::startTest(const unsigned short &shElementIndex, const bool &bFirstTest, const bool &bExecuteStart)
{
    //准备测试
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return false;
    }

    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;
    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("startTest::getElementInfo false");
        return false;
    }

    //标志表明是否设置某个测试单元的测试项目
    bool bSetTestProc = true;

    CElementInfo &elementInfo = *pElementInfo;
    elementInfo.m_bActuallyWorking = false;

    //若是开启其中进行第一次测试需要初始化测试信息
    if(bFirstTest)
    {
        initElementInfo(elementInfo);
    }
    else
    {
        //目前的处理逻辑对于多个测试项不支持其中某一项多次测试
        if(m_vecTestItem.size() > 1)
        {
            elementInfo.m_shCurTestItemIndex++;
        }
        else
        {
            bSetTestProc = false;
        }
    }

    if(!setCurTestItem(shElementIndex, bSetTestProc))
    {
        //开启测试错误
        return false;
    }

    //设备为直连模式同时当前开始测试 那么开启语音
    bool bSpeak = bExecuteStart && !pSteDev->isStaionSte();

    //准备测试需要等待语音完成
    setTestState(CTestStateManage::TEST_PREPARE, elementInfo, bSpeak, true, true);

    //通过设备发送开始命令
    if(bExecuteStart)
    {
        //异步模式在此处播放
        if(s_shTestMode == TEST_ASYNC)
        {
            /*
            m_ui.setBtnEnable(CProjectEnums::OTM_ASYNC_COMMIT_BTN, false);
            CTextToSpeech::GetInstance().textToSpeech("3",1000);
            CTextToSpeech::GetInstance().textToSpeech("2",1000);
            CTextToSpeech::GetInstance().textToSpeech("1",1000);
            m_ui.setBtnEnable(CProjectEnums::OTM_ASYNC_COMMIT_BTN, true);
            */
            CTextToSpeech::GetInstance().playStartSpeech();

            auto pDevRunShort = reinterpret_cast<CDevSteRunShort *>(pSteDev);
            unsigned int uiSendNum = 0;
            if(nullptr != pDevRunShort && !s_bUseStartFlag)
            {
                uiSendNum = sendGetTimeCmd(pDevRunShort);
            }

            setTestState(CTestStateManage::TEST_CHECKED, shElementIndex, false, false, true);
            setTestState(CTestStateManage::TEST_IN_PROCESS, shElementIndex, false, false, true);

            if(nullptr != pDevRunShort && !s_bUseStartFlag)
            {
                unsigned int uiDelayTime = 0;

                uiDelayTime = uiSendNum * 200;

                pDevRunShort->setStartTime(shElementIndex,uiDelayTime);
            }
        }
    }

    return true;
}

bool CTestGuideRunShort::stopTest(const bool &bExecuteStart, const bool &bCast)
{
    if(bExecuteStart && bCast)
    {}

    auto vecAllBindElement = getAllBindElements();

    //调用单独开启设备的初始化过程，不执行其中的设备开启命令
    QDateTime time;
    static CLocation sLocation;
    sLocation.m_iSteDevMacID = m_iDevID;

    bool bSpeakTemp = false;
    for(unsigned int i = 0; i < vecAllBindElement.size(); i++)
    {
        vecAllBindElement[i]->stopTest();

        sLocation.m_shElementIndex = vecAllBindElement[i]->getIndex();

        //仅最后一次结束才语音提示
        if(i == vecAllBindElement.size() - 1)
        {
            bSpeakTemp = true;
        }
        setTestState(CTestStateManage::TEST_COMPLETED, sLocation.m_shElementIndex,bSpeakTemp,false,false);

        saveDataToResult(sLocation.m_shElementIndex);

        emit signalTestComplete(sLocation);
    }

    return true;
}

void CTestGuideRunShort::setTestItem(const short &shTestItem)
{
    s_shTestItem = shTestItem;

    if(shTestItem == TEST_ITEM_RUN_BACK_8_50)
    {
        s_uiTargetLap = 9;
    }
    else if(shTestItem == TEST_ITEM_RUN_BACK_4_10)
    {
        s_uiTargetLap = 5;
    }
    else
    {
        s_uiTargetLap = 2;
    }
}

void CTestGuideRunShort::setUseStartFlag(const bool &bUseFlag)
{
    s_bUseStartFlag = bUseFlag;

    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr != pSteDev)
    {
        auto pDevRunShort = reinterpret_cast<CDevSteRunShort *>(pSteDev);
        if(nullptr != pDevRunShort)
        {
            pDevRunShort->setUseStartFlag(bUseFlag);
        }
    }
}

void CTestGuideRunShort::setStartGunExistFlag(const bool &bExistFlag)
{
    s_bExistFlag = bExistFlag;
}

void CTestGuideRunShort::onTestItemChanged()
{
    //修改测试项目
    vector<short> vecTestItem;
    CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsVec(DEV_TYPE_RUN_SHORT, vecTestItem);
    if(vecTestItem.size() > 0)
    {
        setTestItem(vecTestItem[0]);
    }

    //修改是否使用起点
    bool bUseRunShortStartFlag = true;
    bUseRunShortStartFlag = CDatabaseManage::GetInstance().getDevTestInfoConfig()->getUseRunShortStartFlag();
    setUseStartFlag(bUseRunShortStartFlag);

    CTestGuideBase::onTestItemChanged();
}

//修改测试状态显示，同时修改测试标志(保证重测期间不会进行测试)
void CTestGuideRunShort::reStartByNoPoint()
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(nullptr != pSteDev)
    {
        auto vecAllBindElement = getAllBindElements();
        CSubElementBase* pElement = nullptr;
        for (unsigned int i = 0; i < vecAllBindElement.size(); i++)
        {
            pElement = vecAllBindElement[i];

            if(nullptr != pElement)
            {
                pElement->stopTest();

                setTestState(CTestStateManage::TEST_PREPARE, pElement->getIndex(), false, false, true);
            }
        }
    }
}

void CTestGuideRunShort::onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus)
{
    //0.安全保护
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    auto pDevRunShort = reinterpret_cast<CDevSteRunShort *>(pSteDev);

    //1.得到子设备
    CSubElementRunShort *pElement = reinterpret_cast<CSubElementRunShort *>(pDevRunShort->getElement(shElementIndex));
    if(nullptr == pElement)
    {
        LOGERROR("Run SubElement Invalid");
        return;
    }

    //2.获取 测试类型 与 目标圈数 (静态引用)
    short &shTestItem = s_shTestItem;
    unsigned int &uiTargetLap = s_uiTargetLap;

    //3.根据当前圈数显示
    unsigned int uiCurrentLap = pElement->getCurrentLaps();
    int iLap = (uiCurrentLap - 1) / 2;
    if(iLap > 0 && (shTestItem == TEST_ITEM_RUN_BACK_8_50 || shTestItem == TEST_ITEM_RUN_BACK_4_10))
    {
        short shColumnIndex = CDevDataCfg::getColTypeIndex(COL_DEV_ID, DEV_TYPE_RUN_SHORT, TEST_ASYNC);
        short shUpdateIndex = CDevDataCfg::getColTypeIndex(COL_THROUGH_TIMES, DEV_TYPE_RUN_SHORT, TEST_ASYNC);

        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                 QString::number(shElementIndex + 1), shUpdateIndex, QString::number(iLap));
    }

    //4.判断是否完成测试
    bool bFlag = pElement->getStartFlag();
    if(uiCurrentLap >= uiTargetLap && bFlag)
    {
        completeTest(shElementIndex);
    }
}

void CTestGuideRunShort::saveDataToResult(const unsigned short &shElementIndex)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    CSubElementBase *pSubElement = nullptr;
    CElementInfo *pElementInfo = nullptr;
    if(!getElementInfo(&pSubElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("getElementInfo false");
        return ;
    }

    CSubElementRunShort *pElement = reinterpret_cast<CSubElementRunShort *>(pSteDev->getElement(shElementIndex));
    if(nullptr == pElement)
    {
        LOGERROR("Run Short SubElement Invalid");
        return ;
    }

    auto &resultInfo = pElementInfo->m_arrTestResultInfo[pElementInfo->m_shCurTestItemIndex];

    //修改当前测试项目
    resultInfo.m_nItem = s_shTestItem;

    if(!resultInfo.m_strPathResourceRoot.empty())
    {
        dealTestAntiCheating(resultInfo, pSubElement);
    }

    //记录结果是否可信
    bool bCredible = false;
    if(pElementInfo->m_pUserInfo == nullptr)
    {
        LOGERROR("elementInfo.m_pUserInfo = nullptr");
        bCredible = false;
    }
    else
    {
        bCredible =  pElementInfo->m_nFoulCode == NOT_FOUL && pElementInfo->m_cHardwareDataStateCode == DEV_TEST_STATE_EOT_VALID;
    }

    //记录本次成绩
    float fNum = pElement->getCurrentTime()/1000;
    string strSaveValue = value2SaveValue(static_cast<short>(resultInfo.m_nItem), fNum);
    resultInfo.addTestResult(strSaveValue, bCredible);
}

void CTestGuideRunShort::completeTest(const unsigned short &shElementIndex)
{
    auto pSteDev = m_objSteDevMgr.getDevSteByID(m_iDevID);
    if(pSteDev == nullptr)
    {
        LOGERROR("pSteDev == nullptr devid = " + to_string(m_iDevID));
        return;
    }

    auto pDevRunShort = reinterpret_cast<CDevSteRunShort *>(pSteDev);

    //0.获取基本属性
    CElementInfo *pElementInfo = nullptr;
    CSubElementBase *pElement = nullptr;

    if(!getElementInfo(&pElement, &pElementInfo, shElementIndex))
    {
        LOGERROR("procTestEnd:getElementInfo false, shIndex = " + to_string(shElementIndex));
        return;
    }

    CSubElementRunShort *pSubElement = reinterpret_cast<CSubElementRunShort *>(pSteDev->getElement(shElementIndex));
    if(nullptr == pSubElement)
    {
        LOGERROR("Run Short SubElement Invalid");
        return;
    }

    //1.停止测试
    pDevRunShort->stopWork(shElementIndex);

    //2.修改测试状态为完成
    setTestState(CTestStateManage::TEST_COMPLETED, shElementIndex);

    //3.保存测试结果
    saveDataToResult(shElementIndex);

    CTestGuideBase::procTestResult(*pElementInfo,pElement,true);

    /*
    //4.完成测试(转移测试结果)，异步表格中删除
    short shColumnIndex = CDevDataCfg::getColTypeIndex(COL_DEV_ID, DEV_TYPE_RUN_SHORT, TEST_ASYNC);
    short shUpdateIndex = CDevDataCfg::getColTypeIndex(COL_TEST_RESULT, DEV_TYPE_RUN_SHORT, TEST_ASYNC);

    static CLocation sLocation;
    sLocation.m_shElementIndex = pElement->getIndex();
    sLocation.m_iSteDevMacID = m_pSteDevice->getMacID();

    //根据 测试项目 测试模式 分情况处理
    if(s_shTestMode == TEST_SYNC)
    {
        m_ui.updateModelCellData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, \
                                 QString::number(shElementIndex), shUpdateIndex, qsDispValue);
    }
    else
    {
        //异步模式需要转移表格
        m_ui.deleteModelData(CInterAction::COMMON_DATA_MODEL, shColumnIndex, QString::number(shElementIndex));

        //将目前所有结果传到结果表格(设备、姓名、准考、圈数、结果、成绩)
        QStringList vecContent;
        vecContent.append(QString::number(shElementIndex));
        vecContent.append(pElementInfo->m_pUserInfo->m_strName);
        vecContent.append(pElementInfo->m_pUserInfo->m_strStudentId);
        
        if(shCurItem == TEST_ITEM_RUN_BACK_8_50)
        {
            vecContent.append(QString::number(4));
        }
        vecContent.append(qsDispValue);
        if(dScore < 0)
        {
            vecContent.append(QString::number(0));
        }
        else
        {
            vecContent.append(QString::number(dScore));
        }
        m_ui.appendModelData(CInterAction::STUDENT_DATA_MODEL, vecContent);
    }

    signalTestComplete(sLocation);
    */
}

unsigned int CTestGuideRunShort::sendGetTimeCmd(CDevSteRunShort *pDevRunShort)
{
    unsigned int uiSendTime = 0;

    if(nullptr == pDevRunShort)
    {
        return uiSendTime;
    }

    bool bResult = false;
    while(uiSendTime < shMaxSendTime)
    {
        bResult = pDevRunShort->getStartTime();

        if(bResult)
        {
            break;
        }

        uiSendTime ++;
    }

    return uiSendTime;
}
