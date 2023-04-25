#ifndef ICOMENUMERATE_H
#define ICOMENUMERATE_H

#include <QObject>
#include <QString>
#include <QMutex>

#include "CCycleQueue.h"

//连接状态
const   unsigned short      NO_CONNECT          = 0;         //当前无连接
const   unsigned short      CONNECTED           = 1;         //当前为正常连接

//通讯接口类型
enum CommunicateType
{
    SerialPort = 0,
    USB = 1,
    Bluetooth = 2,
    Network = 3
};

//命令优先级(命令要根据此属性依次处理)
enum CommandLevel
{
    COMMAND_LEVEL_0 = 0,
    COMMAND_LEVEL_1 = 1,
    COMMAND_LEVEL_2 = 2
};

class ICommunicate : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief 打开模式
     */
    enum OpenMode
    {
        ReadOnly = 0,
        WriteOnly = 1,
        ReadWrite = 2,
    };

public:
    ICommunicate(){connect(this, &ICommunicate::signalSendData, this, &ICommunicate::onSendData);}
    //根据通讯类型创建对应的硬件对象
    virtual ~ICommunicate(){}

    /**
     * @brief 确定通讯的有效项
     * @return
     */
    virtual bool IsValid() = 0;

    /**
     * @brief 设置打开串口的模式
     * @param mode
     */
    virtual void setOpenMode(OpenMode mode) = 0;

    /**
     * @brief 确定通讯端是否打开
     * @return
     */
    virtual bool IsOpened() = 0;

    /**
     * @brief 打开本地接口
     * @return
     */
    virtual bool open() = 0;

    /**
     * @brief 关闭本地接口
     * @return
     */
    virtual void close() = 0;

    /**
     * @brief 重置本地接口
     * @return
     */
    virtual bool reset() = 0;

    /**
     * @brief 获取当前缓存数据总字节数
     * @return
     */
    virtual unsigned long getDataSize() = 0;

    /**
     * @brief 清空缓存数据
     */
    virtual void clearData() = 0;

    /**
     * @brief 读取数据
     * @param length pData缓存的大小
     * @param pData 数据缓存
     * @return 实际读取的个数
     */
    virtual unsigned int readData(unsigned int length, void *pData) = 0;


    /**
     * @brief 发送数据
     * @param length pData缓存的大小
     * @param pData 数据缓存
     * @return 实际发送的个数
     */
    virtual unsigned int sendData(unsigned int length, void *pData, unsigned int uiCommandLevel = 0, long long llStartTime = 0) = 0;
    virtual unsigned int sendData(const QByteArray &array, unsigned int uiCommandLevel = 0, long long llStartTime = 0) = 0;

    /**
     * @brief 数据存放到缓存队列
     * @param length pData缓存的大小
     * @param pData 数据缓存
     * @return 实际存储个数
     */
    virtual unsigned int stortDataToReceQueue(void *pData, unsigned int length) = 0;

    /**
     * @brief 设置接收循环队列大小，在open之前调用
     * @param lDataPointNum 数据点个数
     * @return
     */
    inline void setReceiveQueSize(unsigned long lDataPointNum){m_lReceiveQueDataNum = lDataPointNum;}

    inline unsigned long getLastErr(){return m_lLastErr;}
signals:
    /**
     * @brief 更新连接状态
     * @param bStatus 是否连接上
     */
    //void updateConnectStatus(bool bStatus);

    /**
     * @brief 更新硬件设备出错信息
     * @param strDesription 错误描述
     * @param strError 错误内容
     */
    //void reportPortErrorSignal(QString strDesription, QString strError) ;

    //外部若是跨线程方式发送通过当前信号
    void signalSendData(const QByteArray &array, unsigned int uiCommandLevel, long long llStartTime);

    //通信发生错误时发送消息，在具体的通讯中根据情况进行发送
    void signalErrOccurred();
public slots:
    void onSendData(const QByteArray &arr, unsigned int uiCommandLevel, long long llStartTime){sendData(arr,uiCommandLevel,llStartTime);}

protected:
    /**
     * @brief 数据接收缓冲循环队列
     */
    CCycleQueue<unsigned char> *m_pReceiveQueue{ nullptr };

    /**
     * @brief 数据发送缓冲循环队列
     */
    CCycleQueue<unsigned char> *m_pSendQueue{nullptr};

    //使用循环队列锁
    QMutex m_mutexReceiveQueue;

    //缓存队列数据点数
    unsigned long m_lReceiveQueDataNum{2048};

    //用于保存通讯错误
    unsigned long m_lLastErr;
};

#endif // ICOMENUMERATE_H
