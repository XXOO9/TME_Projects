#ifndef CCONFIRMDEVRUN_H
#define CCONFIRMDEVRUN_H
#include "CConfirmDevBase.h"

class CConfirmDevRun : public CConfirmDevBase
{
public:
    CConfirmDevRun();
protected:
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen) override;
};

#endif // CCONFIRMDEVRUN_H
