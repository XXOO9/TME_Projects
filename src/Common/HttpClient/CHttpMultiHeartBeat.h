#ifndef CHTTPMULTIHEARTBEAT_H
#define CHTTPMULTIHEARTBEAT_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "CHttpUpLoadCommonScore.h"
#include "../Logger/CLogger.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CHttpMultiHeartBeat : public CHttpAbstractNetWork
{
public:
    CHttpMultiHeartBeat( QObject* parent = nullptr );
    virtual ~CHttpMultiHeartBeat()  override;

    virtual void httpRequest() override;

    void setHeartBeatInfos( const QVariantList &heartBeatInfos );
    QString lastSyncTime() const;

    QString currentTime() const;

    QString schoolId() const;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;


private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;
    virtual bool isRequestSuccess() override;

private:
    QByteArray  m_heartBeatInfos = "";
    QString     m_currentTime = "";
    QString     m_lastSyncTime = "";
    QString     m_schoolId = "";
};

#endif // CHTTPMULTIHEARTBEAT_H
