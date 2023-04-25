#ifndef CConfirmDevBase_H
#define CConfirmDevBase_H

//设备确认对象，由外部指定当前设备对象类型( 适用的场景为握手场景 )
//从外部指定当前使用的通讯类型 和对应的所有信息
//从外部指定设备确认所需要的命令
#include "ICommunicate.h"
#include "IDataParse.h"
#include "CDeviceBase.h"

#include <QByteArray>

class CConfirmDevBase : public CDeviceBase
{
    Q_OBJECT
public:
    CConfirmDevBase();
    virtual ~CConfirmDevBase() override;

    //设置返回命令,发送确认命令之后，底层会返回一个确认命令码，若和设置的值相同，那么表示成功返回
    void setReturnConfirmCmd(size_t szConfirmCmd);

    //发送确认消息
    bool confirmDev(QByteArray &cmd, int nWaitMsec = 1000);

    //开启设备，关闭设备
    /**
     * 打开设备
     */
    virtual bool devOpen() override;

    /**
     * 关闭设备， 确认设备不需要在一个线程中运行不需要进行定时器的频繁删除 申请，只进行关闭即可
     */
    virtual void devClose() override;
protected:
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen) override;
protected:

    //初始规定通用的命令为某一个特定的命令值，这里还需要通用化
    size_t m_szReturnConfirmCmd{0};
};

#endif // CConfirmDevBase_H
