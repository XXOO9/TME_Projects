#ifndef CINTERACTIONCLASS_H
#define CINTERACTIONCLASS_H

#include <QObject>
#include "CAbstractInteraction.h"
#include "ProjectMetaEnums.h"
#include "CStudentInfoMgr.h"
#include "CAbstractTableModel.h"
#include "CIdentificDevNFC.h"
#include "CHttpUpDateUserCardNum.h"
#include "CHttpUserLogin.h"
#include <QDebug>

class CInteractionClass : public CAbstractInteraction
{
    Q_OBJECT
    Q_PROPERTY(QString m_stuCardNum READ stuCardNum WRITE setStuCardNum NOTIFY sigCurStuCardNumChanged)
public:
    CInteractionClass(QObject *parent = nullptr);

    QString className() const;
    void setClassName(const QString &className);

    Q_INVOKABLE QString stuId() const;
    Q_INVOKABLE void setStuId(const QString &stuId);

    QString name() const;
    void setName(const QString &name);

    QString gender() const;
    void setGender(const QString &gender);

    Q_INVOKABLE bool loginState() const;
    Q_INVOKABLE void setLoginState(bool loginState);

    QString account() const;
    Q_INVOKABLE void setAccount(const QString &account);

    QString passwd() const;
    Q_INVOKABLE void setPasswd(const QString &passwd);

    QString stuCardNum() const;
    Q_INVOKABLE void setStuCardNum(const QString &stuCardNum);

    Q_INVOKABLE void tryLogin();
    Q_INVOKABLE void cancelLogin();

    Q_INVOKABLE void detectedCard( ProjectMetaEnums::cardDetectState cardState );

    //测试   卡绑定状态
    Q_INVOKABLE void testStuView_cardBind();

    //测试   卡未绑定状态
    Q_INVOKABLE void testStuView_cardNotBind();

    //表格操作
public:
    Q_INVOKABLE void appendNewRow( QString val );

//    刷卡操作
public:
    void initConnection();

    // 切换界面
    Q_INVOKABLE virtual void setCurrentPage();



public slots:
    void onFindNFCCard( QString cardId );
    void onNFCDecetedStateChanged(bool ok);

signals:
    void sigDisplayIdChanged( QString id );
    void sigTryLogin( QString val );
    void sigLoginRetAvailable(bool ret);
    void sigDecetedCardStatedChanged( int cardState );
    void sigNewRowInfoAvailable( QString valList );
    void sigCurStuCardNumChanged( QString cardNum );

public slots:
    void onLoginRetAvailable(bool ok);

private:
    QString                 m_className;
    QString                 m_stuId;
    QString                 m_name;
    QString                 m_gender;
    bool                    m_loginState;
    QString                 m_account;
    QString                 m_passwd;
    CHttpUpDateUserCardNum  m_updateUserBindCardNumber;
    CHttpUserLogin          m_loginer;
    QString                 m_stuCardNum;

};

#endif // CINTERACTIONCLASS_H
