#include "CTestProcBasic.h"
#include "CLogger.h"
#include "CDevDataCfg.h"
#include "CDatabaseManage.h"
#include "SteTestGuide/CTestGuideBase.h"
#include <QDebug>

CTestProcBasic::CTestProcBasic()
    : m_shTestState(CTestStateManage::TEST_NOT_START)
    , m_shTestItem(TEST_ITEM_UNKNOW)
{
    initTestProcess();
}

CTestProcBasic::~CTestProcBasic()
{

}

//外部设置当前测试状态
bool CTestProcBasic::setTestState(short shTestState)
{
    //设置当前测试状态
    m_shTestState = shTestState;

    CTestStateManage *pTestStateManage = m_pTestStateManage;
    if(pTestStateManage == nullptr)
    {
        LOGERROR("pTestStateManage invalide");
        return false;
    }

    //设置对应设备测试单元状态
    pTestStateManage->setTestState(shTestState);

    //游客模式一直进行下一次测试(身高体重体脂率里面需要特殊处理)
    if(shTestState == CTestStateManage::TEST_COMPLETED)
    {
        m_shTestTimesCur += 1;
    }

    return false;
}

short CTestProcBasic::getCurTestState() const
{
    short shRet = CTestStateManage::TEST_NOT_START;
    CTestStateManage *pTestStateManage = m_pTestStateManage;
    if(pTestStateManage != nullptr)
    {
        shRet = pTestStateManage->getTestState();
    }

    return shRet;
}

//SYSTEM_TEST_PREPARE SYSTEM_TEST_DEV_CHECKED SYSTEM_TESTING SYSTEM_TEST_COMPLETED
//目前只会有这4种状态
QString CTestProcBasic::getTestProcDispContent()
{
    QString qsTemp;
    auto itFind = m_mapCommonProcContent.find(m_shTestState);
    if(itFind != m_mapCommonProcContent.end())
    {
        qsTemp = itFind->second;

        //播报开始测试时非游客需要进行处理(对开始测试的语音进行处理)
        auto pTestState = m_pTestStateManage;
        if(m_shTestTimesTotal > 1 && pTestState != nullptr && m_shTestState == CTestStateManage::TEST_CHECKED
                && pTestState->getTestModel() != CTestStateManage::VISITORS)
        {
            short shTestMode = getActualTestMode(CDevDataCfg::getDevDirectConnectFlag(), CTestGuideBase::s_shTestMode);
            switch (shTestMode) {
            //同步异步仅在第一次进行语音播报，同时异步存在多次时开始语音播报开始进行第%d次测试
            case TEST_ASYNC:
                if(m_shTestTimesCur == 0)
                    qsTemp = QString("开始进行第%1次测试").arg(m_shTestTimesCur + 1);
            case TEST_SYNC:
                break;
            //直连每次都处理
            case TEST_MODE_COUNT:
                qsTemp = QString("开始进行第%1次测试").arg(m_shTestTimesCur + 1);
                break;
            }
        }
    }

    return qsTemp;
}

short CTestProcBasic::getTestItem()
{
    return  m_shTestItem;
}

string CTestProcBasic::getTestItemDescribe()
{
    return CDevDataCfg::getTestItemDescribe(m_shTestItem);
}

short CTestProcBasic::getCurTestTimesIndex()
{
    return m_shTestTimesCur;
}

int CTestProcBasic::getTestStartWaitTime()
{
    return m_nTestStartWaitTime;
}

//统一的处理
bool CTestProcBasic::getTestCompleteFlag()
{
    if(m_pTestStateManage == nullptr)
    {
        LOGERROR("m_pTestStateManage invalid");
        return true;
    }

    //统一的处理，除开身高 体重 体脂率游客模式下直接返回
    if(m_pTestStateManage->getTestModel() == CTestStateManage::VISITORS)
    {
        if(m_shTestItem == TEST_ITEM_HEIGHT || m_shTestItem == TEST_ITEM_WEIGHT || m_shTestItem == TEST_ITEM_HBC)
        {
            if(m_shTestTimesCur >= 1)
            {
                return true;
            }
        }

        return false;
    }
    else
    {
        if(m_shTestTimesCur >= m_shTestTimesTotal)
            return  true;
    }

    return false;
}

void CTestProcBasic::loadTestCfg()
{
    auto pTable = CDatabaseManage::GetInstance().getDevSWInfoConfig();

    if(pTable == nullptr)
    {
        LOGERROR("pTableDevSWInfoConfig == nullptr");
        return;
    }

    short shDevType = CDevDataCfg::getDevTypeByTestItem(m_shTestItem);

    //获取项目开始等待时长
    int nTemp;
    if(pTable->getDevTestStartWaitTime(shDevType, nTemp))
    {
        m_nTestStartWaitTime = nTemp;
    }

    if(pTable->getDevTestTimes(shDevType, nTemp))
    {
        m_shTestTimesTotal = static_cast<short>(nTemp);
    }
}

void CTestProcBasic::reset()
{
    setTestState(CTestStateManage::TEST_NOT_START);
    m_shTestTimesCur = 0;
}

void CTestProcBasic::initTestProcess()
{

    m_mapCommonProcContent[CTestStateManage::TEST_PREPARE] =  /*QStringLiteral*/("请准备");

    m_mapCommonProcContent[CTestStateManage::TEST_CHECKED] = /*QStringLiteral*/("开始测试");

    m_mapCommonProcContent[CTestStateManage::TEST_IN_PROCESS] = /*QStringLiteral*/("测试中");

    m_mapCommonProcContent[CTestStateManage::TEST_COMPLETED] = /*QStringLiteral*/("测试完成");

    m_mapCommonProcContent[CTestStateManage::TEST_ERR] = /*QStringLiteral*/("测试出错，将在语音结束后返回首页");
}
