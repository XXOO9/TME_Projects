#ifndef CDEVSTEPULLUP_H
#define CDEVSTEPULLUP_H
#include "CDevHardwareBase.h"

//class CDealPullUp{
//public:
//    static void dealTestData(short shCurTestItem, QByteArray &arr);
//};

class CDevStePullUp : public CDevHardwareBase
{
public:
    CDevStePullUp(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevStePullUp() override;

    //校准阈值(（范围 5 ~ 30 cm）)
    bool calibrateThreashold(const float &fValue);
    //校准停止时间，两次之间的间隔
    bool calibrateInterval(int value);
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDEVSTEPULLUP_H
