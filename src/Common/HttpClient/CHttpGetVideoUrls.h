#ifndef CHTTPGETVIDEOURLS_H
#define CHTTPGETVIDEOURLS_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"
#include "CGetSyncVideoUrlsRet.h"

class COMMONSHARED_EXPORT CHttpGetVideoUrls : public CHttpAbstractNetWork
{
public:
    CHttpGetVideoUrls( QString timeStamp, QObject *parent = nullptr );
    virtual ~CHttpGetVideoUrls() override;

    virtual void httpRequest() override;

    int resultCount();
    QMap<QString, QString> getGroupRet( int index );
    QString getSingleRet( int index, QString key );

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CGetSyncVideoUrlsRet m_rets;
};

#endif // CHTTPGETVIDEOURLS_H
