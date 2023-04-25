#include "CHttpGetVideoUrls.h"
#include "../Config/CDatabaseManage.h"
CHttpGetVideoUrls::CHttpGetVideoUrls(QString timeStamp, QObject *parent) : CHttpAbstractNetWork (parent)
{
    m_strNetWorkName = "CHttpGetVideoUrls";

    setTextPara( timeStamp );
    initConfig();
    initConnection();
}

CHttpGetVideoUrls::~CHttpGetVideoUrls()
{

}

void CHttpGetVideoUrls::initConfig()
{
    setInterfaceType( requestType::GETVIDEOURLS );
    setUrl(QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getTestInstructionVideSyncFuncUrl())\
           + INTERFACE_SYNVVIDEO_UPDATETIME\
           + textPara());
}

void CHttpGetVideoUrls::httpRequest()
{
    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    qDebug() << "sync video urls url = " << request().url().toString();

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    m_pReply = m_pNetWorkMgr->get( request() );
    startTimeoutTimer( TIMEOUT_SHORT );
}

bool CHttpGetVideoUrls::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, SYNCVIDEO_ARRAY_NAME ).toJsonArray();
    QJsonArray::iterator iter = dataArray.begin();
    for( ; iter != dataArray.end(); iter++ ){
        QMap< QString, QString > retMap;
        QJsonObject dataObj = iter->toObject();
        retMap.insert( SYNCVIDEO_NAME_TYPE, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_TYPE ).toString() );
        retMap.insert( SYNCVIDEO_NAME_URL, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_URL ).toString() );
        retMap.insert( SYNCVIDEO_NAME_UPDATETIME, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_UPDATETIME ).toString() );
        retMap.insert( SYNCVIDEO_NAME_PICURL, takeValueOfKeyFromJsonObject( dataObj, SYNCVIDEO_NAME_PICURL ).toString() );
        m_rets.appendRet( retMap );
    }

    return true;
}

bool CHttpGetVideoUrls::isParameterEmpty()
{
    return false;
}

int CHttpGetVideoUrls::resultCount()
{
    return m_rets.size();
}

QMap<QString, QString> CHttpGetVideoUrls::getGroupRet(int index)
{
    if( index < 0 || index > m_rets.size() ){
        return QMap<QString, QString>();
    }
    return m_rets.getRetMapByIndex( index );
}

QString CHttpGetVideoUrls::getSingleRet(int index, QString key)
{
    if( index < 0 || index > m_rets.size() ){
        return QString();
    }
    return m_rets.getRet( index, key );
}


void CHttpGetVideoUrls::onNewReplyAvailable(QNetworkReply *pReply)
{
    if( nullptr == pReply ){
        return;
    }
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        sigRequestRetAvailable( false );
        qDebug() << "error string = " << pReply->errorString();

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

void CHttpGetVideoUrls::onTimeoutHandler()
{
    if( nullptr == m_pReply ){
        return;
    }

    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
