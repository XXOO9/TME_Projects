#include "CTestProcHeight.h"

CTestProcHeight::CTestProcHeight()
    : CTestProcBasic ()
{
    initTestProcess();
    m_shTestItem = TEST_ITEM_HEIGHT;

    //体重测试完成之后立即开始身高测试
    m_nTestStartWaitTime = 0;

    loadTestCfg();
}

void CTestProcHeight::initTestProcess()
{
    CTestProcBasic::initTestProcess();
    m_mapCommonProcContent[CTestStateManage::TEST_CHECKED] = QStringLiteral("开始测试身高");
    m_mapCommonProcContent[CTestStateManage::TEST_ERR] = QStringLiteral("身高测试出错，将在2s内返回上一页");
}
