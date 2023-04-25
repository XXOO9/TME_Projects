#ifndef CHTTPUSERIDENTIFY_H
#define CHTTPUSERIDENTIFY_H

#include <QObject>

#include "CHttpAbstractNetWork.h"
#include "CHttpConfigCommonDefine.h"
#include "CUserIdentifyRet.h"
#include "../common_global.h"

namespace userIdentifyParaKey {
          const QString faceFeature     =     "faceFeature";
          const QString cardNo          =     "cardNo";
          const QString faceEngineType  =     "faceEngineType";
          const QString name            =     "name";
          const QString year            =     "year";
          const QString clazzId         =     "clazzId";
          const QString role            =     "role";
          const QString code            =     "code";
          const QString gender          =     "gender";
          const QString birthday        =     "birthday";
          const QString nation          =     "nation";
          const QString description     =     "description";
          const QString avatarUrl       =     "avatarUrl";
          const QString updateTime      =     "updateTime";
          const QString userIdentId     =     "id";
}

class COMMONSHARED_EXPORT CHttpUserIdentify : public CHttpAbstractNetWork
{
public:
    CHttpUserIdentify( QString faceFeature = "", FACE_ENGINE faceEngine = Arc_Soft, QString cardNo = "", QString code = "", QObject *parent = nullptr );
    virtual ~CHttpUserIdentify() override;

    virtual void httpRequest() override;

    int userInfoCount();
    QString getUserInfoByKey( QString key );

    //对外获取是被出来的用户信息
    QMap<QString, QString > getMapUserInfo() const;

    void setIndentifyCondition(QString faceFeature , QString cardNo = "", FACE_ENGINE faceEngine = TMEc_Soft, QString code = QLatin1String("") );

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;


public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CUserIdentifyRet        m_ret;
    QMap<QString, QString>  m_mapUserInfo;
};

#endif // CHTTPUSERIDENTIFY_H
