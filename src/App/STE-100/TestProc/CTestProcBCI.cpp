#include "CTestProcBCI.h"

CTestProcBCI::CTestProcBCI()
    : CTestProcBasic ()
{
    initTestProcess();
    m_shTestItem = TEST_ITEM_HBC;

    loadTestCfg();
}

void CTestProcBCI::initTestProcess()
{
    CTestProcBasic::initTestProcess();
    m_mapCommonProcContent[CTestStateManage::TEST_CHECKED] = QStringLiteral("开始人体成分测量");
    m_mapCommonProcContent[CTestStateManage::TEST_ERR] = QStringLiteral("人体成分测试出错，将在2s内返回上一页");
}
