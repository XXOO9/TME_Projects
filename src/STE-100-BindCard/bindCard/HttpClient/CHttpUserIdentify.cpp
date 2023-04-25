#include "CHttpUserIdentify.h"

CHttpUserIdentify::CHttpUserIdentify( QString faceFeature, QString cardNo, QObject *parent ) : CHttpAbstractNetWork ( parent )
{
    if( !faceFeature.isEmpty() ){
        insertMultiPara( userIdentifyParaKey::faceFeature, faceFeature );
    }

    if( !cardNo.isEmpty() ){
        insertMultiPara( userIdentifyParaKey::cardNo, cardNo );
    }

    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    initConfig();
    initConnection();
}

CHttpUserIdentify::~CHttpUserIdentify()
{

}

void CHttpUserIdentify::initConfig()
{
    setInterfaceType( requestType::USERIDENTIFY );
    CHttpConfigReaderSingeton *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_USERIDENTIFY_KEY );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
    setUrl( baseIp + interface );
}

void CHttpUserIdentify::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpUserIdentify::onNewReplyAvailable );
    connect( this, &CHttpUserIdentify::sigTimeout, this, &CHttpUserIdentify::onTimeoutHandler );
}

void CHttpUserIdentify::httpRequest()
{
    QByteArray sendJsonData = this->toUserIdentifySendJson();
    if( sendJsonData.isEmpty() ){
        return;
    }

    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpUserIdentify::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QJsonObject dataObj = takeValueOfKeyFromJsonObject( jsObj, REQUEST_VALUE ).toJsonObject();
    QMap< QString, QString > retMap;
    QString ret = takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::id).toString();
    retMap.insert( userIdentifyKeys::id, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::id).toString() );
    retMap.insert( userIdentifyKeys::year, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::year).toString() );
    retMap.insert( userIdentifyKeys::clazzId, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::clazzId).toString() );
    retMap.insert( userIdentifyKeys::role, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::role).toString() );
    retMap.insert( userIdentifyKeys::code, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::code).toString() );
    retMap.insert( userIdentifyKeys::cardNo, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::cardNo).toString() );
    retMap.insert( userIdentifyKeys::name, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::name).toString() );
    retMap.insert( userIdentifyKeys::gender, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::gender).toString() );
    retMap.insert( userIdentifyKeys::nation, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::nation).toString() );
    retMap.insert( userIdentifyKeys::faceFeature, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::faceFeature).toString() );
    retMap.insert( userIdentifyKeys::description, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::description).toString() );
    retMap.insert( userIdentifyKeys::avatarUrl, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::avatarUrl).toString() );
    retMap.insert( userIdentifyKeys::updateTime, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::updateTime).toString() );
    retMap.insert( userIdentifyKeys::birthday, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::birthday).toString() );

    m_ret.appendRet( retMap );
    return true;
}

bool CHttpUserIdentify::isParameterEmpty()
{
    return true;
}

int CHttpUserIdentify::userInfoCount()
{
    return m_ret.size();
}

QString CHttpUserIdentify::getUserInfoByKey(QString key)
{
    if( userInfoCount() < 0 ){
        return QString();
    }

    return m_ret.getRet( 0, key );
}


void CHttpUserIdentify::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
        qDebug() << "error string = " << pReply->errorString();
        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
    this->setRecvJsonData( recvData );
    bool ok = parseJson();
    emit sigRequestRetAvailable( ok );
}

void CHttpUserIdentify::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
