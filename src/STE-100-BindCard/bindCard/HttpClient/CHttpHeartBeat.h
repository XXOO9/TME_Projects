#ifndef CHTTPHEARTBEAT_H
#define CHTTPHEARTBEAT_H

#include <QObject>
#include "CHttpAbstractNetWork.h"

class CHttpHeartBeat : public CHttpAbstractNetWork
{
public:
    CHttpHeartBeat( QObject *parent = nullptr );
    virtual ~CHttpHeartBeat() override;

    virtual void initConfig() override;
    virtual void initConnection() override;
    virtual void httpRequest() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    void setStatus( devStatus status );
    void setBattery( QString battery );
    void setDevCode( QString devCode );
    void setDevType( DEV_TYPE devType );

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;
};

#endif // CHTTPHEARTBEAT_H
