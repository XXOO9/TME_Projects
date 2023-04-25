#ifndef CTestGuideFootball_H
#define CTestGuideFootball_H
#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteVC.h"

class CTestGuideFootball : public CTestGuideBase
{
public:
    CTestGuideFootball();

    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

    static void setFirstStartFlag(const bool &bFirstStart){s_bFirstStartFlag = bFirstStart;}
    static bool getFirstStartFlag(){return  s_bFirstStartFlag;}

    static void setFirstStartTime(const long long &llFirstStartTime){s_llFirstStartTime = llFirstStartTime;}
    static long long getFirstStartTime(){return  s_llFirstStartTime;}

protected:
    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    //测试完成处理数据，只需要肺活量等相似测试继承重写
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement) override;

    //第一次启动的标志
    static atomic<bool> s_bFirstStartFlag;
    //第一次启动的时间
    static atomic<long long> s_llFirstStartTime;
};

#endif // CTestGuideFootball_H
