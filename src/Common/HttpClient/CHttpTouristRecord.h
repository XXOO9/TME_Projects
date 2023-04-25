#ifndef CHTTPTOURISTRECORD_H
#define CHTTPTOURISTRECORD_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CHttpTouristRecord : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpTouristRecord( QObject *parent = nullptr );
    virtual ~CHttpTouristRecord() override;

    virtual void httpRequest() override;

    void setDeviceCode( QString deviceCode );
    void setDeviceType(short deviceType );
    void setRecordTime( QString timeStamp );

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;
};

#endif // CHTTPTOURISTRECORD_H
