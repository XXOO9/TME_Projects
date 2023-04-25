#ifndef CTESTPROCANTICHEATINGVC_H
#define CTESTPROCANTICHEATINGVC_H
#include "CTestProcAntiCheatingBasic.h"

class CTestProcAntiCheatingVC : public CTestProcAntiCheatingBasic
{
public:
    CTestProcAntiCheatingVC();

protected:
    //数据变更触发当前函数
    virtual void onRecord() override;
};

#endif // CTESTPROCANTICHEATINGVC_H
