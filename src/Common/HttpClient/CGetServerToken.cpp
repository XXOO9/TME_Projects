#include "CGetServerToken.h"
#include "../Config/CDatabaseManage.h"

CGetServerToken::CGetServerToken( CHttpAbstractNetWork *parent ) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CGetServerToken";

    initConfig();
    initGetServerTokenTimer();
    initConnection();
}

CGetServerToken::~CGetServerToken()
{

}

void CGetServerToken::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    parseParam();

    m_pReply = m_pNetWorkMgr->post( request(), m_sendData );

    startTimeoutTimer( 3000 );
#if QT_NO_DEBUG
#else
    qDebug() << "get token url = " << request().url().toString();
#endif
}

void CGetServerToken::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();

    //接收到获取token的返回后，立马开启获取token的定时器
    m_getServerTokenTimer.start();

    if( nullptr == pReply ){
        return;
    }

    if( QNetworkReply::NoError != pReply->error() ){
        handleRequestError( pReply );
        return;
    }

    QByteArray recvJsonByteArray = pReply->readAll();
    setRecvJsonData( recvJsonByteArray );
    bool ok = parseJson();

    pReply->deleteLater();
    pReply = nullptr;
    m_pReply = nullptr;
#if QT_NO_DEBUG
    QString logInfo = "get server token,recv json string = " + QString::fromUtf8( recvJsonByteArray );
    LOGINFO( logInfo.toStdString() );
#else
    qDebug() << "get server token,recv json string = " << QString::fromUtf8( recvJsonByteArray );
#endif

    emit sigRequestRetAvailable( ok );
}

void CGetServerToken::onTimeoutHandler()
{
    if( m_pReply != nullptr ){
        if( m_pReply->isRunning() ){
            m_pReply->abort();
        }
    }else{
        m_getServerTokenTimer.start();
    }

    setErrType( errorType::TIMEOUT_ERROR );
}

void CGetServerToken::onGetServerTokenTimerTimeout()
{
    //触发后，立即停止定时器,在接收到请求返回后，再开启
    m_getServerTokenTimer.stop();
    httpRequest();
}

void CGetServerToken::initConfig()
{
    //临时写在这里，实际使用根据情况动态获取
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
    setUrl( QString::fromStdString( CDatabaseManage::GetInstance().getServerFunConfig().getLoginUrl() ));
}

bool CGetServerToken::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    return true;
}

bool CGetServerToken::isParameterEmpty()
{
    return false;
}

bool CGetServerToken::isRequestSuccess()
{
    QJsonDocument jsDoc = QJsonDocument::fromJson( recvJsonData() );
    QJsonObject tmpJson = jsDoc.object();

    if( !tmpJson.contains( REQUEST_RESULT_KEY ) ){
        return false;
    }

    //这个地方跟体测的成功标志不一样， 体测是 0
    const int successCode = 0000;

    //开始解析报告信息
    int requestRetCode = tmpJson.value( REQUEST_RESULT_KEY ).toVariant().toInt();
    if( successCode !=  requestRetCode ){
        setErrorCode( QString::number( requestRetCode ) );
        return false;
    }

    m_token = tmpJson.value( "data" ).toObject().take( getServerTokenKeys::token ).toString();
    m_tokenHead = tmpJson.value( "data" ).toObject().take( getServerTokenKeys::tokenHead ).toString();

    return true;
}

void CGetServerToken::parseParam(DATA_SRC dataSrc)
{
    m_sendData = jsonObjectToByteArray( QJsonObject::fromVariantMap( m_loginParaMap ) );


#if QT_NO_DEBUG
#else
    qDebug() <<  "CGetServerToken::parseParam() :: login info = " << QString::fromLocal8Bit( m_sendData );
#endif
}

void CGetServerToken::handleRequestError(QNetworkReply *pReply)
{
#if QT_NO_DEBUG
#else
    qDebug() << "request error string = " << pReply->errorString();
#endif

    pReply->deleteLater();
    pReply = nullptr;
    m_pReply = nullptr;
    emit sigRequestRetAvailable( false );
}

void CGetServerToken::initGetServerTokenTimer()
{
    m_getServerTokenTimer.setInterval( 30 * 1000 );
    connect( &m_getServerTokenTimer, &QTimer::timeout, this, &CGetServerToken::onGetServerTokenTimerTimeout, Qt::QueuedConnection );
}

QString CGetServerToken::token() const
{
    return m_token;
}

void CGetServerToken::setToken(const QString &token)
{
    m_token = token;
}

void CGetServerToken::setLoginParaMap(const QVariantMap &loginParaMap)
{
    m_loginParaMap = loginParaMap;
    
    //更新登录信息后，先停止定时器,然后立马发出请求
    if( m_getServerTokenTimer.isActive() ){
        m_getServerTokenTimer.stop();
    }

    httpRequest();

}
