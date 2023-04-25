#ifndef CTESTPROCBCI_H
#define CTESTPROCBCI_H
#include "CTestProcBasic.h"

class CTestProcBCI : public CTestProcBasic
{
public:
    CTestProcBCI();
protected:
    virtual void initTestProcess() override;
};

#endif // CTESTPROCBCI_H
