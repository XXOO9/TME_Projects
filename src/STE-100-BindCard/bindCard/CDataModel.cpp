#include "CDataModel.h"

CDataModel::CDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

QVariant CDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant();
}

int CDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_vecRets.size();
}

int CDataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    //!!!!!!!!!!!!不加这个的话,在表格为空时要报错！
    if( m_vecRets.isEmpty() ){
        return 0;
    }
    return m_vecRets[0].size();
}

QVariant CDataModel::data(const QModelIndex &index, int role) const
{
    // FIXME: Implement me!
    int row = index.row();
    int column = index.column();

    if( row < 0 || row > m_vecRets.size() ){
        return "";
    }
    ColumnIndex columnEnum = ColumnIndex( column );
    RoleNames roleEnum = RoleNames( role );
    CUnitData data = m_vecRets[row][columnEnum];
    QString ret = data[roleEnum];
//    qDebug() << "row = " << row;
//    qDebug() << "column = " << column;
//    qDebug() << "data = " << ret;
    return ret;
}

bool CDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
    return true;
}

bool CDataModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool CDataModel::appendRow(QHash<ColumnIndex, CUnitData> data)
{
    if( !isSingleton( data[FirstColumn][UniqueRole] ) ){
        return false;
    }

    beginInsertRows( QModelIndex(), m_vecRets.size(), m_vecRets.size() );
    m_vecRets << data;
    endInsertRows();
    emit layoutChanged();
    return true;
}

bool CDataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool CDataModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}

QHash<int, QByteArray> CDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FirstColumn]      =    "firstColumn";
    roles[SecondColumn]     =    "secondColumn";
    roles[ThirdColumn]      =    "thirdColumn";
    roles[ForthColumn]      =    "forthColumn";
    roles[FifthColumn]      =    "fifthColumn";
    roles[SixthColumn]      =    "sixthColumn";
    roles[SevengthColumn]   =    "sevengthColumn";
    roles[EightColumn]      =    "eightColumn";

    return roles;
}

bool CDataModel::setSingleData(const QString uniqeuString, const ColumnIndex column, const QString val, const RoleNames roleName )
{
    bool find = false;
    for( auto &each : m_vecRets ){
        if( uniqeuString == each[FirstColumn][UniqueRole] ){
            each[column][roleName] = val;
            find = true;
            emit layoutChanged();
            break;
        }
    }

    if( !find ){
        qDebug() << "empty uniquesString...";
    }
    return find;
}

void CDataModel::cleanAll()
{
    m_vecRets.clear();
    emit layoutChanged();
}

bool CDataModel::isSingleton(QString uniqueString)
{
    for( auto &each : m_vecRets ){
        if( uniqueString == each[FirstColumn][UniqueRole] ){
            return false;
        }
    }
    return true;
}

CUnitData::CUnitData()
{
    m_data.insert( CDataModel::DisplayRole, "" );
    m_data.insert( CDataModel::UniqueRole, "" );
    m_data.insert( CDataModel::DataRole, "false" );
}

CUnitData::~CUnitData()
{

}

void CUnitData::setDisplayContent(const QString val)
{
    m_data.insert( CDataModel::DisplayRole, val );
}

QString CUnitData::displayContent() const
{
    return m_data[CDataModel::DisplayRole];

}

void CUnitData::setUnique(const QString val)
{
    m_data.insert( CDataModel::UniqueRole, val );
}

QString CUnitData::unique() const
{
    return m_data[CDataModel::UniqueRole];
}

void CUnitData::setDataContent(const QString val)
{
    m_data.insert( CDataModel::DataRole, val );
}

QString CUnitData::data() const
{
    return m_data[CDataModel::DataRole];
}

QString &CUnitData::operator[](CDataModel::RoleNames roleName)
{
    return m_data[roleName];
}
