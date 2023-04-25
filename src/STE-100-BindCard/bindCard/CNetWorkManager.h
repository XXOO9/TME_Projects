#ifndef CNETWORKMANAGER_H
#define CNETWORKMANAGER_H

#include <QObject>
#include "CHttpSyncClassInfo.h"
#include "CHttpSyncUserInfo.h"
#include "CSyncClassInfoRet.h"
#include "CSyncUserInfoRet.h"


class CNetWorkManager : public QObject
{
    Q_OBJECT
public:
    explicit CNetWorkManager(QObject *parent = nullptr);

    void syncStudentInfo( QString timeStamp );

    void syncClassInfo( QString timeStamp );

    void setVecClassInfo(QVector<QMap<QString, QString> > *vecClassInfo);

    CSyncUserInfoRet stuInfo() const;

    CSyncClassInfoRet classInfo() const;

signals:
    void sigNewStuInfoAvailable();
    void sigNewClassInfoAvailable();

public slots:
    void onNewStuInfoAvailable( bool ok );
    void onNewClassInfoAvailable( bool ok );

private:
    CHttpSyncClassInfo   *m_pSyncClassInfo;
    CHttpSyncUserInfo    *m_pSyncUserInfo;

    CSyncUserInfoRet      m_stuInfo;
    CSyncClassInfoRet     m_classInfo;

};

#endif // CNETWORKMANAGER_H
