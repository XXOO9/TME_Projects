#include "CNetWorkManager.h"

CNetWorkManager::CNetWorkManager(QObject *parent) : QObject(parent), m_pSyncClassInfo( nullptr )
  ,m_pSyncUserInfo( nullptr )
{
}

void CNetWorkManager::syncStudentInfo( QString timeStamp )
{
    if( nullptr == m_pSyncUserInfo ){
        m_pSyncUserInfo = new CHttpSyncUserInfo( timeStamp, this );
    }

    connect( m_pSyncUserInfo, &CHttpSyncUserInfo::sigRequestRetAvailable,
             this, &CNetWorkManager::onNewStuInfoAvailable );

    m_pSyncUserInfo->httpRequest();
}

void CNetWorkManager::syncClassInfo(QString timeStamp)
{
    if( nullptr == m_pSyncClassInfo ){
        m_pSyncClassInfo = new CHttpSyncClassInfo( timeStamp, this );
    }


    connect( m_pSyncClassInfo, &CHttpSyncClassInfo::sigRequestRetAvailable,
             this, &CNetWorkManager::onNewClassInfoAvailable );

    m_pSyncClassInfo->httpRequest();
}

void CNetWorkManager::onNewStuInfoAvailable(bool ok)
{
    if( !ok ){
        qDebug() << "error string = " << m_pSyncUserInfo->getErrorString();
        return;
    }
    qDebug() << "sync user info finished...";

    int retCount = m_pSyncUserInfo->resultCount();
    for( int index = 0; index < retCount; index++ ){
       m_stuInfo.appendRet( m_pSyncUserInfo->getGroupRet( index ) );
    }
    emit sigNewStuInfoAvailable();
}

void CNetWorkManager::onNewClassInfoAvailable(bool ok)
{
    if( !ok ){
        qDebug() << "error string = " << m_pSyncClassInfo->getErrorString();
        return;
    }

    int retCount = m_pSyncClassInfo->resultCount();
    for( int index = 0; index < retCount; index++ ){
        QMap<QString, QString> retMap = m_pSyncClassInfo->getGroupInfo( index );
        m_classInfo.appendRet( retMap );
    }
    emit sigNewClassInfoAvailable();
}

CSyncClassInfoRet CNetWorkManager::classInfo() const
{
    return m_classInfo;
}

CSyncUserInfoRet CNetWorkManager::stuInfo() const
{
    return m_stuInfo;
}
