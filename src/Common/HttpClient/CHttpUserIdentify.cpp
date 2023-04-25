#include "CHttpUserIdentify.h"
#include "../Config/CDatabaseManage.h"
#include <QEventLoop>
#include <QThread>

CHttpUserIdentify::CHttpUserIdentify( QString faceFeature,
                                      FACE_ENGINE faceEngine,
                                      QString cardNo,
                                      QString code,
                                      QObject *parent
                                      ) : CHttpAbstractNetWork ( parent )
{
    m_strNetWorkName = "CHttpUserIdentify";

    if( !faceFeature.isEmpty() ){
        insertMultiPara( userIdentifyParaKey::faceFeature, faceFeature );
    }

    if( !cardNo.isEmpty() ){
        insertMultiPara( userIdentifyParaKey::cardNo, cardNo );
    }

    if( Arc_Soft == faceEngine || TMEc_Soft == faceEngine ){
        int engine = static_cast<int>(faceEngine);
        insertMultiPara( userIdentifyParaKey::faceEngineType, QString::number( engine ) );
    }

    if( !code.isEmpty() ){
        insertMultiPara( userIdentifyKeys::code, code );
    }

    initConfig();
    initConnection();
}

CHttpUserIdentify::~CHttpUserIdentify()
{

}

void CHttpUserIdentify::initConfig()
{
    setInterfaceType( requestType::USERIDENTIFY );
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
    setUrl(QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getUserIdentifyFuncUrl()) );
}

void CHttpUserIdentify::httpRequest()
{
    QByteArray sendJsonData = this->toUserIdentifySendJson();
    if( sendJsonData.isEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    qDebug() << "setIndentifyCondition = " << QString::fromUtf8(sendJsonData);

    QEventLoop loop;

#if QT_NO_DEBUG
#else
    qDebug() << "server url = " << request().url().toString();
#endif

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    LOGINFO( "start user identify http request..." );

    m_pReply = m_pNetWorkMgr->post( request(), sendJsonData );
    qDebug() << "request user identify ret thread ID = " <<QThread::currentThreadId();
    startTimeoutTimer( TIMEOUT_RECOG );
    connect( m_pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit );
    loop.exec();
}

bool CHttpUserIdentify::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QJsonObject dataObj = takeValueOfKeyFromJsonObject( jsObj, REQUEST_VALUE ).toJsonObject();
    QMap< QString, QString > retMap;
    m_mapUserInfo.clear();
    retMap.insert( userIdentifyKeys::id, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::id).toString() );
    retMap.insert( userIdentifyKeys::year, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::year).toString() );
    retMap.insert( userIdentifyKeys::clazzId, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::clazzId).toString() );
    retMap.insert( userIdentifyKeys::role, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::role).toString() );
    retMap.insert( userIdentifyKeys::code, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::code).toString() );
    retMap.insert( userIdentifyKeys::cardNo, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::cardNo).toString() );
    retMap.insert( userIdentifyKeys::name, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::name).toString() );
    retMap.insert( userIdentifyKeys::gender, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::gender).toString() );
    retMap.insert( userIdentifyKeys::nation, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::nation).toString() );
    retMap.insert( userIdentifyKeys::faceFeature, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::faceFeature).toString() );
    retMap.insert( userIdentifyKeys::description, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::description).toString() );
    retMap.insert( userIdentifyKeys::avatarUrl, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::avatarUrl).toString() );
    retMap.insert( userIdentifyKeys::updateTime, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::updateTime).toString() );
    retMap.insert( userIdentifyKeys::birthday, takeValueOfKeyFromJsonObject( dataObj, userIdentifyKeys::birthday).toString() );

    m_ret.appendRet( retMap );
    m_mapUserInfo = retMap;

    return true;
}

bool CHttpUserIdentify::isParameterEmpty()
{
    return true;
}

int CHttpUserIdentify::userInfoCount()
{
    return m_ret.size();
}

QString CHttpUserIdentify::getUserInfoByKey(QString key)
{
    if( userInfoCount() < 0 ){
        return QString();
    }

    return m_ret.getRet( 0, key );
}

QMap<QString, QString> CHttpUserIdentify::getMapUserInfo() const
{
    return m_mapUserInfo;
}

void CHttpUserIdentify::setIndentifyCondition(QString faceFeature, QString cardNo, FACE_ENGINE faceEngine, QString code)
{
    clearParamMap();

    if( !faceFeature.isEmpty() ){
        insertMultiPara( userIdentifyParaKey::faceFeature, faceFeature );
    }

    if( !cardNo.isEmpty() ){
        insertMultiPara( userIdentifyParaKey::cardNo, cardNo );
    }

    if( Arc_Soft == faceEngine || TMEc_Soft == faceEngine ){
        int tmpEngine = static_cast<int>(faceEngine);
        insertMultiPara( userIdentifyParaKey::faceEngineType, QString::number( tmpEngine ) );
    }

    if( !code.isEmpty() ){
        insertMultiPara( userIdentifyKeys::code, code );
    }
}


void CHttpUserIdentify::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    m_ret.clear();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        qDebug() << "error string = " << pReply->errorString();
        LOGINFO( "user identify failed, error string = " + pReply->errorString().toStdString() );
        emit sigRequestRetAvailable( false );

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
    LOGINFO( "user identify finished, ret = " + recvString.toStdString() );
    qDebug() << "recv string = " << recvString;
    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply ){
        pReply->deleteLater();
        pReply = nullptr;
        m_pReply = nullptr;
    }
    qDebug() << "sig user identify ret thread ID = " <<QThread::currentThreadId();
    emit sigRequestRetAvailable( ok );
}

void CHttpUserIdentify::onTimeoutHandler()
{
    if( nullptr == m_pReply ){
        return;
    }

    if( m_pReply->isFinished() ){
        return;
    }

    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
