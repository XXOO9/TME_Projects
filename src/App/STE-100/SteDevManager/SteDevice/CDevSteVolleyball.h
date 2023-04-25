#ifndef CDevSteVolleyball_H
#define CDevSteVolleyball_H

#include "CDevHardwareBase.h"

/*************************************************
  <类    名>    CDevSteVolleyball
  <功能描述>     体测具体设备：肺活量，都是由体测设备工厂来创建
                该设备没有独立的通讯机制

  <作    者>    cl
  <时    间>    2022-10-08
**************************************************/
class CDevSteVolleyball : public CDevHardwareBase
{
public:
    //构建纯虚拟设备对象
    CDevSteVolleyball(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteVolleyball() override;

    bool calibrateInterval(int value);
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDevSteVolleyball_H
