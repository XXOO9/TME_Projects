#ifndef CDevSteSolidball_H
#define CDevSteSolidball_H

#include "CDevHardwareBase.h"

/*************************************************
  <类    名>    CDevSteSolidball
  <功能描述>     体测具体设备：肺活量，都是由体测设备工厂来创建
                该设备没有独立的通讯机制

  <作    者>    cl
  <时    间>    2022-10-08
**************************************************/
class CDevSteSolidball : public CDevHardwareBase
{
public:
    //构建纯虚拟设备对象
    CDevSteSolidball(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteSolidball() override;

    //设置起跳点位置 Start_Pos 0~65535 cm
    void setStartPos(const int &pos);

    //是否校准了起点
    bool getCalibrationSPosFlag();
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDevSteSolidball_H
