#ifndef CDevSteVertivalJump_H
#define CDevSteVertivalJump_H

#include "CDevHardwareBase.h"

/*************************************************
  <类    名>    CDevSteVertivalJump
  <功能描述>     体测具体设备：肺活量，都是由体测设备工厂来创建
                该设备没有独立的通讯机制

  <作    者>    cl
  <时    间>    2022-10-08
**************************************************/
class CDevSteVertivalJump : public CDevHardwareBase
{
public:
    //构建纯虚拟设备对象
    CDevSteVertivalJump(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteVertivalJump() override;
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDevSteVertivalJump_H
