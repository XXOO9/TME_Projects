#ifndef CTESTPROCANTICHEATINGSITSFB_H
#define CTESTPROCANTICHEATINGSITSFB_H
#include "CTestProcAntiCheatingBasic.h"

class CTestProcAntiCheatingSitSFB : public CTestProcAntiCheatingBasic
{
public:
    CTestProcAntiCheatingSitSFB();

protected:
    //数据变更触发当前函数
    virtual void onRecord() override;
};

#endif // CTESTPROCANTICHEATINGSITSFB_H
