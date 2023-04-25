#include "CHttpSyncLocalScores.h"

CHttpSyncLocalScores::CHttpSyncLocalScores()
{
    m_pUploadCommonScore   = new CHttpUpLoadCommonScore( this );
    m_pUploadBodyScore     = new CHttpUpLoadBodyScore( this );
    m_pUploadEyeSightScore = new CHttpUpLoadEyeSightScore( this );
    init();
}

CHttpSyncLocalScores::~CHttpSyncLocalScores()
{

}

void CHttpSyncLocalScores::setVecCommonScore(const QVector<QMap<QString, QVariant> > &vecCommonScore)
{
    m_vecCommonScore = vecCommonScore;
    m_uploadCommonTotalCount = m_vecCommonScore.size();
}

void CHttpSyncLocalScores::setVecBodyScore(const QVector<QMap<QString, QVariant> > &vecBodyScore)
{
    m_vecBodyScore = vecBodyScore;
    m_uploadBodyTotalCount = m_vecBodyScore.size();
}

void CHttpSyncLocalScores::setVecEyeSightScore(const QVector<QMap<QString, QVariant> > &vecEyeSightScore)
{
    m_vecEyeSightScore = vecEyeSightScore;
    m_uploadEyeSightTotalCount = m_vecEyeSightScore.size();
}

void CHttpSyncLocalScores::startSync()
{
    //同时开启三个递归
    startSyncCommonScore();

    startSyncBodyScore();

    startSyncEyeSightScore();

}

void CHttpSyncLocalScores::onSyncCommonFinished(bool ok)
{
    //得到请求结果后, 首先清除vec中的第一个数据
    if( !m_vecCommonScore.isEmpty() ){
        m_vecCommonScore.removeFirst();
    }

    QString userId = m_mapCurCommonScore.value( HTTP_KEYS::userId ).toString();
    QString recordTime = m_mapCurCommonScore.value( HTTP_KEYS::recordTime ).toString();
    QString testItem = m_mapCurCommonScore.value( HTTP_KEYS::testItem ).toString();

    int requestRetCode = m_pUploadBodyScore->getRequestRetCode();

    //    QString logInfo = QString( "sync ret = %1, userId = %2, recordTime = %3, testItem = %4" ).arg( QString::number( ok ), userId, recordTime, testItem );
    //    qDebug() << logInfo;
    if( !ok ){
        if( nullptr != m_pUploadCommonScore ){
            qDebug() << "error string = " << m_pUploadCommonScore->getErrorString();
        }
    }
    //不论是否上传成功,都通过信号发出去
    CHttpUploadInfos infos = getUploadInfos( DATA_SRC::DataBase, UPLOAD_TYPE::Upload_Common, ok, requestRetCode );
    QVariant data = CHttpRequestMgr::upLoadInfos2Variant( infos );
    emit sigSingleSyncResultAvailable( data, UPLOAD_TYPE::Upload_Common );
    qDebug() << "vec common size = " << m_vecCommonScore.size();


    startSyncCommonScore();
}

void CHttpSyncLocalScores::onSyncBodyFinished(bool ok)
{
    //得到请求结果后, 首先清除vec中的第一个数据
    if( !m_vecBodyScore.isEmpty() ){
        m_vecBodyScore.removeFirst();
    }

    int requestRetCode = m_pUploadBodyScore->getRequestRetCode();

    QString userId = m_mapCurBodyScore.value( HTTP_KEYS::userId ).toString();
    QString recordTime = m_mapCurBodyScore.value( HTTP_KEYS::recordTime ).toString();
    QString testItem = m_mapCurBodyScore.value( UPLOADBODYSCORE_KEY_HEIGHT ).toString();

    //    QString logInfo = QString( "sync ret = %1, userId = %2, recordTime = %3, testItem = %4" ).arg( QString::number( ok ), userId, recordTime, testItem );
    //    qDebug() << logInfo;
    CHttpUploadInfos infos = getUploadInfos( DATA_SRC::DataBase, UPLOAD_TYPE::Upload_Body, ok, requestRetCode );
    QVariant data = CHttpRequestMgr::upLoadInfos2Variant( infos );
    emit sigSingleSyncResultAvailable( data, UPLOAD_TYPE::Upload_Body );
    qDebug() << "local body scores remain = " << m_vecBodyScore.size();

    //开始递归
    startSyncBodyScore();
}

void CHttpSyncLocalScores::onSyncEyeSightFinished(bool ok)
{
    //得到请求结果后, 首先清除vec中的第一个数据
    if( !m_vecEyeSightScore.isEmpty() ){
        m_vecEyeSightScore.removeFirst();
    }

    QString userId = m_mapCurCommonScore.value( HTTP_KEYS::userId ).toString();
    QString recordTime = m_mapCurCommonScore.value( HTTP_KEYS::recordTime ).toString();
    QString testItem = m_mapCurCommonScore.value( HTTP_KEYS::testItem ).toString();

    int requestRetCode = m_pUploadBodyScore->getRequestRetCode();

    //    QString logInfo = QString( "sync ret = %1, userId = %2, recordTime = %3, testItem = %4" ).arg( QString::number( ok ), userId, recordTime, testItem );
    //    qDebug() << logInfo;

    CHttpUploadInfos infos = getUploadInfos( DATA_SRC::DataBase, Upload_EyeSight, ok, requestRetCode );
    QVariant data = CHttpRequestMgr::upLoadInfos2Variant( infos );
    emit sigSingleSyncResultAvailable( data, Upload_EyeSight );
    qDebug() << "local eyeSight scores remain = " << m_vecEyeSightScore.size();

    startSyncEyeSightScore();
}

void CHttpSyncLocalScores::init()
{
    initConnection();
}

void CHttpSyncLocalScores::initConnection()
{
    connect( m_pUploadCommonScore, &CHttpUpLoadCommonScore::sigRequestRetAvailable,
             this, &CHttpSyncLocalScores::onSyncCommonFinished, Qt::QueuedConnection );

    connect( m_pUploadBodyScore, &CHttpUpLoadBodyScore::sigRequestRetAvailable,
             this, &CHttpSyncLocalScores::onSyncBodyFinished, Qt::QueuedConnection );

    connect( m_pUploadEyeSightScore, &CHttpUpLoadEyeSightScore::sigRequestRetAvailable,
             this, &CHttpSyncLocalScores::onSyncEyeSightFinished, Qt::QueuedConnection );
}

void CHttpSyncLocalScores::startSyncCommonScore()
{
    if( m_vecCommonScore.isEmpty() || nullptr == m_pUploadCommonScore ){
        return;
    }
    m_mapCurCommonScore = m_vecCommonScore.first();
    m_pUploadCommonScore->setParaMap( m_mapCurCommonScore );
    m_pUploadCommonScore->httpRequest();
}

void CHttpSyncLocalScores::startSyncBodyScore()
{
    if( m_vecBodyScore.isEmpty() || nullptr == m_pUploadBodyScore ){
        return;
    }

    m_mapCurBodyScore = m_vecBodyScore.first();
    m_pUploadBodyScore->setParaMap( m_mapCurBodyScore );
    m_pUploadBodyScore->httpRequest();
}

void CHttpSyncLocalScores::startSyncEyeSightScore()
{
    if( m_vecEyeSightScore.isEmpty() || nullptr == m_pUploadEyeSightScore ){
        return;
    }

    m_mapCurEyeSightScore = m_vecEyeSightScore.first();
    m_pUploadEyeSightScore->setParaMap( m_mapCurEyeSightScore );
    m_pUploadEyeSightScore->httpRequest();
}

CHttpUploadInfos CHttpSyncLocalScores::getUploadInfos(DATA_SRC dataSrc, UPLOAD_TYPE type, bool ok, int requestRetCode )
{
    CHttpUploadInfos infos;
    if( type == UPLOAD_TYPE::Upload_Common ){
        infos.m_userId         = m_mapCurCommonScore.value( HTTP_KEYS::userId ).toString();
        infos.m_testItem       = m_mapCurCommonScore.value( HTTP_KEYS::testItem ).toString();
        infos.m_recordTime     = m_mapCurCommonScore.value( HTTP_KEYS::recordTime ).toString();
        infos.m_isFinished     = m_vecCommonScore.size() > 0 ? false : true;
        infos.m_isSuccess      = ok;
        infos.m_dataSrc        = static_cast<short>(dataSrc);
        infos.m_testRet        = m_mapCurCommonScore.value( HTTP_KEYS::result ).toString();
        infos.m_commitProgress = QString( "%1 / %2" ).arg( QString::number( m_uploadCommonTotalCount - m_vecCommonScore.size() ), QString::number( m_uploadCommonTotalCount ) );
    }

    if( type == UPLOAD_TYPE::Upload_Body ){
        infos.m_userId         = m_mapCurBodyScore.value( HTTP_KEYS::userId ).toString();
        infos.m_testItem       = QString::number( HTTP_TEST_ITEM_HEIGHT );
        infos.m_recordTime     = m_mapCurBodyScore.value( HTTP_KEYS::recordTime ).toString();
        infos.m_isFinished     = m_vecBodyScore.size() > 0 ? false : true;
        infos.m_isSuccess      = ok;
        infos.m_dataSrc        = static_cast<short>(dataSrc);
        infos.m_testRet        = m_mapCurBodyScore.value( HTTP_KEYS::result ).toString();
        infos.m_commitProgress = QString( "%1 / %2" ).arg( QString::number( m_uploadBodyTotalCount - m_vecBodyScore.size() ), QString::number( m_uploadBodyTotalCount ));
    }

    if( type == Upload_EyeSight ){
        infos.m_userId         = m_mapCurEyeSightScore.value( HTTP_KEYS::userId ).toString();
        infos.m_testItem       = QString::number( HTTP_TEST_ITEM_VISION_LEFT );
        infos.m_recordTime     = m_mapCurEyeSightScore.value( HTTP_KEYS::recordTime ).toString();
        infos.m_isFinished     = m_vecEyeSightScore.size() > 0 ? false : true;
        infos.m_isSuccess      = ok;
        infos.m_dataSrc        = static_cast<short>(dataSrc);
        infos.m_testRet        = m_mapCurEyeSightScore.value( HTTP_KEYS::result ).toString();
        infos.m_commitProgress = QString( "%1 / %2" ).arg( QString::number( m_uploadEyeSightTotalCount - m_vecEyeSightScore.size() ), QString::number( m_uploadEyeSightTotalCount ) );
    }

    infos.m_isServerOnline = CHttpRequestMgr::getInstance()->getBIsOnline();
    infos.requestCode = requestRetCode;

    return infos;
}
