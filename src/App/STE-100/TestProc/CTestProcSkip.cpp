#include "CTestProcSkip.h"

CTestProcSkip::CTestProcSkip()
    : CTestProcBasic ()
{
    initTestProcess();

    m_shTestItem = TEST_ITEM_ROPE_SKIP;

    loadTestCfg();
}

CTestProcSkip::~CTestProcSkip()
{

}

void CTestProcSkip::initTestProcess()
{
    CTestProcBasic::initTestProcess();
    m_mapCommonProcContent[CTestStateManage::TEST_CHECKED] = /*QStringLiteral*/("开始");
}
