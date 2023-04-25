#ifndef CSERIALPORTCOM_H
#define CSERIALPORTCOM_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <queue>
#include <QTimer>

#include "ICommunicate.h"

using namespace std;

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
/*************************************************
  <类    名>    CSerialPortCom
  <功能描述>     该类直接和采集器进行串口通讯，提供基本的
                串口通讯读写接口，将COM口传递包数据解析为对应的帧数据

  <作    者>     cpc
  <时    间>     2021/9/16
**************************************************/

class  CSerialPortCom : public ICommunicate
{
    Q_OBJECT

public:
    /**
     * @brief 利用助手枚举的目标串口初始化串口通讯对象
     * @param portInfo
     */
    CSerialPortCom();
    CSerialPortCom(QSerialPortInfo portInfo);
    virtual ~CSerialPortCom() override;

    /**
     * @brief 确定通讯的有效项
     * @return
     */
    virtual bool IsValid() override;

    /**
     * @brief 确定串口是否打开
     * @return
     */
    virtual bool IsOpened() override;

    /**
     * @brief 打开串口设备
     * @return
     */
    virtual bool open() override;

    /**
     * @brief 关闭串口
     * @return
     */
    virtual void close() override;

    /**
     * @brief 重置串口
     * @return
     */
    virtual bool reset() override;

    /**
     * @brief 获取当前串口缓存数据总字节数
     * @return
     */
    virtual unsigned long getDataSize() override;

    /**
     * @brief 清空串口缓存数据
     */
    virtual void clearData() override;

    /**
     * @brief 读取数据
     * @param length pData缓存的大小
     * @param pData 数据缓存
     * @return 实际读取的个数
     */
    virtual unsigned int readData(unsigned int length, void *pData) override;


    /**
     * @brief 发送数据
     * @param length pData缓存的大小
     * @param pData 数据缓存
     * @return 实际发送的个数
     */
    virtual unsigned int sendData(unsigned int length, void *pData, unsigned int uiCommandLevel = 0, long long llStartTime = 0) override;
    virtual unsigned int sendData(const QByteArray &array, unsigned int uiCommandLevel = 0, long long llStartTime = 0)override;

    /**
     * @brief 设置打开串口的模式
     * @param mode
     */
    void setOpenMode(OpenMode mode) override;

    /**
     * @brief 获取串口名
     * @param mode
     */
    QString getComName();

    void setComName(QString qsComname);

    /**
     * @brief 设置波特率
     * @param baudRate
     */
    void setPortBaudRate(QSerialPort::BaudRate baudRate = QSerialPort::BaudRate::Baud115200);

    /**
     * @brief 设置数据位
     * @param dataBits
     */
    void setPortDataBits(QSerialPort::DataBits dataBits = QSerialPort::DataBits::Data8);

    /**
     * @brief 设置检验位
     * @param parity
     */
    void setPortParity(QSerialPort::Parity parity = QSerialPort::Parity::NoParity);

    /**
     * @brief 设置停止位
     * @param stopBits
     */
    void setPortStopBits(QSerialPort::StopBits stopBits = QSerialPort::StopBits::OneStop);

    virtual unsigned int stortDataToReceQueue(void *pData, unsigned int length) override;

    void onSendCommandTimeOut();

    //数据终端准备好 DTR（Data Terminal Ready）
    //请求发送 RTS（Request To Send）
    void setDTR(bool bState);

    /**
     * @brief 设置是否使用队列发送消息
     * @param stopBits
     */
    void setUseQueueFlag(bool bUse){m_bUseQueue = bUse;}
signals:
    /**
     * @brief 更新连接状态
     * @param bStatus 是否连接上
     */
    void updateConnectStatus(bool bStatus);
private slots:

    /**
     * @brief 串口数据接收处理
     * @param byteArray 数据字节数据
     */
    void onReceivedData();

    /**
     * @brief 串口出错管理
     * @param error
     */
    void onSerialPortError(QSerialPort::SerialPortError error);
protected:

    /**
     * @brief 初始化一些属性
     */
    virtual void initialize();

    /**
     * @brief 释放资源
     */
    virtual void uninitialize();
private:

    /**
     * @brief 标识是否打开
     */
    bool m_bOpened{false};

    /**
     * @brief 串口的打开模式
     */
    int m_iOpenMode{QIODevice::ReadWrite};

    /**
     * @brief 当前串口连接状态
     */
    int m_nconnectStatus{NO_CONNECT};

    /**
     * @brief 串口参数
     */
    QSerialPort::BaudRate m_baudRate{QSerialPort::Baud115200};
    QSerialPort::DataBits m_dataBits{QSerialPort::Data8};
    QSerialPort::StopBits m_stopBits{QSerialPort::OneStop};
    QSerialPort::Parity m_parity{QSerialPort::NoParity};

    //串口对象指针
    QSerialPort    *m_pSerialPort{nullptr};

    //串口信息
    QSerialPortInfo m_serialPortInfo;

    //命令分为：心跳消息，设备命令，通过定时器处理(优先处理设备命令)
    queue<QByteArray> m_queueHeartCommand;
    queue<QByteArray> m_queueDeviceCommand;

    //新增专用于跳绳的队列命令(最高优先级)
    queue<QByteArray> m_queueSkipCommand;
    long long       m_llStartTime{0};
    unsigned short  m_usWaitTime{5000};

    QTimer          m_timerSendCommand;
    int             m_iTimerInterval{100};
    QMutex          m_mutexSendCommand;

    bool            m_bUseQueue{false};
};

#endif // CSERIALPORTCOM_H
