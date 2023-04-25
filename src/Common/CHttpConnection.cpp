//#include "CFilePath.h"
//#include "CHttpConnection.h"
//#include "Logger/CLogger.h"

//HttpConnection::HttpConnection( QObject *parent ) : QObject( parent )
//{
//    //首先读取配置文件
//    readConfigFile();
//    initNetwork();
//}

//HttpConnection::~HttpConnection()
//{
//    destoryFilePointer();
//}

//void HttpConnection::initThread()
//{
//    //    m_pHttpDownloader = new HttpDownloadThread();
//    //    m_pDownloaderWorkThread = new QThread();

//    //    m_pHttpDownloader->moveToThread( m_pDownloaderWorkThread );

//    //    connect( m_pDownloaderWorkThread, &QThread::finished, m_pDownloaderWorkThread, &QObject::deleteLater );
//    //    connect( m_pDownloaderWorkThread, &QThread::finished, m_pHttpDownloader, &QObject::deleteLater );

//    ////    connect( this, &HttpConnection::signalStartHttpDownload,
//    ////             m_pHttpDownloader, &HttpDownloadThread::onStartHttpDownload );

//    ////    connect( m_pHttpDownloader, &HttpDownloadThread::signalDownloadFinished,
//    ////             this, &HttpConnection::onHttpDownloadFinished );

//    //    m_pDownloaderWorkThread->start();

//}

//void HttpConnection::initNetwork()
//{
//    //初始化 拼接各个接口的地址
//    m_sBaseUrl       =    QString::fromLocal8Bit( "http://172.16.16.16:9000/http-test/" );
//    m_sExFormData    =    QString::fromLocal8Bit( "form-data" );
//    m_sExRaw         =    QString::fromLocal8Bit( "body" );
//    m_sExParameter   =    QString::fromLocal8Bit( "param?id=" );
//    m_sExDownload    =    QString::fromLocal8Bit( "download?fileName=" );
//    m_pNetworkMgr    =    new QNetworkAccessManager( this );
//    connect( m_pNetworkMgr, &QNetworkAccessManager::finished, this, &HttpConnection::onNewNetworkReplyFinished );
//}

//void HttpConnection::postFormDataWithMultipart( QString filePath, QByteArray id )
//{
//    QFileInfo fileInfo( filePath );
//    if( !fileInfo.exists() || filePath.isEmpty() )
//    {
//        //        qDebug() << "path error, path = " << fileInfo.absoluteFilePath();
//        return;
//    }
//    QString fileName = fileInfo.fileName();
//    //    qDebug() << "file name = " << fileName;
//    //    qDebug() << "file path = " << fileInfo.absoluteFilePath();

//    //new一个稍后发送的multipart
//    QHttpMultiPart *pHttpMultiPart =    new QHttpMultiPart( QHttpMultiPart::FormDataType );
//    QHttpPart       filePart       =    loadFilePart( filePath );
//    QHttpPart       textPart       =    loadTextPart( id, "id" );

//    pHttpMultiPart->append( filePart );
//    pHttpMultiPart->append( textPart );

//    QNetworkRequest request;
//    request.setUrl( QUrl( m_sBaseUrl + m_sExFormData ) );
//    m_baRecvJsonData.clear();

//    m_pNetworkMgr->post( request, pHttpMultiPart );
//}

//void HttpConnection::postJsonInBody( QByteArray jsonData )
//{
//    QNetworkRequest request;
//    QByteArray      header      = QString( "Content-type" ).toLocal8Bit();
//    QByteArray      headerValue = QString( "application/json" ).toLocal8Bit();
//    request.setRawHeader( header, headerValue );
//    //    request.setUrl( QUrl( m_sBaseUrl + m_sExRaw ) );

//    setCurRequestUrl( m_structHttpInterface.baseIp + m_structHttpInterface.userIdentify );
//    request.setUrl( QUrl( m_sCurRequestUrl ) );

//    //  http://172.16.16.16:9000/api/base/user/identify
//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->post( request, jsonData );
//}

//void HttpConnection::postJsonInBodyWait( QByteArray jsonData )
//{
//    QNetworkRequest request;
//    QByteArray      header      = QString( "Content-type" ).toLocal8Bit();
//    QByteArray      headerValue = QString( "application/json" ).toLocal8Bit();
//    request.setRawHeader( header, headerValue );
//    //    request.setUrl( QUrl( "http://172.16.16.16:9000/api/base/user/identify" ) );
//    request.setUrl( QUrl( m_structHttpInterface.baseIp + m_structHttpInterface.userIdentify ) );
//    m_pNetworkMgr->post( request, jsonData );
//    QEventLoop loop;
//    //    connect( m_pNetworkMgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit );
//    connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//    loop.exec();
//    qDebug() << "synchronization http post!";
//}

//void HttpConnection::getWithParameter( QString id )
//{
//    QNetworkRequest request;
//    request.setUrl( m_sBaseUrl + m_sExParameter + id );
//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->get( request );
//}

//void HttpConnection::getWithoutParameter()
//{
//    QNetworkRequest request;
//    request.setUrl( m_sBaseUrl );
//    m_pNetworkMgr->get( request );
//}

//void HttpConnection::getWithParameterTimeStampWait( QString timeStamp )
//{
//    QNetworkRequest request;
//    //    QString strUrl = QString( "http://172.16.16.16:9000/api/base/user/sync?updateTime=" ) + timeStamp;
//    QString strUrl = QString( m_structHttpInterface.baseIp + m_structHttpInterface.userInfoSync + "?updateTime=" ) + timeStamp;
//    setCurRequestUrl( strUrl );
//    QUrl url( strUrl );
//    request.setUrl( url );
//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->get( request );
//    QEventLoop loop;
//    //    connect( m_pNetworkMgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit );
//    connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//    loop.exec();
//    qDebug() << " http get! wait  " << __FUNCTION__ << __LINE__;
//}

//void HttpConnection::getDownloadFile( QString fileName )
//{
//    QString downloadAddr = m_sBaseUrl + m_sExDownload + fileName;
//    initThread();
//    emit signalStartHttpDownload( downloadAddr );
//}

//void HttpConnection::destoryFilePointer()
//{
//    if( m_pFile != nullptr )
//    {
//        delete m_pFile;
//        m_pFile = nullptr;
//        qDebug() << "destory file pointer";
//        return;
//    }
//    qDebug() << "file pointer is nullPtr";
//}

//QHttpPart HttpConnection::loadFilePart( QString filePath )
//{
//    destoryFilePointer();
//    m_pFile = new QFile( filePath );
//    QFileInfo fileInfo( filePath );
//    if( !m_pFile->open( QIODevice::ReadWrite ) )
//    {
//        qDebug() << "file open failed";
//    }
//    QHttpPart filePart;
//    QString filePartHeader = QString::fromLocal8Bit( "form-data; name=\"files\"; filename=\"%1\"" ).arg( fileInfo.fileName() );
//    filePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( filePartHeader ) );
//    filePart.setBodyDevice( m_pFile );

//    return filePart;
//}

//QHttpPart HttpConnection::loadTextPart( QString content, QString key )
//{
//    QHttpPart textPart;
//    QString header = QString( "form-data; name=\"%1\"" ).arg( key );
//    textPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( header ) );
//    QString sBodyContent = QString( "%1" ).arg( content );
//    textPart.setBody( sBodyContent.toLocal8Bit() );
//    return  textPart;
//}

//QByteArray HttpConnection::createFaceFeatureAndCardNoJson( QLatin1String faceFeature, QString cardNo )
//{
//    //qDebug() << "create json = " <<faceFeature;
//    QJsonObject jsObj;
//    jsObj.insert( "faceFeature",  faceFeature  );
//    jsObj.insert( "cardNo", cardNo );

//    QJsonDocument jsDoc;
//    jsDoc.setObject( jsObj );
//    QByteArray jsData = jsDoc.toJson( QJsonDocument::Compact );
//    //qDebug() << "create json ret = " << QString::fromLocal8Bit( jsData );
//    return jsData;
//}

//void HttpConnection::setCurRequestUrl(QString url)
//{
//    m_sCurRequestUrl = url;
//}

//QString HttpConnection::getCurRequestUrl()
//{
//    return m_sCurRequestUrl;
//}

//QByteArray HttpConnection::getHttpReplyJsonData()
//{
//    return m_baRecvJsonData;
//}

//bool HttpConnection::checkStuInfoReplyCorrect( QByteArray jsonData )
//{
//    QJsonParseError error;
//    QJsonDocument jsDoc = QJsonDocument::fromJson( jsonData, &error );
//    if( error.error != QJsonParseError::NoError )
//    {
//        qDebug() << "json parse error";
//        return false;
//    }
//    QJsonObject jsObj = jsDoc.object();
//    if( !jsObj.contains( "code" ) )
//    {
//        qDebug() << "json value error...";
//        return false;
//    }
//    if( jsObj.take( "code" ).toVariant().toInt() == 0 )
//    {
//        return true;
//    }
//    return false;
//}

//QJsonObject HttpConnection::jsonDataToJsonObject( QByteArray jsonData )
//{
//    QJsonParseError error;
//    QJsonObject jsObj;
//    QJsonDocument jsDoc = QJsonDocument::fromJson( jsonData, &error );
//    if( error.error != QJsonParseError::NoError )
//    {
//        return jsObj;
//    }
//    jsObj = jsDoc.object();
//    return jsObj;
//}

//QMap< QString, QString > HttpConnection::parseRecvCurStudentInfo( QJsonObject jsObj )
//{
//    QMap< QString, QString > mapCurStuInfo;
//    if( !jsObj.contains( "data" ) )
//    {
//        qDebug() << "jsObj is error";
//        mapCurStuInfo.clear();
//        return mapCurStuInfo;
//    }
//    QJsonObject dataObj = jsObj.take( "data" ).toObject();
//    QJsonObject::iterator iter;
//    int count = 0;
//    for( iter = dataObj.begin(); iter != dataObj.end(); iter++ )
//    {
//        QString key = iter.key();
//        QJsonValue jsValue = iter.value();
//        mapCurStuInfo.insert( key, jsValue.toVariant().toString() );
//        count++;
//    }
//    return mapCurStuInfo;
//}

//QVariant HttpConnection::takeValueOfKeyFromJsonObject( QJsonObject jsObj, QString key )
//{
//    if( jsObj.isEmpty() )
//    {
//        qDebug() << "empty jsObj    " << __FUNCTION__ << __LINE__;
//        return QVariant();
//    }
//    if( jsObj.contains( key ) )
//    {
//        return jsObj.take( key ).toVariant();
//    }
//    qDebug() << "no this key    " + key << __FUNCTION__ << __LINE__;
//    return QVariant();
//}

//bool HttpConnection::parseRecvJsonUpDateUserInfo( vector<CUserInfo> &vec, vector<int> &deleteinfo, qint64 &timeStamp )
//{
//    QJsonObject jsObj = jsonDataToJsonObject( m_baRecvJsonData );
//    if( jsObj.isEmpty() )
//    {
//        qDebug() << " jsObj is empty    " << __FUNCTION__ << __LINE__;
//        return false;
//    }
//    int recvRet = takeValueOfKeyFromJsonObject( jsObj, "code" ).toInt();
//    if( recvRet != 0 )
//    {
//        qDebug() << "recv Ret error, code = " << recvRet << __FUNCTION__ << __LINE__;
//        return false;
//    }

//    QJsonObject dataObj = takeValueOfKeyFromJsonObject( jsObj, "data" ).toJsonObject();
//    QJsonArray deleteInfoArray = takeValueOfKeyFromJsonObject( dataObj, "deletedUser" ).toJsonArray();
//    QJsonArray userInfoArray = takeValueOfKeyFromJsonObject( dataObj, "userInfoList" ).toJsonArray();

//    //获取最大的时间戳
//    timeStamp = getMaxTimeStampFromJsonArray( userInfoArray );

//    //获取需要删除的信息
//    QJsonArray::iterator iter = deleteInfoArray.begin();
//    for( ;iter != deleteInfoArray.end(); iter++ )
//    {
//        deleteinfo.push_back(iter->toInt());
//    }

//    //提取每个学生的信息
//    vec = getEachUserInfo( userInfoArray ).toStdVector();
//    return true;
//}

//qint64 HttpConnection::getMaxTimeStampFromJsonArray( QJsonArray jsArray )
//{
//    if( jsArray.isEmpty() )
//    {
//        qDebug() << "jsArray is empty   " << __FUNCTION__ << __LINE__;
//        return 0;
//    }
//    set< qint64 > setUserInfoTimeStamp;
//    QJsonArray::iterator iter = jsArray.begin();
//    QJsonObject jsObj;
//    for( ; iter != jsArray.end(); iter++ )
//    {
//        jsObj = iter->toObject();
//        qint64 timeStamp = takeValueOfKeyFromJsonObject( jsObj, "updateTime" ).toLongLong();
//        setUserInfoTimeStamp.insert( timeStamp );
//    }
//    return *setUserInfoTimeStamp.rbegin();
//}

//QVector< CUserInfo > HttpConnection::getEachUserInfo( QJsonArray jsArray )
//{
//    QJsonArray::iterator jsArrayIter = jsArray.begin();
//    QJsonObject jsObj;
//    QVector< CUserInfo > vecUserInfos;

//    for( ; jsArrayIter != jsArray.end(); jsArrayIter++ )
//    {
//        CUserInfo user;
//        vecUserInfos.append( user );
//    }
//    QString count = QString::number( vecUserInfos.size() );
//    LOGINFO( "-------------------recv " + count.toStdString()  + " userinfos ------------------------" );
//    return vecUserInfos;
//}

//bool HttpConnection::sampleVideoSync( QString timeStamp, bool bBlock )
//{
//    QNetworkRequest request;
//    QString strUrl = m_structHttpInterface.baseIp +     \
//            m_structHttpInterface.videoSync +   \
//            QString::fromLocal8Bit( "?updateTime=%1" ).arg( timeStamp );

//    QUrl url( strUrl );
//    request.setUrl( url );

//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->get( request );
//    if( bBlock )
//    {
//        QEventLoop loop;
//        //        connect( m_pNetworkMgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit );
//        connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//        loop.exec();
//        return checkRequestSuccessful();
//    }

//    return true;
//}

//QVector< structSampleVideoSync > HttpConnection::parseSampleVideoSync()
//{
//    QJsonObject jsObj;
//    QVector< structSampleVideoSync >  vecVideoUrls;
//    jsObj= jsonDataToJsonObject( m_baRecvJsonData );
//    if( jsObj.isEmpty() )
//    {
//        vecVideoUrls.clear();
//        return  vecVideoUrls;
//    }

//    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, "data" ).toJsonArray();
//    QJsonArray::iterator iter = dataArray.begin();
//    for( ; iter != dataArray.end(); iter++ )
//    {
//        structSampleVideoSync sampleVideoInfo;
//        QJsonObject dataObj = iter->toObject();
//        sampleVideoInfo.type = takeValueOfKeyFromJsonObject( dataObj, "type" ).toString();
//        sampleVideoInfo.url = takeValueOfKeyFromJsonObject( dataObj, "url" ).toString();
//        sampleVideoInfo.upDateTime = takeValueOfKeyFromJsonObject( dataObj, "updateTime" ).toString();
//        vecVideoUrls.append( sampleVideoInfo );
//    }
//    QString log = QString( "sync %1 videos..." ).arg( vecVideoUrls.count() );
//    LOGINFO( log.toStdString() );
//    return vecVideoUrls;
//}

//bool HttpConnection::scoreStanderdSync( bool bBlock )
//{
//    QNetworkRequest request;
//    QString strUrl = m_structHttpInterface.baseIp + m_structHttpInterface.scoreStandardSync;
//    QUrl url( strUrl );
//    request.setUrl( url );

//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->get( request );
//    if( bBlock )
//    {
//        QEventLoop loop;
//        //        connect( m_pNetworkMgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit );
//        connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//        loop.exec();

//        return checkRequestSuccessful();
//    }

//    return true;
//}

//QVector<CScoreRule> HttpConnection::parseScoreStanderdSync()
//{
//    QJsonObject jsObj;
//    QVector< CScoreRule >  vecSocreRule;
//    jsObj= jsonDataToJsonObject( m_baRecvJsonData );
//    if( jsObj.isEmpty() )
//    {
//        vecSocreRule.clear();
//        return  vecSocreRule;
//    }

//    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, "data" ).toJsonArray();
//    QJsonArray::iterator iter = dataArray.begin();
//    for( ; iter != dataArray.end(); iter++ )
//    {
//        CScoreRule rule;
//        QJsonObject dataObj = iter->toObject();
//        rule.m_nItem = takeValueOfKeyFromJsonObject( dataObj, "testItem" ).toInt();
//        rule.m_bGender = takeValueOfKeyFromJsonObject( dataObj, "gender" ).toBool();
//        rule.m_fValLeft = takeValueOfKeyFromJsonObject( dataObj, "resultStart" ).toFloat();
//        rule.m_fValRight = takeValueOfKeyFromJsonObject( dataObj, "resultEnd" ).toFloat();
//        rule.m_fScore = takeValueOfKeyFromJsonObject( dataObj, "score" ).toFloat();
//        rule.m_nGrade = takeValueOfKeyFromJsonObject( dataObj, "grade" ).toInt();
//        vecSocreRule.append( rule );
//    }

//    return vecSocreRule;
//}

//bool HttpConnection::classInfoSync( bool bBlock )
//{
//    QNetworkRequest request;
//    request.setUrl( m_structHttpInterface.baseIp + m_structHttpInterface.classInfoSync );
//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->get( request );
//    if( bBlock )
//    {
//        QEventLoop loop;
//        connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//        loop.exec();
//        return checkRequestSuccessful();
//    }
//    return true;
//}

//QVector<CClassInfo> HttpConnection::parseClassInfo()
//{
//    QJsonObject jsObj = jsonDataToJsonObject( m_baRecvJsonData );
//    QVector< CClassInfo > vecClassInfos;
//    if( jsObj.isEmpty() )
//    {
//        vecClassInfos.clear();
//        return vecClassInfos;
//    }
//    QJsonObject jsData = takeValueOfKeyFromJsonObject( jsObj, "data" ).toJsonObject();
//    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsData, "clazzInfoList" ).toJsonArray();
//    QJsonArray::iterator iter = dataArray.begin();
//    for( ; iter != dataArray.end(); iter++ )
//    {
//        QJsonObject dataObj = iter->toObject();
//        CClassInfo classInfo;
//        classInfo.m_nUniqueID = takeValueOfKeyFromJsonObject( dataObj, "id" ).toInt();
//        classInfo.m_strClassDescribe = takeValueOfKeyFromJsonObject( dataObj, "name" ).toString().toStdString();
//        vecClassInfos.append( classInfo );
//    }

//    return vecClassInfos;
//}

//bool HttpConnection::queryHistoryResult( structRequestHistory requestHistory, bool bBlock )
//{
//    QString userId =  "?userId=" + requestHistory.userId;
//    QString testItem = "&testItem=" +requestHistory.testItem;
//    QString timeInterval = "&timeInterval=" + requestHistory.timeInterval;
//    QString type = "&type=" + requestHistory.type;

//    QNetworkRequest request;
//    QString preFixRequestUrl = m_structHttpInterface.baseIp + m_structHttpInterface.getHistoryScore;
//    QString sufFixRequestUrl = userId + testItem + timeInterval + type;
//    request.setUrl( preFixRequestUrl + sufFixRequestUrl );
//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->get( request );
//    if( bBlock )
//    {
//        QEventLoop loop;
//        //        connect( m_pNetworkMgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit );
//        connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//        loop.exec();

//        return checkRequestSuccessful();
//    }

//    return true;
//}


//QVector<structHistory> HttpConnection::parseHistory()
//{
//    QVector< structHistory > vecContent;
//    vecContent.clear();
//    QJsonObject jsObj = jsonDataToJsonObject( m_baRecvJsonData );
//    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, "data" ).toJsonArray();
//    QJsonArray::iterator iter = dataArray.begin();
//    for( ;iter != dataArray.end(); iter++ )
//    {
//        structHistory historyRecord;
//        QJsonObject dataObj = iter->toObject();
//        historyRecord.testItem = takeValueOfKeyFromJsonObject( dataObj, "testItem" ).toString();
//        historyRecord.recordTime = takeValueOfKeyFromJsonObject( dataObj, "recordTime" ).toString();
//        historyRecord.record = takeValueOfKeyFromJsonObject( dataObj, "record" ).toString();
//        historyRecord.score = takeValueOfKeyFromJsonObject( dataObj, "score" ).toString();
//        vecContent.append( historyRecord );
//    }
//    return  vecContent;
//}

//void HttpConnection::onSendCurTestRetToServerWait( structUpDateTestResultDate upDateInfo )
//{
//    QHttpMultiPart *multiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType, this );

//    //用户ID
//    QHttpPart userIdPart;
//    userIdPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"userId\"" ) );
//    userIdPart.setBody( upDateInfo.userId.toLocal8Bit() );

//    //体测类型， 练习模式还是考试模式
//    QHttpPart typePart;
//    typePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"type\"" ) );
//    typePart.setBody( upDateInfo.type.toLocal8Bit() );

//    //体测项目
//    QHttpPart testItemPart;
//    testItemPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"testItem\"" ) );
//    testItemPart.setBody( upDateInfo.testItem.toLocal8Bit() );

//    //体测成绩
//    QHttpPart resultPart;
//    resultPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"result\"" ) );
//    resultPart.setBody( upDateInfo.result.toLocal8Bit() );

//    //体测时间
//    QHttpPart recordTimePart;
//    recordTimePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"recordTime\"" ) );
//    recordTimePart.setBody( upDateInfo.recordTime.toLocal8Bit() );

//    //设备类型
//    QHttpPart deviceTypePart;
//    deviceTypePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"deviceType\"" ) );
//    deviceTypePart.setBody( upDateInfo.deviceType.toLocal8Bit() );

//    //设备编号
//    QHttpPart deviceCodePart;
//    deviceCodePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"deviceCode\"" ) );
//    deviceCodePart.setBody( upDateInfo.devicedCode.toLocal8Bit() );

//    //签名路径
//    if( !CFilePath::isFileExist( upDateInfo.signPath.toStdString() ) ){
//        qDebug() << QString::fromLocal8Bit( "签名路径无效...\n->" + upDateInfo.signPath.toLocal8Bit() );
//    }else {
//        QHttpPart signPart;
//        QString signHeader = QString( "for-data; name=\"signPic\"; filename=\"%1\"" ).arg( upDateInfo.signPath );
//        QFile *signPicFile = new QFile( this );
//        if( !signPicFile->open( QIODevice::ReadOnly ) ){
//            qDebug() << "sign picture file open failed...";
//            signPicFile->close();
//            delete signPicFile;
//            signPicFile = nullptr;
//            return;
//        }
//        signPart.setHeader( QNetworkRequest::ContentDispositionHeader, signHeader );
//        signPart.setBodyDevice( signPicFile );
//        signPicFile->setParent( multiPart );
//        multiPart->append( signPart );
//    }

//    //过程照片路径
//    if( !upDateInfo.filePaths.isEmpty() ){

//        for( auto path : upDateInfo.filePaths ){
//            if( path.isEmpty() || !CFilePath::isFileExist( path.toStdString() )){
//                continue;
//            }
//            QHttpPart filePart;
//            QString header = QString( "form-data; name=\"files\"; filename=\"%1\"" ).arg( path );
//            qDebug() << "process picture path = " << path;
//            QFile *file = new QFile( path );
//            //打开文件
//            if( !file->open( QIODevice::ReadOnly ) ){
//                qDebug() << "file open failed...";
//                file->close();
//                delete file;
//                file = nullptr;
//                return;
//            }
//            filePart.setHeader( QNetworkRequest::ContentDispositionHeader, header );
//            filePart.setBodyDevice( file );
//            file->setParent( multiPart );
//            multiPart->append( filePart );
//        }
//    }else{
//        qDebug() << "process picture path is null";
//    }


//    multiPart->append( userIdPart );
//    multiPart->append( typePart );
//    multiPart->append( testItemPart );
//    multiPart->append( resultPart );
//    multiPart->append( recordTimePart );
//    multiPart->append( deviceTypePart );
//    multiPart->append( deviceCodePart );

//    QNetworkRequest request;
//    //    request.setUrl( QUrl( "http://172.16.16.16:9000/api/standard/device-record" ) );
//    LOGINFO( "上传成绩的请求链接 url = " + m_structHttpInterface.baseIp.toStdString() + m_structHttpInterface.upDateTestRet.toStdString() );
//    QString addr = m_structHttpInterface.baseIp + m_structHttpInterface.upDateTestRet;
//    qDebug() <<"reqeust addr = " << addr;
//    request.setUrl( QUrl( m_structHttpInterface.baseIp + m_structHttpInterface.upDateTestRet ) );

//    //创建超时定时器,超时就丢弃此处请求
//    QTimer timeoutTimer;
//    timeoutTimer.setSingleShot( true );
//    connect( &timeoutTimer, &QTimer::timeout, [=](){
//        qDebug() << "ERROR->>>> upDateScore timeout...";
//        m_pReply->abort();

//    });

//    m_baRecvJsonData.clear();
//    m_pReply = m_pNetworkMgr->post( request, multiPart );
//    QEventLoop loop;
//    timeoutTimer.start( m_iHttpTimeout );
//    qDebug() << "timeout timer is start...";

//    connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//    loop.exec();
//    if( timeoutTimer.isActive() )
//    {
//        qDebug() << "INFO->>>> get http reply, timer is no timeout!!!";
//        timeoutTimer.stop();
//    }
//}

//void HttpConnection::onSendCurTestToServerForHumanFormMeasure( structUpdateHumanFormMeasure humanMeasure )
//{
//    QHttpMultiPart *multiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType );

//    QHttpPart userIdPart;
//    userIdPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"userId\"" ) );
//    userIdPart.setBody( humanMeasure.userId.toLocal8Bit() );

//    QHttpPart deviceCodePart;
//    deviceCodePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"deviceCode\"" ) );
//    deviceCodePart.setBody( humanMeasure.deviceCode.toLocal8Bit( ) );

//    QHttpPart deviceTypePart;
//    deviceTypePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"deviceType\"" ) );
//    deviceTypePart.setBody( humanMeasure.deviceType.toLocal8Bit( ) );

//    //人体成分信息字符串
//    QHttpPart humanBodyCompositionPart;
//    //拼接人体成分信息字符串
//    QString humanCompostion = getHumanComposition( humanMeasure );
//    LOGINFO("##############" + humanCompostion.toStdString());
//    humanBodyCompositionPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"recordListStr\"" ) );
//    humanBodyCompositionPart.setBody( humanCompostion.toLocal8Bit( ) );

//    QHttpPart recordTimePart;
//    recordTimePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"recordTime\"" ) );
//    recordTimePart.setBody( humanMeasure.timeStamp.toLocal8Bit( ) );

//    QHttpPart filesPart;
//    filesPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"files\"" ) );
//    filesPart.setBody( humanMeasure.testingPicturesPath.toLocal8Bit() );

//    multiPart->append( userIdPart );
//    multiPart->append( deviceCodePart );
//    multiPart->append( deviceTypePart );
//    multiPart->append( recordTimePart );
//    multiPart->append( humanBodyCompositionPart );

//    QNetworkRequest request;
//    request.setUrl( QUrl( m_structHttpInterface.baseIp + m_structHttpInterface.upDateHumanFormMeasure ) );

//    //创建超时定时器,超时就丢弃此处请求
//    QTimer timeoutTimer;
//    timeoutTimer.setSingleShot( true );
//    connect( &timeoutTimer, &QTimer::timeout, [=](){
//        qDebug() << "ERROR->>>> upDateScore timeout...";
//        m_pReply->abort();

//    });

//    m_baRecvJsonData.clear();
//    m_pReply = m_pNetworkMgr->post( request, multiPart );

//    QEventLoop loop;
//    timeoutTimer.start( m_iHttpTimeout );
//    qDebug() << "timeout timer is start...";

//    connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//    loop.exec();

//    if( timeoutTimer.isActive() )
//    {
//        qDebug() << "INFO->>>> get http reply, timer is no timeout!!!";
//        timeoutTimer.stop();
//    }

//    //
//    multiPart->deleteLater();
//}

//QString HttpConnection::getHumanComposition(structUpdateHumanFormMeasure humanMeasure)
//{
//    QString humanCompostion;

//    humanCompostion += humanMeasure.humanHeight.size() == 0 ? "" : QString( "1=" + humanMeasure.humanHeight );
//    humanCompostion += humanMeasure.humanWeight.size() == 0 ? "" : QString( "@2=" + humanMeasure.humanWeight );
//    humanCompostion += humanMeasure.fatRate.size() == 0 ? "" : QString( "@3=" + humanMeasure.fatRate );
//    humanCompostion += humanMeasure.BMI.size() == 0 ? "" : QString( "@9=" + humanMeasure.BMI );
//    humanCompostion += humanMeasure.headWeight.size() == 0 ? "" : QString( "@10=" + humanMeasure.headWeight );
//    humanCompostion += humanMeasure.headFat.size() == 0 ? "" : QString( "@11=" + humanMeasure.headFat );
//    humanCompostion += humanMeasure.headMuscle.size() == 0 ? "" : QString( "@12=" + humanMeasure.headMuscle );
//    humanCompostion += humanMeasure.TBMM.size() == 0 ? "" : QString( "@13=" + humanMeasure.TBMM );
//    humanCompostion += humanMeasure.WBIS.size() == 0 ? "" : QString( "@14=" + humanMeasure.WBIS );
//    humanCompostion += humanMeasure.TBW.size() == 0 ? "" : QString( "@15=" + humanMeasure.TBW );
//    humanCompostion += humanMeasure.WHR.size() == 0 ? "" : QString( "@16=" + humanMeasure.WHR );
//    humanCompostion += humanMeasure.VFA.size() == 0 ? "" : QString( "@17=" + humanMeasure.VFA );

//    return humanCompostion;

//}

//bool HttpConnection::onSendHeartBeat( structDevHeartBeat heartBeatInfo, bool bBlock )
//{
//    QNetworkRequest request;

//    //创建心跳包
//    QByteArray sendHeatBeatInfo = createHeartBeatJson( heartBeatInfo );

//    //stsatus为空时，sendHeartBeat为空
//    if( sendHeatBeatInfo.isEmpty() )
//    {
//        qDebug() << "heart beat pack is empty...";
//        return false;
//    }

//    QByteArray headerName = QString( "Content-type" ).toLocal8Bit();
//    request.setRawHeader( headerName, QString( "application/json" ).toLocal8Bit() );

//    request.setUrl( QUrl( m_structHttpInterface.baseIp  + m_structHttpInterface.upDataHeartBeat ) );

//    //    disconnect( m_pNetworkMgr, &QNetworkAccessManager::finished,
//    //                this, &HttpConnection::onNewNetworkReplyFinished );

//    //    QNetworkReply *pReply = m_pNetworkMgr->post( request, sendHeatBeatInfo );
//    m_baRecvJsonData.clear();
//    m_pReply = m_pNetworkMgr->post( request, sendHeatBeatInfo );
//    QTimer timeoutTimer;
//    QEventLoop loop;
//    timeoutTimer.setSingleShot( true );
//    //    timeoutTimer.start( m_iHttpTimeout );

//    connect( &timeoutTimer, &QTimer::timeout, [=]()
//    {
//        qDebug() << QString::fromLocal8Bit( "请求超时,即将丢弃此次请求..." );
//        m_pReply->abort();
//        qDebug() << QString::fromLocal8Bit( "超时退出..." );
//        return false;
//    });

//    if( bBlock )
//    {
//        timeoutTimer.start( m_iHttpTimeout );
//        connect( &timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit );
//        connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//        loop.exec();
//        if( timeoutTimer.isActive() )
//        {
//            qDebug() << QString::fromLocal8Bit( "未超时" );
//            timeoutTimer.stop();
//        }else
//        {
//            qDebug() << QString::fromLocal8Bit( "定时器超时..." );
//        }

//        bool ok = checkRequestSuccessful();
//        return ok;
//    }

//    return true;

//}

//bool HttpConnection::singalCheck(QByteArray data)
//{
//    qDebug() << "singalCheck data = " << QString::fromLocal8Bit( data );
//    QString str =  QString::fromLocal8Bit( data );
//    QJsonObject jsObj = jsonDataToJsonObject( data );
//    if( jsObj.isEmpty() )
//    {
//        return false;
//    }
//    //判断是否请求成功
//    int ret = takeValueOfKeyFromJsonObject( jsObj, "code" ).toInt();
//    if( ret != 0 )
//    {
//        return false;
//    }
//    return true;
//}

//bool HttpConnection::checkRequestSuccessful()
//{
//    //若值为默认值表明没有
//    m_qsLastErr = "false";
//    m_nLastErr = -1;

//    QString str =  QString::fromUtf8( m_baRecvJsonData );
//    QJsonObject jsObj = jsonDataToJsonObject( m_baRecvJsonData );

//    if( jsObj.isEmpty() )
//    {
//        LOGINFO("invalid json" + str.toStdString());
//        qDebug() << "invalid json" << __FUNCTION__ << __LINE__;
//        return false;
//    }
//    //判断是否请求成功
//    int ret = takeValueOfKeyFromJsonObject( jsObj, "code" ).toInt();

//    //code:30004 重复提交直接返回true
//    if( ret != 0 && ret != 30004)
//    {
//        LOGINFO( "update test result Http request failed( code != 0 )..." );
//        LOGINFO( QString( "http request failed, recv json = " ).toStdString() +
//                 str.toLocal8Bit().toStdString() );

//        qDebug() << "Http request failed..." << __FUNCTION__ << __LINE__;
//        qDebug() << "error json = " << QString::fromUtf8( m_baRecvJsonData );
//        m_nLastErr = ret;
//        m_qsLastErr = takeValueOfKeyFromJsonObject( jsObj, "msg" ).toString().toLocal8Bit();
//        return false;
//    }

//    qDebug() << "Http request success" << __FUNCTION__ << __LINE__ << "\n" << "recv json = " << str << "\n";
//    LOGINFO( "recv json = " + str.toLocal8Bit().toStdString() );
//    return true;
//}

//int HttpConnection::getLastErrorRequest(QString &qsLastError)
//{
//    qsLastError = m_qsLastErr;
//    return m_nLastErr;
//}

//structClassInfo HttpConnection::queryClassInfoByClassID(int classID)
//{
//    structClassInfo classInfo;
//    QString strClassID = QString::number( classID );
//    QString strUrl = m_structHttpInterface.baseIp + m_structHttpInterface.queryClassInfo + strClassID;
//    QUrl url( strUrl );

//    QNetworkRequest request;
//    request.setUrl( url );
//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->get( request );

//    QEventLoop loop;
//    //    connect( m_pNetworkMgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit );
//    connect( this, &HttpConnection::signalFinish, &loop, &QEventLoop::quit );
//    loop.exec();

//    classInfo.name = getClassName();
//    return  classInfo;

//}

//QString HttpConnection::getClassName()
//{
//    QJsonDocument jsDoc = QJsonDocument::fromJson( m_baRecvJsonData );
//    QJsonObject jsObj = jsDoc.object();
//    QString className;
//    if( jsObj.contains("data"))
//    {

//        QJsonObject dataObj  = takeValueOfKeyFromJsonObject(jsObj, "data").toJsonObject();
//        className = takeValueOfKeyFromJsonObject( dataObj, "name" ).toString();
//    }
//    return className;
//}

//QByteArray HttpConnection::createHeartBeatJson( structDevHeartBeat heartBeatInfo )
//{
//    QString code = heartBeatInfo.code;
//    QString type = heartBeatInfo.type;
//    QString status = heartBeatInfo.status;
//    QString battery = heartBeatInfo.battery;

//    QJsonObject jsObj;
//    if( !code.isNull() )
//    {
//        jsObj.insert( "code", code );
//    }
//    if( !type.isNull() && type != "0" )
//    {
//        jsObj.insert( "type", type );
//    }
//    if( !battery.isNull() )
//    {
//        jsObj.insert( "battery", battery );
//    }

//    //status 不能为空
//    if( status.isNull() )
//    {
//        LOGINFO( "设备状态错误?   CHttpConnection::CreateHeartBeat" )
//                return QByteArray();
//    }

//    jsObj.insert( "status", status );

//    QJsonDocument jsDoc;
//    jsDoc.setObject( jsObj );
//    return jsDoc.toJson();
//}

//void HttpConnection::readConfigFile()
//{
//    QString configFilePath = QApplication::applicationDirPath() + "/cfg/httpConfig.txt";
//    QFileInfo fileInfo( configFilePath );

//    if( m_pSettingConfiger == nullptr )
//    {
//        m_pSettingConfiger = new QSettings( configFilePath, QSettings::IniFormat, this );
//    }

//    //如果配置文件不存在, 则创建默认配置文件
//    if( !fileInfo.exists() )
//    {
//        createDefaultConfigFile();
//    }

//    m_structHttpInterface.baseIp    = m_pSettingConfiger->value( "http/baseIp" ).toString();
//    m_structHttpInterface.videoSync = m_pSettingConfiger->value( "http/videoSync" ).toString();
//    m_structHttpInterface.scoreStandardSync = m_pSettingConfiger->value( "http/scoreStandardSync" ).toString();
//    m_structHttpInterface.userInfoSync = m_pSettingConfiger->value( "http/userInfoSync" ).toString();
//    m_structHttpInterface.upDateTestRet = m_pSettingConfiger->value( "http/upDateTestRet" ).toString();
//    m_structHttpInterface.upDataHeartBeat = m_pSettingConfiger->value( "http/upDataHeartBeat" ).toString();
//    m_structHttpInterface.getHistoryScore = m_pSettingConfiger->value( "http/getHistoryScore" ).toString();
//    m_structHttpInterface.classInfoSync = m_pSettingConfiger->value( "http/classInfoSync" ).toString();
//    m_structHttpInterface.upDateHumanFormMeasure = m_pSettingConfiger->value( "http/upDateHumanFormMeasure" ).toString();
//    m_structHttpInterface.userIdentify = m_pSettingConfiger->value( "http/userIdentify" ).toString();
//    m_structHttpInterface.touristUsageRecord = m_pSettingConfiger->value( "http/touristUsageRecord" ).toString();
//    m_structHttpInterface.queryClassInfo = m_pSettingConfiger->value( "http/queryClassInfo" ).toString();
//    m_iHttpTimeout = m_pSettingConfiger->value( "http/timeout" ).toInt();
//}

//void HttpConnection::createDefaultConfigFile()
//{
//    QString configFilePath = QApplication::applicationDirPath() + "/cfg/httpConfig.txt";
//    if( m_pSettingConfiger == nullptr )
//    {
//        m_pSettingConfiger = new QSettings( configFilePath, QSettings::IniFormat, this );
//    }

//    m_pSettingConfiger->beginGroup( "http" );
//    m_pSettingConfiger->setValue( "baseIp", "http://172.16.10.182:7701" );
//    m_pSettingConfiger->setValue( "videoSync", "/api/standard/sample-video/sync" );
//    m_pSettingConfiger->setValue( "scoreStandardSync", "/api/standard/sport-rule-detail/sync" );
//    m_pSettingConfiger->setValue( "userInfoSync", "/api/base/user/sync" );
//    m_pSettingConfiger->setValue( "upDateTestRet", "/api/standard/device-record" );
//    m_pSettingConfiger->setValue( "upDataHeartBeat", "/api/standard/device/heart" );
//    m_pSettingConfiger->setValue( "getHistoryScore", "/api/standard/device-record/student-history-record" );
//    m_pSettingConfiger->setValue( "classInfoSync", "/api/base/clazz/sync" );
//    m_pSettingConfiger->setValue( "upDateHumanFormMeasure", "/api/standard/device-record/bc-data" );
//    m_pSettingConfiger->setValue( "userIdentify", "/api/base/user/identify" );
//    m_pSettingConfiger->setValue( "touristUsageRecord", "/api/standard/device-usage/visitor" );
//    m_pSettingConfiger->setValue( "timeout", "5000" );
//    m_pSettingConfiger->setValue( "queryClassInfo", "/api/base/clazz/" );
//    m_pSettingConfiger->endGroup();
//}

//void HttpConnection::downloadFromUrl( QString strUrl )
//{
//    QUrl url( strUrl );
//    QNetworkRequest request;
//    request.setUrl( url );
//}

//void HttpConnection::recordTouristUsageToServer( QString devCode, QString devType )
//{
//    if( devCode.isEmpty() || devType.isEmpty() ){
//        return;
//    }
//    QJsonObject jsObj;
//    QString timeStamp = QString::number( QDateTime::currentDateTime().toMSecsSinceEpoch() );
//    jsObj.insert( "code", devCode );
//    jsObj.insert( "type", devType );
//    jsObj.insert( "createTime", timeStamp );

//    QJsonDocument jsDoc;
//    jsDoc.setObject( jsObj );
//    QByteArray jsData = jsDoc.toJson( QJsonDocument::Compact );

//    QNetworkRequest request;
//    QByteArray headerName = QString( "Content-type" ).toLocal8Bit();
//    request.setRawHeader( headerName, QString( "application/json" ).toLocal8Bit() );
//    QString qsUrl =  m_structHttpInterface.baseIp + m_structHttpInterface.touristUsageRecord;
//    request.setUrl( QUrl( qsUrl ));

//    m_baRecvJsonData.clear();
//    m_pNetworkMgr->post( request, jsData );
//}

//void HttpConnection::onNewNetworkReplyFinished( QNetworkReply *pReply )
//{
//    if( pReply->error() != QNetworkReply::NoError )
//    {
//        qDebug() << "reply error!!! error string = " << QString::fromUtf8( pReply->errorString().toUtf8() );
//        emit signalFinish();
//        return;
//    }

//    emit upDateLastTime();

//    m_baRecvJsonData.clear();
//    m_baRecvJsonData = pReply->readAll();
//    QByteArray baTmp = m_baRecvJsonData;
//    LOGINFO("orginal recv json = " + QString::fromUtf8( baTmp ).toStdString())
//            qDebug() << "orginal recv json = " << QString::fromUtf8( baTmp );
//    emit signalFinish();
//}

//void HttpConnection::onHttpDownloadFinished()
//{
//    if( m_pDownloaderWorkThread != nullptr )
//    {
//        m_pDownloaderWorkThread->quit();
//    }
//    m_pDownloaderWorkThread->wait();
//}
