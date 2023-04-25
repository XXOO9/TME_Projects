#ifndef CDEVSTESFB_H
#define CDEVSTESFB_H
#include "CDevHardwareBase.h"

class CDevSteSFB : public CDevHardwareBase
{
public:
    CDevSteSFB(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteSFB() override;

    //校准刻度
    bool calibrateScale(const float &fValue);
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDEVSTESFB_H
