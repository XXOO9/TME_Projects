#include "CTestProcRun1000.h"

CTestProcRun1000::CTestProcRun1000()
    : CTestProcBasic ()
{
    m_shTestItem = TEST_ITEM_RUN_1000;

    loadTestCfg();
}

CTestProcRun1000::~CTestProcRun1000()
{

}
