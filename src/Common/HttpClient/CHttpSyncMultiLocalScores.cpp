#include "CHttpSyncMultiLocalScores.h"

CHttpSyncMultiLocalScores::CHttpSyncMultiLocalScores(QObject *parent) : QObject(parent)
{
    init();
    initTimer();
}

void CHttpSyncMultiLocalScores::setCommonScoreList( QVariantList &list)
{
    //    m_pSyncCommonScores->setUpLoadMultiScores( list );
    if( list.isEmpty() ){
        return;
    }

    m_tmpCommonMutex.lock();
    m_tmpCommonTestItemScoresList.append( list );

    m_tmpCommonMutex.unlock();
    if( !m_commonCommitTimer.isActive() ){
        m_commonCommitTimer.start();
    }
}

void CHttpSyncMultiLocalScores::setBodyScoreList( QVariantList &list)
{
    m_pSyncBodyScores->setUpLoadMultiScores( list );
}

void CHttpSyncMultiLocalScores::setEyeSightScoreList( QVariantList &list)
{
    m_pSyncEyeSightScores->setUpLoadMultiScores( list );
}

void CHttpSyncMultiLocalScores::startSync(const QString serverTokenString)
{
    m_vecTotalScoreRetInfos.clear();

    m_pSyncCommonScores->setServerTokenString( serverTokenString );
    m_pSyncBodyScores->setServerTokenString( serverTokenString );
    m_pSyncEyeSightScores->setServerTokenString( serverTokenString );

    m_pSyncCommonScores->httpRequest();
    m_pSyncBodyScores->httpRequest();
    m_pSyncEyeSightScores->httpRequest();
}

void CHttpSyncMultiLocalScores::onSyncCommonScoresFinished(bool ok)
{
#if QT_NO_DEBUG
#else
    qDebug() << "sync common ret = " << ok ;
#endif

    //同步完成一条 后，继续开启定时器
    if( !m_commonCommitTimer.isActive() ){
        m_commonCommitTimer.start();
    }

    //如果失败, 则将错误信息提取并传出去, 不论成功失败，步骤都加一
    emit sigSyncFinished( m_pSyncCommonScores->getVecUploadMulRetInfos() );     //onSyncMultiLocalScoresFinished
}

void CHttpSyncMultiLocalScores::onSyncBodyScoresFinished(bool ok)
{
#if QT_NO_DEBUG
#else
    qDebug() << "sync Body ret = " << ok ;
#endif
    emit sigSyncFinished( m_pSyncBodyScores->getVecUploadMulRetInfos() );
}

void CHttpSyncMultiLocalScores::onSyncEyeSightScoresFinished(bool ok)
{
#if QT_NO_DEBUG
#else
    qDebug() << "sync eye sight ret = " << ok ;
#endif
    emit sigSyncFinished( m_pSyncEyeSightScores->getVecUploadMulRetInfos() );
}

void CHttpSyncMultiLocalScores::onSyncTimerTimeout()
{
    m_tmpCommonMutex.lock();
    //如果待上传的缓存为空，则直接退出
    if( m_tmpCommonTestItemScoresList.isEmpty() ){
        m_tmpCommonMutex.unlock();
        return;
    }

    qDebug() << "###############################################" << "size = " << m_tmpCommonTestItemScoresList.size();

    //如果待上传的缓存区不为空，则取出第一个
    m_pSyncCommonScores->setUpLoadMultiScores( QVariantList() << m_tmpCommonTestItemScoresList.takeFirst() );
    m_tmpCommonMutex.unlock();

    m_commonCommitTimer.stop();
    m_pSyncCommonScores->httpRequest();

}

void CHttpSyncMultiLocalScores::init()
{
    if( nullptr == m_pSyncCommonScores ){
        m_pSyncCommonScores = new CHttpUpLoadMultiCommonScore( this );
    }

    if( nullptr == m_pSyncBodyScores ){
        m_pSyncBodyScores = new CHttpUpLoadMultiBodyScores( this );
    }

    if( nullptr == m_pSyncEyeSightScores ){
        m_pSyncEyeSightScores = new CHttpUpLoadMultiEyeSightScores( this );
    }

    m_pSyncCommonScores->setDataSrc( DATA_SRC::DataBase );
    m_pSyncBodyScores->setDataSrc( DATA_SRC::DataBase );
    m_pSyncEyeSightScores->setDataSrc( DATA_SRC::DataBase );

    initConnection();
}

void CHttpSyncMultiLocalScores::initTimer()
{
    m_commonCommitTimer.setInterval( 1000 );
    connect( &m_commonCommitTimer, &QTimer::timeout, this, &CHttpSyncMultiLocalScores::onSyncTimerTimeout, Qt::QueuedConnection );
}

void CHttpSyncMultiLocalScores::timerSwitch(const bool start)
{
    if( start ){
        m_commonCommitTimer.start();
    }else{
        m_commonCommitTimer.stop();
    }
}

void CHttpSyncMultiLocalScores::initConnection()
{
    connect( m_pSyncCommonScores, &CHttpUpLoadMultiCommonScore::sigRequestRetAvailable,
             this, &CHttpSyncMultiLocalScores::onSyncCommonScoresFinished, Qt::QueuedConnection );

    connect( m_pSyncBodyScores, &CHttpUpLoadMultiBodyScores::sigRequestRetAvailable,
             this, &CHttpSyncMultiLocalScores::onSyncBodyScoresFinished, Qt::QueuedConnection );

    connect( m_pSyncEyeSightScores, &CHttpUpLoadMultiEyeSightScores::sigRequestRetAvailable,
             this, &CHttpSyncMultiLocalScores::onSyncEyeSightScoresFinished, Qt::QueuedConnection );
}

CHttpUpLoadMultiEyeSightScores *CHttpSyncMultiLocalScores::pSyncEyeSightScores() const
{
    return m_pSyncEyeSightScores;
}

CHttpUpLoadMultiCommonScore *CHttpSyncMultiLocalScores::pSyncCommonScores() const
{
    return m_pSyncCommonScores;
}

CHttpUpLoadMultiBodyScores *CHttpSyncMultiLocalScores::pSyncBodyScores() const
{
    return m_pSyncBodyScores;
}

void CHttpSyncMultiLocalScores::appendSyncSetp()
{


}

int CHttpSyncMultiLocalScores::syncSetp()
{
    m_stepLocker.lock();
    int ret = m_syncSetp;
    m_stepLocker.unlock();

    return ret;
}
