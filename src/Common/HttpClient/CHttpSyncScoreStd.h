#ifndef CHTTPSYNCSCORESTD_H
#define CHTTPSYNCSCORESTD_H

#include "CHttpAbstractNetWork.h"
#include <QObject>
#include "CSyncScoreStdRet.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CHttpSyncScoreStd : public CHttpAbstractNetWork
{
public:
    CHttpSyncScoreStd( QObject *parent = nullptr );
    virtual ~CHttpSyncScoreStd() override;

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
    CSyncScoreStdRet    m_ret;
};

#endif // CHTTPSYNCSCORESTD_H
