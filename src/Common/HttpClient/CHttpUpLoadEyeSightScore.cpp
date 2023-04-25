#include "CHttpUpLoadEyeSightScore.h"
#include "../Config/CDatabaseManage.h"
CHttpUpLoadEyeSightScore::CHttpUpLoadEyeSightScore(QObject *parent) : CHttpAbstractNetWork ( parent )
{
    m_strNetWorkName = "CHttpUpLoadEyeSightScore";

    initConfig();
    initConnection();
}

CHttpUpLoadEyeSightScore::~CHttpUpLoadEyeSightScore()
{

}

void CHttpUpLoadEyeSightScore::initConfig()
{
    setInterfaceType( requestType::UPLOADEYESIGHT );
    QString url = QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getUpLoadEyeSightFuncUrl());
    setUrl( url );
}

void CHttpUpLoadEyeSightScore::httpRequest()
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

bool CHttpUpLoadEyeSightScore::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUpLoadEyeSightScore::isParameterEmpty()
{
    if( nullptr == m_pMultiPart ){
        return true;
    }
    return false;
}

void CHttpUpLoadEyeSightScore::setUserId(QString userId)
{
    this->appendTextPart( upLoadEyeScore::userId, userId );
}

void CHttpUpLoadEyeSightScore::setDeviceCode(QString devCode)
{
    this->appendTextPart( upLoadEyeScore::deviceCode, devCode );
}

void CHttpUpLoadEyeSightScore::setDeviceType(short devType)
{
    QString strDevType = QString::number( devType, 10 );
    this->appendTextPart( upLoadEyeScore::deviceType, strDevType );
}

void CHttpUpLoadEyeSightScore::setRecordTime(QString recordTime)
{
    this->appendTextPart( upLoadEyeScore::recordTime, recordTime );
}

void CHttpUpLoadEyeSightScore::setProcessPics(QStringList paths)
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

void CHttpUpLoadEyeSightScore::setSignPicPath(QString signPath)
{
    if( signPath.isEmpty() ){
        return;
    }
    this->appendSignPart( signPath );
}

void CHttpUpLoadEyeSightScore::setBothEyeSight(QString leftEye, QString rightEeye)
{
    QString sendVal = upLoadEyeScore::leftEyeSightpreFix + leftEye +
            upLoadEyeScore::link + upLoadEyeScore::rightEyeSightPreFix +
            rightEeye;
    this->appendTextPart( upLoadEyeScore::recordListStr, sendVal );
}

void CHttpUpLoadEyeSightScore::setParaMap(QMap<QString, QVariant> infos)
{
    setUserId( infos.value( HTTP_KEYS::userId ).toString() );
    setDeviceCode( infos.value( HTTP_KEYS::devCode ).toString() );
    setDeviceType( short(infos.value( HTTP_KEYS::devType ).toInt()));
    setRecordTime( infos.value( HTTP_KEYS::recordTime ).toString() );
//    setProcessPics( infos.value( HTTP_KEYS::files ).toStringList() );
//    setSignPicPath( infos.value( HTTP_KEYS::signPic ).toString() );

    QString leftEyeSight  = infos.value( HTTP_KEYS::leftEyeSight ).toString();
    QString rightEyeSight = infos.value( HTTP_KEYS::rightEyeSight ).toString();

    setBothEyeSight( leftEyeSight, rightEyeSight );
}

void CHttpUpLoadEyeSightScore::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        QString errorString = pReply->errorString();
        setErrType( REQUEST_ERROR, errorString );
        emit sigRequestRetAvailable( false );
        qDebug() << "error string = " << errorString;

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
    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply){
        pReply->deleteLater();
        pReply = nullptr;
        m_pReply = nullptr;
    }

    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadEyeSightScore::onTimeoutHandler()
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
