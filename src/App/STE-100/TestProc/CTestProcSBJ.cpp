#include "CTestProcSBJ.h"

CTestProcSBJ::CTestProcSBJ()
    : CTestProcBasic ()
{
    m_shTestTimesTotal = 2;

    m_shTestItem = TEST_ITEM_SBJ;

    loadTestCfg();
}

CTestProcSBJ::~CTestProcSBJ()
{

}
