#include "CHttpQueryHistoryScore.h"

CHttpQueryHistoryScore::CHttpQueryHistoryScore(QObject *parent) : CHttpAbstractNetWork( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    initConfig();
    initConnection();
}

CHttpQueryHistoryScore::~CHttpQueryHistoryScore()
{

}

void CHttpQueryHistoryScore::initConfig()
{
    setInterfaceType( requestType::QUERYCOMMONHISTORYSCORE );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_QUERYHISTORY_KEY );
    QString urlString = baseIp + interface;
    setUrl( urlString );
}

void CHttpQueryHistoryScore::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpQueryHistoryScore::onNewReplyAvailable );
    connect( this, &CHttpQueryHistoryScore::sigTimeout, this, &CHttpQueryHistoryScore::onTimeoutHandler );
}

void CHttpQueryHistoryScore::httpRequest()
{
    QString userId = this->getSinglePara( QUERYHISTORY_KEY_USERID );
    QString testItem = this->getSinglePara( QUERYHISTORY_KEY_TESTITEM );
    QString timeInterval = this->getSinglePara( QUERYHISTORY_KEY_TIMEINTERVAL );
    QString type = this->getSinglePara( QUERYHISTORY_KEY_TYPE);
    QString url = request().url().toString();
    url += QUERYHISTORY_USERID + userId + QUERYHISTORY_TESTITEM +
            testItem + QUERYHISTORY_TIMEINTERVAL + timeInterval + QUERYHISTORY_TYPE + type;
    setUrl( url );
    qDebug() << "query history url = " << request().url().toString();
    m_pReply = m_pNetWorkMgr->get( request() );
    startTimer( TIMEOUT_SHORT );
}

bool CHttpQueryHistoryScore::parseJson()
{

    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, QUERYHISTORY_KEY_ARRAY_NAME ).toJsonArray();
    QJsonArray::iterator iter = dataArray.begin();
    for( ; iter != dataArray.end(); iter++ ){
        QJsonObject dataObj = iter->toObject();
        QMap<QString, QString> retMap;
        retMap.insert( QUERYHISTORY_TEST_ITEM, takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_TEST_ITEM ).toString() );
        retMap.insert( QUERYHISTORY_SCORE, takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_SCORE ).toString() );
        retMap.insert( QUERYHISTORY_RECORD, takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_RECORD ).toString() );
        retMap.insert( QUERYHISTORY_RECORDTIME, takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_RECORDTIME ).toString() );

        m_ret.appendRet( retMap );
    }
    return true;
}

bool CHttpQueryHistoryScore::isParameterEmpty()
{
    return true;
}

void CHttpQueryHistoryScore::setUserId(QString userId)
{
    appendMultiParaMap( QUERYHISTORY_KEY_USERID, userId );
}

void CHttpQueryHistoryScore::setTestItem(TEST_ITEM testItem )
{
    QString strTestItem = QString::number( testItem, 10 );
    this->appendMultiParaMap( QUERYHISTORY_KEY_TESTITEM, strTestItem );
}

void CHttpQueryHistoryScore::setTimeInterval(queryTimeInterval timeInterval)
{
    QString strTimeInterval = QString::number( timeInterval );
    this->appendMultiParaMap( QUERYHISTORY_KEY_TIMEINTERVAL, strTimeInterval );
}

void CHttpQueryHistoryScore::setTestScoreType(testType type)
{
    QString strType = QString::number( type );
    this->appendMultiParaMap( QUERYHISTORY_KEY_TYPE, strType );
}

int CHttpQueryHistoryScore::resultCount()
{
    return m_ret.size();
}

QMap<QString, QString> CHttpQueryHistoryScore::getGroupInfo(int index)
{
    if( index < 0 || index > m_ret.size() ){
        return QMap<QString, QString>();
    }
    return m_ret.getRetMapByIndex( index );
}

QString CHttpQueryHistoryScore::getSingleRet(int index, QString key)
{
    if( index < 0 || index > m_ret.size() ){
        return QString();
    }
    return m_ret.getRet( index, key );
}

void CHttpQueryHistoryScore::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
    this->setRecvJsonData( recvData );
    bool ok = parseJson();
    emit sigRequestRetAvailable( ok );
}

void CHttpQueryHistoryScore::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
