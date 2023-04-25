#include "CVRBDataBatchCommit.h"

CVRBDataBatchCommit::CVRBDataBatchCommit(CHttpAbstractNetWork *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CVRBDataBatchCommit";

    initConfig();
    initConnection();
}

CVRBDataBatchCommit::~CVRBDataBatchCommit()
{
}

void CVRBDataBatchCommit::httpRequest()
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

void CVRBDataBatchCommit::setCommitParameters(const QList<QVariantMap> &paraMapList)
{
    m_commitParaMapList = paraMapList;
}

QList<QVariantMap> CVRBDataBatchCommit::errorElementList() const
{
    return m_errorElementList;
}

void CVRBDataBatchCommit::onNewReplyAvailable(QNetworkReply *pReply)
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

void CVRBDataBatchCommit::onTimeoutHandler()
{
    if( m_pReply != nullptr && m_pReply->isRunning() ){
        m_pReply->abort();
    }

    setErrType( errorType::TIMEOUT_ERROR );
}

void CVRBDataBatchCommit::initConfig()
{
    //临时写在这里，实际使用根据情况动态获取
    const QString serverInterfacePath = "http://172.16.16.97:28088/pas/terminal-data/batch/vrb";
    setUrl( serverInterfacePath );
    qDebug() << "url = " << request().url().toString();
    setRequestRawHeader( rawHeaderKeys::HEADERNAME_CONTENTTYPE, rawHeaderKeys::HEADERVALUE_JSON );
}

bool CVRBDataBatchCommit::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    return true;
}

bool CVRBDataBatchCommit::isParameterEmpty()
{
    return m_commitParaMapList.isEmpty();
}

bool CVRBDataBatchCommit::isRequestSuccess()
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

//    m_errorElementList.clear();
//    QJsonArray errorInfoArray = tmpJson.take( REQUEST_VALUE ).toArray();

//    for( auto &ele : errorInfoArray ){
//        m_errorElementList << ele.toObject().toVariantMap();
//    }

    return false;
}

void CVRBDataBatchCommit::parseParam(DATA_SRC dataSrc)
{
    Q_UNUSED( dataSrc );
    m_sendJson.clear();

    QJsonObject sendJsonObject;
    QJsonArray  paraArray;

    for( auto &ele : m_commitParaMapList ){
        paraArray << QJsonObject::fromVariantMap( ele );
    }

    sendJsonObject.insert( VRBKeys::paraArrayName, paraArray );

    m_sendJson = jsonObjectToByteArray( sendJsonObject );

#if QT_NO_DEBUG
#else
    qDebug() << "send json = " << QString::fromUtf8( m_sendJson );
#endif
}

void CVRBDataBatchCommit::handleRequestError(QNetworkReply *pReply)
{
#if QT_NO_DEBUG
#else
    qDebug() << "request error string = " << pReply->errorString();
#endif

    pReply->deleteLater();
    pReply = nullptr;
    emit sigRequestRetAvailable( false );
}
