#ifndef CHTTPDISBINDUSERCARDNUMBER_H
#define CHTTPDISBINDUSERCARDNUMBER_H

#include "CHttpAbstractNetWork.h"
#include "../common_global.h"
#include <QObject>

class COMMONSHARED_EXPORT CHttpDisBindUserCardNumber : public CHttpAbstractNetWork
{
public:
    CHttpDisBindUserCardNumber();
    virtual ~CHttpDisBindUserCardNumber() override;

    virtual void initConfig();

    virtual void httpRequest();
    virtual bool parseJson();
    virtual bool isParameterEmpty();

    QString userId() const;
    void setUserId(const QString &userId);

public slots:
    virtual void initConnection();
    virtual void onNewReplyAvailable( QNetworkReply *pReply );
    virtual void onTimeoutHandler();

private:
    QString     m_userId;
};

#endif // CHTTPDISBINDUSERCARDNUMBER_H
