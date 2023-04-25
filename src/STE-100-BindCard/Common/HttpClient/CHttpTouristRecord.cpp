#include "CHttpTouristRecord.h"

CHttpTouristRecord::CHttpTouristRecord(QObject *parent) : CHttpAbstractNetWork( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    initConfig();
    initConnection();
}

CHttpTouristRecord::~CHttpTouristRecord()
{

}

void CHttpTouristRecord::initConfig()
{
    setInterfaceType( requestType::TOURISTUSAGERECORD );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_TOURISTRECORD_KEY );
    QString urlString = baseIp + interface;
    setUrl( urlString );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

void CHttpTouristRecord::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpTouristRecord::onNewReplyAvailable );
    connect( this, &CHttpTouristRecord::sigTimeout, this, &CHttpTouristRecord::onTimeoutHandler );
}

void CHttpTouristRecord::httpRequest()
{
    QByteArray sendJsonData = toJson( requestType::TOURISTRECORD );
    qDebug() << "url string = " << request().url().toString();
//    qDebug() << "post json = " << QString::fromUtf8( sendJsonData );
    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpTouristRecord::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpTouristRecord::isParameterEmpty()
{
    return true;
}

void CHttpTouristRecord::setDeviceCode(QString deviceCode)
{
    this->appendMultiParaMap( TOURISTRECORD_KEY_DEVICECODE, deviceCode );
}

void CHttpTouristRecord::setDeviceType(DEV_TYPE deviceType)
{
    QString strDevType = QString::number( deviceType );
    this->appendMultiParaMap( TOURISTRECORD_KEY_DEVICETYPE, strDevType );
}

void CHttpTouristRecord::setRecordTime(QString timeStamp)
{
    this->appendMultiParaMap( TOURISTRECORD_KEY_CREATETIME, timeStamp );
}

void CHttpTouristRecord::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
        qDebug() << "error string = " << pReply->errorString();
        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
    this->setRecvJsonData( recvData );
    bool ok = parseJson();
    emit sigRequestRetAvailable( ok );
}

void CHttpTouristRecord::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
