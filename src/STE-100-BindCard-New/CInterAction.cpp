#include "CInterAction.h"

CInterAction::CInterAction(QObject *parent) : QObject(parent)
{

}

CInterAction::~CInterAction()
{
}

void CInterAction::setPViewEngine(QQmlApplicationEngine *pViewEngine)
{
    m_pViewEngine = pViewEngine;
    registeInteraction();
}

void CInterAction::setCurrentCardNumber(const QString &cardNumber)
{
    qDebug() << "new card deceted: " << cardNumber;
    emit sigNewCardDeceted( cardNumber );
}

void CInterAction::appendNewUsers(const QVector<CUserInfoElement> &newUsers)
{
    for( auto &ele : newUsers ){
        m_totalUsers.appendNewUser( ele );
    }

    m_filterModel.setSourceModel( &m_totalUsers );

    qDebug() << "load total users finished, size = " << newUsers.size();
    emit sigCloseLoading();
}

bool CInterAction::isCardBinded(const QString cardNumber)
{
    QModelIndexList tmpList = m_totalUsers.match( m_totalUsers.index( 0, 0 ), CUserInfoElement::UserBindCardNumber, cardNumber );

    return !tmpList.isEmpty();
}

QString CInterAction::getClassNameByUserId(const QString userId)
{
    QString tmpClassId = m_totalUsers.queryInfoByUserId( userId, CUserInfoElement::UserClassID ).toString();
    QString tmpClassName = m_classInfoMap.value( tmpClassId ).toString();
    return tmpClassName;
}

void CInterAction::bindUserWithCardNumber(QString userId, QString cardNumber)
{
    m_curUserId = userId;
    m_curCardNumber = cardNumber;
    emit sigBindUserWithCardNumber( userId, cardNumber );   //onBindUser
}

void CInterAction::registeInteraction()
{
    m_pViewEngine->rootContext()->setContextProperty( "CInterAction", this );
    m_pViewEngine->rootContext()->setContextProperty( "FilterModel", &m_filterModel );
}

void CInterAction::setClassInfoMap(const QVariantMap &classInfoMap)
{
    m_classInfoMap = classInfoMap;
}

void CInterAction::appendNewBindedUsers()
{
    QJsonObject tmpJs;
    tmpJs.insert( "className", getClassNameByUserId( m_curUserId) );
    tmpJs.insert( "userId", m_curUserId );
    tmpJs.insert( "cardNumber", m_curCardNumber );
    tmpJs.insert( "userName",  m_totalUsers.queryInfoByUserId( m_curUserId, CUserInfoElement::UserName ).toString() );


    emit sigAppendBindedUserInfo( tmpJs );
}
