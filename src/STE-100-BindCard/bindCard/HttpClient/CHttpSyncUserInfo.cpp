#include "CHttpSyncUserInfo.h"

CHttpSyncUserInfo::CHttpSyncUserInfo(QString timeStamp, QObject *parent) : CHttpAbstractNetWork ( parent )
{
    if( timeStamp.isEmpty() ){
        timeStamp = QString::number( CHttpAbstractNetWork::currentTimeStamp() );
    }
    this->setTextPara( timeStamp );
    initConfig();
    initConnection();
}

CHttpSyncUserInfo::~CHttpSyncUserInfo()
{

}

void CHttpSyncUserInfo::initConfig()
{
    setInterfaceType( requestType::SYNCUSERINFO );
    CHttpConfigReaderSingeton *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_SYNCUSERINFO_KEY );
    QString parameter = textPara();
    QString urlString = baseIp + interface + SYNCUSERINFO_NAME_UPDATETIME + parameter;
    this->setUrl( urlString );
}

void CHttpSyncUserInfo::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpSyncUserInfo::onNewReplyAvailable );
    connect( this, &CHttpSyncUserInfo::sigTimeout, this, &CHttpSyncUserInfo::onTimeoutHandler );
}

void CHttpSyncUserInfo::httpRequest()
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }

    if( isParameterEmpty() ){
        return;
    }

    QNetworkRequest request = this->request();
    qDebug() << "sync userinfo url = " << request.url().toString();
    m_pReply = m_pNetWorkMgr->get( request );
    startTimer( TIMEOUT_SHORT );

}

bool CHttpSyncUserInfo::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    QJsonObject rootObj = jsonDataToJsonObject( recvJsonData() );
    QJsonObject dataObj = takeValueOfKeyFromJsonObject( rootObj, REQUEST_VALUE ).toJsonObject();
    QJsonArray userArray = takeValueOfKeyFromJsonObject( dataObj, SYNCUSERINFO_ARRAY_NAME ).toJsonArray();
    QJsonArray::iterator iter = userArray.begin();
    for( ; iter != userArray.end(); iter++ ){
        QJsonObject jsObj = iter->toObject();
        QMap< QString, QString > retMap;
        retMap.insert( USERIDENTIFY_KEY_ID, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_ID ).toString() );
        retMap.insert( USERIDENTIFY_KEY_YEAR, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_YEAR ).toString() );
        retMap.insert( USERIDENTIFY_KEY_CLASSID, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_CLASSID ).toString() );
        retMap.insert( USERIDENTIFY_KEY_ROLE, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_ROLE ).toString() );
        retMap.insert( USERIDENTIFY_KEY_CODE, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_CODE ).toString() );
        retMap.insert( USERIDENTIFY_KEY_CARDNO, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_CARDNO ).toString() );
        retMap.insert( USERIDENTIFY_KEY_NAME, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_NAME ).toString() );
        retMap.insert( USERIDENTIFY_KEY_GENDER, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_GENDER ).toString() );
        retMap.insert( USERIDENTIFY_KEY_BIRTHDAY, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_BIRTHDAY ).toString() );
        retMap.insert( USERIDENTIFY_KEY_NATION, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_NATION ).toString() );
        retMap.insert( USERIDENTIFY_KEY_FACEFEATURE, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_FACEFEATURE ).toString() );
        retMap.insert( USERIDENTIFY_KEY_DESCRIPTION, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_DESCRIPTION ).toString() );
        retMap.insert( USERIDENTIFY_KEY_AVATARURL, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_AVATARURL ).toString() );
        retMap.insert( USERIDENTIFY_KEY_UPDATETIME, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_UPDATETIME ).toString() );
        m_ret.appendRet( retMap );
    }
    return true;
}

bool CHttpSyncUserInfo::isParameterEmpty()
{
    if( textPara().isEmpty() ){
        return true;
    }
    return false;
}

int CHttpSyncUserInfo::resultCount()
{
    return m_ret.size();
}

QMap<QString, QString> CHttpSyncUserInfo::getGroupRet(int index)
{
    if( index < 0 || index > m_ret.size() ){
        return QMap<QString, QString>();
    }
    return m_ret.getRetMapByIndex( index );
}

QString CHttpSyncUserInfo::getSingleInfo(int index, QString key)
{
    if( index < 0 || index > m_ret.size() ){
        return QString();
    }
    return m_ret.getRet( index, key );
}

void CHttpSyncUserInfo::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpSyncUserInfo::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
