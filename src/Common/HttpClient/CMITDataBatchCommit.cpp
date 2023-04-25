#include "CMITDataBatchCommit.h"

CMITDataBatchCommit::CMITDataBatchCommit(CHttpAbstractNetWork *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CMITDataBatchCommit";

    initConfig();
    initConnection();
}

CMITDataBatchCommit::~CMITDataBatchCommit()
{

}

void CMITDataBatchCommit::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    parseParam();

    m_pReply = m_pNetWorkMgr->post( request(), m_sendJson );
    startTimeoutTimer( 3000 );
#if QT_NO_DEBUG
#else
    qDebug() << "start commit MIT, url = " << request().url().toString();
#endif
}

void CMITDataBatchCommit::setCommitParameters(const QVariantList &paraMapList)
{
    m_commitParaMapList = paraMapList;
}

void CMITDataBatchCommit::onNewReplyAvailable(QNetworkReply *pReply)
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
    m_pMultiPart = nullptr;
#if QT_NO_DEBUG
#else
    qDebug() << "recv json string = " << QString::fromUtf8( recvJsonByteArray );
#endif

    emit sigRequestRetAvailable( ok );
}

void CMITDataBatchCommit::onTimeoutHandler()
{
    if( m_pReply != nullptr && m_pReply->isRunning() ){
        m_pReply->abort();
    }

    setErrType( errorType::TIMEOUT_ERROR );
}

void CMITDataBatchCommit::initConfig()
{
    //
//    setInterfaceType();

    //临时写在这里，实际使用根据情况动态获取
    const QString serverInterfacePath = "http://172.16.16.107:28088/pas/terminal-data/batch/mit";
    setUrl( serverInterfacePath );
    qDebug() << "url = " << request().url().toString();
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

bool CMITDataBatchCommit::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    return true;
}

bool CMITDataBatchCommit::isParameterEmpty()
{
    return m_commitParaMapList.isEmpty();
}

bool CMITDataBatchCommit::isRequestSuccess()
{
    QJsonDocument jsDoc = QJsonDocument::fromJson( recvJsonData() );
    QJsonObject tmpJson = jsDoc.object();

    if( !tmpJson.contains( REQUEST_RESULT_KEY ) ){
        return false;
    }

    //这个地方跟体测的成功标志不一样， 体测是 0
    const int successCode = 0000;

    //提交成功直接返回, 不需要继续往下解析
    if( successCode == tmpJson.value( REQUEST_RESULT_KEY ).toVariant().toInt() ){
        return true;
    }

    m_errorElementList.clear();
    QJsonArray errorInfoArray = tmpJson.take( REQUEST_VALUE ).toArray();

    for( auto &ele : errorInfoArray ){
        m_errorElementList << ele.toObject().toVariantMap();
    }

    return false;

}

void CMITDataBatchCommit::parseParam(DATA_SRC dataSrc)
{
    Q_UNUSED( dataSrc );
    m_sendJson.clear();

    QJsonObject sendJsonObject;
    QJsonArray  paraArray;
    QJsonObject tmpUserParaJson;

    for( auto &ele : m_commitParaMapList ){
        paraArray << QJsonObject::fromVariantMap( ele.toMap() );
    }

    sendJsonObject.insert( MITKeys::recordList, paraArray );
    m_sendJson = jsonObjectToByteArray( sendJsonObject );

}

void CMITDataBatchCommit::handleRequestError( QNetworkReply *pReply )
{
#if QT_NO_DEBUG
#else
    qDebug() << "request error string = " << pReply->errorString();
#endif
    pReply->deleteLater();
    pReply = nullptr;
    m_pMultiPart = nullptr;
    emit sigRequestRetAvailable( false );
}

QList<QVariantMap> CMITDataBatchCommit::errorElementList() const
{
    return m_errorElementList;
}
