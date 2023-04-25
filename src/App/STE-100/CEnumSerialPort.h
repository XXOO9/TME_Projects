#ifndef CENUMSERIALPORT_H
#define CENUMSERIALPORT_H
#include "CSingletonPattern.h"
#include "CSerialPortHelper.h"
#include <QMutex>
//软件中有多处地方会用到枚举串口的地方，在多线程中或者多个地方调用枚举串口可能会出现问题
//使用统一的类保证串口的使用，保证在一个时间段内仅有一个地方会使用到串口

class CEnumSerialPort : public CSingletonPtr<CEnumSerialPort>
{
    //声明为单例模式
    DECLARE_SINGLETON_CLASS(CEnumSerialPort)

public:
    CEnumSerialPort();

    //在使用串口之前调用
    bool lockUseSerialPortCom(int nMsec = 0);

    //获取串口
    const QStringList &getSerialPortCom();

    //获取可用串口
    const QStringList &getAvailableSerialPortCom();

    //在使用串口之后调用
    void unlockUseSerialPortCom();
private:
    QStringList m_listComName;
    QStringList m_listAvailableComName;

    QMutex m_mutexUse;

    CSerialPortHelper m_serialPortHelper;
};

#endif // CENUMSERIALPORT_H
