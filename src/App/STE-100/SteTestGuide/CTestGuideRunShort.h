///////////////////////////////////////////////////////////
//  CTestFlowRunShort.h
//  Implementation of the Class CTestGuideRunShort
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_29466F3D_4DD9_483f_BF90_4F317F980F53_INCLUDED_)
#define EA_29466F3D_4DD9_483f_BF90_4F317F980F53_INCLUDED_

/*
 * 短跑分为两类：同步 与 异步
 * 同步：有明确的开始,明确的结束
 */

#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteRunShort.h"
#include "CDatabaseManage.h"

class CTestGuideRunShort : public CTestGuideBase
{

public:
    CTestGuideRunShort();
    virtual ~CTestGuideRunShort() override;
    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

    virtual bool startTest(const bool &bExecuteStart, const bool &bCast) override;

    virtual bool startTest(const unsigned short &shElementIndex, const bool &bFirstTest, const bool &bExecuteStart) override;

    virtual bool stopTest(const bool &bExecuteStart, const bool &bCast) override;

    void setTestItem(const short &shTestItem);
    static short getTestItem(){ return s_shTestItem; }

    void setUseStartFlag(const bool &bUseFlag);
    static bool getUseStartFlag(){ return s_bUseStartFlag; }

    static void setStartGunExistFlag(const bool &bExistFlag);

    //测试项目变更
    virtual void onTestItemChanged() override;

    //无起点短跑重测，需要调用此函数，进行对应设置
    void reStartByNoPoint();

protected:
    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;
private:
    void saveDataToResult(const unsigned short &shElementIndex);

    void completeTest(const unsigned short &shElementIndex);

    unsigned int sendGetTimeCmd(CDevSteRunShort* pDevRunShort);

private:
    static unsigned int s_uiTargetLap;

    static short s_shTestItem;

    static bool s_bUseStartFlag;

    static bool s_bExistFlag;
};
#endif // !defined(EA_29466F3D_4DD9_483f_BF90_4F317F980F53_INCLUDED_)
