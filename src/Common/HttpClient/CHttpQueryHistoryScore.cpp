#include "CHttpQueryHistoryScore.h"
#include "../Config/CDatabaseManage.h"

CHttpQueryHistoryScore::CHttpQueryHistoryScore(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpQueryHistoryScore";

    initConfig();
    initConnection();
}

CHttpQueryHistoryScore::~CHttpQueryHistoryScore()
{

}

void CHttpQueryHistoryScore::initConfig()
{
    setInterfaceType( requestType::QUERYCOMMONHISTORYSCORE );
    m_baseInterfaceUrl = QString::fromStdString(
                CDatabaseManage::GetInstance().getServerFunConfig().getQueryHistoryScoreFuncUrl() );
    setUrl( m_baseInterfaceUrl );
}

void CHttpQueryHistoryScore::httpRequest()
{
    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    QString userId = this->getSinglePara( QUERYHISTORY_KEY_USERID );

    //    userId = "4391";

    QString testItem = this->getSinglePara( QUERYHISTORY_KEY_TESTITEM );
    QString timeInterval = this->getSinglePara( QUERYHISTORY_KEY_TIMEINTERVAL );
    QString type = this->getSinglePara( QUERYHISTORY_KEY_TYPE);
    QString url = m_baseInterfaceUrl + QUERYHISTORY_USERID + userId + QUERYHISTORY_TESTITEM +
            testItem + QUERYHISTORY_TIMEINTERVAL + timeInterval + QUERYHISTORY_TYPE + type;
    setUrl( url );
#if QT_NO_DEBUG
    LOGINFO( request().url().toString().insert( 0, "query history url = " ).toStdString() );
#else
    qDebug() << "query history url = " << request().url().toString();
#endif

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    m_pReply = m_pNetWorkMgr->get( request() );
    startTimeoutTimer( TIMEOUT_SHORT );
    clearParamMap();
}

void CHttpQueryHistoryScore::cleanHumanScoreList()
{
    m_huamenCompositionList.clear();
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
    m_ret.clear();

    //如果查询的是身高体重,则单独处理
    if( HTTP_TEST_ITEM_HEIGHT == m_curTestItem || HTTP_TEST_ITEM_WEIGHT == m_curTestItem ){
        configHeightAndWeightData( dataArray );
        return true;
    }

    for( ; iter != dataArray.end(); iter++ ){
        QJsonObject dataObj = iter->toObject();
        QMap<QString, QString> retMap;
        retMap.insert( QUERYHISTORY_TEST_ITEM,
                       takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_TEST_ITEM ).toString() );

        retMap.insert( QUERYHISTORY_SCORE,
                       takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_SCORE ).toString() );

        retMap.insert( QUERYHISTORY_RECORD,
                       takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_RECORD ).toString() );

        retMap.insert( QUERYHISTORY_RECORDTIME,
                       takeValueOfKeyFromJsonObject( dataObj, QUERYHISTORY_RECORDTIME ).toString() );

        m_ret.appendRet( retMap );
    }
    return true;
}

bool CHttpQueryHistoryScore::isParameterEmpty()
{
    return true;
}

void CHttpQueryHistoryScore::configHeightAndWeightData( const QJsonArray &srcArr )
{
    QJsonArray::const_iterator iter = srcArr.constBegin();
    QVariantMap     eleRecordMap;
    QJsonObject tmpObj;

    QVariantMap heightMap;
    QVariantMap weightMap;
    QString     tmpRecordTime = "";
    long long   timeStamp = 0;


    for( ; iter != srcArr.constEnd(); iter++ ){
        eleRecordMap.clear();

        tmpObj = iter->toObject();

        //解析时间
        timeStamp = tmpObj.value( QUERYHISTORY_RECORDTIME ).toVariant().toLongLong();
        tmpRecordTime = CHttpAbstractNetWork::timeStampToDateTime( timeStamp );
        eleRecordMap[ QUERYHISTORY_RECORDTIME ] = tmpRecordTime;
        eleRecordMap[ retKeys::heightRecordKey ] = tmpObj.value( "heightRecord" ).toVariant().toString();
        eleRecordMap[ retKeys::weightRecordKey ] = tmpObj.value( "weightRecord" ).toVariant().toString();

        m_huamenCompositionList << eleRecordMap;
    }

    /*
    // 遍历所有的数据, 将身高数据和体重数据分离开来
    for( ;iter != srcArr.constEnd(); iter++ ){
        tmpObj = iter->toObject();

        //解析时间
        timeStamp = tmpObj.value( QUERYHISTORY_RECORDTIME ).toVariant().toLongLong();
        tmpRecordTime = CHttpAbstractNetWork::timeStampToDateTime( timeStamp );



        if( HTTP_TEST_ITEM_HEIGHT == tmpObj.value( QUERYHISTORY_TEST_ITEM ).toVariant().toInt() ){
            heightMap[ tmpRecordTime ] = tmpObj.value( QUERYHISTORY_RECORD ).toVariant().toString();
        }else{
            weightMap[ tmpRecordTime ] = tmpObj.value( QUERYHISTORY_RECORD ).toVariant().toString();
        }
    }

    //寻找时间戳相同的数据
    QVariantMap::iterator heightIter = heightMap.begin();
    QString               tmpKeyRecordtime = "";

    m_huamenCompositionList.clear();

    for( ; heightIter != heightMap.end(); heightIter++ ){
        eleRecordMap.clear();

        tmpKeyRecordtime = heightIter.key();
        eleRecordMap[ QUERYHISTORY_TEST_ITEM ] = QString::number( HTTP_TEST_ITEM_HEIGHT );
        eleRecordMap[ QUERYHISTORY_RECORDTIME ] = tmpKeyRecordtime;
        eleRecordMap[ retKeys::heightRecordKey ] = heightIter.value().toString();
        eleRecordMap[ retKeys::weightRecordKey ] = weightMap.value( tmpKeyRecordtime ).toString();

        m_huamenCompositionList << eleRecordMap;
    }

    */
}

bool CHttpQueryHistoryScore::isServerRecordsContainsTargetRecordTime(const QString &recordTime)
{
    for( auto &ele : m_huamenCompositionList ){
        if( recordTime == ele.toMap().value( "recordTime" ) ){
            return true;
        }
    }

    return false;
}

void CHttpQueryHistoryScore::sortHumanScoreLists(QVariantList &humanScoreList)
{
    QDateTime tmpDateTime = QDateTime::currentDateTime();
    QDateTime tmpMaxDateTime = tmpDateTime;
    int maxIndex = 0;
    const int size = humanScoreList.size();

    for( int startIndex = 0; startIndex < size; startIndex++ ){
        maxIndex = startIndex;
        tmpMaxDateTime = QDateTime::fromString( humanScoreList[ startIndex ].toMap().
                                                value( QUERYHISTORY_RECORDTIME ).toString(), "yyyy-MM-dd hh:mm:ss" );

        for( int offset = startIndex; offset < size; offset++ ){

            tmpDateTime = QDateTime::fromString( humanScoreList[ offset ].toMap().
                                                 value( QUERYHISTORY_RECORDTIME ).toString(), "yyyy-MM-dd hh:mm:ss" );

            if( tmpMaxDateTime < tmpDateTime ){
                tmpMaxDateTime = tmpDateTime;
                maxIndex = offset;
            }
        }

        humanScoreList.swap( maxIndex, startIndex );
    }
}

void CHttpQueryHistoryScore::setUserId(QString userId)
{
    appendMultiParaMap( QUERYHISTORY_KEY_USERID, userId );
}

void CHttpQueryHistoryScore::setTestItem(TEST_ITEM testItem )
{
    m_curTestItem = testItem;
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
    if( nullptr == m_pReply || nullptr == pReply ){
        return;
    }
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        qDebug() << "http inside error string = " << pReply->errorString();
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

#if QT_NO_DEBUG
#else
    qDebug() << "query history ret string = " << recvString;
#endif
    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply){
        pReply->deleteLater();
        pReply = nullptr;
        m_pReply = nullptr;
    }

    emit sigRequestRetAvailable( ok );  //CHttpRequestMgr::onQueryHistoryScoreResultAvailable
}

void CHttpQueryHistoryScore::onTimeoutHandler()
{
    if( nullptr == m_pReply ){
        return;
    }
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}

QVariantList CHttpQueryHistoryScore::getHuamenCompositionList() const
{
    return m_huamenCompositionList;
}

TEST_ITEM CHttpQueryHistoryScore::getCurTestItem() const
{
    return m_curTestItem;
}

QVector<QStringList> CHttpQueryHistoryScore::getVecDisplayRets() const
{
    return m_vecDisplayRets;
}

void CHttpQueryHistoryScore::setVecDisplayRets(QVector<QStringList> vecRets)
{
    m_vecDisplayRets = vecRets;
}

void CHttpQueryHistoryScore::clearDisplayRets()
{
    m_ret.clear();
    m_vecDisplayRets.clear();
}

void CHttpQueryHistoryScore::removeCommonDatas()
{
    QVariantMap uniqueMap;
    for( auto &eachEle : m_vecDisplayRets )
    {
        if( eachEle.size() < retKeys::retStringMinSize ){
            continue;
        }

#if QT_NO_DEBUG
#else
        qDebug() << "score = " << eachEle[ retKeys::index_testRet ];
#endif

        if( eachEle[ retKeys::index_testRet ] == QStringLiteral( "无效成绩" ) ){
            eachEle[ retKeys::index_testScore ] = "-";
        }
        uniqueMap.insert( eachEle.first(), eachEle );
    }

    m_vecDisplayRets.clear();
    QVariantMap::iterator iter = uniqueMap.begin();
    for( ;iter != uniqueMap.end(); iter++ ){
        m_vecDisplayRets << iter.value().toStringList();
    }
}

void CHttpQueryHistoryScore::mergeAndRemoveCommonBodyTestResults(QVector< QStringList > &localBodyResults)
{
    QVariantMap tmpBodyResultMap;
    QString tmpRecordTime = "";
    for( auto &ele : localBodyResults ){
        tmpBodyResultMap.clear();

        //转换时间, 进行去重的时候以 yyyy-MM-dd hh:mm:ss 格式的时间为索引
        tmpRecordTime = plainNumberTimeFormat2String( ele.first() );
        ele.first()   = tmpRecordTime;

        //如果没有相同时间的测试记录，则添加到 m_huamenCompositionList
        if( !isServerRecordsContainsTargetRecordTime( tmpRecordTime ) ){
            tmpBodyResultMap.clear();
            tmpBodyResultMap[ "recordTime" ] = ele[ 0 ];
            tmpBodyResultMap[ "height" ]     = ele[ 1 ];
            tmpBodyResultMap[ "weight" ]     = ele[ 2 ];
            m_huamenCompositionList << tmpBodyResultMap;
        }
    }

    //按时间倒序排序
    sortHumanScoreLists( m_huamenCompositionList );
}

CQueryCommonHistoryRet CHttpQueryHistoryScore::getRet() const
{
    return m_ret;
}

void CHttpQueryHistoryScore::setMapLocalHistoryScores(const QVariantMap &mapLocalHistoryScores)
{
    m_mapLocalHistoryScores = mapLocalHistoryScores;
}

long long CHttpQueryHistoryScore::getQueryStartDate(queryTimeInterval timeInterval)
{
    QDate today = QDate::currentDate();
    switch( timeInterval ){
    case recent_one_week:
        today = today.addDays( -7 );
        break;
    case recent_one_month:
        today = today.addMonths( -1 );
        break;
    case recent_three_month:
        today = today.addMonths( -3 );
        break;
    default:
        break;
    }

    QString date = today.toString( "yyyyMMdd" ) + "000000";
    return date.toLongLong();
}

QVariantMap CHttpQueryHistoryScore::getMapLocalHistoryScores() const
{
    return m_mapLocalHistoryScores;
}
