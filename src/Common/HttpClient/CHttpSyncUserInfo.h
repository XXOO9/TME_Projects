#ifndef CHTTPSYNCUSERINFO_H
#define CHTTPSYNCUSERINFO_H

#include <QObject>
#include <QVector>
#include "CHttpAbstractNetWork.h"
#include "CHttpConfigCommonDefine.h"
#include "CAbstractResult.h"
#include "../common_global.h"
#include "CSyncUserInfoRet.h"


class COMMONSHARED_EXPORT CHttpSyncUserInfo : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpSyncUserInfo( QString timeStamp = "0", FACE_ENGINE faceEngine = Arc_Soft, QObject *parent = nullptr );
    virtual ~CHttpSyncUserInfo() override;

    virtual void httpRequest() override;


    int resultCount();
    QMap<QString, QString> getGroupRet( int index );
    QString getSingleInfo( int index, QString key );

    QString getDelSpliTime();
    QVector<QString> getDelUsers();

    void setCurMaxTimeStamp(const QString &curMaxTimeStamp);

    void setSchoolID(const QString &schoolID);

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CSyncUserInfoRet   m_ret;
    QString            m_baseInterfaceUrl;

    //用户更新的时间线
    QString            m_userDelSpliTime;

    QString            m_curMaxTimeStamp = "0";

    //存储删除用户的数组
    QVector< QString >            m_vecDelUsers;

    //2023.03.10 新增，学校ID
    QString             m_schoolID = "";

};

#endif // CHTTPSYNCUSERINFO_H
