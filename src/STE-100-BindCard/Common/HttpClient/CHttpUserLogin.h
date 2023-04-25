#ifndef CHTTPUSERLOGIN_H
#define CHTTPUSERLOGIN_H

#include "CHttpAbstractNetWork.h"
#include "../common_global.h"
#include <QObject>

class COMMONSHARED_EXPORT CHttpUserLogin : public CHttpAbstractNetWork
{
public:
    CHttpUserLogin();
    virtual void initConfig();

    virtual void httpRequest();
    virtual bool parseJson();
    virtual bool isParameterEmpty();

    QString userAccount() const;
    void setUserAccount(const QString &userAccount);

    QString loginPasswd() const;
    void setLoginPasswd(const QString &loginPasswd);

    Q_INVOKABLE void abortRequest();

public slots:
    virtual void initConnection();
    virtual void onNewReplyAvailable( QNetworkReply *pReply );
    virtual void onTimeoutHandler();

private:
    QString     m_userAccount;
    QString     m_loginPasswd;
};

#endif // CHTTPUSERLOGIN_H
