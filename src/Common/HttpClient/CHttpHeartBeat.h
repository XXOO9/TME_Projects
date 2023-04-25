#ifndef CHTTPHEARTBEAT_H
#define CHTTPHEARTBEAT_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CHttpHeartBeat : public CHttpAbstractNetWork
{
public:
    CHttpHeartBeat( QObject *parent = nullptr );
    virtual ~CHttpHeartBeat() override;

    virtual void httpRequest() override;

    void setStatus( devStatus status );
    void setBattery( QString battery );
    void setDevCode( QString devCode );
    void setDevType( short devType );

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;
public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;
};

#endif // CHTTPHEARTBEAT_H
