#include "CHttpGetVideoUrls.h"

CHttpGetVideoUrls::CHttpGetVideoUrls(QString timeStamp, QObject *parent) : CHttpAbstractNetWork (parent)
{
    setTextPara( timeStamp );
    initConfig();
    initConnection();
}

CHttpGetVideoUrls::~CHttpGetVideoUrls()
{

}

void CHttpGetVideoUrls::initConfig()
{
    setInterfaceType( requestType::GETVIDEOURLS );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_SYNCVIDEO_KEY );
    QString timeStamp = this->textPara();
    QString urlString = baseIp + interface + INTERFACE_SYNVVIDEO_UPDATETIME + timeStamp;
    setUrl( urlString );
}

void CHttpGetVideoUrls::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpGetVideoUrls::onNewReplyAvailable );
    connect( this, &CHttpGetVideoUrls::sigTimeout, this, &CHttpGetVideoUrls::onTimeoutHandler );
}

void CHttpGetVideoUrls::httpRequest()
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    qDebug() << "sync video urls url = " << request().url().toString();
    m_pReply = m_pNetWorkMgr->get( request() );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpGetVideoUrls::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, SYNCVIDEO_ARRAY_NAME ).toJsonArray();
    QJsonArray::iterator iter = dataArray.begin();
    for( ; iter != dataArray.end(); iter++ ){
        QMap< QString, QString > retMap;
        QJsonObject dataObj = iter->toObject();
        retMap.insert( SYNCVIDEO_NAME_TYPE, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_TYPE ).toString() );
        retMap.insert( SYNCVIDEO_NAME_URL, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_URL ).toString() );
        retMap.insert( SYNCVIDEO_NAME_UPDATETIME, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_UPDATETIME ).toString() );
        retMap.insert( SYNCVIDEO_NAME_PICURL, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_PICURL ).toString() );
        m_rets.appendRet( retMap );
    }

    return true;
}

bool CHttpGetVideoUrls::isParameterEmpty()
{
    return false;
}

int CHttpGetVideoUrls::resultCount()
{
    return m_rets.size();
}

QMap<QString, QString> CHttpGetVideoUrls::getGroupRet(int index)
{
    if( index < 0 || index > m_rets.size() ){
        return QMap<QString, QString>();
    }
    return m_rets.getRetMapByIndex( index );
}

QString CHttpGetVideoUrls::getSingleRet(int index, QString key)
{
    if( index < 0 || index > m_rets.size() ){
        return QString();
    }
    return m_rets.getRet( index, key );
}


void CHttpGetVideoUrls::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        sigRequestRetAvailable( false );
        qDebug() << "error string = " << pReply->errorString();
        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
    this->setRecvJsonData( recvData );
    bool ok = parseJson();
    emit sigRequestRetAvailable( ok );
}

void CHttpGetVideoUrls::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
