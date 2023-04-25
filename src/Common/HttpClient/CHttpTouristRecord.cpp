#include "CHttpTouristRecord.h"
#include "../Config/CDatabaseManage.h"
CHttpTouristRecord::CHttpTouristRecord(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpTouristRecord";

    initConfig();
    initConnection();
}

CHttpTouristRecord::~CHttpTouristRecord()
{

}

void CHttpTouristRecord::httpRequest()
{
    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    QByteArray sendJsonData = toJson( requestType::TOURISTRECORD );
    qDebug() << "url string = " << request().url().toString();

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimeoutTimer( TIMEOUT_SHORT );
}

void CHttpTouristRecord::initConfig()
{
    setInterfaceType( requestType::TOURISTUSAGERECORD );
    setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getUpdateTouristUsageRecordFuncUrl()));
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

bool CHttpTouristRecord::parseJson()
{
    if( !isJson() ){
        this->setRequestRetCode( UNKNOWNERROR );
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

void CHttpTouristRecord::setDeviceType(short deviceType)
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

        //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
        if( nullptr != m_pReply){
            pReply->deleteLater();
            pReply = nullptr;
            m_pReply = nullptr;
        }

        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
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

void CHttpTouristRecord::onTimeoutHandler()
{
    if( nullptr == m_pReply ){
        return;
    }

    if( m_pReply->isFinished() ){
        return;
    }

    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
