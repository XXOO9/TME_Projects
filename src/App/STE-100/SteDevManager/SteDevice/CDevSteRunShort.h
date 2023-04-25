#ifndef CDevSteRunShort_H
#define CDevSteRunShort_H
#include "CDevHardwareBase.h"

/*
 * mxq 2022-01-21 短跑数据包格式发生变化
 * 一个完整的跑道信息共 28字节 包括：6字节的身份码(1字节设备类型+1字节数据点数+4字节身份码)
 * 2字节数据类型 + 2字节数据状态 + 8字节时间戳 + 4字节短跑时间 + 2字节跑道ID + 2字节跑道总数 + 2字节标志
 * 发送逻辑修改：仅发送存在的跑道数据，若通道不存在，则不发送对应跑道信息，即不会存在全空数据
 */

//短跑设备
class CDevSteRunShort : public CDevHardwareBase
{
public:
    CDevSteRunShort(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteRunShort() override;

    //得到已连接的跑道数量
    int getConnectSubElementNum();

    void setUseStartFlag(bool bFlag);

    //在不使用起点的情况下，需要调用以下两个函数
    //获取开始时间
    bool getStartTime();

    //设置开始时间
    bool setStartTime(unsigned int uiDelayTime = 0);
    bool setStartTime(unsigned short usElementIndex, unsigned int uiDelayTime = 0);

    //存在错误返回true
    bool existErrInBindElement() override;

    //开启关闭当前类型设备
    virtual bool startWork(bool bCast, bool bWait = true) override;
    virtual bool stopWork(bool bCast, bool bWait = true) override;

    //开启关闭当前设备 下面的确切设备
    virtual bool startWork(unsigned short shElementIndex, bool bWait = true) override;
    virtual bool stopWork(unsigned short shElementIndex, bool bWait = true) override;
protected:
    //单包解析
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

    //重载父类定时器
    virtual void onUpdateTimeOut() override;

private:
    unsigned short m_usRunwayTotalNum{0};

    set<unsigned short> m_setReceiveStart;
    set<unsigned short> m_setReceiveEnd;

    int m_iInsertNum{0};

    int m_iTargetNum{14};

    bool m_bUseStartFlag{true};
};

#endif // CDevSteRunShort_H
