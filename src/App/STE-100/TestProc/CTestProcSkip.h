#ifndef CTESTPROCSKIP_H
#define CTESTPROCSKIP_H
#include "CTestProcBasic.h"

class CTestProcSkip : public CTestProcBasic
{
public:
    CTestProcSkip();
    virtual ~CTestProcSkip();

    virtual void initTestProcess() override;
};

#endif // CTESTPROCSKIP_H
