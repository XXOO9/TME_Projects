#ifndef CDATAPARSESTARTINGGUN_H
#define CDATAPARSESTARTINGGUN_H
#include "CDataParseBase.h"

class CDataParseStartingGun : public CDataParseBase
{
public:
    CDataParseStartingGun();

protected:
    virtual void dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData) override;
};

#endif // CDATAPARSESTARTINGGUN_H
