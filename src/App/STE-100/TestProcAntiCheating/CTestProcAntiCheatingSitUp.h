#ifndef CTESTPROCANTICHEATINGSITUP_H
#define CTESTPROCANTICHEATINGSITUP_H
#include "CTestProcAntiCheatingBasic.h"

//数据变更触发抓拍
class CTestProcAntiCheatingSitUp : public CTestProcAntiCheatingBasic
{
public:
    CTestProcAntiCheatingSitUp();

protected:
    //数据变更触发当前函数
    virtual void onRecord() override;
};

#endif // CTESTPROCANTICHEATINGSITUP_H
