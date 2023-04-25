#include "CHttpSyncClassInfo.h"

CHttpSyncClassInfo::CHttpSyncClassInfo(QString timeSatmp, QObject *parent) : CHttpAbstractNetWork ( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    setTextPara( timeSatmp );
    initConfig();
    initConnection();
}

CHttpSyncClassInfo::~CHttpSyncClassInfo()
{

}

void CHttpSyncClassInfo::initConfig()
{
    setInterfaceType( requestType::SYNCCLASSINFO );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_SYNCCLASSINFO_KEY );
    QString urlString = baseIp + interface + INTERFACE_SYNCCLASSINFO_UPDATETIME + textPara();
    setUrl( urlString );
}

void CHttpSyncClassInfo::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpSyncClassInfo::onNewReplyAvailable );
    connect( this, &CHttpSyncClassInfo::sigTimeout, this, &CHttpSyncClassInfo::onTimeoutHandler );
}

void CHttpSyncClassInfo::httpRequest()
{
    m_pReply = m_pNetWorkMgr->get( request() );
    QString str = request().url().toString();
    startTimer( TIMEOUT_SHORT );
}

bool CHttpSyncClassInfo::parseJson()
{
    if( !isJson() ){
        return false;
    }
    if( !isRequestSuccess() ){
        return false;
    }

    QJsonObject rootObj = jsonDataToJsonObject( recvJsonData() );
    QJsonObject dataObj = takeValueOfKeyFromJsonObject( rootObj, REQUEST_VALUE ).toJsonObject();
    QJsonArray classInfoArray = takeValueOfKeyFromJsonObject( dataObj, SYNCCLASSINFO_ARRAYNAME ).toJsonArray();
    QJsonArray::iterator iter = classInfoArray.begin();
    for( ; iter != classInfoArray.end(); iter++ ){
        QJsonObject jsObj = iter->toObject();
        QMap< QString, QString > retMap;
        retMap.insert( SYNCCLASSINFO_KEY_ID, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_ID ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_NAME, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_NAME ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_YEAR, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_YEAR ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_SCHOOL, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_SCHOOL ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_TYPE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_TYPE ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_ENABLE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_ENABLE ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_GRADUATE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_GRADUATE ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_DESCRIPTION, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_DESCRIPTION ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_UPDATETIME, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_UPDATETIME ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_GRADE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_GRADE ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_ID, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_ID ).toString() );
        m_ret.appendRet( retMap );
    }
    return true;
}

bool CHttpSyncClassInfo::isParameterEmpty()
{
    if( textPara().isEmpty() ){
        return true;
    }
    return false;
}

int CHttpSyncClassInfo::resultCount()
{
    return m_ret.size();
}

QMap<QString, QString> CHttpSyncClassInfo::getGroupInfo(int index)
{
    if( index < 0 || index > m_ret.size() ){
        return QMap<QString, QString>();
    }
    return m_ret.getRetMapByIndex( index );
}

QString CHttpSyncClassInfo::getSingleRet(int index, QString key)
{
    if( index < 0 || index > m_ret.size() ){
        return QString();
    }
    return m_ret.getRet( index, key );
}

void CHttpSyncClassInfo::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpSyncClassInfo::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();

}
