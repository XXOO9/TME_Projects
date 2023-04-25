#include "CHttpUpDateUserCardNum.h"

CHttpUpDateUserCardNum::CHttpUpDateUserCardNum()
{
    m_pNetWorkMgr = new QNetworkAccessManager( this );
    initConfig();
    initConnection();
}

CHttpUpDateUserCardNum::~CHttpUpDateUserCardNum()
{

}

void CHttpUpDateUserCardNum::initConfig()
{
    setInterfaceType( UPDATEUSERBINDCARDNUMBER );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString inetrface = configReader->getValue( INTERRFACR_UPDATEUSERBINDCARDNUMBER_KEY );
    QString urlString = baseIp + inetrface;
    setUrl( urlString );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

void CHttpUpDateUserCardNum::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }
    QByteArray sendJsonData = toJson( requestType::UPDATEUSERBINDCARDNUMBER );
    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpUpDateUserCardNum::parseJson()
{
    if( !isJson() ){
        return false;
    }
    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUpDateUserCardNum::isParameterEmpty()
{
    if( multiParaSize() <= 0 ){
        return true;
    }
    return false;
}

void CHttpUpDateUserCardNum::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpUpDateUserCardNum::onNewReplyAvailable );
    connect( this, &CHttpUpDateUserCardNum::sigTimeout, this, &CHttpUpDateUserCardNum::onTimeoutHandler );
}

void CHttpUpDateUserCardNum::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpUpDateUserCardNum::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}

QString CHttpUpDateUserCardNum::userId() const
{
    return m_userId;
}

void CHttpUpDateUserCardNum::setUserId(const QString &userId)
{
    m_userId = userId;
    this->appendMultiParaMap( BINDCARD_KEY_UPDATEUSERNUMBER_USERID, userId );
}

QString CHttpUpDateUserCardNum::cardNo() const
{
    return m_cardNo;
}

void CHttpUpDateUserCardNum::setCardNo(const QString &cardNo)
{
    m_cardNo = cardNo;
    this->appendMultiParaMap( BINDCARD_KEY_UPDATEUSERNUMBER_CARDNO, m_cardNo );
}

ProjectMetaEnums::CardType CHttpUpDateUserCardNum::cardType() const
{
    return m_cardType;
}

void CHttpUpDateUserCardNum::setCardType(const ProjectMetaEnums::CardType &cardType = ProjectMetaEnums::School_Card )
{
    m_cardType = cardType;
    QString strCardType = QString::number( (int)m_cardType );
    this->appendMultiParaMap( BINDCARD_KEY_UPDATEUSERNUMBER_CARDTYPE, strCardType );
}

void CHttpUpDateUserCardNum::cleanPara()
{
    cleanMultiParaMap();
}
