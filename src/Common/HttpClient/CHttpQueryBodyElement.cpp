#include "CHttpQueryBodyElement.h"
#include "../Config/CDatabaseManage.h"

CHttpQueryBodyElement::CHttpQueryBodyElement( QObject *parent ) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpQueryBodyElement";

    initConfig();
    initConnection();
}

CHttpQueryBodyElement::~CHttpQueryBodyElement()
{

}

void CHttpQueryBodyElement::initConfig()
{
    setInterfaceType( requestType::QUERYBODYELEMENTHISTORY );
    //暂时未使用
    /*setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig()()));*/
}

void CHttpQueryBodyElement::httpRequest()
{
    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    QByteArray sendJson = this->toJson( requestType::TOURISTRECORD );
    m_pReply = m_pNetWorkMgr->post( request(), sendJson );
    startTimeoutTimer( TIMEOUT_SHORT );
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
    if( nullptr == m_pReply || nullptr == pReply ){
        return;
    }

    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
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

void CHttpQueryBodyElement::onTimeoutHandler()
{
    if( nullptr == m_pReply ){
        return;
    }
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
