#ifndef CCONFIRMDEVPOWERUART_H
#define CCONFIRMDEVPOWERUART_H
#include "CConfirmDevBase.h"

class CConfirmDevPowerUart : public CConfirmDevBase
{
public:
    CConfirmDevPowerUart();

protected:
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen) override;
};

#endif // CCONFIRMDEVPOWERUART_H
