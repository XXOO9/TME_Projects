#include "CInteractionClass.h"

CInteractionClass::CInteractionClass(QObject *parent) : CAbstractInteraction(parent)
  ,m_loginState( false )
{
    initConnection();
}

QString CInteractionClass::className() const
{
    return m_className;
}

void CInteractionClass::setClassName(const QString &className)
{
    m_className = className;
}

QString CInteractionClass::stuId() const
{
    return m_stuId;
}

void CInteractionClass::setStuId(const QString &stuId)
{
    m_stuId = stuId;
    emit sigDisplayIdChanged( m_stuId );
}

QString CInteractionClass::name() const
{
    return m_name;
}

void CInteractionClass::setName(const QString &name)
{
    m_name = name;
}

QString CInteractionClass::gender() const
{
    return m_gender;
}

void CInteractionClass::setGender(const QString &gender)
{
    m_gender = gender;
}

bool CInteractionClass::loginState() const
{
    return m_loginState;
}

void CInteractionClass::setLoginState(bool loginState)
{
    m_loginState = loginState;
}

QString CInteractionClass::account() const
{
    return m_account;
}

void CInteractionClass::setAccount(const QString &account)
{
    m_account = account;
}

QString CInteractionClass::passwd() const
{
    return m_passwd;
}

void CInteractionClass::setPasswd(const QString &passwd)
{
    m_passwd = passwd;
}

void CInteractionClass::tryLogin()
{
    QString loginInfo = m_account + "+" + m_passwd;
    m_loginer.setUserAccount( m_account );
    m_loginer.setLoginPasswd( m_passwd );
    m_loginer.httpRequest();
}

void CInteractionClass::cancelLogin()
{
    m_loginer.abortRequest();
}

void CInteractionClass::detectedCard(ProjectMetaEnums::cardDetectState cardState)
{
    emit sigDecetedCardStatedChanged( (int)cardState );
}

void CInteractionClass::testStuView_cardBind()
{
    detectedCard( ProjectMetaEnums::Exist_Card_Bind );
}

void CInteractionClass::testStuView_cardNotBind()
{
    detectedCard( ProjectMetaEnums::Exist_Card_UnBind );
}

void CInteractionClass::appendNewRow(QString val)
{
    emit sigNewRowInfoAvailable(val);
}

void CInteractionClass::initConnection()
{
    connect( &m_loginer, &CHttpUserLogin::sigRequestRetAvailable, this, &CInteractionClass::onLoginRetAvailable );
    connect( this, &CAbstractInteraction::sigDecetedCardExist, this, &CInteractionClass::onNFCDecetedStateChanged );
}

void CInteractionClass::setCurrentPage()
{
    emit sigCurrentPageChanged( ProjectMetaEnums::Page_Class_Bind );
}

void CInteractionClass::onFindNFCCard(QString cardId)
{

}

void CInteractionClass::onNFCDecetedStateChanged(bool ok)
{
    QString cardNumberTmp = cardNum();
    QString stuCardnum = m_stuCardNum;

    qDebug() << "card no = " << cardNumberTmp;
    qDebug() << "cur stu card no = " << stuCardnum;
    if( ok && ( m_stuCardNum == cardNumberTmp )){
        detectedCard( ProjectMetaEnums::Exist_Card_Bind );
    }else if(ok && ( m_stuCardNum != cardNumberTmp )){
        detectedCard( ProjectMetaEnums::Exist_Card_UnBind );
    }else{
        detectedCard( ProjectMetaEnums::NO_Card );
    }
}

void CInteractionClass::onLoginRetAvailable(bool ok)
{
    emit sigLoginRetAvailable( ok );
}

QString CInteractionClass::stuCardNum() const
{
    return m_stuCardNum;
}

void CInteractionClass::setStuCardNum(const QString &stuCardNum)
{
    m_stuCardNum = stuCardNum;
}
