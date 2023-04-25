#include "CTestProcPullUp.h"

CTestProcPullUp::CTestProcPullUp()
    : CTestProcBasic ()
{
    m_shTestItem = TEST_ITEM_PULL_UP;

    loadTestCfg();
}

CTestProcPullUp::~CTestProcPullUp()
{

}
