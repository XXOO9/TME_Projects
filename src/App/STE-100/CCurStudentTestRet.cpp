#include "CCurStudentTestRet.h"

CCurStudentTestRet::CCurStudentTestRet(QObject *parent)
    : QAbstractTableModel(parent)
{
}

//QVariant CCurStudentTestRet::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    // FIXME: Implement me!
//}

int CCurStudentTestRet::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_arrayCurTestData.size();
}

int CCurStudentTestRet::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 3;
}

QHash<int, QByteArray> CCurStudentTestRet::roleNames() const
{
    QHash< int, QByteArray > roles;
    roles[ 0 ] = "id";
    roles[ 1 ] = "order";
    roles[ 2 ] = "ret";
    roles[ 3 ] = "score";
    roles[ 4 ] = "check";
    return roles;
}

QVariant CCurStudentTestRet::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return m_arrayCurTestData[ index.row() ][ role ];
}

void CCurStudentTestRet::addItem( QString name, QString trackNumber, QString ret, QString score )
{
    beginInsertRows( QModelIndex(), m_arrayCurTestData.size(), m_arrayCurTestData.size() );
    QVector< QString > list;
    list << name << trackNumber << ret << score << "" << "";
    m_arrayCurTestData << list;
    endInsertRows();
}

void CCurStudentTestRet::addItem( structDataModel dataModel )
{
    //    for( auto eachUser : m_arrayCurTestData ){
    //        if( eachUser[0] == dataModel.index ){
    //            return;
    //        }
    //    }

    QString id =  QString::fromLocal8Bit( dataModel.name.toLocal8Bit() );
    QString trackNumber = QString::fromLocal8Bit( dataModel.trackNumber.toLocal8Bit() );
    QString order = QString::fromLocal8Bit( dataModel.ret.toLocal8Bit() );
    QString score = QString::fromLocal8Bit( dataModel.score.toLocal8Bit() );
    QString index = QString::fromLocal8Bit( dataModel.index.toLocal8Bit() );
    QString isCheck = "uncheck";

    beginInsertRows( QModelIndex(), m_arrayCurTestData.size(), m_arrayCurTestData.size() );
    QVector< QString > list;
    list << id << trackNumber << order << score << isCheck << index;
    m_arrayCurTestData << list;
    endInsertRows();
}

void CCurStudentTestRet::addRopeSkip(structRopeSkip dataModel)
{
    QString id = QString::fromLocal8Bit( dataModel.name.toLocal8Bit() );
    QString battery = QString::fromLocal8Bit( dataModel.battery.toLocal8Bit() );
    QString order = QString::fromLocal8Bit( dataModel.ret.toLocal8Bit() );
    QString score = QString::fromLocal8Bit( dataModel.score.toLocal8Bit() );
    QString index = QString::fromLocal8Bit( dataModel.index.toLocal8Bit() );
    QString isCheck = "unCheck";

    beginInsertRows( QModelIndex(), m_arrayCurTestData.size(), m_arrayCurTestData.size() );
    QVector< QString > list;
    list << id << battery << order << score << isCheck << index;
    m_arrayCurTestData << list;
    endInsertRows();
}

void CCurStudentTestRet::deleteItem(QString index)
{
    for(int i = 0; i < m_arrayCurTestData.size() ; i++)
    {
        if( m_arrayCurTestData[i].at( DATATYPE::Type_Index ) == index )
        {
            m_arrayCurTestData.removeAt(i);
        }
    }
    emit layoutChanged();
}

void CCurStudentTestRet::updateItem( QString id, QString name, QString aws,QString score,QString number )
{
    for(int i = 0; i < m_arrayCurTestData.size() ; i++)
    {
        if( m_arrayCurTestData[i].first() == id )
        {
            QVector< QString > list;
            list << id <<name << aws << score<< number;
            m_arrayCurTestData.replace(i, list);
        }
    }
    emit layoutChanged();
}

void CCurStudentTestRet::cleanAll()
{
    m_arrayCurTestData.clear();
    emit layoutChanged();
}

void CCurStudentTestRet::check( QString index )
{
    for(int i = 0; i < m_arrayCurTestData.size() ; i++)
    {
        if( m_arrayCurTestData[ i ].at( DATATYPE::Type_Index ) == index )
        {
            QVector< QString > list = m_arrayCurTestData[i];
            list.replace( DATATYPE::Type_Check, "check" );
            m_arrayCurTestData.replace( i, list );
        }
    }
    emit layoutChanged();
}

void CCurStudentTestRet::setSingleData( QString index, CCurStudentTestRet::DATATYPE dataType, QString data )
{
    for( int i = 0; i < m_arrayCurTestData.size(); i++ ){
        if( m_arrayCurTestData[ i ].at( DATATYPE::Type_Index ) == index ){
            QVector< QString > list = m_arrayCurTestData[ i ];
            list.replace( dataType, data );
            m_arrayCurTestData.replace( i, list );
        }
    }
    emit layoutChanged();
}

void CCurStudentTestRet::setRopeSkinSingleData(QString index, CCurStudentTestRet::DATATYPE_RopeSkip dataType, QString data)
{
    for( int i = 0; i < m_arrayCurTestData.size(); i++ ){
        if( m_arrayCurTestData[ i ].at( DATATYPE_RopeSkip::RopeSkip_Index ) == index ){
            QVector< QString > list = m_arrayCurTestData[ i ];
            list.replace( dataType, data );
            m_arrayCurTestData.replace( i, list );
        }
    }
    emit layoutChanged();
}


void CCurStudentTestRet::setSocre( )
{
    setSingleData( "1", DATATYPE::Type_Score, "setScore" );
}
