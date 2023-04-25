#include "CTestProcAntiCheatingSitUp.h"

CTestProcAntiCheatingSitUp::CTestProcAntiCheatingSitUp()
    : CTestProcAntiCheatingBasic()
{
    m_shTestItem = TEST_ITEM_SIT_UP;
}

//数据变更进行一次图像抓取即可
void CTestProcAntiCheatingSitUp::onRecord()
{
    grab();
}
