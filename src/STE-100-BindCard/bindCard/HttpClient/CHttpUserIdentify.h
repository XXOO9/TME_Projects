#ifndef CHTTPUSERIDENTIFY_H
#define CHTTPUSERIDENTIFY_H

#include <QObject>

#include "CHttpAbstractNetWork.h"
#include "CHttpConfigCommonDefine.h"
#include "CUserIdentifyRet.h"
#include "../common_global.h"

namespace userIdentifyParaKey {
          const QString faceFeature = "faceFeature";
          const QString cardNo = "cardNo";
}

class COMMONSHARED_EXPORT CHttpUserIdentify : public CHttpAbstractNetWork
{
public:
    CHttpUserIdentify( QString faceFeature , QString cardNo = "", QObject *parent = nullptr );
    virtual ~CHttpUserIdentify() override;

    virtual void initConfig() override;
    virtual void initConnection() override;
    virtual void httpRequest() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    int userInfoCount();
    QString getUserInfoByKey( QString key );

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CUserIdentifyRet    m_ret;
};

#endif // CHTTPUSERIDENTIFY_H
