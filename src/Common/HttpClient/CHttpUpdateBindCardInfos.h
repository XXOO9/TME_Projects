#ifndef CHTTPUPDATEBINDCARDINFOS_H
#define CHTTPUPDATEBINDCARDINFOS_H

#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

#include <QObject>

class COMMONSHARED_EXPORT CHttpUpdateBindCardInfos : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpUpdateBindCardInfos();
    virtual ~CHttpUpdateBindCardInfos() override;

    virtual void httpRequest() override;

    void setBindInfos( const QString &userId, const QString &cardNumber );

private:
    virtual void initConfig() override;

    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;



public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    QByteArray  m_postData = "";
};

#endif // CHTTPUPDATEBINDCARDINFOS_H
