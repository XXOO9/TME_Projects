#ifndef CDEVRUNNFC_H
#define CDEVRUNNFC_H
#include "CSerialPortCom.h"
#include "CDeviceBase.h"
#include "AssistDevManager/CDataParseRunNFC.h"

#include <QDateTime>
#include <atomic>
#include <QTimer>

//在外部设置解析，解析和外部的设备确认类使用一套协议
class CDevRunNFC : public CDeviceBase
{
    Q_OBJECT
public:
    CDevRunNFC();

    ~CDevRunNFC() override;

    void setComName(QString str);

    //在确认设备和开启当前设备时进行使用
    static void cfgComInfo(CSerialPortCom *pCom, const QString &comName);

    static QByteArray getCmdDevInfo();
protected:

    /**
     * 成功解析一帧函数触发消息，和解析类处理同一个线程，当前类需要设置到解析类中进行触发
     * 详细的解析看解析类中的东西
     */
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen) override;

    //重写断连判断函数，当前串口如果使用38400的串口底层不会返回心跳
    //通过串口是否存在进行判断
    bool detectDisconnected() override;
signals:
    void signalRunNFCRecognizeCard(unsigned int nCard);

private:
    //当前设备为串口设备类对象
    CSerialPortCom *m_pSerialPort{nullptr};

    //更新上一次卡识别时间
    qint64 m_int64LastCardTime;
    unsigned int m_nCardID{0};

    //解析出来的数据结构长度
    unsigned int m_nParseDataLen{sizeof (CDataParseRunNFC::PARSE_DATA_RUN_NFC)};

    //记录当前串口名称
    QString m_qsComName;

    QStringList m_listComName;
};

#endif // CDEVRUNNFC_H
