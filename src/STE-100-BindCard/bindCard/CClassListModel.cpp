#include "CClassListModel.h"

CClassListModel::CClassListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int CClassListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    int count = m_vecClassList.size();
    return count;
}

QVariant CClassListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    int rowIndex = index.row();

    return m_vecClassList[ rowIndex ].className();
}

QVariant CClassListModel::data(int rowIndex)
{
    return m_vecClassList[ rowIndex ].className();
}

bool CClassListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool CClassListModel::appendRows(CClassInfoItem newClassInfo)
{
    beginInsertRows( QModelIndex(), m_vecClassList.size(), m_vecClassList.size() );
    m_vecClassList << newClassInfo;
    endInsertRows();
    emit layoutChanged();
    return true;
}

bool CClassListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

CClassInfoItem::CClassInfoItem()
{

}

CClassInfoItem::~CClassInfoItem()
{

}

QString CClassInfoItem::classId() const
{
    return m_classId;
}

void CClassInfoItem::setClassId(const QString &classId)
{
    m_classId = classId;
}

QString CClassInfoItem::className() const
{
    return m_className;
}

void CClassInfoItem::setClassName(const QString &className)
{
    m_className = className;
}
