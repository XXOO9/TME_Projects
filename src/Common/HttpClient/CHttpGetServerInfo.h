#ifndef CHTTPGETSERVERINFO_H
#define CHTTPGETSERVERINFO_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

class CHttpGetServerInfo : public CHttpAbstractNetWork
{
public:
    CHttpGetServerInfo();
    virtual ~CHttpGetServerInfo() override;


    virtual void httpRequest() override;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;


};

#endif // CHTTPGETSERVERINFO_H
