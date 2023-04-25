#ifndef CHTTPSYNCCLASSINFO_H
#define CHTTPSYNCCLASSINFO_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "CSyncClassInfoRet.h"

class CHttpSyncClassInfo : public CHttpAbstractNetWork
{
public:
    CHttpSyncClassInfo( QString timeSatmp = "", QObject *parent = nullptr );
    virtual ~CHttpSyncClassInfo() override;
    virtual void initConfig() override;
    virtual void initConnection() override;
    virtual void httpRequest() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    int resultCount();
    QMap<QString, QString> getGroupInfo( int index );
    QString getSingleRet( int index, QString key );

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CSyncClassInfoRet m_ret;
};

#endif // CHTTPSYNCCLASSINFO_H
