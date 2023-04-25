#ifndef CHTTPUPLOADCOMMONSCORE_H
#define CHTTPUPLOADCOMMONSCORE_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../Logger/CLogger.h"
#include "../common_global.h"

namespace upLoadCommonScore {
            const QString userId       = "userId";
            const QString socreType    = "type";
            const QString testItem     = "testItem";
            const QString result       = "result";
            const QString recordTime   = "recordTime";
            const QString processPics  = "files";
            const QString deviceCode   = "deviceCode";
            const QString deviceType   = "deviceType";
            const QString signPicPath  = "signPic";
}

class COMMONSHARED_EXPORT CHttpUpLoadCommonScore : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpUpLoadCommonScore( QObject *parent = nullptr );
    virtual ~CHttpUpLoadCommonScore() override;

    virtual void httpRequest() override;

    void setUserId( QString userId );
    void setScoreType(testType type );
    void setTestItem( TEST_ITEM testItem );
    void setResult( QString result );
    void setRecordTime( QString recordTime );
    void setProcessPics( QStringList paths );
    void setDeviceCode( QString deviceCode );
    void setDeviceType( short deviceType );
    void setSignPicPath( QString signPath );

    void setParaMap(QMap<QString, QVariant> infos );

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;



public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:

};

#endif // CHTTPUPLOADCOMMONSCORE_H
