#include "CHttpSyncUserInfo.h"
#include "../Config/CDatabaseManage.h"
#include "QThread"
CHttpSyncUserInfo::CHttpSyncUserInfo(QString timeStamp, FACE_ENGINE faceEngine, QObject *parent) : CHttpAbstractNetWork ( parent )
{
    m_strNetWorkName = "CHttpSyncUserInfo";

    if( timeStamp.isEmpty() ){
        timeStamp = QString::number( CHttpAbstractNetWork::currentTimeStamp() );
    }

    m_curMaxTimeStamp = timeStamp;

    appendMultiParaMap( SYNCUSERINFO_KEY_UPDATETIME, timeStamp );

    if( Arc_Soft == faceEngine || TMEc_Soft == faceEngine ){
        QString engineType = QString::number( static_cast<int>(faceEngine) );
        appendMultiParaMap( SYNCUSERINFO_KEY_FACEENGINE, engineType );
    }

    initConfig();
    initConnection();
}

CHttpSyncUserInfo::~CHttpSyncUserInfo()
{

}

void CHttpSyncUserInfo::initConfig()
{
    setInterfaceType( requestType::SYNCUSERINFO );
    m_baseInterfaceUrl = QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getUserInfoSyncFuncUrl());
}

void CHttpSyncUserInfo::httpRequest()
{
    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    if( isParameterEmpty() ){
        return;
    }
    QString timeStamp  = getSinglePara( SYNCUSERINFO_KEY_UPDATETIME );
    QString faceEngine = getSinglePara( SYNCUSERINFO_KEY_FACEENGINE );

    QString schoolID   = m_schoolID.isEmpty() ? "" : m_schoolID;

    QString url = m_baseInterfaceUrl
                + "?"
                + SYNCUSERINFO_KEY_UPDATETIME
                + "="
                + m_curMaxTimeStamp
                + "&"
                + SYNCUSERINFO_KEY_FACEENGINE
                + "="
                + faceEngine
                + "&schoolId"
                + schoolID;

    setUrl( url );

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    QNetworkRequest request = this->request();
    qDebug() << "sync userinfo url = " << request.url().toString();
    m_pReply = m_pNetWorkMgr->get( request );
    startTimeoutTimer( TIMEOUT_LONG );

}

bool CHttpSyncUserInfo::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }

    QJsonObject rootObj = jsonDataToJsonObject( recvJsonData() );
    QJsonObject dataObj = takeValueOfKeyFromJsonObject( rootObj, REQUEST_VALUE ).toJsonObject();

    //获取用户更新时间线
    m_userDelSpliTime = takeValueOfKeyFromJsonObject( dataObj, SYNCUSERINFO_del_SPLITIME).toString();

    //获取删除的用户
    QJsonArray delUserArray = takeValueOfKeyFromJsonObject( dataObj, SYNCUSERINFO_del_USER_ARRAY ).toJsonArray();
    for( int count = 0; count < delUserArray.count(); count++ ){
        m_vecDelUsers << QString::number( delUserArray.at( count ).toInt() );
    }

    QJsonArray userArray = takeValueOfKeyFromJsonObject( dataObj, SYNCUSERINFO_ARRAY_NAME ).toJsonArray();
    QJsonArray::iterator iter = userArray.begin();
    QJsonObject jsObj;
    QMap< QString, QString > retMap;

    for( ; iter != userArray.end(); iter++ ){
        jsObj = iter->toObject();
        retMap.insert( USERIDENTIFY_KEY_ID, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_ID ).toString() );
        retMap.insert( USERIDENTIFY_KEY_YEAR, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_YEAR ).toString() );
        retMap.insert( USERIDENTIFY_KEY_CLASSID, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_CLASSID ).toString() );
        retMap.insert( USERIDENTIFY_KEY_ROLE, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_ROLE ).toString() );
        retMap.insert( USERIDENTIFY_KEY_CODE, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_CODE ).toString() );
        retMap.insert( USERIDENTIFY_KEY_CARDNO, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_CARDNO ).toString() );
        retMap.insert( USERIDENTIFY_KEY_NAME, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_NAME ).toString() );
        retMap.insert( USERIDENTIFY_KEY_GENDER, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_GENDER ).toString() );
        retMap.insert( USERIDENTIFY_KEY_BIRTHDAY, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_BIRTHDAY ).toString() );
        retMap.insert( USERIDENTIFY_KEY_NATION, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_NATION ).toString() );
        retMap.insert( USERIDENTIFY_KEY_FACEFEATURE, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_FACEFEATURE ).toString() );
        retMap.insert( USERIDENTIFY_KEY_DESCRIPTION, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_DESCRIPTION ).toString() );
        retMap.insert( USERIDENTIFY_KEY_AVATARURL, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_AVATARURL ).toString() );
        retMap.insert( USERIDENTIFY_KEY_UPDATETIME, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_UPDATETIME ).toString() );

        if(jsObj.find(USERIDENTIFY_KEY_STAGE) != jsObj.end())
        {
            retMap.insert( USERIDENTIFY_KEY_STAGE, takeValueOfKeyFromJsonObject( jsObj, USERIDENTIFY_KEY_STAGE ).toString() );
        }

        m_ret.appendRet( retMap );
    }

    return true;
}

bool CHttpSyncUserInfo::isParameterEmpty()
{
    if( multiParaSize() <= 0 ){
        return true;
    }
    return false;
}

int CHttpSyncUserInfo::resultCount()
{
    return m_ret.size();
}

QMap<QString, QString> CHttpSyncUserInfo::getGroupRet(int index)
{
    if( index < 0 || index > m_ret.size() ){
        return QMap<QString, QString>();
    }
    return m_ret.getRetMapByIndex( index );
}

QString CHttpSyncUserInfo::getSingleInfo(int index, QString key)
{
    if( index < 0 || index > m_ret.size() ){
        return QString();
    }
    return m_ret.getRet( index, key );
}

QString CHttpSyncUserInfo::getDelSpliTime()
{
    return m_userDelSpliTime;
}

QVector<QString> CHttpSyncUserInfo::getDelUsers()
{
    return m_vecDelUsers;
}

void CHttpSyncUserInfo::onNewReplyAvailable(QNetworkReply *pReply)
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

    emit sigRequestRetAvailable( ok );  //onSyncUserInfoFinished
}

void CHttpSyncUserInfo::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}

void CHttpSyncUserInfo::setSchoolID(const QString &schoolID)
{
    m_schoolID = schoolID;
}

void CHttpSyncUserInfo::setCurMaxTimeStamp(const QString &curMaxTimeStamp)
{
    m_curMaxTimeStamp = curMaxTimeStamp;
}
