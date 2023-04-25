#include "CMainFrame.h"

CMainFrame::CMainFrame(  QObject *parent ) : QObject( parent )
{
    initConnections();
    startSyncUserInfos();

    startSyncClassInfos();

    m_nfcThread.start();
}


CMainFrame::~CMainFrame()
{
    if( m_nfcThread.isRunning() ){
        m_nfcThread.quit();
        m_nfcThread.wait();
    }
}

void CMainFrame::setViewEngine( QQmlApplicationEngine *pEngine)
{
    m_ui.setPViewEngine( pEngine );
}

void CMainFrame::onSyncUserInfosFinished(bool ok)
{
    if( !ok ){
        return;
    }

    int retCnt = m_pNetworkMgr->getPSyncUserInfo()->resultCount();
    CHttpSyncUserInfo   *pSyncUserInfoRequest = m_pNetworkMgr->getPSyncUserInfo();


    CUserInfoElement tmpUserInfo;
    QVector<CUserInfoElement> tmpUsers;

    for( int index = 0; index < retCnt; index++ ){
        tmpUserInfo.m_userName = pSyncUserInfoRequest->getSingleInfo( index, "name" );
        tmpUserInfo.m_userBindCardNumber = pSyncUserInfoRequest->getSingleInfo( index, "cardNo" );
        tmpUserInfo.m_userClassID = pSyncUserInfoRequest->getSingleInfo( index, "clazzId" );
        tmpUserInfo.m_userUniqueNumber = pSyncUserInfoRequest->getSingleInfo( index, "id" );

        tmpUsers << tmpUserInfo;
    }

    m_ui.appendNewUsers( tmpUsers );


}

void CMainFrame::onSyncClassInfosFinished(bool ok)
{
    if( !ok ){
        return;
    }

    CHttpSyncClassInfo  *pSyncClassInfoRet = m_pNetworkMgr->getPSyncClassInfo();

    int retCnt = pSyncClassInfoRet->resultCount();

    QString tmpClassId = "-1";
    QString tmpClassName = "null";
    QVariantMap tmpClassInfoMap;

    for( int i = 0; i < retCnt; i++ ){
        tmpClassId   = pSyncClassInfoRet->getSingleRet( i, "id" );
        tmpClassName = pSyncClassInfoRet->getSingleRet( i, "name" );

        tmpClassInfoMap[ tmpClassId ] = tmpClassName;

        m_ui.setClassInfoMap( tmpClassInfoMap );
    }

    if( tmpClassInfoMap.size() > 0 ){
        startSyncUserInfos();
    }
}

void CMainFrame::onDecetedNewCard(QString &cardNumber)
{
    qDebug() << "new card number = " << cardNumber;
    m_ui.setCurrentCardNumber( cardNumber );
}

void CMainFrame::onBindUser(QString &userId, QString &cardNumber)
{
    if( cardNumber == "-1" ){
        return;
    }

    m_upDateBindInfos.setBindInfos( userId, cardNumber );

    m_upDateBindInfos.httpRequest();
}

void CMainFrame::onBindUserFinished(bool ok)
{
    if( !ok ){
        return;
    }

    m_ui.appendNewBindedUsers();
}

void CMainFrame::initConnections()
{
    m_pNetworkMgr = CHttpRequestMgr::getInstance();
    connect( m_pNetworkMgr, &CHttpRequestMgr::sigSyncUserInfoFinished,
             this, &CMainFrame::onSyncUserInfosFinished, Qt::QueuedConnection );

    connect( m_pNetworkMgr, &CHttpRequestMgr::sigSyncClassInfoFinished,
             this, &CMainFrame::onSyncClassInfosFinished, Qt::QueuedConnection );

    connect( &m_nfcThread, &CNFCThread::sigNewCardDeceted, this, &CMainFrame::onDecetedNewCard );
    connect( &m_nfcThread, &CNFCThread::sigCardExisted, this, &CMainFrame::onDecetedNewCard );
    connect( &m_ui, &CInterAction::sigBindUserWithCardNumber, this, &CMainFrame::onBindUser );
    connect( &m_upDateBindInfos, &CHttpUpdateBindCardInfos::sigRequestRetAvailable, this, &CMainFrame::onBindUserFinished );


}

void CMainFrame::startSyncUserInfos()
{
    qDebug() << "11111111";

    m_pNetworkMgr->setIsOnline( true );
    m_pNetworkMgr->syncUserInfo();      //onSyncUserInfo
}

void CMainFrame::startSyncClassInfos()
{
    m_pNetworkMgr->setIsOnline( true );
    m_pNetworkMgr->syncClassInfo();      //onSyncUserInfo
}
