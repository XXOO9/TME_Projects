#ifndef CHTTPUPLOADBODYSCORE_H
#define CHTTPUPLOADBODYSCORE_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CHttpUpLoadBodyScore : public CHttpAbstractNetWork
{
public:
    CHttpUpLoadBodyScore( QObject *parent = nullptr );
    virtual ~CHttpUpLoadBodyScore() override;
    virtual void initConfig() override;
    virtual void initConnection() override;
    virtual void httpRequest() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    //4项必选用户信息
    void setUserId( QString userId );
    void setDeviceCode( QString deviceCode );
    void setDeviceType(DEV_TYPE deviceType );
    void setRecordTime( QString recordTime );

    //2项可选信息
    void setProcessFiles( QStringList paths );
    void setSignPic( QString signPath );

    //12项人体数据
    void setheight( QString height );
    void setweight( QString weight );
    void setFatRate( QString fatRate );
    void setBMI( QString BMI );
    void setHeadWeight( QString headWeight );
    void setHeadFat( QString headFat );
    void setHeadMuscle( QString headMuscle );
    void setTBMM( QString TBMM );
    void setWBIS( QString WBIS );
    void setTBW( QString TBW );
    void setWHR( QString WHR );
    void setVFA( QString VFA );

    bool constructInfoString();


public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    QString m_bodyElement;
};

#endif // CHTTPUPLOADBODYSCORE_H
