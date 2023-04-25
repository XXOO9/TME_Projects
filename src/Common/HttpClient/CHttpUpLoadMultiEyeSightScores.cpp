#include "CHttpUpLoadMultiEyeSightScores.h"
#include "../Config/CDatabaseManage.h"

CHttpUpLoadMultiEyeSightScores::CHttpUpLoadMultiEyeSightScores(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpUpLoadMultiEyeSightScores";

    initConfig();
    initConnection();
}

CHttpUpLoadMultiEyeSightScores::~CHttpUpLoadMultiEyeSightScores()
{

}

void CHttpUpLoadMultiEyeSightScores::httpRequest()
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

void CHttpUpLoadMultiEyeSightScores::setUpLoadMultiScores(QVariantList &scoresList)
{
    m_listMapUpLoadScores.clear();
    m_listMapUpLoadScores = scoresList;
}

void CHttpUpLoadMultiEyeSightScores::appendUpLoadScore(QVariantMap &score)
{
    m_listMapUpLoadScores.append( score );
}

QString CHttpUpLoadMultiEyeSightScores::lastErrorString() const
{
    return m_lastErrorString;
}

QVariantList CHttpUpLoadMultiEyeSightScores::getFailedScoreInfoList() const
{
    return m_listMapFailedScore;
}

void CHttpUpLoadMultiEyeSightScores::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    m_listMapFailedScore.clear();

    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }

    m_pMultiPart->setParent( pReply );
    if( QNetworkReply::NoError != pReply->error() ){
        QString errorString = pReply->errorString();
        if( errorString.isEmpty() ){
            errorString = "multi upload eye sight score, UnKnown Error";
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
    qDebug() << "upload multi eyeSight scores, recv string = " << recvString;
#endif
    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    LOGINFO( "###################  parse json result = " + QString::number( ok ).toStdString() );
    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadMultiEyeSightScores::onTimeoutHandler()
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

void CHttpUpLoadMultiEyeSightScores::initConfig()
{
    setInterfaceType( requestType::UPLOADMULTIEYESIGHTSCORES );
    setUrl( QString::fromStdString( CDatabaseManage::GetInstance().getServerFunConfig().getStrMultiUpLoadEyeSightScoreUrl() ));
}

bool CHttpUpLoadMultiEyeSightScores::isRequestSuccess()
{
    //先不写在基类，以防需求又变导致判断条件不一样,三种批量上传各自写一个该函数
    m_listMapFailedScore.clear();
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
        index = tmpObj.value( REQUEST_INDEX ).toInt();

        eleFailedScoreUnit = &m_vecUploadMulRetInfos[ index ];
        eleFailedScoreUnit->setUpLoadSuccess( false );
        eleFailedScoreUnit->setErrorCode( tmpObj.value( REQUEST_RESULT_KEY ).toVariant().toString() );
        eleFailedScoreUnit->setErrorMsg( tmpObj.value( REQUEST_MESSAGE ).toString() );
    }

    eleFailedScoreUnit = nullptr;
    return false;
}

bool CHttpUpLoadMultiEyeSightScores::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUpLoadMultiEyeSightScores::isParameterEmpty()
{
    if( m_listMapUpLoadScores.size() <= 0 ){
        return true;
    }
    return false;
}

void CHttpUpLoadMultiEyeSightScores::parseParam()
{
    int scoreCount = m_listMapUpLoadScores.size();

    //每解析一次传进来的 m_listMapUpLoadScores 就重置一次
    m_vecUploadMulRetInfos.clear();

    QVariantMap tmpMap;
    QVariantList progressPicsList;
    CMultiUploadRetInfo retInfo;

    retInfo.setScoreDataSrc( m_dataSrc );
    retInfo.setUploadType( Upload_EyeSight );

    tmpMap = m_listMapUpLoadScores.first().toMap();

    //设置设备号
    appendTextPart( HTTP_KEYS::devCode, tmpMap.value( HTTP_KEYS::devCode ).toString() );

    //设置设备类型
    appendTextPart( HTTP_KEYS::devType, tmpMap.value( HTTP_KEYS::devType ).toString() );
    retInfo.setTestItem( HTTP_TEST_ITEM_VISION_RIGHT );

    for( int i = 0; i < scoreCount; i++ ){
        tmpMap = m_listMapUpLoadScores[ i ].toMap();

        //设置用户ID
        appendTextPart( uploadMultiEyeSightScores::userId.arg( QString::number( i )), tmpMap.value( HTTP_KEYS::userId ).toString() );
        retInfo.setUserId( tmpMap.value( HTTP_KEYS::userId ).toString() );

        //设置视力结果
        appendTextPart( uploadMultiEyeSightScores::recordListStr.arg( QString::number( i )), tmpMap.value( HTTP_KEYS::recordListStr ).toString() );

        //设置测试时间戳
        appendTextPart( uploadMultiEyeSightScores::recordTime.arg( QString::number( i )), tmpMap.value( HTTP_KEYS::recordTime ).toString() );
        retInfo.setTimeStamp( tmpMap.value( HTTP_KEYS::recordTime ).toString() );

        //2022/08/16 屏蔽, 所有的图片文件与数据分开传输
        /*
         *
        //设置签名照片
        appendSignPart( tmpMap.value( HTTP_KEYS::signPic ).toString(), uploadMultiEyeSightScores::signPic.arg( QString::number( i ) ));

        //设置过程照片
        progressPicsList.clear();
        progressPicsList = tmpMap.values( HTTP_KEYS::files );
        if( progressPicsList.size() > 0 ){
            for( int j = 0; j < progressPicsList.size(); j++ ){
                appendFilePart( progressPicsList[ j ].toString(), uploadMultiEyeSightScores::files.arg( i ).arg( j ) );
            }
        }
        */

        m_vecUploadMulRetInfos << retInfo;
    }
}

void CHttpUpLoadMultiEyeSightScores::setDataSrc(const DATA_SRC &dataSrc)
{
    m_dataSrc = dataSrc;
}

QVector<CMultiUploadRetInfo> CHttpUpLoadMultiEyeSightScores::getVecUploadMulRetInfos() const
{
    return m_vecUploadMulRetInfos;
}
