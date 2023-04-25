#include "CHttpSyncThread.h"

CHttpSyncThread::CHttpSyncThread(QObject *parent) : QObject(parent)
{
    init();
}

void CHttpSyncThread::startSyncUserInfo(QString timeStamp, FACE_ENGINE engine)
{
    if(timeStamp.isEmpty() || (engine != Arc_Soft && engine != TMEc_Soft))
    {
        return;
    }
}

void CHttpSyncThread::startSyncScoreStd()
{

}

CHttpSyncThread *CHttpSyncThread::getInstance()
{
    if( nullptr == m_pSyncThread ){
        m_locker.lock();
        if( nullptr == m_pSyncThread ){
            m_pSyncThread = new CHttpSyncThread();
        }
        m_locker.unlock();
    }
    return m_pSyncThread;
}

void CHttpSyncThread::init()
{

}

void CHttpSyncThread::initThread()
{
    m_pWorkThread = new QThread();
    this->moveToThread( m_pWorkThread );
    m_pWorkThread->start();
}

void CHttpSyncThread::initConnection()
{
    connect( m_pSyncUserInfo, &CHttpSyncUserInfo::sigRequestRetAvailable, this, &CHttpSyncThread::onSyncUserInfoFinished );
    connect( m_pSyncClassInfo, &CHttpSyncClassInfo::sigRequestRetAvailable, this, &CHttpSyncThread::onSyncClassInfoFinished );
    connect( m_pSyncScoreStd, &CHttpSyncScoreStd::sigRequestRetAvailable, this, &CHttpSyncThread::onSyncScoreStd );
}

void CHttpSyncThread::onSyncUserInfoFinished(bool ok)
{
    if( !ok ){
        qDebug() << "sync user info failed...";
        m_errorString = m_pSyncUserInfo->getErrorString();
        qDebug() << "error string = " << m_errorString;
    }else{
        int urlsCount = m_pSyncUserInfo->resultCount();
        //所有的信息都存放在results里面
        m_vecUserInfo.clear();
        for( int index = 0; index < urlsCount; index++ ){
            QMap< QString, QString > retGroup = m_pSyncUserInfo->getGroupRet( index );
            m_vecUserInfo << retGroup;
        }
        qDebug() << "sync user info success...";
    }
    emit sigSyncUserInfoFinished( ok );
}

void CHttpSyncThread::onSyncClassInfoFinished(bool ok)
{
    if( !ok ){
        qDebug() << "sync class info failed...";
        m_errorString = m_pSyncUserInfo->getErrorString();
        qDebug() << "error string = " << m_errorString;
    }else{
        int urlsCount = m_pSyncClassInfo->resultCount();
        m_vecClassInfo.clear();
        //所有的信息都存放在 m_vecClassInfo 里面
        for( int index = 0; index < urlsCount; index++ ){
            QMap< QString, QString > retGroup = m_pSyncClassInfo->getGroupInfo( index );
            m_vecClassInfo << retGroup;
        }
        qDebug() << "sync class info success...";
    }
    emit sigSyncClassInfoFinished( ok );
}

void CHttpSyncThread::onSyncScoreStd(bool ok)
{
    if( !ok ){
        qDebug() << "sync score std failed...";
        m_errorString = m_pSyncScoreStd->getErrorString();
        qDebug() << "error string = " << m_errorString;
    }else{
        int urlsCount = m_pSyncScoreStd->resultCount();

        //所有的信息都存放在 m_vecScoreStd 里面
        m_vecScoreStd.clear();
        for( int index = 0; index < urlsCount; index++ ){
            QMap< QString, QString > retGroup = m_pSyncScoreStd->getGroupRet( index );
            m_vecScoreStd << retGroup;
        }
        qDebug() << "sync score std success...";
    }
    emit sigSyncScoreStdFinished( ok );
}

QVector<QMap<QString, QString> > CHttpSyncThread::getVecClassInfo() const
{
    return m_vecClassInfo;
}

QVector<QMap<QString, QString> > CHttpSyncThread::getVecUserInfo() const
{
    return m_vecUserInfo;
}

QVector<QMap<QString, QString> > CHttpSyncThread::getVecScoreStd() const
{
    return m_vecScoreStd;
}

CHttpSyncThread *CHttpSyncThread::m_pSyncThread = nullptr;
QMutex           CHttpSyncThread::m_locker;
