#include "CTestProcVC.h"

CTestProcVC::CTestProcVC()
    : CTestProcBasic ()
{
    //肺活量测量三次
    m_shTestTimesTotal = 2;

    m_shTestItem = TEST_ITEM_VC;

    m_nTestStartWaitTime = 0;

    loadTestCfg();
}

CTestProcVC::~CTestProcVC()
{

}
