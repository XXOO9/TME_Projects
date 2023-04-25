#include "CHttpSyncScoreStd.h"
#include "../Config/CDatabaseManage.h"
CHttpSyncScoreStd::CHttpSyncScoreStd(QObject *parent) : CHttpAbstractNetWork( parent )
{
    m_strNetWorkName = "CHttpSyncScoreStd";

    initConfig();
    initConnection();
}

CHttpSyncScoreStd::~CHttpSyncScoreStd()
{

}

void CHttpSyncScoreStd::initConfig()
{
    setInterfaceType( requestType::SYNCSCORESTD );
    this->setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getScoreStandardSyncFuncUrl() ) );
}

void CHttpSyncScoreStd::httpRequest()
{
    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    m_pReply = m_pNetWorkMgr->get( request() );
    startTimeoutTimer( TIMEOUT_SHORT );
}

bool CHttpSyncScoreStd::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    QJsonObject jsObj = jsonDataToJsonObject( recvJsonData() );
    QJsonArray dataArray = takeValueOfKeyFromJsonObject( jsObj, REQUEST_VALUE ).toJsonArray();
    QJsonArray::iterator iter = dataArray.begin();
    for( ; iter != dataArray.end(); iter++ ){
        QJsonObject dataObj = iter->toObject();
        QMap< QString, QString > retMap;
        retMap.insert( syncScoreStdKeys::grade,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::grade ).toString());

        retMap.insert( syncScoreStdKeys::testItem,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::testItem ).toString());

        retMap.insert( syncScoreStdKeys::score,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::score ).toString());

        retMap.insert( syncScoreStdKeys::gender,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::gender ).toString());

        retMap.insert( syncScoreStdKeys::resultStart,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::resultStart ).toString());

        retMap.insert( syncScoreStdKeys::resultEnd,
                       takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::resultEnd ).toString());

//2023.01.03 修改， 针对公安局版本这里需要多加三个字段( version, ageStart, ageEnd ),
//考虑到正常版本并没有这三个字段，但是不影响数据库存入得分规则，故此处代码不做版本区别
        if(jsObj.find(syncScoreStdKeys::version) != jsObj.end())
        {
            retMap.insert( syncScoreStdKeys::version,
                           takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::version ).toString());
        }

        if(jsObj.find(syncScoreStdKeys::ageStart) != jsObj.end())
        {
            retMap.insert( syncScoreStdKeys::ageStart,
                           takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::ageStart ).toString());
        }

        if(jsObj.find(syncScoreStdKeys::ageEnd) != jsObj.end())
        {
            retMap.insert( syncScoreStdKeys::ageEnd,
                           takeValueOfKeyFromJsonObject( dataObj, syncScoreStdKeys::ageEnd ).toString());
        }

        m_ret.appendRet( retMap );
    }


    return true;
}

bool CHttpSyncScoreStd::isParameterEmpty()
{
    return false;
}

int CHttpSyncScoreStd::resultCount()
{
    return m_ret.size();
}

QMap<QString, QString> CHttpSyncScoreStd::getGroupRet(int index)
{
    if( index < 0 || index >= m_ret.size() ){
        return QMap<QString, QString>();
    }
    return m_ret.getRetMapByIndex( index );
}

QString CHttpSyncScoreStd::getSingleRet(int index, QString key)
{
    if( index < 0 || index > m_ret.size() ){
        return QString();
    }
    return m_ret.getRet( index, key );
}

void CHttpSyncScoreStd::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
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

#if QT_NO_DEBUG
#else
    qDebug() << "recv string = " << recvString;
#endif

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply){
        pReply->deleteLater();
        pReply = nullptr;
        m_pReply = nullptr;
    }

    emit sigRequestRetAvailable( ok );
}

void CHttpSyncScoreStd::onTimeoutHandler()
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
