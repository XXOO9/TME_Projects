#ifndef CIDENTIFICDEVNFC_H
#define CIDENTIFICDEVNFC_H
#include "../../thirdparty/RFID1356/include/RFID1356.h"

#include <QTimer>
#include <QObject>
#include <string>


using namespace std;

//一般情况在线程中进行识别
class  CIdentificDevNFC : public QObject
{
    Q_OBJECT
public:
    enum emInterfaceComOrUsb{
            Interface_HID, Interface_PCSC};

    //函数采用信号触发，是避免多线程启动直接调用函数出问题
    void open(bool bAutoList = true);
    void close();
signals:
    ///通知外部信号
    //成功识别卡号
    void signalFindCard(QString strCardID);
    //设备连接状态变更
    void signalConnectChange(bool bConnect);

    //识别是否存在卡
    void sigDecetedCard(bool exist);

    ///内部使用信号
    void signalOpen(bool bAtoList);
    void signalClose();
protected slots:
    void onListCardTimeOut();
    void onOpen(bool bAutoList);
    void onClose();

    void onReconnectTimeOut();
public:
    CIdentificDevNFC();
    ~CIdentificDevNFC();

    //在打开设备之前设置
    void setInterfaceMode(short shMode);
    
    bool autoListCard(int nMesc);

    bool buzzing();
    bool isCardExist() const;
    void setIsCardExist(bool isCardExist);

protected:
    
    void startTimer(int nMesc);
    
    void INTANSFER_();
    void READ_CARDID_();
    void READ_QUEUE_();

    void startReconnectTimer();
private:
    short m_shInterfaceMode;
    bool m_bDevOpenStatus{false};
    bool m_bAutoListCard{false};

    //定时器使用指针是为了多线程使用
    QTimer *m_pTimerListCard{nullptr};
    QTimer *m_pTimerReconnect{nullptr};
        
    volatile int  READ_CARDID_TEMP = 0;
    volatile int  READ_CARDID = 0;
    volatile unsigned int READ_CARDTICK = 0;
    volatile int READ_QUEUE_TEMP = 0;
    volatile int READ_QUEUE = 0;
    volatile unsigned int READ_QUEUETICK = 0;
    volatile int INTANSFER = 0;

    //string
    string m_strLastCardID;
    bool   m_isCardExist;
};

#endif // CIDENTIFICDEVNFC_H
