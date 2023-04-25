#ifndef CHTTPUPLOADEYESIGHTSCORE_H
#define CHTTPUPLOADEYESIGHTSCORE_H

#include <QObject>
#include <QDebug>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

namespace upLoadEyeScore {
            const QString userId               = "userId";
            const QString socreType            = "type";
            const QString testItem             = "testItem";
            const QString result               = "result";
            const QString recordTime           = "recordTime";
            const QString processPics          = "files";
            const QString deviceCode           = "deviceCode";
            const QString deviceType           = "deviceType";
            const QString signPicPath          = "signPic";
            const QString recordListStr        = "recordListStr";
            const QString leftEyeSightpreFix   = "20=";
            const QString link                 = "@";
            const QString rightEyeSightPreFix  = "21=";
}

class COMMONSHARED_EXPORT CHttpUpLoadEyeSightScore : public CHttpAbstractNetWork
{
public:
    CHttpUpLoadEyeSightScore( QObject *parent = nullptr );
    virtual ~CHttpUpLoadEyeSightScore() override;

    virtual void httpRequest() override;

    void setUserId( QString userId );
    void setDeviceCode( QString devCode );
    void setDeviceType( short devType );
    void setRecordTime( QString recordTime);
    void setProcessPics( QStringList paths );
    void setSignPicPath( QString signPath );
    void setBothEyeSight( QString leftEye, QString rightEeye );

    void setParaMap( QMap<QString, QVariant> infos );
private:
    virtual void initConfig() override;

    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;



public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;
};

#endif // CHTTPUPLOADEYESIGHTSCORE_H
