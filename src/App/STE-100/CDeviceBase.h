#ifndef CDEVICE_BASE_H
#define CDEVICE_BASE_H
/*************************************************
  <类    名>    CDevice
  <功能描述>     硬件设备的抽象类

  <作    者>     cpc
  <时    间>     2021/9/15
**************************************************/

//在使用当前类时，后续的设备类使用继承完善设备类
//设备类中包含了 通讯类基类指针 解析类基类指针
//其中的解析类比较特殊，由具体的使用方式进行区分，可分为继承使用和内部成员这两种方式
//可将两种方式都按照内部成员方式进行统一使用，方法是在继承解析类时，将成员变量设置为自身即可
//由于这里的原因，解析类中原本的继承关系不应该继承自QObject
#include "ICommunicate.h"
#include "CDataParseBase.h"
#include "CDevice.h"

//使用定时器需要响应，响应需要继承QObject
//另外的使用方式是，使用一个类继承QObject，专门触发数据获取函数，就不需要继承QObject
#include <QTimer>
#include <QString>
#include <QDate>
#include <atomic>

class CDeviceBase : public CDevice
{
    Q_OBJECT

    //定义每次从通讯类中获取数据的大小
    enum{GET_DATA_ARR_SIZE = 1024};
public:
    CDeviceBase();
    virtual ~CDeviceBase() override;

    //设置通讯时必须调用当前函数
    //设置通讯类型(通讯方式由外部决定，也可由设备内部生成，调用当前函数设置) 在父类定义

    //设置解析类时必须调用当前函数，
    void setDataParse(CDataParseBase *pDataParse);

    void clearData() override;

    virtual bool isValid() override;
    virtual void devClose() override;
    virtual bool devOpen() override;
    virtual bool devReset() override;

    /**
     * 设置自动重连标志
     */
    void setSelfReconnectFlag(bool bState);

    //通讯是否有效
    inline bool isValidCommunicate(){return m_pCom != nullptr;}

    //解析是否有效
    inline bool isValidDataParse(){return m_pDataParse != nullptr;}
signals:
    //设备连接变更,在断连信号中不能删除设备（信号限制）
    void signalConnectChange(void *pDev, bool bConnect);

    //在运行时内部通过发送重连信号来实现不阻塞的运行函数
    void signalReconnect();
protected:
    /**
     * 获取设备定时更新，内部涉及设备的重连，保证函数执行完成
     */
    virtual void onUpdateTimeOut() override;

    /**
     * 成功解析一帧函数触发消息，和解析类处理同一个线程，当前类需要设置到解析类中进行触发
     */
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen);
    //进行类回调函数绑定不能使用虚函数，这里进行一个中间函数处理
    void onDataParase(void *pData, unsigned int nDataLen);

    /**
     * 设备断开之后若需要重新连接，那么在设备开启之前需要设置重连标志，同时完善具体的重连函数
     * 重连逻辑在数据处理当中，若重写设备定时更新函数需要注意
     */
    virtual void reconnect();

    virtual void onComErrOccurred() override;

    /**
      * 数据解析对于当前来说无用
      */
    virtual  void dataParse(QByteArray &array) override{if(array.size())return;}

    //检测是否断连，通常的处理形式是判断锻炼时间是否超时，也可通过串口是否断连进行判断
    virtual bool detectDisconnected();
public:

    /**
     * 设备号
     */
    int getDevNO();

    /**
     * 设备名称
     */
    void setDevName(QString strName);

    /**
     * 设备名称
     */
    QString getDevName();

protected:
    //设备类型
    short m_shDevType{0};

protected:
    //解析类
    CDataParseBase *m_pDataParse{nullptr};

    //设备自行重连标志
    bool m_bSelfReconnectFlag{false}; //外部用于控制设备断连之后是否重连
    //设备重连时间间隔
    unsigned int m_iReconnectInteval{ 500 };
    unsigned int m_iReconnectIntevalTotal{ 0 };

    //设备信息更新定时器,设备在使用时可能线程和构造函数的timer不在一个线程
    //需要在具体的线程中定义同时 开启定时器
    QTimer *m_pTimerUpdateInfo{nullptr};
};

#endif // CDEVICE_BASE_H
