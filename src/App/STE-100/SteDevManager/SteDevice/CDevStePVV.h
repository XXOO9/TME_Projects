#ifndef CDEVSTEPVV_H
#define CDEVSTEPVV_H

#include "CDevHardwareBase.h"

class CDevStePVV : public CDevHardwareBase
{
public:
    //构建纯虚拟设备对象
    CDevStePVV(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevStePVV() override;
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDEVSTEPVV_H
