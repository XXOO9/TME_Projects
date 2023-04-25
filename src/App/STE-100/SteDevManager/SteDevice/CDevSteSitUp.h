#ifndef CDEVSTESITUP_H
#define CDEVSTESITUP_H
#include "CDevHardwareBase.h"
//仰卧起坐
class CDevSteSitUp : public CDevHardwareBase
{
public:
    CDevSteSitUp(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteSitUp() override;
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDEVSTESITUP_H
