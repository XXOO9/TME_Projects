#include "CHttpSyncClassInfo.h"
#include "../Config/CDatabaseManage.h"

CHttpSyncClassInfo::CHttpSyncClassInfo(QString timeSatmp, QObject *parent) : CHttpAbstractNetWork ( parent )
{
    m_strNetWorkName = "CHttpSyncClassInfo";

    setTextPara( timeSatmp );
    initConfig();
    initConnection();
}

CHttpSyncClassInfo::~CHttpSyncClassInfo()
{

}

void CHttpSyncClassInfo::initConfig()
{
    setInterfaceType( requestType::SYNCCLASSINFO );
    setUrl(  QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getClassInfoSyncFuncUrl())\
           + INTERFACE_SYNCCLASSINFO_UPDATETIME  \
           + textPara());
}

void CHttpSyncClassInfo::httpRequest()
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

bool CHttpSyncClassInfo::parseJson()
{
    if( !isJson() ){
        return false;
    }
    if( !isRequestSuccess() ){
        return false;
    }

    QJsonObject rootObj = jsonDataToJsonObject( recvJsonData() );
    QJsonObject dataObj = takeValueOfKeyFromJsonObject( rootObj, REQUEST_VALUE ).toJsonObject();

    //获取删除时间线
    m_delSpliTime = takeValueOfKeyFromJsonObject( dataObj, SYNCCLASSINFO_DELSPLITIME ).toString();

    //获取删除的班级ID
    QJsonArray delClass = takeValueOfKeyFromJsonObject( dataObj, SYNCCLASSINFO_DEL_CLASSID ).toJsonArray();
    for( int count = 0; count < delClass.count(); count++ ){
        m_vecDelClass << QString::number( delClass.at( count ).toInt() );
    }

    QJsonArray classInfoArray = takeValueOfKeyFromJsonObject( dataObj, SYNCCLASSINFO_ARRAYNAME ).toJsonArray();
    QJsonArray::iterator iter = classInfoArray.begin();

    for( ; iter != classInfoArray.end(); iter++ ){
        QJsonObject jsObj = iter->toObject();
        QMap< QString, QString > retMap;
        retMap.insert( SYNCCLASSINFO_KEY_ID, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_ID ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_NAME, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_NAME ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_YEAR, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_YEAR ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_SCHOOL, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_SCHOOL ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_TYPE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_TYPE ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_ENABLE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_ENABLE ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_GRADUATE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_GRADUATE ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_DESCRIPTION, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_DESCRIPTION ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_UPDATETIME, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_UPDATETIME ).toString() );
        retMap.insert( SYNCCLASSINFO_KEY_GRADE, takeValueOfKeyFromJsonObject( jsObj, SYNCCLASSINFO_KEY_GRADE ).toString() );
        m_ret.appendRet( retMap );
    }
    return true;
}

bool CHttpSyncClassInfo::isParameterEmpty()
{
    if( textPara().isEmpty() ){
        return true;
    }
    return false;
}

int CHttpSyncClassInfo::resultCount()
{
    return m_ret.size();
}

QMap<QString, QString> CHttpSyncClassInfo::getGroupInfo(int index)
{
    if( index < 0 || index > m_ret.size() ){
        return QMap<QString, QString>();
    }
    return m_ret.getRetMapByIndex( index );
}

QString CHttpSyncClassInfo::getSingleRet(int index, QString key)
{
    if( index < 0 || index > m_ret.size() ){
        return QString();
    }
    return m_ret.getRet( index, key );
}

void CHttpSyncClassInfo::onNewReplyAvailable(QNetworkReply *pReply)
{
    if( nullptr == pReply ){
        return;
    }

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

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply){
        pReply->deleteLater();
        pReply = nullptr;
        m_pReply = nullptr;
    }

    emit sigRequestRetAvailable( ok );
}

void CHttpSyncClassInfo::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    if( nullptr == m_pReply ){
        return;
    }
    m_pReply->abort();

}

QVector<QString> CHttpSyncClassInfo::getVecDelClass()
{
    return m_vecDelClass;
}

QString CHttpSyncClassInfo::getDelSpliTime()
{
    return m_delSpliTime;
}
