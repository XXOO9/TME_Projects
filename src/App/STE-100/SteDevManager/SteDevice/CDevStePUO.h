#ifndef CDevStePUO_H
#define CDevStePUO_H

#include "CDevHardwareBase.h"

/*************************************************
  <类    名>    CDevStePUO
  <功能描述>     体测具体设备：肺活量，都是由体测设备工厂来创建
                该设备没有独立的通讯机制

  <作    者>    cl
  <时    间>    2022-10-08
**************************************************/
class CDevStePUO : public CDevHardwareBase
{
public:
    //构建纯虚拟设备对象
    CDevStePUO(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevStePUO() override;

    //校准阈值（范围 5 ~ 30 cm）
    bool calibrateThreashold(const float &fValue);
    //校准停止时间，两次之间的间隔 倒计时
    bool calibrateInterval(int value);
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDevStePUO_H
