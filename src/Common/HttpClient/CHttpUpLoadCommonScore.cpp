#include "CHttpUpLoadCommonScore.h"
#include "../Config/CDatabaseManage.h"
CHttpUpLoadCommonScore::CHttpUpLoadCommonScore( QObject *parent ) : CHttpAbstractNetWork ( parent )
{
    m_strNetWorkName = "CHttpUpLoadCommonScore";

    initConfig();
    initConnection();
}

CHttpUpLoadCommonScore::~CHttpUpLoadCommonScore()
{

}

void CHttpUpLoadCommonScore::initConfig()
{
    setInterfaceType( requestType::UPLOADSCORECOMMON );
    setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getUpdateTestResultFuncUrl()) );
}

void CHttpUpLoadCommonScore::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    m_pReply = m_pNetWorkMgr->post( request(), m_pMultiPart );
    startTimeoutTimer( UPLOAD_TIMEOUT );
}

bool CHttpUpLoadCommonScore::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUpLoadCommonScore::isParameterEmpty()
{
    if( nullptr == m_pMultiPart ){
        return true;
    }
    return false;
}

void CHttpUpLoadCommonScore::setUserId(QString userId)
{
    this->appendTextPart( upLoadCommonScore::userId, userId );
}

void CHttpUpLoadCommonScore::setScoreType(testType type)
{
    QString strType = QString::number( type, 10 );
    this->appendTextPart( upLoadCommonScore::socreType, strType );
}

void CHttpUpLoadCommonScore::setTestItem(TEST_ITEM testItem)
{
    QString strTestItem = QString::number( testItem, 10 );
    this->appendTextPart( upLoadCommonScore::testItem, strTestItem );
}

void CHttpUpLoadCommonScore::setResult(QString result)
{
    this->appendTextPart( upLoadCommonScore::result, result );
}

void CHttpUpLoadCommonScore::setRecordTime(QString recordTime)
{
    this->appendTextPart( upLoadCommonScore::recordTime, recordTime );
}

void CHttpUpLoadCommonScore::setProcessPics(QStringList paths)
{
    if( paths.isEmpty() ){
        return;
    }

    for( auto &eachFile : paths ){
        bool ok = this->appendFilePart( eachFile );
        if( !ok ){
            continue;
        }
    }
}

void CHttpUpLoadCommonScore::setDeviceCode(QString deviceCode)
{
    this->appendTextPart( upLoadCommonScore::deviceCode, deviceCode );
}

void CHttpUpLoadCommonScore::setDeviceType(short deviceType )
{
    QString strDevType = QString::number( deviceType, 10 );
    this->appendTextPart( upLoadCommonScore::deviceType, strDevType );
}

void CHttpUpLoadCommonScore::setSignPicPath(QString signPath)
{
    if( signPath.isEmpty() ){
        return;
    }
    this->appendSignPart( signPath );
}

void CHttpUpLoadCommonScore::setParaMap(QMap<QString, QVariant> infos)
{
    QString  userId           =    infos.value( HTTP_KEYS::userId ).toString();
    QString  devCode          =    infos.value( HTTP_KEYS::devCode ).toString();
    short    devType          =    short(infos.value( HTTP_KEYS::devType ).toInt());
    testType scoreType        =    static_cast<testType>(infos.value( HTTP_KEYS::type ).toInt());
    TEST_ITEM testItem        =    static_cast<TEST_ITEM>(infos.value( HTTP_KEYS::testItem ).toInt());
    QString result            =    infos.value( HTTP_KEYS::result ).toString();
    QString recordTime        =    infos.value( HTTP_KEYS::recordTime ).toString();
    QStringList files         =    infos.value( HTTP_KEYS::files ).toStringList( );
    QString signPic           =    infos.value( HTTP_KEYS::signPic ).toString();

    setUserId( userId );
    setDeviceCode( devCode );
    setDeviceType( devType );
    setScoreType( scoreType );
    setTestItem( testItem );
    setResult( result );
    setRecordTime( recordTime );

//    setProcessPics( files );
//    setSignPicPath( signPic );
}

void CHttpUpLoadCommonScore::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }
    m_pMultiPart->setParent( pReply );

    if( QNetworkReply::NoError != pReply->error() ){
        QString errorString = pReply->errorString();

#if QT_NO_DEBUG
        QString logString = ">>>>>>>>>>>>>>>upload single common error string( not from server ) = " + errorString;
        LOGERROR( logString.toStdString() );
#else
#endif

        if( errorString.isEmpty() ){
            errorString = "No Error string";
        }
        setErrType( REQUEST_ERROR, errorString );
        emit sigRequestRetAvailable( false );
        qDebug() << "error string = " << errorString;
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
    LOGINFO(  "upload single common score, recv from server string" + recvString.toStdString() );
#else
    qDebug() << "upload singal score ret = " << recvString;
#endif
    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    LOGINFO( "###################  parse json result = " + QString::number( ok ).toStdString() );



    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadCommonScore::onTimeoutHandler()
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
