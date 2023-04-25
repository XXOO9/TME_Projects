#include "CQuerySandReport.h"

CQuerySandReport::CQuerySandReport(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CQuerySandReport";

    initConfig();
    initConnection();
}

CQuerySandReport::~CQuerySandReport()
{

}

void CQuerySandReport::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    QString targetGetUrl = request().url().toString() + "userId=" + m_userId + "&recordTime=" + m_queryRecordTime;
    setUrl( targetGetUrl );

    m_pNetWorkMgr->get( request() );
    startTimeoutTimer( 3000 );
    #if QT_NO_DEBUG
    #else
        qDebug() << "start commit MIT, url = " << request().url().toString();
    #endif
}

void CQuerySandReport::setQueryCondition(const QString &userId, const QString &recordTime)
{
    m_userId = userId;
    m_queryRecordTime = recordTime;
}

void CQuerySandReport::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();

    if( nullptr == pReply ){
        return;
    }

    if( QNetworkReply::NoError != pReply->error() ){
        handleRequestError( pReply );
        return;
    }

    QByteArray recvJsonByteArray = pReply->readAll();
    setRecvJsonData( recvJsonByteArray );
    bool ok = parseJson();

    pReply->deleteLater();
    pReply = nullptr;
    m_pReply = nullptr;
#if QT_NO_DEBUG
#else
    qDebug() << "recv json string = " << QString::fromUtf8( recvJsonByteArray );
#endif

    emit sigRequestRetAvailable( ok );
}

void CQuerySandReport::onTimeoutHandler()
{
    if( m_pReply != nullptr && m_pReply->isRunning() ){
        m_pReply->abort();
    }

    setErrType( errorType::TIMEOUT_ERROR );
}

void CQuerySandReport::initConfig()
{
    //临时写在这里，实际使用根据情况动态获取
    const QString serverInterfacePath = "http://172.16.16.107:28088/pas/terminal-eps/report?";
    setUrl( serverInterfacePath );
}

bool CQuerySandReport::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    return false;
}

bool CQuerySandReport::isParameterEmpty()
{
    return m_userId.isEmpty() && m_queryRecordTime.isEmpty();
}

bool CQuerySandReport::isRequestSuccess()
{
    QJsonDocument jsDoc = QJsonDocument::fromJson( recvJsonData() );
    QJsonObject tmpJson = jsDoc.object();

    if( !tmpJson.contains( REQUEST_RESULT_KEY ) ){
        return false;
    }

    //这个地方跟体测的成功标志不一样， 体测是 0
    const int successCode = 0000;

    //开始解析报告信息
    if( successCode != tmpJson.value( REQUEST_RESULT_KEY ).toVariant().toInt() ){
        return false;
    }

    m_reportInfoMap = tmpJson.take( QueryReportKeys::data ).toObject().toVariantMap();



    return true;
}

void CQuerySandReport::parseParam(DATA_SRC dataSrc)
{

}

void CQuerySandReport::handleRequestError(QNetworkReply *pReply)
{
#if QT_NO_DEBUG
#else
    qDebug() << "request error string = " << pReply->errorString();
#endif

    pReply->deleteLater();
    pReply = nullptr;
    emit sigRequestRetAvailable( false );
}

QVariantMap CQuerySandReport::reportInfoMap() const
{
    return m_reportInfoMap;
}
