///////////////////////////////////////////////////////////
//  TestMode.h
//  Implementation of the Enumeration TestMode
//  Created on:      08-1月-2022 11:19:55
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_CB7AE8B3_D9D8_4950_8608_D2C760066B2A_INCLUDED_)
#define EA_CB7AE8B3_D9D8_4950_8608_D2C760066B2A_INCLUDED_
//目前的测试模式只有在组网状态下才会生效，在直连模式下测试模式是无效的
enum TestMode
{
    TEST_SYNC,  //同步测试
    TEST_ASYNC, //异步测试
    TEST_MODE_COUNT //记录测试模式数量(普遍意义表示直连)
};

static short getActualTestMode(const bool &bDirectConnect, const short &shCurTestMode)
{
    short shTestMode = TEST_MODE_COUNT;
    if(!bDirectConnect)
    {
        shTestMode = shCurTestMode;
    }

    return shTestMode;
}

#endif // !defined(EA_CB7AE8B3_D9D8_4950_8608_D2C760066B2A_INCLUDED_)
