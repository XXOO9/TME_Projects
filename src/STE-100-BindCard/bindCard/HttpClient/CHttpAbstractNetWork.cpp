#include "CHttpAbstractNetWork.h"

CHttpAbstractNetWork::CHttpAbstractNetWork(QObject *parent) :QObject ( parent ),
    m_pNetWorkMgr( nullptr ),
    m_pMultiPart( nullptr )
{
    m_pNetWorkMgr = new QNetworkAccessManager( this );
    initTimer();
}


void CHttpAbstractNetWork::setUrl(QString urlString)
{
    QUrl url = QUrl( urlString );
    m_request.setUrl( url );
}

void CHttpAbstractNetWork::setUrl(QUrl url)
{
    m_request.setUrl( url );
}


QJsonObject CHttpAbstractNetWork::jsonDataToJsonObject(QByteArray jsonData)
{
    QJsonParseError error;
    QJsonObject jsObj;
    QJsonDocument jsDoc = QJsonDocument::fromJson( jsonData, &error );
    if( error.error != QJsonParseError::NoError )
    {
        QString errString = error.errorString();
        appendAddtionErrorString( errString );
        return jsObj;
    }
    jsObj = jsDoc.object();
    return jsObj;
}

QVariant CHttpAbstractNetWork::takeValueOfKeyFromJsonObject(QJsonObject jsObj, QString key)
{
    if( jsObj.isEmpty() )
    {
        qDebug() << "empty jsObj    " << __FUNCTION__ << __LINE__;
        QString errString = "error while taking value of json object, the json object is empty...";
        appendAddtionErrorString( errString );
        return QVariant();
    }
    if( jsObj.contains( key ) )
    {
        return jsObj.take( key ).toVariant();
    }
    QString errString = QString("error while taking value of json object, there is no key named:: %1 in json object").arg( key );
    appendAddtionErrorString( errString );
    qDebug() << "no this key::" + key << __FUNCTION__ << __LINE__;
    return QVariant();
}

bool CHttpAbstractNetWork::isJson()
{
    QJsonObject jsObj = jsonDataToJsonObject( m_recvJsonData );
    if( jsObj.size() <= 0 ){
        setErrType( errorType::PARSE_JSON_FORMAT_ERR );
        return false;
    }
    return true;
}

bool CHttpAbstractNetWork::isRequestSuccess()
{
    QJsonObject jsObj = jsonDataToJsonObject( m_recvJsonData );
    QString requestRet = takeValueOfKeyFromJsonObject( jsObj, REQUEST_RESULT_KEY ).toString();
    if( requestRet != REQUEST_RESULT_SUCCESS ){
        QString errorMsg = takeValueOfKeyFromJsonObject( jsObj, REQUEST_MESSAGE ).toString();
        setErrType( errorType::REQUEST_ERROR );
        setServerErrorString( errorMsg );
        return false;
    }
    return true;
}

void CHttpAbstractNetWork::setTimeoutInterval(int interval)
{
    m_timeoutTimer.setInterval( interval );
}

void CHttpAbstractNetWork::startTimer(int timeInterval)
{
    m_isFinished = false;
    if( !m_timeoutTimer.isActive() ){
        m_timeoutTimer.start( timeInterval );
    }
}

void CHttpAbstractNetWork::stopTimer()
{
    m_isFinished = true;
    if( m_timeoutTimer.isActive() ){
        m_timeoutTimer.stop();
    }

}

void CHttpAbstractNetWork::setRequestFinished(bool isFinished)
{
    m_isFinished = isFinished;
}

void CHttpAbstractNetWork::initTimer()
{
    m_timeoutTimer.setSingleShot( true );
    connect( &m_timeoutTimer, &QTimer::timeout, [=](){
        if( !m_isFinished ){
            setErrType( errorType::TIMEOUT_ERROR );
            emit sigTimeout();
        }
    });
}

QString CHttpAbstractNetWork::getServerErrorString() const
{
    return m_serverErrorString;
}

QStringList CHttpAbstractNetWork::getAddtionErrorStringList() const
{
    return m_addtionErrStringList;
}

void CHttpAbstractNetWork::setServerErrorString(const QString &serverErrorString)
{
    if( NO_ERROR_STRING == m_serverErrorString ){
        QMetaEnum metaEnum = QMetaEnum::fromType<errorType>();
        QString errorTypeString = metaEnum.valueToKey( m_localErrType );
        m_serverErrorString = errorTypeString + "::" + serverErrorString;
    }
}

void CHttpAbstractNetWork::appendAddtionErrorString(QString errorString)
{
    m_addtionErrStringList << errorString;
}

errorType CHttpAbstractNetWork::getLocalErrType() const
{
    return m_localErrType;
}

void CHttpAbstractNetWork::setErrType(const errorType &errType)
{
    if( NO_ERR != m_localErrType ){
        return;
    }
    m_localErrType = errType;
    switch ( errType ) {
    case errorType::PARSE_JSON_FORMAT_ERR:
        setLocalErrorString( "error while parsing json string..." );
        break;
    case errorType::TIMEOUT_ERROR:
        setLocalErrorString( "http request timeout, operation canceled..." );
    default:
        break;
    }
}

QString CHttpAbstractNetWork::getErrorString() const
{
    QString requestType = QString::number( m_interfaceType );
    QString errorTypeString = QString::number( m_localErrType );
    if( NO_ERROR_STRING != m_localErrorString ){
        return requestType + errorTypeString + "::" + m_localErrorString;
    }

    if( NO_ERROR_STRING != m_serverErrorString ){
        return requestType + errorTypeString + "::" + m_serverErrorString;
    }
    return QString();
}

void CHttpAbstractNetWork::setLocalErrorString(const QString &localErrorString)
{
    //只有第一次能设置
    if( NO_ERROR_STRING != m_localErrorString ){
        return;
    }
    m_localErrorString = localErrorString;
}

QMap<QString, QString> CHttpAbstractNetWork::multiParaMap() const
{
    return m_multiParaMap;
}

void CHttpAbstractNetWork::setMultiParaMap(const QMap<QString, QString> &jsonParaMap)
{
    m_multiParaMap = jsonParaMap;
}

int CHttpAbstractNetWork::multiParaSize() const
{
    return m_multiParaMap.size();
}

void CHttpAbstractNetWork::appendMultiParaMap(QString key, QString value)
{
    m_multiParaMap.insert( key, value );
}

QByteArray CHttpAbstractNetWork::jsonObjectToByteArray(QJsonObject &jsObj)
{
    if( jsObj.isEmpty() ){
        return QByteArray();
    }
    QJsonDocument jsDoc;
    jsDoc.setObject( jsObj );
    QByteArray jsonData = jsDoc.toJson( QJsonDocument::Compact );
    return jsonData;
}

void CHttpAbstractNetWork::insertMultiPara(QString key, QString value)
{
    m_multiParaMap.insert( key, value );
}

QString CHttpAbstractNetWork::getSinglePara(QString key)
{
    if( m_multiParaMap.contains( key ) ){
        QString ret = m_multiParaMap.value( key );
        return ret;
    }
    return QString();
}

QByteArray CHttpAbstractNetWork::recvJsonData() const
{
    return m_recvJsonData;
}

void CHttpAbstractNetWork::setRecvJsonData(const QByteArray &recvJsonData)
{
    m_recvJsonData = recvJsonData;
}

requestType CHttpAbstractNetWork::interfaceType()
{
    return m_interfaceType;
}

void CHttpAbstractNetWork::setInterfaceType(requestType interfaceType)
{
    m_interfaceType = interfaceType;
}

QNetworkRequest CHttpAbstractNetWork::request() const
{
    return m_request;
}

void CHttpAbstractNetWork::setRequest(const QNetworkRequest &request)
{
    m_request = request;
}

void CHttpAbstractNetWork::setRequestHeader(QNetworkRequest::KnownHeaders headerName, QVariant &value)
{
    m_request.setHeader( headerName, value );
}

void CHttpAbstractNetWork::setRequestRawHeader(const QByteArray &headerName, const QByteArray &value)
{
    m_request.setRawHeader( headerName, value );
}

QHttpMultiPart *CHttpAbstractNetWork::pMultiPart() const
{
    return m_pMultiPart;
}

void CHttpAbstractNetWork::setPMultiPart(QHttpMultiPart *pMultiPart)
{
    m_pMultiPart = pMultiPart;
}

bool CHttpAbstractNetWork::appendFilePart(QString path)
{
    if( !QFileInfo::exists( path ) ){
        qDebug() << "path is invaild...";
        return false;
    }

    if( nullptr == m_pMultiPart ){
        m_pMultiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType, this );
    }

    QFile *file = new QFile( path );
    if( !file->open( QIODevice::ReadOnly )){
        qDebug() << "open file failed, named -> " << path;
        delete file;
        file = nullptr;
        return false;
    }

    QHttpPart part;
    QString headerValue = m_sFileHeaderContent.arg( UPLOADSOCRE_FILES_KEY ).arg( path );
    part.setHeader( QNetworkRequest::ContentDispositionHeader, headerValue );
    part.setBodyDevice( file );
    m_pMultiPart->append( part );
    file->setParent( m_pMultiPart );
    return true;
}

bool CHttpAbstractNetWork::appendSignPart(QString path)
{
    if( !QFileInfo::exists( path ) ){
        qDebug() << "path is invaild...";
        return false;
    }

    if( nullptr == m_pMultiPart ){
        m_pMultiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType, this );
    }

    QFile *file = new QFile( path );
    if( !file->open( QIODevice::ReadOnly )){
        qDebug() << "open file failed, named -> " << path;
        delete file;
        file = nullptr;
        return false;
    }

    QHttpPart part;
    QString headerValue = m_sFileHeaderContent.arg( UPLOADSOCRE_SIGN_KEY ).arg( path );
    part.setHeader( QNetworkRequest::ContentDispositionHeader, headerValue );
    part.setBodyDevice( file );
    m_pMultiPart->append( part );
    file->setParent( m_pMultiPart );
    return true;
}

void CHttpAbstractNetWork::appendTextPart(QString key, QString textValue)
{
    if( nullptr == m_pMultiPart ){
        m_pMultiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType, this );
    }
    QHttpPart part;
    QString headerValue = m_sTextHeaderContent.arg( key );
    part.setHeader( QNetworkRequest::ContentDispositionHeader, headerValue );
    part.setBody( textValue.toLocal8Bit() );

    m_pMultiPart->append( part );
}

qint64 CHttpAbstractNetWork::currentTimeStamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

QByteArray CHttpAbstractNetWork::toJson(requestType type)
{
    if( m_multiParaMap.isEmpty() ){
        return QByteArray();
    }

    QByteArray jsonData;
    switch ( type ) {
    case USERIDENTIFY:
        jsonData = toUserIdentifySendJson();
        return jsonData;
    case HEARTBEAT:
        jsonData = toHeartBeatSendJson();
        return jsonData;
    case TOURISTRECORD:
        jsonData = toTouristRecordSendJson();
        return jsonData;
    default:
        break;
    }
    return QByteArray();
}

QByteArray CHttpAbstractNetWork::toUserIdentifySendJson()
{
    QString faceFeature = m_multiParaMap.value( USERIDENTIFY_FACEFEATURE_KEY );
    QString cardNo = m_multiParaMap.value( USERIDENTIFY_CARDNO_KEY );
    QJsonObject jsObj;
    jsObj.insert( USERIDENTIFY_FACEFEATURE_KEY, faceFeature );
    jsObj.insert( USERIDENTIFY_CARDNO_KEY, cardNo );

    QByteArray sendJson = jsonObjectToByteArray( jsObj );
    return sendJson;
}

QByteArray CHttpAbstractNetWork::toHeartBeatSendJson()
{
    if( !m_multiParaMap.contains( HEARTBEAT_KEY_STATUS ) ){
        return QByteArray();
    }
    QJsonObject jsObj;
    QString status  = m_multiParaMap.value( HEARTBEAT_KEY_STATUS );
    jsObj.insert( HEARTBEAT_KEY_STATUS, status );
    QString battery;
    QString code;
    QString type;

    if( m_multiParaMap.contains( HEARTBEAT_KEY_CODE ) ){
        code = m_multiParaMap.value( HEARTBEAT_KEY_CODE );
        jsObj.insert( HEARTBEAT_KEY_CODE, code );
    }
    if( m_multiParaMap.contains( HEARTBEAT_KEY_BATTERY ) ){
        battery = m_multiParaMap.value( HEARTBEAT_KEY_BATTERY );
        jsObj.insert( HEARTBEAT_KEY_BATTERY, battery );
    }
    if( m_multiParaMap.contains( HEARTBEAT_KEY_TYPE ) ){
        type = m_multiParaMap.value( HEARTBEAT_KEY_TYPE );
        jsObj.insert( HEARTBEAT_KEY_TYPE, type );
    }

    QByteArray sendJson = jsonObjectToByteArray( jsObj );
    return sendJson;
}

QByteArray CHttpAbstractNetWork::toTouristRecordSendJson()
{
    if( !m_multiParaMap.contains( TOURISTRECORD_KEY_DEVICECODE ) ){
        return QByteArray();
    }

    if( !m_multiParaMap.contains( TOURISTRECORD_KEY_DEVICETYPE ) ){
        return QByteArray();
    }

    if( !m_multiParaMap.contains( TOURISTRECORD_KEY_CREATETIME ) ){
        return QByteArray();
    }

    QString deviceCode = getSinglePara( TOURISTRECORD_KEY_DEVICECODE );
    QString deviceType = getSinglePara( TOURISTRECORD_KEY_DEVICETYPE );
    QString timeStamp = getSinglePara( TOURISTRECORD_KEY_CREATETIME );

    QJsonObject jsObj;
    jsObj.insert( TOURISTRECORD_KEY_DEVICECODE, deviceCode );
    jsObj.insert( TOURISTRECORD_KEY_DEVICETYPE, deviceType );
    jsObj.insert( TOURISTRECORD_KEY_CREATETIME, timeStamp );

    QByteArray sendJsonData = jsonObjectToByteArray( jsObj );
    return sendJsonData;
}

QString CHttpAbstractNetWork::textPara() const
{
    return m_textPara;
}

void CHttpAbstractNetWork::setTextPara(const QString &textPara)
{
    if( textPara.isEmpty() ){
        return;
    }
    m_textPara = textPara;
}
