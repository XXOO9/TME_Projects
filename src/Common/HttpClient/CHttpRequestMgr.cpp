#include "CHttpRequestMgr.h"
#include <CTestResultInfo.h>
#include <QMetaType>
#include "../common/Config/CDatabaseManage.h"

CHttpRequestMgr::CHttpRequestMgr(QObject *parent) : QObject(parent)
  ,m_pGetVideoUrl( nullptr )
  ,m_pSyncScoreStd( nullptr )
  ,m_pSyncUserInfo( nullptr )
  ,m_pSyncClassInfo( nullptr )
  ,m_pUserIdentify( nullptr )
  ,m_pHeartBeat( nullptr )
  ,m_pUploadCommonScore( nullptr )
  ,m_pUploadBodyScore( nullptr )
  ,m_pUploadEyeSightScore( nullptr )
  ,m_pTouristRecord( nullptr )
  ,m_pWorkThread( nullptr )
  ,m_pTimer( nullptr )
{
    qRegisterMetaType<CHttpUploadInfos>("CHttpUploadInfos");//注册自定义类型信号槽
    qRegisterMetaType<CHttpUploadInfos>("CHttpUploadInfos&");//注册自定义类型的引用

    init();
    qDebug() << "CHttpRequestMgr thread ID = " << QThread::currentThreadId();
}

void CHttpRequestMgr::init()
{
    initThread();
    initConnection();

    QString account = CDatabaseManage::GetInstance().getServerFunConfig().getTokenAccount();
    QString passwd  = CDatabaseManage::GetInstance().getServerFunConfig().getTokenPasswd();

    getServerTokenInfo( account, passwd );

    emit insideSigInitMemberConnection();
}

void CHttpRequestMgr::initThread()
{
    if( nullptr == m_pWorkThread ){
        m_pWorkThread = new QThread();
    }

    this->moveToThread( m_pWorkThread );
    m_pWorkThread->start();

    qDebug() << "2 init thread function Thread ID = " << QThread::currentThreadId();
}

CHttpRequestMgr *CHttpRequestMgr::getInstance()
{
    if( nullptr == m_pHttpRequestMgr ){
        m_locker.lock();
        if( nullptr == m_pHttpRequestMgr ){
            m_pHttpRequestMgr = new CHttpRequestMgr();
        }
        m_locker.unlock();
    }
    return m_pHttpRequestMgr;
}

CHttpRequestMgr::~CHttpRequestMgr()
{
    if( nullptr != m_pGetVideoUrl ){
        m_pGetVideoUrl->deleteLater();
        m_pGetVideoUrl = nullptr;
    }

    if( nullptr != m_pSyncScoreStd ){
        m_pSyncScoreStd->deleteLater();
        m_pSyncScoreStd = nullptr;
    }

    if( nullptr != m_pSyncUserInfo ){
        m_pSyncUserInfo->deleteLater();
        m_pSyncUserInfo = nullptr;
    }

    if( nullptr != m_pSyncClassInfo ){
        m_pSyncClassInfo->deleteLater();
        m_pSyncClassInfo = nullptr;
    }

    if( nullptr != m_pUserIdentify ){
        m_pUserIdentify->deleteLater();
        m_pUserIdentify = nullptr;
    }
    if( nullptr != m_pHeartBeat ){
        m_pHeartBeat->deleteLater();
        m_pHeartBeat = nullptr;
    }

    if( nullptr != m_pUploadCommonScore ){
        m_pUploadCommonScore->deleteLater();
        m_pUploadCommonScore = nullptr;
    }

    if( nullptr != m_pUploadBodyScore ){
        m_pUploadBodyScore->deleteLater();
        m_pUploadBodyScore = nullptr;
    }

    if( nullptr != m_pUploadEyeSightScore ){
        m_pUploadEyeSightScore->deleteLater();
        m_pUploadEyeSightScore = nullptr;
    }

    if( nullptr != m_pTouristRecord ){
        m_pTouristRecord->deleteLater();
        m_pTouristRecord = nullptr;
    }

    if( nullptr != m_pWorkThread ){
        connect( m_pWorkThread, &QThread::finished, [=](){
            m_pWorkThread->deleteLater();
            qDebug() << "work thread is delete...";
        } );
        if( m_pWorkThread->isRunning() ){
            m_pWorkThread->quit();
            m_pWorkThread->wait();
        }
        //        m_pWorkThread = nullptr;
    }

    if( nullptr != m_pTimer ){
        if( m_pTimer->isActive() ){
            m_pTimer->stop();
        }
        delete m_pTimer;
        m_pTimer = nullptr;
    }
}

void CHttpRequestMgr::getServerTokenInfo(const QString &userName, const QString &passwd)
{
    QVariantMap logIngoMap = { { getServerTokenKeys::userName, userName },
                               { getServerTokenKeys::password, passwd }
                             };

    emit insideSigStartGetServerToken( logIngoMap );    //onStartGetServerToken



}

void CHttpRequestMgr::syncMultiLocalScores(QVariantList &commonList, QVariantList &bodyList, QVariantList &eyeSightList)
{
    emit insideSigSyncMultiLcoalScores( commonList, bodyList, eyeSightList ); //onSyncMultiLocalScores
}

void CHttpRequestMgr::uploadMultiCommonScores( const QVariantList &scoreList )
{
    emit insideSigUploadMultiCommonScores( scoreList );    //onUploadMultiCommonScores
}

void CHttpRequestMgr::uploadMultiBodyScores(const QVariantList &scoreList )
{
    emit insideSigUploadMultiBodyScores( scoreList );   //onUploadMultiBodyScores
}

void CHttpRequestMgr::sendMultiHeartBeat( const QVariantList &heartBeatList )
{
    emit insideSigSendMultiHeartBeat( heartBeatList );  //onSendMultiHeartBeat
}

void CHttpRequestMgr::uploadMultiEyeSightScores(const QVariantList &scoreList)
{
    emit insideSigUploadMultiEyeSightScores( scoreList );
}

void CHttpRequestMgr::getSampleVideoUrls(QString timeStamp)
{
    qDebug() << "getSampleVideoUrls emit sig thread ID = " << QThread::currentThreadId();

    emit insideSigGetVideoUrls( timeStamp );
}

void CHttpRequestMgr::syncScoreStd()
{
    qDebug() << "syncScoreStd emit sig thread ID = " << QThread::currentThreadId();
    emit insideSigSyncScoreStd();
}

void CHttpRequestMgr::syncUserInfo(QString timeStamp, FACE_ENGINE faceEngine)
{
    int faceEngineType = static_cast<int>( faceEngine );
    emit insideSigSyncUserInfo( timeStamp, faceEngineType );      //onSyncUserInfo
}

void CHttpRequestMgr::syncClassInfo(QString timeStamp)
{
    emit insideSigSyncClassInfo( timeStamp );   //onSyncClassInfo
}

void CHttpRequestMgr::userIdentify(QString faceFeature, FACE_ENGINE faceEngine, QString cardNo, QString code)
{
    int tmpFaceEngine = static_cast<int>( faceEngine );
    emit insideSigUserIdentify( faceFeature, tmpFaceEngine, cardNo, code ); //onUserIdentify
}

void CHttpRequestMgr::sendHeartBeat(devStatus status, QString battery, QString devCode, short devType)
{
    qDebug() << "sendHeartBeat emit sig thread ID = " << QThread::currentThreadId();
    int tmpStatus = int(status);

    emit insideSigSendHeartBeat(tmpStatus, battery, devCode, devType );
}

void CHttpRequestMgr::upLoadCommonScore(const QMap<QString, QVariant> &infos)
{
    QString timeStamp = infos.value( HTTP_KEYS::recordTime ).toString();
    QString userId    = infos.value( HTTP_KEYS::userId ).toString();
    LOGINFO( "***********************each record*****************************");
    LOGINFO( "upload keys -> recordTime = " + timeStamp.toStdString() );
    LOGINFO("upload keys -> userId = " + userId.toStdString() );
    QVariant data;
    data.setValue( infos );
    emit insideSigUploadCommonScore( data );    //onUploadCommonScore
}

void CHttpRequestMgr::upLoadCommonScore(const QVector<QMap<QString, QVariant> > &vecInfos)
{
    QVariant data;
    data.setValue( vecInfos );
    emit insideSigUploadCommonScoreByVector(data);  //onUploadCommonScoreByVector
}

void CHttpRequestMgr::upLoadBodyScore(const QMap<QString, QVariant> &infos)
{
    QVariant data;
    data.setValue( infos );
    emit insideSigUploadBodyScore( data );
}

void CHttpRequestMgr::upLoadEyeSightScore(const QMap<QString, QVariant> &infos)
{
    QVariant data;
    data.setValue( infos );
    emit insideSigUploadEyeSightScore( data );
}

void CHttpRequestMgr::touristUsageRecord(const QString &devCode, const short &devType, const QString &timeStamp )
{
    emit insideSigToutisUsageRecord( devCode, devType, timeStamp);
}

void CHttpRequestMgr::upLoadLocalScore()
{
    //通过心跳验证服务器连接后, 判断存储成绩的vec是否为空, 如果为空则跳过
    if( m_vecUnUploadCommonScore.isEmpty() && m_vecUnUploadBodyScore.isEmpty() && m_vecUnUploadEyesightScore.isEmpty() ){
        return;
    }
}

void CHttpRequestMgr::queryHistoryScore(QVariantMap params)
{
    emit insideSigQueryHistoryScore( params );  // onQueryHistoryScore
}


void CHttpRequestMgr::onGetSampleVideoUrls(QString timeStamp)
{
    //检查网络是否在线
    if( !m_bIsOnline ){
        qDebug() << "net work is not available...";
        LOGINFO( "get sample video urls failed, network is not available..." );
        return;
    }
    qDebug() << "onGetSampleVideoUrls thread ID = " << QThread::currentThreadId();
    if( nullptr == m_pGetVideoUrl ){
        m_pGetVideoUrl = new CHttpGetVideoUrls( timeStamp, this );
    }

    m_pGetVideoUrl->setServerTokenString( getServerTokenString() );

    connect( m_pGetVideoUrl, &CHttpGetVideoUrls::sigRequestRetAvailable, this, &CHttpRequestMgr::onGetSampleVideoUrlsFinished, Qt::UniqueConnection );

    m_pGetVideoUrl->httpRequest();
}

void CHttpRequestMgr::onSyncScoreStandard()
{
    //检查网络是否在线
    if( !m_bIsOnline ){
        qDebug() << "net work is not available...";
        LOGINFO( "Sync Score Standard failed, network is not available..." );
        return;
    }
    qDebug() << "sync score std thread ID = " << QThread::currentThreadId();
    if( nullptr == m_pSyncScoreStd ){
        m_pSyncScoreStd = new CHttpSyncScoreStd( this );
    }

    m_pSyncScoreStd->setServerTokenString( getServerTokenString() );

    connect( m_pSyncScoreStd, &CHttpSyncScoreStd::sigRequestRetAvailable, this, &CHttpRequestMgr::onSyncScoreStandardFinished, Qt::UniqueConnection );

    m_pSyncScoreStd->httpRequest();
}

void CHttpRequestMgr::onSyncUserInfo(QString timeStamp, int faceEngine )
{
    //检查网络是否在线
    if( !m_bIsOnline ){
        qDebug() << "net work is not available...";
        LOGINFO( "Sync UserInfo failed, network is not available..." );
        return;
    }
    FACE_ENGINE engineType = static_cast<FACE_ENGINE>(faceEngine);
    qDebug() << "onSyncUserInfo thread ID = " << QThread::currentThreadId();
    if( nullptr == m_pSyncUserInfo ){
        m_pSyncUserInfo = new CHttpSyncUserInfo( timeStamp, engineType, this );
    }else{
        m_pSyncUserInfo->setCurMaxTimeStamp( timeStamp );
    }

    QString schoolID = CHttpRequestMgr::getInstance()->getPMultiHeartBeatInfos()->schoolId();
    m_pSyncUserInfo->setSchoolID( schoolID );
    m_pSyncUserInfo->setServerTokenString( getServerTokenString() );


    connect( m_pSyncUserInfo, &CHttpSyncUserInfo::sigRequestRetAvailable,
             this, &CHttpRequestMgr::onSyncUserInfoFinished, Qt::UniqueConnection );

    m_pSyncUserInfo->httpRequest();
}

void CHttpRequestMgr::onSyncClassInfo(QString timeStamp)
{
    //检查网络是否在线
    if( !m_bIsOnline ){
        qDebug() << "net work is not available...";
        LOGINFO("sync class info failed, network is not available...");
        return;
    }

    if( nullptr == m_pSyncClassInfo ){
        m_pSyncClassInfo = new CHttpSyncClassInfo(timeStamp, this);
    }

    m_pSyncClassInfo->setServerTokenString( getServerTokenString() );

    connect( m_pSyncClassInfo, &CHttpSyncClassInfo::sigRequestRetAvailable, this, &CHttpRequestMgr::onSyncClassInfoFinished, Qt::UniqueConnection );

    m_pSyncClassInfo->httpRequest();
}

void CHttpRequestMgr::onUserIdentify(QString faceFeature, int faceEngine, QString cardNo, QString code)
{
    //检查网络是否在线
    if( !m_bIsOnline ){
        qDebug() << "net work is not available...";
        LOGINFO("user indentify failed, network is not available...");
        return;
    }

    qDebug() << "onUserIdentify thread ID = " << QThread::currentThreadId();

    FACE_ENGINE tmpFaceEngine = static_cast<FACE_ENGINE>(faceEngine);

    if( nullptr == m_pUserIdentify ){
        m_pUserIdentify = new CHttpUserIdentify( faceFeature, tmpFaceEngine, cardNo, code, this );
    }
    else {
        m_pUserIdentify->setIndentifyCondition(faceFeature, cardNo, TMEc_Soft, code );
    }


    m_pUserIdentify->setServerTokenString( getServerTokenString() );

    connect( m_pUserIdentify, &CHttpUserIdentify::sigRequestRetAvailable, this, &CHttpRequestMgr::onUserIdentifyFinished, Qt::UniqueConnection );

    m_pUserIdentify->httpRequest();
}

void CHttpRequestMgr::onSendHeartBeat(int status, QString battery, QString devCode, short devType)
{
    qDebug() << "onSendHeartBeat thread ID = " << QThread::currentThreadId();

    //定时器的初始化放这里是因为，该函数通过信号触发，在子线程中运行，如果定时器放在init()函数中初始化，
    //则定时器m_timer则是属于主线程
    //并不会触发timeout槽函数
    initTimer();
    devStatus tmpStatus  = devStatus(status);
    if( nullptr == m_pHeartBeat ){
        m_pHeartBeat = new CHttpHeartBeat( this );
    }

    connect( m_pHeartBeat, &CHttpHeartBeat::sigRequestRetAvailable, this, &CHttpRequestMgr::onHeartBeatSendFinished, Qt::UniqueConnection );

    m_pHeartBeat->setServerTokenString( getServerTokenString() );

    m_pHeartBeat->setStatus( tmpStatus );
    m_pHeartBeat->setBattery( battery );
    m_pHeartBeat->setDevCode( devCode );
    m_pHeartBeat->setDevType( devType );

    m_pHeartBeat->httpRequest();
}

//上传通用成绩
void CHttpRequestMgr::onUploadCommonScore(QVariant data)
{
    //0.检查是否联网，未联网则直接返回成功
    if( !m_bIsOnline )
    {
        LOGINFO("Upload CommonScore failed, network is not available...");
        m_vecUnUploadCommonScore.removeFirst();
        m_curCommonUploadInfos = getUploadInfos( DATA_SRC::UserExam, UPLOAD_TYPE::Upload_Common, true, OFFLINE );
        QVariant data = upLoadInfos2Variant( m_curCommonUploadInfos );
        emit sigUploadCommonScore( data );
        return;
    }

    //1.构建连接HTTP对象(一个对象仅进行一次上传)
    QMap<QString, QVariant> infos = data.value< QMap<QString, QVariant> >();
    if( nullptr == m_pUploadCommonScore )
    {
        m_pUploadCommonScore = new CHttpUpLoadCommonScore( this );
    }

    //2.响应服务器返回信息(不论成功失败均有返回值)
    connect( m_pUploadCommonScore, &CHttpUpLoadCommonScore::sigRequestRetAvailable,
             this, &CHttpRequestMgr::onUploadCommonScoreFinished, Qt::UniqueConnection );

    //2022/07/06修改，不使用lambda表达式绑定信号
    /*

    connect( m_pUploadCommonScore, &CHttpUpLoadCommonScore::sigRequestRetAvailable, [=](bool ok){
        //2.1 安全保护(接收到返回前都是不允许提交的)
        //注：避免界面锁死，就算出错也必须给界面返回
        if(m_isRequestFinished)
        {
            m_errorString = "Can't allowed receive common score return info";
            LOGERROR("Can't allowed receive common score return info");
            //            m_curCommonUploadInfos = getUploadInfos( UserExam, Upload_Common, false );
            //这个信号只是为了出现异常情况的时候能够关闭界面上提交中的弹窗
            //            emit sigUploadCommonScore( m_curCommonUploadInfos );
            return ;
        }

        //获取请求结果码，判断是不是超时引起的请求失败
        int requestRetCode = m_pUploadCommonScore->getRequestRetCode();

        //2.2 提交失败后赋值错误信息，用于界面显示
        if( !ok )
        {
            m_errorString = m_pUploadCommonScore->getErrorString();
            qDebug() << "error string = " << m_errorString;
            QString userId = m_vecUnUploadCommonScore.first().value( HTTP_KEYS::userId ).toString();
            QString testItem = m_curCommonUploadInfos.m_testItem;
            QString recordTime = infos.value( HTTP_KEYS::recordTime ).toString();
            QString dataBaseTime = QString::fromStdString( CTestResultInfo::getSaveTimeStamp(recordTime) );
            LOGINFO("upload common socre failed, userid = " + userId.toStdString() + "erorr string = " + m_errorString.toStdString() );
            QString logInfo = QString( "failed userid = %1, testItem = %2, recordtime = %3" ).arg( userId, testItem, dataBaseTime );
            LOGINFO( logInfo.toStdString() );
        }


        //2.3 释放HTTP对象资源
        if( nullptr != m_pUploadCommonScore )
        {
            delete m_pUploadCommonScore;
            m_pUploadCommonScore = nullptr;
        }

        //2.4 移除成绩提交存储
        if(!m_vecUnUploadCommonScore.isEmpty())
        {
            m_vecUnUploadCommonScore.removeFirst();
        }

        //2.5 修改标志位
        m_isRequestFinished = true;

        //在此处组装当前提交结果信息，发送至CTestGuideManager::onUploadCommonScoreFinished中， 做出相应的处理
        m_curCommonUploadInfos = getUploadInfos( UserExam, Upload_Common, ok, requestRetCode );
        QVariant data = upLoadInfos2Variant( m_curCommonUploadInfos );
        emit sigUploadCommonScore( data );

        //2.6 递归提交(此逻辑下一次触发只会发送一条消息)
        onUploadCommon();
    });
         */

    //3.发送请求保护(下一条请求必须在接收后才允许发送)
    if(!m_isRequestFinished)
    {
        m_errorString = "Can't allowed send common score";
        LOGERROR("Can't allowed send common score");
        //        emit sigUploadCommonScore( false );
        return ;
    }

    //4.修改标志位
    m_isRequestFinished = false;

    m_pUploadCommonScore->setServerTokenString( getServerTokenString() );

    //5.组装信息，发送请求
    m_pUploadCommonScore->setParaMap( infos );
    m_pUploadCommonScore->httpRequest();
}

void CHttpRequestMgr::onUploadCommonScoreByVector(QVariant data)
{
    qDebug() << "onUploadCommonScoreByVector thrad ID = " << QThread::currentThreadId();
    //    QVector<QMap<QString, QVariant> > totalInfos = data.value<QVector<QMap<QString, QVariant>>>();

    if( m_vecUnUploadCommonScore.size() <= 0 ){
        return;
    }
    upLoadCommonScore( m_vecUnUploadCommonScore.first() );
}

//上传身高体重成绩
void CHttpRequestMgr::onUpLoadBodyScore(QVariant data)
{
    //0.检查是否联网，未联网则直接返回成功
    if( !m_bIsOnline )
    {
        LOGINFO("upload body score failed, network is not available...");
        m_vecUnUploadBodyScore.removeFirst();
        m_curBodyUploadInfos = getUploadInfos( DATA_SRC::UserExam, UPLOAD_TYPE::Upload_Body, true, OFFLINE );
        QVariant data = upLoadInfos2Variant( m_curBodyUploadInfos );
        emit sigUploadBodyScore( data );
        return;
    }

    //1.构建连接HTTP对象(一个对象仅进行一次上传)
    QMap<QString, QVariant> infos = data.value< QMap<QString, QVariant> >();
    if( nullptr == m_pUploadBodyScore )
    {
        m_pUploadBodyScore = new CHttpUpLoadBodyScore( this );
    }

    //2022/07/06修改，不使用lambda表达式绑定信号
    connect( m_pUploadBodyScore, &CHttpUpLoadBodyScore::sigRequestRetAvailable, this, &CHttpRequestMgr::onUploadBodyScoreFinished, Qt::UniqueConnection );

    /*
    //2.响应服务器返回信息(不论成功失败均有返回值)
    connect( m_pUploadBodyScore, &CHttpUpLoadBodyScore::sigRequestRetAvailable, [=](bool ok){
        //2.1 安全保护(接收到返回前都是不允许提交的)
        //注：避免界面锁死，就算出错也必须给界面返回
        if(m_isRequestFinished)
        {
            m_errorString = "Can't allowed receive body score return info";
            LOGERROR("Can't allowed receive body score return info");
            //            emit sigUploadBodyScore( false );
            return ;
        }

        //获取请求结果码，判断是不是超时引起的请求失败
        int requestRetCode = m_pUploadCommonScore->getRequestRetCode();

        //2.2 提交失败后赋值错误信息，用于界面显示
        if( !ok )
        {
            m_errorString = m_pUploadBodyScore->getErrorString();
            LOGINFO("upload body score failed, error string = " + m_errorString.toStdString() );
        }

        //2.3 释放HTTP对象资源
        if( nullptr != m_pUploadBodyScore )
        {
            delete m_pUploadBodyScore;
            m_pUploadBodyScore = nullptr;
        }

        //2.4 移除成绩提交存储
        if(!m_vecUnUploadBodyScore.isEmpty())
        {
            m_vecUnUploadBodyScore.removeFirst();
        }

        //2.5 修改标志位
        m_isRequestFinished = true;

        //2.6 递归提交(此逻辑下一次触发只会发送一条消息)
        m_curBodyUploadInfos = getUploadInfos( UserExam, Upload_Body, ok, requestRetCode );
        QVariant data = upLoadInfos2Variant( m_curBodyUploadInfos );
        emit sigUploadBodyScore( data );

        onUploadBody();
    });
    */

    //3.发送请求保护(下一条请求必须在接收后才允许发送)
    if(!m_isRequestFinished)
    {
        m_errorString = "Can't allowed send body score";
        LOGERROR("Can't allowed send body score");
        //        emit sigUploadBodyScore( false );
        return ;
    }

    //4.修改标志位
    m_isRequestFinished = false;

    m_pUploadBodyScore->setServerTokenString( getServerTokenString() );

    //5.组装信息，发送请求
    m_pUploadBodyScore->setParaMap( infos );
    m_pUploadBodyScore->httpRequest();
}

//上传视力成绩
void CHttpRequestMgr::onUpLoadEyeSightScore(QVariant data)
{
    //0.检查是否联网，未联网则直接返回成功
    if( !m_bIsOnline )
    {
        LOGINFO("upload eye sight failed, network is not available...");
        m_vecUnUploadEyesightScore.removeFirst();
        m_curEyeSightUploadInfos = getUploadInfos( UserExam, Upload_EyeSight, true, OFFLINE );
        QVariant data = upLoadInfos2Variant( m_curEyeSightUploadInfos );
        emit sigUploadEyeSightScore( data );
        return;
    }

    //1.构建连接HTTP对象(一个对象仅进行一次上传)
    QMap<QString, QVariant> infos = data.value< QMap<QString, QVariant> >();
    if( nullptr == m_pUploadEyeSightScore )
    {
        m_pUploadEyeSightScore = new CHttpUpLoadEyeSightScore( this );
    }

    //2022/07/06修改，不使用lambda表达式绑定信号
    connect( m_pUploadEyeSightScore, &CHttpUpLoadEyeSightScore::sigRequestRetAvailable, this, &CHttpRequestMgr::onUploadEyesightScoreFinished, Qt::UniqueConnection );
    /*
    //2.响应服务器返回信息(不论成功失败均有返回值)
    connect( m_pUploadEyeSightScore, &CHttpUpLoadEyeSightScore::sigRequestRetAvailable, [=](bool ok){
        //2.1 安全保护(接收到返回前都是不允许提交的)
        //注：避免界面锁死，就算出错也必须给界面返回
        if(m_isRequestFinished)
        {
            m_errorString = "Can't allowed receive eyesight score return info";
            LOGERROR("Can't allowed receive eyesight score return info");
//            emit sigUploadEyeSightScore(false);
            return ;
        }

        //获取请求结果码，判断是不是超时引起的请求失败
        int requestRetCode = m_pUploadCommonScore->getRequestRetCode();

        //2.2 提交失败后赋值错误信息，用于界面显示
        if( !ok )
        {
            m_errorString = m_pUploadEyeSightScore->getErrorString();
            LOGINFO( "upload eyesight socre failed, error string = " + m_errorString.toStdString() );
        }

        //2.3 释放HTTP对象资源
        if( nullptr != m_pUploadEyeSightScore )
        {
            delete m_pUploadEyeSightScore;
            m_pUploadEyeSightScore = nullptr;
        }

        //2.4 移除成绩提交存储
        if(!m_vecUnUploadEyesightScore.isEmpty())
        {
            m_vecUnUploadEyesightScore.removeFirst();
        }

        //2.5 修改标志位
        m_isRequestFinished = true;

        //2.6 递归提交(此逻辑下一次触发只会发送一条消息)
        m_curEyeSightUploadInfos = getUploadInfos( UserExam, Upload_EyeSight, ok, requestRetCode );
        QVariant data = upLoadInfos2Variant( m_curEyeSightUploadInfos );
        emit sigUploadEyeSightScore( data );

        onUploadEyesight();
    });
    */

    //3.发送请求保护(下一条请求必须在接收后才允许发送)
    if(!m_isRequestFinished)
    {
        m_errorString = "Can't allowed send eyesight score";
        LOGERROR("Can't allowed send eyesight score");
        //        emit sigUploadEyeSightScore( false );
        return ;
    }

    //4.修改标志位
    m_isRequestFinished = false;

    m_pUploadEyeSightScore->setServerTokenString( getServerTokenString() );

    //5.组装信息，发送请求
    m_pUploadEyeSightScore->setParaMap( infos );
    m_pUploadEyeSightScore->httpRequest();
}

void CHttpRequestMgr::onTouristUsageRecord(QString devCode, short devType, QString timeStamp)
{
    //检查网络是否在线
    if( !m_bIsOnline ){
        qDebug() << "net work is not available...";
        LOGINFO("tourist usage record failed, network is not available...");
        return;
    }

    //等待上次提交完成
    while( !m_isRequestFinished ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    if( nullptr == m_pTouristRecord ){
        m_pTouristRecord = new CHttpTouristRecord( this );
    }
    connect( m_pTouristRecord, &CHttpTouristRecord::sigRequestRetAvailable, this, &CHttpRequestMgr::onTouristUsageRecordFinished, Qt::UniqueConnection );
    /*
    connect( m_pTouristRecord, &CHttpTouristRecord::sigRequestRetAvailable, [=](bool ok){
        if( !ok ) {
            qDebug() << "tourist usage record failed...";
            m_errorString = m_pTouristRecord->getErrorString();
            qDebug() << "error string = " << m_errorString;
            LOGINFO("tourist usage record failed, erorr string = " + m_errorString.toStdString() );
        }else{
            qDebug() << "tourist usage record success...";
            LOGINFO("tourist usage successs...");
        }

        //设置提交请求完成
        m_isRequestFinished = true;
        emit sigTouristUsageRecordFinished( ok );
    });
*/

    m_pTouristRecord->setDeviceCode( devCode );
    m_pTouristRecord->setDeviceType( devType );
    m_pTouristRecord->setRecordTime( timeStamp );


    m_pTouristRecord->setServerTokenString( getServerTokenString() );


    m_pTouristRecord->httpRequest();
    //发出请求后,将标志位 置为false
    m_isRequestFinished = false;
}

void CHttpRequestMgr::onTimerTimeout()
{
    if( m_useMultiHeartbeat ){
        m_pMultiHeartBeatInfos->setServerTokenString( getServerTokenString() );
        m_pMultiHeartBeatInfos->httpRequest();
    }else{
        m_pHeartBeat->setServerTokenString( getServerTokenString() );
        m_pHeartBeat->httpRequest();
    }


    //定时器在发出请求后，立马关闭，再收到心跳返回后,再重新开启
    qDebug() << "timer state = " << m_pTimer->isActive();
    if( m_pTimer->isActive() ){
        m_pTimer->stop();
    }
}

void CHttpRequestMgr::onAutoUpload()
{
    onUploadCommon();
}

void CHttpRequestMgr::onAutoUploadBodyScore()
{
    onUploadBody();
}

void CHttpRequestMgr::onAutoUploadEyesightScore()
{
    onUploadEyesight();
}

void CHttpRequestMgr::onUploadCommon()
{
    //0.提交保护(此处只进行空值判断，是否有网络后续会判断)
    if( m_vecUnUploadCommonScore.size() <= 0)
    {
        LOGINFO("m_vecUnUploadCommonScore is empty");
        return;
    }

    //1.设置上传成绩
    QVariant data;
    QMap<QString, QVariant> tmpMap = m_vecUnUploadCommonScore.first();
    m_mapCurCommonScore = m_vecUnUploadCommonScore.first();
    data.setValue( tmpMap );

    //2.开始上传
    onUploadCommonScore( data );
}

void CHttpRequestMgr::onUploadBody()
{
    //0.提交保护(此处只进行空值判断，是否有网络后续会判断)
    if( m_vecUnUploadBodyScore.size() <= 0)
    {
        LOGINFO("m_vecUnUploadBodyScore is empty");
        return;
    }

    //1.设置上传成绩
    QVariant data;
    QMap<QString, QVariant> tmpMap = m_vecUnUploadBodyScore.first();
    m_mapCurBodyScore = m_vecUnUploadBodyScore.first();
    data.setValue( tmpMap );

    //2.开始上传
    onUpLoadBodyScore( data );
}

void CHttpRequestMgr::onUploadEyesight()
{
    //0.提交保护(此处只进行空值判断，是否有网络后续会判断)
    if( m_vecUnUploadEyesightScore.size() <= 0)
    {
        LOGINFO("m_vecUnUploadEyesightScore is empty");
        return;
    }

    //1.设置上传成绩
    QVariant data;
    QMap<QString, QVariant> tmpMap = m_vecUnUploadEyesightScore.first();
    m_mapCurEyeSightScore = m_vecUnUploadEyesightScore.first();
    data.setValue( tmpMap );

    //2.开始上传
    onUpLoadEyeSightScore( data );
}

void CHttpRequestMgr::onSyncLocalScores()
{
    m_pSyncLocalScores->startSync();
}



void CHttpRequestMgr::onSyncSingleScoreResultAvailable(QVariant data, int uploadType)
{
    CHttpUploadInfos infos = Variant2UploadInfos( data );

    if( uploadType == UPLOAD_TYPE::Upload_Common ){
        emit sigUploadCommonScore( data );
    }

    if( uploadType == UPLOAD_TYPE::Upload_Body ){
        emit sigUploadBodyScore( data );
    }

    if( uploadType == UPLOAD_TYPE::Upload_EyeSight ){
        emit sigUploadEyeSightScore( data );
    }

}

void CHttpRequestMgr::onQueryHistoryScoreResultAvailable(bool ok)       //CHttpQueryHistoryScore::onNewReplyAvailable
{
    QVector<QStringList>    allHistoryScores;
    QVariant data;
    TEST_ITEM curTestItem = m_pQueryHistoryScore->getCurTestItem();


    if( !ok ){
        qDebug() << "error string = " << m_pQueryHistoryScore->getErrorString();

        //如果查询的是身高体重，并且查询失败，那么及只加载本地的成绩
        if( HTTP_TEST_ITEM_HEIGHT == curTestItem || HTTP_TEST_ITEM_WEIGHT == curTestItem ){
            emit sigQueryHistoryScoreFinished( false, data );  //CMainFrame::onQueryScoreFinished
            return;
        }


    }else{
        int retCount = m_pQueryHistoryScore->resultCount();

        QStringList rowData;
        QMap<QString,QString> tmpRet;
        QString timeStamp;
        QString score;
        QString record;
        QString heightRecord;
        QString weightRecord;

        /*
         * 如果查询的事身高体重, 身高体重从服务器查询下来的，已经组装好了的成绩放在 m_pQueryHistoryScore 的 m_humanComposition 中,
         * 为兼容旧的信号，在这里发送一个空的data对象，实际取数据，通过 getHuamenCompositionList 来获取服务器查询出来的结果
        */
        if( HTTP_TEST_ITEM_HEIGHT == curTestItem || HTTP_TEST_ITEM_WEIGHT == curTestItem ){

            emit sigQueryHistoryScoreFinished( ok, data );  //CMainFrame::onQueryScoreFinished
            return;
        }



        //如果查询的是一般项目
        for( int i = 0; i < retCount; i++ ){
            rowData.clear();
            tmpRet.clear();

            tmpRet    = m_pQueryHistoryScore->getGroupInfo( i );
            timeStamp = tmpRet.value( retKeys::recordTime );
            timeStamp = CHttpAbstractNetWork::timeStampToDateTime( timeStamp.toLongLong() );
            score     = tmpRet.value( retKeys::score );
            record    = tmpRet.value( retKeys::record );

            if( record == INVALID_RESULT_SERVER ){
                record = QString::fromLocal8Bit( "无效成绩" );
                score = "-";
            }

            rowData << timeStamp << record << score;
            allHistoryScores << rowData;
        }
    }
    data.setValue( allHistoryScores );
    emit sigQueryHistoryScoreFinished( ok, data );  //CMainFrame::onQueryScoreFinished
}

CHttpQueryHistoryScore *CHttpRequestMgr::getHttpQueryRequestObj() const
{
    return m_pQueryHistoryScore;
}

CHttpUpLoadMultiCommonScore *CHttpRequestMgr::getUploadMultiCommonScoreObj() const
{
    return m_pUploadMultiCommonScores;
}

CHttpUpLoadMultiBodyScores *CHttpRequestMgr::getUploadMultiBodyScoreObj() const
{
    return m_pUploadMultiBodyScores;
}

CHttpUpLoadMultiEyeSightScores *CHttpRequestMgr::getUploadMultiEyeSightScoreObj() const
{
    return m_pUploadMultiEyeSightScores;
}

CHttpMultiHeartBeat *CHttpRequestMgr::getSendMultiHeartBeatObj() const
{
    return m_pMultiHeartBeatInfos;
}

void CHttpRequestMgr::onGetSampleVideoUrlsFinished(bool ok)
{
    if( !ok ){
        qDebug() << "get video urls failed...";
        m_errorString = m_pGetVideoUrl->getErrorString();
        qDebug() << "error string = " << m_errorString;
        LOGINFO( "get sample video urls failed, error string = " + string(m_errorString.toLocal8Bit()) );
    }else{
        m_vecSampleVieoUrls.clear();
        int urlsCount = m_pGetVideoUrl->resultCount();

        //所有的信息都存放在results里面
        QVector<QMap< QString, QString >> results;
        for( int index = 0; index < urlsCount; index++ ){
            QMap< QString, QString > retGroup = m_pGetVideoUrl->getGroupRet( index );
            m_vecSampleVieoUrls << retGroup;
        }
        qDebug() << "get sample video urls finished...";
        LOGINFO("get sample video urls finished...");
    }
    emit sigGetSampleVideoFinished( ok );
}

void CHttpRequestMgr::onSyncScoreStandardFinished(bool ok)
{
    if( !ok ){
        qDebug() << "sync score std failed...";
        m_errorString = m_pSyncScoreStd->getErrorString();
        qDebug() << "error string = " << m_errorString;
        LOGINFO( "Sync Score Standard failed, error string = " + m_errorString.toStdString() );
    }else{
        int urlsCount = m_pSyncScoreStd->resultCount();

        //所有的信息都存放在 m_vecScoreStd 里面
        m_vecScoreStd.clear();
        for( int index = 0; index < urlsCount; index++ ){
            QMap< QString, QString > retGroup = m_pSyncScoreStd->getGroupRet( index );
            m_vecScoreStd << retGroup;
        }

        qDebug() << "sync score std success...";
        LOGINFO("sync score std success...");
    }
    emit sigSyncScoreStdFinished( ok );
}

void CHttpRequestMgr::onSyncUserInfoFinished(bool ok)
{
    if( !ok ){
        qDebug() << "sync user info failed...";

        m_errorString = m_pSyncUserInfo->getErrorString();
        LOGINFO("sync user info failed, error string = " + m_errorString.toStdString() );
        qDebug() << "error string = " << m_errorString;
    }else{
        int urlsCount = m_pSyncUserInfo->resultCount();
        //所有的信息都存放在results里面
        m_vecUserInfo.clear();
        m_vecUserInfo.resize(urlsCount);
        for( int index = 0; index < urlsCount; index++ ){

            m_vecUserInfo[index] = m_pSyncUserInfo->getGroupRet( index );
        }
        qDebug() << "sync user info success...";
        LOGINFO("sync user info success...");
    }
    emit sigSyncUserInfoFinished( ok ); //onSyncUserInfoFinished
}

void CHttpRequestMgr::onSyncClassInfoFinished(bool ok)
{
    if( !ok ){
        qDebug() << "sync class info failed...";
        m_errorString = m_pSyncClassInfo->getErrorString();
        qDebug() << "error string = " << m_errorString;
        LOGINFO("sync class info failed, error string = " + m_errorString.toStdString() );
    }else{
        int urlsCount = m_pSyncClassInfo->resultCount();
        m_vecClassInfo.clear();
        //所有的信息都存放在 m_vecClassInfo 里面
        for( int index = 0; index < urlsCount; index++ ){
            QMap< QString, QString > retGroup = m_pSyncClassInfo->getGroupInfo( index );
            m_vecClassInfo << retGroup;
        }
        qDebug() << "sync class info success...";
        LOGINFO("sync class info success...");
    }

    emit sigSyncClassInfoFinished( ok );
}

void CHttpRequestMgr::onUserIdentifyFinished(bool ok)
{
    if( !ok ){
        qDebug() << "user indentify failed...";
        m_errorString = m_pUserIdentify->getErrorString();
        qDebug() << "error string = " << m_errorString;
        LOGINFO("user indedntify failed, error string = " + m_errorString.toStdString() );
    }else{
        QString userName = m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::name );
        qDebug() << "user indentify success..." << userName;
        LOGINFO("user indentify success, user name = " + userName.toStdString() );
        m_mapUserInfo.clear();
        /*int infoCount =*/ m_pUserIdentify->userInfoCount();
        //获取名字等信息
        m_mapUserInfo.insert( userIdentifyParaKey::userIdentId, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::userIdentId ) );
        m_mapUserInfo.insert( userIdentifyParaKey::year, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::year ) );
        m_mapUserInfo.insert( userIdentifyParaKey::clazzId, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::clazzId ) );
        m_mapUserInfo.insert( userIdentifyParaKey::role, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::role ) );
        m_mapUserInfo.insert( userIdentifyParaKey::code, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::code ) );
        m_mapUserInfo.insert( userIdentifyParaKey::cardNo, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::cardNo ) );
        m_mapUserInfo.insert( userIdentifyParaKey::name, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::name ) );
        m_mapUserInfo.insert( userIdentifyParaKey::gender, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::gender ) );
        m_mapUserInfo.insert( userIdentifyParaKey::birthday, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::birthday ) );
        m_mapUserInfo.insert( userIdentifyParaKey::nation, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::nation ) );
        m_mapUserInfo.insert( userIdentifyParaKey::faceFeature, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::faceFeature ) );
        m_mapUserInfo.insert( userIdentifyParaKey::description, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::description ) );
        m_mapUserInfo.insert( userIdentifyParaKey::avatarUrl, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::avatarUrl ) );
        m_mapUserInfo.insert( userIdentifyParaKey::updateTime, m_pUserIdentify->getUserInfoByKey( userIdentifyParaKey::updateTime ) );

        qDebug() << "user name = " << m_mapUserInfo.value( userIdentifyParaKey::name );
    }
    emit sigUserIdentifyFinished( ok );
}

void CHttpRequestMgr::onHeartBeatSendFinished(bool ok)
{
    if( !ok ){
        qDebug() << "send heart beat failed...";
        m_errorString = m_pHeartBeat->getErrorString();
        qDebug() << "error string = " << m_errorString;
        LOGINFO("send heart beat failed, error string = " + m_errorString.toStdString() );
    }else{
        //至此心跳发送成功
        qDebug() << "send heart beat success...";
    }
    //如果服务器状态发生改变,则发信号通知绑定的槽函数
    if( ok != m_bIsOnline ){
        m_bIsOnline = ok;
        emit sigHeartBeatFinished( ok );
    }

    //收到请求返回后, 立马开启定时器
    m_pTimer->start();
}

void CHttpRequestMgr::onUploadCommonScoreFinished(bool ok)
{
    //2.1 安全保护(接收到返回前都是不允许提交的)
    //注：避免界面锁死，就算出错也必须给界面返回
    if(m_isRequestFinished)
    {
        m_errorString = "Can't allowed receive common score return info";
        LOGERROR("Can't allowed receive common score return info");
        //            m_curCommonUploadInfos = getUploadInfos( UserExam, Upload_Common, false );
        //这个信号只是为了出现异常情况的时候能够关闭界面上提交中的弹窗
        //            emit sigUploadCommonScore( m_curCommonUploadInfos );
        return ;
    }

    //获取请求结果码，判断是不是超时引起的请求失败
    int requestRetCode = m_pUploadCommonScore->getRequestRetCode();

    //2.2 提交失败后赋值错误信息，用于界面显示
    if( !ok )
    {
        m_errorString = m_pUploadCommonScore->getErrorString();
        qDebug() << "error string = " << m_errorString;
        QString userId = m_vecUnUploadCommonScore.first().value( HTTP_KEYS::userId ).toString();
        QString testItem = m_vecUnUploadCommonScore.first().value(HTTP_KEYS::testItem).toString();
        QString recordTime = m_vecUnUploadCommonScore.first().value(HTTP_KEYS::recordTime).toString();
        QString dataBaseTime = QString::fromStdString( CTestResultInfo::getSaveTimeStamp(recordTime) );
        LOGINFO("upload common socre failed, userid = " + userId.toStdString() + "erorr string = " + m_errorString.toStdString() );
        QString logInfo = QString( "failed userid = %1, testItem = %2, recordtime = %3" ).arg( userId, testItem, dataBaseTime );
        LOGINFO( logInfo.toStdString() );
    }


    //2.3 释放HTTP对象资源
    if( nullptr != m_pUploadCommonScore )
    {
        delete m_pUploadCommonScore;
        m_pUploadCommonScore = nullptr;
    }

    //2.4 移除成绩提交存储
    if(!m_vecUnUploadCommonScore.isEmpty())
    {
        m_vecUnUploadCommonScore.removeFirst();
    }

    //2.5 修改标志位
    m_isRequestFinished = true;

    //在此处组装当前提交结果信息，发送至CTestGuideManager::onUploadCommonScoreFinished中， 做出相应的处理
    m_curCommonUploadInfos = getUploadInfos( DATA_SRC::UserExam, UPLOAD_TYPE::Upload_Common, ok, requestRetCode );
    QVariant data = upLoadInfos2Variant( m_curCommonUploadInfos );
    emit sigUploadCommonScore( data );

    //2.6 递归提交(此逻辑下一次触发只会发送一条消息)
    onUploadCommon();
}

void CHttpRequestMgr::onUploadBodyScoreFinished(bool ok)
{
    //2.1 安全保护(接收到返回前都是不允许提交的)
    //注：避免界面锁死，就算出错也必须给界面返回
    if(m_isRequestFinished)
    {
        m_errorString = "Can't allowed receive body score return info";
        LOGERROR("Can't allowed receive body score return info");
        //            emit sigUploadBodyScore( false );
        return ;
    }

    //获取请求结果码，判断是不是超时引起的请求失败
    int requestRetCode = m_pUploadBodyScore->getRequestRetCode();

    //2.2 提交失败后赋值错误信息，用于界面显示
    if( !ok )
    {
        m_errorString = m_pUploadBodyScore->getErrorString();
        LOGINFO("upload body score failed, error string = " + m_errorString.toStdString() );
    }

    //2.3 释放HTTP对象资源
    if( nullptr != m_pUploadBodyScore )
    {
        delete m_pUploadBodyScore;
        m_pUploadBodyScore = nullptr;
    }

    //2.4 移除成绩提交存储
    if(!m_vecUnUploadBodyScore.isEmpty())
    {
        m_vecUnUploadBodyScore.removeFirst();
    }

    //2.5 修改标志位
    m_isRequestFinished = true;

    //2.6 递归提交(此逻辑下一次触发只会发送一条消息)
    m_curBodyUploadInfos = getUploadInfos( DATA_SRC::UserExam, UPLOAD_TYPE::Upload_Body, ok, requestRetCode );
    QVariant data = upLoadInfos2Variant( m_curBodyUploadInfos );
    emit sigUploadBodyScore( data );

    onUploadBody();
}

void CHttpRequestMgr::onUploadEyesightScoreFinished(bool ok)
{
    if(m_isRequestFinished)
    {
        m_errorString = "Can't allowed receive eyesight score return info";
        LOGERROR("Can't allowed receive eyesight score return info");
        //            emit sigUploadEyeSightScore(false);
        return ;
    }

    //获取请求结果码，判断是不是超时引起的请求失败
    int requestRetCode = m_pUploadEyeSightScore->getRequestRetCode();

    //2.2 提交失败后赋值错误信息，用于界面显示
    if( !ok )
    {
        m_errorString = m_pUploadEyeSightScore->getErrorString();
        LOGINFO( "upload eyesight socre failed, error string = " + m_errorString.toStdString() );
    }

    //2.3 释放HTTP对象资源
    if( nullptr != m_pUploadEyeSightScore )
    {
        delete m_pUploadEyeSightScore;
        m_pUploadEyeSightScore = nullptr;
    }

    //2.4 移除成绩提交存储
    if(!m_vecUnUploadEyesightScore.isEmpty())
    {
        m_vecUnUploadEyesightScore.removeFirst();
    }

    //2.5 修改标志位
    m_isRequestFinished = true;

    //2.6 递归提交(此逻辑下一次触发只会发送一条消息)
    m_curEyeSightUploadInfos = getUploadInfos( UserExam, Upload_EyeSight, ok, requestRetCode );
    QVariant data = upLoadInfos2Variant( m_curEyeSightUploadInfos );
    emit sigUploadEyeSightScore( data );

    onUploadEyesight();
}

void CHttpRequestMgr::onTouristUsageRecordFinished(bool ok)
{
    if( !ok ) {
        qDebug() << "tourist usage record failed...";
        m_errorString = m_pTouristRecord->getErrorString();
        qDebug() << "error string = " << m_errorString;
        LOGINFO("tourist usage record failed, erorr string = " + m_errorString.toStdString() );
    }else{
        qDebug() << "tourist usage record success...";
        LOGINFO("tourist usage successs...");
    }

    //设置提交请求完成
    m_isRequestFinished = true;
    emit sigTouristUsageRecordFinished( ok, m_bIsOnline, m_pTouristRecord->getRequestRetCode() );
}

void CHttpRequestMgr::onUploadMultiCommonScoreFinished(bool ok)
{
    QVector<CMultiUploadRetInfo> vec = m_pUploadMultiCommonScores->getVecUploadMulRetInfos();
    emit sigUploadMultiCommonScoreFinished( vec );      //onUploadMulityScoreFinished
}

void CHttpRequestMgr::onUploadMultiBodyScoreFinished(bool ok)
{
    emit sigUploadMultiBodyScoreFinished( m_pUploadMultiBodyScores->getVecUploadMulRetInfos() );    //onUploadMulityScoreFinished
}

void CHttpRequestMgr::onUploadMultiEyeSightScoreFinished(bool ok)
{
    emit sigUploadMultiEyeSightScoreFinished( m_pUploadMultiEyeSightScores->getVecUploadMulRetInfos() );
}

void CHttpRequestMgr::onSendMultiHeartBeatFinished(bool ok)
{
    //如果服务器状态发生改变,则发信号通知绑定的槽函数
    if( ok != m_bIsOnline ){
        m_bIsOnline = ok;
        emit sigHeartBeatFinished( ok );
    }

    //收到请求返回后, 立马开启定时器
    m_pTimer->start();

    emit sigSendMultiHeartBeatFinished( ok );   //CMainFrame::onSendMultiHeartBeatFinished
}

void CHttpRequestMgr::onSyncMultiLocalScoresFinished(QVector<CMultiUploadRetInfo> &totalSyncScoreInfos)
{
    emit sigSyncMultiLocalScoresFinished( totalSyncScoreInfos );    //onUploadMulityScoreFinished
}

void CHttpRequestMgr::onQueryHistoryScore(QVariantMap params)
{
    //    if( nullptr == m_pQueryHistoryScore || !m_bIsOnline ){
    //        return;
    //    }
    QString           userId       = params.value( "userId" ).toString();
    TEST_ITEM         tmpTestItem  = static_cast<TEST_ITEM>(params.value( "testItem" ).toInt());
    queryTimeInterval tmpInterval  = static_cast<queryTimeInterval>(params.value( "timeInterval" ).toInt());
    testType          tmpScoreType = static_cast<testType>( params.value( "scoreType" ).toInt() );

    m_pQueryHistoryScore->setUserId( userId );
    m_pQueryHistoryScore->setTestItem( tmpTestItem );
    m_pQueryHistoryScore->setTimeInterval( tmpInterval );
    m_pQueryHistoryScore->setTestScoreType( tmpScoreType );

    m_pQueryHistoryScore->setServerTokenString( getServerTokenString() );

    m_pQueryHistoryScore->httpRequest();    //CHttpQueryHistoryScore::onNewReplyAvailable
}

void CHttpRequestMgr::initConnection()
{
    qDebug() << "3 init thread function Thread ID = " << QThread::currentThreadId();
    //绑定多线程信号
    connect( this, &CHttpRequestMgr::insideSigGetVideoUrls,              this, &CHttpRequestMgr::onGetSampleVideoUrls, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigSyncScoreStd,              this, &CHttpRequestMgr::onSyncScoreStandard, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigSyncUserInfo,              this, &CHttpRequestMgr::onSyncUserInfo, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigSyncClassInfo,             this, &CHttpRequestMgr::onSyncClassInfo, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUserIdentify,              this, &CHttpRequestMgr::onUserIdentify, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigSendHeartBeat,             this, &CHttpRequestMgr::onSendHeartBeat, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUploadCommonScore,         this, &CHttpRequestMgr::onUploadCommonScore, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUploadBodyScore,           this, &CHttpRequestMgr::onUpLoadBodyScore, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUploadEyeSightScore,       this, &CHttpRequestMgr::onUpLoadEyeSightScore, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigToutisUsageRecord,         this, &CHttpRequestMgr::onTouristUsageRecord, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUploadCommonScoreByVector, this, &CHttpRequestMgr::onUploadCommonScoreByVector, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigStartAuto,                 this, &CHttpRequestMgr::onAutoUpload, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigStartAutoUploadBodyScore,  this, &CHttpRequestMgr::onAutoUploadBodyScore, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigStartAutoEyesightScore,    this, &CHttpRequestMgr::onAutoUploadEyesightScore, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigSyncLocalScore,            this, &CHttpRequestMgr::onSyncLocalScores, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigQueryHistoryScore,         this, &CHttpRequestMgr::onQueryHistoryScore, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigInitMemberConnection,      this, &CHttpRequestMgr::onInitInsideMemberConnection, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUploadMultiCommonScores,   this, &CHttpRequestMgr::onUploadMultiCommonScores, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUploadMultiBodyScores,     this, &CHttpRequestMgr::onUploadMultiBodyScores, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigUploadMultiEyeSightScores, this, &CHttpRequestMgr::onUploadMultiEyeSightScores, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigSendMultiHeartBeat,        this, &CHttpRequestMgr::onSendMultiHeartBeat, Qt::QueuedConnection );
    connect( this, &CHttpRequestMgr::insideSigSyncMultiLcoalScores,      this, &CHttpRequestMgr::onSyncMultiLocalScores/*, Qt::QueuedConnection */);
    connect( this, &CHttpRequestMgr::insideSigStartGetServerToken,       this, &CHttpRequestMgr::onStartGetServerToken, Qt::QueuedConnection );
}

void CHttpRequestMgr::onInitInsideMemberConnection()
{
    qDebug() << "onInitInsideMemberConnection thread ID = " << QThread::currentThreadId();
    if( nullptr == m_pSyncLocalScores ){
        m_pSyncLocalScores = new CHttpSyncLocalScores();
    }

    if( nullptr == m_pQueryHistoryScore ){
        m_pQueryHistoryScore = new CHttpQueryHistoryScore();
    }

    connect( m_pSyncLocalScores,   &CHttpSyncLocalScores::sigSingleSyncResultAvailable, this, &CHttpRequestMgr::onSyncSingleScoreResultAvailable );
    connect( m_pQueryHistoryScore, &CHttpQueryHistoryScore::sigRequestRetAvailable,     this, &CHttpRequestMgr::onQueryHistoryScoreResultAvailable );

}

void CHttpRequestMgr::initTimer()
{
    if( nullptr == m_pTimer ){
        qDebug() << "1 init thread function Thread ID = " << QThread::currentThreadId();
        m_pTimer = new QTimer();
        m_pTimer->setInterval( HEARTBEART_INTERVAL );
        connect( m_pTimer, &QTimer::timeout, this, &CHttpRequestMgr::onTimerTimeout );
    }
}

CHttpUploadInfos CHttpRequestMgr::getUploadInfos(DATA_SRC dataSrc, UPLOAD_TYPE type, bool ok, int requestRetCode )
{
    CHttpUploadInfos infos;
    if( type == UPLOAD_TYPE::Upload_Common ){
        infos.requestCode   = requestRetCode;
        infos.m_userId      =  m_mapCurCommonScore.value( HTTP_KEYS::userId ).toString();
        infos.m_testItem    =  m_mapCurCommonScore.value( HTTP_KEYS::testItem ).toString();
        infos.m_recordTime  =  m_mapCurCommonScore.value( HTTP_KEYS::recordTime ).toString();
        infos.m_isFinished  =  m_vecUnUploadCommonScore.size() > 0 ? false : true;
        infos.m_isSuccess   =  ok;
        infos.m_dataSrc     =  static_cast<short>(dataSrc);
        infos.m_remainCount =  m_vecUnUploadCommonScore.size();
        infos.m_testRet     =  m_mapCurCommonScore.value( HTTP_KEYS::result ).toString();
        infos.m_commitProgress = QString( "%1 / %2" ).arg( QString::number( m_uploadCommonTotalCount - m_vecUnUploadCommonScore.size() ), QString::number( m_uploadCommonTotalCount ) );
    }

    if( type == UPLOAD_TYPE::Upload_Body ){
        infos.requestCode   = requestRetCode;
        infos.m_userId      =  m_mapCurBodyScore.value( HTTP_KEYS::userId ).toString();
        infos.m_testItem    =  m_mapCurBodyScore.value( UPLOADBODYSCORE_KEY_HEIGHT ).toString();
        infos.m_recordTime  =  m_mapCurBodyScore.value( HTTP_KEYS::recordTime ).toString();
        infos.m_isFinished  =  m_vecUnUploadBodyScore.size() > 0 ? false : true;
        infos.m_isSuccess   =  ok;
        infos.m_dataSrc     =  static_cast<short>(dataSrc);
        infos.m_testRet     =  m_mapCurBodyScore.value( HTTP_KEYS::result ).toString();
        infos.m_commitProgress = QString( "%1 / %2" ).arg( QString::number( m_uploadBodyTotalCount - m_vecUnUploadBodyScore.size() ), QString::number( m_uploadBodyTotalCount ) );
    }

    if( type == UPLOAD_TYPE::Upload_EyeSight ){
        infos.requestCode   = requestRetCode;
        infos.m_userId      =  m_mapCurEyeSightScore.value( HTTP_KEYS::userId ).toString();
        infos.m_testItem    =  m_mapCurEyeSightScore.value( HTTP_KEYS::leftEyeSight ).toString();
        infos.m_recordTime  =  m_mapCurEyeSightScore.value( HTTP_KEYS::recordTime ).toString();
        infos.m_isFinished  =  m_vecUnUploadEyesightScore.size() > 0 ? false : true;
        infos.m_isSuccess   =  ok;
        infos.m_dataSrc     =  static_cast<short>(dataSrc);
        infos.m_testRet     =  m_mapCurEyeSightScore.value( HTTP_KEYS::result ).toString();
        infos.m_commitProgress = QString( "%1 / %2" ).arg( QString::number( m_uploadEyeSightTotalCount - m_vecUnUploadEyesightScore.size() ), QString::number( m_uploadEyeSightTotalCount ) );
    }

    infos.m_isServerOnline = m_bIsOnline;

    return infos;
}

CHttpMultiHeartBeat *CHttpRequestMgr::getPMultiHeartBeatInfos() const
{
    return m_pMultiHeartBeatInfos;
}

QString CHttpRequestMgr::getServerTokenString() const
{
    return m_serverTokenString;
}

void CHttpRequestMgr::setServerTokenString(const QString &serverTokenString)
{
    m_serverTokenString = serverTokenString;
}

CHttpSyncClassInfo *CHttpRequestMgr::getPSyncClassInfo() const
{
    return m_pSyncClassInfo;
}

void CHttpRequestMgr::onStartGetServerToken(const QVariantMap loginInfoMap)
{
    if( nullptr == m_pGetServerToken ){
        m_pGetServerToken = new CGetServerToken();
    }

    connect( m_pGetServerToken, &CGetServerToken::sigRequestRetAvailable, this, &CHttpRequestMgr::onGetServerTokenFinished, Qt::UniqueConnection );

    // setLoginParaMap 内部已经发出请求
    m_pGetServerToken->setLoginParaMap( loginInfoMap );

}

CHttpSyncUserInfo *CHttpRequestMgr::getPSyncUserInfo() const
{
    return m_pSyncUserInfo;
}

CHttpSyncMultiLocalScores *CHttpRequestMgr::getPSyncMultiLocalScores() const
{
    return m_pSyncMultiLocalScores;
}

void CHttpRequestMgr::onGetServerTokenFinished(bool ok)
{
    if( !ok ){
        setServerTokenString( "" );
        return;
    }

    setServerTokenString( m_pGetServerToken->token() );
}

CHttpQueryHistoryScore *CHttpRequestMgr::getPQueryHistoryScore() const
{
    return m_pQueryHistoryScore;
}

QString CHttpRequestMgr::getSerializationResList(const QMap<int, QString> &tmpMap)
{
    QString qsRet, qsTemp;

    int i = 0, iCount = tmpMap.size();

    for(auto it = tmpMap.begin(); it != tmpMap.end(); it++)
    {
        //例如身高对应项目 为 1 值为 176.0 那么生成的qsTemp 为 1=176.0
        qsTemp = QString::number(it.key()) + '=' + it.value();

        qsRet.append(qsTemp);
        if(i != iCount - 1){
            qsRet.append('@');
        }

        i++;
    }

    return qsRet;
}

void CHttpRequestMgr::onUploadMultiCommonScores(QVariantList list)
{
    if( nullptr == m_pUploadMultiCommonScores ){
        m_pUploadMultiCommonScores = new CHttpUpLoadMultiCommonScore( this );
    }

    connect( m_pUploadMultiCommonScores, &CHttpUpLoadMultiCommonScore::sigRequestRetAvailable, this, &CHttpRequestMgr::onUploadMultiCommonScoreFinished, Qt::UniqueConnection );
    m_pUploadMultiCommonScores->setServerTokenString( getServerTokenString() );

    m_pUploadMultiCommonScores->setUpLoadMultiScores( list );
    m_pUploadMultiCommonScores->httpRequest();
}

void CHttpRequestMgr::onUploadMultiBodyScores(QVariantList list)
{
    if( nullptr == m_pUploadMultiBodyScores ){
        m_pUploadMultiBodyScores = new CHttpUpLoadMultiBodyScores( this );
    }

    m_pUploadMultiBodyScores->setServerTokenString( getServerTokenString() );

    connect( m_pUploadMultiBodyScores, &CHttpUpLoadMultiBodyScores::sigRequestRetAvailable, this, &CHttpRequestMgr::onUploadMultiBodyScoreFinished, Qt::UniqueConnection );
    m_pUploadMultiBodyScores->setUpLoadMultiScores( list );
    m_pUploadMultiBodyScores->httpRequest();
}

void CHttpRequestMgr::onUploadMultiEyeSightScores(QVariantList list)
{
    if( nullptr == m_pUploadMultiEyeSightScores ){
        m_pUploadMultiEyeSightScores = new CHttpUpLoadMultiEyeSightScores( this );
    }

    connect( m_pUploadMultiEyeSightScores, &CHttpUpLoadMultiEyeSightScores::sigRequestRetAvailable, this, &CHttpRequestMgr::onUploadMultiEyeSightScoreFinished, Qt::UniqueConnection );
    m_pUploadMultiEyeSightScores->setUpLoadMultiScores( list );
    m_pUploadMultiEyeSightScores->httpRequest();
}

void CHttpRequestMgr::onSendMultiHeartBeat(QVariantList list)
{
    if( nullptr == m_pMultiHeartBeatInfos ){
        m_pMultiHeartBeatInfos = new CHttpMultiHeartBeat( this );
    }

    /*
      一般情况下，只会调用两次发送心跳函数,  第一次设备为空时，发送空心跳包，仅用于查询服务器状态
      第二次调用时，一般为设备连接上的时候, 这个时候就包含了设备信息, 第二次发送之前，先停掉自动发送心跳的死的定时器
    */

    //如果是第一次调用心跳函数, 则初始化定时器
    if( nullptr == m_pTimer ){
        initTimer();
    }else{
        //如果定时器已经被初始化，说明是第二次主动调用发送心跳的函数, 则先停掉定时器, 装载完数据后，发送请求，在接受请求结果的地方开启定时器
        if( m_pTimer->isActive() ){
            m_pTimer->stop();
        }
    }

    m_pMultiHeartBeatInfos->setServerTokenString( getServerTokenString() );

    connect( m_pMultiHeartBeatInfos, &CHttpMultiHeartBeat::sigRequestRetAvailable, this, &CHttpRequestMgr::onSendMultiHeartBeatFinished, Qt::UniqueConnection );
    m_pMultiHeartBeatInfos->setHeartBeatInfos( list );
    m_pMultiHeartBeatInfos->httpRequest();          //onSendMultiHeartBeatFinished
}

void CHttpRequestMgr::onSyncMultiLocalScores(QVariantList commonList, QVariantList bodyList, QVariantList eyeSightList)
{
    if( nullptr == m_pSyncMultiLocalScores ){
        m_pSyncMultiLocalScores = new CHttpSyncMultiLocalScores( this );
    }

    connect( m_pSyncMultiLocalScores, &CHttpSyncMultiLocalScores::sigSyncFinished, this, &CHttpRequestMgr::onSyncMultiLocalScoresFinished, Qt::UniqueConnection );
    m_pSyncMultiLocalScores->setCommonScoreList( commonList );
    m_pSyncMultiLocalScores->setBodyScoreList( bodyList );
    m_pSyncMultiLocalScores->setEyeSightScoreList( eyeSightList );

    m_pSyncMultiLocalScores->startSync( getServerTokenString() );
}

QVariant CHttpRequestMgr::map2Variant(QMap<QString, QVariant> &tmpMap)
{
    QVariant data;
    data.setValue( tmpMap );
    return data;
}

QMap<QString, QVariant> CHttpRequestMgr::variant2Map(QVariant &data)
{
    QMap<QString, QVariant> infos;
    infos = data.value<QMap<QString, QVariant>>();
    return infos;
}

QVariant CHttpRequestMgr::upLoadInfos2Variant(CHttpUploadInfos &infos)
{
    QVariant data;
    data.setValue( infos );
    return data;
}

CHttpUploadInfos CHttpRequestMgr::Variant2UploadInfos(QVariant &data)
{
    CHttpUploadInfos infos = data.value<CHttpUploadInfos>();
    return infos;
}

testType CHttpRequestMgr::convertScoreType2QueryScoreType(const short &storageScoreType)
{
    if( storageScoreType == CProjectEnums::TestNature::TEST_INFORMAL ){
        return testType::exerciseType;
    }

    if( storageScoreType == CProjectEnums::TestNature::TEST_FORMAL ){
        return testType::examType;
    }

    return testType::exerciseType;
}

void CHttpRequestMgr::setVecUnUploadEyesightScore(QVector<QMap<QString, QVariant> > vecUnUploadEyesightScore, bool bSyncSignal/* = false*/)
{
    //0.保存需要上传的成绩
    m_vecUnUploadEyesightScore = vecUnUploadEyesightScore;

    //1.赋值标志变量
    m_bSyncFlag = bSyncSignal;
    m_uploadEyeSightTotalCount = m_vecUnUploadEyesightScore.size();

    //2.通过信号发送，为了使用HTTP线程
    autoUploadEyesightScore();
}

void CHttpRequestMgr::startSyncLocalScore()
{
    emit insideSigSyncLocalScore();
}

void CHttpRequestMgr::setSyncCommonScores(QVector<QMap<QString, QVariant> > vecCommonScores)
{
    qDebug() << "setSyncCommonScores thread ID = " << QThread::currentThreadId();
    if( nullptr == m_pSyncLocalScores ){
        return;
    }
    m_pSyncLocalScores->setVecCommonScore( vecCommonScores );
}

void CHttpRequestMgr::setSyncBodyScores(QVector<QMap<QString, QVariant> > vecBodyScores)
{
    if( nullptr == m_pSyncLocalScores ){
        return;
    }
    m_pSyncLocalScores->setVecBodyScore( vecBodyScores );
}

void CHttpRequestMgr::setSyncEyeSightScores(QVector<QMap<QString, QVariant> > vecEyeSightScores)
{
    if( nullptr == m_pSyncLocalScores ){
        return;
    }

    m_pSyncLocalScores->setVecEyeSightScore( vecEyeSightScores );
}

void CHttpRequestMgr::setVecUnUploadBodyScore(QVector<QMap<QString, QVariant> > vecUnUploadBodyScore, bool bSyncSignal/* = false*/)
{
    //0.保存需要上传的成绩
    m_vecUnUploadBodyScore = vecUnUploadBodyScore;

    //1.赋值标志变量
    m_bSyncFlag = bSyncSignal;
    m_uploadBodyTotalCount = m_vecUnUploadBodyScore.size();

    //2.通过信号发送，为了使用HTTP线程
    autoUploadBodyScore();
}

void CHttpRequestMgr::setVecUnUploadCommonScore(QVector<QMap<QString, QVariant> > vecUnUploadCommonScore, bool bSyncSignal/* = false*/)
{
    //0.保存需要上传的成绩
    m_vecUnUploadCommonScore = vecUnUploadCommonScore;

    //1.赋值标志变量
    m_bSyncFlag = bSyncSignal;
    m_uploadCommonTotalCount = m_vecUnUploadCommonScore.size();

    //    for( auto &eachEle : m_vecUnUploadCommonScore ){
    //        QString logInfo = QString( "======================== userId = %1, test Ret = %2" ).arg( eachEle.value( HTTP_KEYS::userId ).toString(), eachEle.value( HTTP_KEYS::result ).toString() );
    //        LOGINFO( logInfo.toStdString() );
    //    }


    //2.通过信号发送，为了使用HTTP线程
    autoUpload();
}

void CHttpRequestMgr::autoUpload()
{
    emit insideSigStartAuto();  // onAutoUpload
}

void CHttpRequestMgr::autoUploadBodyScore()
{
    emit insideSigStartAutoUploadBodyScore();
}

void CHttpRequestMgr::autoUploadEyesightScore()
{
    emit insideSigStartAutoEyesightScore();
}

void CHttpRequestMgr::uploadScore(const short &shDevType, const QVector<QMap<QString, QVariant> > &vecUploadScore, DATA_SRC dataSrc)
{
    if(vecUploadScore.size() <= 0)
        return;

    switch (shDevType) {
    case DEV_TYPE_BODY:
        if(dataSrc == UserExam){
            setVecUnUploadBodyScore(vecUploadScore);
        }else{
            setSyncBodyScores(vecUploadScore);
        }
        break;
    case DEV_TYPE_EYESIGHT:
        if(dataSrc == UserExam){
            setVecUnUploadEyesightScore(vecUploadScore);
        }else{
            setSyncEyeSightScores(vecUploadScore);
        }
        break;
    default:
        if(dataSrc == UserExam){
            setVecUnUploadCommonScore(vecUploadScore);
        }else{
            setSyncCommonScores(vecUploadScore);
        }
        break;
    }

    if( DATA_SRC::DataBase == dataSrc ){
        startSyncLocalScore();
    }

}


void CHttpRequestMgr::uploadScore(const short &shDevType, const QVariantList &listUploadScore, DATA_SRC dataSrc)
{
    if(listUploadScore.size() <= 0)
        return;

    switch (shDevType) {
    case DEV_TYPE_BODY:
        if(dataSrc == UserExam){
            uploadMultiBodyScores(listUploadScore);
        }else{

        }
        break;
    case DEV_TYPE_EYESIGHT:
        if(dataSrc == UserExam){
            uploadMultiEyeSightScores(listUploadScore);
        }else{

        }
        break;
    default:
        if(dataSrc == UserExam){
            uploadMultiCommonScores(listUploadScore);
        }else{

        }
        break;
    }
}

bool CHttpRequestMgr::getBIsOnline() const
{
    return m_bIsOnline;
}

void CHttpRequestMgr::setIsOnline(bool ok)
{
    m_bIsOnline = ok;
}

QMap<QString, QString> CHttpRequestMgr::getMapUserInfo() const
{
    return m_mapUserInfo;
}

QVector<QMap<QString, QString> > CHttpRequestMgr::getVecClassInfo() const
{
    return m_vecClassInfo;
}

QString CHttpRequestMgr::getClassSpliTime() const
{
    if( nullptr == m_pSyncClassInfo ){
        return "";
    }
    return m_pSyncClassInfo->getDelSpliTime();
}

QVector<QString> CHttpRequestMgr::getDelClass() const
{
    if( nullptr == m_pSyncClassInfo ){
        return QVector<QString>();
    }
    return m_pSyncClassInfo->getVecDelClass();
}

QVector<QMap<QString, QString> > CHttpRequestMgr::getVecUserInfo() const
{
    return m_vecUserInfo;
}

QString CHttpRequestMgr::getUserSpliTime() const
{
    if( nullptr == m_pSyncUserInfo ){
        return "";
    }
    return m_pSyncUserInfo->getDelSpliTime();
}

QVector<QString> CHttpRequestMgr::getDelUsers() const
{
    return m_pSyncUserInfo->getDelUsers();
}

QVector<QMap<QString, QString> > CHttpRequestMgr::getVecScoreStd() const
{
    return m_vecScoreStd;
}

QString CHttpRequestMgr::getErrorString() const
{
    return m_errorString;
}

QVector<QMap<QString, QString> > CHttpRequestMgr::getVecSampleVieoUrls() const
{
    return m_vecSampleVieoUrls;
}

CHttpRequestMgr *CHttpRequestMgr::m_pHttpRequestMgr = nullptr;
QMutex CHttpRequestMgr::m_locker;

