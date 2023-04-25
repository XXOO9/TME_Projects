#include "CTestProcAntiCheatingSitSFB.h"

CTestProcAntiCheatingSitSFB::CTestProcAntiCheatingSitSFB()
    : CTestProcAntiCheatingBasic()
{
    m_shTestItem = TEST_ITEM_SFB;
    m_itimerInterval = 500;
}

//坐位体前屈触发记录时立即进行一次
void CTestProcAntiCheatingSitSFB::onRecord()
{
    if(isTimerStart())
        return;

    //立即抓取一次
    grab();
    startTimerC(m_itimerInterval);
}
