#ifndef CHTTPTOURISTRECORD_H
#define CHTTPTOURISTRECORD_H

#include <QObject>
#include "CHttpAbstractNetWork.h"

class CHttpTouristRecord : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpTouristRecord( QObject *parent = nullptr );
    virtual ~CHttpTouristRecord() override;

    virtual void initConfig() override;
    virtual void initConnection() override;
    virtual void httpRequest() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    void setDeviceCode( QString deviceCode );
    void setDeviceType(DEV_TYPE deviceType );
    void setRecordTime( QString timeStamp );

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;
};

#endif // CHTTPTOURISTRECORD_H
