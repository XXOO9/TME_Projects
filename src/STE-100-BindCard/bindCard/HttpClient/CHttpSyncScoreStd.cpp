#include "CHttpSyncScoreStd.h"

CHttpSyncScoreStd::CHttpSyncScoreStd(QObject *parent) : CHttpAbstractNetWork( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    initConfig();
    initConnection();
}

CHttpSyncScoreStd::~CHttpSyncScoreStd()
{

}

void CHttpSyncScoreStd::initConfig()
{
    setInterfaceType( requestType::SYNCSCORESTD );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_SYNCSCORE_KEY );
    QString urlString = baseIp + interface;

    this->setUrl( urlString );
}

void CHttpSyncScoreStd::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpSyncScoreStd::onNewReplyAvailable );
    connect( this, &CHttpSyncScoreStd::sigTimeout, this, &CHttpSyncScoreStd::onTimeoutHandler );
}

void CHttpSyncScoreStd::httpRequest()
{
    m_pReply = m_pNetWorkMgr->get( request() );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpSyncScoreStd::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, REQUEST_VALUE ).toJsonArray();
    QJsonArray::iterator iter = dataArray.begin();
    for( ; iter != dataArray.end(); iter++ ){
        QJsonObject dataObj = iter->toObject();
        QMap< QString, QString > retMap;
        retMap.insert( syncScoreStdKeys::grade,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::grade ).toString());

        retMap.insert( syncScoreStdKeys::testItem,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::testItem ).toString());

        retMap.insert( syncScoreStdKeys::score,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::score ).toString());

        retMap.insert( syncScoreStdKeys::gender,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::gender ).toString());

        retMap.insert( syncScoreStdKeys::resultStart,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::resultStart ).toString());

        retMap.insert( syncScoreStdKeys::resultEnd,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::resultEnd ).toString());
        m_ret.appendRet( retMap );
    }

    return true;
}

bool CHttpSyncScoreStd::isParameterEmpty()
{
    return false;
}

int CHttpSyncScoreStd::resultCount()
{
    return m_ret.size();
}

QMap<QString, QString> CHttpSyncScoreStd::getGroupRet(int index)
{
    if( index < 0 || index > m_ret.size() ){
        return QMap<QString, QString>();
    }
    return m_ret.getRetMapByIndex( index );
}

QString CHttpSyncScoreStd::getSingleRet(int index, QString key)
{
    if( index < 0 || index > m_ret.size() ){
        return QString();
    }
    return m_ret.getRet( index, key );
}

void CHttpSyncScoreStd::onNewReplyAvailable(QNetworkReply *pReply)
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

void CHttpSyncScoreStd::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
