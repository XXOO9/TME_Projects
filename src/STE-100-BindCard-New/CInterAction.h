#ifndef CINTERACTION_H
#define CINTERACTION_H

#include <QObject>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "../Common/HttpClient/CHttpSyncUserInfo.h"
#include "../DataModel/CUserInfos.h"
#include "../DataModel/CUserFilterModel.h"

class CInterAction : public QObject
{
    Q_OBJECT
public:
    explicit CInterAction(QObject *parent = nullptr);
    virtual ~CInterAction() override;

    void setPViewEngine(QQmlApplicationEngine *pViewEngine);
    void setCurrentCardNumber( const QString &cardNumber );
    void appendNewUsers( const QVector<CUserInfoElement> &newUsers );

public:
    //只要绑定的不是当前学生的学号，就是为未绑定
    Q_INVOKABLE bool isCardBinded( const QString cardNumber );
    Q_INVOKABLE QString getClassNameByUserId( const QString userId );
    Q_INVOKABLE void bindUserWithCardNumber( QString userId, QString cardNumber );

    void setClassInfoMap(const QVariantMap &classInfoMap);
    void appendNewBindedUsers( );

signals:
    Q_INVOKABLE void sigNewCardDeceted( QString cardNumber );
    Q_INVOKABLE void sigCloseLoading();

    Q_INVOKABLE void sigAppendBindedUserInfo( QJsonObject infos );

    void sigBindUserWithCardNumber( QString &userId, QString &cardNumber );

private:
    void registeInteraction();

private:
//    CHttpSyncUserInfo        m_syncUserInfo;
    QQmlApplicationEngine   *m_pViewEngine = nullptr;
    QVariantMap              m_classInfoMap;
    CUserFilterModel         m_filterModel;
    CUserInfos               m_totalUsers;
    QString                  m_curUserId = "-1";
    QString                  m_curCardNumber = "-1";
};

#endif // CINTERACTION_H
