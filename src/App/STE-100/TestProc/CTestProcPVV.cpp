#include "CTestProcPVV.h"

CTestProcPVV::CTestProcPVV()
{
    //肺活量测量三次
    m_shTestTimesTotal = 1;

    m_shTestItem = TEST_ITEM_PVV;

    m_nTestStartWaitTime = 0;

    loadTestCfg();
}

CTestProcPVV::~CTestProcPVV()
{

}
