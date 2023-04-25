#ifndef CGETSERVERTOKEN_H
#define CGETSERVERTOKEN_H

#include "CHttpAbstractNetWork.h"
#include "../common_global.h"
#include <QObject>
#include <QTimer>

class CGetServerToken;

namespace getServerTokenKeys {
    const QString userName      = "username";
    const QString password      = "password";
    const QString client_id     = "client_id";
    const QString client_secret = "client_secret";
    const QString grant_type    = "grant_type";
    const QString refresh_token = "refresh_token";
    const QString token         = "token";
    const QString tokenHead     = "tokenHead";
}


class COMMONSHARED_EXPORT CGetServerToken : public CHttpAbstractNetWork
{
public:
    CGetServerToken( CHttpAbstractNetWork *parent = nullptr );
    virtual ~CGetServerToken() override;



public:
    virtual void httpRequest() override;

    void setLoginParaMap(const QVariantMap &loginParaMap);

    QString token() const;
    void setToken(const QString &token);

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

    void onGetServerTokenTimerTimeout();

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;
    virtual bool isRequestSuccess() override;
    virtual void parseParam( DATA_SRC dataSrc = UserExam ) override;

    void handleRequestError( QNetworkReply *pReply );

    void initGetServerTokenTimer();

private:
    QString     m_token = "";
    QString     m_tokenHead = "";
    QVariantMap m_loginParaMap;
    QByteArray  m_sendData = "";
    QTimer      m_getServerTokenTimer;
};

#endif // CGETSERVERTOKEN_H
