#include "CUserFilterModel.h"

CUserFilterModel::CUserFilterModel()
{

}

CUserFilterModel::~CUserFilterModel()
{

}

bool CUserFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QString userName = sourceModel()->index( source_row, 0, source_parent )
            .data( CUserInfoElement::InfoElement::UserName ).toString();

    QString userUniqueNumber = sourceModel()->index( source_row, 0, source_parent )
            .data( CUserInfoElement::InfoElement::UserUniqueNumber ).toString();

    if( userName.contains( filterRegExp() ) || userUniqueNumber.contains( filterRegExp() ) ){
        return true;
    }

    return false;
}

QString CUserFilterModel::filterPattern() const
{
    return m_filterPattern;
}

void CUserFilterModel::setFilterPattern(const QString &filterPattern)
{
    m_filterPattern = filterPattern;
}
