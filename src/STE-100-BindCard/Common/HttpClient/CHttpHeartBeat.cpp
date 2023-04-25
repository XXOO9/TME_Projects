#include "CHttpHeartBeat.h"

CHttpHeartBeat::CHttpHeartBeat(QObject *parent) : CHttpAbstractNetWork( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    initConfig();
    initConnection();
}

CHttpHeartBeat::~CHttpHeartBeat()
{

}

void CHttpHeartBeat::initConfig()
{
    setInterfaceType( requestType::HEARTBEAT );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_HEARTBEAT_KEY );
    QString urlString = baseIp + interface;
    setUrl( baseIp + interface );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

void CHttpHeartBeat::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpHeartBeat::onNewReplyAvailable );
    connect( this, &CHttpHeartBeat::sigTimeout, this, &CHttpHeartBeat::onTimeoutHandler );
}

void CHttpHeartBeat::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }
    qDebug() << "heart beat url = " << request().url().toString();
    QByteArray sendJsonData = toJson( requestType::HEARTBEAT );
    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpHeartBeat::parseJson()
{
    if( !isJson() ){
        return false;
    }
    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpHeartBeat::isParameterEmpty()
{
    if( multiParaSize() <= 0 ){
        return true;
    }
    return false;
}

void CHttpHeartBeat::setStatus(devStatus status)
{
    QString strStatus = QString::number( status );
    this->appendMultiParaMap( HEARTBEAT_KEY_STATUS, strStatus );
}

void CHttpHeartBeat::setBattery(QString battery)
{
    this->appendMultiParaMap( HEARTBEAT_KEY_BATTERY, battery );
}

void CHttpHeartBeat::setDevCode(QString devCode)
{
    this->appendMultiParaMap( HEARTBEAT_KEY_CODE, devCode );
}

void CHttpHeartBeat::setDevType(DEV_TYPE devType)
{
    QString type = QString::number( devType, 10 );
    this->appendMultiParaMap( HEARTBEAT_KEY_TYPE, type );
}

void CHttpHeartBeat::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
    this->setRecvJsonData( recvData );
    bool ok = parseJson();
    emit sigRequestRetAvailable( ok );
}

void CHttpHeartBeat::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
