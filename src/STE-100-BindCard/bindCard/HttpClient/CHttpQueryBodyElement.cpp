#include "CHttpQueryBodyElement.h"

CHttpQueryBodyElement::CHttpQueryBodyElement( QObject *parent ) : CHttpAbstractNetWork( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    initConfig();
    initConnection();
}

CHttpQueryBodyElement::~CHttpQueryBodyElement()
{

}

void CHttpQueryBodyElement::initConfig()
{
    setInterfaceType( requestType::QUERYBODYELEMENTHISTORY );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_TOURISTRECORD_KEY );
    QString urlString = baseIp + interface;
    setUrl( urlString );
}

void CHttpQueryBodyElement::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpQueryBodyElement::onNewReplyAvailable );
    connect( this, &CHttpQueryBodyElement::sigTimeout, this, &CHttpQueryBodyElement::onTimeoutHandler );
}

void CHttpQueryBodyElement::httpRequest()
{
    QByteArray sendJson = this->toJson( requestType::TOURISTRECORD );
    m_pReply = m_pNetWorkMgr->post( request(), sendJson );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpQueryBodyElement::parseJson()
{
    return true;
}

bool CHttpQueryBodyElement::isParameterEmpty()
{
    return true;
}

void CHttpQueryBodyElement::setUserId(QString userId)
{
    this->appendMultiParaMap( QUERYBODYELEMENT_KEY_USERID, userId );
}

void CHttpQueryBodyElement::setTimeInterval(queryTimeInterval timeinterval)
{
    QString strTimeInterval = QString::number( timeinterval );
    this->appendMultiParaMap( QUERYBODYELEMENT_KEY_TIMEINTERVAL, strTimeInterval );
}

void CHttpQueryBodyElement::setQueryDevType(QString devType)
{
    this->appendMultiParaMap( QUERYBODYELEMENT_KEY_TYPE, devType );
}

void CHttpQueryBodyElement::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpQueryBodyElement::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
