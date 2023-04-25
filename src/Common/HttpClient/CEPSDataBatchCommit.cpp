#include "CEPSDataBatchCommit.h"

CEPSDataBatchCommit::CEPSDataBatchCommit(CHttpAbstractNetWork *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CEPSDataBatchCommit";

    initConfig();
    initConnection();
}

CEPSDataBatchCommit::~CEPSDataBatchCommit()
{

}

void CEPSDataBatchCommit::httpRequest()
{
    if( isParameterEmpty() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    parseParam();
    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }

    m_pReply = m_pNetWorkMgr->post( request(), m_pMultiPart );
    startTimeoutTimer( 3000 );
#if QT_NO_DEBUG
#else
    qDebug() << "start commit MIT, url = " << request().url().toString();
#endif
}

void CEPSDataBatchCommit::setCommitParameters(const QList<CEPSElement> &paraMapList)
{
    m_commitParaMapList = paraMapList;
}

QList<QVariantMap> CEPSDataBatchCommit::errorElementList() const
{
    return m_errorElementList;
}

void CEPSDataBatchCommit::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();

    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }
    m_pMultiPart->setParent( pReply );

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

void CEPSDataBatchCommit::onTimeoutHandler()
{
    if( m_pReply != nullptr && m_pReply->isRunning() ){
        m_pReply->abort();
    }

    setErrType( errorType::TIMEOUT_ERROR );
}

void CEPSDataBatchCommit::initConfig()
{
    //临时写在这里，实际使用根据情况动态获取
    const QString serverInterfacePath = "http://172.16.16.97:28088/pas/terminal-data/batch/eps";
    setUrl( serverInterfacePath );
    qDebug() << "url = " << request().url().toString();
}

bool CEPSDataBatchCommit::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    return true;
}

bool CEPSDataBatchCommit::isParameterEmpty()
{
    return m_commitParaMapList.isEmpty();
}

bool CEPSDataBatchCommit::isRequestSuccess()
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

void CEPSDataBatchCommit::parseParam(DATA_SRC dataSrc)
{
    Q_UNUSED( dataSrc );
    m_sendJson.clear();
    CEPSElement tmpElement;

    QVariantMap tmpTrackMap;
    QVariantMap tmpStatisMap;
    QVariantMap tmpMeaingMap;

    const int size = m_commitParaMapList.size();

    for( int index = 0; index < size; index++ ){
        tmpElement = m_commitParaMapList.at( index );

        appendTextPart( parameterTemplateKeys::userId.arg( index ), tmpElement.m_userId );
        appendTextPart( parameterTemplateKeys::recordTime.arg( index ), tmpElement.m_recordTime );
        appendTextPart( parameterTemplateKeys::sandboxName.arg( index ), tmpElement.m_sandboxName );
        appendTextPart( parameterTemplateKeys::sandboxScenes.arg( index ), tmpElement.m_sandboxScenes );
        appendTextPart( parameterTemplateKeys::sandboxTimeCost.arg( index ), tmpElement.m_sandboxTimeCost );
        appendFilePart( tmpElement.m_filePath, parameterTemplateKeys::picFile.arg( index ) );

        for( int trackIndex = 0; trackIndex < tmpElement.m_itemUseTrackList.size(); trackIndex++ ){
            tmpTrackMap = tmpElement.m_itemUseTrackList.at( trackIndex );

            appendTextPart( parameterTemplateKeys::itemUseTrackListUseOrder.arg( index ).arg( trackIndex ),
                            tmpTrackMap.value( EPSKeys::useOrder ).toString() );

            appendTextPart( parameterTemplateKeys::itemUseTrackListItemName.arg( index ).arg( trackIndex ),
                            tmpTrackMap.value( EPSKeys::itemName ).toString() );

            appendTextPart( parameterTemplateKeys::itemUseTrackListCategory.arg( index ).arg( trackIndex ),
                            tmpTrackMap.value( EPSKeys::category ).toString() );

            appendTextPart( parameterTemplateKeys::itemUseTrackListIsDeleted.arg( index ).arg( trackIndex ),
                            tmpTrackMap.value( EPSKeys::isDeleted ).toString() );
        }

        for( int statisIndex = 0; statisIndex < tmpElement.m_itemUseStatisList.size(); statisIndex++ ){
            tmpStatisMap = tmpElement.m_itemUseStatisList.at( statisIndex );

            appendTextPart( parameterTemplateKeys::itemUseStaticListCategory.arg( index ).arg( statisIndex ),
                            tmpStatisMap.value( EPSKeys::category ).toString() );

            appendTextPart( parameterTemplateKeys::itemUseStaticListColdNum.arg( index ).arg( statisIndex ),
                            tmpStatisMap.value( EPSKeys::coldNum ).toString() );

            appendTextPart( parameterTemplateKeys::itemUseStaticListWarmNum.arg( index ).arg( statisIndex ),
                            tmpStatisMap.value( EPSKeys::warmNum ).toString() );
        }

        for( int meaingIndex = 0; meaingIndex < tmpElement.m_itemMeaningList.size(); meaingIndex++ ){
            tmpMeaingMap = tmpElement.m_itemMeaningList.at( meaingIndex );

            appendTextPart( parameterTemplateKeys::itemMeaningListIndex.arg( index ).arg( meaingIndex ),
                            tmpMeaingMap.value( EPSKeys::index ).toString() );

            appendTextPart( parameterTemplateKeys::itemMeaningListItemName.arg( index ).arg( meaingIndex ),
                            tmpMeaingMap.value( EPSKeys::meaning ).toString() );

            appendTextPart( parameterTemplateKeys::itemMeaningListMeaning.arg( index ).arg( meaingIndex ),
                            tmpMeaingMap.value( EPSKeys::itemName ).toString() );
        }
    }
}

void CEPSDataBatchCommit::handleRequestError(QNetworkReply *pReply)
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
