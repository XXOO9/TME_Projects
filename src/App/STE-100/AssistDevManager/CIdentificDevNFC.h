#ifndef CIDENTIFICDEVNFC_H
#define CIDENTIFICDEVNFC_H
#include "RFID1356.h"

#include <QTimer>
#include <QObject>
#include <string>
using namespace std;

//一般情况在线程中进行识别
class CIdentificDevNFC : public QObject
{
    Q_OBJECT
public:
    enum emInterfaceComOrUsb{
            Interface_HID, Interface_PCSC};

    //函数采用信号触发，是避免多线程启动直接调用函数出问题
    void open(bool bAutoList = true);
    void close();

    void readCard();
signals:
    ///通知外部信号
    //成功识别卡号
    void signalFindCard(const string &strCardID);
    //设备连接状态变更
    void signalConnectChange(bool bConnect);

    ///内部使用信号
    void signalOpen(bool bAtoList);
    void signalClose();
protected slots:
    void onOpen(bool bAutoList);
    void onClose();

    void onTimeOut();
public:
    CIdentificDevNFC();
    ~CIdentificDevNFC();

    //在打开设备之前设置
    void setInterfaceMode(short shMode);
    
    //仅仅是开启定时器进行寻卡
    bool startAutoListCard();

    bool buzzing();
protected:
    
    void INTANSFER_();
    void READ_CARDID_();
    void READ_QUEUE_();

    //开启定时器，分为寻卡定时器和重连定时器，将两个定时器
    //写在一起的原因是两个定时器只能同时开启一个
    void startTimerCustom();
private:
    short m_shInterfaceMode{Interface_HID};
    bool m_bDevOpenStatus{false};
    bool m_bAutoListCard{false};

    //定时器使用指针是为了多线程使用
    QTimer *m_pTimer{nullptr};
    int m_nUpdateTimeOut{100}; //100ms刷新一次定时器

    //寻卡间隔
    int m_nReadCardInterval{300};
    int m_nReadCardTotal{0};
    //重连间隔
    int m_nReconnectInterval{1000};
    int m_nReconnectTotal{0};

    //变量表示是否处于寻卡
    bool m_bReadCardState{false};

    volatile int  READ_CARDID {0};
    volatile unsigned int READ_CARDTICK {0};
    volatile int READ_QUEUE {0};
    volatile unsigned int READ_QUEUETICK {0};
    volatile int INTANSFER {0};

    //string
    string m_strLastCardID{""};

    //第一次尝试重连标志
    bool m_bFirstReconnectFlag{true};
};

#endif // CIDENTIFICDEVNFC_H
