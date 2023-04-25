#include "CTestProcAntiCheatingPullUp.h"

CTestProcAntiCheatingPullUp::CTestProcAntiCheatingPullUp()
    : CTestProcAntiCheatingBasic()
{
    m_shTestItem = TEST_ITEM_PULL_UP;
}

//引体向上进行数据抓取即可
void CTestProcAntiCheatingPullUp::onRecord()
{
    grab();
}
