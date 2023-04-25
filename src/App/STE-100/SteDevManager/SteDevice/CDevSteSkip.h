#ifndef CDEVSTESKIP_H
#define CDEVSTESKIP_H

#include "CDevHardwareBase.h"

/*
 * mxq 2022-02-18 跳绳数据包
 * 一个完整的跳绳信息共 16字节 包括：6字节的身份码(1字节设备类型+1字节数据点数+4字节身份码)
 * 2字节数据类型 + 2字节数据状态 + 4字节跳绳次数 + 1字节充电状态 + 1字节电量
 * 发送逻辑修改：仅发送存在的跳绳数据，若绳子不存在，则不发送对应跳绳信息，即不会存在全空数据
 */

class CDevSteSkip : public CDevHardwareBase
{
    Q_OBJECT
public:
    CDevSteSkip(unsigned int iDevID, ICommunicate &communicate);

    bool checkSkipBindState(const unsigned int &nCardId);

    virtual bool setElementBindFlag(const unsigned short &shIndex, bool bBind) override;

    //全部开启/全部结束
    virtual bool startWork(bool bCast, bool bWait = true) override;
    virtual bool stopWork(bool bCast, bool bWait = true) override;

    //指定设备开启/指定设备结束
    virtual bool startWork(unsigned short shElementIndex, bool bWait = true) override;
    virtual bool stopWork(unsigned short shElementIndex, bool bWait = true) override;

    void setSuccessFlag(bool bFlag);
signals:
    void signalStartSkipRope(const CLocation &location,const QDateTime &startTime);

    void signalSkipBind(const unsigned int &uiElementID);

    void signalCheckSubElement(const unsigned int &iSubID, const bool &bFlag);
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

    //重载父类定时器
    virtual void onUpdateTimeOut() override;

private:
    void checkSubElementStatus(bool bCheckDisconnect = false);

private:
    //绳子唯一编号 - 设备Index
    map<unsigned int, unsigned short> m_mapSkipToElement;

    //跳绳计时时间
    unsigned short m_usSkipTime{60};

    //是否停止发送命令的标志，即开始命令成功标志
    bool m_bSuccessFlag{false};

    //以下变量用于判断子设备是否断连

    //正常连接的跳绳设备
    set<unsigned short> m_setReceiveIndex;

    //已经断连的跳绳设备
    set<unsigned short> m_setDisconnectIndex;

    int m_iInsertNum{0};

    int m_iTargetNum{14};
};

#endif // CDEVSTESKIP_H
