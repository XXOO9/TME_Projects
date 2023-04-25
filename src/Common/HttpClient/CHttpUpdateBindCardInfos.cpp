#include "CHttpUpdateBindCardInfos.h"

#include "../Config/CDatabaseManage.h"

CHttpUpdateBindCardInfos::CHttpUpdateBindCardInfos()
{
    m_strNetWorkName = "CHttpUpdateBindCardInfos";

    initConfig();
    initConnection();
}

CHttpUpdateBindCardInfos::~CHttpUpdateBindCardInfos()
{

}

void CHttpUpdateBindCardInfos::httpRequest()
{
    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    m_pReply = m_pNetWorkMgr->post( request(), m_postData );
    startTimeoutTimer( 3000 );
}

void CHttpUpdateBindCardInfos::setBindInfos(const QString &userId, const QString &cardNumber)
{
    QVariantMap tmpVariant = {
        { "userId", userId },
        { "cardNo", cardNumber },
        { "cardType", "0" }
                             };

    QJsonObject tmpJsonObject = QJsonObject::fromVariantMap( tmpVariant );
    QJsonDocument tmpJsDoc;

    tmpJsDoc.setObject( tmpJsonObject );

    m_postData = tmpJsDoc.toJson();
}

void CHttpUpdateBindCardInfos::initConfig()
{
    setInterfaceType( requestType::HEARTBEAT );
    setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getUpdateCardInfoUrl() ) );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

bool CHttpUpdateBindCardInfos::parseJson()
{
    return true;
}

bool CHttpUpdateBindCardInfos::isParameterEmpty()
{
    return false;
}

void CHttpUpdateBindCardInfos::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpUpdateBindCardInfos::onTimeoutHandler()
{
    if( m_pReply->isRunning() ){
        m_pReply->abort();
    }
}
