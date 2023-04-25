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
#include <QJsonValue>
#include <QMap>
#include <QUrl>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QElapsedTimer>
#include <QNetworkProxy>
#include <QDateTime>
#include <QMetaEnum>
#include "CHttpConfigCommonDefine.h"
#include "../common_global.h"
#include "../Logger/CLogger.h"

namespace HTTP_KEYS {
          const QString userId        =     "userId";
          const QString devCode       =     "deviceCode";
          const QString devType       =     "deviceType";
          const QString type          =     "type";
          const QString testItem      =     "testItem";
          const QString result        =     "result";
          const QString files         =     "files";
          const QString signPic       =     "signPic";
          const QString recordListStr =     "recordListStr";
          const QString faceFeature   =     "faceFeature";
          const QString cardNo        =     "cardNo";
          const QString name          =     "name";
          const QString year          =     "year";
          const QString clazzId       =     "clazzId";
          const QString role          =     "role";
          const QString code          =     "code";
          const QString gender        =     "gender";
          const QString birthday      =     "birthday";
          const QString nation        =     "nation";
          const QString description   =     "description";
          const QString avatarUrl     =     "avatarUrl";
          const QString updateTime    =     "updateTime";
          const QString recordTime    =     "recordTime";
          const QString leftEyeSight  =     "leftEyeSight";
          const QString rightEyeSight =     "rightEyeSight";
          const QString errorIndex    =     "index";
          const QString errorCode     =     "code";
          const QString errorMsg      =     "msg";
}

//定义服务器中的无效成绩
#define INVALID_RESULT_SERVER "-50"

class COMMONSHARED_EXPORT CHttpAbstractNetWork : public QObject
{
    Q_OBJECT

public:
    CHttpAbstractNetWork( QObject *parent = nullptr );
    virtual ~CHttpAbstractNetWork();

    QString getErrorString() const;
    errorType getLocalErrType() const;
    QString getServerErrorString() const;
    QStringList getAddtionErrorStringList() const;

    static qint64 currentTimeStamp();

    //时间戳转换函数
    static QString timeStampToDateTime( qint64 timeStamp = currentTimeStamp(), bool containsTime = true );

    //纯数字格式的日期转为string  例如: 20221021153826  ->   2022-10-21 15:38:26
    static QString plainNumberTimeFormat2String( const QString &plainNumberTime );

    //请求错误码相关
    void setErrorCode(const QString &errorCode);
    QString getErrorCode() const;

    /*******************控制请求相关*************************/
    virtual void httpRequest() = 0;

    //丢弃此次请求
    void abortRequest();

    //判断请求是否已经完成
    bool reuqestRunning() const;

    int getRequestRetCode() const;
    void setRequestRetCode(int requestRetCode);



    QNetworkReply *getPReply() const;
    QUrl getUrl() const;

    //设置服务器token值
    void setServerTokenString(const QString &serverTokenString);
    QString getServerTokenString() const;

protected:

    void setUrl( QString urlString );
    void setUrl( QUrl url );

    Q_ENUM( errorType )

    virtual void initConfig() = 0;
    virtual void initConnection();
    virtual bool parseJson() = 0;
    virtual bool isParameterEmpty() = 0;



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
    bool appendFilePart( QString path, QString key = "" );
    bool appendSignPart( QString path, QString key = "" );
    void appendTextPart( QString key, QString textValue );




    /********************请求配置相关************************/
    QNetworkRequest request() const;
    void setRequest(const QNetworkRequest &request);

    void setRequestHeader( QNetworkRequest::KnownHeaders headerName, QVariant &value );
    void setRequestRawHeader( const QByteArray &headerName, const QByteArray &value );

    requestType interfaceType();
    void setInterfaceType(requestType interfaceType);


    inline const QByteArray &recvJsonData() const {return m_recvJsonData;}
    void setRecvJsonData(const QByteArray &recvJsonData);

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) = 0;
    virtual void onTimeoutHandler() = 0;

protected:
    QJsonObject jsonDataToJsonObject( const QByteArray &jsonData );
    inline QVariant takeValueOfKeyFromJsonObject( QJsonObject &jsObj, const QString &key );

    /****************内部设置错误信息相关************************/
    void setLocalErrorString(const QString &localErrorString);
    void setErrType(const errorType &errType, QString errorString = "" );
    void setServerErrorString(const QString &serverErrorString);
    void appendAddtionErrorString( QString errorString );

    /********************解析返回结果相关************************/
    bool isJson();
    virtual bool isRequestSuccess();

    /*********************设置超时定时器**************************/
    void setTimeoutInterval( int interval );
    void startTimeoutTimer( int timeInterval );
    void stopTimer();

    void setRequestFinished( bool isFinished );

    //清空服务器上传参数
    void clearParamMap();


    //解析参数, 目前主要用在批量提交处
    virtual void parseParam( DATA_SRC dataSrc = UserExam );



private:
    void initTimer();

signals:
    void sigTimeout();
    void sigRequestRetAvailable( bool ok );

protected:
    QNetworkAccessManager                *m_pNetWorkMgr{nullptr};
    QHttpMultiPart                       *m_pMultiPart{nullptr};
    QNetworkReply                        *m_pReply{nullptr};

    //由于继承虚函数在基类虚函数中不能获取子类函数名称进行日志打印，故在此添加变量名称用于子类中变更
    string                               m_strNetWorkName{"CHttpAbstractNetWork"};
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
    QString                               m_errorCode{""};

    //启用自组网后，该变量用于存储每个学校对应的服务器地址,  相应CommonDefine里面的Base IP则对应根服务器地址
    QString                               m_specificServerUrl = "";

    //请求结果码
    int                                   m_requestRetCode = 0;
    QMutex                                m_requestRetCodeMutex;


    QString                               m_serverTokenString = "";                         
};

#endif // CHTTPABSTRACTNETWORK_H
