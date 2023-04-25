#ifndef CTESTPROCWEIGHT_H
#define CTESTPROCWEIGHT_H
#include "CTestProcBasic.h"

class CTestProcWeight : public CTestProcBasic
{
public:
    CTestProcWeight();
    virtual ~CTestProcWeight() override;
protected:
    virtual void initTestProcess() override;
};

#endif // CTESTPROCWEIGHT_H
