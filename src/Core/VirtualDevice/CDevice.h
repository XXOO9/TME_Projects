#ifndef CDEVICE_H
#define CDEVICE_H

#include <QString>
#include <QDate>
#include <ICommunicate.h>
#include <QTimer>
#include <QThread>

#include "IDataParse.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
/*************************************************
  <类    名>    CDevice
  <功能描述>     硬件设备的抽象类，主要是抽象的是本地设备对象
                例如蓝牙，串口，Socket设备，都是抽象的本地
                蓝牙、串口、Socket接口

  <作    者>     zhy
  <时    间>     2021/9/15
**************************************************/
class CDevice : public IDataParse
{
	Q_OBJECT

public:
    enum DevErr
    {

    };

    CDevice();

    virtual ~CDevice()override;

	/**
	 * 在操作设备前，判定设备是否是合法可用设备，具体设备具体判定
	 */
    virtual bool isValid();

    /**
     * 打开设备
     */
    virtual bool devOpen();

    /**
     * 关闭设备
     */
    virtual void devClose();

    /**
     * 获取设备基本信息
     */
    virtual void devGetBasicInfo();

    /**
     * 设备重置
     */
	virtual bool devReset();

    /**
     * 获取硬件设备号
     */
    inline const unsigned int& getMacID(){return m_iDevID;}
    inline void setMacID(unsigned int iMacID){m_iDevID = iMacID;}

    /**
     * 设置设备是否检查断连标志
     */
    inline void setDetermineDisconnectFlag(bool bDetermine){m_bDetermineDisconnect = bDetermine;}

    /**
     * 设置通讯的意义在于正常结束上一个通讯，同时正常初始化当前通讯
     */
    virtual void setCommunicate(ICommunicate *pCommunicate);

    //发送数据,后续只需要重载 根据数组发送的发送数据函数
    virtual bool sendData(QByteArray &array, bool bWait = false, int nMsec = 300, unsigned int uiCommandLevel = COMMAND_LEVEL_0, long long llStartTime = 0);
    virtual bool sendData(unsigned long length, void *pData, bool bWait, int nMsec, unsigned int uiCommandLevel, long long llStartTime = 0);

    /**
     * 清除设备数据，包含缓存数据和通讯接口数据
     */
    virtual void clearData();

    //表示设备是否正在发送数据
    bool getSendingDataState() const {return m_bSendingData;}
signals:
	/**
	 * 设备连接状态信号
	 */
    void signalDevStatus(bool bConnect, void *pDev);

    /**
     * 向设备发送数据或者命令，一般用于多线程向通讯接口发送数据
     */
    void signalSendDataToDev(const QByteArray &array, unsigned int uiCommandLevel, long long llStartTime);

    /**
     * 解析繁忙，出现此种问题说明数据解析逻辑存在问题，需要进行优化
     */
    void signalParseBusy();

protected:
	/**
     * 获取设备定时更新
     */
    virtual void onUpdateTimeOut();

    /**
     * 通讯出现错误,根据具体通讯错误进行处理
     */
    virtual void onComErrOccurred(){}

    //设置数据更新状态
    void setUpdateDataState(bool bOpen);

protected:
    //设备MAC ID
    unsigned int m_iDevID{ 0 };
	
    //设备是否已经打开
    std::atomic<bool> m_bOpened{ false };

    //设备的通讯接口
	ICommunicate *m_pCom{ nullptr };

    //设备信息更新定时器
    QTimer m_updateDevInfoTimer;

    //通讯线程
    QThread m_communicationThread;

	//设备信息更新时间间隔
    unsigned int m_iUpdateInteval{ 50 };

	//设备信息更新超时时间间隔，如果超时则认为设备断开
    unsigned int m_iNoDataInteval{ 4000 };

	//间隔总数
	unsigned int m_iIntervalTotal{ 0 };

    //每次解析数据的缓存大小
    int m_iCacheSize{ 1024 }; //仅在初始化缓存时使用
    int m_iExistCacheSize{0}; //每次获取数据之后使用

    //存储读取到数据，可能会遗留上一次未解析完的数据
    QByteArray m_Cache;

    //是否判断断连，某些设备需要判断断连某些不需要判断，在设备打开前进行设置
    bool m_bDetermineDisconnect{true};

    std::atomic<bool> m_bSendCommandSucess;

    //变量表示设备是否在发送命令,当前设置中发送命令存在等待，所以需要有变量表示状态
    std::atomic<bool> m_bSendingData{false};

    //函数中的 setUpdateDataState 可能会进行连续调用，使用变量表示活动状态
    //允许函数被多次调用
    std::atomic<bool> m_bUpdateDataState{false};
};

#endif // CDEVICE_H
