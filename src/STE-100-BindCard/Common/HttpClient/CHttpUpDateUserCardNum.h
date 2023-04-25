#ifndef CHTTPUPDATEUSERCARDNUM_H
#define CHTTPUPDATEUSERCARDNUM_H

#include "CHttpAbstractNetWork.h"
#include "../bindCard/ProjectMetaEnums.h"
#include "../common_global.h"
#include <QObject>

class COMMONSHARED_EXPORT CHttpUpDateUserCardNum : public CHttpAbstractNetWork
{
public:
    CHttpUpDateUserCardNum();
    virtual ~CHttpUpDateUserCardNum() override;

    virtual void initConfig();

    virtual void httpRequest();
    virtual bool parseJson();
    virtual bool isParameterEmpty();

public slots:
    virtual void initConnection();
    virtual void onNewReplyAvailable( QNetworkReply *pReply );
    virtual void onTimeoutHandler();

public:

    QString userId() const;
    void setUserId(const QString &userId);

    QString cardNo() const;
    void setCardNo(const QString &cardNo);

    ProjectMetaEnums::CardType cardType() const;
    void setCardType(const ProjectMetaEnums::CardType &cardType);

    void cleanPara();

private:
    QString                    m_userId;
    QString                    m_cardNo;
    ProjectMetaEnums::CardType m_cardType;
};

#endif // CHTTPUPDATEUSERCARDNUM_H
