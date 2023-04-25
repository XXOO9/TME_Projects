#include "CTestProcSFB.h"

CTestProcSFB::CTestProcSFB()
    : CTestProcBasic ()
{
    m_shTestTimesTotal = 2;
    m_shTestItem = TEST_ITEM_SFB;

    m_nTestStartWaitTime = 100;
    loadTestCfg();
}

CTestProcSFB::~CTestProcSFB()
{

}
