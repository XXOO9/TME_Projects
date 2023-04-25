#include "CTestProcSitUp.h"

CTestProcSitUp::CTestProcSitUp()
    : CTestProcBasic ()
{
    m_shTestItem = TEST_ITEM_SIT_UP;

    loadTestCfg();
}

CTestProcSitUp::~CTestProcSitUp()
{

}
