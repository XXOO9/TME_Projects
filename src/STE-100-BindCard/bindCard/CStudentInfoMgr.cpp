#include "CStudentInfoMgr.h"

CStudentInfoMgr::CStudentInfoMgr(QObject *parent) : QObject(parent)
{

}

QString CStudentInfoMgr::getStuId() const
{
    return m_stuId;
}

void CStudentInfoMgr::setStuId(const QString &value)
{
    m_stuId = value;
}

QString CStudentInfoMgr::getName() const
{
    return m_name;
}

void CStudentInfoMgr::setName(const QString &value)
{
    m_name = value;
}

QString CStudentInfoMgr::getClassName() const
{
    return m_className;
}

void CStudentInfoMgr::setClassName(const QString &value)
{
    m_className = value;
}

QString CStudentInfoMgr::toJsonString()
{
    QJsonObject jsObj;
    jsObj.insert( SyncUserInfoKeys::keyName, m_name );
    jsObj.insert( SyncUserInfoKeys::keyClassName, m_className );
    jsObj.insert( SyncUserInfoKeys::keyId, m_stuId );
    jsObj.insert( SyncUserInfoKeys::keyGender, m_gender );
    jsObj.insert( SyncUserInfoKeys::keyAvatarUrl, m_picUrl );
    jsObj.insert( SyncUserInfoKeys::keyCardNo, m_cardNum );

    QJsonDocument jsDoc = QJsonDocument(jsObj);
    QString jsonString = QString::fromUtf8( jsDoc.toJson(QJsonDocument::Compact) );
    return jsonString;
}

QString CStudentInfoMgr::getPicUrl() const
{
    return m_picUrl;
}

void CStudentInfoMgr::setPicUrl(const QString &picUrl)
{
    m_picUrl = picUrl;
}

QString CStudentInfoMgr::getCardNum() const
{
    return m_cardNum;
}

void CStudentInfoMgr::setCardNum(const QString &cardNum)
{
    m_cardNum = cardNum;
}

QString CStudentInfoMgr::getGender() const
{
    return m_gender;
}

void CStudentInfoMgr::setGender(const QString &gender)
{
    m_gender = gender;
}
