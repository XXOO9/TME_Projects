#include "CCommonDataModel.h"

CCommonDataModel::CCommonDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void CCommonDataModel::addRow(QStringList data, bool isOrdered )
{
    if( isOrdered ){
        beginInsertRows( QModelIndex(), m_arryData.size(), m_arryData.size() );
    }else{
        beginInsertRows( QModelIndex(), 0, 1 );
    }


    int fillCount = m_maxColumnCount - data.size();
    if(fillCount < 0)
    {
        //只取前8位
        QStringList list;
        for(int i=0;i<m_maxColumnCount;i++)
        {
            list<<data[i];
        }
        m_arryData << list;
    }
    else
    {
        //不足则补空
        for(int i=0;i<fillCount;i++)
        {
            data<<"";
        }
        if( isOrdered ){
            m_arryData << data;
        }else{
            m_arryData.push_front( data );
        }

    }

    endInsertRows();
}

void CCommonDataModel::updateRowData(int cloumnIndex, QString columnData, QStringList data)
{
    int iRowCount, j, iMinSize;
    int iDataSize = data.size();
    for(int i = 0; i < m_arryData.size() ; i++)
    {
        //越界保护
        iRowCount = m_arryData[i].count();
        if( cloumnIndex >= iRowCount)
        {
            return;
        }

        if( m_arryData[i].at(cloumnIndex) == columnData )
        {
            iMinSize = iRowCount > iDataSize ? iDataSize : iRowCount;

            for(j = 0; j < iMinSize; j++)
            {
                m_arryData[i][j] = data[j];
            }

            //长度不足填空字符
            if(j < iRowCount)
            {
                for(; j < iRowCount; j++)
                {
                    m_arryData[i][j] = "";
                }
            }

            //只变更第一次遇到的
            break;
        }
    }
    emit layoutChanged();
}

void CCommonDataModel::updateCellData(int cloumnIndex, QString columnData, int updateCloumnIndex, QString data)
{
    for(int i = 0; i < m_arryData.size() ; i++)
    {
        //越界保护
        if( cloumnIndex >= m_arryData[i].count())
        {
            return;
        }

        if( m_arryData[i].at(cloumnIndex) == columnData )
        {
            if(updateCloumnIndex < m_arryData[i].count())
            {
                m_arryData[i].replace(updateCloumnIndex, data);
            }

            //只变更第一次遇到的
            break;
        }
    }
    emit layoutChanged();
}

void CCommonDataModel::deleteRow(int cloumnIndex, QString columnData)
{
    for(int i = 0; i < m_arryData.size() ; i++)
    {
        //越界保护
        if( cloumnIndex >= m_arryData[i].count())
        {
            return;
        }

        if( m_arryData[i].at(cloumnIndex) == columnData )
        {
            m_arryData.removeAt(i);

            //每次只删除一个
            break;
        }
    }
    emit layoutChanged();
}

void CCommonDataModel::deleteAll()
{
    m_arryData.clear();
    emit layoutChanged();
}

bool CCommonDataModel::checkCellDataExist(const int &cloumnIndex, const QString &columnData, int limitRowIndex)
{
    //取在范围中的值进行对比
    int nAllRowIndex = m_arryData.size() - 1;
    int nRowIndex = limitRowIndex;
    if(nRowIndex < 0){
        nRowIndex = nAllRowIndex;
    }
    else{
        if(nRowIndex > nAllRowIndex)
            nRowIndex = nAllRowIndex;
    }

    bool bRet = false;
    for(int i = 0; i <= nRowIndex; i++)
    {
        auto &rowData = m_arryData[i];
        if( cloumnIndex >= rowData.count())
            break;

        if( rowData.at(cloumnIndex) == columnData )
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

QString CCommonDataModel::getCellData(int col, int row)
{
    QString qsTemp("");
    if(row < m_arryData.size())
    {
        auto &rowData = m_arryData[row];
        if(col < rowData.count())
        {
            qsTemp = rowData[col];
        }
    }

    return qsTemp;
}

int CCommonDataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_maxColumnCount;
}

int CCommonDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_arryData.size();
}

QVariant CCommonDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return m_arryData[ index.row() ][ role ];
}

QHash<int, QByteArray> CCommonDataModel::roleNames() const
{
    QHash< int, QByteArray > roles;
    roles[ 0 ] = "first";
    roles[ 1 ] = "second";
    roles[ 2 ] = "third";
    roles[ 3 ] = "forth";
    roles[ 4 ] = "fifth";
    roles[ 5 ] = "sixth";
    roles[ 6 ] = "seventh";
    roles[ 7 ] = "eighth";
    return roles;
}
