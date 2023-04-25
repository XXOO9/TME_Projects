#ifndef CDEVSTARTINGGUN_H
#define CDEVSTARTINGGUN_H
#include "CSerialPortCom.h"
#include "CDeviceBase.h"
#include "CDataParseStartingGun.h"

class CDevStartingGun : public CDeviceBase
{
    Q_OBJECT
public:
    CDevStartingGun();

    //配置com口信息
    void setComName(QString str);

signals:
    //开始指令
    void signalStartingGun();
protected:
    virtual void onDataParseSucess(void *pData, unsigned int &nDataLen) override;

    //重写断连判断函数,串口底层不会返回心跳
    //通过串口是否存在进行判断
    bool detectDisconnected() override;

    //记录当前串口名称
    QString m_qsComName;

    //当前设备为串口设备类对象
    CSerialPortCom *m_pSerialPort{nullptr};
    CDataParseStartingGun m_objDataParse;
};

#endif // CDEVSTARTINGGUN_H
