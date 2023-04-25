#ifndef CConfirmDevFoulControl_H
#define CConfirmDevFoulControl_H
#include "CConfirmDevBase.h"

class CConfirmDevFoulControl : public CConfirmDevBase
{
public:
    CConfirmDevFoulControl();

protected:
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen) override;
};

#endif // CConfirmDevFoulControl_H
