#ifndef CHTTPSYNCUSERINFO_H
#define CHTTPSYNCUSERINFO_H

#include <QObject>
#include <QVector>
#include "CHttpAbstractNetWork.h"
#include "CHttpConfigCommonDefine.h"
#include "CAbstractResult.h"
#include "CSyncUserInfoRet.h"


class CHttpSyncUserInfo : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpSyncUserInfo( QString timeStamp = "", QObject *parent = nullptr );
    virtual ~CHttpSyncUserInfo() override;

    virtual void initConfig() override;
    virtual void initConnection() override;
    virtual void httpRequest() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    int resultCount();
    QMap<QString, QString> getGroupRet( int index );
    QString getSingleInfo( int index, QString key );

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CSyncUserInfoRet   m_ret;

};

#endif // CHTTPSYNCUSERINFO_H
