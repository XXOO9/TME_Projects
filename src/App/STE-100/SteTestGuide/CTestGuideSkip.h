#ifndef CTESTGUIDESKIP_H
#define CTESTGUIDESKIP_H

/*
 * 跳绳分为两类：同步 与 异步
 * 同步：所有子设备共用同一个计时器
 * 异步：单独子设备中绑定一个计时器
 */

#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteSkip.h"

class CTestGuideSkip : public CTestGuideBase
{
    Q_OBJECT
public:
    CTestGuideSkip();
    virtual ~CTestGuideSkip() override;
    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

    bool checkSkipBindState(const unsigned int &nCardId);

    void stopUpdate(const unsigned short &shElementIndex);

    bool checkStartSuccess(const unsigned short &shElementIndex);

    int  getSkipTestState(const unsigned short &shElementIndex);

    void setRestartFlag(const unsigned short &shElementIndex, const bool &bFlag);

    bool getRestartFlag(const unsigned short &shElementIndex);

    void forcedStopTest(const unsigned short &shElementIndex);

    virtual bool bindUserToElement(const unsigned short &shElementIndex, const CUserInfo &userInfo) override;

    virtual void procTestEnd(const unsigned short &shElementIndex) override;

    static void setFirstStartFlag(const bool &bFirstStart){s_bFirstStartFlag = bFirstStart;}
    static bool getFirstStartFlag(){return  s_bFirstStartFlag;}
protected:
    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    //跳绳不重载基类函数，使用类似逻辑的函数
    void saveDataToResult(const unsigned short &shElementIndex);

    //开始启动的原子变量，用于跳绳设备的广播启动
    static atomic<bool> s_bFirstStartFlag;
};

#endif // CTESTGUIDESKIP_H
