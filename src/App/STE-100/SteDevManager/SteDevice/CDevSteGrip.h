#ifndef CDEVSTEGRIP_H
#define CDEVSTEGRIP_H
#include "CDevHardwareBase.h"

class CDevSteGrip : public CDevHardwareBase
{
public:
    CDevSteGrip(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteGrip() override;
    //校准握力
    bool calibrateGrip(const float &fValue);
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDEVSTEGRIP_H
