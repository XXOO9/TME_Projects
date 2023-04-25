#include "CUserInfos.h"

CUserInfos::CUserInfos(QObject *parent)
    : QAbstractListModel(parent)
{
}

int CUserInfos::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_vecTotalUsers.size();
}

QVariant CUserInfos::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int targetRowIndex = index.row();

    switch (role) {

    case CUserInfoElement::InfoElement::UserName:
        return m_vecTotalUsers.at( targetRowIndex ).m_userName;

    case CUserInfoElement::InfoElement::UserUniqueNumber:
        return m_vecTotalUsers.at( targetRowIndex ).m_userUniqueNumber;

    case CUserInfoElement::InfoElement::UserClassID:
        return m_vecTotalUsers.at( targetRowIndex ).m_userClassID;

    case CUserInfoElement::InfoElement::UserBindCardNumber:
        return m_vecTotalUsers.at( targetRowIndex ).m_userBindCardNumber;

    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> CUserInfos::roleNames() const
{
    QHash<int,QByteArray> roleNames;

    roleNames[ CUserInfoElement::InfoElement::UserName ]           = "userName";
    roleNames[ CUserInfoElement::InfoElement::UserUniqueNumber ]   = "userUniqueNum";
    roleNames[ CUserInfoElement::InfoElement::UserClassID ]        = "userClassID";
    roleNames[ CUserInfoElement::InfoElement::UserBindCardNumber ] = "userBindCardNum";

    return roleNames;
}

void CUserInfos::appendNewUsers(const QVector<CUserInfoElement> &srcUserInfos)
{
    for( auto &ele : srcUserInfos ){
        m_vecTotalUsers << ele;
    }
}
