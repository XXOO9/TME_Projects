#ifndef CHTTPABSTRACTNETWORK_H
#define CHTTPABSTRACTNETWORK_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QUrl>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QDateTime>
#include <QMetaEnum>
#include "CHttpConfigCommonDefine.h"
#include "CHttpConfigReaderSingeton.h"

class CHttpAbstractNetWork : public QObject
{
    Q_OBJECT

public:

    Q_ENUM( errorType )

    CHttpAbstractNetWork( QObject *parent = nullptr );
    virtual ~CHttpAbstractNetWork(){}

    virtual void initConfig() = 0;
    virtual void initConnection() = 0;
    virtual void httpRequest() = 0;
    virtual bool parseJson() = 0;
    virtual bool isParameterEmpty() = 0;

    void setUrl( QString urlString );
    void setUrl( QUrl url );

    /********************参数配置相关************************/
    QString textPara() const;
    void setTextPara(const QString &textPara);

    /*******************参数转换为Json*************************/
    QByteArray jsonObjectToByteArray( QJsonObject &jsObj );
    QByteArray toJson( requestType type );
    QByteArray toUserIdentifySendJson();
    QByteArray toHeartBeatSendJson();
    QByteArray toTouristRecordSendJson();

    QMap<QString, QString> multiParaMap() const;
    void setMultiParaMap(const QMap<QString, QString> &multiParaMap);
    int multiParaSize() const;
    void appendMultiParaMap( QString key, QString value );
    void insertMultiPara( QString key, QString value );
    QString getSinglePara( QString key );

    QHttpMultiPart *pMultiPart() const;
    void setPMultiPart(QHttpMultiPart *pMultiPart);
    bool appendFilePart( QString path );
    bool appendSignPart( QString path );
    void appendTextPart( QString key, QString textValue );

    static qint64 currentTimeStamp();


    /********************请求配置相关************************/
    QNetworkRequest request() const;
    void setRequest(const QNetworkRequest &request);

    void setRequestHeader( QNetworkRequest::KnownHeaders headerName, QVariant &value );
    void setRequestRawHeader( const QByteArray &headerName, const QByteArray &value );

    requestType interfaceType();
    void setInterfaceType(requestType interfaceType);


    QByteArray recvJsonData() const;
    void setRecvJsonData(const QByteArray &recvJsonData);


    QString getErrorString() const;
    errorType getLocalErrType() const;
    QString getServerErrorString() const;
    QStringList getAddtionErrorStringList() const;


public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) = 0;
    virtual void onTimeoutHandler() = 0;

protected:
    QJsonObject jsonDataToJsonObject( QByteArray jsonData );
    QVariant takeValueOfKeyFromJsonObject( QJsonObject jsObj, QString key );


    /****************内部设置错误信息相关************************/
    void setLocalErrorString(const QString &localErrorString);
    void setErrType(const errorType &errType);
    void setServerErrorString(const QString &serverErrorString);
    void appendAddtionErrorString( QString errorString );

    /********************解析返回结果相关************************/
    bool isJson();
    bool isRequestSuccess();

    /*********************设置超时定时器**************************/
    void setTimeoutInterval( int interval );
    void startTimer( int timeInterval );
    void stopTimer();

    void setRequestFinished( bool isFinished );

private:
    void initTimer();

signals:
    void sigTimeout();
    void sigRequestRetAvailable( bool ok );

protected:
    QNetworkAccessManager                *m_pNetWorkMgr;
    QHttpMultiPart                       *m_pMultiPart;
    QNetworkReply                        *m_pReply;

    //配置请求相关
private:
    requestType                           m_interfaceType{ requestType::UNKNOWNREQUEST };
    QNetworkRequest                       m_request;
    QByteArray                            m_recvJsonData;
    QTimer                                m_timeoutTimer;
    bool                                  m_isFinished{ false };

    //请求参数相关
private:
    QString                               m_textPara;
    QMap<QString, QString>                m_multiParaMap;


private:
    QString                               m_sTextHeaderContent{ "form-data; name=\"%1\"" };
    QString                               m_sFileHeaderContent{ "form-data; name=\"%1\"; filename=\"%2\"" };

    //请求错误相关
    QString                               m_localErrorString{ NO_ERROR_STRING };
    errorType                             m_localErrType{ NO_ERR };
    QStringList                           m_addtionErrStringList;
    QString                               m_serverErrorString{ NO_ERROR_STRING };

};

#endif // CHTTPABSTRACTNETWORK_H
