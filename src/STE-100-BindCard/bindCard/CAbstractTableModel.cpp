#include "CAbstractTableModel.h"

CAbstractTableModel::CAbstractTableModel(qint8 columnCount, QObject *parent) : CDataModel (parent)
{
    m_columnCount = columnCount;
    m_horizontalHeaderList << QString::fromLocal8Bit( "第一列" );
    m_horizontalHeaderList << QString::fromLocal8Bit( "第二列" );
    m_horizontalHeaderList << QString::fromLocal8Bit( "第三列" );
    m_horizontalHeaderList << QString::fromLocal8Bit( "第四列" );
    m_horizontalHeaderList << QString::fromLocal8Bit( "第五列" );
}

CAbstractTableModel::~CAbstractTableModel()
{

}

QVariant CAbstractTableModel::horizontalHeader(int columnIndex)
{
    if( m_horizontalHeaderList.isEmpty() ){
        qDebug() << "empty columnindex...";
        return QVariant("empty");
    }
    ColumnIndex targetColumn = ColumnIndex( columnIndex );
    if( targetColumn > m_horizontalHeaderList.size() ){
        return QVariant();
    }
    return m_horizontalHeaderList[ targetColumn ];
}

void CAbstractTableModel::setHorizontalHeader(QStringList headers)
{
    m_horizontalHeaderList = headers;
    emit layoutChanged();
}

void CAbstractTableModel::setHorizontalHeader(QString val)
{
    QStringList header = val.split( m_splitFlag );
    setHorizontalHeader( header );
}

int CAbstractTableModel::horizontalHeaderCount() const
{
    return m_columnCount;
}

bool CAbstractTableModel::appendRowDatas(QStringList rowDisplayDatas, QString uniqueString)
{
    if( !isSingleton( uniqueString ) ){
        return false;
    }

    QHash<ColumnIndex, CUnitData> rowData;
    for( int i = 0; i < m_columnCount; i++ ){
        CUnitData unitData;
        ColumnIndex column = ColumnIndex( i );
        unitData.setUnique( uniqueString );

        //如果传进来的QStringList的size小于表格的列数，则超出部分的DisplayRole全部显示为空
        if( i > rowDisplayDatas.size() - 1 ){
            unitData.setDisplayContent( "" );
        }else{
            QString displayContent = rowDisplayDatas[i];
            unitData.setDisplayContent( displayContent );
        }
        rowData.insert( column, unitData );
    }

    bool ok = appendRow( rowData );
    emit layoutChanged();
//    qDebug() << "is append row success ? ->" << ok;
    return ok;
}

bool CAbstractTableModel::appendRowDatas(QHash<CDataModel::ColumnIndex, CUnitData> rowMap)
{
    if( rowMap[FirstColumn][UniqueRole].isEmpty() ){
        return false;
    }

    if( !isSingleton( rowMap[FirstColumn][UniqueRole] ) ){
        return false;
    }

    bool ok = appendRow( rowMap );
    emit layoutChanged();
    return ok;
}

bool CAbstractTableModel::removeRow(QString uniqueingString)
{
    return true;
}


bool CAbstractTableModel::setSpecificData(QString uniqueString, CDataModel::ColumnIndex targetColumn, QString val)
{
    bool ok = setSingleData( uniqueString, targetColumn, val );
    return ok;
}

bool CAbstractTableModel::check(QString uniqueString)
{
    bool ok = setSingleData( uniqueString, FirstColumn, "true", RoleNames::DataRole );
    return ok;
}

void CAbstractTableModel::testfunc1()
{
    QStringList rowData;
    rowData << "first" << "second" << "third" << "forth" << "fifth";
    QString uniqueString = "339";

    /*bool ok = */appendRowDatas( rowData, uniqueString );
    emit layoutChanged();
}

void CAbstractTableModel::testfunc2()
{
    QStringList rowData;
    rowData << "Class1" << "Class2" << "Class3" << "Class4" << "Class5";
    QString uniqueString = "340";

    /*bool ok = */appendRowDatas( rowData, uniqueString );
    emit layoutChanged();
}

void CAbstractTableModel::testfunc3()
{
    QStringList rowData;
    rowData << "first" << "second" << "third" << "forth" << "fifth";
    QString uniqueString = "341";

    /*bool ok = */appendRowDatas( rowData, uniqueString );
    emit layoutChanged();
}

void CAbstractTableModel::testfunc4()
{
    QStringList rowData;
    rowData << "first" << "second" << "third" << "forth" << "fifth";
    QString uniqueString = "342";

    /*bool ok = */appendRowDatas( rowData, uniqueString );
    emit layoutChanged();
}

void CAbstractTableModel::testfunc5()
{
    QStringList rowData;
    rowData << "first" << "second" << "third" << "forth" << "fifth";
    QString uniqueString = "343";

    /*bool ok = */appendRowDatas( rowData, uniqueString );
    emit layoutChanged();
}

void CAbstractTableModel::testfunc6()
{
    QStringList l;
    l << "W" << "D" << "N" << "M" << "D";
    setHorizontalHeader( l );
}
