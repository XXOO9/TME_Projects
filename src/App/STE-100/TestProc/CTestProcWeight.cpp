#include "CTestProcWeight.h"
#include "CTestProcWeight.h"

CTestProcWeight::CTestProcWeight()
    : CTestProcBasic ()
{
    initTestProcess();

    m_shTestItem = TEST_ITEM_WEIGHT;

    //体重测试等待时间
    m_nTestStartWaitTime = 0;

    loadTestCfg();
}

CTestProcWeight::~CTestProcWeight()
{

}

void CTestProcWeight::initTestProcess()
{
    CTestProcBasic::initTestProcess();
    m_mapCommonProcContent[CTestStateManage::TEST_CHECKED] = QStringLiteral("开始测试体重");
    m_mapCommonProcContent[CTestStateManage::TEST_ERR] = QStringLiteral("体重测试出错，将在2s内返回上一页");
}
