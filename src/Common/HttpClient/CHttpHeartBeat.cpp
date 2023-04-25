#include "CHttpHeartBeat.h"
#include "../Config/CDatabaseManage.h"
CHttpHeartBeat::CHttpHeartBeat(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpHeartBeat";

    initConfig();
    initConnection();
}

CHttpHeartBeat::~CHttpHeartBeat()
{

}

void CHttpHeartBeat::initConfig()
{
    setInterfaceType( requestType::HEARTBEAT );
    setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getHeartBeatFuncUrl()) );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

void CHttpHeartBeat::httpRequest()
{
    if( isParameterEmpty()){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

#if QT_NO_DEBUG
    LOGINFO( request().url().toString().insert( 0, "heart beat url = " ).toStdString() );
#else
    qDebug() << "heart beat url = " << request().url().toString();
#endif

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    QByteArray sendJsonData = toJson( requestType::HEARTBEAT );
    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimeoutTimer( 3000 );
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

void CHttpHeartBeat::setDevType(short devType)
{
    QString type = QString::number( devType, 10 );
    this->appendMultiParaMap( HEARTBEAT_KEY_TYPE, type );
}

void CHttpHeartBeat::onNewReplyAvailable(QNetworkReply *pReply)
{
    if( nullptr == m_pReply || nullptr == pReply ){
        return;
     }
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        qDebug() << "send heart beat failed, error string = " << pReply->errorString();
        //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
        if( nullptr != m_pReply){
            pReply->deleteLater();
            pReply = nullptr;
            m_pReply = nullptr;
        }
        emit sigRequestRetAvailable( false );
        return;
    }

    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
#if QT_NO_DEBUG
#else
    qDebug() << "heart beat ret = " << recvString;
#endif
    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply){
        pReply->deleteLater();
        pReply = nullptr;
        m_pReply = nullptr;
    }
    emit sigRequestRetAvailable( ok );
}

void CHttpHeartBeat::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    if( nullptr == m_pReply ){
        return;
    }
    m_pReply->abort();
}
