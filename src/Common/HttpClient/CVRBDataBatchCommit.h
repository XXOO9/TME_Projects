#ifndef CVRBDATABATCHCOMMIT_H
#define CVRBDATABATCHCOMMIT_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

namespace VRBKeys {
 const QString paraArrayName   = "recordList";
 const QString userId          = "userId";
 const QString recordTime      = "recordTime";
 const QString rideMode        = "rideMode";
 const QString rideMap         = "rideMap";
 const QString rideMusic       = "rideMusic";
 const QString rideBikeType    = "rideBikeType";
 const QString rideDistance    = "rideDistance";
 const QString rideSpeed       = "rideSpeed";
 const QString rideSpeedAvg    = "rideSpeedAvg";
 const QString rideHrMax       = "rideHrMax";
 const QString rideHrAvg       = "rideHrAvg";
 const QString rideCalorieCost = "rideCalorieCost";
 const QString rideTimeCost    = "rideTimeCost";
}


class COMMONSHARED_EXPORT CVRBDataBatchCommit : public CHttpAbstractNetWork
{
public:
    CVRBDataBatchCommit( CHttpAbstractNetWork *parent = nullptr );
    virtual ~CVRBDataBatchCommit() override;

public:
    virtual void httpRequest() override;
    void setCommitParameters(const QList<QVariantMap> &paraMapList );
    QList<QVariantMap> errorElementList() const;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;
private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;
    virtual bool isRequestSuccess() override;
    virtual void parseParam( DATA_SRC dataSrc = UserExam ) override;

    void handleRequestError( QNetworkReply *pReply );

private:
    QList< QVariantMap >    m_errorElementList;  //请求失败的错误单元
    QList< QVariantMap >    m_commitParaMapList;
    QByteArray              m_sendJson;
};

#endif // CVRBDATABATCHCOMMIT_H
