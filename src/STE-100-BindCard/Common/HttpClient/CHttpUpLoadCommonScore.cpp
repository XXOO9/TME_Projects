#include "CHttpUpLoadCommonScore.h"

CHttpUpLoadCommonScore::CHttpUpLoadCommonScore( QObject *parent ) : CHttpAbstractNetWork ( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }
    initConfig();
    initConnection();
}

CHttpUpLoadCommonScore::~CHttpUpLoadCommonScore()
{

}

void CHttpUpLoadCommonScore::initConfig()
{
    setInterfaceType( requestType::UPLOADSCORECOMMON );
    CHttpConfigReaderSingeton *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_UPLOADSOCRE_KEY );
    QString urlString = baseIp + interface;
    setUrl( urlString );
}

void CHttpUpLoadCommonScore::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpUpLoadCommonScore::onNewReplyAvailable );
    connect( this, &CHttpUpLoadCommonScore::sigTimeout, this, &CHttpUpLoadCommonScore::onTimeoutHandler );
}

void CHttpUpLoadCommonScore::httpRequest()
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }

    if( isParameterEmpty() ){
        return;
    }
//    qDebug() << "upload common score url = " << request().url().toString();
    m_pReply = m_pNetWorkMgr->post( request(), m_pMultiPart );
    startTimer( TIMEOUT_LONG );
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

void CHttpUpLoadCommonScore::setDeviceType( DEV_TYPE deviceType )
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

void CHttpUpLoadCommonScore::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
        qDebug() << "error string = " << pReply->errorString();
        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
    this->setRecvJsonData( recvData );
    bool ok = parseJson();
    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadCommonScore::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
