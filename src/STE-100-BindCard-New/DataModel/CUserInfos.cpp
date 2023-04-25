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

QModelIndexList CUserInfos::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
{
    QModelIndexList targetModelIndexList;
    QModelIndex tmpIndex;

    int size = rowCount();
    for( int i = start.row(); i < size; i++ ){
        tmpIndex = this->index( i, 0 );
        if( value == data( tmpIndex, role ) ){
            targetModelIndexList << tmpIndex;

            if( targetModelIndexList.size() >= hits ){
                break;
            }
        }
    }

    return targetModelIndexList;
}

void CUserInfos::appendNewUser(const CUserInfoElement &srcUserInfo)
{
    m_vecTotalUsers.append( srcUserInfo );
}

QVariant CUserInfos::queryInfoByUserId(const QString &userId, const int &role)
{
    QModelIndexList tmpList = match( this->index( 0, 0 ), CUserInfoElement::UserUniqueNumber, userId );

    QVariant ret = this->data( tmpList.first(), role );

    return ret;
}
