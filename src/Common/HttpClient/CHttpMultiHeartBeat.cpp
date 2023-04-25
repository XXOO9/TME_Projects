#include "CHttpMultiHeartBeat.h"

#include <CDatabaseManage.h>

CHttpMultiHeartBeat::CHttpMultiHeartBeat(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpMultiHeartBeat";

    initConfig();
    initConnection();
}

CHttpMultiHeartBeat::~CHttpMultiHeartBeat()
{

}

void CHttpMultiHeartBeat::httpRequest()
{
    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    if( m_pReply != nullptr ){
        if( m_pReply->isRunning() ){
            LOGERROR( "multi heart beat is running while new request is comming" );
            return;
        }
    }

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    m_pReply = m_pNetWorkMgr->post( request(), m_heartBeatInfos );

#if QT_NO_DEBUG
#else
    qDebug() << "send multi heart beat info = " << QString::fromUtf8( m_heartBeatInfos );
#endif
    startTimeoutTimer( HEARTBEART_TIMEOUT );
}

void CHttpMultiHeartBeat::setHeartBeatInfos(const QVariantList &heartBeatInfos)
{
    m_heartBeatInfos.clear();
    QJsonArray jsArr = QJsonArray::fromVariantList( heartBeatInfos );
    QJsonObject jsObj;
    QJsonDocument jsDoc;
    jsDoc.setArray( jsArr );

    m_heartBeatInfos = jsDoc.toJson();


#if QT_NO_DEBUG
#else
    qDebug() << "multi heartBeat js info = " << m_heartBeatInfos;
#endif
}

void CHttpMultiHeartBeat::onNewReplyAvailable(QNetworkReply *pReply)
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
    qDebug() << "multi heart recv str = " << recvString;
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

void CHttpMultiHeartBeat::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    if( nullptr == m_pReply ){
        return;
    }
    m_pReply->abort();
}

void CHttpMultiHeartBeat::initConfig()
{
    setInterfaceType( requestType::MULTIHEARTBEAT );
    setUrl( QString::fromStdString( CDatabaseManage::GetInstance().getServerFunConfig().getStrMultiHeartBeatUrl() ));
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

bool CHttpMultiHeartBeat::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    return true;
}

bool CHttpMultiHeartBeat::isParameterEmpty()
{
    return true;
}

bool CHttpMultiHeartBeat::isRequestSuccess()
{
    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QString requestRet = takeValueOfKeyFromJsonObject( jsObj, REQUEST_RESULT_KEY ).toString();
    setRequestRetCode( requestRet.toInt() );
    if( requestRet != REQUEST_RESULT_SUCCESS ){
        setErrType( errorType::REQUEST_ERROR, "failed..." );
        return false;
    }

    QJsonObject dataObj = jsObj.take( REQUEST_VALUE ).toObject();
    m_lastSyncTime = dataObj.value( HEARTBEAT_KEY_LASTSYNCTIME ).toVariant().toString();
    m_currentTime  = dataObj.value( HEARTBEAT_KEY_CURRENTTIME ).toVariant().toString();
    m_schoolId     = dataObj.value( "schoolId" ).toVariant().toString();

    return true;
}

QString CHttpMultiHeartBeat::schoolId() const
{
    return m_schoolId;
}

QString CHttpMultiHeartBeat::currentTime() const
{
    return m_currentTime;
}

QString CHttpMultiHeartBeat::lastSyncTime() const
{
    return m_lastSyncTime;
}
