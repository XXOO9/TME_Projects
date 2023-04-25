#include "CHttpDisBindUserCardNumber.h"

CHttpDisBindUserCardNumber::CHttpDisBindUserCardNumber()
{
    m_pNetWorkMgr = new QNetworkAccessManager( this );
    initConfig();
    initConnection();
}

CHttpDisBindUserCardNumber::~CHttpDisBindUserCardNumber()
{

}

void CHttpDisBindUserCardNumber::initConfig()
{
    setInterfaceType( DISBINDUSERCARDNUMBER );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString inetrface = configReader->getValue( INTERRFACR_DISBINDCARDNUMBER_KEY );
    QString urlString = baseIp + inetrface;
    setUrl( urlString );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

void CHttpDisBindUserCardNumber::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }
    QByteArray sendJsonData = toJson( requestType::DISBINDUSERCARDNUMBER );
    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpDisBindUserCardNumber::parseJson()
{
    if( !isJson() ){
        return false;
    }
    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpDisBindUserCardNumber::isParameterEmpty()
{
    if( multiParaSize() <= 0 ){
        return true;
    }
    return false;
}

void CHttpDisBindUserCardNumber::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpDisBindUserCardNumber::onNewReplyAvailable );
    connect( this, &CHttpDisBindUserCardNumber::sigTimeout, this, &CHttpDisBindUserCardNumber::onTimeoutHandler );
}

void CHttpDisBindUserCardNumber::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpDisBindUserCardNumber::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}

QString CHttpDisBindUserCardNumber::userId() const
{
    return m_userId;
}

void CHttpDisBindUserCardNumber::setUserId(const QString &userId)
{
    m_userId = userId;
    this->appendMultiParaMap( BINDCARD_KEY_DISBINDCARDNUMBER_USERID, userId );
}
