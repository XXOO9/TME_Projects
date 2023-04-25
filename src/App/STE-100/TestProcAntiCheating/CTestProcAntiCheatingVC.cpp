#include "CTestProcAntiCheatingVC.h"

CTestProcAntiCheatingVC::CTestProcAntiCheatingVC()
    : CTestProcAntiCheatingBasic()
{
    m_itimerInterval = 500;
    m_shTestItem = TEST_ITEM_VC;
}

void CTestProcAntiCheatingVC::onRecord()
{
    if(isTimerStart())
        return;

    //立即抓取一次
    grab();
    startTimerC(m_itimerInterval);
}
