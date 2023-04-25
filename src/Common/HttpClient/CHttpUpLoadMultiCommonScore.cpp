#include "CHttpUpLoadMultiCommonScore.h"

#include "../Config/CDatabaseManage.h"

CHttpUpLoadMultiCommonScore::CHttpUpLoadMultiCommonScore(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpUpLoadMultiCommonScore";

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    m_pNetWorkMgr->setProxy( QNetworkProxy::NoProxy );

    initConfig();
    initConnection();
}

CHttpUpLoadMultiCommonScore::~CHttpUpLoadMultiCommonScore()
{

}

void CHttpUpLoadMultiCommonScore::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

#if QT_NO_DEBUG
#else
    QElapsedTimer timer;
    timer.start();
#endif

    //解析函数中会对 m_pMultiPart进行赋值
    parseParam();
    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }

#if QT_NO_DEBUG
#else
    qDebug() << "parse Par cost " << timer.restart();
    qDebug() << "upload multi common score url = " << request().url().toString();

#endif

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    QString logInfo = "upload multi url = " + request().url().toString();
    LOGINFO( logInfo.toStdString() );

    m_pReply = m_pNetWorkMgr->post( request(), m_pMultiPart );
    qDebug() << "start post, " + logInfo;

#if QT_NO_DEBUG
#else
    qDebug() << "post step 1 cost " << timer.elapsed();
#endif

    startTimeoutTimer( UPLOAD_TIMEOUT );
}

void CHttpUpLoadMultiCommonScore::setUpLoadMultiScores(QVariantList &scoresList)
{
    m_listMapUpLoadScores.clear();
    m_listMapUpLoadScores = scoresList;
}

void CHttpUpLoadMultiCommonScore::appendUpLoadScore(QVariantMap &score)
{
    m_listMapUpLoadScores << score;
}

void CHttpUpLoadMultiCommonScore::onNewReplyAvailable(QNetworkReply *pReply)
{
    LOGINFO( "开始处理批量common提交..." );
    stopTimer();
    m_listMapFailedScore.clear();

    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }
    m_pMultiPart->setParent( pReply );

    //服务器未接收到, 先判断是不是超时引起的，如果不是，大概率就是网络问题,可能是接口不对，可能是网撇
    if( QNetworkReply::NoError != pReply->error() ){
        LOGERROR( "upload multi common score failed..." );

        QMap< CHANGE_SCORE_TYPE, QVariant > retInfoMap;
        //如果是超时引起的, 就判定为提交成功, 但是要把 m_vecUploadMulRetInfos 里面的 errorCode 置为超时错误码
        if( errorType::TIMEOUT_ERROR == getLocalErrType() ){
#if QT_NO_DEBUG
            LOGERROR( QString( "upload multi common score failed, error string = time out..." ).toStdString() );
#else
            qDebug() << "########CHttpUpLoadMultiCommonScore -> upload multi common score failed, error string = time out...";
#endif
            retInfoMap[ CHANGE_SCORE_TYPE::ErrorCode ] = TIMEOUT;
            retInfoMap[ CHANGE_SCORE_TYPE::ErrorMsg ] = "time out error";
            retInfoMap[ CHANGE_SCORE_TYPE::UpLoadSuccessFlag ] = false;

            setSpecificScoreVecVal( retInfoMap );

            emit sigRequestRetAvailable( true );

            if( nullptr != m_pReply){
                pReply->deleteLater();
                pReply = nullptr;
                m_pReply = nullptr;
                m_pMultiPart = nullptr;
            }

            return;
        }

        //如果不是超时引起的失败, 则设置错误原因
        QString errorString = pReply->errorString();

        if( errorString.isEmpty() ){
            errorString = "########CHttpUpLoadMultiCommonScore -> multi upload common score, UnKnown Error";
        }

        retInfoMap[ CHANGE_SCORE_TYPE::ErrorCode ] = REQUESTERROR;
        retInfoMap[ CHANGE_SCORE_TYPE::ErrorMsg ] = errorString;
        retInfoMap[ CHANGE_SCORE_TYPE::UpLoadSuccessFlag ] = false;

        setSpecificScoreVecVal( retInfoMap );

#if QT_NO_DEBUG
        LOGERROR( errorString.insert( 0, "upload multi score failed, error string = " ).toStdString() );
#else
        qDebug() << "########CHttpUpLoadMultiCommonScore -> upload multi common score failed, " + errorString;
#endif

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
    LOGERROR( "upload multi common score has recvived from server, start to parse json string..." );

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
    QString tmpLoginfo = "########CHttpUpLoadMultiCommonScore ->>>>>>>>>> upload multi common scores, recv string = " + recvString;
    LOGINFO( tmpLoginfo.toStdString() );
#else
    qDebug() << "########CHttpUpLoadMultiCommonScore ->>>>>>>>>> upload multi common scores, recv string = " + recvString;
#endif

    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    LOGINFO( "###################  parse json result = " + QString::number( ok ).toStdString() );
    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadMultiCommonScore::onTimeoutHandler()
{
    LOGERROR( "upload multi score timeout, going to stop count timer..." );
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

void CHttpUpLoadMultiCommonScore::initConfig()
{
    setInterfaceType( requestType::UPLOADMULTICOMMONSCORES );
    setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getStrMultiUpLoadCommonScoreUrl()) );
}

bool CHttpUpLoadMultiCommonScore::isRequestSuccess()
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

    //如果有失败的成绩单元, 根据返回回来的字段  index来判断是哪一条成绩失败
    QVariantMap mapEleFailedUnit;
    QJsonObject tmpObj;
    CMultiUploadRetInfo *eleUploadRetInfo = nullptr;
    QJsonArray failedArray = takeValueOfKeyFromJsonObject( jsObj, REQUEST_VALUE ).toJsonArray();

    int index = -1;

    for( auto &ele : failedArray ){
        tmpObj = ele.toObject();

        //重复上传判定为成功
        if( tmpObj.value( REQUEST_RESULT_KEY ).toVariant().toInt() == REPEATEUPLOAD ){
            continue;
        }

        //获取失败成绩单元的index
        index = tmpObj.value( REQUEST_INDEX ).toInt();

        eleUploadRetInfo = &m_vecUploadMulRetInfos[ index ];
        eleUploadRetInfo->setUpLoadSuccess( false );
        eleUploadRetInfo->setErrorCode( tmpObj.value( REQUEST_RESULT_KEY ).toVariant().toString() );
        eleUploadRetInfo->setErrorMsg( tmpObj.value( REQUEST_MESSAGE ).toString() );
    }

    eleUploadRetInfo = nullptr;
    return false;

}

bool CHttpUpLoadMultiCommonScore::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUpLoadMultiCommonScore::isParameterEmpty()
{
    if( m_listMapUpLoadScores.size() <= 0 ){
        return true;
    }
    return false;
}

void CHttpUpLoadMultiCommonScore::parseParam(DATA_SRC dataSrc)
{
    Q_UNUSED( dataSrc )
    //每解析一次传进来的 m_listMapUpLoadScores 就重置一次
    m_vecUploadMulRetInfos.clear();

    QVariantMap tmpMap;
    QVariantList progressPicsList;
    CMultiUploadRetInfo retInfo;

    tmpMap = m_listMapUpLoadScores.first().toMap();

    //设置成绩数据源和类型
    retInfo.setScoreDataSrc( m_dataSrc );
    retInfo.setUploadType( UPLOAD_TYPE::Upload_Common );

    //保证每一条批量上传的请求中的 设备号(devCode), 设备类型(devType) 和 测试项目(testItem) 是一样的
    //设置设备号
    appendTextPart( upLoadCommonScore::deviceCode, tmpMap.value( upLoadCommonScore::deviceCode ).toString() );

    //设置设备类型
    appendTextPart( upLoadCommonScore::deviceType, tmpMap.value( upLoadCommonScore::deviceType ).toString() );

    //设置测试项目, 并初始化相应index的提交结果( retInfo )信息
    appendTextPart( upLoadCommonScore::testItem, tmpMap.value( upLoadCommonScore::testItem).toString() );
    retInfo.setTestItem( tmpMap.value( upLoadCommonScore::testItem).toInt() );

    LOGINFO( QString( "即将上传 %1条Common成绩" ).arg( QString::number( m_listMapUpLoadScores.size() ) ).toStdString() );

    for( int i = 0; i < m_listMapUpLoadScores.size(); i++){
        tmpMap.clear();
        tmpMap = m_listMapUpLoadScores[ i ].toMap();

        //设置用户ID, 并初始化相应index的提交结果( retInfo )信息
        appendTextPart( uploadMultiScoreKeys::userId.arg( QString::number( i )), tmpMap.value( upLoadCommonScore::userId ).toString() );
        retInfo.setUserId( tmpMap.value( upLoadCommonScore::userId ).toString() );

        //设置成绩类型
        appendTextPart( uploadMultiScoreKeys::scoreType.arg( QString::number( i )),
                        tmpMap.value( upLoadCommonScore::socreType).toString() );

        //设置测试结果
        appendTextPart( uploadMultiScoreKeys::result.arg( QString::number( i )),
                        tmpMap.value( upLoadCommonScore::result).toString() );

        //设置测试时间戳
        appendTextPart( uploadMultiScoreKeys::recordTime.arg( QString::number( i )),
                        tmpMap.value( upLoadCommonScore::recordTime).toString() );

        retInfo.setTimeStamp( tmpMap.value( upLoadCommonScore::recordTime).toString() );

        //追加进提交结果信息, 在提交完成后,只需用按照index 设置相应index对象的 m_upLoadSuccess 属性
        m_vecUploadMulRetInfos << retInfo;
    }
}

void CHttpUpLoadMultiCommonScore::setSpecificScoreVecVal(QMap<CHANGE_SCORE_TYPE, QVariant> &valMap, QList<int> tarIndexs)
{
    //如果list为空, 则表示修改全部
    if( tarIndexs.isEmpty() ){
        for( CMultiUploadRetInfo &ele : m_vecUploadMulRetInfos ){
            if( valMap.contains( CHANGE_SCORE_TYPE::ErrorCode ) ){
                ele.setErrorCode( valMap.value( CHANGE_SCORE_TYPE::ErrorCode ).toString() );
            }

            if( valMap.contains( CHANGE_SCORE_TYPE::ErrorMsg ) ){
                ele.setErrorMsg( valMap.value( CHANGE_SCORE_TYPE::ErrorMsg ).toString() );
            }

            if( valMap.contains( CHANGE_SCORE_TYPE::UpLoadSuccessFlag ) ){
                ele.setUpLoadSuccess( valMap.value( CHANGE_SCORE_TYPE::UpLoadSuccessFlag ).toBool() );
            }
        }
        return;
    }

    //如果不为空, 则只改变对应index的值
    CMultiUploadRetInfo tmpInfo;

    for( int &eleTarIndex : tarIndexs ){
        tmpInfo = m_vecUploadMulRetInfos.at( eleTarIndex );

        if( valMap.contains( CHANGE_SCORE_TYPE::ErrorCode ) ){
            tmpInfo.setErrorCode( valMap.value( CHANGE_SCORE_TYPE::ErrorCode ).toString() );
        }

        if( valMap.contains( CHANGE_SCORE_TYPE::ErrorMsg ) ){
            tmpInfo.setErrorMsg( valMap.value( CHANGE_SCORE_TYPE::ErrorMsg ).toString() );
        }

        if( valMap.contains( CHANGE_SCORE_TYPE::UpLoadSuccessFlag ) ){
            tmpInfo.setUpLoadSuccess( valMap.value( CHANGE_SCORE_TYPE::UpLoadSuccessFlag ).toBool() );
        }
    }

}

void CHttpUpLoadMultiCommonScore::setDataSrc(const DATA_SRC &dataSrc)
{
    m_dataSrc = dataSrc;
}

QVector<CMultiUploadRetInfo> CHttpUpLoadMultiCommonScore::getVecUploadMulRetInfos() const
{
    return m_vecUploadMulRetInfos;
}

QString CHttpUpLoadMultiCommonScore::lastErrorString() const
{
    return m_lastErrorString;
}
