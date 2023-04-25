#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include <QDebug>
#include "CInterAction.h"
#include "../Common/HttpClient/CHttpRequestMgr.h"
#include "./DataModel/CUserInfos.h"
#include "./DataModel/CUserFilterModel.h"
#include <QQmlApplicationEngine>
#include "./NFC/CNFCThread.h"
#include "../Common/HttpClient/CHttpUpdateBindCardInfos.h"

class CMainFrame : public QObject
{
    Q_OBJECT
public:
    CMainFrame( QObject *parent = nullptr );
    virtual ~CMainFrame() override;

    void setViewEngine( QQmlApplicationEngine *pEngine );

public slots:
    void onSyncUserInfosFinished( bool ok );
    void onSyncClassInfosFinished( bool ok );

    void onDecetedNewCard( QString &cardNumber );
    void onBindUser( QString &userId, QString &cardNumber );
    void onBindUserFinished( bool ok );
private:
    void initConnections();
    void startSyncUserInfos();
    void startSyncClassInfos();


private:
    CInterAction     m_ui;
    CHttpRequestMgr *m_pNetworkMgr = nullptr;
    CNFCThread       m_nfcThread;
    CHttpUpdateBindCardInfos    m_upDateBindInfos;
};

#endif // CMAINFRAME_H
