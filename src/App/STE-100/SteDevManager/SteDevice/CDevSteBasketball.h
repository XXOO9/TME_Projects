#ifndef CDEVSTEBASKETBALL_H
#define CDEVSTEBASKETBALL_H

#include "CDevHardwareBase.h"

/*************************************************
  <类    名>    CDevSteBasketball
  <功能描述>     体测具体设备：肺活量，都是由体测设备工厂来创建
                该设备没有独立的通讯机制

  <作    者>    cl
  <时    间>    2022-10-08
**************************************************/
class CDevSteBasketball : public CDevHardwareBase
{
public:
    //构建纯虚拟设备对象
    CDevSteBasketball(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteBasketball() override;

    //在中考体育项目中，足球、篮球 需要类似跳绳广播启动一样，在一定时间内，连续发送启动命令
    virtual bool startWork(bool bCast, bool bWait = true) override;

    void setSuccessFlag(bool bFlag);

protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;

    //开始命令成功的标志
    bool m_bSuccessFlag{false};

    //篮球语音时间
    unsigned int m_uiVoiceTime{4000};
};

#endif // CDEVSTEBASKETBALL_H
