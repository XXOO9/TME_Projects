#include "CHttpUserLogin.h"

CHttpUserLogin::CHttpUserLogin()
{
    m_pNetWorkMgr = new QNetworkAccessManager( this );
    initConfig();
    initConnection();
}

void CHttpUserLogin::initConfig()
{
    setInterfaceType( USERLOGIN );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString inetrface = configReader->getValue( INTERFACE_USERLOGIN_KEY );
    QString urlString = baseIp + inetrface;
    qDebug() << "log in url = " << urlString;
    setUrl( urlString );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

void CHttpUserLogin::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    qDebug() << "url = " << request().url().toString();
    QByteArray sendJsonData = toJson( requestType::USERLOGIN );
    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpUserLogin::parseJson()
{
    if( !isJson() ){
        return false;
    }
    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUserLogin::isParameterEmpty()
{
    if( multiParaSize() <= 0 ){
        return true;
    }
    return false;
}

void CHttpUserLogin::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpUserLogin::onNewReplyAvailable );
    connect( this, &CHttpUserLogin::sigTimeout, this, &CHttpUserLogin::onTimeoutHandler );
}

void CHttpUserLogin::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpUserLogin::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}

QString CHttpUserLogin::loginPasswd() const
{
    return m_loginPasswd;
}

void CHttpUserLogin::setLoginPasswd(const QString &loginPasswd)
{
    m_loginPasswd = loginPasswd;
    this->appendMultiParaMap( LOGIN_PASSWD_KEY, m_loginPasswd );
}

void CHttpUserLogin::abortRequest()
{
    if( nullptr != m_pReply ){
        m_pReply->abort();
    }
}

QString CHttpUserLogin::userAccount() const
{
    return m_userAccount;
}

void CHttpUserLogin::setUserAccount(const QString &userAccount)
{
    m_userAccount = userAccount;
    this->appendMultiParaMap( LOGIN_ACCOUNT_KEY, m_userAccount );
}
