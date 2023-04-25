#include "CHttpUpLoadMultiBodyScores.h"
#include "../Config/CDatabaseManage.h"

CHttpUpLoadMultiBodyScores::CHttpUpLoadMultiBodyScores(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpUpLoadMultiBodyScores";

    initConfig();
    initConnection();
}

CHttpUpLoadMultiBodyScores::~CHttpUpLoadMultiBodyScores()
{

}

void CHttpUpLoadMultiBodyScores::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    parseParam();
    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }


    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    m_pReply = m_pNetWorkMgr->post( request(), m_pMultiPart );
    startTimeoutTimer( UPLOAD_TIMEOUT );
}

void CHttpUpLoadMultiBodyScores::setUpLoadMultiScores(QVariantList &scoresList)
{
    m_listMapUpLoadScores.clear();
    m_listMapUpLoadScores = scoresList;
}

QString CHttpUpLoadMultiBodyScores::lastErrorString() const
{
    return m_lastErrorString;
}

QVariantList CHttpUpLoadMultiBodyScores::getFailedScoreInfoList() const
{
    return m_listMapRetInfos;
}

QString CHttpUpLoadMultiBodyScores::translateBodyComposition(const QVariantMap &bodyCompositionMap)
{
    QString bodyCompositionStr = "%1%2%3%4";
    if( !bodyCompositionMap.contains( UPLOADBODYSCORE_KEY_HEIGHT ) ){
        LOGINFO( "未包含必要信息-> 身高, 即将退出函数..." );
        return "";
    }

    if( !bodyCompositionMap.contains( UPLOADBODYSCORE_KEY_WEIGHT ) ){
        LOGINFO( "未包含必要信息-> 体重, 即将退出函数..." );
        return "";
    }

    //如果只包含身高体重
    if( bodyCompositionMap.size() == 2 ){
        bodyCompositionStr = bodyCompositionStr
                .arg( HEIGHT_FLAG )
                .arg( bodyCompositionMap.value( UPLOADBODYSCORE_KEY_HEIGHT ).toString() )
                .arg( WEIGHT_FLAG )
                .arg( bodyCompositionMap.value( UPLOADBODYSCORE_KEY_WEIGHT ).toString() );

        return bodyCompositionStr;
    }

    //如果包含所有的人体成分
    bodyCompositionStr = "";
    bodyCompositionStr += ( HEIGHT_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_HEIGHT ).toString() );
    bodyCompositionStr += ( WEIGHT_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_WEIGHT ).toString() );
    bodyCompositionStr += ( FATRATE_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_FARATE ).toString() );
    bodyCompositionStr += ( BMI_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_BMI ).toString() );
    bodyCompositionStr += ( HEADWEIGHT_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_HEADWEIGHT ).toString() );
    bodyCompositionStr += ( HEADFAT_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_HEADFAT ).toString() );
    bodyCompositionStr += ( HEADMUSCLE_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_HEADMUSCLE ).toString() );
    bodyCompositionStr += ( TBMM_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_TBMM ).toString() );
    bodyCompositionStr += ( WBIS_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_WBIS ).toString() );
    bodyCompositionStr += ( TBW_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_TBW ).toString() );
    bodyCompositionStr += ( WHR_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_WHR ).toString() );
    bodyCompositionStr += ( VFA_FLAG + bodyCompositionMap.value( UPLOADBODYSCORE_KEY_VFA ).toString() );

    return bodyCompositionStr;
}

void CHttpUpLoadMultiBodyScores::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    m_listMapRetInfos.clear();

    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }
    m_pMultiPart->setParent( pReply );
    if( QNetworkReply::NoError != pReply->error() ){

        m_vecUploadMulRetInfos.first().setUpLoadSuccess( false );
        m_vecUploadMulRetInfos.first().setErrorCode( QString::number( UNKNOWNERROR ) );
        //如果是超时引起的, 就判定为提交成功, 但是要把 m_vecUploadMulRetInfos 里面的 errorCode 置为超时错误码
        if( errorType::TIMEOUT_ERROR == getLocalErrType() ){
            m_vecUploadMulRetInfos.first().setUpLoadSuccess( false );
            m_vecUploadMulRetInfos.first().setErrorCode( QString::number( TIMEOUT ) );
            m_vecUploadMulRetInfos.first().setErrorMsg( "time out error" );
            LOGERROR( "upload multi body score time out...." );
            emit sigRequestRetAvailable( true );

            if( nullptr != m_pReply){
                pReply->deleteLater();
                pReply = nullptr;
                m_pReply = nullptr;
                m_pMultiPart = nullptr;
            }

            return;
        }

        QString errorString = pReply->errorString();
        if( errorString.isEmpty() ){
            errorString = "multi upload body score, UnKnown Error";
        }

        m_lastErrorString = errorString;
        setErrType( REQUEST_ERROR, errorString );
        emit sigRequestRetAvailable( false );

        //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
        if( nullptr != m_pReply){
            pReply->deleteLater();
            pReply = nullptr;
            m_pReply = nullptr;
            m_pMultiPart = nullptr;
        }
        return;
    }

    QByteArray recvData = pReply->readAll();

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply){
        pReply->deleteLater();
        pReply   = nullptr;
        m_pReply = nullptr;
        m_pMultiPart = nullptr;
    }

    QString recvString =  QString::fromUtf8( recvData );

#if QT_NO_DEBUG
#else
    qDebug() << "upload multi body scores, recv str = " << recvString;
#endif

    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    LOGINFO( "###################  parse json result = " + QString::number( ok ).toStdString() );
    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadMultiBodyScores::onTimeoutHandler()
{
    if( nullptr == m_pReply ){
        return;
    }

    if( m_pReply->isFinished() ){
        return;
    }

    setErrType( errorType::TIMEOUT_ERROR );
    setRequestRetCode( TIMEOUT );
    m_pReply->abort();
}

void CHttpUpLoadMultiBodyScores::initConfig()
{
    setInterfaceType( requestType::UPLOADMULTIBODYSCORES );
    setUrl( QString::fromStdString( CDatabaseManage::GetInstance().getServerFunConfig().getStrMultiUpLoadBodyScoreUrl() ));
}

bool CHttpUpLoadMultiBodyScores::isRequestSuccess()
{
    //先不写在基类，以防需求又变导致判断条件不一样,三种批量上传各自写一个该函数
    m_listMapRetInfos.clear();
    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QString requestRet = takeValueOfKeyFromJsonObject( jsObj, REQUEST_RESULT_KEY ).toString();
    setRequestRetCode( requestRet.toInt() );

    //判断是否全部上传成功
    if( REQUEST_RESULT_SUCCESS == requestRet ){
        return true;
    }

    //如果有失败的成绩单元
    CMultiUploadRetInfo *eleFailedScoreUnit = nullptr;
    QJsonObject tmpObj;
    int index = -1;
    QJsonArray failedArray = takeValueOfKeyFromJsonObject( jsObj, REQUEST_VALUE ).toJsonArray();

    for( auto &ele : failedArray ){
        tmpObj = ele.toObject();

        if( tmpObj.value( REQUEST_RESULT_KEY ).toVariant().toInt() == REPEATEUPLOAD ){
            continue;
        }

        //获取失败成绩单元的 index
        index = tmpObj.value( REQUEST_INDEX ).toInt();
        eleFailedScoreUnit = &m_vecUploadMulRetInfos[ index ];
        eleFailedScoreUnit->setUpLoadSuccess( false );
        eleFailedScoreUnit->setErrorCode( tmpObj.value( REQUEST_RESULT_KEY ).toVariant().toString() );
        eleFailedScoreUnit->setErrorMsg( tmpObj.value( REQUEST_MESSAGE ).toString() );
    }

    eleFailedScoreUnit = nullptr;
    return false;
}

bool CHttpUpLoadMultiBodyScores::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUpLoadMultiBodyScores::isParameterEmpty()
{
    if( m_listMapUpLoadScores.size() <= 0 ){
        return true;
    }
    return false;
}

void CHttpUpLoadMultiBodyScores::parseParam()
{
    int scoreCount = m_listMapUpLoadScores.size();

    //每解析一次传进来的 m_listMapUpLoadScores 就重置一次
    m_vecUploadMulRetInfos.clear();

    QVariantMap tmpMap;
    QVariantList progressPicsList;
    CMultiUploadRetInfo retInfo;

    retInfo.setScoreDataSrc( m_dataSrc );
    retInfo.setUploadType( UPLOAD_TYPE::Upload_Body );

    tmpMap = m_listMapUpLoadScores.first().toMap();

    //设置设备号
    appendTextPart( HTTP_KEYS::devCode, tmpMap.value( HTTP_KEYS::devCode ).toString() );

    //设置设备类型
    appendTextPart( HTTP_KEYS::devType, tmpMap.value( HTTP_KEYS::devType ).toString() );
    retInfo.setTestItem( HTTP_TEST_ITEM_HEIGHT );

    for( int i = 0; i < scoreCount; i++ ){
        tmpMap = m_listMapUpLoadScores[ i ].toMap();

        //设置用户ID, 并初始化相应index的提交结果( retInfo )信息
        appendTextPart( uploadMultiBodyScores::userId.arg( QString::number( i ) ), tmpMap.value( HTTP_KEYS::userId ).toString() );
        retInfo.setUserId( tmpMap.value( HTTP_KEYS::userId ).toString() );

        //设置测试结果
        appendTextPart( uploadMultiBodyScores::recordListStr.arg( QString::number( i ) ), tmpMap.value( HTTP_KEYS::recordListStr ).toString() );

        //设置测试时间戳
        appendTextPart( uploadMultiBodyScores::recordTime.arg( QString::number( i ) ), tmpMap.value( HTTP_KEYS::recordTime ).toString() );
        retInfo.setTimeStamp( tmpMap.value( HTTP_KEYS::recordTime ).toString() );

        //设置成绩类型
        appendTextPart( uploadMultiBodyScores::scoreType.arg( QString::number( i ) ), tmpMap.value( HTTP_KEYS::type ).toString() );

        m_vecUploadMulRetInfos << retInfo;
    }
}

void CHttpUpLoadMultiBodyScores::setDataSrc(const DATA_SRC &dataSrc)
{
    m_dataSrc = dataSrc;
}

QVector<CMultiUploadRetInfo> CHttpUpLoadMultiBodyScores::getVecUploadMulRetInfos() const
{
    return m_vecUploadMulRetInfos;
}
