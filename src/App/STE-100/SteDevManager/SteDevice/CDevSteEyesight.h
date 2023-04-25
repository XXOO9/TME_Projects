#ifndef CDEVSTEEYESIGHT_H
#define CDEVSTEEYESIGHT_H
#include "CDevHardwareBase.h"

class CDevSteEyesight : public CDevHardwareBase
{
public:
    CDevSteEyesight(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteEyesight() override;
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDEVSTEEYESIGHT_H
