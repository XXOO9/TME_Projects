#ifndef COMMONDATAMODEL_H
#define COMMONDATAMODEL_H
#include <QAbstractTableModel>
#include <QDebug>
#include <QVector>
#include <QHash>
#include <QString>
#include <stdarg.h>
using namespace std;
class CCommonDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ColumnIndex{
        FirstColumn = 0,
        SecondColumn,
        ThirdColumn,
        ForthColumn,
        FifthColumn,
        SixthColumn,
        SevengthColumn,
        EightColumn
    };
    Q_ENUM( ColumnIndex )

    explicit CCommonDataModel(QObject *parent = nullptr);
    void addRow(QStringList data, bool isOrdered = true);
    void updateRowData(int cloumnIndex, QString columnData, QStringList data);
    void updateCellData(int cloumnIndex, QString columnData, int updateCloumnIndex, QString data);
    void deleteRow(int cloumnIndex, QString columnData);
    void deleteAll();
    bool checkCellDataExist(const int &cloumnIndex,  const QString &columnData, int limitRowIndex = -1);
    QString getCellData(int col, int row);
    int getRows() {return m_arryData.size();}

public:
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    int m_maxColumnCount{8};
    QVector< QStringList > m_arryData;
};

#endif // COMMONDATAMODEL_H
